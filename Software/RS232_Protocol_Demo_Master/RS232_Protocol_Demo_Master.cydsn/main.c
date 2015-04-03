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
#include <stdio.h>

CY_ISR_PROTO(UART_OE_RX);

enum {
    RX_IDLE = 0,
    RX_LEN_HIGH = 1,
    RX_LEN_LOW = 2,
    RX_CMD = 3,
    RX_ARGS = 4,
    RX_READY = 5,
};

#define fs_number 10
uint8 fs_types[fs_number] = {0};
uint16 fs_values[fs_number] = {0};


uint8 cmds[50] = {0};
uint8* cmd_start = 0;
uint8* cmd_stop = 0;

uint8_t cmd_rx_state = RX_IDLE;
uint16_t cmd_rx_len = 0;

char debug_msg[10];

void startMsg(uint8 receiver, uint8 cmd, uint16 len) {
    UART_OE_TX_WriteControlRegister(16);
    UART_OE_TX_PutChar(receiver);
    while (!(UART_OE_TX_ReadTxStatus() & UART_OE_TX_TX_STS_COMPLETE))
    {}
    UART_OE_TX_WriteControlRegister(8);
    UART_OE_TX_PutChar(len >> 8);
    UART_OE_TX_PutChar(len);
    UART_OE_TX_PutChar(cmd);
}

void addToMsg(uint8 ch) {
    UART_OE_TX_PutChar(ch);
}

CY_ISR(UART_OE_RX) {
    uint8_t ch;
    if(cmd_rx_state == RX_IDLE) {
        cmd_start = &cmds[0];
        cmd_stop = &cmds[0];
        cmd_rx_state = RX_LEN_HIGH;
    }
    ch = UART_OE_RX_ReadRxData();
    *cmd_start = ch;
    if (cmd_start != &cmds[49])
        ++cmd_start;
    else
        cmd_start = &cmds[0];
       
    UART_OE_RX_ClearRxBuffer();
}

void verifyCmd() {
    //Verifying command
    while (cmd_start != cmd_stop) {
        switch(cmd_rx_state)
        {
            case RX_LEN_HIGH:
                ++cmd_rx_state;
                cmd_rx_len = *cmd_stop;
                cmd_rx_len <<= 8;
                ++cmd_stop;
                break;
            case RX_LEN_LOW:
                ++cmd_rx_state;
                cmd_rx_len += *cmd_stop;
                ++cmd_stop;
                break;
            case RX_CMD:
                ++cmd_rx_state;
                ++cmd_stop;
                if (cmd_rx_len == 0) {
                    cmd_rx_state = RX_READY;
                }
                break;
            case RX_ARGS:
                if (cmd_stop != &cmds[49])
                    ++cmd_stop;
                else
                    cmd_stop = &cmds[0];
                if (--cmd_rx_len == 0) {
                    cmd_rx_state = RX_READY;
                }
                break;
            case RX_IDLE:
                UART_PC_UartPutString("Idle error\r\n");
                cmd_stop = cmd_start;
                break;
            default:
                break;
        }
    }
}

int main()
{
    uint8_t ch = 0u;
    uint8 i = 0;
    
    //Init components
    Clock_Start();
    isr_OE_RX_StartEx(UART_OE_RX);
    UART_OE_RX_Start();
    UART_OE_TX_Start();
    UART_PC_Start();

    CyGlobalIntEnable;
    
    UART_PC_UartPutString("Initializing: Done\r\n");
    for(;;)
    {
        //Dummy user input
        ch = UART_PC_UartGetChar();
        if (ch != 0u) {
            switch(ch)
            {
                case 'c':
                    i = 0;
                    while(i < fs_number) {
                        fs_types[i] = 0;
                        fs_values[i] = 0;
                        ++i;
                    }
                    break;
                case 'p':
                    i=0;
                    while(i < fs_number) {
                        sprintf(debug_msg, "ID %d: Value: %d,%d Type: %d\r\n", i, fs_values[i] >> 8, ((fs_values[i] & 128)/128)*5, fs_types[i]);
                        UART_PC_UartPutString(debug_msg);
                        ++i;
                    }
                    break;
                case 'q':
                    UART_OE_TX_WriteControlRegister(16);
                    UART_OE_TX_PutChar('q');
                    break;
                case 'w':
                    UART_OE_TX_WriteControlRegister(8);
                    UART_OE_TX_PutChar('w');
                    break;
                case 'v':
                    startMsg('q','v',0);
                    break;
                case '0':
                    startMsg('q','t',1);
                    addToMsg(ch - '0');
                    break;
                case '1':
                    startMsg('q','t',1);
                    addToMsg(ch - '0');
                    break;
                case '2':
                    startMsg('q','t',1);
                    addToMsg(ch - '0');
                    break;
                case '3':
                    startMsg('q','t',1);
                    addToMsg(ch - '0');
                    break;
                case '8':
                    startMsg('q','t',1);
                    addToMsg(ch - '0');
                    break;
                default:
                    break;
            }
        }
        
        verifyCmd();
        
        //Ready to parse command
        if(cmd_rx_state == RX_READY) {
            cmd_rx_state = RX_IDLE;
            if (cmds[2] == (uint8)'V') {
                UART_PC_UartPutString("Value Reply\r\n");
                cmd_rx_len = cmds[0] << 8;
                cmd_rx_len += cmds[1];
                i = 0;
                while(cmd_rx_len != 0) {
                    UART_PC_UartPutChar(cmds[cmd_rx_len] + '0');
                    fs_values[cmds[cmd_rx_len]] = (cmds[cmd_rx_len+1] << 8) + cmds[cmd_rx_len+2];
                    if (fs_types[cmds[cmd_rx_len]] == 0) {
                        fs_types[cmds[cmd_rx_len]] = 255;
                    }
                    //sprintf(debug_msg, "ID %d: Value %d,%d\r\n", cmds[cmd_rx_len], cmds[cmd_rx_len+2], (cmds[cmd_rx_len+1]/128)*5);
                    //UART_PC_UartPutString(debug_msg);
                    cmd_rx_len -= 3;
                }
                //UART_PC_UartPutChar('\r');
                //UART_PC_UartPutChar('\n');
            }
            if (cmds[2] == 'T') {
                UART_PC_UartPutString("Type Reply\r\n");
                fs_types[cmds[3]] = cmds[4];
                //sprintf(debug_msg, "ID %d: Type %d\r\n\r\n", cmds[3], cmds[4]);
                //UART_PC_UartPutString(debug_msg);
            }
        }
        
        i = 0;
        while(i < fs_number) {
            if(fs_types[i] == 255) {
                fs_types[i] = 254;
                startMsg('q','t',1);
                addToMsg(i);
            }
            if(fs_types[i] == 254) {
                i = fs_number;
            }
            ++i;
        }
    }
}

/* [] END OF FILE */
