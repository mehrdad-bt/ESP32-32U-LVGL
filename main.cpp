#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include "esp_system.h"
#include "esp_task_wdt.h"

extern "C"
{
#include "ui/ui.h"
#include "ui/screens.h"
}

#include "uart.h"
#include "tasks.h"
#include "screen_manager.h"

// ==================================================
// Watchdog Configuration
// ==================================================

#define WDT_TIMEOUT_SECONDS 4

// ==================================================
// Display Configuration
// ==================================================

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

// --------------------------------------------------
// LVGL Draw Buffer
// RGB565 = 2 bytes per pixel
// 320 x 20 x 2 = 12800 bytes
// --------------------------------------------------

#define LVGL_BUF_LINES 20

TFT_eSPI tft = TFT_eSPI();

static lv_disp_draw_buf_t draw_buf;

static lv_color_t buf1[
    SCREEN_WIDTH * LVGL_BUF_LINES
];

// ==================================================
// Touch Calibration
// ==================================================

static uint16_t calData[5] =
{
    351,
    3465,
    306,
    3446,
    7
};

// ==================================================
// Reset Reason
// ==================================================

static const char *reset_reason_name(
    esp_reset_reason_t reason
)
{
    switch (reason)
    {
        case ESP_RST_UNKNOWN:
            return "UNKNOWN";

        case ESP_RST_POWERON:
            return "POWER_ON";

        case ESP_RST_EXT:
            return "EXTERNAL";

        case ESP_RST_SW:
            return "SOFTWARE";

        case ESP_RST_PANIC:
            return "PANIC_EXCEPTION";

        case ESP_RST_INT_WDT:
            return "INTERRUPT_WDT";

        case ESP_RST_TASK_WDT:
            return "TASK_WDT";

        case ESP_RST_WDT:
            return "OTHER_WDT";

        case ESP_RST_DEEPSLEEP:
            return "DEEP_SLEEP";

        case ESP_RST_BROWNOUT:
            return "BROWNOUT";

        case ESP_RST_SDIO:
            return "SDIO";

        default:
            return "OTHER";
    }
}

static void print_reset_reason(void)
{
    esp_reset_reason_t reason =
        esp_reset_reason();

    Serial.print("Reset reason: ");
    Serial.print((int)reason);
    Serial.print(" - ");
    Serial.println(reset_reason_name(reason));
}

// ==================================================
// Watchdog
// ==================================================

static void watchdog_init(void)
{
    Serial.println("Initializing task watchdog...");

    // --------------------------------------------------
    // Arduino-ESP32 2.x already initializes TWDT in some
    // board/core configurations. If it is not initialized,
    // initialize it here with a 4 second timeout.
    // --------------------------------------------------

    esp_err_t result =
        esp_task_wdt_init(
            WDT_TIMEOUT_SECONDS,
            true
        );

    if (
        result != ESP_OK &&
        result != ESP_ERR_INVALID_STATE
    )
    {
        Serial.print("WDT init error: ");
        Serial.println((int)result);
    }

    // --------------------------------------------------
    // Register the current Arduino loop task.
    // NULL means the current task.
    // --------------------------------------------------

    esp_err_t status =
        esp_task_wdt_status(NULL);

    if (status == ESP_ERR_NOT_FOUND)
    {
        result =
            esp_task_wdt_add(NULL);

        if (result != ESP_OK)
        {
            Serial.print("WDT add error: ");
            Serial.println((int)result);
        }
    }

    Serial.print("Task watchdog: ENABLED, timeout = ");
    Serial.print(WDT_TIMEOUT_SECONDS);
    Serial.println(" seconds");
}

static inline void watchdog_feed(void)
{
    esp_task_wdt_reset();
}

// ==================================================
// LVGL Display Flush
// ==================================================

static void my_disp_flush(
    lv_disp_drv_t *disp,
    const lv_area_t *area,
    lv_color_t *color_p
)
{
    uint32_t w =
        area->x2 - area->x1 + 1;

    uint32_t h =
        area->y2 - area->y1 + 1;

    tft.startWrite();

    tft.setAddrWindow(
        area->x1,
        area->y1,
        w,
        h
    );

    tft.pushColors(
        (uint16_t *)&color_p->full,
        w * h,
        true
    );

    tft.endWrite();

    lv_disp_flush_ready(disp);
}

// ==================================================
// LVGL Touch Input
// ==================================================

static void my_touchpad_read(
    lv_indev_drv_t *indev_drv,
    lv_indev_data_t *data
)
{
    uint16_t x;
    uint16_t y;

    (void)indev_drv;

    if (tft.getTouch(&x, &y))
    {
        data->state =
            LV_INDEV_STATE_PR;

        data->point.x =
            x;

        data->point.y =
            y;
    }
    else
    {
        data->state =
            LV_INDEV_STATE_REL;
    }
}

// ==================================================
// Setup
// ==================================================

