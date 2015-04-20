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

#define DEBUG_UART 1
    
#include <cytypes.h>
#include "..\..\Includes\avs_debug.h"

void  `$INSTANCE_NAME`_Start();
int   `$INSTANCE_NAME`_printf(const char* string, ...);
void  `$INSTANCE_NAME`_PutChar(char8 txDataByte);
void  `$INSTANCE_NAME`_PutString(const char8 string[]);
void  `$INSTANCE_NAME`_Communicate();
void  `$INSTANCE_NAME`_DebugHandle(const char ch);
void  `$INSTANCE_NAME`_AddComponent(void (*debugchar) (char));
    
#endif

/* [] END OF FILE */
