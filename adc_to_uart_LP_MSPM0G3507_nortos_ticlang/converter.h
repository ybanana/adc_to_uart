#ifndef CONVERTER_H
#define CONVERTER_H

#include <stdint.h>

/*
 * @unit: mV or mA
*/
typedef enum {
    /* ----- 5.1V ----------------------------------- */
    DC_5V1                = 5100,
    D51_OC_TRIP           = 24000,
    D51_OC_RECOVERY       = 21000,
    /* ----- 3.3V ----------------------------------- */
    DC_3V3                = 3300,
    D33_OC_TRIP           = 6000,
    D33_OC_RECOVERY       = 4000,
    /* ----- 1.8V ----------------------------------- */
    DC_1V8                = 1800,
    D18_OC_TRIP           = 5000,
    D18_OC_RECOVERY       = 3500,
} conv_voltage_t;

typedef struct {
    conv_voltage_t voltage;
    uint16_t ocp_trip;
    uint16_t ocp_recovery;
} conv_config_t;

typedef struct {
    uint16_t voltage;
    uint16_t current;
    uint16_t address;
} conv_info_t;

typedef struct dc_converter {
    conv_info_t *instance;
    conv_config_t *cfg;
    void (*set_func)(struct dc_converter* self);
    uint16_t (*read_func)(struct dc_converter* self);
} dc_converter_t;

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

enum regulator_enable {
    ENABLE_BY_EN_PIN,
    ENABLE_BY_PMBUS_COMM,
};

struct regulator_supervision {
    uint16_t output_voltage;
    uint16_t input_voltage;
    uint16_t output_current;
};

/* struct regulator_desc - static regulator description.*/
struct regulator_desc {
    enum regulator_name name;
    enum regulator_type type;
    uint16_t fixed_mV;
    enum regulator_enable ena;
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
    void (*enable)(struct regulator_dev *);
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

// Declaration
uint16_t read_voltage(dc_converter_t* self); 
uint16_t read_current(dc_converter_t* self);

//void InitDC1V2(struct regulator_dev *rdev);

/* ----- interfaces to access the regulator device. ----- */

// TODO: Use uint16_t instead of int if the data range is 0-65535 to cover the range and resolution.
// Analog regulator
uint16_t regulator_get_voltage_by_adc(struct regulator_dev *rdev);
// Digital regulator with PMBus
int regulator_get_voltage_by_regmap(struct regulator_dev *rdev);
// For analog regulator, MSPM0 gpio pin tie high to enable the regulator.
void regulator_enable_analog(struct regulator_dev *rdev);
void regulator_disable_analog(struct regulator_dev *rdev);

#endif // CONVERTER_H
