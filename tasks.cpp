#include <Arduino.h>
#include <lvgl.h>

#include "tasks.h"
#include "uart.h"
#include "screen_manager.h"

#include "MainPage.h"
#include "SettingsPage.h"
#include "BuzzerPage.h"
#include "VCRangePage.h"

extern "C"
{
#include "ui/screens.h"
#include "ui/actions.h"
}

// ==================================================
// Hardware Configuration
// ==================================================

#define BTN_RIGHT   22
#define BTN_SELECT  23

// ==================================================
// Timing Configuration
// ==================================================

#define BUTTON_DEBOUNCE_MS 50
#define UART_TIMEOUT_MS    3000

// ==================================================
// System State
// ==================================================

struct SystemState
{
    float voltage;
    float current;

    bool data_received;
    bool uart_timeout;
    bool connection_lost;

    bool voltage_ok;
    bool current_ok;
    bool system_ok;

    bool low_voltage;
};

// ==================================================
// System State Instance
// ==================================================

static SystemState system_state =
{
    // Voltage
    0.0f,

    // Current
    0.0f,

    // Data state
    false,

    // UART timeout
    true,

    // Connection lost
    true,

    // Voltage OK
    false,

    // Current OK
    false,

    // System OK
    false,

    // Low voltage
    false
};

// ==================================================
// UART State
// ==================================================

static uint32_t last_valid_uart_time =
    0;

static bool valid_uart_received_once =
    false;

// ==================================================
// Button State
// ==================================================

static bool right_last_state =
    HIGH;

static bool select_last_state =
    HIGH;

static uint32_t right_last_change =
    0;

static uint32_t select_last_change =
    0;

// ==================================================
// Forward Declarations
// ==================================================

static void buttons_task(void);
static void uart_task(void);
static void safety_task(void);
static void gui_task(void);

// ==================================================
// Public System Getters
// ==================================================

float tasks_get_voltage(void)
{
    return system_state.voltage;
}

float tasks_get_current(void)
{
    return system_state.current;
}

bool tasks_data_received(void)
{
    return system_state.data_received;
}

bool tasks_connection_lost(void)
{
    return system_state.connection_lost;
}

bool tasks_low_voltage(void)
{
    return system_state.low_voltage;
}

// ==================================================
// UART Task
// ==================================================

static void uart_task(void)
{
    // --------------------------------------------------
    // Receive UART data
    // --------------------------------------------------

    uart_receive();

    // --------------------------------------------------
    // Temporary received values
    // --------------------------------------------------

    float new_voltage =
        0.0f;

    float new_current =
        0.0f;

    // --------------------------------------------------
    // Check for valid packet
    // --------------------------------------------------

    if (
        uart_get_values(
            &new_voltage,
            &new_current
        )
    )
    {
        // --------------------------------------------------
        // Store received values
        // --------------------------------------------------

        system_state.voltage =
            new_voltage;

        system_state.current =
            new_current;

        // --------------------------------------------------
        // Connection is alive
        // --------------------------------------------------

        system_state.data_received =
            true;

        system_state.uart_timeout =
            false;

        system_state.connection_lost =
            false;

        // --------------------------------------------------
        // Save time of last valid packet
        // --------------------------------------------------

        last_valid_uart_time =
            millis();

        valid_uart_received_once =
            true;
    }
}

// ==================================================
// Safety Task
// ==================================================

