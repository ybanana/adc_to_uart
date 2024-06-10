//
// Created by ehuiyue on 6/7/2024.
//

#ifndef BUFFER_H
#define BUFFER_H

#include <sys/stdint.h>

/* State of the buffer */
enum state {
    BUFFER_EMPTY = 0x0E,
    BUFFER_FULL = 0x0F,
};

/* General buffer (pool) */
struct buffer {
    uint16_t len;           /* maximum length would be 64K Bytes. */
    uint8_t *p_array;       /* pointer to the array of the buffer */
    uint8_t *write_index;   /* pointer to the location in the buffer to write */
    uint8_t *read_index;    /* pointer to the location in the buffer to read */
    enum state bState;      /* state of the buffer */
};

/* ------------- Public API ------------------- */
struct buffer msg_pool_init(void);

#endif //BUFFER_H
