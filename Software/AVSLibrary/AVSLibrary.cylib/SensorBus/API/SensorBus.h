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
#if !defined(`$INSTANCE_NAME`_H_)
#define `$INSTANCE_NAME`_H_

#include <cytypes.h>
#include "..\..\Includes\avs_debug.h"
#include "..\..\Includes\avs_enums.h"
    
`#cy_declare_enum SType`   
    
enum
{
    `$INSTANCE_NAME`_REQ_VALUE  = 1,
    `$INSTANCE_NAME`_REQ_TYPE = 2,
    `$INSTANCE_NAME`_RES_READY = 3,
};
    
    
#define `$INSTANCE_NAME`_DEBUG_UART                 (`$SENSOR_BUS_DEBUG`)
#define `$INSTANCE_NAME`_TYPE                       (`$SENSOR_TYPE`)
#define `$INSTANCE_NAME`_MASTER_ID                  (0x08u)
#define `$INSTANCE_NAME`_ADDRESS_INIT               (`$SENSOR_ADDRESS`)
#define `$INSTANCE_NAME`_FIELDSENSOR                (`$SENSOR_TYPE` != `$INSTANCE_NAME`__SENSOROE)   

void `$INSTANCE_NAME`_Start();
void `$INSTANCE_NAME`_Communicate();

uint8 `$INSTANCE_NAME`_Read(uint8 addr, uint8 * str, uint8 len, uint8 blocking_mode);
uint8 `$INSTANCE_NAME`_Write(uint8 addr, uint8 * str, uint8 len, uint8 blocking_mode);
uint8 `$INSTANCE_NAME`_ParseRead();
uint8 `$INSTANCE_NAME`_ParseWrite();

uint8 `$INSTANCE_NAME`_GetAddress();
void `$INSTANCE_NAME`_ChangeAddress(uint8 addr);


#if `$INSTANCE_NAME`_FIELDSENSOR
    //Fielsensor Only
    uint8 `$INSTANCE_NAME`_GetHigh(uint8 buffer);
    uint8 `$INSTANCE_NAME`_GetLow(uint8 buffer);

    void  `$INSTANCE_NAME`_ChangeMode(uint8 mode);
    void  `$INSTANCE_NAME`_RefreshData();

    void  `$INSTANCE_NAME`_LoadValue(uint8 high, uint8 low);
    void  `$INSTANCE_NAME`_LoadValue_Float32(float32 value);
#else
    //Sensoroe only
    void   `$INSTANCE_NAME`_Scan();
    uint8  `$INSTANCE_NAME`_SensorPoll(uint8 addr, uint8 *values, uint8 *type);
    void   `$INSTANCE_NAME`_SensorChangeMode(uint8 addr, uint8 mode);
#endif


#if `$INSTANCE_NAME`_DEBUG_UART
    void  `$INSTANCE_NAME`_DebugHandle(const char ch);
    void  `$INSTANCE_NAME`_DebugState();
#endif

#endif
/* [] END OF FILE */