static void safety_task(void)
{
    uint32_t now =
        millis();

    // ==================================================
    // UART Connection State
    // ==================================================

    if (!valid_uart_received_once)
    {
        // --------------------------------------------------
        // No valid packet has ever been received
        // --------------------------------------------------

        system_state.data_received =
            false;

        system_state.uart_timeout =
            true;

        system_state.connection_lost =
            true;
    }
    else if (
        now - last_valid_uart_time >
        UART_TIMEOUT_MS
    )
    {
        // --------------------------------------------------
        // Communication timeout
        // --------------------------------------------------

        system_state.data_received =
            false;

        system_state.uart_timeout =
            true;

        system_state.connection_lost =
            true;
    }
    else
    {
        // --------------------------------------------------
        // Communication is OK
        // --------------------------------------------------

        system_state.data_received =
            true;

        system_state.uart_timeout =
            false;

        system_state.connection_lost =
            false;
    }

    // ==================================================
    // Voltage Validation
    // ==================================================

    system_state.voltage_ok =
        (
            system_state.voltage >=
            get_voltage_min_limit()
        )
        &&
        (
            system_state.voltage <=
            get_voltage_max_limit()
        );

    // ==================================================
    // Current Validation
    // ==================================================

    system_state.current_ok =
        (
            system_state.current >=
            get_current_min_limit()
        )
        &&
        (
            system_state.current <=
            get_current_max_limit()
        );

    // ==================================================
    // Low Voltage Detection
    // ==================================================

    system_state.low_voltage =
        (
            system_state.voltage <
            get_voltage_min_limit()
        );

    // ==================================================
    // Overall System State
    // ==================================================

    system_state.system_ok =
        system_state.data_received
        &&
        !system_state.connection_lost
        &&
        system_state.voltage_ok
        &&
        system_state.current_ok;
}

// ==================================================
// RIGHT Button Handling
// ==================================================

static void handle_right_release(void)
{
    enum ScreensEnum screen =
        screen_manager_get();

    switch (screen)
    {
        // ==================================================
        // Main
        // ==================================================

        case SCREEN_ID_MAIN:

            /*
             * Currently Main has only one selectable option.
             * Therefore RIGHT has no additional navigation work.
             */

            break;

        // ==================================================
        // Settings
        // ==================================================

        case SCREEN_ID_SETTINGS_PAGE:

            settings_page_handle_right();

            break;

        // ==================================================
        // Buzzer
        // ==================================================

        case SCREEN_ID_BUZZER_SETTINGS:

            buzzer_page_handle_right();

            break;

        // ==================================================
        // Voltage / Current Range
        // ==================================================

        case SCREEN_ID_V_C_RANGE_SETTINGS:

            vc_range_page_handle_right();

            break;

        // ==================================================
        // Other Screens
        // ==================================================

        default:

            break;
    }
}

// ==================================================
// SELECT Button Handling
// ==================================================

static void handle_select_release(void)
{
    enum ScreensEnum screen =
        screen_manager_get();

    switch (screen)
    {
        // ==================================================
        // Main
        // ==================================================

        case SCREEN_ID_MAIN:

            action_go_to_settings_page(
                NULL
            );

            break;

        // ==================================================
        // Settings
        // ==================================================

        case SCREEN_ID_SETTINGS_PAGE:

            settings_page_handle_select();

            break;

        // ==================================================
        // Buzzer
        // ==================================================

        case SCREEN_ID_BUZZER_SETTINGS:

            buzzer_page_handle_select();

            break;

        // ==================================================
        // Voltage / Current Range
        // ==================================================

        case SCREEN_ID_V_C_RANGE_SETTINGS:

            vc_range_page_handle_select();

            break;

        // ==================================================
        // Other Screens
        // ==================================================

        default:

            break;
    }
}

// ==================================================
// Button Task
// ==================================================

