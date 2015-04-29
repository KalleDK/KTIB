#ifndef AVS_STRUCTS_H_
#define AVS_STRUCTS_H_

typedef struct
{
    uint8 addr;
    uint8 status;
    uint8 type;
    uint8 values[2];
} Fieldsensor;

typedef struct {
    uint8 addr;
    Fieldsensor* fieldsensors[10];
    uint8 fieldsensor_connected;
    uint8 status;
    uint8 ventil;
} Sensoroe;

#endif