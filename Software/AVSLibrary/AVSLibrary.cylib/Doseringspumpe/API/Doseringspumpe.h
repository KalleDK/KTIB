/* ========================================
 *
 * Project: E3PRJ3 (AVS)
 * Styring af doseringspumpe 
 * Author:  Kenn H Eskildsen / Kalle R. Møller
 * Date:    20150312    
 * Ver.     1.0         Setup
 * Ver.     1.1         Custom Component
 * Ver.     1.2         Debug and Inverted mode
 * ========================================
*/

#if !defined(`$INSTANCE_NAME`_H_)
#define `$INSTANCE_NAME`_H_

#include <cytypes.h>
#include "..\..\Includes\avs_debug.h"
#include "..\..\Includes\avs_enums.h"
    
#define `$INSTANCE_NAME`_DEBUG_UART                 (`$PUMPE_DEBUG`)
#define `$INSTANCE_NAME`_INVERT                     (`$PUMPE_INVERT`)
    
void  `$INSTANCE_NAME`_Start();
void  `$INSTANCE_NAME`_Run();
void  `$INSTANCE_NAME`_Block();
void  `$INSTANCE_NAME`_SetSpeed(uint8 percent);
void  `$INSTANCE_NAME`_ApplySpeed();
uint8 `$INSTANCE_NAME`_GetSpeed();

#if `$INSTANCE_NAME`_DEBUG_UART
    void  `$INSTANCE_NAME`_DebugHandle(const char ch);
    void  `$INSTANCE_NAME`_DebugState();
#endif

#endif

/* [] END OF FILE */
