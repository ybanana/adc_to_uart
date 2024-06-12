//
// Created by ehuiyue on 5/28/2024.
//
#ifndef FLAG_H
#define FLAG_H

#include <sys/stdint.h>

enum interrupt_flag {
    FALSE = 0,
    TRUE = 1,
};

// struct for interrupt status
struct interrupt_status {
    uint8_t adc0;
    uint8_t uart0;
    uint8_t (*toggle)(uint8_t flag);
};

// Declaration
uint8_t toggle_flag(uint8_t flag);
void init_flag(struct interrupt_status *p_flag);
void wait_ADC_conversion_done(struct interrupt_status *p_flag);

#endif //FLAG_H
