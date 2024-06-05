#include "converter.h" 
#include "dl_gpio_custom.h"
#include <stdint.h>
#include "ti/driverlib/dl_gpio.h"

uint16_t regulator_get_voltage_by_adc(struct regulator_dev *rdev) {
    return rdev->monitor->output_voltage;
}

void analog_conv_enable_active_high(struct regulator_dev *rdev, struct gpio_handle *hgpio, uint32_t pin) {
    // Set pin to high to enable
    DL_GPIO_setPins(hgpio->instance, pin);
    // Set the DC converter status to Enabled.
    rdev->monitor->ena = ENABLE_BY_EN_PIN;
}