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
void (*`$INSTANCE_NAME`_DEBUG_PUTSTRING)(const char8 string[]);
char8 `$INSTANCE_NAME`_debug_msg[50];

#define \
    `$INSTANCE_NAME`_DEBUG(...) \
    ({ \
        sprintf(`$INSTANCE_NAME`_debug_msg, __VA_ARGS__); \
        `$INSTANCE_NAME`_DEBUG_PUTSTRING(`$INSTANCE_NAME`_debug_msg); \
    })

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
    
    #if `$INSTANCE_NAME`_DEBUG_UART
    `$INSTANCE_NAME`_DEBUG("\r\n\r\n");
    #endif
    
    `$INSTANCE_NAME`_reading_buffer = `$INSTANCE_NAME`_buffer_array;
    `$INSTANCE_NAME`_receiving_buffer = `$INSTANCE_NAME`_buffer_array;
    `$INSTANCE_NAME`_ClearRxMessage();
    
    #if `$INSTANCE_NAME`_DEBUG_UART
    `$INSTANCE_NAME`_DEBUG("Initializing: Done\r\n\r\n");
    `$INSTANCE_NAME`_DEBUG("\r\nPress 'h' for help\r\n");
    #endif
        
}

void  `$INSTANCE_NAME`_SetAddress(uint8 addr)
{
    `$INSTANCE_NAME`_address = addr;
    `$INSTANCE_NAME`_UART_UartSetRxAddress(addr);
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
    `$INSTANCE_NAME`_DEBUG("Message Cleared\r\n\r\n");
    #endif
}

void  `$INSTANCE_NAME`_PutTxMessage(uint8 receiver, uint8 len, uint8 cmd)
{
	//Set specialbit to mark receiver
	//`$INSTANCE_NAME`_UART_SpiUartWriteTxData(receiver);
    `$INSTANCE_NAME`_UART_SpiUartWriteTxData(receiver | `$INSTANCE_NAME`_UART_UART_MP_MARK );
    `$INSTANCE_NAME`_UART_SpiUartWriteTxData(`$INSTANCE_NAME`_address);
	`$INSTANCE_NAME`_UART_SpiUartWriteTxData(len);
	`$INSTANCE_NAME`_UART_SpiUartWriteTxData(cmd);
    
    #if `$INSTANCE_NAME`_DEBUG_UART
    `$INSTANCE_NAME`_DEBUG("TX: R 0x%02X, L %d, C 0x%02X\r\n", receiver, len, cmd);
    #endif
    
}

void  `$INSTANCE_NAME`_PutTxMessageArg(uint8 arg)
{
    `$INSTANCE_NAME`_UART_SpiUartWriteTxData(arg);
}

#if `$INSTANCE_NAME`_DEBUG_UART
void `$INSTANCE_NAME`_DebugInit(void (*UART_PutString)(const char8 string[]))
{
    `$INSTANCE_NAME`_DEBUG_PUTSTRING = UART_PutString;
}

void `$INSTANCE_NAME`_DebugMsg(`$INSTANCE_NAME`_MSG_STRUCT *msg)
{
    int i;
    `$INSTANCE_NAME`_DEBUG("Receiver:\t0x%02X\r\n", msg->receiver);
    `$INSTANCE_NAME`_DEBUG("Transmitter:\t0x%02X\r\n", msg->transmitter);
    `$INSTANCE_NAME`_DEBUG("Length:\t\t%d\r\n", msg->len);
    `$INSTANCE_NAME`_DEBUG("Command:\t0x%02X\r\n", msg->cmd);
    `$INSTANCE_NAME`_DEBUG("Arguments:\t");
    for(i = 0; i < msg->len; ++i) {
        `$INSTANCE_NAME`_DEBUG("0x%02X ", msg->args[i]);
    }
    `$INSTANCE_NAME`_DEBUG("\r\n\r\n");
}
#endif

/* [] END OF FILE */
