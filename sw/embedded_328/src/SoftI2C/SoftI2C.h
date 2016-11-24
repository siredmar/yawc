//********************************************************************************************************
//
//  Projekt:                HVT
//  Modul:
//  Verfasser:              CAggou
//                          Fa. HSS, Wendelstein, 09129 2852 0
//
//  Kunde:                  Signaltec
//
//  Datum:            		17.03.2016
//
//********************************************************************************************************
#ifndef _I2C_H_
#define _I2C_H_
#include "Std_Types.h"
#include "Gpio.h"

//*****************************************
// Defines
//*****************************************
typedef enum
{
    SOFTI2C_UNIT_0 = (0u),
    SOFTI2C_MAX_NUM_OF_UNITS
} SoftI2C_HwUnitType;

//*****************************************
// Strukturen
//*****************************************
typedef struct
{
    Gpio_ChannelType SclPin_e;
    Gpio_ChannelType SdaPin_e;
} SoftI2C_UnitType;

typedef struct
{
    SoftI2C_HwUnitType HwUnit;
} SoftI2C_DataType;

//*****************************************
// Funktionsprototypen
//*****************************************
void SoftI2C_MasterInit(void);
void SoftI2C_MasterDeInit(void);
void SoftI2C_WriteData(uint8 deviceAddr, uint16 writeAddress, uint8 *writeData, uint8 Length);
void SoftI2C_ReadData(uint8 deviceAddr, uint8 readAddress, uint8 *readBuffer, uint8 Length);



#endif
