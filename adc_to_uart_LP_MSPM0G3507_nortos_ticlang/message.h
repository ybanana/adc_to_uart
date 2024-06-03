#ifndef MSG_BUFFER_H
#define MSG_BUFFER_H

#include "message.h"
#include <sys/stdint.h>

/*---------------------------------- buffer -----------------------------------*/
// Set up a struct to store the bytes to be transmitted.
struct buffer {
    uint8_t len;    // maximum length would be 256 Bytes.
//    uint16_t len;   // maximum length would be 64K Bytes.
    uint8_t *p_array;
    uint8_t *write_index;
    uint8_t *read_index;
};

/* --------------------------- message format ------------------------------- */
// enum to indicate the types of messages, say log, plot date
enum message_type {
    MSG_LOG = 0xA1,
    MSG_PLOT_DATA = 0xD1,
};

// enum to indicate the state of the buffer, say empty, full
enum state {
    BUFFER_EMPTY = 0,
    BUFFER_FULL = 1,
};

// Set up a message to send out to PC according to the protocol
struct message {
    enum message_type type;
    struct buffer *buf;
    enum state gState;
};

#endif // MSG_BUFFER_H