#include <Arduino.h>
#include <lvgl.h>

#include "MainPage.h"
#include "font_persian_16.h"
#include "font_persian_24.h"
#include "screen_manager.h"

extern "C"
{
#include "ui/ui.h"
#include "ui/screens.h"
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

static bool page_was_active = false;

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
    page_was_active = false;
}

// ==================================================
// Update
// ==================================================

void main_page_update(void)
{
    if (!screen_manager_is(SCREEN_ID_MAIN))
    {
        page_was_active = false;
        return;
    }

    if (!page_was_active)
    {
        configure_settings_button_text();
        configure_main_page_labels();
        page_was_active = true;
    }

    // Main data, LED and Error Box remain owned by tasks.cpp.
}
