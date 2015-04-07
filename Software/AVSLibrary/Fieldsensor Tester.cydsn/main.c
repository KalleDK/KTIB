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
    Debug_Start();
    
    #if SB_DEBUG_UART
    SB_DebugInit(Debug_PutString);
    #endif
    
    SB_Start();

    CyGlobalIntEnable;
    for(;;)
    {
        
        #if SB_DEBUG_UART
        SB_DebugChar(Debug_GetChar());
        #endif
        
        SB_Communicate();
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
