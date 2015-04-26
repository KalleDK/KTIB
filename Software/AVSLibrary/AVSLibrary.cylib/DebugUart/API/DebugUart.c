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

uint8 `$INSTANCE_NAME`_handler_selected;
uint8 `$INSTANCE_NAME`_handlers_connected;
void (*`$INSTANCE_NAME`_handlers[10]) (const char ch);


void  `$INSTANCE_NAME`_Start()
{
    `$INSTANCE_NAME`_UART_Start();
    `$INSTANCE_NAME`_handler_selected = 0;
    `$INSTANCE_NAME`_handlers_connected = 0;
    avs_debug_change_putstring(`$INSTANCE_NAME`_PutString);
    `$INSTANCE_NAME`_AddComponent(&`$INSTANCE_NAME`_DebugHandle);
    printf("Debug UART\n\r\n\rPress '?' for help\n\r\n\r");
}

void  `$INSTANCE_NAME`_PutChar(char8 txDataByte)
{
    `$INSTANCE_NAME`_UART_PutChar(txDataByte);
}

void  `$INSTANCE_NAME`_PutString(const char8 string[])
{
    `$INSTANCE_NAME`_UART_PutString(string);
}

void `$INSTANCE_NAME`_ChangeHandler(uint8 id)
{
    `$INSTANCE_NAME`_handler_selected = id;
    printf("Handler changed to: ");
}

void `$INSTANCE_NAME`_Communicate()
{
    char ch = `$INSTANCE_NAME`_UART_GetChar();
    if (ch != 0) {
        if (ch == 27)
            `$INSTANCE_NAME`_ChangeHandler(0);
        
        (*`$INSTANCE_NAME`_handlers[`$INSTANCE_NAME`_handler_selected])(ch);
        printf("\n\r");
    }
   
}

void  `$INSTANCE_NAME`_DebugHandle(const char ch)
{
    uint8 i;
    switch(ch)
    {
        case 27:
            printf("`$INSTANCE_NAME` ");
            break;
        case '?':
            printf("\n\r`$INSTANCE_NAME`\n\r\n\r");
            for (i=0; i < `$INSTANCE_NAME`_handlers_connected; ++i)
            {
                    printf("%d\t Change handler to ", i);
                    (*`$INSTANCE_NAME`_handlers[i])(27);
                    printf("\n\r");
            }
            break;
        case '0' ... '9':
            if (ch - '0' < `$INSTANCE_NAME`_handlers_connected) {
                `$INSTANCE_NAME`_ChangeHandler(ch - '0');
                (*`$INSTANCE_NAME`_handlers[`$INSTANCE_NAME`_handler_selected])(27);
                printf("\n\r");
            }
            break;
    }
}

void  `$INSTANCE_NAME`_AddComponent(void (*handler) (char))
{
    `$INSTANCE_NAME`_handlers[`$INSTANCE_NAME`_handlers_connected] = handler;
    ++`$INSTANCE_NAME`_handlers_connected;
}



/* [] END OF FILE */
