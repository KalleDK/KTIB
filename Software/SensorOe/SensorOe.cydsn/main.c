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
#include <stdlib.h>
#include "..\..\Includes\avs_debug.h"
#include "..\..\Includes\avs_debug.c"
#include "..\..\Includes\avs_enums.h"



#define HIGH 0
#define LOW 1

typedef struct
{
    uint8 addr;
    uint8 status;
    uint8 type;
    uint8 values[2];
} Fieldsensor;

Fieldsensor* fieldsensors[10];
uint8 fieldsensor_connected = 0;
uint8 ventil = 0;

void changeVentil(uint8 state)
{
    ventil = state;
    Ventil_SetState(state);
}

void addFieldsensor(uint8 addr)
{
    fieldsensors[fieldsensor_connected] = malloc(sizeof(Fieldsensor));
    fieldsensors[fieldsensor_connected]->addr = addr;
    fieldsensors[fieldsensor_connected]->status = FS_OFFLINE;
    fieldsensors[fieldsensor_connected]->type = SensorBus__UNKNOWN;
    fieldsensors[fieldsensor_connected]->values[HIGH] = 0;
    fieldsensors[fieldsensor_connected]->values[LOW] = 0;
    ++fieldsensor_connected;
}

void pollFieldsensor(Fieldsensor* fs)
{
    if (SensorBus_SensorPoll(fs->addr, fs->values, &fs->type)) {
        #if OEBUS_DEBUG_UART
        if (fs->status == FS_OFFLINE) {
            printf("Sensor Detected:\n\r");
        }
        #endif
        fs->status = FS_ONLINE;
    } else {
        fs->status = FS_OFFLINE;
    }
}

void sendFieldsensors(uint8 receiver)
{
    uint8 i = 0;
    while(fieldsensors[i] != 0) {++i;}
    OEBUS_PutTxMessage(receiver, i*4, RES_OE_FS_DATA);
    i = 0;
    while(fieldsensors[i] != 0) {
        OEBUS_PutTxMessageArg(fieldsensors[i]->addr);
        OEBUS_PutTxMessageArg(fieldsensors[i]->status);
        OEBUS_PutTxMessageArg(fieldsensors[i]->values[HIGH]);
        OEBUS_PutTxMessageArg(fieldsensors[i]->values[LOW]);
        ++i;
    }
}

void parseOEBUS()
{
    OEBUS_MSG_STRUCT msg;
    OEBUS_GetRxMessage(&msg);
    #if OEBUS_DEBUG_UART
    OEBUS_DebugMsg(&msg);
    #endif
    
    switch(msg.cmd) {
        case REQ_OE_FS_DATA:
            sendFieldsensors(msg.transmitter);
            break;
        case REQ_OE_VENTIL:
            changeVentil(msg.args[0]);
            OEBUS_PutTxMessage(msg.transmitter, 1, RES_OE_VENTIL);
            OEBUS_PutTxMessageArg(ventil);
            break;
        default:
            break;
    }
    OEBUS_ClearRxMessage();
}

#if defined(DEBUG_UART)
void DebugHandle(const char ch) {
   
    Fieldsensor* fs = 0;
    
    switch(ch)
    {
        case 27:
            printf("SensorOE");
            break;
        case '?':
            printf("Commands\n\r\n\r");
            printf("0-5\tShow Fieldsensor 0-9\n\r");
            printf("f\tSend fieldsensors to 0x01\n\r");
            printf("o\tOpen ventil\n\r");
            printf("O\tClose ventil\n\r");
            printf("\n\r");
            break;
        case 'o':
            changeVentil(VENTIL_OPEN);
            break;
        case 'O':
            changeVentil(VENTIL_CLOSE);
            break;
        case 'f':
            sendFieldsensors(0x01);
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
            fs = fieldsensors[ch - '0'];
            if (fs != 0) {
                printf("Sensor\t\t0\n\r");
                printf("ID\t\t0x%02X\n\r", fs->addr);
                     
                printf("Status:\t\t0x%02X (", fs->status);
                
                switch(fs->status)
                {
                    case FS_OFFLINE:
                        printf("OFFLINE");
                        break;
                    case FS_ONLINE:
                        printf("ONLINE");
                        break;
                    default:
                        printf("UNKNOWN");
                        break;
                }
                printf(")\n\r");
                              
                printf("Type:\t\t0x%02X \n\r", fs->type);
                       
                printf("Value High:\t%d\n\r", fs->values[HIGH]);
                printf("Value Low:\t%d\n\r", fs->values[LOW]);
                printf("Value Total:\t%d,%d\n\r\n\r", fs->values[HIGH], (fs->values[LOW]>>7)*5);
            } else {
                printf("Sensor does not exists\n\r\n\r");
            }
            break;
        default:
            break;
    }
    
}
#endif

int main()
{
    #if defined(DEBUG_UART)
        Clock_Start();
        Debug_Start();
        Debug_AddComponent(DebugHandle);
        #if Ventil_DEBUG_UART
            Debug_AddComponent(Ventil_DebugHandle);
        #endif
        #if OEBUS_DEBUG_UART
            Debug_AddComponent(OEBUS_DebugHandle);
        #endif
        #if SensorBus_DEBUG_UART
            Debug_AddComponent(SensorBus_DebugHandle);
        #endif
    #endif

    uint8 polling_nr = 0;
    addFieldsensor(0x0C);
    addFieldsensor(0x09);
    addFieldsensor(0x0A);
    addFieldsensor(0x0B);
   
    //Initialise components
    
    Ventil_Start();
    OEBUS_Start();
    SensorBus_Start();
    
    CyGlobalIntEnable;
    
    for(;;)
    {   
        #if defined(DEBUG_UART)
        Debug_Communicate();
        #endif 
        
        if (OEBUS_ReadRxStatus() == OEBUS_MSG_READY)
            parseOEBUS();
        
        //We poll a single sensor, before we loop around, to be sure we don't spend to much time polling
        //Could be empty array if no sensors are added
        if (fieldsensors[polling_nr] != 0)
            pollFieldsensor(fieldsensors[polling_nr]);
        if ( fieldsensors[++polling_nr] == 0 )
            polling_nr = 0;
        
        CyDelay(1);
    }
}

/* [] END OF FILE */
