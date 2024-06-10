//
// Created by ehuiyue on 6/7/2024.
//
#include "buffer.h"

/* Set up a message pool */
#define MESSAGE_POOL_SIZE 64

/* Static memory for an array */
uint8_t msg_array[MESSAGE_POOL_SIZE];

/* Allocate a static memory to a message pool */
static struct buffer msg_pool= {
        .len = sizeof(msg_array),
        .p_array = msg_array,
        .write_index = msg_array,
        .read_index = msg_array,
        .bState = BUFFER_EMPTY,
};

/* ------------- Implementation of Public API ------------------- */
struct buffer msg_pool_init(void) {
    return msg_pool;
}