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

CY_ISR_PROTO(UART_Received);

char debug_msg[50];

int main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    char rxData = 0;
    CyGlobalIntEnable;  /* Uncomment this line to enable global interrupts. */
    Clock_UART_Start();
    isr_UART_StartEx(UART_Received);
    UART_Start();
    UART_PC_Start();
    
    for(;;)
    {
        rxData = UART_PC_UartGetChar();
        if(rxData)
        {
            UART_PutChar(rxData);
            rxData = 0;
        }
        CyDelay(200);
    }
}

CY_ISR(UART_Received){
    char returnME = UART_GetChar();
    sprintf(debug_msg, "Received: %c\r\n", returnME);
    UART_PC_UartPutString(debug_msg);
    if(returnME == 'r'){
        Pin_R_Write(!Pin_R_Read());
    }
}

/* [] END OF FILE */
