#include <Arduino.h>
#include <lvgl.h>
#include <stdio.h>

#include "MainPage.h"
#include "tasks.h"
#include "screen_manager.h"
#include "VCRangePage.h"
#include "font_persian_24.h"
#include "font_persian_16.h"

extern "C"
{
#include "ui/ui.h"
#include "ui/screens.h"
}

// ==================================================
// Configuration
// ==================================================

#define LED_BLUE    0x0000FF
#define LED_GREEN   0x00FF00
#define LED_RED     0xFF0000
#define LED_ORANGE  0xFFA500

#define ERROR_TEXT_BLINK_MS 500

// ==================================================
// Error Types
// ==================================================

enum ErrorType
{
    ERROR_NONE = 0,
    ERROR_CONNECTION,
    ERROR_VOLTAGE_LOW,
    ERROR_VOLTAGE_HIGH,
    ERROR_CURRENT_LOW,
    ERROR_CURRENT_HIGH
};

// ==================================================
// Main GUI Cache
// ==================================================

static float gui_last_voltage = -1000.0f;
static float gui_last_current = -1000.0f;

static ErrorType gui_last_error =
    ERROR_NONE;

// ==================================================
// LED State
// ==================================================

static uint32_t led_blink_color =
    LED_BLUE;

static bool led_blink_state =
    true;

static uint32_t led_blink_timer =
    0;

static bool led_blink_enable =
    false;

static ErrorType led_last_state =
    ERROR_NONE;

// ==================================================
// Error Message Label
// ==================================================

static lv_obj_t *error_msg_label =
    NULL;

// ==================================================
// Error Text Blink
// ==================================================

static uint32_t error_text_blink_timer =
    0;

static bool error_text_blink_state =
    true;

// ==================================================
// Error Box Cache
// ==================================================

static uint32_t error_box_last_color =
    0xFFFFFFFFUL;

// ==================================================
// Helper
// ==================================================

static bool main_screen_active(void)
{
    return screen_manager_is(
        SCREEN_ID_MAIN
    );
}

// ==================================================
// Determine Error
// ==================================================

static ErrorType get_error_type(void)
{
    if (
        tasks_connection_lost()
    )
    {
        return ERROR_CONNECTION;
    }

    float voltage =
        tasks_get_voltage();

    float current =
        tasks_get_current();

    if (
        voltage <
        get_voltage_min_limit()
    )
    {
        return ERROR_VOLTAGE_LOW;
    }

    if (
        voltage >
        get_voltage_max_limit()
    )
    {
        return ERROR_VOLTAGE_HIGH;
    }

    if (
        current <
        get_current_min_limit()
    )
    {
        return ERROR_CURRENT_LOW;
    }

    if (
        current >
        get_current_max_limit()
    )
    {
        return ERROR_CURRENT_HIGH;
    }

    return ERROR_NONE;
}

// ==================================================
// Configure LED
// ==================================================

