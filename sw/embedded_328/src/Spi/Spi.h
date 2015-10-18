
#ifndef SPI_H_
#define SPI_H_

#include "Std_Types.h"
#include "Spi_Lcfg.h"
#include "Spi_Cfg.h"
#include "Gpio.h"

#define SPI_CALLBACK_NULL_PTR ((void*)0)

typedef void (*Spi_callbackType)(void);

typedef enum
{
	SPI_INTERRUPT_DISABLED = 0U,
	SPI_INTERRUPT_ENABLED
}Spi_spiInterruptControlType;

typedef enum
{
	SPI_MODULE_DISABLED = 0U,
	SPI_MODULE_ENABLED
}Spi_spiEnableControlType;

typedef enum
{
	SPI_LSB_FIRST = 0U,
	SPI_MSB_FIRST
}Spi_spiDataOrderControlType;

typedef enum
{
	SPI_SLAVE_MODE = 0U,
	SPI_MASTER_MODE
}Spi_spiMasterSlaveControlType;

typedef enum
{
	SPI_CPOL_0 = 0U,
	SPI_CPOL_1
}Spi_spiClockPolarityControlType;

typedef enum
{
	SPI_CPHA_0 = 0U,
	SPI_CPHA_1
}Spi_spiDataPhaseControlType;

/*
* CKP: 0, DAP: 0
* Clock Idle: High
* Data Phase: Latch on Trailing Edge
* ________     ___     ___     ___     ___     ___     ___     ___     ___     _____
*         |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|
* __________ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___
* __________/ D7\___/ D6\___/ D5\___/ D4\___/ D4\___/ D3\___/ D2\___/ D1\___/ D0\___
*             ^       ^       ^       ^       ^       ^       ^       ^       ^
*             |       |       |       |       |       |       |       |       |
*___________________________________________________________________________________
* CKP: 0, DAP: 1
* Clock Idle: High
* Data Phase: Latch on Leading Edge
* ________     ___     ___     ___     ___     ___     ___     ___     ___     _____
*         |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|  
* ______ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ _______
* ______/ D7\___/ D6\___/ D5\___/ D4\___/ D4\___/ D3\___/ D2\___/ D1\___/ D0\_______
*         ^       ^       ^       ^       ^       ^       ^       ^       ^
*         |       |       |       |       |       |       |       |       |
*____________________________________________________________________________________
* CKP: 1, DAP: 0
* Clock Idle: Low
* Data Phase: Latch on Trailing Edge
*          ___     ___     ___     ___     ___     ___     ___     ___     ___
* ________|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |_____
* __________ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___
* __________/ D7\___/ D6\___/ D5\___/ D4\___/ D4\___/ D3\___/ D2\___/ D1\___/ D0\___
*             ^       ^       ^       ^       ^       ^       ^       ^       ^
*             |       |       |       |       |       |       |       |       |
*___________________________________________________________________________________
* CKP: 1, DAP: 1
* Clock Idle: Low
* Data Phase: Latch on Leading Edge
*          ___     ___     ___     ___     ___     ___     ___     ___     ___  
* ________|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |______
* ______ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ________
* ______/ D7\___/ D6\___/ D5\___/ D4\___/ D4\___/ D3\___/ D2\___/ D1\___/ D0\________
*         ^       ^       ^       ^       ^       ^       ^       ^       ^
*         |       |       |       |       |       |       |       |       |
*____________________________________________________________________________________
*/

typedef enum
{
	SPI_CLOCK_PRESCALER_DIV_4 = 0U,
	SPI_CLOCK_PRESCALER_DIV_16,
	SPI_CLOCK_PRESCALER_DIV_64,
	SPI_CLOCK_PRESCALER_DIV_128
}Spi_spiClockPrescalerControlType;

typedef enum
{
	SPI_DOUBLE_SPEED_DISABLED = 0U,
	SPI_DOUBLE_SPEED_ENABLED
}Spi_spiDoubleSpeedControlType;

typedef enum
{
	SPI_CS_ACTIVE_LOW = 0U,
	SPI_CS_ACTIVE_HIGH
}Spi_csActiveLevelControlType;

typedef struct
{
	Spi_spiEnableControlType         Spi_spiControl_e;
	Spi_spiMasterSlaveControlType    Spi_MasterSlaveControl_e;
	Spi_spiInterruptControlType      Spi_interruptControl_e;
	Spi_callbackType                 Spi_callback_pv;
	Spi_spiDataOrderControlType      Spi_dataOrderControl_e;
	Spi_spiClockPolarityControlType  Spi_cpol_e;
	Spi_spiDataPhaseControlType      Spi_cpha_e;
	Spi_spiClockPrescalerControlType Spi_clockPrescaler_e;
	Spi_spiDoubleSpeedControlType    Spi_doubleSpeed_e;
}Spi_ConfigType;

typedef struct
{
   uint8* Spi_ControlRegister_pui8;
   uint8* Spi_SpecialFunctionRegister_pui8;
   uint8* Spi_DataRegister_pui8;
}Spi_RegisterAddressType;


void Spi_init(void);
uint8 Spi_sendData(uint8 data_ui8, Gpio_ChannelType Spi_chipSelect_e, Spi_csActiveLevelControlType Spi_csLevel_e, uint8 start, uint8 stop);
static void Spi_highCS(Gpio_ChannelType Spi_chipSelect_e);
static void Spi_lowCS(Gpio_ChannelType Spi_chipSelect_e);


#endif
