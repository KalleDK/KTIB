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
        #if Master_DEBUG_UART
            Debug_AddComponent(Master_DebugHandle);
        #endif
        #if Slave_DEBUG_UART
            Debug_AddComponent(Slave_DebugHandle);
        #endif
    #endif
    
    Slave_Start();
    Master_Start();
    CyGlobalIntEnable;  /* Uncomment this line to enable global interrupts. */
    for(;;)
    {
        Master_Communicate();
        Slave_Communicate();
        #if defined(DEBUG_UART)
            Debug_Communicate();
        #endif  
    }
}

/* [] END OF FILE */
