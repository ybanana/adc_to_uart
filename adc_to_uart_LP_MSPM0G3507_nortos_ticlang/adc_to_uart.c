/*
 * Copyright (c) 2023, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// TI's API
#include "dl_gpio_custom.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"
#include "ti/driverlib/dl_adc12.h"
#include "ti/driverlib/dl_gpio.h"
#include "ti/driverlib/dl_uart_extend.h"
#include "ti/driverlib/dl_uart_main.h"
#include "ti/driverlib/m0p/dl_core.h"
#include "ti_msp_dl_config.h"
// My libraries
#include "converter.h"
#include "message.h"
#include "dl_uart_custom.h"   // Use customized UART struct
#include "flag.h"
#include "log.h"
#include "message_log.h"
// Standard C libraries
// #include <stdint.h>
// #include <stdlib.h>

// Show log in Dear ImGui by enabling MCU_LOG.
#define MCU_LOG

// Test some of the functions or not
// #define FUNC_TEST

static struct interrupt_status gCheck;    // static keyword allocates a memory to struct and initialize to 0.

// Create an object of UART0
uart_handle_t huart0;

// Create an object of GPIOA
struct gpio_handle hgpioa;

// Create an object of GPIOB
struct gpio_handle hgpiob;

// Declaration
static void InitUART0(void);
static void InitGPIOA(void);
static void InitGPIOB(void);
static void enable_dummy_rail(struct gpio_handle *hgpio, uint32_t pin);

//static void LogSent(message_t *p_msg, log_type_t log_type, log_num_t number);
static void PlotDataSent(DL_ADC12_MEM_IDX index);

// Use extern to declare the function in the other .c file if no header file.
extern struct regulator_dev CreateDC1V2(void);
extern void enable_dc_1v2(struct regulator_dev *rdev, struct gpio_handle *hgpio, uint32_t pin);

// Main start
int main(void) {
    // Initialize the peripherals
    SYSCFG_DL_init();

    // Enable the interrupt of ADC12, UART0, ...
    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);

    InitUART0();
    InitGPIOA();
    InitGPIOB();

    // log message created.
    struct log_message *p_log_msg = InitLogMessage();

#ifdef MCU_LOG
    LogMessageSent(p_log_msg, LOG_INFO, MEMORY_ALLOCATED);
    LogMessageSent(p_log_msg, LOG_INFO, UART0_INIT);
#endif

    // New way to initialize the DC converter. 
    struct regulator_dev dc_1v2 = CreateDC1V2();

#ifdef FUNC_TEST
    // Test read functions
    dc_5v.read_func = read_voltage;
    uint16_t tmp_vol_read = dc_5v.read_func(&dc_5v);
#endif

#ifdef MCU_LOG
    LogMessageSent(p_log_msg, LOG_INFO, MESSAGE_BUFFER_INIT);
#endif

    // Initialize interrupt flag
    InitFlag(&gCheck);

    // delay time
    const int tmp_cycles = CPUCLK_FREQ * 1 / 1;

    // test HAL_UART_Transmit function
#ifdef FUNC_TEST
    uint8_t tmp_array[5] = {0x11, 0x22, 0x33, 0xAA, 0xDD};
    // Send out 5 bytes
    hal_status_t tmp_status;
    tmp_status = HAL_UART_Transmit(&huart0, tmp_array, sizeof(tmp_array));
#endif

    // enable dc_1v2 by GPIO PB27 (see schematic and sysconfig). Would be put in system_paramter.h
    enable_dc_1v2(&dc_1v2, &hgpiob, DL_GPIO_PIN_27);
    
    // enable dummy_rail by GPIO PA0.
    enable_dummy_rail(&hgpioa, DL_GPIO_PIN_0);

    /* --------------- Super loop -----------------*/
    while (1) {
        DL_ADC12_startConversion(ADC0);

        // Wait until the ADC conversion is done.
        WaitADCConversionDone(&gCheck);

        // Send out the data to GUI for plotting.
        PlotDataSent(DL_ADC12_MEM_IDX_0);

        // Send out the log about sending the plotting data done.
#ifdef MCU_LOG
        LogMessageSent(p_log_msg, LOG_INFO, PLOT_DATA_SENT_UART0);
#endif
        // Add a delay
        delay_cycles(tmp_cycles);

        // Toggle the status of ADC0 interrupt flag.
        gCheck.toggle = ToggleFlag;
        gCheck.toggle(gCheck.adc0);

        // Enable ADC conversion again.
        DL_ADC12_enableConversions(ADC0);
    }
}

/* ============== Interrupt handlers ===================== */
// ADC0 interrupt handler
void ADC12_0_INST_IRQHandler(void) {
    switch (DL_ADC12_getPendingInterrupt(ADC0)) {
    case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
        gCheck.toggle = ToggleFlag;
        gCheck.toggle(gCheck.adc0);
        break;
    default:
        break;
    }
}

// UART0 interrupt handler
void UART_0_INST_IRQHandler(void) {
    switch (DL_UART_Main_getPendingInterrupt(UART0)) {
    case DL_UART_MAIN_IIDX_TX:
        // toggle the status of UART0 interrupt flag.
        gCheck.toggle = ToggleFlag;
        gCheck.toggle(gCheck.uart0);
        break;
    default:
        break;
    }
}
/* ============== End of Interrupt handlers ============== */

/*
 * @brief, Read out the result of ADC conversion. Send out the high byte and low
 * byte in order with D1 as a message type.
 * @param,
 * -------- p_msg, a pointer or an address of a message instance.
 * -------- index, (0 - 11), choose which ADC12 Memory conversion would be used.
 * @retval, none.
 * @note, no FIFO enable. Send one byte per time by calling HAL_UART_Transmit.
 */
void PlotDataSent(DL_ADC12_MEM_IDX index) {
    volatile uint16_t gADCResult;
    uint8_t tmp_array[3];
    tmp_array[0] = MSG_PLOT_DATA;
    /*
     *  UART is 8 bits and the ADC result is 16 bits
     *  Split the ADC result into 2 then send via UART.
     */
    gADCResult = DL_ADC12_getMemResult(ADC0, index);
    tmp_array[1] = (uint8_t)((gADCResult >> 8) & 0xFF);
    tmp_array[2] = (uint8_t)(gADCResult & 0xFF);
    // Transmit the bytes in the array
    HAL_UART_Transmit(&huart0, tmp_array, sizeof(tmp_array));
}

// Initialize UART0 peripheral.
void InitUART0(void) {
    // Initialize the object of huart0
    huart0.instance = UART0;
    huart0.gState = HAL_UART_STATE_READY;
}

// Initialize GPIOA peripheral.
void InitGPIOA(void) {
    // Initialize the object of hgpioa
    hgpioa.instance = GPIOA;
}

// Initialize GPIOB peripheral.
void InitGPIOB(void) {
    // Initialize the object of hgpiob
    hgpiob.instance = GPIOB;
}

// Enable dummy rail
void enable_dummy_rail(struct gpio_handle *hgpio, uint32_t pin) {
    DL_GPIO_clearPins(hgpio->instance, pin);
}