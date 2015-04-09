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

int main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    Pumpe_Start();
    /* CyGlobalIntEnable; */ /* Uncomment this line to enable global interrupts. */
    for(;;)
    {
        //Full speed for 2 sec
        Pumpe_SetSpeed(25);
        Pumpe_Run();
        CyDelay(1000);
        
        Pumpe_SetSpeed(50);
        Pumpe_Run();
        CyDelay(1000);
        
        Pumpe_SetSpeed(99);
        Pumpe_Run();
        CyDelay(1000);
        
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