static void buttons_task(void)
{
    // ==================================================
    // Read Inputs
    // ==================================================

    bool right_state =
        digitalRead(
            BTN_RIGHT
        );

    bool select_state =
        digitalRead(
            BTN_SELECT
        );

    uint32_t now =
        millis();

    // ==================================================
    // RIGHT Button
    // ==================================================

    if (
        right_state !=
        right_last_state
    )
    {
        if (
            now - right_last_change >=
            BUTTON_DEBOUNCE_MS
        )
        {
            right_last_change =
                now;

            right_last_state =
                right_state;

            // --------------------------------------------------
            // Only process release
            // --------------------------------------------------

            if (
                right_state ==
                HIGH
            )
            {
                handle_right_release();
            }
        }
    }

    // ==================================================
    // SELECT Button
    // ==================================================

    if (
        select_state !=
        select_last_state
    )
    {
        if (
            now - select_last_change >=
            BUTTON_DEBOUNCE_MS
        )
        {
            select_last_change =
                now;

            select_last_state =
                select_state;

            // --------------------------------------------------
            // Only process release
            // --------------------------------------------------

            if (
                select_state ==
                HIGH
            )
            {
                handle_select_release();
            }
        }
    }
}

// ==================================================
// GUI Task
// ==================================================

static void gui_task(void)
{
    static uint32_t last_gui_update =
        0;

    uint32_t now =
        millis();

    // ==================================================
    // GUI Update Period
    // ==================================================

    if (
        now - last_gui_update <
        20
    )
    {
        return;
    }

    last_gui_update =
        now;

    // ==================================================
    // Main Page
    // ==================================================

    main_page_update();

    // ==================================================
    // Settings Page
    // ==================================================

    settings_page_update();

    // ==================================================
    // Buzzer Page
    // ==================================================

    buzzer_page_update();

    // ==================================================
    // Voltage / Current Page
    // ==================================================

    vc_range_page_update();

    // ==================================================
    // EEZ Screen Tick
    //
    // V/C screen is handled separately because
    // its sliders are manually synchronized.
    // ==================================================

    if (
        !screen_manager_is(
            SCREEN_ID_V_C_RANGE_SETTINGS
        )
    )
    {
        int16_t screen_index =
            (int16_t)screen_manager_get() - 1;

        if (
            screen_index >= 0 &&
            screen_index < 4
        )
        {
            tick_screen(
                screen_index
            );
        }
    }
}

// ==================================================
// Task Initialization
// ==================================================

void tasks_init(void)
{
    // ==================================================
    // Initialize Buttons
    // ==================================================

    pinMode(
        BTN_RIGHT,
        INPUT_PULLUP
    );

    pinMode(
        BTN_SELECT,
        INPUT_PULLUP
    );

    // ==================================================
    // Read Initial Button States
    // ==================================================

    right_last_state =
        digitalRead(
            BTN_RIGHT
        );

    select_last_state =
        digitalRead(
            BTN_SELECT
        );

    right_last_change =
        millis();

    select_last_change =
        millis();

    // ==================================================
    // Reset UART State
    // ==================================================

    last_valid_uart_time =
        millis();

    valid_uart_received_once =
        false;

    // ==================================================
    // Reset System State
    // ==================================================

    system_state.voltage =
        0.0f;

    system_state.current =
        0.0f;

    system_state.data_received =
        false;

    system_state.uart_timeout =
        true;

    system_state.connection_lost =
        true;

    system_state.voltage_ok =
        false;

    system_state.current_ok =
        false;

    system_state.system_ok =
        false;

    system_state.low_voltage =
        false;

    // ==================================================
    // Initialize Pages
    // ==================================================

    main_page_init();

    settings_page_init();

    buzzer_page_init();

    vc_range_page_init();
}

// ==================================================
// Main Task Runner
// ==================================================

void tasks_run(void)
{
    // ==================================================
    // 1. Read physical buttons
    // ==================================================

    buttons_task();

    // ==================================================
    // 2. Receive UART
    // ==================================================

    uart_task();

    // ==================================================
    // 3. Validate system state
    // ==================================================

    safety_task();

    // ==================================================
    // 4. Run buzzer
    // ==================================================

    buzzer_page_runtime_task(
        system_state.data_received,
        system_state.connection_lost,
        system_state.low_voltage
    );

    // ==================================================
    // 5. Apply pending screen changes
    // ==================================================

    screen_manager_process();

    // ==================================================
    // 6. Update page GUI
    // ==================================================

    gui_task();
}