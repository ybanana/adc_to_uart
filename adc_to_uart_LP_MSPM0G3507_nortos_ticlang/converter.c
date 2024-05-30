#include "converter.h" 
#include <stdint.h>

uint16_t read_voltage(dc_converter_t* self) {
    return self->instance->voltage;
}

uint16_t read_current(dc_converter_t* self) {
    return self->instance->current;
}

uint16_t regulator_get_voltage_by_adc(struct regulator_dev *rdev) {
    return rdev->monitor->output_voltage;
}

void regulator_enable_analog(struct regulator_dev *rdev) {
    // Enable the regulator by the pin xx.
}
