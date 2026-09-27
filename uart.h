#ifndef UART_MODULE_H
#define UART_MODULE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif


void serial_init(void);

void uart_receive(void);


bool uart_get_message(
    char *buffer,
    uint16_t size
);


bool uart_get_values(
    float *voltage,
    float *current
);


#ifdef __cplusplus
}
#endif

#endif