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
    //Starting components
    #if defined(DEBUG_UART)
        Clock_Start();
        Debug_Start();
        #if SB_DEBUG_UART
            Debug_AddComponent(SB_DebugHandle);
        #endif
    #endif
    
    uint8 measure_sleep = 0;
  
    SB_Start();
    
    CyGlobalIntEnable;
    for(;;)
    {
        /*
        Insert function to populate value_high and value_low from fieldsensor
        - uint8 value_high = 0; //Heltal værdier fra sensor
        - uint8 value_low = 0; //Sættes til 128 hvis det er 0,5
        */
        if (measure_sleep >= 100) {
            SB_LoadValue_Float32(35.5);
            measure_sleep = 0;
        } else {
            CyDelay(1);
            ++measure_sleep;
        }
        
        #if defined(DEBUG_UART)
        Debug_Communicate();
        #endif
        
        SB_Communicate();
        
    }
}
