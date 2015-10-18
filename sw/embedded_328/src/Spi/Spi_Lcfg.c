#include "Spi_Lcfg.h"
#include "Spi.h"

extern void Spi_Callback();

const Spi_ConfigType Spi_initialConfiguration_s =
{
		SPI_MODULE_ENABLED,
		SPI_MASTER_MODE,
		SPI_INTERRUPT_ENABLED,
		NULL,
		SPI_LSB_FIRST,
		SPI_CPOL_1,
		SPI_CPHA_1,
		SPI_CLOCK_PRESCALER_DIV_4,
		SPI_DOUBLE_SPEED_ENABLED
};

const void *Spi_getlcfgdata(void)
{
   return ((void*) &Spi_initialConfiguration_s);
}
