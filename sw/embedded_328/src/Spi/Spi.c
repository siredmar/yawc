
#include "Spi.h"
#include "Spi_Cfg.h"

static Spi_ConfigType		*lcfgConfigPtr_ps;
static Spi_callbackType		 Spi_callbackLocal_pv;
static Spi_spiInterruptControlType  Spi_interruptControlLocal_e;

void Spi_Callback()
{

}

volatile const Spi_RegisterAddressType Spi_RegisterAdress_as =
{
    (uint8*) SPI_SPCR_ADDRESS, 
    (uint8*) SPI_SPSR_ADDRESS, 
    (uint8*) SPI_SPDR_ADDRESS 
};

void Spi_init(void)
{
   Spi_spiEnableControlType         Spi_spiControlLocal_e;
   Spi_spiMasterSlaveControlType    Spi_MasterSlaveControlLocal_e;
   Spi_spiDataOrderControlType      Spi_dataOrderControlLocal_e;
   Spi_spiClockPolarityControlType  Spi_cpolLocal_e;
   Spi_spiDataPhaseControlType      Spi_cphaLocal_e;
   Spi_spiClockPrescalerControlType Spi_clockPrescalerLocal_e;
   Spi_spiDoubleSpeedControlType    Spi_doubleSpeedLocal_e;
   
   
   lcfgConfigPtr_ps = (Spi_ConfigType*)Spi_getlcfgdata();

   Spi_spiControlLocal_e         = lcfgConfigPtr_ps->Spi_spiControl_e;
   Spi_MasterSlaveControlLocal_e = lcfgConfigPtr_ps->Spi_MasterSlaveControl_e;
   Spi_interruptControlLocal_e   = lcfgConfigPtr_ps->Spi_interruptControl_e;
   Spi_dataOrderControlLocal_e   = lcfgConfigPtr_ps->Spi_dataOrderControl_e;
   Spi_cpolLocal_e               = lcfgConfigPtr_ps->Spi_cpol_e;
   Spi_cphaLocal_e               = lcfgConfigPtr_ps->Spi_cpha_e;
   Spi_clockPrescalerLocal_e     = lcfgConfigPtr_ps->Spi_clockPrescaler_e;
   Spi_doubleSpeedLocal_e        = lcfgConfigPtr_ps->Spi_doubleSpeed_e;
   Spi_callbackLocal_pv          = lcfgConfigPtr_ps->Spi_callback_pv;
   
   *(Spi_RegisterAdress_as.Spi_SpecialFunctionRegister_pui8)  = (Spi_doubleSpeedLocal_e << SPI_SPR_BIT_POSITION_UI8);
   
   *(Spi_RegisterAdress_as.Spi_ControlRegister_pui8) =  ((Spi_interruptControlLocal_e   << SPI_SPIE_BIT_POSITION_UI8) | \
																(Spi_spiControlLocal_e         << SPI_SPE_BIT_POSITION_UI8)  | \
																(Spi_dataOrderControlLocal_e   << SPI_DORD_BIT_POSITION_UI8) | \
																(Spi_MasterSlaveControlLocal_e << SPI_MSTR_BIT_POSITION_UI8) | \
																(Spi_cpolLocal_e               << SPI_CPOL_BIT_POSITION_UI8) | \
																(Spi_cphaLocal_e               << SPI_CPHA_BIT_POSITION_UI8) | \
																(Spi_clockPrescalerLocal_e     << SPI_SPR_BIT_POSITION_UI8));
}

uint8 Spi_sendData(uint8 data_ui8, Gpio_ChannelType Spi_chipSelect_e, Spi_csActiveLevelControlType Spi_csLevel_e, uint8 start, uint8 stop)
{
	uint8 return_ui8;
	if(start){
		if(Spi_csLevel_e == SPI_CS_ACTIVE_LOW)
		{
			Spi_lowCS(Spi_chipSelect_e);
		}
		else
		{
			Spi_highCS(Spi_chipSelect_e);
		}
	}
	
	/* send data */
	*(Spi_RegisterAdress_as.Spi_DataRegister_pui8) = data_ui8;

	if(Spi_interruptControlLocal_e == SPI_INTERRUPT_DISABLED)
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
			Spi_highCS(Spi_chipSelect_e);
		}
		else
		{
			Spi_lowCS(Spi_chipSelect_e);
		}
	}	
	return_ui8 = *(Spi_RegisterAdress_as.Spi_DataRegister_pui8);
	return return_ui8;
}

static void Spi_highCS(Gpio_ChannelType Spi_chipSelect_e)
{
	Gpio_WriteChannel(Spi_chipSelect_e, GPIO_HIGH);
}

static void Spi_lowCS(Gpio_ChannelType Spi_chipSelect_e)
{
	Gpio_WriteChannel(Spi_chipSelect_e, GPIO_LOW);
}

ISR(SPI_STC_vect)
{
	if(Spi_callbackLocal_pv != NULL)
	{
		Spi_callbackLocal_pv();
	}
	else
	{
		/* do nothing */
	}
}
