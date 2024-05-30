//
// Created by ehuiyue on 5/30/2024.
//
#ifndef MESSAGE_LOG_H
#define MESSAGE_LOG_H

#include "message.h"
#include "log.h"
#include "dl_uart_custom.h"

struct log_message {
    struct message *msg;
    struct log *log;
};

// Public API
struct log_message *InitLogMessage(void);
hal_status_t LogMessageSent(struct log_message *p_log_msg,
                    enum log_type log_type,
                    enum log_num number);

#endif //MESSAGE_LOG_H
