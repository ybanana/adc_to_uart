//
// Created by ehuiyue on 6/10/2024.
//

#ifndef MESSAGE_ADC_H
#define MESSAGE_ADC_H

#include "buffer.h"

/* --------------------------- message format to GUI ------------------------------- */
// enum to indicate the types of messages, say log, plot date
enum message_type {
    MSG_LOG = 0xA1,
    MSG_PLOT_DATA = 0xD1,
};

struct message_gui {
    uint8_t type;
    struct buffer *buf;
    enum state gState;
};

/* ---------------- Public functions ---------------- */
// Create an ADC message.
struct message_gui init_ADC_message(void);

#endif // MESSAGE_ADC_H
