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

CY_ISR_PROTO(UART_SLAVE_RX);

enum {
    RX_IDLE = 0,
    RX_LEN_HIGH = 1,
    RX_LEN_LOW = 2,
    RX_CMD = 3,
    RX_ARGS = 4,
    RX_READY = 5,
};

enum {
    I2C_OFFLINE = 0,
    I2C_ONLINE = 1,
};

#define fs_number 5
uint8 fs_types[fs_number] = {0};
uint16 fs_values[fs_number] = {0};
uint8 fs_status[fs_number] = {0};
uint8 fs_ids[fs_number] = {0};

uint8_t listening = 0;
uint8_t address = 'q';

uint8 cmds[50] = {0};
uint8* cmd_start = 0;
uint8* cmd_stop = 0;

uint8_t cmd_rx_state = RX_IDLE;
uint16_t cmd_rx_len = 0;

char debug_msg[50];

CY_ISR(UART_SLAVE_RX) {
    uint32	RxStatus;
    uint8_t ch;
    uint8_t chs;
    RxStatus = UART_SLAVE_GetRxInterruptSource();
    if (RxStatus & UART_SLAVE_INTR_RX_PARITY_ERROR) {
        UART_SLAVE_ClearRxInterruptSource(UART_SLAVE_INTR_RX_PARITY_ERROR);
        chs = UART_SLAVE_SpiUartReadRxData();
        while(chs != 0)
        {
            ch = chs;
            chs = UART_SLAVE_SpiUartReadRxData();
        }
        if (ch == address)
        {
            listening = 1;
            cmd_start = &cmds[0];
            cmd_stop = &cmds[0];
            cmd_rx_state = RX_LEN_HIGH;
        }
    } else if (listening) {
        ch = UART_SLAVE_SpiUartReadRxData();
        *cmd_start = ch;
        if (cmd_start != &cmds[49])
            ++cmd_start;
        else
            cmd_start = &cmds[0];  
    }
    UART_SLAVE_SpiUartClearRxBuffer();
    UART_SLAVE_ClearRxInterruptSource(UART_SLAVE_INTR_RX_NOT_EMPTY | UART_SLAVE_INTR_RX_PARITY_ERROR);
}

void startMsg(uint8 cmd, uint16 len) {
    UART_SLAVE_UartPutChar(len >> 8);
    UART_SLAVE_UartPutChar(len);
    UART_SLAVE_UartPutChar(cmd);
}

void addToMsg(uint8 ch) {
    UART_SLAVE_UartPutChar(ch);
}

void verifyCmd() {
    //Verifying command
    while (cmd_start != cmd_stop) {
        sprintf(debug_msg, "%02x - %d\r\n", *cmd_stop, cmd_rx_state);
        UART_PC_PutString(debug_msg);
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
                    listening = 0;
                }
                break;
            case RX_ARGS:
                if (cmd_stop != &cmds[49])
                    ++cmd_stop;
                else
                    cmd_stop = &cmds[0];
                if (--cmd_rx_len == 0) {
                    cmd_rx_state = RX_READY;
                    listening = 0;
                }
                break;
            case RX_IDLE:
                UART_PC_PutString("Idle error\r\n");
                cmd_stop = cmd_start;
                break;
            default:
                break;
        }
    }
}

void parseCmd() {
    uint8 i = 0;
    uint8 fs_online = 0;
    if(cmd_rx_state == RX_READY) {
            cmd_rx_state = RX_IDLE;
            if (cmds[2] == 'v') {
                UART_PC_PutString("Replying with values\r\n");
                i = 0;
                while (i < fs_number) {
                    if (fs_status[i] == I2C_ONLINE) {
                        ++fs_online;  
                        
                    }
                    ++i;
                }
                UART_PC_PutChar(fs_online + '0');
                startMsg('V', 3*fs_online);
                i = 0;
                while (i < fs_number) {
                    if (fs_status[i] == I2C_ONLINE) {
                        addToMsg(fs_ids[i]);
                        addToMsg(fs_values[i] >> 8);   // Temp hele grader
                        addToMsg(fs_values[i]);  // Temp halve grader
                        
                    }
                    ++i;
                }
            }
            if (cmds[2] == 't') {
                UART_PC_PutString("Replying with type\r\n");
                startMsg('T', 2);
                i = 0;
                while(i < fs_number)
                {
                    if (fs_ids[i] == cmds[3]) {
                        addToMsg(cmds[3]);              //ID
                        addToMsg(fs_types[i]);    //TYPE
                    }
                    ++i;
                }
            }
        }
}

