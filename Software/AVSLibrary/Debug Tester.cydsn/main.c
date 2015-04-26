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
#include "..\..\Includes\avs_debug.h"
#include "..\..\Includes\avs_debug.c"


int main()
{
    char8 addr;
   
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    #if defined(DEBUG_UART)
    Clock_Start();
    Debug_Start();
    Debug_AddComponent(Ventil_1_DebugHandle);
    #endif
    
    CyGlobalIntEnable; /* Uncomment this line to enable global interrupts. */
    addr = 'a';
    for(;;)
    {
        CyDelay(500);
        ++addr;
        #if defined(DEBUG_UART)
        Debug_Communicate();
        #endif
        printf("Address: 0x%02X\r\n", addr);
        /* Place your applica
        tion code here. */
    }
}

/* [] END OF FILE */
