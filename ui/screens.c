#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;

//
// Event handlers
//

lv_obj_t *tick_value_change_obj;

static void event_handler_cb_buzzer_settings_obj1(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_dropdown_get_selected(ta);
            set_var_buzzer_mode(value);
        }
    }
}

static void event_handler_cb_v_c_range_settings_voltage_minimum(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_slider_get_value(ta);
            set_var_voltage_min(value);
        }
    }
}

static void event_handler_cb_v_c_range_settings_voltage_maximum(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_slider_get_value(ta);
            set_var_voltage_max(value);
        }
    }
}

static void event_handler_cb_v_c_range_settings_current_minimum(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_slider_get_value(ta);
            set_var_current_min(value);
        }
    }
}

static void event_handler_cb_v_c_range_settings_current_maximum(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_slider_get_value(ta);
            set_var_current_max(value);
        }
    }
}

//
// Screens
//

void create_screen_main() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // Voltage
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.voltage = obj;
            lv_obj_set_pos(obj, 142, 81);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Voltage");
        }
        {
            // BTN_SETTINGS
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.btn_settings = obj;
            lv_obj_set_pos(obj, 11, 167);
            lv_obj_set_size(obj, 82, 50);
            lv_obj_add_event_cb(obj, action_go_to_settings_page, LV_EVENT_RELEASED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x7d7d7d), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xfff9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // settings_button_text
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.settings_button_text = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "SETTINGS");
                }
            }
        }
        {
            // Current
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.current = obj;
            lv_obj_set_pos(obj, 140, 112);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Current");
        }
        {
            lv_obj_t *obj = lv_led_create(parent_obj);
            objects.obj0 = obj;
            lv_obj_set_pos(obj, 27, 41);
            lv_obj_set_size(obj, 32, 32);
            lv_led_set_color(obj, lv_color_hex(0x0000ff));
            lv_led_set_brightness(obj, 255);
        }
        {
            // Status_text_main_page
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.status_text_main_page = obj;
            lv_obj_set_pos(obj, 18, 12);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "STATUS");
        }
        {
            // voltage_label_main_static
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.voltage_label_main_static = obj;
            lv_obj_set_pos(obj, 231, 81);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfefefe), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "VOLTAGE:");
        }
        {
            // current_label_main_static
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.current_label_main_static = obj;
            lv_obj_set_pos(obj, 231, 112);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "CURRENT:");
        }
    }
    
    tick_screen_main();
}

void tick_screen_main() {
}

void create_screen_settings_page() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.settings_page = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // EXIT_SETTINGS
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.exit_settings = obj;
            lv_obj_set_pos(obj, 257, 195);
            lv_obj_set_size(obj, 52, 34);
            lv_obj_add_event_cb(obj, action_exit_to_main_page, LV_EVENT_RELEASED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x7d7d7d), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // Exit_label_settinhs_page
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.exit_label_settinhs_page = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "EXIT");
                }
            }
        }
        {
            // BUZZER
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.buzzer = obj;
            lv_obj_set_pos(obj, 8, 49);
            lv_obj_set_size(obj, 110, 42);
            lv_obj_add_event_cb(obj, action_go_to_buzzer_settings, LV_EVENT_RELEASED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x7d7d7d), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // buzzer_text_settings_page
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.buzzer_text_settings_page = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x090101), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "BUZZER");
                }
            }
        }
        {
            // TOUCH_CALIBRATION
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.touch_calibration = obj;
            lv_obj_set_pos(obj, 8, 109);
            lv_obj_set_size(obj, 110, 41);
            lv_obj_add_event_cb(obj, action_go_to_touch_calibration, LV_EVENT_RELEASED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x7d7d7d), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // Calibration_text_settings_page
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.calibration_text_settings_page = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, " CALIBRATION");
                }
            }
        }
        {
            // VOLTAGE RANGE
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.voltage_range = obj;
            lv_obj_set_pos(obj, 9, 167);
            lv_obj_set_size(obj, 110, 43);
            lv_obj_add_event_cb(obj, action_go_to_v_c_range_settings, LV_EVENT_RELEASED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x7d7d7d), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // vc_range_text_settings_page
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.vc_range_text_settings_page = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "V/C RANGE");
                }
            }
        }
        {
            // Settings_text_settings_page
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.settings_text_settings_page = obj;
            lv_obj_set_pos(obj, 134, 0);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "SETTINGS");
        }
    }
    
    tick_screen_settings_page();
}

void tick_screen_settings_page() {
}

