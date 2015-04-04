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
#include <stdio.h>            //definitions for all standard io functions

#define CLEAR_SCREEN    (0x0C)
#define SIZE    100
#define txSize 20
#define rxSize 10
#define on 0
#define off 1
#define EEPROM_PH4 0
#define EEPROM_PH7 1

uint8 ph7;
uint8 ph4;
uint8 flag;
int16 adcResult;
int16 adcResultmV;
char txBuffer[txSize];
char rxBuffer[rxSize];
char buffer[3];
static const uint8 CYCODE eepromArray[2]; // oprettelse af en virtuel EEPROM

uint8 rxString(char*rxBuffer, uint8 size);
int16 mvToPh(float, float, float);
uint8 toggle = 0;
void calibrate();
void requestCalibrate();

CY_ISR(Calibrate)
{   
    flag = 0;
    while(SW_Calibrate_Read() == on){ // vent til at knappen har været holdt nede i 5s
        CyDelay(5000);
        flag = 1;
        break;          
    }
    if (flag == 0){ // hvis der ikke er ventet i 5s
        return;
    }
    else
    calibrate();
}

CY_ISR(PWM_Compare)
{   
    requestCalibrate();
}

int main()
{
    Opamp_1_Start();
    ADC_SAR_Seq_1_Start();
    ADC_SAR_Seq_1_StartConvert();
    UART_Start();
    UART_UartPutChar(CLEAR_SCREEN);
    UART_UartPutString("Ph-probe\n\r");
    TCPWM_1_Start();
    Em_EEPROM_1_Start();
    ph7 = eepromArray[EEPROM_PH7];
    ph4 = eepromArray[EEPROM_PH4];
    isr_pwm_StartEx(PWM_Compare);
    isr_Calibrate_StartEx(Calibrate);
    CyGlobalIntEnable; 
    requestCalibrate();

   while(1){
       
       if(rxString(rxBuffer,rxSize)){ // hvis der er modtaget nyt data       
                 adcResult=ADC_SAR_Seq_1_GetResult16(0);
                 adcResultmV = ADC_SAR_Seq_1_CountsTo_mVolts(0,adcResult);
                 sprintf(txBuffer,"Probe 1: %d\n\r",mvToPh(adcResultmV,ph4,ph7));  // UARTBuffer has now the string converted
                 UART_UartPutString(txBuffer);  
            }
    
        }
    }
int16 mvToPh(float mv, float ph4, float ph7){
    
    return ((ph7-mv)/((ph4-ph7)/3)+7)*100;

}

void calibrate(){
    
     while(1){
        if(SW_PH7_Read() == on){ 
            adcResult=ADC_SAR_Seq_1_GetResult16(0);
            ph7 = ADC_SAR_Seq_1_CountsTo_mVolts(0,adcResult);
            Em_EEPROM_1_Write(&ph7,&eepromArray[EEPROM_PH7],1u); 
            break;
        }
        if(SW_PH4_Read() == on){
            adcResult=ADC_SAR_Seq_1_GetResult16(0);
            ph4 = ADC_SAR_Seq_1_CountsTo_mVolts(0,adcResult);
            Em_EEPROM_1_Write(&ph4,&eepromArray[EEPROM_PH4],1u); 
            break;
        }
   }
    GREEN_LED_Write(on);
    TCPWM_1_Start();  
}
void requestCalibrate(){
    TCPWM_1_Stop();
    TCPWM_1_WriteCounter(0);
    UART_UartPutString("Ph-probe skal kalibreres\n\r");  
    GREEN_LED_Write(off);
    RED_LED_Write(on);    
}
uint8 rxString(char*rxBuffer, uint8 size){
    int x;
    
    rxBuffer[0]=UART_UartGetChar();
    if(rxBuffer[0] == 0){
        return 0; // intet at modtage
    }
    else{
        for(x = 1; x<size; x++){
            rxBuffer[x] = UART_UartGetChar();
            if(rxBuffer[x] == 0){
                rxBuffer[x]='\0';
                break;
            }
        }
    }
        return 1; // fuldført 

}

    
/* [] END OF FILE */
