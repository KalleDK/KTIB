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

#if `$INSTANCE_NAME`_DEBUG_UART
#include "..\..\Includes\avs_debug.h"
#endif

void  `$INSTANCE_NAME`_Start()
{
    `$INSTANCE_NAME`_SetState(0);
}

void `$INSTANCE_NAME`_SetState(uint8 state)
{
    `$INSTANCE_NAME`_PIN_Write(state);
    #if `$INSTANCE_NAME`_DEBUG_UART
    printf("`$INSTANCE_NAME` SetState: 0x%02X\r\n", state);
    #endif
}

uint8 `$INSTANCE_NAME`_GetState(void)
{
    return `$INSTANCE_NAME`_PIN_Read();
}

#if `$INSTANCE_NAME`_DEBUG_UART
    void  `$INSTANCE_NAME`_DebugHandle(const char ch)
    {
        switch(ch)
        {
            case 27:
                printf("`$INSTANCE_NAME`");
                break;
            case '?':
                printf("\n\r`$INSTANCE_NAME`\n\r\n\r");
                printf("s\tShow State\n\r");
                printf("%d\tVentil Open\n\r", VENTIL_OPEN);
                printf("%d\tVentil Close\n\r", VENTIL_CLOSE);
                printf("\n\r");
                break;
            case 's':
                `$INSTANCE_NAME`_DebugState();
                break;
            case VENTIL_OPEN + '0':
            case VENTIL_CLOSE + '0':
                `$INSTANCE_NAME`_SetState(ch - '0');
                break;
        }
    }
    
    void  `$INSTANCE_NAME`_DebugState()
    {
        printf("`$INSTANCE_NAME` State: 0x%02X\n\r\n\r", `$INSTANCE_NAME`_PIN_Read());
    }
#endif
/* [] END OF FILE */
