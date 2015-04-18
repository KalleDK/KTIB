/* ========================================
 *
 * Project: E3PRJ3 (AVS)
 * Styring af doseringspumpe 
 * Author:  Kenn H Eskildsen / Kalle R. Møller
 * Date:    20150312    
 * Ver.     1.0         setup
 * Ver.     1.1         Custom Component
 * ========================================
*/

#if !defined(`$INSTANCE_NAME`_H_)
#define `$INSTANCE_NAME`_H_

#include <cytypes.h>
    
void  `$INSTANCE_NAME`_Start();
void  `$INSTANCE_NAME`_Run();
void  `$INSTANCE_NAME`_Block();
void  `$INSTANCE_NAME`_SetSpeed(uint8 percent);
uint8 `$INSTANCE_NAME`_GetSpeed();

#endif

/* [] END OF FILE */
