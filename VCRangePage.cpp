#include <Arduino.h>
#include <lvgl.h>
#include <stdio.h>

#include "VCRangePage.h"
#include "screen_manager.h"

extern "C"
{
#include "ui/screens.h"
#include "ui/actions.h"
}

// ==================================================
// External EEZ Object
// ==================================================

extern lv_obj_t *tick_value_change_obj;

// ==================================================
// Default Voltage / Current Limits
// ==================================================

#define DEFAULT_VOLTAGE_MIN 20.0f
#define DEFAULT_VOLTAGE_MAX 25.0f

#define DEFAULT_CURRENT_MIN 0.0f
#define DEFAULT_CURRENT_MAX 1.0f

// ==================================================
// Allowed Voltage / Current Limits
// ==================================================

#define VOLTAGE_LIMIT_MIN 0.0f
#define VOLTAGE_LIMIT_MAX 30.0f

#define CURRENT_LIMIT_MIN 0.0f
#define CURRENT_LIMIT_MAX 3.0f

// ==================================================
// Focus Color
// ==================================================

#define FOCUS_COLOR 0xFF0000

// ==================================================
// Focus Index
//
// 0 = Voltage Minimum
// 1 = Voltage Maximum
// 2 = Current Minimum
// 3 = Current Maximum
// 4 = Back
// ==================================================

static uint8_t vc_focus =
    0;

// ==================================================
// Edit Mode
//
// false = navigation mode
// true  = value editing mode
// ==================================================

static bool vc_edit_mode =
    false;

// ==================================================
// Page Activity State
// ==================================================

static bool page_was_active =
    false;

// ==================================================
// Voltage / Current Limits
// ==================================================

static float voltage_min_limit =
    DEFAULT_VOLTAGE_MIN;

static float voltage_max_limit =
    DEFAULT_VOLTAGE_MAX;

static float current_min_limit =
    DEFAULT_CURRENT_MIN;

static float current_max_limit =
    DEFAULT_CURRENT_MAX;

// ==================================================
// GUI Cache
// ==================================================

static float gui_last_voltage_min =
    -1000.0f;

static float gui_last_voltage_max =
    -1000.0f;

static float gui_last_current_min =
    -1000.0f;

static float gui_last_current_max =
    -1000.0f;

// ==================================================
// Check V/C Screen
// ==================================================

static bool vc_range_screen_active(void)
{
    return screen_manager_is(
        SCREEN_ID_V_C_RANGE_SETTINGS
    );
}

// ==================================================
// Voltage Minimum Setter
// ==================================================

void set_voltage_min_limit(
    float value
)
{
    if (value < VOLTAGE_LIMIT_MIN)
    {
        value =
            VOLTAGE_LIMIT_MIN;
    }

    if (value > VOLTAGE_LIMIT_MAX)
    {
        value =
            VOLTAGE_LIMIT_MAX;
    }

    voltage_min_limit =
        value;
}

// ==================================================
// Voltage Maximum Setter
// ==================================================

void set_voltage_max_limit(
    float value
)
{
    if (value < VOLTAGE_LIMIT_MIN)
    {
        value =
            VOLTAGE_LIMIT_MIN;
    }

    if (value > VOLTAGE_LIMIT_MAX)
    {
        value =
            VOLTAGE_LIMIT_MAX;
    }

    voltage_max_limit =
        value;
}

// ==================================================
// Current Minimum Setter
// ==================================================

void set_current_min_limit(
    float value
)
{
    if (value < CURRENT_LIMIT_MIN)
    {
        value =
            CURRENT_LIMIT_MIN;
    }

    if (value > CURRENT_LIMIT_MAX)
    {
        value =
            CURRENT_LIMIT_MAX;
    }

    current_min_limit =
        value;
}

// ==================================================
// Current Maximum Setter
// ==================================================

void set_current_max_limit(
    float value
)
{
    if (value < CURRENT_LIMIT_MIN)
    {
        value =
            CURRENT_LIMIT_MIN;
    }

    if (value > CURRENT_LIMIT_MAX)
    {
        value =
            CURRENT_LIMIT_MAX;
    }

    current_max_limit =
        value;
}

