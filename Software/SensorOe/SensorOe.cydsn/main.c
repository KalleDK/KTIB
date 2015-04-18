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
    Ventil_ChangeState(state);
}

void addFieldsensor(uint8 addr)
{
    fieldsensors[fieldsensor_connected] = malloc(sizeof(Fieldsensor));
    fieldsensors[fieldsensor_connected]->addr = addr;
    fieldsensors[fieldsensor_connected]->status = OEBUS_FS_OFFLINE;
    fieldsensors[fieldsensor_connected]->type = SensorBus__UNKNOWN;
    fieldsensors[fieldsensor_connected]->values[HIGH] = 0;
    fieldsensors[fieldsensor_connected]->values[LOW] = 0;
    ++fieldsensor_connected;
}

void pollFieldsensor(Fieldsensor* fs)
{
    if (SensorBus_SensorPoll(fs->addr, fs->values, &fs->type)) {
        #if OEBUS_DEBUG_UART
        if (fs->status == OEBUS_FS_OFFLINE) {
            Debug_PutString("Sensor Detected:\r\n");
        }
        #endif
        fs->status = OEBUS_FS_ONLINE;
    } else {
        fs->status = OEBUS_FS_OFFLINE;
    }
}

void sendFieldsensors(uint8 receiver)
{
    uint8 i = 0;
    while(fieldsensors[i] != 0) {++i;}
    OEBUS_PutTxMessage(receiver, i*4, OEBUS_OE_RES_FS_DATA);
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
        case OEBUS_OE_REQ_FS_DATA:
            sendFieldsensors(msg.transmitter);
            break;
        case OEBUS_OE_REQ_VENTIL:
            changeVentil(msg.args[0]);
            OEBUS_PutTxMessage(msg.transmitter, 1, OEBUS_OE_RES_VENTIL);
            OEBUS_PutTxMessageArg(ventil);
            break;
        default:
            break;
    }
    OEBUS_ClearRxMessage();
}

#if SensorBus_DEBUG_UART || OEBUS_Debug_UART
void debugUart() {
    char ch;
    char debug_msg[50];
    ch = Debug_GetChar();
    Fieldsensor* fs = 0;
    
    switch(ch)
    {
        case 'h':
            Debug_PutString("Commands\r\n\r\n");
            Debug_PutString("0-5\tShow Fieldsensor 0-9\r\n");
            Debug_PutString("f\tSend fieldsensors to 0x01\r\n");
            Debug_PutString("o\tOpen ventil\r\n");
            Debug_PutString("O\tClose ventil\r\n");
            Debug_PutString("\r\n");
            break;
        case 'o':
            changeVentil(Ventil_OPEN);
            break;
        case 'O':
            changeVentil(Ventil_CLOSE);
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
                Debug_PutString("Sensor\t\t0\r\n");
                  
                sprintf(debug_msg, "ID\t\t0x%02X\r\n", fs->addr);
                Debug_PutString(debug_msg);
                     
                sprintf(debug_msg, "Status:\t\t0x%02X (", fs->status);
                Debug_PutString(debug_msg);
                switch(fs->status)
                {
                    case OEBUS_FS_OFFLINE:
                        Debug_PutString("OFFLINE");
                        break;
                    case OEBUS_FS_ONLINE:
                        Debug_PutString("ONLINE");
                        break;
                    default:
                        Debug_PutString("UNKNOWN");
                        break;
                }
                Debug_PutString(")\r\n");
                              
                sprintf(debug_msg, "Type:\t\t0x%02X (", fs->type);
                Debug_PutString(debug_msg);
                Debug_PutString(")\r\n");
                       
                sprintf(debug_msg, "Value High:\t%d\r\n", fs->values[HIGH]);
                Debug_PutString(debug_msg);
                sprintf(debug_msg, "Value Low:\t%d\r\n", fs->values[LOW]);
                Debug_PutString(debug_msg);
                sprintf(debug_msg, "Value Total:\t%d,%d\r\n\r\n", fs->values[HIGH], (fs->values[LOW]>>7)*5);
                Debug_PutString(debug_msg);
            } else {
                Debug_PutString("Sensor does not exists\r\n\r\n");
            }
            break;
        default:
            break;
    }
    
}
#endif

int main()
{
    #if SensorBus_DEBUG_UART || OEBUS_Debug_UART
    Debug_Start();
    #endif
    
    #if SensorBus_DEBUG_UART
    SensorBus_DebugInit(Debug_PutString);
    #endif
    
    #if OEBUS_DEBUG_UART
    OEBUS_DebugInit(Debug_PutString);
    #endif

    uint8 polling_nr = 0;
    addFieldsensor(0x0C);
    addFieldsensor(0x09);
    addFieldsensor(0x0A);
    addFieldsensor(0x0B);
   
    //Initialise components
    
    OEBUS_Start();

    SensorBus_Start();
    
    CyGlobalIntEnable;
    
    for(;;)
    {   
        #if SensorBus_DEBUG_UART
        debugUart();
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
