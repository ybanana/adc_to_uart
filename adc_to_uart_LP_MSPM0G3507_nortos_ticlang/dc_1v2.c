//
// Created by ehuiyue on 5/29/2024.
//
#include "ti/driverlib/dl_gpio.h"
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
    .get_voltage = regulator_get_voltage_by_adc,
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

/*
 * @brief Enable 1.2V DC/DC converter by setting the pin to high.
 * @param rdev: pointer to the converter instance.
 * @param hgpio: pointer to the gpio handle.
 * @param pin: pin number to enable the converter.
 * @return: the status of the converter.
 * @note: n/a
 * @diagram:   MSPM0            PWM regulator
 * @         ----------        -------
 *           |   PBxx |--------| EN  |
 *           |        |        |     |
 *           ---------         -------
 * @ PBxx could be from PB1 to PB31.
 */
enum converter_enable_status enable_dc_1v2(struct converter *rdev, struct gpio_handle *hgpio, uint32_t pin) {
    rdev->operations->enable(rdev, hgpio, pin);
    return rdev->monitor->ena;
}