// ==================================================
// Voltage Minimum Getter
// ==================================================

float get_voltage_min_limit(void)
{
    return voltage_min_limit;
}

// ==================================================
// Voltage Maximum Getter
// ==================================================

float get_voltage_max_limit(void)
{
    return voltage_max_limit;
}

// ==================================================
// Current Minimum Getter
// ==================================================

float get_current_min_limit(void)
{
    return current_min_limit;
}

// ==================================================
// Current Maximum Getter
// ==================================================

float get_current_max_limit(void)
{
    return current_max_limit;
}

// ==================================================
// Get V/C Back Button
// ==================================================

static lv_obj_t *get_vc_back_button(void)
{
    if (
        objects.exit_from_v_c_menu_button == NULL
    )
    {
        return NULL;
    }

    return lv_obj_get_parent(
        objects.exit_from_v_c_menu_button
    );
}

// ==================================================
// Clear All Focus Borders
// ==================================================

static void clear_vc_focus(void)
{
    // --------------------------------------------------
    // Voltage Minimum
    // --------------------------------------------------

    if (
        objects.voltage_minimum != NULL
    )
    {
        lv_obj_set_style_border_width(
            objects.voltage_minimum,
            0,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );
    }

    // --------------------------------------------------
    // Voltage Maximum
    // --------------------------------------------------

    if (
        objects.voltage_maximum != NULL
    )
    {
        lv_obj_set_style_border_width(
            objects.voltage_maximum,
            0,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );
    }

    // --------------------------------------------------
    // Current Minimum
    // --------------------------------------------------

    if (
        objects.current_minimum != NULL
    )
    {
        lv_obj_set_style_border_width(
            objects.current_minimum,
            0,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );
    }

    // --------------------------------------------------
    // Current Maximum
    // --------------------------------------------------

    if (
        objects.current_maximum != NULL
    )
    {
        lv_obj_set_style_border_width(
            objects.current_maximum,
            0,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );
    }

    // --------------------------------------------------
    // Back Button
    // --------------------------------------------------

    lv_obj_t *back_button =
        get_vc_back_button();

    if (back_button != NULL)
    {
        lv_obj_set_style_border_width(
            back_button,
            0,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );
    }
}

// ==================================================
// Apply Current Focus
// ==================================================

