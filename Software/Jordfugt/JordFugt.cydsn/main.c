/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>
#include <math.h>

#include "..\..\Includes\avs_debug.h"
#include "..\..\Includes\avs_debug.c"

// Til udregning af fugtigheden 
#define SIZE 2
#define CHANNEL 0
#define RESISTOR 10000
#define VDD 4.8

int8 humidity[SIZE];

// læs fugtigheden i jorden og skriv den i de globale variabler
float32 getHumidity(){
    float32 Vout;
    float32 Rspd;
    float32 humidity;
    int16 adcResult;
    
    MOSFET_Write(1);
    ADC_SAR_Seq_1_Wakeup();
    ADC_SAR_Seq_1_IsEndConversion(ADC_SAR_Seq_1_WAIT_FOR_RESULT);
    adcResult = ADC_SAR_Seq_1_GetResult16(CHANNEL);
    Vout = ADC_SAR_Seq_1_CountsTo_Volts(CHANNEL,adcResult);
    ADC_SAR_Seq_1_Sleep();
    MOSFET_Write(0);
    Rspd = -((Vout*RESISTOR)/(Vout-VDD));
    humidity = 113.47/pow(Rspd,0.26472);
    
    //if( (humidity >=0) && (humidity <=25)){
        return humidity;
    //}
    //else{
    //    return 35.5;
    //}
}

int main()
{    //Starting components
    
    #if defined(DEBUG_UART)
        Clock_Start();
        Debug_Start();
        #if SB_DEBUG_UART
            Debug_AddComponent(SB_DebugHandle);
        #endif
    #endif
    
    uint8 measure_sleep = 0;
    
    SB_Start();
    
    ADC_SAR_Seq_1_Start(); 
    ADC_SAR_Seq_1_StartConvert();
    ADC_SAR_Seq_1_Sleep();

    CyGlobalIntEnable;
    
    for(;;)
    {
        
        #if defined(DEBUG_UART)
        Debug_Communicate();
        #endif
       
        if (measure_sleep >= 100) {
            SB_LoadValue_Float32(getHumidity());
            measure_sleep = 0;
        } else {
            CyDelay(1);
            ++measure_sleep;
        }
        SB_Communicate();
        
    }
 
    
}

/* [] END OF FILE */
