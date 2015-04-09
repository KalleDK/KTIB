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


#if !defined(`$INSTANCE_NAME`_H_)
#define `$INSTANCE_NAME`_H_

#define `$INSTANCE_NAME`_DEBUG_UART                 (`$RS485_DEBUG`)
    
#include <cytypes.h>
    
#if `$INSTANCE_NAME`_DEBUG_UART
#include <stdio.h>
#endif
/***************************************
*       Type Definitions
***************************************/

#define `$INSTANCE_NAME`_MSG_BUFFER_SIZE            (`$RS485_MSG_SIZE`)

enum {
    `$INSTANCE_NAME`_FS_OFFLINE = 0,
    `$INSTANCE_NAME`_FS_ONLINE = 1,
};

enum {
	`$INSTANCE_NAME`_OE_REQ_NONE = 0,
	`$INSTANCE_NAME`_OE_REQ_FS_DATA,
	`$INSTANCE_NAME`_OE_RES_FS_DATA,
	`$INSTANCE_NAME`_OE_REQ_VENTIL,
	`$INSTANCE_NAME`_OE_RES_VENTIL,
};

enum {
	`$INSTANCE_NAME`_REQ_KAR_NONE = 0,
    `$INSTANCE_NAME`_REQ_KAR_SENSOR_DATA,
	`$INSTANCE_NAME`_RES_KAR_SENSOR_DATA,
    `$INSTANCE_NAME`_REQ_KAR_AKTUATOR_DATA,
	`$INSTANCE_NAME`_RES_KAR_AKTUATOR_DATA,
    `$INSTANCE_NAME`_REQ_KAR_OE_SENSOR_DATA,
    `$INSTANCE_NAME`_RES_KAR_OE_SENSOR_DATA,
	`$INSTANCE_NAME`_REQ_KAR_OE_VENTIL,
    `$INSTANCE_NAME`_RES_KAR_OE_VENTIL,
    `$INSTANCE_NAME`_REQ_KAR_OE_SENSOR_TYPE,
    `$INSTANCE_NAME`_RES_KAR_OE_SENSOR_TYPE,
    `$INSTANCE_NAME`_REQ_KAR_VENTIL,
    `$INSTANCE_NAME`_RES_KAR_VENTIL,
    `$INSTANCE_NAME`_REQ_KAR_OE_LIST,
    `$INSTANCE_NAME`_RES_KAR_OE_LIST,
};

enum {
    `$INSTANCE_NAME`_MSG_RECEIVER_POS = 0,
	`$INSTANCE_NAME`_MSG_TRANSMITTER_POS,
	`$INSTANCE_NAME`_MSG_LEN_POS,
	`$INSTANCE_NAME`_MSG_CMD_POS,
	`$INSTANCE_NAME`_MSG_ARG_START_POS,
};

enum {
    `$INSTANCE_NAME`_MSG_EMPTY = 0,
	`$INSTANCE_NAME`_MSG_RECEIVER,
    `$INSTANCE_NAME`_MSG_TRANSMITTER,
    `$INSTANCE_NAME`_MSG_LEN,
    `$INSTANCE_NAME`_MSG_CMD,
    `$INSTANCE_NAME`_MSG_ARGS,
    `$INSTANCE_NAME`_MSG_READY,
};

typedef struct {
    uint8  receiver;
    uint8  transmitter;
    uint8  len;
    uint8  cmd;
    uint8* args;
} `$INSTANCE_NAME`_MSG_STRUCT;

typedef struct {
    uint8  data[`$INSTANCE_NAME`_MSG_BUFFER_SIZE];
    uint8* head;
    uint8  missing_args;
    uint8  status;
} `$INSTANCE_NAME`_BUFFER_STRUCT;

/***************************************
*        Function Prototypes
***************************************/
    
void  `$INSTANCE_NAME`_Start();

void  `$INSTANCE_NAME`_SetAddress(uint8 addr);

uint8 `$INSTANCE_NAME`_ReadRxStatus();
void  `$INSTANCE_NAME`_GetRxMessage(`$INSTANCE_NAME`_MSG_STRUCT *msg);
void  `$INSTANCE_NAME`_ClearRxMessage();

void  `$INSTANCE_NAME`_PutTxMessage(uint8 receiver, uint8 len, uint8 cmd);
void  `$INSTANCE_NAME`_PutTxMessageArg(uint8 arg);

#if `$INSTANCE_NAME`_DEBUG_UART
    void `$INSTANCE_NAME`_DebugInit(void (*UART_PutString)(const char8 string[]));
    void `$INSTANCE_NAME`_DebugMsg(`$INSTANCE_NAME`_MSG_STRUCT *msg);
#endif

#endif

/* [] END OF FILE */