void create_screen_buzzer_settings() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.buzzer_settings = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_add_event_cb(obj, action_go_from_buzzer_settings_page_to_settings_page, LV_EVENT_PRESSED, (void *)0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.obj1 = obj;
            lv_obj_set_pos(obj, 85, 75);
            lv_obj_set_size(obj, 150, LV_SIZE_CONTENT);
            lv_dropdown_set_options_static(obj, "MODE 1\nMODE 2\nMODE 3");
            lv_obj_add_event_cb(obj, event_handler_cb_buzzer_settings_obj1, LV_EVENT_ALL, 0);
        }
        {
            // buzzer_settings_page_label
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.buzzer_settings_page_label = obj;
            lv_obj_set_pos(obj, 94, 9);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xebf900), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "BUZZER SETTINGS");
        }
        {
            // buzzer_settings_page_back_button
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.buzzer_settings_page_back_button = obj;
            lv_obj_set_pos(obj, 16, 189);
            lv_obj_set_size(obj, 78, 41);
            lv_obj_add_event_cb(obj, action_go_from_buzzer_settings_page_to_settings_page, LV_EVENT_RELEASED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x7d7d7d), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // back_text_buzzer_page
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.back_text_buzzer_page = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "BACK");
                }
            }
        }
    }
    
    tick_screen_buzzer_settings();
}

void tick_screen_buzzer_settings() {
    {
        if (!(lv_obj_get_state(objects.obj1) & LV_STATE_EDITED)) {
            int32_t new_val = get_var_buzzer_mode();
            int32_t cur_val = lv_dropdown_get_selected(objects.obj1);
            if (new_val != cur_val) {
                tick_value_change_obj = objects.obj1;
                lv_dropdown_set_selected(objects.obj1, new_val);
                tick_value_change_obj = NULL;
            }
        }
    }
}

void create_screen_v_c_range_settings() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.v_c_range_settings = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x010000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // VC_text_header_vc_page
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.vc_text_header_vc_page = obj;
            lv_obj_set_pos(obj, 47, 2);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfbff00), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "VOLTAGE AND CURRENT SETTINGS");
        }
        {
            // voltage_minimum
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.voltage_minimum = obj;
            lv_obj_set_pos(obj, 97, 59);
            lv_obj_set_size(obj, 150, 10);
            lv_slider_set_range(obj, 0, 30);
            lv_obj_add_event_cb(obj, event_handler_cb_v_c_range_settings_voltage_minimum, LV_EVENT_ALL, 0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // v_min_text
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.v_min_text = obj;
            lv_obj_set_pos(obj, 21, 56);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xf3f3f3), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "V Min");
        }
        {
            // v_max_text
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.v_max_text = obj;
            lv_obj_set_pos(obj, 19, 95);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "V Max");
        }
        {
            // c_min_text
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.c_min_text = obj;
            lv_obj_set_pos(obj, 19, 132);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfbfbfb), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "C Min");
        }
        {
            // c_max_text
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.c_max_text = obj;
            lv_obj_set_pos(obj, 17, 167);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xf4f4f4), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "C Max");
        }
        {
            // voltage_min_value
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.voltage_min_value = obj;
            lv_obj_set_pos(obj, 274, 56);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            // voltage_max_value
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.voltage_max_value = obj;
            lv_obj_set_pos(obj, 274, 93);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            // current_min_value
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.current_min_value = obj;
            lv_obj_set_pos(obj, 278, 129);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            // current_max_value
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.current_max_value = obj;
            lv_obj_set_pos(obj, 280, 167);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfdfdfd), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            // voltage_maximum
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.voltage_maximum = obj;
            lv_obj_set_pos(obj, 97, 101);
            lv_obj_set_size(obj, 150, 10);
            lv_slider_set_range(obj, 0, 30);
            lv_obj_add_event_cb(obj, event_handler_cb_v_c_range_settings_voltage_maximum, LV_EVENT_ALL, 0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // current_minimum
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.current_minimum = obj;
            lv_obj_set_pos(obj, 97, 135);
            lv_obj_set_size(obj, 150, 10);
            lv_slider_set_range(obj, 0, 3);
            lv_obj_add_event_cb(obj, event_handler_cb_v_c_range_settings_current_minimum, LV_EVENT_ALL, 0);
        }
        {
            // current_maximum
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.current_maximum = obj;
            lv_obj_set_pos(obj, 97, 170);
            lv_obj_set_size(obj, 150, 10);
            lv_slider_set_range(obj, 0, 3);
            lv_obj_add_event_cb(obj, event_handler_cb_v_c_range_settings_current_maximum, LV_EVENT_ALL, 0);
        }
        {
            // exit_from_v/c_menu_button
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.exit_from_v_c_menu_button = obj;
            lv_obj_set_pos(obj, 254, 196);
            lv_obj_set_size(obj, 57, 34);
            lv_obj_add_event_cb(obj, action_exit_from_v_c_menu_to_settings, LV_EVENT_RELEASED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x7d7d7d), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xfcfcfc), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // exit_from_v/c_menu_text
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.exit_from_v_c_menu_text = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "EXIT");
                }
            }
        }
    }
    
    tick_screen_v_c_range_settings();
}

