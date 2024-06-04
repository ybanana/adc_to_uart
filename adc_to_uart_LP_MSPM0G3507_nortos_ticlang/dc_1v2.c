//
// Created by ehuiyue on 5/29/2024.
//
#include "ti/driverlib/dl_gpio.h"
#include "converter.h"
#include "dl_gpio_custom.h"

/* --------- Private variables --------- */
static const struct regulator_desc dc_1v2_desc = {
    .name = TPS542A52,
    .fixed_mV = 1200,
    .type = ANALOG_REGULATOR,
};

static struct regulator_supervision dc_1v2_monitor = {
    .output_voltage = 1210,    // test value as if the output voltage was detected.
    .ena = DISABLE_BY_EN_PIN,
};

static const struct regulator_ops dc_1v2_ops = {
    .enable = regulator_enable_analog,
    .get_voltage = regulator_get_voltage_by_adc,
};

static const struct regulator_constraints dc_1v2_constraints = {
        .oovp_trip = 1250,
        .oovp_recover = 1220,
        .ouvp_trip = 1150,
        .ouvp_recover = 1180,
};

// Initialize 1.2V DC/DC converter by combining all the structures above.
static struct regulator_dev dc_1v2 = {
    .desc = &dc_1v2_desc,    // Initialize the pointer of desc by giving an address of the instance of regulator_desc.
    .constraints = &dc_1v2_constraints,
    .monitor = &dc_1v2_monitor,
    .operations = &dc_1v2_ops,
};

/* --------- Public functions --------- */
// Function to create and initialize the 1.2V DC/DC converter.
struct regulator_dev CreateDC1V2(void) {
    return dc_1v2;
}

// Function to enable DC_1V2
void enable_dc_1v2(struct regulator_dev *rdev, struct gpio_handle *hgpio, uint32_t pin) {
    // Set pin to high to enable
    DL_GPIO_setPins(hgpio->instance, pin);
    // Set the DC converter status to Enabled.
    rdev->monitor->ena = ENABLE_BY_EN_PIN;
}