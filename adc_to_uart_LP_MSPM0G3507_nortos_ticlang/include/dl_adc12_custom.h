//
// Created by ehuiyue on 6/10/2024.
//

#ifndef DL_ADC_CUSTOM_H
#define DL_ADC_CUSTOM_H

#include "../devices/msp/peripherals/hw_adc12.h"

/* ---------- Private Variables ------------ */
struct adc_handle {
    ADC12_Regs *instance;
};

/* ------------- Public API -----------------*/

#endif //DL_ADC_CUSTOM_H
