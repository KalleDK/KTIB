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

#include "../../Includes/OE_Bus.h"
#include "../../Includes/Kar_Bus.h"

#ifdef DEBUG
#include <stdio.h>
#endif

volatile RS485Bus* oebus;
volatile RS485Bus* karbus;

#ifdef DEBUG
char debug_msg[50];
void printMessage(volatile RS485Bus* bus);
#endif

CY_ISR_PROTO(RX485_OEBUS);
CY_ISR_PROTO(RX485_KARBUS);

//Debug UART
#ifdef DEBUG    
void printRS485Bus(volatile RS485Bus* bus) {
        uint16 len;
        uint16 i = 0;
        len = bus->msg[RS_LEN_HIGH];
        len <<= 8;
        len += bus->msg[RS_LEN_LOW];
        sprintf(debug_msg, "Status:\t\t0x%02X (", bus->status);
        UART_PC_PutString(debug_msg);
        UART_PC_PutString(msg_status_str(bus->status));
        UART_PC_PutString(")\r\n");
        sprintf(debug_msg, "Receiver:\t0x%02X\r\n", bus->msg[RS_RECEIVER]);
        UART_PC_PutString(debug_msg);
        sprintf(debug_msg, "Sender:\t\t0x%02X\r\n", bus->msg[RS_TRANSMITTER]);
        UART_PC_PutString(debug_msg);
        sprintf(debug_msg, "Len High:\t%d\r\n", bus->msg[RS_LEN_HIGH]);
        UART_PC_PutString(debug_msg);
        sprintf(debug_msg, "Len Low:\t%d\r\n", bus->msg[RS_LEN_LOW]);
        UART_PC_PutString(debug_msg);
        sprintf(debug_msg, "Len Toal:\t%d\r\n", len);
        UART_PC_PutString(debug_msg);
        sprintf(debug_msg, "Missing Args:\t%d\r\n", bus->missing_args);
        UART_PC_PutString(debug_msg);
        sprintf(debug_msg, "Command:\t0x%02X (", bus->msg[RS_CMD]);
        UART_PC_PutString(debug_msg);
        UART_PC_PutString(oeb_cmd_str(bus->msg[RS_CMD]));
        UART_PC_PutString(")\r\n");
        UART_PC_PutString("Message:\t");
        for(i = 0; i < len; ++i) {
            sprintf(debug_msg, "0x%02X ", bus->msg[RS_ARG_START+i]);
            UART_PC_PutString(debug_msg); 
        }
        UART_PC_PutString("\r\n\r\n");
}
    
void debugUart() {
    char ch;
    ch = UART_PC_GetChar();
 
    switch(ch)
    {
        case '?':
            UART_PC_PutString("Commands\r\n\r\n");
            UART_PC_PutString("k\tShow karbus message\r\n");
            UART_PC_PutString("c\tClear karbus message\r\n");
            UART_PC_PutString("r\tRequest Fieldsensors from 0x03\r\n");
            UART_PC_PutString("o\tOpen ventil at 0x03\r\n");
            UART_PC_PutString("O\tClose ventil at 0x03\r\n");
            UART_PC_PutString(".\tToggle address 0x03 0x04\r\n");
            UART_PC_PutString("\r\n");
            break;
        case 'o':
            UART_PC_PutString("Opening ventil\r\n\r\n");
            sendCommand(oebus, 0x03, 1, OEB_REQ_VENTIL);
            sendArg(oebus,1);
            break;
        case 'O':
            UART_PC_PutString("Closing ventil\r\n\r\n");
            sendCommand(oebus, 0x03, 1, OEB_REQ_VENTIL);
            sendArg(oebus,0);
            break;
        case 'r':
            UART_PC_PutString("Requesting sensors\r\n\r\n");
            sendCommand(oebus, 0x03, 0, OEB_REQ_FS_DATA);
            break;
        case '.':
            if (oebus->busid == 0x03)
                oebus->busid = 0x04;
            else
                oebus->busid = 0x03;
            OEBUS_Master_UartSetRxAddress(oebus->busid);
            sprintf(debug_msg, "ID: 0x%02X\r\n\r\n", oebus->busid);
            UART_PC_PutString(debug_msg);
            
            break;
        case 'k':
            printRS485Bus(oebus);
            break;
        case 'c':
            clearRS485Bus(oebus);
            printRS485Bus(oebus);
            break;
        
        default:
            break;
    }
    
}

#endif

void parseKARBus(volatile RS485Bus* bus) {
    if (bus->status == RS_MSG_READY) {
        switch(bus->msg[RS_CMD]) {
            
            default:
                break;
        }
        #ifdef DEBUG
        printRS485Bus(bus);
        #endif
        clearRS485Bus(bus);
    }
}

void parseOEBus(volatile RS485Bus* bus) {
    if (bus->status == RS_MSG_READY) {
        switch(bus->msg[RS_CMD]) {
            case OEB_RES_FS_DATA:
                
                break;
            case OEB_RES_VENTIL:
                
            break;
            default:
                break;
        }
        #ifdef DEBUG
        printRS485Bus(bus);
        #endif
        clearRS485Bus(bus);
    }
}

CY_ISR(RX485_OEBUS) {
    pushRS485Bus(oebus, OEBUS_Master_UartGetChar());
    OEBUS_Master_ClearRxInterruptSource(OEBUS_Master_INTR_RX_NOT_EMPTY);
}

CY_ISR(RX485_KARBUS) {
    pushRS485Bus(karbus, KARBUS_Slave_UartGetChar());
    KARBUS_Slave_ClearRxInterruptSource(KARBUS_Slave_INTR_RX_NOT_EMPTY);
}

int main()
{    
    //Populate variables
    initRS485Bus(&oebus, 0x04, OEBUS_Master_SpiUartWriteTxData);
    initRS485Bus(&karbus, 0x03, KARBUS_Slave_SpiUartWriteTxData);
    
    //Initialise components
    
    OEBUS_Master_Start();
    isr_RS485_OEBUS_StartEx(RX485_OEBUS);
    isr_RS485_KARBUS_StartEx(RX485_KARBUS);
    
    #ifdef DEBUG
    Clock_Start();
    UART_PC_Start();
    UART_PC_PutString("Initializing: Done\r\n");
    #endif
    
    OEBUS_Master_UartSetRxAddress(oebus->busid);
    KARBUS_Slave_UartSetRxAddress(karbus->busid);
    
    CyGlobalIntEnable;
    
    for(;;)
    {
        #ifdef DEBUG
        debugUart();
        #endif

        verifyRS485Bus(oebus);
        parseOEBus(oebus);
        
        verifyRS485Bus(karbus);
        parseKARBus(karbus);
        
        CyDelay(50);
    }
}

/* [] END OF FILE */
