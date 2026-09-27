#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include "ui/screens.h"

void screen_manager_init(void);

void screen_manager_show(
    enum ScreensEnum screen
);

void screen_manager_process(void);

void screen_manager_reload(void);

enum ScreensEnum screen_manager_get(void);

bool screen_manager_is(
    enum ScreensEnum screen
);

#ifdef __cplusplus
}
#endif

#endif