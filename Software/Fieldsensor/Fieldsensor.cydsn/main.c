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
#define FS_STRINGS
#include "../../AVS_Header.h"

//Til sprintf, som ikke burde skulle benyttes i release
#ifdef DEBUG
#include <stdio.h>
#endif

//I2C Variabler
volatile uint8 state = returnValues;
volatile uint8 rdBuffer[3] = {0};
volatile uint8 wrBuffer[3] = {0};
    
//Sensor Variabler
volatile const uint8 fs_type = tempsensor;
volatile uint8 value_high = 0; //Heltal værdier fra sensor
volatile uint8 value_low = 0; //Sættes til 128 hvis det er 0,5

//Debug UART
#ifdef DEBUG
void debugUart() {
    char ch;
    char debug_msg[50];
    ch = UART_PC_GetChar();
    switch(ch)
    {
        case '?':
            UART_PC_PutString("Commands\r\n\r\n");
            UART_PC_PutString("1 Change state to returnValues\r\n");
            UART_PC_PutString("2 Change state to returnTypes\r\n");
            UART_PC_PutString("s Show State\r\n");
            UART_PC_PutString("v Show Values\r\n");
            UART_PC_PutString("t Show Type\r\n");
            UART_PC_PutString("l Load dummy values\r\n");
            UART_PC_PutString("c Clear values\r\n");
            UART_PC_PutString("\r\n");
            break;
        case '1':
            state = returnValues;
            UART_PC_PutString("State: returnValues\r\n");
            break;
        case '2':
            state = returnTypes;
            UART_PC_PutString("State: returnTypes\r\n");
            break;
        case 's':
            UART_PC_PutString("State: ");
            switch(state)
            {
                case returnValues:
                    UART_PC_PutString("returnValues\r\n");
                    break;
                case returnTypes:
                    UART_PC_PutString("returnTypes\r\n");
                    break;
                default:
                    UART_PC_PutString("unknown\r\n");
                    break;
            }
            break;
        case 't':
            sprintf(debug_msg, "Type: 0x%02x (", fs_type);
            UART_PC_PutString(debug_msg);
            UART_PC_PutString(fs_types_str(fs_type));
            UART_PC_PutString(")\r\n");
            break;
        case 'v':
            sprintf(debug_msg, "Value High: %d\r\nValue Low: %d\r\nValue Total: %d,%d\r\n", value_high, value_low, value_high, (value_low>>7)*5);
            UART_PC_PutString(debug_msg);
            break;
        case 'l':
            value_high = 32;
            value_low = 128;
            UART_PC_PutString("Dummy values loaded\r\n");
            break;
        case 'c':
            value_high = 0;
            value_low = 0;
            UART_PC_PutString("Values cleared\r\n");
            break;
        default:
            break;
    }
};
#endif

//I2C Read Cleanup (nulstil efter hver read)
void I2C_Read_Cleanup() {
    if(0u != (OEBUS_I2CSlaveStatus() & OEBUS_I2C_SSTAT_RD_CMPLT))
    {
        OEBUS_I2CSlaveClearReadBuf();
    }
}

//I2C Write Parsing (skift state til det der ønskes)
void I2C_Write_Parse()
{
    //Hvis der er kommet et write til os
    if(0u != (OEBUS_I2CSlaveStatus() & OEBUS_I2C_SSTAT_WR_CMPLT))
    {
        //Clear write status helt
        OEBUS_I2CSlaveClearWriteStatus();
        //Skift state baseret på commandoen vi fik
        switch(*wrBuffer)
        {
            case returnTypes:
                state = returnTypes;
                break;
            case returnValues:
                state = returnValues;
                break;
            default:
                break;
        }
        //Clear bufferen, da alle I2C vi har er på én byte
        OEBUS_I2CSlaveClearWriteBuf();
    }
}

//Sæt værdierne klar i read buffer
//Kunne ændres til kun at være i tilfælde af
//state change eller value change
void loadReturnData() {
    switch(state)
    {
        case returnValues:
            rdBuffer[0] = value_high;
            rdBuffer[1] = value_low;
            break;
        case returnTypes:
            rdBuffer[0] = fs_type;
            break;
        default:
            break;
    }
}

int main()
{
    //Starting components
    OEBUS_I2CSlaveInitReadBuf((uint8 *) rdBuffer, 3);
    OEBUS_I2CSlaveInitWriteBuf((uint8 *) wrBuffer, 3);
    OEBUS_Start();
    
    //Starting Debug UART
    #ifdef DEBUG
    UART_PC_Start();
    UART_PC_PutString("Initializing: Done\r\n");
    #endif
    
    CyGlobalIntEnable;
    for(;;)
    {
        /*
        Insert function to populate value_high and value_low from fieldsensor
        - uint8 value_high = 0; //Heltal værdier fra sensor
        - uint8 value_low = 0; //Sættes til 128 hvis det er 0,5
        */
        
        loadReturnData();
        
        #ifdef DEBUG
        debugUart();
        #endif
        
        I2C_Read_Cleanup();
        
        I2C_Write_Parse();
        
    }
}
