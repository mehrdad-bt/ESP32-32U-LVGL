#ifndef TASKS_H
#define TASKS_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

void tasks_init(void);
void tasks_run(void);

float tasks_get_voltage(void);
float tasks_get_current(void);

bool tasks_data_received(void);
bool tasks_connection_lost(void);
bool tasks_low_voltage(void);

#ifdef __cplusplus
}
#endif

#endif