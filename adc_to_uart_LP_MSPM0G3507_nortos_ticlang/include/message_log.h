//
// Created by ehuiyue on 5/30/2024.
//
#ifndef MESSAGE_LOG_H
#define MESSAGE_LOG_H

#include "message_ucd.h"
#include "log.h"
#include "dl_uart_custom.h"

/* ---------------- Private variables ---------------- */
struct log_message {
    struct message_gui *msg;
    struct log *log;
};

/* ---------------- Public API --------------------------- */

// Initialize a log message.
struct log_message *InitLogMessage(void);

// Send a log message. Return UART status, say HAL_OK if sent successfully.
hal_status_t LogMessageSent(struct log_message *p_log_msg,
                    enum log_type log_type,
                    enum log_num number);

#endif //MESSAGE_LOG_H
