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
#include "..\..\Includes\avs_structs.h"

#define DISABLE_POLLING 0

Sensoroe oe;

void SetVentilState(uint8 state)
{
    oe.ventil = state;
    Ventil_SetState(state);
}

uint8 GetOebusAddr()
{
    return OEBUS_GetAddress();
}

void SetOebusAddr(uint8 addr)
{
    oe.addr = addr;
    OEBUS_SetAddress(addr);
}

void addFieldsensor(uint8 addr)
{
    oe.fieldsensors[oe.fieldsensor_connected] = malloc(sizeof(Fieldsensor));
    oe.fieldsensors[oe.fieldsensor_connected]->addr = addr;
    oe.fieldsensors[oe.fieldsensor_connected]->status = FS_STATUS_OFFLINE;
    oe.fieldsensors[oe.fieldsensor_connected]->type = SensorBus__UNKNOWN;
    oe.fieldsensors[oe.fieldsensor_connected]->values[FS_VALUE_HIGH] = 0;
    oe.fieldsensors[oe.fieldsensor_connected]->values[FS_VALUE_LOW] = 0;
    ++oe.fieldsensor_connected;
    #if defined(DEBUG_UART)
        printf("Fieldsensor added\t[ 0x%02X ]\n\r", addr);  
    #endif
    
}

void pollFieldsensor(Fieldsensor* fs)
{
    #if !DISABLE_POLLING
    if (SensorBus_SensorPoll(fs->addr, fs->values, &fs->type)) {
        #if OEBUS_DEBUG_UART
        if (fs->status == FS_STATUS_OFFLINE) {
            printf("Sensor Detected:\n\r");
        }
        #endif
        fs->status = FS_STATUS_ONLINE;
    } else {
        fs->status = FS_STATUS_OFFLINE;
    }
    #endif
}

void sendFieldsensors(uint8 receiver)
{
    uint8 i = 0;
    while(oe.fieldsensors[i] != 0) {++i;}
    OEBUS_PutTxMessage(receiver, i*5, RES_OE_FS_DATA);
    i = 0;
    while(oe.fieldsensors[i] != 0) {
        OEBUS_PutTxMessageArg(oe.fieldsensors[i]->addr);
        OEBUS_PutTxMessageArg(oe.fieldsensors[i]->type);
        OEBUS_PutTxMessageArg(oe.fieldsensors[i]->status);
        OEBUS_PutTxMessageArg(oe.fieldsensors[i]->values[FS_VALUE_HIGH]);
        OEBUS_PutTxMessageArg(oe.fieldsensors[i]->values[FS_VALUE_LOW]);
        ++i;
    }
}

Fieldsensor* findFieldsensor(uint8 sensor_addr)
{
    uint8 i;
    for (i = 0; i < oe.fieldsensor_connected; ++i) {
        if (oe.fieldsensors[i]->addr == sensor_addr)
            return oe.fieldsensors[i];
    }
    return NULL;    
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
            SetVentilState(msg.args[0]);
            OEBUS_PutTxMessage(msg.transmitter, 1, RES_OE_VENTIL);
            OEBUS_PutTxMessageArg(oe.ventil);
            break;
        default:
            break;
    }
    OEBUS_ClearRxMessage();
}

void scanFieldsensors(uint8 import)
{
    //0x09 - 0x78
    uint8 scan_addr;
    uint8 dummy;
    #if defined(DEBUG_UART)
        printf("Scanning\n\r");
    #endif
    for (scan_addr = 0x08; scan_addr <= 0x78; ++scan_addr) {
        if (scan_addr == SensorBus_GetAddress())
            continue;
        
        if (SensorBus_Read(scan_addr, &dummy, 1, 0)) {
            #if defined(DEBUG_UART)
                printf("Found Sensor:\t [ 0x%02X ]\n\r", scan_addr);
            #endif
            if (import == 1 && findFieldsensor(scan_addr) == NULL)
                addFieldsensor(scan_addr);
        }
    }
    #if defined(DEBUG_UART)
        printf("Done\n\r");
    #endif
}

