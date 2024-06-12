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
#include "devices/msp/m0p/mspm0g350x.h"
#include "driverlib/dl_adc12.h"
#include "driverlib/dl_gpio.h"
#include "driverlib/dl_uart_extend.h"
#include "driverlib/dl_uart_main.h"
#include "driverlib/m0p/dl_core.h"
#include "./syscfg/ti_msp_dl_config.h"
// My libraries
#include "../include/dl_gpio_custom.h"
#include "../include/converter.h"
#include "../include/message_ucd.h"
#include "../include/dl_uart_custom.h"       /* Use customized UART struct */
#include "../include/dl_adc12_custom.h"      /* Use customized ADC12 struct */
#include "../include/flag.h"
#include "../include/log.h"
#include "../include/message_log.h"
#include "../include/message_gui.h"

// Show log in Dear ImGui by enabling MCU_LOG.
#define MCU_LOG

// Test some of the functions or not
// #define FUNC_TEST

#define GUI_PLOT        /* Send the data through UART to GUI for plotting. */

static struct interrupt_status gCheck;    /* static keyword allocates a memory to struct and initialize to 0. */
uart_handle_t huart0;                     /* Create an object of UART0 */
static struct gpio_handle hgpioa;         /* Create an object of GPIOA */
static struct gpio_handle hgpiob;         /* Create an object of GPIOB */
static struct adc_handle hadc0;           /* Create an object of ADC0 */

/* Declaration of functions defined and implemented at the bottom of this C file. */
static void InitUART0(void);
static void InitGPIOA(void);
static void InitGPIOB(void);
static void InitADC0(void);
static void enable_dummy_rail(struct gpio_handle *hgpio, uint32_t pin);
static void send_plot_data(DL_ADC12_MEM_IDX index);

/* Use extern to declare the function in the other .c file if no corresponding header file as an interface. */
extern struct converter CreateDC1V2(void);
extern enum converter_enable_status enable_dc_1v2(struct converter *rdev, struct gpio_handle *hgpio, uint32_t pin);

/* --------------------- Main start ----------------------- */
int main(void) {
    SYSCFG_DL_init();

    /* Enable the interrupt of ADC12, UART0 etc. */
    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);

    /* Initialize the peripherals*/
    InitUART0();
    InitGPIOA();
    InitGPIOB();
    InitADC0();

    struct log_message *p_log_msg = init_log_message();    /* log message created. */

#ifdef MCU_LOG
    send_log_message(p_log_msg, LOG_INFO, MEMORY_ALLOCATED);
    send_log_message(p_log_msg, LOG_INFO, UART0_INIT);
#endif

    struct converter dc_1v2 = CreateDC1V2();    /* New way to initialize the DC converter. */

#ifdef MCU_LOG
    send_log_message(p_log_msg, LOG_INFO, MESSAGE_BUFFER_INIT);
#endif

    init_flag(&gCheck);    /* Initialize interrupt flag */

    const int tmp_cycles = CPUCLK_FREQ * 1 / 1;    /* define a delay time */

    // enable dc_1v2 by GPIO PB27 (see schematic and sysconfig). Would be put in system_paramter.h
    enable_converter_by_pin(&dc_1v2, &hgpiob, DL_GPIO_PIN_27);
    
    // enable dummy_rail by GPIO PA0.
    enable_dummy_rail(&hgpioa, DL_GPIO_PIN_0);

/* -------------------- Super loop -----------------------*/
    while (1) {
        DL_ADC12_startConversion(hadc0.instance);

        wait_ADC_conversion_done(&gCheck);      /* Wait until the ADC conversion is done. */

#ifdef GUI_PLOT
        send_plot_data(DL_ADC12_MEM_IDX_0);    /* Send out the data to GUI for plotting. */
#endif

#ifdef MCU_LOG
        // Send out the log about sending the plotting data done.
        send_log_message(p_log_msg, LOG_INFO, PLOT_DATA_SENT_UART0);
#endif
        delay_cycles(tmp_cycles);    // Insert a delay

        /* Toggle the status of ADC0 interrupt flag. */
        gCheck.toggle = toggle_flag;
        gCheck.toggle(gCheck.adc0);

        DL_ADC12_enableConversions(hadc0.instance);    /* Enable ADC conversion again. */

        disable_converter_by_pin(&dc_1v2, &hgpiob, DL_GPIO_PIN_27);    /* For test. Add a breakpoint here. */
    }
}

/* ============== Interrupt handlers ===================== */
// ADC0 interrupt handler
void ADC12_0_INST_IRQHandler(void) {
    switch (DL_ADC12_getPendingInterrupt(hadc0.instance)) {
    case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
        gCheck.toggle = toggle_flag;
        gCheck.toggle(gCheck.adc0);
        break;
    default:
        break;
    }
}

// UART0 interrupt handler
void UART_0_INST_IRQHandler(void) {
    switch (DL_UART_Main_getPendingInterrupt(huart0.instance)) {
    case DL_UART_MAIN_IIDX_TX:
        // toggle the status of UART0 interrupt flag.
        gCheck.toggle = toggle_flag;
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
void send_plot_data(DL_ADC12_MEM_IDX index) {
    volatile uint16_t gADCResult;
    uint8_t tmp_array[3];
    tmp_array[0] = MSG_PLOT_DATA;
    /*
     *  UART is 8 bits and the ADC result is 16 bits
     *  Split the ADC result into 2 then send via UART.
     */
    gADCResult = DL_ADC12_getMemResult(hadc0.instance, index);
    tmp_array[1] = (uint8_t)((gADCResult >> 8) & 0xFF);
    tmp_array[2] = (uint8_t)(gADCResult & 0xFF);
    // Transmit the bytes in the array
    HAL_UART_Transmit(&huart0, tmp_array, sizeof(tmp_array));
}

/* Initialize UART0 peripheral. */
void InitUART0(void) {
    huart0.instance = UART0;    /* Set the base address of UART0 registers huart0 */
    huart0.gState = HAL_UART_STATE_READY;
}

/* Initialize GPIOA peripheral. */
void InitGPIOA(void) {
    hgpioa.instance = GPIOA;    /* Set the base address of GPIOA registers hgpioa */
}

/* Initialize GPIOB peripheral. */
void InitGPIOB(void) {
    hgpiob.instance = GPIOB;    /* Set the base address of GPIOB registers to hgpiob */
}

/* Initialize ADC12 peripheral. */
void InitADC0(void) {
    // Initialize the object of hgpiob
    hadc0.instance = ADC0;    /* Set the base address of ADC0 registers to hadc0 */
}

// Enable dummy rail
void enable_dummy_rail(struct gpio_handle *hgpio, uint32_t pin) {
    DL_GPIO_clearPins(hgpio->instance, pin);
}