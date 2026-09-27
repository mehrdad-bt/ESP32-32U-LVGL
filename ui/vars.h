#ifndef EEZ_LVGL_UI_VARS_H
#define EEZ_LVGL_UI_VARS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// enum declarations

// Flow global variables

enum FlowGlobalVariables {
    FLOW_GLOBAL_VARIABLE_VOLTAGE_MIN = 0,
    FLOW_GLOBAL_VARIABLE_VOLTAGE_MAX = 1,
    FLOW_GLOBAL_VARIABLE_CURRENT_MIN = 2,
    FLOW_GLOBAL_VARIABLE_CURRENT_MAX = 3,
    FLOW_GLOBAL_VARIABLE_VOLTAGE_MIN_TEXT = 4,
    FLOW_GLOBAL_VARIABLE_VOLTAGE_MAX_TEXT = 5,
    FLOW_GLOBAL_VARIABLE_CURRENT_MIN_TEXT = 6,
    FLOW_GLOBAL_VARIABLE_CURRENT_MAX_TEXT = 7,
    FLOW_GLOBAL_VARIABLE_BUZZER_MODE = 8
};

// Native global variables

extern float get_var_voltage_min();
extern void set_var_voltage_min(float value);
extern float get_var_voltage_max();
extern void set_var_voltage_max(float value);
extern float get_var_current_min();
extern void set_var_current_min(float value);
extern float get_var_current_max();
extern void set_var_current_max(float value);
extern const char *get_var_voltage_min_text();
extern void set_var_voltage_min_text(const char *value);
extern const char *get_var_voltage_max_text();
extern void set_var_voltage_max_text(const char *value);
extern const char *get_var_current_min_text();
extern void set_var_current_min_text(const char *value);
extern const char *get_var_current_max_text();
extern void set_var_current_max_text(const char *value);
extern int32_t get_var_buzzer_mode();
extern void set_var_buzzer_mode(int32_t value);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/