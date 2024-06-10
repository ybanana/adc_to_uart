//
// Created by ehuiyue on 5/30/2024.
//
#include "message_log.h"
#include "message_gui.h"

// Use the UART0 instance that is already created in adc_to_uart.c.
extern uart_handle_t huart0;

#define LOG_BUFFER_SIZE 64
uint8_t log_array[LOG_BUFFER_SIZE];

/* ---------------- Private variables ---------------- */
// Initialize the message buffer.
static struct buffer log_buffer = {
    .len = LOG_BUFFER_SIZE,
    .p_array = log_array,
    .write_index = log_array,
    .read_index = log_array,
};

// Initialize a message.
static struct message_gui msg = {
    .type = MSG_LOG,
    .buf = &log_buffer,
    .gState = BUFFER_EMPTY,
};

// Initialize a formatted log message.
static struct log log = {
    .type = LOG_INFO,
    .number = NULL_LOG,
};

// Initialize a log message.
static struct log_message log_msg = {
    .msg = &msg,
    .log = &log,
};

/* ---------------- Private functions ---------------- */
// None.

/* ---------------- Public functions ---------------- */
// Create a log message.
struct log_message *InitLogMessage(void) {
    return &log_msg;
}

/*
 * @brief, Send a log with type, say INFO, WARN or ERROR etc., and number,
 * @param, p_log_msg, a pointer or an address of a log message instance.
 * @param, log_type, type of the log, say INFO, WARN, ERROR or FATAL etc.
 * @param, number, see the definition in enum called log_num_t.
 * @retval, uart's status, say HAL_OK, HAL_ERROR, ... .
 * @note, This function only works when UART0 is initialized.
 * @note, no FIFO enable. Send one byte per time by calling HAL_UART_Transmit.
 */
hal_status_t LogMessageSent(struct log_message *p_log_msg,
                    enum log_type log_type,
                    enum log_num number) {
    uint8_t tmp_array[3];
    tmp_array[0] = p_log_msg->msg->type;
    p_log_msg->log->type = log_type;
    tmp_array[1] = p_log_msg->log->type;
    p_log_msg->log->number = number;
    tmp_array[2] = p_log_msg->log->number;
    return HAL_UART_Transmit(&huart0, tmp_array, sizeof(tmp_array));
}