uint8 testFieldsensor(uint8 fieldsensor){
    uint32 status;
    uint8 reply = 0;
    status = I2C_MASTER_I2CMasterSendStart(fieldsensor, I2C_MASTER_I2C_READ_XFER_MODE);
    if(status == I2C_MASTER_I2C_MSTR_NO_ERROR) {
        I2C_MASTER_I2CMasterReadByte(I2C_MASTER_I2C_NAK_DATA);
        reply = 1;
    }
    I2C_MASTER_I2CMasterSendStop();
    return reply;
}

void pollFieldsensor(uint8 index) {
    uint8 data[2] = {0};
    //Not valid address
    if (fs_ids[index] == 0) {
        fs_status[index] = I2C_OFFLINE;
        return;
    }
    if (testFieldsensor(fs_ids[index])) {
        
        I2C_MASTER_I2CMasterReadBuf(0x08, data, 2, I2C_MASTER_I2C_MODE_COMPLETE_XFER);
        while (!(I2C_MASTER_I2CMasterStatus() & I2C_MASTER_I2C_MSTAT_RD_CMPLT));
        fs_values[index] = data[0];
        fs_values[index] <<= 8;
        fs_values[index] += data[1];
        if (fs_types[index] == 0 || fs_status[index] == I2C_OFFLINE) {
            data[0] = 1;
            I2C_MASTER_I2CMasterWriteBuf(0x08, data, 1, I2C_MASTER_I2C_MODE_COMPLETE_XFER);
            while (!(I2C_MASTER_I2CMasterStatus() & I2C_MASTER_I2C_MSTAT_WR_CMPLT));
            I2C_MASTER_I2CMasterReadBuf(0x08, data, 1, I2C_MASTER_I2C_MODE_COMPLETE_XFER);
            while (!(I2C_MASTER_I2CMasterStatus() & I2C_MASTER_I2C_MSTAT_RD_CMPLT));
            I2C_MASTER_I2CMasterReadBuf(0x08, data, 1, I2C_MASTER_I2C_MODE_COMPLETE_XFER);
            while (!(I2C_MASTER_I2CMasterStatus() & I2C_MASTER_I2C_MSTAT_RD_CMPLT));
            fs_types[index] = data[0];
            data[0] = 2;
            I2C_MASTER_I2CMasterWriteBuf(0x08, data, 1, I2C_MASTER_I2C_MODE_COMPLETE_XFER);
            while (!(I2C_MASTER_I2CMasterStatus() & I2C_MASTER_I2C_MSTAT_WR_CMPLT));
        }
        fs_status[index] = I2C_ONLINE;
    } else {
        fs_status[index] = I2C_OFFLINE;
    }
}

int main()
{
    char ch;
    uint8 cmd = 1;
    uint8 polling_nr = 0;
    uint8 i = 0;
    
    fs_ids[0] = 0x08;    
    
    Clock_Start();
    UART_SLAVE_Start();
    I2C_MASTER_Start();
    UART_PC_Start();
    isr_SLAVE_RX_StartEx(UART_SLAVE_RX);
    CyGlobalIntEnable;
    
    UART_PC_PutString("Initializing: Done\r\n");
    for(;;)
    {
        //I2C_MASTER_I2CMasterReadBuf(0x08, &data, 1, I2C_MASTER_I2C_MODE_COMPLETE_XFER);
        //while (!(I2C_MASTER_I2CMasterStatus() & I2C_MASTER_I2C_MSTAT_RD_CMPLT));
           
        ch = UART_PC_GetChar();
        switch(ch)
        {
            case 'u':
                cmd -= 1;
                cmd = !cmd;
                cmd += 1;
                UART_PC_PutChar(I2C_MASTER_I2CMasterWriteBuf(0x08, &cmd, 1, I2C_MASTER_I2C_MODE_COMPLETE_XFER) + '0');
                while (!(I2C_MASTER_I2CMasterStatus() & I2C_MASTER_I2C_MSTAT_WR_CMPLT));
            case 'c':
                break;
            case 's':
                i = 0;
                while (i < fs_number) {
                    if (fs_status[i] == I2C_OFFLINE) {
                        sprintf(debug_msg, "Index: %d, ID: %d, OFFLINE\r\n", i, fs_ids[i]);
                    } else {
                        sprintf(debug_msg, "Index: %d, ID: %d, ONLINE - Value: %d, Type: %d\r\n", i, fs_ids[i], fs_values[i], fs_types[i]);
                    }
                    UART_PC_PutString(debug_msg);
                    ++i;
                }
                break;
            default:
                break;
        }
        
       verifyCmd();
    
       parseCmd();
    
       pollFieldsensor(polling_nr);
    
        CyDelay(50);
        //UART_PC_PutChar('.');
    
       if (++polling_nr == fs_number)
            polling_nr = 0;
    }
}

/* [] END OF FILE */
