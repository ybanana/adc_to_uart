//
// Created by ehuiyue on 5/28/2024.
//
#include "flag.h"
#include "ti/driverlib/m0p/dl_core.h"
#include <stdint.h>

uint8_t ToggleFlag(uint8_t flag) {
    if (flag == FALSE) {
        flag = TRUE;
        return flag;
    } else {
        flag = FALSE;
        return flag;
    }
}

void InitFlag(struct interrupt_status *p_flag) {
    p_flag->adc0 = FALSE;
    p_flag->uart0 = FALSE;
    p_flag->toggle = ToggleFlag;
}

void WaitADCConversionDone(struct interrupt_status *p_flag) {
    p_flag->toggle = ToggleFlag;
    uint8_t tmp_flag = p_flag->toggle(p_flag->adc0);
    while (FALSE == tmp_flag) {
        // tmp_flag would be updated to TRUE,
        // once the ADC conversion is done, and the program jumps to the interrupt handler.
        __WFE();   // valid with the library of dl_core.h
    }
}