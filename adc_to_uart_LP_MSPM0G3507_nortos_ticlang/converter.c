#include "converter.h" 
#include <stdint.h>

uint16_t regulator_get_voltage_by_adc(struct regulator_dev *rdev) {
    return rdev->monitor->output_voltage;
}

void regulator_enable_analog(struct regulator_dev *rdev) {
    // Enable the regulator by the pin xx.
}
