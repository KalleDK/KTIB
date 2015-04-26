/* ========================================
 *
 * Project: E3PRJ3 (AVS)
 * Styring af doseringspumpe 
 * Author:  Kenn H Eskildsen / Kalle R. Møller
 * Date:    20150312    
 * Ver.     1.0         Setup
 * Ver.     1.1         Custom Component
 * ========================================
*/

#include <project.h>

#if `$INSTANCE_NAME`_DEBUG_UART
#include "..\..\Includes\avs_debug.h"
#endif

uint8 `$INSTANCE_NAME`_pwm;
uint8 `$INSTANCE_NAME`_running;

void  `$INSTANCE_NAME`_Start()
{
    `$INSTANCE_NAME`_PWM_Start();
    `$INSTANCE_NAME`_running = 1;
    `$INSTANCE_NAME`_Block();   
}
    
void  `$INSTANCE_NAME`_Run()
{
    if (`$INSTANCE_NAME`_GetSpeed() == 0)
        `$INSTANCE_NAME`_SetSpeed(100);
        
    `$INSTANCE_NAME`_running = 1;
    
    `$INSTANCE_NAME`_ApplySpeed();
}

void  `$INSTANCE_NAME`_Block()
{
    `$INSTANCE_NAME`_SetSpeed(0);
    
    `$INSTANCE_NAME`_running = 0;
}

void  `$INSTANCE_NAME`_SetSpeed(uint8 percent)
{
    uint16 tmp;
    if (percent > 100)
        percent = 100;
    
    #if `$INSTANCE_NAME`_INVERT
        percent = 100 - percent;
    #endif
    
    tmp = 255 * percent;
    tmp /= 100;
    `$INSTANCE_NAME`_pwm = tmp;
    
    if (`$INSTANCE_NAME`_running)
        `$INSTANCE_NAME`_ApplySpeed();
}

void  `$INSTANCE_NAME`_ApplySpeed()
{
    `$INSTANCE_NAME`_PWM_WriteCompare(`$INSTANCE_NAME`_pwm);    
}

uint8 `$INSTANCE_NAME`_GetSpeed()
{
    uint16 tmp;
    tmp = `$INSTANCE_NAME`_pwm * 100;
    tmp /= 255;
    
    #if `$INSTANCE_NAME`_INVERT
        tmp = 100 - tmp;
    #endif
    
    return tmp;
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
                printf("r\tRun\n\r");
                printf("b\tBlock\n\r");
                printf("+\tIncrease speed 10%%\n\r");
                printf("-\tDecrease speed 10%%\n\r");
                printf("\n\r");
                break;
            case 's':
                `$INSTANCE_NAME`_DebugState();
                break;
            case 'r':
                `$INSTANCE_NAME`_Run();
                `$INSTANCE_NAME`_DebugState();
                break;
            case 'b':
                `$INSTANCE_NAME`_Block();
                `$INSTANCE_NAME`_DebugState();
                break;
            case '+':
                `$INSTANCE_NAME`_SetSpeed(`$INSTANCE_NAME`_GetSpeed()+10);
                `$INSTANCE_NAME`_DebugState();
                break;
            case '-':
                `$INSTANCE_NAME`_SetSpeed((`$INSTANCE_NAME`_GetSpeed() > 10 ? `$INSTANCE_NAME`_GetSpeed() - 10 : 0));
                `$INSTANCE_NAME`_DebugState();
                break;
            default:
                printf("Not implemented\n\r");
        }
    }
    
    void  `$INSTANCE_NAME`_DebugState()
    {
        printf("`$INSTANCE_NAME` Running: [ 0x%02X ]  Percent: [ %d%% ]  PWM: [ 0x%02X ]\n\r\n\r", `$INSTANCE_NAME`_running , `$INSTANCE_NAME`_GetSpeed(), `$INSTANCE_NAME`_pwm);
    }
#endif

/* [] END OF FILE */
