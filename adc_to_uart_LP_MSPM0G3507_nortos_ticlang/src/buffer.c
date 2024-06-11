//
// Created by ehuiyue on 6/7/2024.
//
#include "../include/buffer.h"

#define MESSAGE_POOL_SIZE 64    /* Set up a message pool */
#define ADC_RESULT_POOL_SIZE 32    /* size of adc result pool */

uint8_t msg_array[MESSAGE_POOL_SIZE];    /* Static memory for an array */
uint8_t adc0_ch0_array[ADC_RESULT_POOL_SIZE];    /* static memory for ADC0 channel0 array */

/* Allocate a static memory to a message pool */
static struct buffer msg_pool= {
        .len = sizeof(msg_array),
        .p_array = msg_array,
        .write_index = msg_array,
        .read_index = msg_array,
        .bState = BUFFER_EMPTY,
};

/* Allocate a static memory to a adc result pool */
static struct buffer adc0_ch0_pool= {
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

struct buffer adc0_ch0_pool_init(void) {
    return adc0_ch0_pool;
}