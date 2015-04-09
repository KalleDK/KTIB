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

uint8 speed;
uint8 running;

void  `$INSTANCE_NAME`_Start()
{
    `$INSTANCE_NAME`_Clock_Start();
    `$INSTANCE_NAME`_PWM_Start();
    speed = 0;
    running = 0;
}
    
void  `$INSTANCE_NAME`_Run()
{
    if (speed == 0)
        `$INSTANCE_NAME`_SetSpeed(100);
    
    `$INSTANCE_NAME`_PWM_WriteCompare(speed);
    running = 1;
}

void  `$INSTANCE_NAME`_Block()
{
    `$INSTANCE_NAME`_SetSpeed(0);
    `$INSTANCE_NAME`_PWM_WriteCompare(speed);
    running = 0;
}

void  `$INSTANCE_NAME`_SetSpeed(uint8 percent)
{
    uint16 tmp;
    percent = 100 - percent;
    if (percent > 100)
        percent = 100;
    tmp = 255 * percent;
    tmp /= 100;
    speed = tmp;
    
    if (running)
        `$INSTANCE_NAME`_Run();
}

uint8 `$INSTANCE_NAME`_GetSpeed()
{
    uint16 tmp;
    tmp = speed * 100;
    tmp /= 256;
    return tmp;
}


/* [] END OF FILE */
