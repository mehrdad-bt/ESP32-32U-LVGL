#include "actions.h"
#include "screens.h"

#include "screen_manager.h"
#include "touchCalibration.h"
#include "tasks.h"

// ==================================================
// MAIN -> SETTINGS
// ==================================================

void action_go_to_settings_page(lv_event_t *e)
{
    (void)e;

    screen_manager_show(
        SCREEN_ID_SETTINGS_PAGE
    );
}

// ==================================================
// SETTINGS -> MAIN
// ==================================================

void action_exit_to_main_page(lv_event_t *e)
{
    (void)e;

    screen_manager_show(
        SCREEN_ID_MAIN
    );
}

// ==================================================
// SETTINGS -> BUZZER
// ==================================================

void action_go_to_buzzer_settings(lv_event_t *e)
{
    (void)e;

    screen_manager_show(
        SCREEN_ID_BUZZER_SETTINGS
    );
}

// ==================================================
// SETTINGS -> TOUCH CALIBRATION
// ==================================================

void action_go_to_touch_calibration(lv_event_t *e)
{
    (void)e;

    touch_calibration_start();

    screen_manager_reload();
}

// ==================================================
// SETTINGS -> V/C RANGE
// ==================================================

void action_go_to_v_c_range_settings(lv_event_t *e)
{
    (void)e;

    screen_manager_show(
        SCREEN_ID_V_C_RANGE_SETTINGS
    );
}

// ==================================================
// BUZZER -> SETTINGS
// ==================================================

void action_go_from_buzzer_settings_page_to_settings_page(
    lv_event_t *e
)
{
    (void)e;

    screen_manager_show(
        SCREEN_ID_SETTINGS_PAGE
    );
}

// ==================================================
// V/C RANGE -> SETTINGS
// ==================================================

void action_exit_from_v_c_menu_to_settings(
    lv_event_t *e
)
{
    (void)e;

    screen_manager_show(
        SCREEN_ID_SETTINGS_PAGE
    );
}

// ==================================================
// VOLTAGE MIN
// ==================================================

void action_voltage_min_changed(lv_event_t *e)
{
    (void)e;

    if (objects.voltage_minimum == NULL)
    {
        return;
    }

    int32_t value =
        lv_slider_get_value(
            objects.voltage_minimum
        );

    set_var_voltage_min(
        (float)value
    );
}

// ==================================================
// VOLTAGE MAX
// ==================================================

void action_voltage_max_changed(lv_event_t *e)
{
    (void)e;

    if (objects.voltage_maximum == NULL)
    {
        return;
    }

    int32_t value =
        lv_slider_get_value(
            objects.voltage_maximum
        );

    set_var_voltage_max(
        (float)value
    );
}

// ==================================================
// CURRENT MIN
// ==================================================

void action_current_min_changed(lv_event_t *e)
{
    (void)e;

    if (objects.current_minimum == NULL)
    {
        return;
    }

    int32_t value =
        lv_slider_get_value(
            objects.current_minimum
        );

    set_var_current_min(
        (float)value
    );
}

// ==================================================
// CURRENT MAX
// ==================================================

void action_current_max_changed(lv_event_t *e)
{
    (void)e;

    if (objects.current_maximum == NULL)
    {
        return;
    }

    int32_t value =
        lv_slider_get_value(
            objects.current_maximum
        );

    set_var_current_max(
        (float)value
    );
}