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


CY_ISR_PROTO(UART_Received);

int main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    CyGlobalIntEnable;  /* Uncomment this line to enable global interrupts. */
    Clock_UART_Start();
    isr_UART_StartEx(UART_Received);
    UART_Start();
    
    for(;;)
    {
        /* Place your application code here. */
        if(!Pin_PB_Read()){
            UART_PutChar('r');
            while(!Pin_PB_Read());
        }
    }
}

CY_ISR(UART_Received){
    char returnME = UART_GetChar();
    if(returnME == 'r'){
        Pin_R_Write(!Pin_R_Read());
    }
    if(returnME == 'g'){
        Pin_G_Write(!Pin_G_Read());
    }   
    if(returnME == 'b'){
        Pin_B_Write(!Pin_B_Read());
    }
    //Pin_R_Write(!Pin_R_Read());
}

/* [] END OF FILE */

