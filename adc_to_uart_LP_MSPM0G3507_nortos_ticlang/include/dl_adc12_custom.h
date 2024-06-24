//
// Created by ehuiyue on 6/10/2024.
//

#ifndef DL_ADC_CUSTOM_H
#define DL_ADC_CUSTOM_H

#include "../devices/msp/peripherals/hw_adc12.h"

/* ---------- Private Variables ------------ */
enum adc_state {
    ADC_STATE_RESET = 0x00U,    /* ADC is not yet initialized or disabled */
    ADC_STATE_READY = 0x01U,    /* ADC is initialized and ready for use */
    ADC_STATE_BUSY = 0x02U,    /* ADC is busy with a conversion */
    ADC_STATE_ERROR = 0x03U,    /* ADC error state */
};

struct adc_handle {
    ADC12_Regs *instance;
    enum adc_state gState;
};

/* ------------- Public API -----------------*/

#endif //DL_ADC_CUSTOM_H