static void configure_led(void)
{
    if (objects.obj0 == NULL)
    {
        return;
    }

    lv_obj_set_style_radius(
        objects.obj0,
        LV_RADIUS_CIRCLE,
        LV_PART_MAIN |
        LV_STATE_DEFAULT
    );

    lv_obj_set_style_bg_opa(
        objects.obj0,
        LV_OPA_COVER,
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
}

// ==================================================
// Apply LED Color
// ==================================================

static void led_apply_color(
    uint32_t color
)
{
    if (!main_screen_active())
    {
        return;
    }

    if (objects.obj0 == NULL)
    {
        return;
    }

    lv_led_set_color(
        objects.obj0,
        lv_color_hex(color)
    );

    lv_led_set_brightness(
        objects.obj0,
        255
    );
}

// ==================================================
// Solid LED
// ==================================================

static void set_status_led(
    uint32_t color
)
{
    led_blink_color =
        color;

    led_blink_enable =
        false;

    led_blink_state =
        true;

    led_blink_timer =
        millis();

    led_apply_color(
        color
    );
}

// ==================================================
// Blinking LED
// ==================================================

static void set_status_led_blink(
    uint32_t color
)
{
    if (
        led_blink_enable &&
        led_blink_color == color
    )
    {
        return;
    }

    led_blink_color =
        color;

    led_blink_enable =
        true;

    led_blink_state =
        true;

    led_blink_timer =
        millis();

    led_apply_color(
        color
    );
}

// ==================================================
// LED Task
// ==================================================

static void led_task(void)
{
    if (!main_screen_active())
    {
        return;
    }

    if (objects.obj0 == NULL)
    {
        return;
    }

    if (!led_blink_enable)
    {
        return;
    }

    uint32_t now =
        millis();

    if (
        now - led_blink_timer <
        500
    )
    {
        return;
    }

    led_blink_timer =
        now;

    led_blink_state =
        !led_blink_state;

    if (led_blink_state)
    {
        led_apply_color(
            led_blink_color
        );
    }
    else
    {
        lv_led_set_brightness(
            objects.obj0,
            0
        );
    }
}

// ==================================================
// Error Message Box Initialization
// ==================================================

static void init_error_msgbox(void)
{
    error_msg_label =
        NULL;

    if (objects.error_box == NULL)
    {
        return;
    }

    lv_obj_t *content =
        lv_msgbox_get_content(
            objects.error_box
        );

    if (content == NULL)
    {
        return;
    }

    error_msg_label =
        lv_label_create(content);

    if (error_msg_label == NULL)
    {
        return;
    }

    lv_label_set_text(
        error_msg_label,
        ""
    );

    lv_label_set_long_mode(
        error_msg_label,
        LV_LABEL_LONG_WRAP
    );

    lv_obj_set_style_text_font(
        error_msg_label,
        &font_persian_24,
        LV_PART_MAIN |
        LV_STATE_DEFAULT
    );

    /*
     * این حالت را عمداً مطابق نسخه فعلی نگه می‌داریم.
     */
    lv_obj_set_style_base_dir(
        error_msg_label,
        LV_BASE_DIR_LTR,
        LV_PART_MAIN |
        LV_STATE_DEFAULT
    );

    lv_obj_set_width(
        error_msg_label,
        220
    );

    lv_obj_set_height(
        error_msg_label,
        50
    );

    lv_obj_set_pos(
        error_msg_label,
        9,
        45
    );

    lv_obj_set_style_text_align(
        error_msg_label,
        LV_TEXT_ALIGN_CENTER,
        LV_PART_MAIN |
        LV_STATE_DEFAULT
    );

    lv_obj_set_style_text_color(
        error_msg_label,
        lv_color_hex(0xFFFFFF),
        LV_PART_MAIN |
        LV_STATE_DEFAULT
    );

    lv_obj_set_y(
        error_msg_label,
        40
    );

    lv_obj_set_style_border_width(
        error_msg_label,
        0,
        LV_PART_MAIN |
        LV_STATE_DEFAULT
    );

    lv_obj_set_style_bg_opa(
        error_msg_label,
        LV_OPA_TRANSP,
        LV_PART_MAIN |
        LV_STATE_DEFAULT
    );

    lv_obj_clear_flag(
        error_msg_label,
        LV_OBJ_FLAG_HIDDEN
    );

    error_text_blink_timer =
        millis();

    error_text_blink_state =
        true;
}

// ==================================================
// Set Error Text
// ==================================================

static void set_error_text(
    const char *text
)
{
    if (text == NULL)
    {
        text = "";
    }

    if (error_msg_label != NULL)
    {
        lv_label_set_text(
            error_msg_label,
            text
        );

        lv_obj_set_style_text_font(
            error_msg_label,
            &font_persian_24,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

        lv_obj_set_style_base_dir(
            error_msg_label,
            LV_BASE_DIR_LTR,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

        lv_obj_set_style_text_align(
            error_msg_label,
            LV_TEXT_ALIGN_CENTER,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

        lv_obj_set_style_text_color(
            error_msg_label,
            lv_color_hex(0xFFFFFF),
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );
    }

    error_text_blink_timer =
        millis();

    error_text_blink_state =
        true;
}

// ==================================================
// Error Text Blink
// ==================================================

static void update_error_text_blink(void)
{
    if (!main_screen_active())
    {
        return;
    }

    if (error_msg_label == NULL)
    {
        return;
    }

    if (
        gui_last_error ==
        ERROR_NONE
    )
    {
        return;
    }

    uint32_t now =
        millis();

    if (
        now - error_text_blink_timer <
        ERROR_TEXT_BLINK_MS
    )
    {
        return;
    }

    error_text_blink_timer =
        now;

    error_text_blink_state =
        !error_text_blink_state;

    uint32_t color =
        error_text_blink_state
        ? 0x000000
        : 0xFFFFFF;

    lv_obj_set_style_text_color(
        error_msg_label,
        lv_color_hex(color),
        LV_PART_MAIN |
        LV_STATE_DEFAULT
    );
}

// ==================================================
// Update Error Box
// ==================================================

static void update_error_box(void)
{
    if (!main_screen_active())
    {
        return;
    }

    if (objects.error_box == NULL)
    {
        return;
    }

    if (error_msg_label == NULL)
    {
        return;
    }

    ErrorType error =
        get_error_type();

    // --------------------------------------------------
    // No error
    // --------------------------------------------------

    if (error == ERROR_NONE)
    {
        if (
            gui_last_error !=
            ERROR_NONE
        )
        {
            lv_obj_add_flag(
                objects.error_box,
                LV_OBJ_FLAG_HIDDEN
            );

            set_error_text("");

            gui_last_error =
                ERROR_NONE;

            error_text_blink_state =
                true;

            error_text_blink_timer =
                millis();
        }

        return;
    }

    // --------------------------------------------------
    // Same error
    // --------------------------------------------------

    if (
        error ==
        gui_last_error
    )
    {
        return;
    }

    const char *message =
        "";

    uint32_t color =
        LED_RED;

    switch (error)
    {
        case ERROR_CONNECTION:

            message =
                "اتصال قطع شد";

            color =
                LED_ORANGE;

            break;

        case ERROR_VOLTAGE_LOW:

            message =
                "ولتاژ خیلی پایین";

            color =
                LED_RED;

            break;

        case ERROR_VOLTAGE_HIGH:

            message =
                "ولتاژ خیلی بالا";

            color =
                LED_RED;

            break;

        case ERROR_CURRENT_LOW:

            message =
                "جریان خیلی پایین";

            color =
                LED_RED;

            break;

        case ERROR_CURRENT_HIGH:

            message =
                "جریان خیلی بالا";

            color =
                LED_RED;

            break;

        default:
            break;
    }

    set_error_text(
        message
    );

    lv_obj_set_style_text_color(
        error_msg_label,
        lv_color_hex(0xFFFFFF),
        LV_PART_MAIN |
        LV_STATE_DEFAULT
    );

    if (
        error_box_last_color !=
        color
    )
    {
        lv_obj_set_style_bg_color(
            objects.error_box,
            lv_color_hex(color),
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

        lv_obj_set_style_bg_opa(
            objects.error_box,
            LV_OPA_60,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

        error_box_last_color =
            color;
    }

    lv_obj_clear_flag(
        objects.error_box,
        LV_OBJ_FLAG_HIDDEN
    );

    lv_obj_clear_flag(
        error_msg_label,
        LV_OBJ_FLAG_HIDDEN
    );

    if (objects.error_label != NULL)
    {
        lv_obj_add_flag(
            objects.error_label,
            LV_OBJ_FLAG_HIDDEN
        );
    }

    error_text_blink_timer =
        millis();

    error_text_blink_state =
        true;

    gui_last_error =
        error;
}

// ==================================================
// Update Main Data
// ==================================================

static void update_main_values(void)
{
    float voltage =
        tasks_get_voltage();

    float current =
        tasks_get_current();

    if (
        objects.voltage != NULL &&
        voltage != gui_last_voltage
    )
    {
        char text[16];

        snprintf(
            text,
            sizeof(text),
            "%.2f",
            voltage
        );

        lv_label_set_text(
            objects.voltage,
            text
        );

        gui_last_voltage =
            voltage;
    }

    if (
        objects.current != NULL &&
        current != gui_last_current
    )
    {
        char text[16];

        snprintf(
            text,
            sizeof(text),
            "%.2f",
            current
        );

        lv_label_set_text(
            objects.current,
            text
        );

        gui_last_current =
            current;
    }
}

// ==================================================
// Update LED State
// ==================================================

static void update_led_state(void)
{
    if (!main_screen_active())
    {
        return;
    }

    ErrorType error =
        get_error_type();

    if (
        error ==
        led_last_state
    )
    {
        return;
    }

    led_last_state =
        error;

    if (
        error ==
        ERROR_CONNECTION
    )
    {
        set_status_led_blink(
            LED_ORANGE
        );

        return;
    }

    if (
        !tasks_data_received()
    )
    {
        set_status_led(
            LED_BLUE
        );

        return;
    }

    if (
        error ==
        ERROR_NONE
    )
    {
        set_status_led(
            LED_GREEN
        );

        return;
    }

    set_status_led_blink(
        LED_RED
    );
}

// ==================================================
// Configure Settings Button Text
// ==================================================

static void configure_settings_button_text(void)
{
    if (objects.settings_button_text == NULL)
    {
        return;
    }

    lv_label_set_text(
        objects.settings_button_text,
        "تنظیمات"
    );

    lv_obj_set_style_text_font(
        objects.settings_button_text,
        &font_persian_16,
        LV_PART_MAIN |
        LV_STATE_DEFAULT
    );

    lv_obj_set_style_base_dir(
        objects.settings_button_text,
        LV_BASE_DIR_RTL,
        LV_PART_MAIN |
        LV_STATE_DEFAULT
    );

    lv_obj_set_style_text_color(
        objects.settings_button_text,
        lv_color_hex(0x000000),
        LV_PART_MAIN |
        LV_STATE_DEFAULT
    );
}

// ==================================================
// Configure Main Page Persian Labels
// ==================================================

static void configure_main_page_labels(void)
{
    // --------------------------------------------------
    // Status
    // --------------------------------------------------

    if (objects.status_text_main_page != NULL)
    {
        lv_label_set_text(
            objects.status_text_main_page,
            "وضعیت"
        );

        lv_obj_set_style_text_font(
            objects.status_text_main_page,
            &font_persian_16,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

        lv_obj_set_style_base_dir(
            objects.status_text_main_page,
            LV_BASE_DIR_RTL,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

    }

    // --------------------------------------------------
    // Voltage Static Label
    // --------------------------------------------------

    if (objects.voltage_label_main_static != NULL)
    {
        lv_label_set_text(
            objects.voltage_label_main_static,
            "ولتاژ"
        );

        lv_obj_set_style_text_font(
            objects.voltage_label_main_static,
            &font_persian_24,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

        lv_obj_set_style_base_dir(
            objects.voltage_label_main_static,
            LV_BASE_DIR_RTL,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

    }

    // --------------------------------------------------
    // Current Static Label
    // --------------------------------------------------

    if (objects.current_label_main_static != NULL)
    {
        lv_label_set_text(
            objects.current_label_main_static,
            "جریان"
        );

        lv_obj_set_style_text_font(
            objects.current_label_main_static,
            &font_persian_24,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

        lv_obj_set_style_base_dir(
            objects.current_label_main_static,
            LV_BASE_DIR_RTL,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );
    }
}

// ==================================================
// Initialization
// ==================================================

void main_page_init(void)
{
    configure_led();

    // --------------------------------------------------
    // Settings Button Persian Text
    // --------------------------------------------------

    configure_settings_button_text();

    // --------------------------------------------------
    // Main Page Persian Labels
    // --------------------------------------------------

    configure_main_page_labels();

    if (objects.obj0 != NULL)
    {
        lv_led_set_color(
            objects.obj0,
            lv_color_hex(LED_BLUE)
        );

        lv_led_set_brightness(
            objects.obj0,
            255
        );
    }

    led_blink_color =
        LED_BLUE;

    led_blink_state =
        true;

    led_blink_enable =
        false;

    led_blink_timer =
        millis();

    led_last_state =
        ERROR_NONE;

    // --------------------------------------------------
    // Error Box
    // --------------------------------------------------

    if (objects.error_box != NULL)
    {
        lv_obj_add_flag(
            objects.error_box,
            LV_OBJ_FLAG_HIDDEN
        );

        lv_obj_set_style_bg_color(
            objects.error_box,
            lv_color_hex(LED_RED),
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

        lv_obj_set_style_bg_opa(
            objects.error_box,
            LV_OPA_60,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

        lv_obj_set_style_shadow_width(
            objects.error_box,
            6,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

        lv_obj_set_style_shadow_color(
            objects.error_box,
            lv_color_hex(0x000000),
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

        lv_obj_set_style_shadow_opa(
            objects.error_box,
            LV_OPA_40,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

        lv_obj_set_style_shadow_ofs_x(
            objects.error_box,
            0,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

        lv_obj_set_style_shadow_ofs_y(
            objects.error_box,
            3,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

        error_box_last_color =
            LED_RED;

        init_error_msgbox();
    }

    if (objects.error_label != NULL)
    {
        lv_obj_add_flag(
            objects.error_label,
            LV_OBJ_FLAG_HIDDEN
        );
    }

    gui_last_voltage =
        -1000.0f;

    gui_last_current =
        -1000.0f;

    gui_last_error =
        ERROR_NONE;
}

// ==================================================
// Main Page Update
// ==================================================

void main_page_update(void)
{
    if (!main_screen_active())
    {
        return;
    }

    update_main_values();

    update_error_box();

    update_error_text_blink();

    update_led_state();

    led_task();
}