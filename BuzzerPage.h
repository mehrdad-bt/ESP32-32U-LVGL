#ifndef BUZZER_PAGE_H
#define BUZZER_PAGE_H

#include <stdint.h>

void buzzer_page_init(void);
void buzzer_page_update(void);

void buzzer_page_handle_right(void);
void buzzer_page_handle_select(void);

void buzzer_page_runtime_task(
    bool data_received,
    bool connection_lost,
    bool low_voltage
);

void buzzer_set_mode(uint8_t mode);
uint8_t buzzer_get_mode(void);

#endif