static void apply_vc_focus(void)
{
    if (!vc_range_screen_active())
    {
        return;
    }

    clear_vc_focus();

    lv_obj_t *selected =
        NULL;

    switch (vc_focus)
    {
        // --------------------------------------------------
        // Voltage Minimum
        // --------------------------------------------------

        case 0:

            selected =
                objects.voltage_minimum;

            break;

        // --------------------------------------------------
        // Voltage Maximum
        // --------------------------------------------------

        case 1:

            selected =
                objects.voltage_maximum;

            break;

        // --------------------------------------------------
        // Current Minimum
        // --------------------------------------------------

        case 2:

            selected =
                objects.current_minimum;

            break;

        // --------------------------------------------------
        // Current Maximum
        // --------------------------------------------------

        case 3:

            selected =
                objects.current_maximum;

            break;

        // --------------------------------------------------
        // Back
        // --------------------------------------------------

        case 4:

            selected =
                get_vc_back_button();

            break;

        default:

            break;
    }

    if (selected != NULL)
    {
        lv_obj_set_style_border_width(
            selected,
            vc_edit_mode ? 3 : 2,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

        lv_obj_set_style_border_color(
            selected,
            lv_color_hex(
                FOCUS_COLOR
            ),
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );
    }
}

// ==================================================
// Update Voltage / Current Labels and Sliders
// ==================================================

static void update_vc_range_gui(void)
{
    if (!vc_range_screen_active())
    {
        return;
    }

    // ==================================================
    // Voltage Minimum Label
    // ==================================================

    if (
        objects.voltage_min_value != NULL &&
        voltage_min_limit !=
        gui_last_voltage_min
    )
    {
        char text[16];

        snprintf(
            text,
            sizeof(text),
            "%.1f",
            voltage_min_limit
        );

        lv_label_set_text(
            objects.voltage_min_value,
            text
        );

        gui_last_voltage_min =
            voltage_min_limit;
    }

    // ==================================================
    // Voltage Maximum Label
    // ==================================================

    if (
        objects.voltage_max_value != NULL &&
        voltage_max_limit !=
        gui_last_voltage_max
    )
    {
        char text[16];

        snprintf(
            text,
            sizeof(text),
            "%.1f",
            voltage_max_limit
        );

        lv_label_set_text(
            objects.voltage_max_value,
            text
        );

        gui_last_voltage_max =
            voltage_max_limit;
    }

    // ==================================================
    // Current Minimum Label
    // ==================================================

    if (
        objects.current_min_value != NULL &&
        current_min_limit !=
        gui_last_current_min
    )
    {
        char text[16];

        snprintf(
            text,
            sizeof(text),
            "%.1f",
            current_min_limit
        );

        lv_label_set_text(
            objects.current_min_value,
            text
        );

        gui_last_current_min =
            current_min_limit;
    }

    // ==================================================
    // Current Maximum Label
    // ==================================================

    if (
        objects.current_max_value != NULL &&
        current_max_limit !=
        gui_last_current_max
    )
    {
        char text[16];

        snprintf(
            text,
            sizeof(text),
            "%.1f",
            current_max_limit
        );

        lv_label_set_text(
            objects.current_max_value,
            text
        );

        gui_last_current_max =
            current_max_limit;
    }

    // ==================================================
    // Voltage Minimum Slider
    // ==================================================

    if (
        objects.voltage_minimum != NULL
    )
    {
        int32_t value =
            (int32_t)voltage_min_limit;

        int32_t current =
            lv_slider_get_value(
                objects.voltage_minimum
            );

        if (value != current)
        {
            tick_value_change_obj =
                objects.voltage_minimum;

            lv_slider_set_value(
                objects.voltage_minimum,
                value,
                LV_ANIM_OFF
            );

            tick_value_change_obj =
                NULL;
        }
    }

    // ==================================================
    // Voltage Maximum Slider
    // ==================================================

    if (
        objects.voltage_maximum != NULL
    )
    {
        int32_t value =
            (int32_t)voltage_max_limit;

        int32_t current =
            lv_slider_get_value(
                objects.voltage_maximum
            );

        if (value != current)
        {
            tick_value_change_obj =
                objects.voltage_maximum;

            lv_slider_set_value(
                objects.voltage_maximum,
                value,
                LV_ANIM_OFF
            );

            tick_value_change_obj =
                NULL;
        }
    }

    // ==================================================
    // Current Minimum Slider
    // ==================================================

    if (
        objects.current_minimum != NULL
    )
    {
        int32_t value =
            (int32_t)current_min_limit;

        int32_t current =
            lv_slider_get_value(
                objects.current_minimum
            );

        if (value != current)
        {
            tick_value_change_obj =
                objects.current_minimum;

            lv_slider_set_value(
                objects.current_minimum,
                value,
                LV_ANIM_OFF
            );

            tick_value_change_obj =
                NULL;
        }
    }

    // ==================================================
    // Current Maximum Slider
    // ==================================================

    if (
        objects.current_maximum != NULL
    )
    {
        int32_t value =
            (int32_t)current_max_limit;

        int32_t current =
            lv_slider_get_value(
                objects.current_maximum
            );

        if (value != current)
        {
            tick_value_change_obj =
                objects.current_maximum;

            lv_slider_set_value(
                objects.current_maximum,
                value,
                LV_ANIM_OFF
            );

            tick_value_change_obj =
                NULL;
        }
    }
}

// ==================================================
// Change Selected V/C Value
// ==================================================

static void vc_change_value(void)
{
    if (!vc_range_screen_active())
    {
        return;
    }

    // ==================================================
    // Voltage Minimum
    // ==================================================

    if (vc_focus == 0)
    {
        float value =
            get_voltage_min_limit();

        value += 1.0f;

        if (
            value >
            VOLTAGE_LIMIT_MAX
        )
        {
            value =
                VOLTAGE_LIMIT_MIN;
        }

        set_voltage_min_limit(
            value
        );
    }

    // ==================================================
    // Voltage Maximum
    // ==================================================

    else if (vc_focus == 1)
    {
        float value =
            get_voltage_max_limit();

        value += 1.0f;

        if (
            value >
            VOLTAGE_LIMIT_MAX
        )
        {
            value =
                VOLTAGE_LIMIT_MIN;
        }

        set_voltage_max_limit(
            value
        );
    }

    // ==================================================
    // Current Minimum
    // ==================================================

    else if (vc_focus == 2)
    {
        float value =
            get_current_min_limit();

        value += 1.0f;

        if (
            value >
            CURRENT_LIMIT_MAX
        )
        {
            value =
                CURRENT_LIMIT_MIN;
        }

        set_current_min_limit(
            value
        );
    }

    // ==================================================
    // Current Maximum
    // ==================================================

    else if (vc_focus == 3)
    {
        float value =
            get_current_max_limit();

        value += 1.0f;

        if (
            value >
            CURRENT_LIMIT_MAX
        )
        {
            value =
                CURRENT_LIMIT_MIN;
        }

        set_current_max_limit(
            value
        );
    }

    // ==================================================
    // Refresh GUI
    // ==================================================

    update_vc_range_gui();

    apply_vc_focus();
}

// ==================================================
// Initialize V/C Page
// ==================================================

void vc_range_page_init(void)
{
    // --------------------------------------------------
    // Initial Focus
    // --------------------------------------------------

    vc_focus =
        0;

    // --------------------------------------------------
    // Start in Navigation Mode
    // --------------------------------------------------

    vc_edit_mode =
        false;

    // --------------------------------------------------
    // Page State
    // --------------------------------------------------

    page_was_active =
        false;

    // --------------------------------------------------
    // Reset GUI Cache
    // --------------------------------------------------

    gui_last_voltage_min =
        -1000.0f;

    gui_last_voltage_max =
        -1000.0f;

    gui_last_current_min =
        -1000.0f;

    gui_last_current_max =
        -1000.0f;
}

// ==================================================
// Update V/C Page
// ==================================================

void vc_range_page_update(void)
{
    // --------------------------------------------------
    // Page is not active
    // --------------------------------------------------

    if (!vc_range_screen_active())
    {
        page_was_active =
            false;

        return;
    }

    // --------------------------------------------------
    // First update after entering page
    // --------------------------------------------------

    if (!page_was_active)
    {
        vc_focus =
            0;

        vc_edit_mode =
            false;

        page_was_active =
            true;

        update_vc_range_gui();

        apply_vc_focus();

        return;
    }

    // --------------------------------------------------
    // Normal update
    // --------------------------------------------------

    update_vc_range_gui();
}

// ==================================================
// RIGHT Button
// ==================================================

void vc_range_page_handle_right(void)
{
    if (!vc_range_screen_active())
    {
        return;
    }

    // --------------------------------------------------
    // Edit mode
    // --------------------------------------------------

    if (vc_edit_mode)
    {
        vc_change_value();

        return;
    }

    // --------------------------------------------------
    // Navigation mode
    // --------------------------------------------------

    vc_focus++;

    if (vc_focus > 4)
    {
        vc_focus =
            0;
    }

    apply_vc_focus();
}

// ==================================================
// SELECT Button
// ==================================================

void vc_range_page_handle_select(void)
{
    if (!vc_range_screen_active())
    {
        return;
    }

    // ==================================================
    // Back Button
    // ==================================================

    if (vc_focus == 4)
    {
        if (!vc_edit_mode)
        {
            action_exit_from_v_c_menu_to_settings(
                NULL
            );
        }

        return;
    }

    // ==================================================
    // Enter / Exit Edit Mode
    // ==================================================

    vc_edit_mode =
        !vc_edit_mode;

    apply_vc_focus();
}