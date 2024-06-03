#ifndef DL_UART_CUSTOM_H
#define DL_UART_CUSTOM_H

// Set up a struct for UART
#include "dl_uart_custom.h"
#include "ti/devices/msp/peripherals/hw_uart.h"
#include <stdint.h>

typedef enum {
    HAL_UART_STATE_READY    = 0x20U,
    HAL_UART_STATE_BUSY_TX  = 0x21U,
} hal_uart_state_t;

typedef struct uart_handle {
    UART_Regs                    *instance;    // point to the base address of UART's registers
    const uint8_t                *pTXBuffPtr;
    uint16_t                     TxXferSize;
    volatile uint16_t            TxXferCount;
    volatile hal_uart_state_t    gState;
    volatile uint32_t            ErrorCode;
    void (*TxISR)(struct uart_handle *huart);
} uart_handle_t;

typedef enum {
    HAL_OK        = 0x00U,
    HAL_ERROR     = 0x01U,
    HAL_BUSY      = 0x02U,
    HAL_TIMEOUT   = 0x03U,
} hal_status_t;

enum uart_error_code {
    HAL_UART_ERROR_NONE        = 0x00U,    // No error
    HAL_UART_ERROR_PE          = 0x01U,    // Parity error
};

// Declare functions
hal_status_t HAL_UART_Transmit(uart_handle_t *huart, const uint8_t *pData, uint16_t size);

#endif // DL_UART_CUSTOM_H