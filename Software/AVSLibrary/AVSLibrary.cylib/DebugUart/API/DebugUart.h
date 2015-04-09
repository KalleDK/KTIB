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

void  `$INSTANCE_NAME`_Start();
void  `$INSTANCE_NAME`_PutChar(char8 txDataByte);
void  `$INSTANCE_NAME`_PutString(const char8 string[]);
char8 `$INSTANCE_NAME`_GetChar();    
    
#endif

/* [] END OF FILE */
