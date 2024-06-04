#ifndef CONVERTER_H
#define CONVERTER_H

#include <stdint.h>

/*
 * @unit: mV or mA
*/

/* -------------------------------- regulator, the way Linux Kernel uses. -----------------------------------------------*/
// Forward declaration
struct regulator_dev;

enum regulator_type {
    ANALOG_REGULATOR,
    DIGITAL_REGULATOR,
};

enum regulator_name {
    TPS542A52,
    TPS54321,
    LM5045,
};

enum regulator_enable_status {
    // Analog regulator
    DISABLE_BY_EN_PIN,
    ENABLE_BY_EN_PIN,
    // Digital regulator
    DISABLE_BY_PMBUS_COMM,
    ENABLE_BY_PMBUS_COMM,
};

struct regulator_supervision {
    uint16_t output_voltage;
    uint16_t input_voltage;
    uint16_t output_current;
    enum regulator_enable_status ena;
};

/* struct regulator_desc - static regulator description.*/
struct regulator_desc {
    enum regulator_name name;
    enum regulator_type type;
    uint16_t fixed_mV;
};

/* struct regulator_constraints - */
struct regulator_constraints {
    // output protection
    uint16_t oovp_trip;
    uint16_t oovp_recover;
    uint16_t ouvp_trip;
    uint16_t ouvp_recover;
    uint16_t oocp_trip;
    uint16_t oocp_recover;
    // temperature protection
    uint16_t otp_trip;
    uint16_t otp_recover;
    // input protection
    uint16_t iovp_trip;
    uint16_t iovp_recover;
    uint16_t iuvp_trip;
    uint16_t iuvp_recover;
};

/* struct regulator_ops - regulator operations.*/
struct regulator_ops {
    /* get regulator voltage.*/
    uint16_t (*get_voltage)(struct regulator_dev *);
    /* enable/disable regulator.*/
    void (*enable)(struct regulator_dev *, uint8_t *);
    void (*disable)(struct regulator_dev *);
    /* report regulator status */
    int (*get_status)(struct regulator_dev *rdev);
};

/* struct regulator_dev - regulator device.
 * regulator class device. One for each regulator.
 * Every new converter would be built up from this class.
 */
struct regulator_dev {
    const struct regulator_desc *desc;
    const struct regulator_constraints *constraints;
    const struct regulator_ops *operations;
    struct regulator_supervision *monitor;
};

/* --------------------------------- Public API ------------------------------- */

/*------------------------ Analog regulator ------------------------*/
uint16_t regulator_get_voltage_by_adc(struct regulator_dev *rdev);

// MSPM0 gpio pin tie high to enable the regulator.
void regulator_enable_analog(struct regulator_dev *rdev, uint8_t *pin);
void regulator_disable_analog(struct regulator_dev *rdev);

/* ----------------- Digital regulator with PMBus ----------------- */
int regulator_get_voltage_by_regmap(struct regulator_dev *rdev);

// Enable the regulator by PMBus communication.
void regulator_enable_digital(struct regulator_dev *rdev);
void regulator_disable_digital(struct regulator_dev *rdev);

#endif // CONVERTER_H
