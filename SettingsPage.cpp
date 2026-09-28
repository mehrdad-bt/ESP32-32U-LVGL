#include <Arduino.h>
#include <lvgl.h>

#include "SettingsPage.h"
#include "screen_manager.h"
#include "font_persian_16.h"

extern "C"
{
#include "ui/screens.h"
#include "ui/actions.h"
}

// ==================================================
// Configuration
// ==================================================

#define SETTINGS_OPTION_BUZZER        0
#define SETTINGS_OPTION_CALIBRATION   1
#define SETTINGS_OPTION_VC_RANGE      2
#define SETTINGS_OPTION_BACK          3

#define FOCUS_COLOR 0xFF0000

// ==================================================
// State
// ==================================================

static int settings_selection =
    SETTINGS_OPTION_BUZZER;

static bool page_was_active =
    false;

// ==================================================
// Configure Persian Labels
// ==================================================

static void configure_settings_labels(void)
{
    // --------------------------------------------------
    // Buzzer
    // --------------------------------------------------

    if (objects.buzzer_text_settings_page != NULL)
    {
        lv_label_set_text(
            objects.buzzer_text_settings_page,
            "بازر"
        );

        lv_obj_set_style_text_font(
            objects.buzzer_text_settings_page,
            &font_persian_16,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

        lv_obj_set_style_base_dir(
            objects.buzzer_text_settings_page,
            LV_BASE_DIR_RTL,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

    }

    // --------------------------------------------------
    // Touch Calibration
    // --------------------------------------------------

    if (objects.calibration_text_settings_page != NULL)
    {
        lv_label_set_text(
            objects.calibration_text_settings_page,
            "کالیبراسیون"
        );

        lv_obj_set_style_text_font(
            objects.calibration_text_settings_page,
            &font_persian_16,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

        lv_obj_set_style_base_dir(
            objects.calibration_text_settings_page,
            LV_BASE_DIR_RTL,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

    }

    // --------------------------------------------------
    // Voltage / Current Range
    // --------------------------------------------------

    if (objects.vc_range_text_settings_page != NULL)
    {
        lv_label_set_text(
            objects.vc_range_text_settings_page,
            "محدوده خطا"
        );

        lv_obj_set_style_text_font(
            objects.vc_range_text_settings_page,
            &font_persian_16,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

        lv_obj_set_style_base_dir(
            objects.vc_range_text_settings_page,
            LV_BASE_DIR_RTL,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

    }

    // --------------------------------------------------
    // Exit
    // --------------------------------------------------

    if (objects.exit_label_settinhs_page != NULL)
    {
        lv_label_set_text(
            objects.exit_label_settinhs_page,
            "خروج"
        );

        lv_obj_set_style_text_font(
            objects.exit_label_settinhs_page,
            &font_persian_16,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

        lv_obj_set_style_base_dir(
            objects.exit_label_settinhs_page,
            LV_BASE_DIR_RTL,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

    }
}

// ==================================================
// Clear Focus
// ==================================================

static void clear_settings_highlight(void)
{
    if (objects.buzzer != NULL)
    {
        lv_obj_set_style_border_width(
            objects.buzzer,
            0,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );
    }

    if (objects.touch_calibration != NULL)
    {
        lv_obj_set_style_border_width(
            objects.touch_calibration,
            0,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );
    }

    if (objects.voltage_range != NULL)
    {
        lv_obj_set_style_border_width(
            objects.voltage_range,
            0,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );
    }

    if (objects.exit_settings != NULL)
    {
        lv_obj_set_style_border_width(
            objects.exit_settings,
            0,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );
    }
}

// ==================================================
// Apply Focus
// ==================================================

static void apply_settings_highlight(void)
{
    clear_settings_highlight();

    lv_obj_t *selected =
        NULL;

    switch (settings_selection)
    {
        case SETTINGS_OPTION_BUZZER:
            selected =
                objects.buzzer;
            break;

        case SETTINGS_OPTION_CALIBRATION:
            selected =
                objects.touch_calibration;
            break;

        case SETTINGS_OPTION_VC_RANGE:
            selected =
                objects.voltage_range;
            break;

        case SETTINGS_OPTION_BACK:
            selected =
                objects.exit_settings;
            break;

        default:
            break;
    }

    if (selected != NULL)
    {
        lv_obj_set_style_border_width(
            selected,
            2,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

        lv_obj_set_style_border_color(
            selected,
            lv_color_hex(FOCUS_COLOR),
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );
    }
}

// ==================================================
// Init
// ==================================================

void settings_page_init(void)
{
    settings_selection =
        SETTINGS_OPTION_BUZZER;

    page_was_active =
        false;

    // --------------------------------------------------
    // Persian Labels
    // --------------------------------------------------

    configure_settings_labels();
}

// ==================================================
// Update
// ==================================================

void settings_page_update(void)
{
    if (
        !screen_manager_is(
            SCREEN_ID_SETTINGS_PAGE
        )
    )
    {
        page_was_active =
            false;

        return;
    }

    if (!page_was_active)
    {
        apply_settings_highlight();

        page_was_active =
            true;
    }
}

// ==================================================
// RIGHT
// ==================================================

void settings_page_handle_right(void)
{
    if (
        !screen_manager_is(
            SCREEN_ID_SETTINGS_PAGE
        )
    )
    {
        return;
    }

    settings_selection++;

    if (
        settings_selection >
        SETTINGS_OPTION_BACK
    )
    {
        settings_selection =
            SETTINGS_OPTION_BUZZER;
    }

    apply_settings_highlight();
}

// ==================================================
// SELECT
// ==================================================

void settings_page_handle_select(void)
{
    if (
        !screen_manager_is(
            SCREEN_ID_SETTINGS_PAGE
        )
    )
    {
        return;
    }

    switch (settings_selection)
    {
        case SETTINGS_OPTION_BUZZER:

            action_go_to_buzzer_settings(
                NULL
            );

            break;

        case SETTINGS_OPTION_CALIBRATION:

            action_go_to_touch_calibration(
                NULL
            );

            break;

        case SETTINGS_OPTION_VC_RANGE:

            action_go_to_v_c_range_settings(
                NULL
            );

            break;

        case SETTINGS_OPTION_BACK:

            action_exit_to_main_page(
                NULL
            );

            break;

        default:
            break;
    }
}