void tick_screen_v_c_range_settings() {
    {
        int32_t new_val = get_var_voltage_min();
        int32_t cur_val = lv_slider_get_value(objects.voltage_minimum);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.voltage_minimum;
            lv_slider_set_value(objects.voltage_minimum, new_val, LV_ANIM_ON);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_current_min_text();
        const char *cur_val = lv_label_get_text(objects.voltage_min_value);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.voltage_min_value;
            lv_label_set_text(objects.voltage_min_value, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_voltage_max_text();
        const char *cur_val = lv_label_get_text(objects.voltage_max_value);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.voltage_max_value;
            lv_label_set_text(objects.voltage_max_value, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_current_min_text();
        const char *cur_val = lv_label_get_text(objects.current_min_value);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.current_min_value;
            lv_label_set_text(objects.current_min_value, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_current_max_text();
        const char *cur_val = lv_label_get_text(objects.current_max_value);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.current_max_value;
            lv_label_set_text(objects.current_max_value, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_voltage_max();
        int32_t cur_val = lv_slider_get_value(objects.voltage_maximum);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.voltage_maximum;
            lv_slider_set_value(objects.voltage_maximum, new_val, LV_ANIM_ON);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_current_min();
        int32_t cur_val = lv_slider_get_value(objects.current_minimum);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.current_minimum;
            lv_slider_set_value(objects.current_minimum, new_val, LV_ANIM_ON);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_current_max();
        int32_t cur_val = lv_slider_get_value(objects.current_maximum);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.current_maximum;
            lv_slider_set_value(objects.current_maximum, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
}

typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
    tick_screen_settings_page,
    tick_screen_buzzer_settings,
    tick_screen_v_c_range_settings,
};
void tick_screen(int screen_index) {
    if (screen_index >= 0 && screen_index < 4) {
        tick_screen_funcs[screen_index]();
    }
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen(screenId - 1);
}

//
// Fonts
//

ext_font_desc_t fonts[] = {
#if LV_FONT_MONTSERRAT_8
    { "MONTSERRAT_8", &lv_font_montserrat_8 },
#endif
#if LV_FONT_MONTSERRAT_10
    { "MONTSERRAT_10", &lv_font_montserrat_10 },
#endif
#if LV_FONT_MONTSERRAT_12
    { "MONTSERRAT_12", &lv_font_montserrat_12 },
#endif
#if LV_FONT_MONTSERRAT_14
    { "MONTSERRAT_14", &lv_font_montserrat_14 },
#endif
#if LV_FONT_MONTSERRAT_16
    { "MONTSERRAT_16", &lv_font_montserrat_16 },
#endif
#if LV_FONT_MONTSERRAT_18
    { "MONTSERRAT_18", &lv_font_montserrat_18 },
#endif
#if LV_FONT_MONTSERRAT_20
    { "MONTSERRAT_20", &lv_font_montserrat_20 },
#endif
#if LV_FONT_MONTSERRAT_22
    { "MONTSERRAT_22", &lv_font_montserrat_22 },
#endif
#if LV_FONT_MONTSERRAT_24
    { "MONTSERRAT_24", &lv_font_montserrat_24 },
#endif
#if LV_FONT_MONTSERRAT_26
    { "MONTSERRAT_26", &lv_font_montserrat_26 },
#endif
#if LV_FONT_MONTSERRAT_28
    { "MONTSERRAT_28", &lv_font_montserrat_28 },
#endif
#if LV_FONT_MONTSERRAT_30
    { "MONTSERRAT_30", &lv_font_montserrat_30 },
#endif
#if LV_FONT_MONTSERRAT_32
    { "MONTSERRAT_32", &lv_font_montserrat_32 },
#endif
#if LV_FONT_MONTSERRAT_34
    { "MONTSERRAT_34", &lv_font_montserrat_34 },
#endif
#if LV_FONT_MONTSERRAT_36
    { "MONTSERRAT_36", &lv_font_montserrat_36 },
#endif
#if LV_FONT_MONTSERRAT_38
    { "MONTSERRAT_38", &lv_font_montserrat_38 },
#endif
#if LV_FONT_MONTSERRAT_40
    { "MONTSERRAT_40", &lv_font_montserrat_40 },
#endif
#if LV_FONT_MONTSERRAT_42
    { "MONTSERRAT_42", &lv_font_montserrat_42 },
#endif
#if LV_FONT_MONTSERRAT_44
    { "MONTSERRAT_44", &lv_font_montserrat_44 },
#endif
#if LV_FONT_MONTSERRAT_46
    { "MONTSERRAT_46", &lv_font_montserrat_46 },
#endif
#if LV_FONT_MONTSERRAT_48
    { "MONTSERRAT_48", &lv_font_montserrat_48 },
#endif
};

//
// Color themes
//

uint32_t active_theme_index = 0;

//
//
//

void create_screens() {

// Set default LVGL theme
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    // Initialize screens
    // Create screens
    create_screen_main();
    create_screen_settings_page();
    create_screen_buzzer_settings();
    create_screen_v_c_range_settings();
}