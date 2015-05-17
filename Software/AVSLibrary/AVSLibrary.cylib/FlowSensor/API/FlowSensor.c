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

CY_ISR_PROTO(`$INSTANCE_NAME`_FLOW);

uint16 `$INSTANCE_NAME`_flowTicks;
uint16 `$INSTANCE_NAME`_flowTickResolution;
uint16 `$INSTANCE_NAME`_flowLiters;

CY_ISR(`$INSTANCE_NAME`_FLOW) {
    `$INSTANCE_NAME`_flowTicks += 1;
}

void  `$INSTANCE_NAME`_Start() {
    `$INSTANCE_NAME`_flowTicks = 0;
    `$INSTANCE_NAME`_flowLiters = 0;
    `$INSTANCE_NAME`_flowTickResolution = 36;
    `$INSTANCE_NAME`_Flow_StartEx(`$INSTANCE_NAME`_FLOW);

}

uint16 `$INSTANCE_NAME`_getFlowTicks() {
    return `$INSTANCE_NAME`_flowTicks;
}

uint16 `$INSTANCE_NAME`_getFlowLiters() {
    return `$INSTANCE_NAME`_flowLiters;
}

uint16 `$INSTANCE_NAME`_calcFlowLiters() {
    uint8 whole, half;
    whole = `$INSTANCE_NAME`_flowTicks / `$INSTANCE_NAME`_flowTickResolution;
    half = `$INSTANCE_NAME`_flowTicks % `$INSTANCE_NAME`_flowTickResolution;
    `$INSTANCE_NAME`_flowLiters = (whole << 8);
    if(half >= `$INSTANCE_NAME`_flowTickResolution/2) {
        `$INSTANCE_NAME`_flowLiters = `$INSTANCE_NAME`_flowLiters | (1 << 7);
    }
}
/* [] END OF FILE */
