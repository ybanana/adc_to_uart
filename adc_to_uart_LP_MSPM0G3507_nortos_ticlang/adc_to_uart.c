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
#include "ti/devices/msp/m0p/mspm0g350x.h"
#include "ti/driverlib/dl_adc12.h"
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
//#include <stdint.h>
//#include <stdlib.h>

// Show log in Dear ImGui by enabling MCU_LOG.
#define MCU_LOG

// Test some of the functions or not
// #define FUNC_TEST

static struct interrupt_status gCheck;    // static keyword allocates a memory to struct and initialize to 0.

// Create objects in static memory.
static struct message msg;
static conv_voltage_t dc_param;

// Create an object of UART0
uart_handle_t huart0;

// Create objects in static memory.
static dc_converter_t dc_5v;  // Create an object of 5V DC converter.
static dc_converter_t dc_1v8; // Create an object of 1.8V DC converter.
static dc_converter_t dc_3v3; // Create an object of 3.3V DC converter.

// Declaration
static void InitUART0(void);
static void InitDC5V1(void);
static void InitDCConv(dc_converter_t *p_dc_conv, conv_voltage_t *p_dc_param);
//static void LogSent(message_t *p_msg, log_type_t log_type, log_num_t number);
static void PlotDataSent(DL_ADC12_MEM_IDX index);
// Use extern to declare the function in the other .c file if no header file.
extern struct regulator_dev *CreateDC1V2(void);

int main(void) {
    // Initialize the peripherals
    SYSCFG_DL_init();

    // Enable the interrupt of ADC12, UART0, ...
    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);

//    AllocMem4Message(&msg);
    InitUART0();

    // log message created.
    struct log_message *p_log_msg = InitLogMessage();

#ifdef MCU_LOG
    LogMessageSent(p_log_msg, LOG_INFO, MEMORY_ALLOCATED);
    LogMessageSent(p_log_msg, LOG_INFO, UART0_INIT);
#endif

    InitDC5V1();
    InitDCConv(&dc_1v8, &dc_param);
    struct regulator_dev *p_dc_1v2 = CreateDC1V2();
    uint16_t tmp_output_voltage = p_dc_1v2->desc->fixed_mV;

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

/* ================= Implementation ===========================================
 */

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

// Initialize the UART0 peripheral.
void InitUART0(void) {
    // Initialize the object of huart0
    huart0.instance = UART0;
    huart0.gState = HAL_UART_STATE_READY;
}

// Allocate memory for the message.
/*
void AllocMem4Message(message_t *p_msg) {
    // Create an object of log_t struct in static memory.
    static log_t log;
    p_msg->log = &log;

    // Create an object of adc_buffer_t struct in static memory.
    static adc_buffer_t adc_buffer;
    p_msg->adc_buffer = &adc_buffer;
}
*/

/* ================= End of Implementation ===================================
 */

/*---------------------------------- Initialize DC converter
 * ----------------------------*/
static void InitDC5V1(void) {
    dc_5v.cfg->voltage = DC_5V1;
    dc_5v.cfg->ocp_trip = D51_OC_TRIP;
    dc_5v.cfg->ocp_recovery = D51_OC_RECOVERY;
}

static void InitDCConv(dc_converter_t *p_dc_conv, conv_voltage_t *p_dc_param) {
    /* ----- Allocate the memory for cfg and instance within dc_converter_t
     * struct. ----- */
    static conv_config_t config;
    p_dc_conv->cfg = &config;
    static conv_info_t info;
    p_dc_conv->instance = &info;

    // Set the parameters to the converter.
    p_dc_conv->cfg->voltage = *(p_dc_param + 0);
    p_dc_conv->cfg->ocp_trip = *(p_dc_param + 1);
    p_dc_conv->cfg->ocp_recovery = *(p_dc_param + 2);
}
