#include "driverlib/dl_gpio.h"
#include "../include/converter.h" 
#include "../include/dl_gpio_custom.h"
#include <sys/stdint.h>


/* -------------- Implementations of enabling a converter ----------------------- */
/* Active High enable pin */
void analog_conv_enable_active_high(struct gpio_handle *hgpio, uint32_t pin) {
    DL_GPIO_setPins(hgpio->instance, pin);    /* Set pin to high to enable */
}

void analog_conv_disable_active_high(struct gpio_handle *hgpio, uint32_t pin) {
    DL_GPIO_clearPins(hgpio->instance, pin);    /* Set pin to low to disable */
}

/* Active Low enable pin */
void analog_conv_enable_active_low(struct gpio_handle *hgpio, uint32_t pin) {
    DL_GPIO_clearPins(hgpio->instance, pin);    /* Set pin to low to enable */
}

void analog_conv_disable_active_low(struct gpio_handle *hgpio, uint32_t pin) {
    DL_GPIO_setPins(hgpio->instance, pin);    /* Set pin to high to disable */
}

/* Any converter can be enabled by enable pin on converter */
void enable_converter_by_pin(struct converter *rdev, struct gpio_handle *hgpio, uint32_t pin) {
    rdev->operations->enable(hgpio, pin);
}

/* Any converter can be disabled by enable pin on converter */
void disable_converter_by_pin(struct converter *rdev, struct gpio_handle *hgpio, uint32_t pin) {
    rdev->operations->disable(hgpio, pin);
}

/* -------------- Implementations of reading out the output voltage of a converter ----------------------- */
/* Get the sampling data of voltage by ADC */
uint16_t get_voltage_by_ADC(struct gpio_handle *hgpio, uint32_t pin) {

    return 0;
}
/* Read out the output voltage of a converter */
uint16_t readout_converter_output_voltage(struct converter *rdev) {
    return rdev->monitor->output_voltage;
}

/* Read out the voltage by PMBus */
