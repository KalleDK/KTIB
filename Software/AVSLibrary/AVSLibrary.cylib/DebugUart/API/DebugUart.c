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

void  `$INSTANCE_NAME`_Start()
{
    `$INSTANCE_NAME`_Clock_Start();
    `$INSTANCE_NAME`_UART_Start();
}

void  `$INSTANCE_NAME`_PutChar(char8 txDataByte)
{
    `$INSTANCE_NAME`_UART_PutChar(txDataByte);
}
void  `$INSTANCE_NAME`_PutString(const char8 string[])
{
    `$INSTANCE_NAME`_UART_PutString(string);
}

char8 `$INSTANCE_NAME`_GetChar()
{
    return `$INSTANCE_NAME`_UART_GetChar();
}

/* [] END OF FILE */
