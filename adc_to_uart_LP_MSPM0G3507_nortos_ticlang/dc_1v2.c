//
// Created by ehuiyue on 5/29/2024.
//
#include "converter.h"

/* --------- Private variables --------- */
static const struct regulator_desc dc_1v2_desc = {
    .name = TPS542A52,
    .fixed_mV = 1200,
    .type = ANALOG_REGULATOR,
};

static struct regulator_supervision dc_1v2_monitor = {
    .output_voltage = 1220,
};

static const struct regulator_ops dc_1v2_ops = {
    .enable = regulator_enable_analog,
    .get_voltage = regulator_get_voltage_by_adc,
};

static const struct regulator_constraints dc_1v2_constraints;

// Initialize 1.2V DC/DC converter by combining all the structures above.
static struct regulator_dev dc_1v2 = {
    .desc = &dc_1v2_desc,    // Initialize the pointer of desc by giving a address of the instance of regulator_desc.
    .constraints = &dc_1v2_constraints,
    .monitor = &dc_1v2_monitor,
    .operations = &dc_1v2_ops,
};

/* --------- Public functions --------- */
// Function to create and initialize the 1.2V DC/DC converter.
struct regulator_dev *CreateDC1V2(void) {
    return &dc_1v2;
}