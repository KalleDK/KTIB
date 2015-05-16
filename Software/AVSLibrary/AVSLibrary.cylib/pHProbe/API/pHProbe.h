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
#define `$INSTANCE_NAME`_ph7Default -29
#define `$INSTANCE_NAME`_ph4Default 149
    
#include <cytypes.h>
#include "..\..\Includes\avs_debug.h"

void  `$INSTANCE_NAME`_Start();
float `$INSTANCE_NAME`_getpH(void);
    
#endif

/* [] END OF FILE */
