#include <Arduino.h>
#include <lvgl.h>

#include "BuzzerPage.h"
#include "screen_manager.h"
#include "font_persian_16.h"
#include "font_persian_24.h"

extern "C"
{
#include "ui/screens.h"
#include "ui/vars.h"
#include "ui/actions.h"
}

// ==================================================
// Configuration
// ==================================================

#define BUZZER_MODE_1  0
#define BUZZER_MODE_2  1
#define BUZZER_MODE_3  2

#define FOCUS_COLOR 0xFF0000

// ==================================================
// Runtime State Owned by Buzzer Page
// ==================================================

static uint8_t buzzer_mode =
    BUZZER_MODE_1;

// ==================================================
// UI State
// ==================================================

static lv_obj_t *buzzer_dropdown =
    NULL;

static bool buzzer_dropdown_open =
    false;

static bool buzzer_focus_back =
    false;

static bool page_was_active =
    false;

// ==================================================
// Configure Persian Labels
// ==================================================

static void configure_buzzer_labels(void)
{
    // --------------------------------------------------
    // Buzzer Settings Page Label
    // --------------------------------------------------

    if (objects.buzzer_settings_page_label != NULL)
    {
        lv_label_set_text(
            objects.buzzer_settings_page_label,
            "تنظیمات بازر"
        );

        lv_obj_set_style_text_font(
            objects.buzzer_settings_page_label,
            &font_persian_24,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

        lv_obj_set_style_base_dir(
            objects.buzzer_settings_page_label,
            LV_BASE_DIR_RTL,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

        lv_obj_set_style_text_color(
            objects.buzzer_settings_page_label,
            lv_color_hex(0xebf900),
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );
    }

    // --------------------------------------------------
    // Back Text
    // --------------------------------------------------

    if (objects.back_text_buzzer_page != NULL)
    {
        lv_label_set_text(
            objects.back_text_buzzer_page,
            "بازگشت"
        );

        lv_obj_set_style_text_font(
            objects.back_text_buzzer_page,
            &font_persian_16,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

        lv_obj_set_style_base_dir(
            objects.back_text_buzzer_page,
            LV_BASE_DIR_RTL,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );

        lv_obj_set_style_text_color(
            objects.back_text_buzzer_page,
            lv_color_hex(0x000000),
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );
    }
}

// ==================================================
// Find Dropdown
// ==================================================

static void buzzer_dropdown_find(void)
{
    if (objects.buzzer_settings == NULL)
    {
        buzzer_dropdown = NULL;
        return;
    }

    buzzer_dropdown =
        lv_obj_get_child(
            objects.buzzer_settings,
            0
        );
}

// ==================================================
// Mode
// ==================================================

void buzzer_set_mode(
    uint8_t mode
)
{
    if (mode > BUZZER_MODE_3)
    {
        mode = BUZZER_MODE_3;
    }

    buzzer_mode = mode;
}

uint8_t buzzer_get_mode(void)
{
    return buzzer_mode;
}

// ==================================================
// Clear Focus
// ==================================================

static void clear_buzzer_focus(void)
{
    buzzer_dropdown_find();

    if (buzzer_dropdown != NULL)
    {
        lv_obj_set_style_border_width(
            buzzer_dropdown,
            0,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );
    }

    if (objects.buzzer_settings_page_back_button != NULL)
    {
        lv_obj_set_style_border_width(
            objects.buzzer_settings_page_back_button,
            0,
            LV_PART_MAIN |
            LV_STATE_DEFAULT
        );
    }
}

// ==================================================
// Apply Focus
// ==================================================

static void apply_buzzer_focus(void)
{
    if (!screen_manager_is(SCREEN_ID_BUZZER_SETTINGS))
    {
        return;
    }

    clear_buzzer_focus();
    buzzer_dropdown_find();

    if (!buzzer_focus_back)
    {
        if (buzzer_dropdown != NULL)
        {
            lv_obj_set_style_border_width(
                buzzer_dropdown,
                buzzer_dropdown_open ? 3 : 2,
                LV_PART_MAIN |
                LV_STATE_DEFAULT
            );

            lv_obj_set_style_border_color(
                buzzer_dropdown,
                lv_color_hex(FOCUS_COLOR),
                LV_PART_MAIN |
                LV_STATE_DEFAULT
            );
        }
    }
    else
    {
        if (objects.buzzer_settings_page_back_button != NULL)
        {
            lv_obj_set_style_border_width(
                objects.buzzer_settings_page_back_button,
                2,
                LV_PART_MAIN |
                LV_STATE_DEFAULT
            );

            lv_obj_set_style_border_color(
                objects.buzzer_settings_page_back_button,
                lv_color_hex(FOCUS_COLOR),
                LV_PART_MAIN |
                LV_STATE_DEFAULT
            );
        }
    }
}

// ==================================================
// Dropdown Navigation
// ==================================================

static void buzzer_dropdown_change(
    int direction
)
{
    buzzer_dropdown_find();

    if (buzzer_dropdown == NULL)
    {
        return;
    }

    uint16_t selected =
        lv_dropdown_get_selected(
            buzzer_dropdown
        );

    if (direction > 0)
    {
        if (selected < 2)
        {
            selected++;
        }
        else
        {
            selected = 0;
        }
    }
    else
    {
        if (selected > 0)
        {
            selected--;
        }
        else
        {
            selected = 2;
        }
    }

    lv_dropdown_set_selected(
        buzzer_dropdown,
        selected
    );

    buzzer_set_mode(
        (uint8_t)selected
    );

    set_var_buzzer_mode(
        (int32_t)selected
    );
}

// ==================================================
// Init
// ==================================================

void buzzer_page_init(void)
{
    buzzer_mode = BUZZER_MODE_1;
    buzzer_focus_back = false;
    buzzer_dropdown_open = false;
    page_was_active = false;

    if (buzzer_dropdown != NULL)
    {
        lv_dropdown_set_selected(
            buzzer_dropdown,
            buzzer_get_mode()
        );
    }
}

// ==================================================
// Update
// ==================================================

void buzzer_page_update(void)
{
    if (!screen_manager_is(SCREEN_ID_BUZZER_SETTINGS))
    {
        page_was_active = false;
        return;
    }

    if (buzzer_dropdown == NULL)
    {
        buzzer_dropdown_find();
    }

    if (!page_was_active)
    {
        configure_buzzer_labels();

        buzzer_focus_back = false;
        buzzer_dropdown_open = false;

        if (buzzer_dropdown != NULL)
        {
            lv_dropdown_close(buzzer_dropdown);
        }

        apply_buzzer_focus();
        page_was_active = true;
    }

    if (buzzer_dropdown == NULL)
    {
        return;
    }

    if (!buzzer_dropdown_open)
    {
        uint16_t selected =
            lv_dropdown_get_selected(
                buzzer_dropdown
            );

        if (selected != buzzer_mode)
        {
            lv_dropdown_set_selected(
                buzzer_dropdown,
                buzzer_mode
            );
        }
    }
}

// ==================================================
// RIGHT
// ==================================================

void buzzer_page_handle_right(void)
{
    if (!screen_manager_is(SCREEN_ID_BUZZER_SETTINGS))
    {
        return;
    }

    if (buzzer_dropdown_open)
    {
        buzzer_dropdown_change(1);
        return;
    }

    if (!buzzer_focus_back)
    {
        buzzer_focus_back = true;
    }
    else
    {
        buzzer_focus_back = false;
    }

    apply_buzzer_focus();
}

// ==================================================
// SELECT
// ==================================================

void buzzer_page_handle_select(void)
{
    if (!screen_manager_is(SCREEN_ID_BUZZER_SETTINGS))
    {
        return;
    }

    buzzer_dropdown_find();

    if (!buzzer_focus_back)
    {
        if (buzzer_dropdown == NULL)
        {
            return;
        }

        if (!buzzer_dropdown_open)
        {
            buzzer_dropdown_open = true;

            lv_dropdown_open(
                buzzer_dropdown
            );

            apply_buzzer_focus();
        }
        else
        {
            buzzer_dropdown_open = false;

            lv_dropdown_close(
                buzzer_dropdown
            );

            uint16_t selected =
                lv_dropdown_get_selected(
                    buzzer_dropdown
                );

            buzzer_set_mode(
                (uint8_t)selected
            );

            set_var_buzzer_mode(
                (int32_t)selected
            );

            apply_buzzer_focus();
        }

        return;
    }

    action_go_from_buzzer_settings_page_to_settings_page(
        NULL
    );
}

// ==================================================
// Runtime Buzzer Task
// ==================================================
// The current project runs the buzzer runtime from tasks.cpp.
// This function is kept as a compatibility entry point.
// ==================================================

void buzzer_page_runtime_task(
    bool data_received,
    bool connection_lost,
    bool low_voltage
)
{
    (void)data_received;
    (void)connection_lost;
    (void)low_voltage;
}
