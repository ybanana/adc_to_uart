#include "../driverlib/dl_uart_main.h"
#include "../driverlib/dl_uart_extend.h"
#include "../driverlib/m0p/dl_core.h"
#include "dl_uart_custom.h"
#include <stdlib.h>

/**
  * @brief Send an amount of data.
  * @note
  * @param huart    UART handle
  * @param pData    Pointer to data buffer (u8 data elements)
  * @param Size     Amount of data elements (u8) to be sent
  * @todo Timeout  Timeout duration
  * @retval HAL status
*/
hal_status_t HAL_UART_Transmit(uart_handle_t *huart, const uint8_t *pData, uint16_t size) {
    const uint8_t *pdata8bits;
    // Check that a Tx process is not already ongoing
    if (huart->gState == HAL_UART_STATE_READY) {
        if ((pData == NULL) || (size == 0U)) {
            return HAL_ERROR;
        }

        huart->ErrorCode = HAL_UART_ERROR_NONE;
        huart->gState = HAL_UART_STATE_BUSY_TX;    // lock UART unaccessible for now.

        // Init tickstart for timeout management
        // tickstart = HAL_GetTick();

        huart->TxXferSize = size;
        huart->TxXferCount = size;

        // assign a pointer to the first place of a array for 8bits data
        pdata8bits = pData;

        while (huart->TxXferCount > 0U) {
            huart->instance->TXDATA = (uint8_t)(*pdata8bits & 0xFFU);
            pdata8bits++;
            huart->TxXferCount--;
            // if TX FIFO is not empty, wait for the data to be sent out.
            while (false == DL_UART_Main_isTXFIFOEmpty(huart->instance)) {
                __WFE();
            }
        }

        huart->gState = HAL_UART_STATE_READY;    // unlock UART to be accessible again.

        return HAL_OK;
    }
    else {
        return HAL_BUSY;
    }
}