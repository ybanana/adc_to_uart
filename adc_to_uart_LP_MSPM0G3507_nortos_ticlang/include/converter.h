#ifndef CONVERTER_H
#define CONVERTER_H

#include "dl_gpio_custom.h"
#include "dl_adc12_custom.h"

/*
 * @unit: mV or mA
*/

/* -------------------------------- regulator, the way Linux Kernel uses. -----------------------------------------------*/
// Forward declaration
struct converter;

enum regulator_type {
    ANALOG_REGULATOR,
    DIGITAL_REGULATOR,
};

enum regulator_name {
    TPS542A52,
    TPS54321,
    LM5045,
};

enum converter_enable_status {
    // Analog regulator
    DISABLE_BY_EN_PIN,
    ENABLE_BY_EN_PIN,
    // Digital regulator
    DISABLE_BY_PMBUS_COMM,
    ENABLE_BY_PMBUS_COMM,
};

struct converter_supervision {
    uint16_t output_voltage;
    uint16_t input_voltage;
    uint16_t output_current;
    enum converter_enable_status ena;
};

/* struct converter_desc - static regulator description.*/
struct converter_desc {
    enum regulator_name name;
    enum regulator_type type;
    uint16_t fixed_mV;
};

/* struct converter_constraints - */
struct converter_constraints {
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

/* struct converter_ops - regulator operations.*/
struct converter_ops {
    /* get regulator voltage.*/
    uint16_t (*get_voltage)(struct gpio_handle *, uint32_t);
    /* enable/disable regulator.*/
    void (*enable)(struct gpio_handle *, uint32_t);
    void (*disable)(struct gpio_handle *, uint32_t);
    /* report regulator status */
    int (*get_status)(struct converter *rdev);
};

/* struct converter - regulator device.
 * regulator class device. One for each regulator.
 * Every new converter would be built up from this class.
 */
struct converter {
    const struct converter_desc *desc;
    const struct converter_constraints *constraints;
    const struct converter_ops *operations;
    struct converter_supervision *monitor;
};

/* --------------------------------- Public API ------------------------------- */

/*------------------------ Analog regulator ------------------------*/
uint16_t get_voltage_by_ADC(struct gpio_handle *hgpio, uint32_t pin);

// MSPM0 gpio pin tie high to enable the regulator.
void analog_conv_enable_active_high(struct gpio_handle *hgpio, uint32_t pin);
void analog_conv_disable_active_high(struct gpio_handle *hgpio, uint32_t pin);

// MSPM0 gpio pin tie **low** to enable the converter.
void analog_conv_enable_active_low(struct gpio_handle *hgpio, uint32_t pin);
void analog_conv_disable_active_low(struct gpio_handle *hgpio, uint32_t pin);

/* ----------------- Digital regulator with PMBus ----------------- */
int regulator_get_voltage_by_regmap(struct converter *rdev);

// Enable the regulator by PMBus communication.
void regulator_enable_digital(struct converter *rdev);
void regulator_disable_digital(struct converter *rdev);

/*
 * @brief Enable DC/DC converter by setting EN pin of PWM regulator. 
 * @param rdev: pointer to the converter instance.
 * @param hgpio: pointer to the gpio handle.
 * @param pin: pin number to enable the converter.
 * @return: n/a
 * @note: n/a
 * @diagram:   MSPM0            PWM regulator
 *           ----------        -------
 *           |   PBxx |--------| EN  |
 *           |or PAxx |        |     |
 *           ---------         -------
 * @ PBxx could be from PB1 to PB31.
 * @ PAxx from PA1 to PA31.
 */
void enable_converter_by_pin(struct converter *rdev, struct gpio_handle *hgpio, uint32_t pin);
void disable_converter_by_pin(struct converter *rdev, struct gpio_handle *hgpio, uint32_t pin);

#endif // CONVERTER_H
