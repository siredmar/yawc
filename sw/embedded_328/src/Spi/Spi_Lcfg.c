#include "Spi_Lcfg.h"
#include "Spi.h"

extern void Spi_Callback();

const Spi_ConfigType Spi_InitialConfiguration_s =
{
		SPI_MODULE_ENABLED,
		SPI_MASTER_MODE,
		SPI_INTERRUPT_ENABLED,
		NULL,
		SPI_MSB_FIRST,
		SPI_CPOL_0,
		SPI_CPHA_0,
		SPI_CLOCK_PRESCALER_DIV_16,
		SPI_DOUBLE_SPEED_ENABLED
};

const void *Spi_GetLcfgData(void)
{
   return ((void*) &Spi_InitialConfiguration_s);
}
