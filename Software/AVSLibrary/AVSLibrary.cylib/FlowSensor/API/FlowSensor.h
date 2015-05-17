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

void  `$INSTANCE_NAME`_Start();
uint16 `$INSTANCE_NAME`_getFlowTicks(void);
uint16 `$INSTANCE_NAME`_getFlowLiters(void);
void `$INSTANCE_NAME`_calcFlowLiters(void);
    
#endif

/* [] END OF FILE */
