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

CY_ISR_PROTO(`$INSTANCE_NAME`_CALIBRATE);

float `$INSTANCE_NAME`_ph7;
float `$INSTANCE_NAME`_ph4;
float `$INSTANCE_NAME`_phVal;
int16 `$INSTANCE_NAME`_adcResult;
int16 `$INSTANCE_NAME`_adcResultmV;

CY_ISR(`$INSTANCE_NAME`_CALIBRATE)
{
    int8 status;
    if(`$INSTANCE_NAME`_CALI_Read() == 1){ // vent på at knappen har været nede i 3 sekunder
    CyDelay(3000);
    if(`$INSTANCE_NAME`_CALI_Read() == 1)
    status = 1;
    }
    if(status == 1){
    `$INSTANCE_NAME`_getpH();
    `$INSTANCE_NAME`_ph7 = `$INSTANCE_NAME`_adcResultmV;
    `$INSTANCE_NAME`_ph4 = `$INSTANCE_NAME`_ph4Default + (`$INSTANCE_NAME`_ph7 - `$INSTANCE_NAME`_ph7Default); // ph4Default + korrektion 
    `$INSTANCE_NAME`_RED_LED_Write(0);
    `$INSTANCE_NAME`_GREEN_LED_Write(1);
    }
}

void  `$INSTANCE_NAME`_Start()
{
    `$INSTANCE_NAME`_ph7 = `$INSTANCE_NAME`_ph7Default;
    `$INSTANCE_NAME`_ph4 = `$INSTANCE_NAME`_ph4Default;
    `$INSTANCE_NAME`_Opamp_Start();
    `$INSTANCE_NAME`_ADC_Start();
    `$INSTANCE_NAME`_ADC_StartConvert();
    `$INSTANCE_NAME`_Calibrate_StartEx(`$INSTANCE_NAME`_CALIBRATE);
    `$INSTANCE_NAME`_RED_LED_Write(1);
    `$INSTANCE_NAME`_GREEN_LED_Write(0);
}

float `$INSTANCE_NAME`_mvToPh(float mv, float ph4, float ph7){     
    return ((ph7-mv)/((ph4-ph7)/3)+7);
}

float `$INSTANCE_NAME`_getpH(){
    while(`$INSTANCE_NAME`_ADC_IsEndConversion(`$INSTANCE_NAME`_ADC_WAIT_FOR_RESULT) == 0){};
    
    `$INSTANCE_NAME`_adcResult = `$INSTANCE_NAME`_ADC_GetResult16(0);
    `$INSTANCE_NAME`_adcResultmV = `$INSTANCE_NAME`_ADC_CountsTo_mVolts(0,`$INSTANCE_NAME`_adcResult);
    
    return `$INSTANCE_NAME`_mvToPh(`$INSTANCE_NAME`_adcResultmV,`$INSTANCE_NAME`_ph4,`$INSTANCE_NAME`_ph7);
}

/* [] END OF FILE */
