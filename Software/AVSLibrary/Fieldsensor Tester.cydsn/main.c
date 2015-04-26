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
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    #if defined(DEBUG_UART)
        Clock_Start();
        Debug_Start();
        #if SB_DEBUG_UART
            Debug_AddComponent(SB_DebugHandle);
        #endif
    #endif
    
    SB_Start();

    CyGlobalIntEnable;
    for(;;)
    {
        
        #if defined(DEBUG_UART)
        Debug_Communicate();
        #endif
        
        SB_Communicate();
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
