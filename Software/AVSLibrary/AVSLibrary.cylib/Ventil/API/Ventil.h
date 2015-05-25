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

#if !defined(`$INSTANCE_NAME`_H_)
#define `$INSTANCE_NAME`_H_
    
#include <cytypes.h>
#include "..\..\Includes\avs_debug.h"
#include "..\..\Includes\avs_enums.h"
    
#define `$INSTANCE_NAME`_DEBUG_UART                 (`$VENTIL_DEBUG`)

void  `$INSTANCE_NAME`_Start(void);
void  `$INSTANCE_NAME`_SetState(uint8 state);
uint8 `$INSTANCE_NAME`_GetState(void);


#if `$INSTANCE_NAME`_DEBUG_UART
    void  `$INSTANCE_NAME`_DebugHandle(const char ch);
    void  `$INSTANCE_NAME`_DebugState(void);
#endif

#endif

/* [] END OF FILE */
