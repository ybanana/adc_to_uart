//
// Created by ehuiyue on 6/13/2024.
//

#ifndef MSG_POOL_TX_H
#define MSG_POOL_TX_H

#include "message_ucd.h"
#include <stdint.h>

/* ==================================== Private enum and strcut =================================== */

/* ----------------------------------- message package ------------------------------- */
/* Where the message comes from. */
enum message_source {
    MSG_SRC_ADC = 0x11,
    MSG_SRC_DAC = 0x12,
    MSG_SRC_DC_1V2 = 0x13,
    MSG_SRC_DC_3V3 = 0x14,
};

/* Where the message goes for. */
enum message_destination {
    MSG_DEST_GUI = 0x21,
    MSG_DEST_UCD = 0x22,
    MSG_DEST_SYSTEM = 0x23,
};

/* What protocol message is transmitted through, say UART, PMBus and SPI etc. */
enum message_protocol {
    MSG_PROTOCOL_UART = 0x01,
    MSG_PROTOCOL_PMBUS = 0x02,
    MSG_PROTOCOL_SPI = 0x03,
};

struct msg_data {
    uint8_t data01;    /* data that would be sent */
    uint8_t data02;    /* data that would be sent */
};

/* Combine all the elements above into a struct collects message information */
struct msg_info {
    const enum message_source msg_src;            /* source of a message */
    const enum message_destination msg_dest;      /* destination of a message */
    const enum message_protocol msg_protocol;     /* protocol of a message, say UART, PMBus, SPI etc. */
    const uint8_t start_byte;    /* start byte of a message package */
    struct message_ucd *data;       /* only data that would be sent */
    const uint8_t end_byte;      /* end byte of a message package */
};

/*----------------------------- message package --------------------------------------*/
/* whether the message package was processed or not */
enum message_pkg_state {
    MSG_PKG_UNPROCESSED = 0xE2U,
    MSG_PKG_PROCESSED = 0xF2U,
};

/* Encapsule all relevant elements in a message into a package which would be stored in a pool. */
struct msg_pkg {
    uint8_t pkg_no;                /* package number */
    enum message_pkg_state pkgState;
    struct msg_info *info;         /* base address of a message information */
    uint8_t pkg_len;               /* length (bytes) of a message package */
};

/* ----------------------------- register list -------------------------------------*/
/* TODO: register list is really needed? */
enum register_list_state {
    REGISTER_LIST_EMPTY = 0xE0U,
    REGISTER_LIST_FULL = 0xF0U,
};

/* register list to record the base address of a message package */
struct register_list {
    uint32_t *array;            /* base address of the register list */
    uint32_t *write_pkg_idx;    /* write index of the message package */
    uint32_t *read_pkg_idx;     /* read index of the message package */
    enum register_list_state rlState;    /* state of the register list */
};

/* ------------------------------- message pool -------------------------------------*/
enum message_pool_state {
    MESSAGE_POOL_EMPTY = 0xE1U,
    MESSAGE_POOL_FULL = 0xF1U,
    MESSAGE_POOL_LOCKED = 0xC0U,
    MESSAGE_POOL_UNLOCKED = 0xC1U,
};

/* When allocating a pool,
 * its register list is also allocated.
 * its operations as function pointers declared
 * */
struct message_pool {
    uint8_t *array;                     /* point to the message pool */
    uint8_t *write_index;               /* write index of the message pool */
    uint8_t *read_index;                /* read index of the message pool */
    enum message_pool_state poolState;    /* the state of message pool */
    // struct register_list *list;         /* point to a register list for the pool */
    void (*write)(struct message_pool *pool, struct msg_pkg *pkg);    /* function pointer to enqueue a message package into the pool */
    void (*read)(struct message_pool *pool, struct msg_pkg *pkg);    /* function pointer to dequeue a message package into the pool */
};

/* ======================================================== Public API ======================================================== */
struct message_pool init_msg_pool_tx(void);
void msg_pkg_enpool(struct message_pool *pool, struct msg_pkg *pkg);
void msg_pkg_depool(struct message_pool *pool, struct msg_pkg *pkg);

struct msg_pkg *msg_pkg_created(struct message_ucd *ucd_msg); 

#endif //MSG_POOL_TX_H