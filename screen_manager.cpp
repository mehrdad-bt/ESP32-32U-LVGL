#include <Arduino.h>
#include <lvgl.h>

#include "screen_manager.h"

extern "C"
{
#include "ui/screens.h"
}

// ==================================================
// Screen Manager State
// ==================================================

static enum ScreensEnum current_screen =
    SCREEN_ID_MAIN;

static enum ScreensEnum pending_screen =
    SCREEN_ID_MAIN;

static bool screen_change_pending =
    false;

static bool screen_reload_pending =
    false;

// ==================================================
// Get LVGL Object for Screen
// ==================================================

static lv_obj_t *get_screen_object(
    enum ScreensEnum screen
)
{
    switch (screen)
    {
        case SCREEN_ID_MAIN:
            return objects.main;

        case SCREEN_ID_SETTINGS_PAGE:
            return objects.settings_page;

        case SCREEN_ID_BUZZER_SETTINGS:
            return objects.buzzer_settings;

        case SCREEN_ID_V_C_RANGE_SETTINGS:
            return objects.v_c_range_settings;

        default:
            return NULL;
    }
}

// ==================================================
// Load Screen
// ==================================================

static void load_screen_now(
    enum ScreensEnum screen
)
{
    // --------------------------------------------------
    // Get LVGL screen object
    // --------------------------------------------------

    lv_obj_t *screen_obj =
        get_screen_object(screen);

    if (screen_obj == NULL)
    {
        return;
    }

    // --------------------------------------------------
    // Already on requested screen
    // --------------------------------------------------

    if (screen == current_screen)
    {
        return;
    }

    // --------------------------------------------------
    // Load screen
    //
    // IMPORTANT:
    // Do NOT call lv_obj_invalidate()
    // Do NOT call lv_refr_now()
    //
    // LVGL will handle the refresh through
    // lv_timer_handler().
    // --------------------------------------------------

    lv_scr_load(screen_obj);

    // --------------------------------------------------
    // Update current screen state
    // --------------------------------------------------

    current_screen =
        screen;
}

// ==================================================
// Initialize Screen Manager
// ==================================================

void screen_manager_init(void)
{
    current_screen =
        SCREEN_ID_MAIN;

    pending_screen =
        SCREEN_ID_MAIN;

    screen_change_pending =
        false;

    screen_reload_pending =
        false;

    // --------------------------------------------------
    // Get main screen
    // --------------------------------------------------

    lv_obj_t *main_screen =
        objects.main;

    if (main_screen == NULL)
    {
        return;
    }

    // --------------------------------------------------
    // Load initial screen
    //
    // No forced refresh here.
    // lv_timer_handler() in loop()
    // will perform the refresh.
    // --------------------------------------------------

    lv_scr_load(
        main_screen
    );
}

// ==================================================
// Request Screen Change
// ==================================================

void screen_manager_show(
    enum ScreensEnum screen
)
{
    // --------------------------------------------------
    // Ignore invalid screen
    // --------------------------------------------------

    if (get_screen_object(screen) == NULL)
    {
        return;
    }

    // --------------------------------------------------
    // If there is already a request for this screen,
    // there is nothing new to do.
    // --------------------------------------------------

    if (
        screen_change_pending &&
        pending_screen == screen
    )
    {
        return;
    }

    // --------------------------------------------------
    // If already on this screen, don't schedule
    // another screen change.
    // --------------------------------------------------

    if (
        !screen_change_pending &&
        current_screen == screen
    )
    {
        return;
    }

    pending_screen =
        screen;

    screen_change_pending =
        true;

    screen_reload_pending =
        false;
}

// ==================================================
// Request Current Screen Reload
// ==================================================

void screen_manager_reload(void)
{
    // --------------------------------------------------
    // Reload current screen
    // --------------------------------------------------

    pending_screen =
        current_screen;

    screen_change_pending =
        true;

    screen_reload_pending =
        true;
}

// ==================================================
// Process Pending Screen Request
// ==================================================

void screen_manager_process(void)
{
    // --------------------------------------------------
    // Nothing pending
    // --------------------------------------------------

    if (!screen_change_pending)
    {
        return;
    }

    // --------------------------------------------------
    // Copy request locally
    // --------------------------------------------------

    enum ScreensEnum requested_screen =
        pending_screen;

    bool reload =
        screen_reload_pending;

    // --------------------------------------------------
    // Clear pending state immediately
    // --------------------------------------------------

    screen_change_pending =
        false;

    screen_reload_pending =
        false;

    // --------------------------------------------------
    // Same screen without reload
    // --------------------------------------------------

    if (
        !reload &&
        requested_screen ==
        current_screen
    )
    {
        return;
    }

    // --------------------------------------------------
    // Reload current screen
    //
    // lv_scr_load() may not cause the behavior we want
    // when the screen is already active, so explicitly
    // invalidate only in the reload case.
    // --------------------------------------------------

    if (
        reload &&
        requested_screen ==
        current_screen
    )
    {
        lv_obj_t *screen_obj =
            get_screen_object(
                requested_screen
            );

        if (screen_obj == NULL)
        {
            return;
        }

        // Force the screen to be redrawn only
        // when an explicit reload was requested.
        lv_obj_invalidate(
            screen_obj
        );

        return;
    }

    // --------------------------------------------------
    // Normal screen change
    // --------------------------------------------------

    load_screen_now(
        requested_screen
    );
}

// ==================================================
// Get Current Screen
// ==================================================

enum ScreensEnum screen_manager_get(void)
{
    return current_screen;
}

// ==================================================
// Check Current Screen
// ==================================================

bool screen_manager_is(
    enum ScreensEnum screen
)
{
    return (
        current_screen ==
        screen
    );
}