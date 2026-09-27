#include "uart.h"

#include <Arduino.h>
#include <stdio.h>
#include <string.h>

// ==================================================
// UART Buffer Configuration
// ==================================================

#define UART_BUFFER_SIZE 128

static char uart_buffer[
    UART_BUFFER_SIZE
];

static uint16_t uart_index =
    0;

// ==================================================
// Parsed Sensor Values
// ==================================================

static float uart_voltage =
    0.0f;

static float uart_current =
    0.0f;

static bool uart_values_ready =
    false;

// ==================================================
// Last Complete Message
// ==================================================

static char uart_message[
    UART_BUFFER_SIZE
];

static bool uart_message_ready =
    false;

// ==================================================
// Serial Initialization
// ==================================================

extern "C"
{

void serial_init(void)
{
    Serial.begin(
        115200
    );
}

// ==================================================
// Receive and Parse UART Data
// ==================================================

void uart_receive(void)
{
    while (Serial.available())
    {
        char c =
            Serial.read();

        // --------------------------------------------------
        // End of message
        // --------------------------------------------------

        if (
            c == '\n' ||
            c == '\r'
        )
        {
            if (uart_index > 0)
            {
                uart_buffer[
                    uart_index
                ] = '\0';

                // --------------------------------------------------
                // Parse:
                // Voltage,Current
                // Example:
                // 23.75,0.82
                // --------------------------------------------------

                float voltage;
                float current;

                int result =
                    sscanf(
                        uart_buffer,
                        "%f,%f",
                        &voltage,
                        &current
                    );

                if (result == 2)
                {
                    uart_voltage =
                        voltage;

                    uart_current =
                        current;

                    uart_values_ready =
                        true;
                }

                // --------------------------------------------------
                // Store original message if requested
                // --------------------------------------------------

                if (!uart_message_ready)
                {
                    strncpy(
                        uart_message,
                        uart_buffer,
                        UART_BUFFER_SIZE
                    );

                    uart_message[
                        UART_BUFFER_SIZE - 1
                    ] = '\0';

                    uart_message_ready =
                        true;
                }

                // --------------------------------------------------
                // Reset receive buffer
                // --------------------------------------------------

                uart_index =
                    0;
            }
        }

        // --------------------------------------------------
        // Store normal character
        // --------------------------------------------------

        else
        {
            if (
                uart_index <
                UART_BUFFER_SIZE - 1
            )
            {
                uart_buffer[
                    uart_index++
                ] = c;
            }
        }
    }
}

// ==================================================
// Get Last Complete Message
// ==================================================

bool uart_get_message(
    char *buffer,
    uint16_t size
)
{
    if (!uart_message_ready)
    {
        return false;
    }

    if (
        buffer == NULL ||
        size == 0
    )
    {
        return false;
    }

    strncpy(
        buffer,
        uart_message,
        size
    );

    buffer[
        size - 1
    ] = '\0';

    uart_message_ready =
        false;

    return true;
}

// ==================================================
// Get Parsed Voltage and Current
// ==================================================

bool uart_get_values(
    float *voltage,
    float *current
)
{
    if (!uart_values_ready)
    {
        return false;
    }

    if (
        voltage == NULL ||
        current == NULL
    )
    {
        return false;
    }

    *voltage =
        uart_voltage;

    *current =
        uart_current;

    uart_values_ready =
        false;

    return true;
}

}