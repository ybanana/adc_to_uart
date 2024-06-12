#ifndef MESSAGE_UCD_H
#define MESSAGE_UCD_H

#include "buffer.h"

/* Message constant for UCD communication protocol */
enum msg_constant_ucd {
    PADDING = 0x00,
    PADDING_2BYTE = 0x0000,
};

/* struct data - data to be sent out
 * union for the data that vary their length.
 */
struct data_varying_length {
    union {
        uint8_t data_1byte;          /* 1 byte data */

        uint16_t data_2byte;         /* 2 bytes data */

        struct {
            uint8_t byte0;           /* 3 separated bytes data */
            uint8_t byte1;
            uint8_t byte2;
        } data_3byte;

        struct {
            uint8_t byte0;           /* 1 bytes data */
            uint16_t byte12;         /* 2 bytes data */
        } data_1byte_2byte;

        struct {
            uint16_t byte01;         /* 2 bytes data */
            uint8_t byte2;           /* 1 bytes data */
        } data_2byte_1byte;
        /* More bytes if needed */
    };
};

enum message_id {
    SET_POWER_RAIL_STATE_REQ = 0x01,
    SET_POWER_RAIL_STATE_CFM = 0x02,
    SET_POWER_RAIL_STATE_REJ = 0x03,
    READ_POWER_RAIL_STATE_REQ = 0x04,
    READ_POWER_RAIL_STATE_CFM = 0x05,
    READ_POWER_RAIL_STATE_REJ = 0x06,
    SET_POWER_RAIL_VOLTAGE_REQ = 0x07,
    SET_POWER_RAIL_VOLTAGE_CFM = 0x08,
    SET_POWER_RAIL_VOLTAGE_REJ = 0x09,
    READ_POWER_RAIL_VOLTAGE_REQ = 0x0A,
    READ_POWER_RAIL_VOLTAGE_CFM = 0x0B,
    READ_POWER_RAIL_VOLTAGE_REJ = 0x0C,
    READ_POWER_RAIL_CURRENT_REQ = 0x0D,
    READ_POWER_RAIL_CURRENT_CFM = 0x0E,
    READ_POWER_RAIL_CURRENT_REJ = 0x0F,
    READ_POWER_RAIL_POWER_REQ = 0x10,
    READ_POWER_RAIL_POWER_CFM = 0x11,
    READ_POWER_RAIL_POWER_REJ = 0x12,
    READ_POWER_RAIL_TEMP_REQ = 0x13,
    READ_POWER_RAIL_TEMP_CFM = 0x14,
    READ_POWER_RAIL_TEMP_REJ = 0x15,
    READ_POWER_RAIL_ALARM_REQ = 0x16,
    READ_POWER_RAIL_ALARM_CFM = 0x17,
    READ_POWER_RAIL_ALARM_REJ = 0x18,
    SET_POWER_RAIL_STATE_EXT_REQ = 0x21,
    SET_POWER_RAIL_STATE_EXT_CFM = 0x22,
    SET_POWER_RAIL_STATE_EXT_REJ = 0x23,
    SET_POWER_RAIL_VOLTAGE_EXT_A_REQ = 0x24,
    SET_POWER_RAIL_VOLTAGE_EXT_A_CFM = 0x25,
    SET_POWER_RAIL_VOLTAGE_EXT_A_REJ = 0x26,
    SET_POWER_RAIL_VOLTAGE_EXT_B_REQ = 0x27,
    SET_POWER_RAIL_VOLTAGE_EXT_B_CFM = 0x28,
    SET_POWER_RAIL_VOLTAGE_EXT_B_REJ = 0x29,
    SET_POWER_FAULT_LOG_STATE_EXT_REQ = 0x2A,
    SET_POWER_FAULT_LOG_STATE_EXT_CFM = 0x2B,
    SET_POWER_FAULT_LOG_STATE_EXT_REJ = 0x2C,
    READ_POWER_SYSTEM_STATUS_REQ = 0x31,
    READ_POWER_SYSTEM_STATUS_CFM = 0x32,
    READ_POWER_SYSTEM_STATUS_REJ = 0x33,
};

// A struct of a message to send out to any device according to the protocol
struct message_ucd {
    enum message_id id;
    struct data_varying_length data;
};

#endif // MESSAGE_UCD_H