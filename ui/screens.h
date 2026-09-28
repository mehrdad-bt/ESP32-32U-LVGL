#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Screens

enum ScreensEnum {
    _SCREEN_ID_FIRST = 1,
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_SETTINGS_PAGE = 2,
    SCREEN_ID_BUZZER_SETTINGS = 3,
    SCREEN_ID_V_C_RANGE_SETTINGS = 4,
    _SCREEN_ID_LAST = 4
};

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *settings_page;
    lv_obj_t *buzzer_settings;
    lv_obj_t *v_c_range_settings;
    lv_obj_t *voltage;
    lv_obj_t *btn_settings;
    lv_obj_t *settings_button_text;
    lv_obj_t *current;
    lv_obj_t *obj0;
    lv_obj_t *status_text_main_page;
    lv_obj_t *voltage_label_main_static;
    lv_obj_t *current_label_main_static;
    lv_obj_t *error_box;
    lv_obj_t *error_label;
    lv_obj_t *exit_settings;
    lv_obj_t *exit_label_settinhs_page;
    lv_obj_t *buzzer;
    lv_obj_t *buzzer_text_settings_page;
    lv_obj_t *touch_calibration;
    lv_obj_t *calibration_text_settings_page;
    lv_obj_t *voltage_range;
    lv_obj_t *vc_range_text_settings_page;
    lv_obj_t *settings_text_settings_page;
    lv_obj_t *obj1;
    lv_obj_t *buzzer_settings_page_label;
    lv_obj_t *buzzer_settings_page_back_button;
    lv_obj_t *back_text_buzzer_page;
    lv_obj_t *vc_text_header_vc_page;
    lv_obj_t *voltage_minimum;
    lv_obj_t *v_min_text;
    lv_obj_t *v_max_text;
    lv_obj_t *c_min_text;
    lv_obj_t *c_max_text;
    lv_obj_t *voltage_min_value;
    lv_obj_t *voltage_max_value;
    lv_obj_t *current_min_value;
    lv_obj_t *current_max_value;
    lv_obj_t *voltage_maximum;
    lv_obj_t *current_minimum;
    lv_obj_t *current_maximum;
    lv_obj_t *exit_from_v_c_menu_button;
    lv_obj_t *exit_from_v_c_menu_text;
} objects_t;

extern objects_t objects;

void create_screen_main();
void tick_screen_main();

void create_screen_settings_page();
void tick_screen_settings_page();

void create_screen_buzzer_settings();
void tick_screen_buzzer_settings();

void create_screen_v_c_range_settings();
void tick_screen_v_c_range_settings();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/