#if defined(DEBUG_UART)

   
void debugListFieldsensors()
{
    uint8 i;
    for (i=0; i < oe.fieldsensor_connected; ++i)
    {
        printf("%d\t Addr:\t [ 0x%02X ]\t Status: [ 0x%02X ]\n\r", i, oe.fieldsensors[i]->addr, oe.fieldsensors[i]->status);
    }
}

void debugConfig()
{
    printf("SensorOe config\n\r\n\r");
    printf("Kar Addr:\t [ 0x%02X ]\n\r", oe.addr);
    printf("Ventil State:\t [ 0x%02X ]\n\r", oe.ventil);
    printf("Fieldsensors:\t [ 0x%02X ]\n\r", oe.fieldsensor_connected);
    printf("====================================================\n\r");
    debugListFieldsensors();
}

void DebugHandle(const char ch) {
   
    Fieldsensor* fs = 0;
    
    switch(ch)
    {
        case 27:
            printf("SensorOE");
            break;
        case '?':
            printf("Commands\n\r\n\r");
            printf("0-9\tShow Fieldsensor 0-9\n\r");
            printf("a\tAdd Fieldsensor 0x09\n\r");
            printf("l\tList Fieldsensors\n\r");
            printf("s\tScan Fieldsensors\n\r");
            printf("S\tScan and Add Fieldsensors\n\r");
            printf("f\tSend fieldsensors to 0x01\n\r");
            printf("o\tOpen ventil\n\r");
            printf("O\tClose ventil\n\r");
            printf("p\tPrint config\n\r");
            printf("\n\r");
            break;
        case 'o':
            SetVentilState(VENTIL_OPEN);
            break;
        case 'O':
            SetVentilState(VENTIL_CLOSE);
            break;
        case 'f':
            sendFieldsensors(0x01);
            break;
        case 'a':
            addFieldsensor(0x09);
            break;
        case 's':
            scanFieldsensors(0);
            break;
        case 'S':
            scanFieldsensors(1);
            break;
        case 'l':
            debugListFieldsensors();
            break;
        case 'p':
            debugConfig();
            break;
        case '0'...'9':
            fs = oe.fieldsensors[ch - '0'];
            if (fs != 0) {
                printf("Sensor\t\t0\n\r");
                printf("ID\t\t0x%02X\n\r", fs->addr);
                     
                printf("Status:\t\t0x%02X (", fs->status);
                
                switch(fs->status)
                {
                    case FS_STATUS_OFFLINE:
                        printf("OFFLINE");
                        break;
                    case FS_STATUS_ONLINE:
                        printf("ONLINE");
                        break;
                    default:
                        printf("UNKNOWN");
                        break;
                }
                printf(")\n\r");
                              
                printf("Type:\t\t0x%02X \n\r", fs->type);
                       
                printf("Value High:\t%d\n\r", fs->values[FS_VALUE_HIGH]);
                printf("Value Low:\t%d\n\r", fs->values[FS_VALUE_LOW]);
                printf("Value Total:\t%d,%d\n\r\n\r", fs->values[FS_VALUE_HIGH], (fs->values[FS_VALUE_LOW]>>7)*5);
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
   
    //Initialise components
    
    Ventil_Start();
    OEBUS_Start();
    SensorBus_Start();
    
    oe.addr  = GetOebusAddr();
    SetVentilState(VENTIL_CLOSE);
    
    CyGlobalIntEnable;
    
    for(;;)
    {   
        #if defined(DEBUG_UART)
            Debug_Communicate();
        #endif 
        
        if (OEBUS_ReadRxStatus() == OEBUS_MSG_READY)
            parseOEBUS();
            
        if (oe.fieldsensor_connected == 0)
            scanFieldsensors(1);
        
        //We poll a single sensor, before we loop around, to be sure we don't spend to much time polling
        //Could be empty array if no sensors are added
		if (oe.fieldsensor_connected > 0) {
			if (oe.fieldsensors[polling_nr] != 0)
				pollFieldsensor(oe.fieldsensors[polling_nr]);
		
			if (oe.fieldsensors[++polling_nr] == 0 )
				polling_nr = 0;
		}
        
        CyDelay(1);
    }
}

/* [] END OF FILE */
