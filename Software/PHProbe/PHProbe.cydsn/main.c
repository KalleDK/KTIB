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

#define ph7Default -29
#define ph4Default 149
float ph7;
float ph4;
float phVal;
int16 adcResult;
int16 adcResultmV;

// Omregner til pH-værdi. ph4 og ph7 er parametrene som bruges til at kalibrere proben
float mvToPh(float mv, float ph4, float ph7){     
    return ((ph7-mv)/((ph4-ph7)/3)+7);
}
float getPhVal(){
    while(ADC_IsEndConversion(ADC_WAIT_FOR_RESULT)== 0){};
    
    adcResult=ADC_GetResult16(0);
    adcResultmV = ADC_CountsTo_mVolts(0,adcResult);
    
    return mvToPh(adcResultmV,ph4,ph7);
}

CY_ISR(CALIBRATE)
{
    int8 status;
    if(CALI_Read() ==  1){ // vent på at knappen har været nede i 3 sekunder
    CyDelay(3000);
    if(CALI_Read() == 1)
    status = 1;
    }
    if(status == 1){
    getPhVal();
    ph7 = adcResultmV;
    ph4 = ph4Default + (ph7 - ph7Default); // ph4Default + korrektion 
    RED_LED_Write(0);
    GREEN_LED_Write(1);
    }
}

int main()
{
    ph7 = ph7Default; // Default værdi
    ph4 = ph4Default; // Default værdi 
    Opamp_1_Start();
    ADC_Start();
    ADC_StartConvert();
    Calibrate_StartEx(CALIBRATE);
    CyGlobalIntEnable; 
    RED_LED_Write(1);
    GREEN_LED_Write(0);
     
   while(1){
   phVal = getPhVal();
}
    return 0;
}
        
/* [] END OF FILE */
