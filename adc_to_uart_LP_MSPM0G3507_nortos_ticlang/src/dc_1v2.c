//
// Created by ehuiyue on 5/29/2024.
//
#include "../driverlib/dl_gpio.h"
#include "converter.h"
#include "dl_gpio_custom.h"

/* --------- Private variables --------- */
static const struct converter_desc dc_1v2_desc = {
    .name = TPS542A52,
    .fixed_mV = 1200,
    .type = ANALOG_REGULATOR,
};

static struct converter_supervision dc_1v2_monitor = {
    .output_voltage = 1210,    // test value as if the output voltage was detected.
    .ena = DISABLE_BY_EN_PIN,
};

static const struct converter_ops dc_1v2_ops = {
    .enable = analog_conv_enable_active_high,
    .disable = analog_conv_disable_active_high,
    .get_voltage = get_voltage_by_ADC,
};

static const struct converter_constraints dc_1v2_constraints = {
        .oovp_trip = 1250,
        .oovp_recover = 1220,
        .ouvp_trip = 1150,
        .ouvp_recover = 1180,
};

// Initialize 1.2V DC/DC converter by combining all the structures above.
static struct converter dc_1v2 = {
    .desc = &dc_1v2_desc,    // Initialize the pointer of desc by giving an address of the instance of converter_desc.
    .constraints = &dc_1v2_constraints,
    .monitor = &dc_1v2_monitor,
    .operations = &dc_1v2_ops,
};

/* --------- Public functions --------- */
// Function to create and initialize the 1.2V DC/DC converter.
struct converter CreateDC1V2(void) {
    return dc_1v2;
}

