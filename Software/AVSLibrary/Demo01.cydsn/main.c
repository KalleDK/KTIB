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
#include <stdio.h>

void parseRS485()
{
   OEB_MSG_STRUCT msg;
   OEB_GetRxMessage(&msg);
   #if OEB_DEBUG_UART
   OEB_DebugMsg(&msg);
   #endif
   OEB_PutTxMessage(msg.transmitter, 2, 3);
   OEB_PutTxMessageArg(0xAA);
   OEB_PutTxMessageArg(0xFF);
   OEB_ClearRxMessage();
}

void parseRS4852()
{
   MASTER_MSG_STRUCT msg;
   MASTER_GetRxMessage(&msg);
   #if MASTER_DEBUG_UART
   MASTER_DebugMsg(&msg);
   #endif
   MASTER_PutTxMessage(msg.transmitter, 2, 2);
   MASTER_PutTxMessageArg(0xAA);
   MASTER_PutTxMessageArg(0xFF);
   MASTER_ClearRxMessage();
}

int main()
{
    #if OEB_DEBUG_UART
    OEB_DebugInit(UART_PC_PutString);
    MASTER_DebugInit(UART_PC_PutString);
    #endif
    Clock_Start();
    UART_PC_Start();
    MASTER_Start();
    OEB_Start();
    
    CyGlobalIntEnable;

    MASTER_PutTxMessage(3,3,4);
    MASTER_PutTxMessageArg(0xAA);
    MASTER_PutTxMessageArg(0xFF);
    MASTER_PutTxMessageArg(0xDF);
    for(;;)
    {
        CyDelay(1000);
        if (MASTER_ReadRxStatus() == MASTER_MSG_READY)
            parseRS4852();
        if (OEB_ReadRxStatus() == OEB_MSG_READY)
            parseRS485();
        
    }
}

/* [] END OF FILE */
