//
// Created by ehuiyue on 6/13/2024.
//
#include "msg_pool.h"
#include "message_ucd.h"

/* ================================== Private variables for message package ===================== */

static struct message_ucd read_power_system_status_req = {
        .id = READ_POWER_SYSTEM_STATUS_REQ,
        .data = {
                .data_3byte = {
                        .byte0 = PADDING,
                        .byte1 = PADDING,
                        .byte2 = PADDING,
                },
        },
};

static struct msg_info msg_info = {
        .msg_src = MSG_SRC_DC_1V2,
        .msg_dest = MSG_DEST_GUI,
        .msg_protocol = MSG_PROTOCOL_UART,
        .start_byte = 0x01,
        .data = &read_power_system_status_req,
        .end_byte = 0x02,
};

static struct msg_pkg msg_pkg_base = {
        .pkg_no = 0,
        .pkgState = MSG_PKG_UNPROCESSED,
        .info = &msg_info,
        .pkg_len = 0,
};

/* ================== Public API for message package =============================*/
struct msg_pkg *msg_pkg_created(struct message_ucd *ucd_msg) {
    struct msg_pkg *val = &msg_pkg_base;
    val->info->data = ucd_msg;
    val->pkg_len = sizeof(msg_pkg_base);
    return val;
}

/* ======= Private variables for message pool ============= */
#define REGISTER_LIST_SIZE 16                       /* register list could store up to 16 pointers to the message packages. */
uint32_t register_list_array[REGISTER_LIST_SIZE];    /* Static memory for a register list */

/* Allocate a static memory to a register list */
static struct register_list reg_list = {
        .array = register_list_array,
        .write_pkg_idx = register_list_array,
        .read_pkg_idx = register_list_array,
//        .rlState = REGISTER_LIST_EMPTY,
};

#define MESSAGE_POOL_TX_SIZE 512                    /* 512 bytes for the message pool */
uint8_t msg_pool_tx_array[MESSAGE_POOL_TX_SIZE];    /* Static memory for an array */

/* Allocate a static memory to a message pool */
static struct message_pool msg_pool_tx = {
        .array = msg_pool_tx_array,
        .write_index = msg_pool_tx_array,
        .read_index = msg_pool_tx_array,
        .poolState = MESSAGE_POOL_UNLOCKED,
        // .list = &reg_list,
        .write = msg_pkg_enpool,
        .read = msg_pkg_depool,
};

/* ------------- Implementation of Public API ------------------- */
struct message_pool init_msg_pool_tx(void) {
    return msg_pool_tx;
}

void msg_pkg_enpool(struct message_pool *pool, struct msg_pkg *pkg) {
    if (pool->poolState == MESSAGE_POOL_LOCKED) {
        // Give some code to indicate pool is locked by others.
        return;
    } else {
        pool->poolState = MESSAGE_POOL_LOCKED;
    }

    /* Check if there is enough room to contain the package. */
    uint8_t room = available_room(&pool);
    uint8_t pkg_size = sizeof(pkg) + sizeof(pkg->info) + sizeof(pkg->info->data) + sizeof(pkg->info->data->id) + sizeof(pkg->info->data->data);
    uint8_t pkg_size_no_addr = pkg_size - 2*4;
    if (room < pkg_size) {
        // No enough room to take the coming package.
        return;
    } else {
        /* Register a message package into the pool */
        *(pool->write_index) = pkg->pkgState;
        pool->write_index++;
        *(pool->write_index) = pkg->info->msg_src;
        pool->write_index++;
        *(pool->write_index) = pkg->info->msg_dest;
        pool->write_index++;
        *(pool->write_index) = pkg->info->msg_protocol;
        pool->write_index++;
        *(pool->write_index) = pkg->info->start_byte;
        pool->write_index++;
        //    *(pool->write_index) = pkg->info->data;
        pool->write_index++;
        *(pool->write_index) = pkg->info->end_byte;
        pool->write_index++;
        *(pool->write_index) = pkg->pkg_no;
        pool->write_index++;
        *(pool->write_index) = pkg->pkg_len;
        pool->write_index++;

        /* Increment package number fo the next coming package. */
        pkg->pkg_no++;
    };

    /* Unlock the pool once the operation is done. */
    pool->poolState = MESSAGE_POOL_UNLOCKED;
}

void msg_pkg_depool(struct message_pool *pool, struct msg_pkg *pkg) {
    if (pool->poolState == MESSAGE_POOL_LOCKED) {
        // Give some code to indicate pool is locked by others.
        return;
    } else {
        pool->poolState = MESSAGE_POOL_LOCKED;
    }

    /* Release the room that message package occupied. */
    uint8_t pkg_size = sizeof(pkg) + sizeof(pkg->info) + sizeof(pkg->info->data) + sizeof(pkg->info->data->id) + sizeof(pkg->info->data->data);
    uint8_t pkg_size_no_addr = pkg_size - 2*4;
    pool->write_index -= pkg_size_no_addr;

    /* Unlock the pool once the operation is done. */
    pool->poolState = MESSAGE_POOL_UNLOCKED;
}