void setup()
{
    // --------------------------------------------------
    // Diagnostic Serial
    // --------------------------------------------------

    Serial.begin(115200);

    delay(100);

    Serial.println();
    Serial.println("================================");
    Serial.println("ESP32 LVGL Application Starting");
    Serial.println("================================");

    print_reset_reason();

    // --------------------------------------------------
    // Project UART
    // --------------------------------------------------

    serial_init();

    // --------------------------------------------------
    // Enable task watchdog
    // --------------------------------------------------

    watchdog_init();

    // --------------------------------------------------
    // Initialize TFT
    // --------------------------------------------------

    Serial.println("Initializing TFT...");

    tft.begin();

    // Landscape
    // 320 x 240

    tft.setRotation(1);

    // --------------------------------------------------
    // Initialize Touch
    // --------------------------------------------------

    Serial.println("Initializing Touch...");

    tft.setTouch(calData);

    // --------------------------------------------------
    // Initialize LVGL
    // --------------------------------------------------

    Serial.println("Initializing LVGL...");

    lv_init();

    // ==================================================
    // LVGL Draw Buffer
    // ==================================================

    lv_disp_draw_buf_init(
        &draw_buf,
        buf1,
        NULL,
        SCREEN_WIDTH * LVGL_BUF_LINES
    );

    // ==================================================
    // Register LVGL Display Driver
    // ==================================================

    static lv_disp_drv_t disp_drv;

    lv_disp_drv_init(&disp_drv);

    disp_drv.hor_res =
        SCREEN_WIDTH;

    disp_drv.ver_res =
        SCREEN_HEIGHT;

    disp_drv.flush_cb =
        my_disp_flush;

    disp_drv.draw_buf =
        &draw_buf;

    lv_disp_drv_register(
        &disp_drv
    );

    // ==================================================
    // Register LVGL Touch Driver
    // ==================================================

    static lv_indev_drv_t indev_drv;

    lv_indev_drv_init(
        &indev_drv
    );

    indev_drv.type =
        LV_INDEV_TYPE_POINTER;

    indev_drv.read_cb =
        my_touchpad_read;

    lv_indev_drv_register(
        &indev_drv
    );

    // ==================================================
    // Initialize EEZ Studio UI
    // ==================================================

    Serial.println("Initializing EEZ UI...");

    ui_init();

    // ==================================================
    // LED Diagnostic Style
    // ==================================================

    if (objects.obj0 != NULL)
    {
        lv_obj_set_style_radius(
            objects.obj0,
            0,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

        lv_obj_set_style_shadow_width(
            objects.obj0,
            0,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

        lv_obj_set_style_shadow_spread(
            objects.obj0,
            0,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

        lv_obj_set_style_border_width(
            objects.obj0,
            0,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

        lv_obj_set_style_bg_opa(
            objects.obj0,
            LV_OPA_COVER,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );
    }

    // ==================================================
    // Initialize Screen Manager
    // ==================================================

    Serial.println("Initializing Screen Manager...");

    screen_manager_init();

    // ==================================================
    // Initialize Application Tasks
    // ==================================================

    Serial.println("Initializing Tasks...");

    tasks_init();

    Serial.println("System initialization complete.");
    Serial.println("================================");

    // --------------------------------------------------
    // First watchdog feed after initialization
    // --------------------------------------------------

    watchdog_feed();
}

// ==================================================
// Main Loop
// ==================================================

void loop()
{
    static uint32_t max_loop_time_us = 0;
    static uint32_t last_report = 0;

    uint32_t loop_start =
        micros();

    // --------------------------------------------------
    // Feed watchdog before application processing
    // --------------------------------------------------

    watchdog_feed();

    // --------------------------------------------------
    // Run application tasks
    // --------------------------------------------------

    tasks_run();

    // --------------------------------------------------
    // Process LVGL
    // --------------------------------------------------

    lv_timer_handler();

    // --------------------------------------------------
    // Measure loop duration
    // --------------------------------------------------

    uint32_t elapsed =
        micros() - loop_start;

    if (elapsed > max_loop_time_us)
    {
        max_loop_time_us =
            elapsed;
    }

    // --------------------------------------------------
    // Report unusually long loops
    // --------------------------------------------------

    if (elapsed > 250000U)
    {
        Serial.print("WARNING: loop blocked for ");
        Serial.print(elapsed);
        Serial.println(" us");
    }

    // --------------------------------------------------
    // Periodic diagnostic report
    // --------------------------------------------------

    uint32_t now =
        millis();

    if (now - last_report >= 5000U)
    {
        last_report = now;

        Serial.print("Alive - max loop time: ");
        Serial.print(max_loop_time_us);
        Serial.println(" us");

        max_loop_time_us = 0;
    }

    // --------------------------------------------------
    // Feed watchdog after application processing
    // --------------------------------------------------

    watchdog_feed();

    // --------------------------------------------------
    // Give ESP32 background tasks CPU time
    // --------------------------------------------------

    yield();
}
