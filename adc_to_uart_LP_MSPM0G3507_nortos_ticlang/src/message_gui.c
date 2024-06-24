#include "message_gui.h"

// Set up a buffer to store the ADC data.
#define ADC_BUFFER_SIZE 128
uint8_t adc_array[ADC_BUFFER_SIZE];

/* ---------------- Private variables ---------------- */
// Initialize the message buffer.
static struct buffer adc_buffer = {
    .len = ADC_BUFFER_SIZE,
    .p_array = adc_array,
    .write_index = adc_array,
    .read_index = adc_array,
};

// Initialize a ADC message.
static struct message_gui adc_msg = {
    .type = MSG_PLOT_DATA,
    .buf = &adc_buffer,
    .gState = BUFFER_EMPTY,
};

/* ---------------- Private functions ---------------- */
// None.

/* ---------------- Public functions ---------------- */
// Create an ADC message.
struct message_gui init_ADC_message(void) {
    return adc_msg;
}