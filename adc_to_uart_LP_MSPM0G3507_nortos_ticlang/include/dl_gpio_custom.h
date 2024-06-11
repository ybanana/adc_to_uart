//
// Created by ehuiyue on 6/4/2024.
//
#ifndef DL_GPIO_CUSTOM_H
#define DL_GPIO_CUSTOM_H

#include "../devices/msp/peripherals/hw_gpio.h"

/* ---------- Private Variables ------------ */
struct gpio_handle {
    GPIO_Regs *instance;
};

/* ------------- Public API -----------------*/

#endif // DL_GPIO_CUSTOM_H
