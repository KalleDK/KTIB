/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>

//Genreal Variables
uint8  `$INSTANCE_NAME`_address;

//Debug variables
#if `$INSTANCE_NAME`_DEBUG_UART
#include "..\..\Includes\avs_debug.h"
#endif

//Rx Variables

`$INSTANCE_NAME`_BUFFER_STRUCT  `$INSTANCE_NAME`_buffer_array[1];
`$INSTANCE_NAME`_BUFFER_STRUCT* `$INSTANCE_NAME`_receiving_buffer;
`$INSTANCE_NAME`_BUFFER_STRUCT* `$INSTANCE_NAME`_reading_buffer;


CY_ISR_PROTO(`$INSTANCE_NAME`_RX485);

CY_ISR(`$INSTANCE_NAME`_RX485)
{
    if (`$INSTANCE_NAME`_receiving_buffer->status == `$INSTANCE_NAME`_MSG_EMPTY) {
        `$INSTANCE_NAME`_receiving_buffer->head = `$INSTANCE_NAME`_receiving_buffer->data;
         ++`$INSTANCE_NAME`_receiving_buffer->status;
    }
    
    *(`$INSTANCE_NAME`_receiving_buffer->head) = `$INSTANCE_NAME`_UART_UartGetChar();
    ++`$INSTANCE_NAME`_receiving_buffer->head;
    
    switch(`$INSTANCE_NAME`_receiving_buffer->status)
    {            
	    case `$INSTANCE_NAME`_MSG_RECEIVER:
        case `$INSTANCE_NAME`_MSG_TRANSMITTER:
        case `$INSTANCE_NAME`_MSG_LEN:
            ++`$INSTANCE_NAME`_receiving_buffer->status;
            break;
        case `$INSTANCE_NAME`_MSG_CMD:
            ++`$INSTANCE_NAME`_receiving_buffer->status;
            `$INSTANCE_NAME`_receiving_buffer->missing_args = `$INSTANCE_NAME`_receiving_buffer->data[`$INSTANCE_NAME`_MSG_LEN_POS];
            break;
        case `$INSTANCE_NAME`_MSG_ARGS:
            --`$INSTANCE_NAME`_receiving_buffer->missing_args;
            break;
        default:
            break;
    }
    
    if (`$INSTANCE_NAME`_receiving_buffer->status == `$INSTANCE_NAME`_MSG_ARGS && `$INSTANCE_NAME`_receiving_buffer->missing_args == 0) {
	    `$INSTANCE_NAME`_receiving_buffer->status = `$INSTANCE_NAME`_MSG_READY;
    }
    
    `$INSTANCE_NAME`_UART_ClearRxInterruptSource(`$INSTANCE_NAME`_UART_INTR_RX_NOT_EMPTY);

}

void `$INSTANCE_NAME`_Start()
{
    `$INSTANCE_NAME`_UART_Start();
    `$INSTANCE_NAME`_UART_isr_StartEx(`$INSTANCE_NAME`_RX485);
    `$INSTANCE_NAME`_SetAddress(`$RS485_ADDRESS`);
    

    
    `$INSTANCE_NAME`_reading_buffer = `$INSTANCE_NAME`_buffer_array;
    `$INSTANCE_NAME`_receiving_buffer = `$INSTANCE_NAME`_buffer_array;
    `$INSTANCE_NAME`_ClearRxMessage();
    
    #if `$INSTANCE_NAME`_DEBUG_UART
    printf("`$INSTANCE_NAME` up and running\n\r\n\r");
    #endif
        
}

void  `$INSTANCE_NAME`_SetAddress(uint8 addr)
{
    `$INSTANCE_NAME`_address = addr;
    `$INSTANCE_NAME`_UART_UartSetRxAddress(addr);
}

uint8  `$INSTANCE_NAME`_GetAddress()
{
    return `$INSTANCE_NAME`_address;
}

uint8 `$INSTANCE_NAME`_ReadRxStatus()
{    
    return `$INSTANCE_NAME`_reading_buffer->status;
}

void  `$INSTANCE_NAME`_GetRxMessage(`$INSTANCE_NAME`_MSG_STRUCT *msg)
{
    msg->receiver = `$INSTANCE_NAME`_reading_buffer->data[`$INSTANCE_NAME`_MSG_RECEIVER_POS];
    msg->transmitter = `$INSTANCE_NAME`_reading_buffer->data[`$INSTANCE_NAME`_MSG_TRANSMITTER_POS];
    msg->len = `$INSTANCE_NAME`_reading_buffer->data[`$INSTANCE_NAME`_MSG_LEN_POS];
    msg->cmd = `$INSTANCE_NAME`_reading_buffer->data[`$INSTANCE_NAME`_MSG_CMD_POS];
    msg->args = &`$INSTANCE_NAME`_reading_buffer->data[`$INSTANCE_NAME`_MSG_ARG_START_POS];
}

void `$INSTANCE_NAME`_ClearRxMessage()
{
    `$INSTANCE_NAME`_receiving_buffer->head = `$INSTANCE_NAME`_receiving_buffer->data;
    `$INSTANCE_NAME`_reading_buffer->head = `$INSTANCE_NAME`_reading_buffer->data;
    `$INSTANCE_NAME`_reading_buffer->missing_args = 0;
    `$INSTANCE_NAME`_reading_buffer->status = `$INSTANCE_NAME`_MSG_EMPTY;
    #if `$INSTANCE_NAME`_DEBUG_UART
    printf("Message Cleared\n\r\n\r");
    #endif
}

void  `$INSTANCE_NAME`_PutTxMessage(uint8 receiver, uint8 len, uint8 cmd)
{
    `$INSTANCE_NAME`_UART_SpiUartWriteTxData(receiver | `$INSTANCE_NAME`_UART_UART_MP_MARK );
    `$INSTANCE_NAME`_UART_SpiUartWriteTxData(`$INSTANCE_NAME`_address);
	`$INSTANCE_NAME`_UART_SpiUartWriteTxData(len);
	`$INSTANCE_NAME`_UART_SpiUartWriteTxData(cmd);
    
    #if `$INSTANCE_NAME`_DEBUG_UART
    printf("TX: R 0x%02X, L %d, C 0x%02X\n\r", receiver, len, cmd);
    #endif
    
}

void  `$INSTANCE_NAME`_PutTxMessageArg(uint8 arg)
{
    `$INSTANCE_NAME`_UART_SpiUartWriteTxData(arg);
}

#if `$INSTANCE_NAME`_DEBUG_UART

    void  `$INSTANCE_NAME`_DebugHandle(const char ch)
    {
        switch(ch)
        {
            case 27:
                printf("`$INSTANCE_NAME`");
                break;
            case '?':
                printf("\n\r`$INSTANCE_NAME`\n\r\n\r");
                printf("Nothing Yet\n\r");
                break;
        }
    }


void `$INSTANCE_NAME`_DebugMsg(`$INSTANCE_NAME`_MSG_STRUCT *msg)
{
    int i;
    printf("Receiver:\t0x%02X\n\r", msg->receiver);
    printf("Transmitter:\t0x%02X\n\r", msg->transmitter);
    printf("Length:\t\t%d\n\r", msg->len);
    printf("Command:\t0x%02X\n\r", msg->cmd);
    printf("Arguments:\t");
    for(i = 0; i < msg->len; ++i) {
        printf("0x%02X ", msg->args[i]);
    }
    printf("\n\r\n\r");
}
#endif

/* [] END OF FILE */
