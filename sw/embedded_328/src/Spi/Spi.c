
#include "Spi.h"
#include "Spi_Cfg.h"

static Spi_ConfigType		*LcfgConfigPtr_ps;
static Spi_CallbackType		 Spi_CallbackLocal_pv;
static Spi_SpiInterruptControlType  Spi_InterruptControlLocal_e;

void Spi_Callback()
{

}

volatile const Spi_RegisterAddressType Spi_RegisterAdress_as =
{
    (uint8*) SPI_SPCR_ADDRESS, 
    (uint8*) SPI_SPSR_ADDRESS, 
    (uint8*) SPI_SPDR_ADDRESS 
};

void Spi_Init(void)
{
   Spi_SpiEnableControlType         Spi_SpiControlLocal_e;
   Spi_SpiMasterSlaveControlType    Spi_MasterSlaveControlLocal_e;
   Spi_SpiDataOrderControlType      Spi_DataOrderControlLocal_e;
   Spi_SpiClockPolarityControlType  Spi_CpolLocal_e;
   Spi_SpiDataPhaseControlType      Spi_CphaLocal_e;
   Spi_SpiClockPrescalerControlType Spi_ClockPrescalerLocal_e;
   Spi_SpiDoubleSpeedControlType    Spi_DoubleSpeedLocal_e;
   
   
   LcfgConfigPtr_ps = (Spi_ConfigType*)Spi_GetLcfgData();

   Spi_SpiControlLocal_e         = LcfgConfigPtr_ps->Spi_SpiControl_e;
   Spi_MasterSlaveControlLocal_e = LcfgConfigPtr_ps->Spi_MasterSlaveControl_e;
   Spi_InterruptControlLocal_e   = LcfgConfigPtr_ps->Spi_InterruptControl_e;
   Spi_DataOrderControlLocal_e   = LcfgConfigPtr_ps->Spi_DataOrderControl_e;
   Spi_CpolLocal_e               = LcfgConfigPtr_ps->Spi_Cpol_e;
   Spi_CphaLocal_e               = LcfgConfigPtr_ps->Spi_Cpha_e;
   Spi_ClockPrescalerLocal_e     = LcfgConfigPtr_ps->Spi_ClockPrescaler_e;
   Spi_DoubleSpeedLocal_e        = LcfgConfigPtr_ps->Spi_DoubleSpeed_e;
   Spi_CallbackLocal_pv          = LcfgConfigPtr_ps->Spi_Callback_pv;
   
   *(Spi_RegisterAdress_as.Spi_SpecialFunctionRegister_pui8)  = (Spi_DoubleSpeedLocal_e << SPI_SPR_BIT_POSITION_UI8);
   
   *(Spi_RegisterAdress_as.Spi_ControlRegister_pui8) =  ((Spi_InterruptControlLocal_e   << SPI_SPIE_BIT_POSITION_UI8) | \
																(Spi_SpiControlLocal_e         << SPI_SPE_BIT_POSITION_UI8)  | \
																(Spi_DataOrderControlLocal_e   << SPI_DORD_BIT_POSITION_UI8) | \
																(Spi_MasterSlaveControlLocal_e << SPI_MSTR_BIT_POSITION_UI8) | \
																(Spi_CpolLocal_e               << SPI_CPOL_BIT_POSITION_UI8) | \
																(Spi_CphaLocal_e               << SPI_CPHA_BIT_POSITION_UI8) | \
																(Spi_ClockPrescalerLocal_e     << SPI_SPR_BIT_POSITION_UI8));
}

uint8 Spi_SendData(uint8 data_ui8, Gpio_ChannelType Spi_chipSelect_e, Spi_CsActiveLevelControlType Spi_csLevel_e, uint8 start, uint8 stop)
{
	uint8 return_ui8;
	if(start)
	{
		if(Spi_csLevel_e == SPI_CS_ACTIVE_LOW)
		{
			Spi_LowCS(Spi_chipSelect_e);
		}
		else
		{
			Spi_HighCS(Spi_chipSelect_e);
		}
	}
	
	/* send data */
	*(Spi_RegisterAdress_as.Spi_DataRegister_pui8) = data_ui8;

	if(Spi_InterruptControlLocal_e == SPI_INTERRUPT_DISABLED)
	{
		/* wait for transmission finished bit */
		while (!(*(Spi_RegisterAdress_as.Spi_SpecialFunctionRegister_pui8) & (1 << SPI_SPIF_BIT_POSITION_UI8)));
	}
	else
	{
		/* NOTE: SPI Interrupt and Callback MUST contain valid values in Linktime-Config! */
	}

	if(stop)
	{
		if(Spi_csLevel_e == SPI_CS_ACTIVE_LOW)
		{
			Spi_HighCS(Spi_chipSelect_e);
		}
		else
		{
			Spi_LowCS(Spi_chipSelect_e);
		}
	}	
	return_ui8 = *(Spi_RegisterAdress_as.Spi_DataRegister_pui8);
	return return_ui8;
}

static void Spi_HighCS(Gpio_ChannelType Spi_chipSelect_e)
{
	Gpio_WriteChannel(Spi_chipSelect_e, GPIO_HIGH);
}

static void Spi_LowCS(Gpio_ChannelType Spi_chipSelect_e)
{
	Gpio_WriteChannel(Spi_chipSelect_e, GPIO_LOW);
}

ISR(SPI_STC_vect)
{
	if(Spi_CallbackLocal_pv != NULL)
	{
		Spi_CallbackLocal_pv();
	}
	else
	{
		/* do nothing */
	}
}
