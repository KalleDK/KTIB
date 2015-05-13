/*******************************************************************************
* File Name: SCB_1_SPI_UART_INT.c
* Version 2.0
*
* Description:
*  This file provides the source code to the Interrupt Service Routine for
*  the SCB Component in SPI and UART modes.
*
* Note:
*
********************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SCB_1_PVT.h"
#include "SCB_1_SPI_UART_PVT.h"


/*******************************************************************************
* Function Name: SCB_1_SPI_UART_ISR
********************************************************************************
*
* Summary:
*  Handles the Interrupt Service Routine for the SCB SPI or UART modes.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
CY_ISR(SCB_1_SPI_UART_ISR)
{
#if(SCB_1_INTERNAL_RX_SW_BUFFER_CONST)
    uint32 locHead;
    uint32 dataRx;
#endif /* (SCB_1_INTERNAL_RX_SW_BUFFER_CONST) */

#if(SCB_1_INTERNAL_TX_SW_BUFFER_CONST)
    uint32 locTail;
#endif /* (SCB_1_INTERNAL_TX_SW_BUFFER_CONST) */

    if(NULL != SCB_1_customIntrHandler)
    {
        SCB_1_customIntrHandler();
    }

    #if(SCB_1_CHECK_SPI_WAKE_ENABLE)
    {
        /* Clear SPI wakeup source */
        SCB_1_ClearSpiExtClkInterruptSource(SCB_1_INTR_SPI_EC_WAKE_UP);
    }
    #endif

    #if(SCB_1_CHECK_RX_SW_BUFFER)
    {
        if(SCB_1_CHECK_INTR_RX_MASKED(SCB_1_INTR_RX_NOT_EMPTY))
        {
            while(0u != SCB_1_GET_RX_FIFO_ENTRIES)
            {
                /* Get data from RX FIFO */
                dataRx = SCB_1_RX_FIFO_RD_REG;

                /* Move local head index */
                locHead = (SCB_1_rxBufferHead + 1u);

                /* Adjust local head index */
                if(SCB_1_RX_BUFFER_SIZE == locHead)
                {
                    locHead = 0u;
                }

                if(locHead == SCB_1_rxBufferTail)
                {
                    /* Overflow: through away new data */
                    SCB_1_rxBufferOverflow = (uint8) SCB_1_INTR_RX_OVERFLOW;
                }
                else
                {
                    /* Store received data */
                    SCB_1_PutWordInRxBuffer(locHead, dataRx);

                    /* Move head index */
                    SCB_1_rxBufferHead = locHead;
                }
            }

            SCB_1_ClearRxInterruptSource(SCB_1_INTR_RX_NOT_EMPTY);
        }
    }
    #endif


    #if(SCB_1_CHECK_TX_SW_BUFFER)
    {
        if(SCB_1_CHECK_INTR_TX_MASKED(SCB_1_INTR_TX_NOT_FULL))
        {
            /* Put data into TX FIFO */
            while(SCB_1_SPI_UART_FIFO_SIZE != SCB_1_GET_TX_FIFO_ENTRIES)
            {
                /* Check for room in TX software buffer */
                if(SCB_1_txBufferHead != SCB_1_txBufferTail)
                {
                    /* Move local tail index */
                    locTail = (SCB_1_txBufferTail + 1u);

                    /* Adjust local tail index */
                    if(SCB_1_TX_BUFFER_SIZE == locTail)
                    {
                        locTail = 0u;
                    }

                    /* Put data into TX FIFO */
                    SCB_1_TX_FIFO_WR_REG = SCB_1_GetWordFromTxBuffer(locTail);

                    /* Move tail index */
                    SCB_1_txBufferTail = locTail;
                }
                else
                {
                    /* TX software buffer is empty: complete transfer */
                    SCB_1_DISABLE_INTR_TX(SCB_1_INTR_TX_NOT_FULL);
                    break;
                }
            }

            SCB_1_ClearTxInterruptSource(SCB_1_INTR_TX_NOT_FULL);
        }
    }
    #endif
}


/* [] END OF FILE */
