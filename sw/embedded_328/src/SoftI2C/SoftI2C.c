//********************************************************************************************************
//
//  Projekt:                HVT
//  Modul:
//  Verfasser:              CAggou
//                          Fa. HSS, Wendelstein, 09129 2852 0
//
//  Kunde:                  Signaltec
//
//  Datum:                  17.03.2016
//
//*******************************************************************************************************

#pragma CHECK_MISRA("none")
#include "string.h"
#include "stdio.h"
#include <msp430.h>
#include <ctype.h>
#pragma RESET_MISRA("all")

#include "Std_Types.h"
#include "SoftI2C.h"
#include "Gpio.h"
#include "Timer.h"
#include "EcuM.h"

SoftI2C_UnitType HwUnitPinDefinition[SOFTI2C_MAX_NUM_OF_UNITS] =
{
        {
                GPIO_CHANNEL_P1_7, /* Sensor: SCL Pin */
                GPIO_CHANNEL_P1_6, /* Sensor: SDA Pin */
        }
};

#define SOFTI2C_DELAY_USEC (4u)
#define SOFTI2C_READ       (1u)
#define SOFTI2C_WRITE      (0u)

static uint8 SoftI2C_MasterRead(void);
static bool SoftI2C_MasterWrite(uint8 data);
static bool SoftI2C_MasterStart(uint8 addressRW, uint8 RW);
static void SoftI2C_MasterStop(void);

SoftI2C_DataType SoftI2C_Data_s;

void SoftI2C_MasterInit(void)
{
    SoftI2C_Data_s.HwUnit = SOFTI2C_UNIT_0;

    Gpio_WriteChannelWithDelayUs(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SclPin_e, GPIO_HIGH, SOFTI2C_DELAY_USEC);
    Gpio_WriteChannelWithDelayUs(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SdaPin_e, GPIO_HIGH, SOFTI2C_DELAY_USEC);
}

void SoftI2C_MasterDeInit(void)
{
    Gpio_SwitchPinDirection(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SclPin_e, GPIO_INPUT);
    Gpio_SwitchPinDirection(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SdaPin_e, GPIO_INPUT);
}

static uint8 SoftI2C_MasterRead(void)
{
    uint8 b = 0u;
    uint8 i = 0u;

    Gpio_SwitchPinDirection(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SdaPin_e, GPIO_INPUT);
    // Read byte
    for (i = 0; i < 8; i++)
    {
        b <<= 1;
        Gpio_WriteChannelWithDelayUs(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SclPin_e, GPIO_HIGH, SOFTI2C_DELAY_USEC);
        if ((uint8)Gpio_ReadChannel(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SdaPin_e)) //(bit_is_set(PINB, TWI_SDA_PIN))
        {
            b |= 1;
        }
        Gpio_WriteChannelWithDelayUs(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SclPin_e, GPIO_LOW, SOFTI2C_DELAY_USEC);
    }
    SoftTimer_DelayUsBlocking(SOFTI2C_DELAY_USEC);

    Gpio_SwitchPinDirection(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SdaPin_e, GPIO_OUTPUT);
    Gpio_WriteChannelWithDelayUs(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SdaPin_e, GPIO_LOW, SOFTI2C_DELAY_USEC);
    Gpio_WriteChannelWithDelayUs(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SclPin_e, GPIO_HIGH, 1);
    Gpio_WriteChannelWithDelayUs(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SdaPin_e, GPIO_LOW, SOFTI2C_DELAY_USEC);
    Gpio_WriteChannelWithDelayUs(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SclPin_e, GPIO_LOW, SOFTI2C_DELAY_USEC);

    return b;
}

static bool SoftI2C_MasterWrite(uint8 data)
{
    uint8 rtn = 0;
    uint8 m = 0u;
    // Write byte
    for (m = 0x80; m != 0; m >>= 1)
    {
        // Don't change this loop unless you verify the change with a scope
        if (m & data)
        {
            Gpio_WriteChannelWithDelayUs(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SdaPin_e, GPIO_HIGH, SOFTI2C_DELAY_USEC);
        }
        else
        {
            Gpio_WriteChannelWithDelayUs(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SdaPin_e, GPIO_LOW, SOFTI2C_DELAY_USEC);
        }
        Gpio_WriteChannelWithDelayUs(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SclPin_e, GPIO_HIGH, SOFTI2C_DELAY_USEC);
        Gpio_WriteChannelWithDelayUs(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SclPin_e, GPIO_LOW, 1);
    }
    Gpio_WriteChannelWithDelayUs(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SdaPin_e, GPIO_LOW, 1);
    Gpio_SwitchPinDirection(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SdaPin_e, GPIO_INPUT);
    Gpio_WriteChannelWithDelayUs(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SclPin_e, GPIO_HIGH, 1);
    rtn = (uint8)Gpio_ReadChannel(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SdaPin_e);//bit_is_set(PINB, TWI_SDA_PIN);
    Gpio_SwitchPinDirection(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SdaPin_e, GPIO_OUTPUT);
    Gpio_WriteChannelWithDelayUs(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SclPin_e, GPIO_LOW, 3);
    return rtn;
}

// Issue a start condition
static bool SoftI2C_MasterStart(uint8 addressRW, uint8 RW)
{
    Gpio_SwitchPinDirection(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SdaPin_e, GPIO_OUTPUT);
    Gpio_SwitchPinDirection(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SclPin_e, GPIO_OUTPUT);
    Gpio_WriteChannelWithDelayUs(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SdaPin_e, GPIO_HIGH, 1);
    Gpio_WriteChannelWithDelayUs(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SclPin_e, GPIO_HIGH, SOFTI2C_DELAY_USEC);
    Gpio_WriteChannelWithDelayUs(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SdaPin_e, GPIO_LOW, SOFTI2C_DELAY_USEC);
    Gpio_WriteChannelWithDelayUs(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SclPin_e, GPIO_LOW, SOFTI2C_DELAY_USEC);
    return SoftI2C_MasterWrite((addressRW << 1) | RW);
}

// Issue a stop condition
static void SoftI2C_MasterStop(void)
{
    Gpio_WriteChannelWithDelayUs(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SdaPin_e, GPIO_LOW, SOFTI2C_DELAY_USEC);
    Gpio_WriteChannelWithDelayUs(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SclPin_e, GPIO_HIGH, SOFTI2C_DELAY_USEC);
    Gpio_WriteChannelWithDelayUs(HwUnitPinDefinition[SoftI2C_Data_s.HwUnit].SdaPin_e, GPIO_HIGH, SOFTI2C_DELAY_USEC);
}

void SoftI2C_WriteData(uint8 deviceAddr, uint16 writeAddress, uint8 *writeData, uint8 Length)
{
    uint8 Bytes = 0u;

    SoftI2C_MasterStart(deviceAddr, SOFTI2C_WRITE);
    SoftI2C_MasterWrite(writeAddress);
    for(Bytes = 0; Bytes < Length; Bytes++)
    {
       SoftI2C_MasterWrite(writeData[Bytes]);
    }

    SoftI2C_MasterStop();
}

void SoftI2C_ReadData(uint8 deviceAddr, uint8 readAddress, uint8 *readBuffer, uint8 Length)
{
    uint8 i = 0u;
    SoftI2C_MasterStart(deviceAddr, SOFTI2C_WRITE);
    SoftI2C_MasterWrite(readAddress);
    SoftI2C_MasterStop();
    SoftI2C_MasterStart(deviceAddr, SOFTI2C_READ);

    for (i = 0; i < Length; i++)
    {
        readBuffer[i] = SoftI2C_MasterRead();
    }

    SoftI2C_MasterStop();
}
