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
        #if MASTER_DEBUG_UART
            Debug_AddComponent(MASTER_DebugHandle);
        #endif
        #if SLAVE_DEBUG_UART
            Debug_AddComponent(SLAVE_DebugHandle);
        #endif
    #endif
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    SLAVE_Start();
    MASTER_Start();
    CyGlobalIntEnable;
    for(;;)
    {
        #if defined(DEBUG_UART)
            Debug_Communicate();
        #endif  
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
