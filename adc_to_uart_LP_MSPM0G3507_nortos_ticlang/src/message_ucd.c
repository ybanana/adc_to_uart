//
// Created by ehuiyue on 6/7/2024.
//

#include "message_ucd.h"

static struct message_ucd read_power_system_status_req = {
        .id = READ_POWER_SYSTEM_STATUS_REQ,
        .data = {
                .data_3byte = {
                        .byte0 = PADDING,
                        .byte1 = PADDING,
                        .byte2 = PADDING,
                },
        },
};

static struct message_ucd read_power_rail_temp_req = {
        .id = READ_POWER_RAIL_TEMP_REQ,
        .data = {
                .data_1byte_2byte = {
                        .byte0 = 0,        /* Rail ID */
                        .byte12 = PADDING_2BYTE
                },
        },
};

static struct message_ucd read_power_rail_temp_cfm = {
        .id = READ_POWER_RAIL_TEMP_CFM,
        .data = {
                .data_1byte_2byte = {
                        .byte0 = 0,        /* Rail ID */
                        .byte12 = PADDING_2BYTE
                },
        },
};

static struct message_ucd read_power_rail_voltage_rej = {
        .id = READ_POWER_RAIL_VOLTAGE_REJ,
        .data = {
                .data_3byte = {
                        .byte0 = 0,        /* Rail ID */
                        .byte1 = PADDING,
                        .byte2 = 0,
                },
        },
};

static struct message_ucd set_power_rail_voltage_req = {
        .id = SET_POWER_RAIL_VOLTAGE_REQ,
        .data = {
                .data_1byte_2byte = {
                        .byte0 = 0,        /* Rail ID */
                        .byte12 = 0,        /* Voltage Setpoint */
                },
        },
};

static struct message_ucd set_power_rail_voltage_ext_a_req = {
        .id = SET_POWER_RAIL_VOLTAGE_EXT_A_REQ,
        .data = {
                .data_3byte = {
                        .byte0 = 0,        /* Rail ID */
                        .byte1 = 0,        /* Voltage Setpoint Byte 0*/
                        .byte2 = 0,        /* CRC-8 */
                },
        },
};

static struct message_ucd set_power_rail_voltage_ext_b_req = {
        .id = SET_POWER_RAIL_VOLTAGE_EXT_B_REQ,
        .data = {
                .data_3byte = {
                        .byte0 = 0,        /* Rail ID */
                        .byte1 = 0,        /* Voltage Setpoint Byte 1*/
                        .byte2 = 0,        /* CRC-8 */
                },
        },
};

/* --------------------------- Public API ------------------------------- */
// Temp location for CalcCRC8
uint8_t CalcCRC8(uint8_t data) {
    // Calculate CRC-8
    return 0;
}

struct message_ucd MSG_SetVoltageExtA(uint8_t rail_id, uint8_t voltage_setpoint_byte0) {
    // Define a temporary message
    struct message_ucd msg = set_power_rail_voltage_ext_a_req;
    msg.data.data_3byte.byte0 = rail_id;
    msg.data.data_3byte.byte1 = voltage_setpoint_byte0;
    msg.data.data_3byte.byte2 = CalcCRC8(msg.data.data_3byte.byte1); // CRC-8
    return msg;
}

struct message_ucd ucd_msg_read_power_system_status(void) {
    struct message_ucd msg = read_power_system_status_req;
    return msg;
}