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

// Definitioner
uint8 RESET = 1;
uint8 START = 0;
int   flow_frequency10;
int   LHour = 0;
int   i;

// ISR prototype
CY_ISR_PROTO(counter);

// Interrupt service routine
CY_ISR(counter)
{
    Pin_MR1_Write(RESET);
    CyDelay(10);
    Pin_MR1_Write(START);
    flow_frequency10 += 10;
    
    // test
    
    Pin_test_LED_Write(0);
    CyDelay(50);
    Pin_test_LED_Write(1);
    
}


int main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    isr_1_StartEx(counter);

    /* Enable global interrupts */
    CyGlobalIntEnable;
    
    while(1)
    { 
        for( i=0; i < 60; i++ )
         {
            LHour = flow_frequency10 * 60 / 7.5;     // antal L/time
            CyDelay(1000);
         }
    }
}

/* [] END OF FILE */
