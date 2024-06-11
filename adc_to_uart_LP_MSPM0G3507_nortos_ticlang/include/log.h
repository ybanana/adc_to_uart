//
// Created by ehuiyue on 5/30/2024.
//
#ifndef LOG_H
#define LOG_H

/*--------------------------------- log format -------------------------*/
// enum to indicate the type of log
enum log_type {
    LOG_INFO     = 0x00U,
    LOG_WARN     = 0x01U,
    LOG_ERROR    = 0x02U,
    LOG_FATAL    = 0x03U,
};

// enum to indicate the data of log
enum log_num {
    NULL_LOG               = 0x00,
    MEMORY_ALLOCATED       = 0xA2,
    UART0_INIT             = 0xB0,
    MESSAGE_BUFFER_INIT    = 0xB1,
    INTERRUPT_INIT         = 0xB2,
    PERIPHERAL_INIT        = 0xB3,
    MSG_SENT_UART0         = 0xC1,    // 0xC1 indicates message sent out throught UART for instance.
    PLOT_DATA_SENT_UART0   = 0xC2,
};

// Set up a struct to store the log data
struct log {
    enum log_type type;    // type consists of info (0xA1), warn (0xB1), error (0xC1) etc.
    enum log_num number;
};
/*---------------------------------------------------------------------*/

#endif //LOG_H
