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

#include "..\..\Includes\avs_debug.h"
#include "..\..\Includes\avs_debug.c"


int main()
{
    #if defined(DEBUG_UART)
        Clock_Start();
        Debug_Start();
        #if Pumpe_DEBUG_UART
            Debug_AddComponent(Pumpe_DebugHandle);
        #endif
    #endif
    Pumpe_Start();
    /* CyGlobalIntEnable; */ /* Uncomment this line to enable global interrupts. */
    for(;;)
    {
        
        #if defined(DEBUG_UART)
        Debug_Communicate();
        #endif    
        
        
    }
}

/* [] END OF FILE */
