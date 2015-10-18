#include "Gpio_Lcfg.h"

#include "Gpio.h"

const Gpio_ConfigType Gpio_initialConfiguration_s =
{
        {
                /* PORT B */
                {
                        GPIO_FALSE,
                        GPIO_CHANNEL_PB0,
                        GPIO_INPUT,
                        GPIO_PIN_INITIAL_HIGH
                },
                {
                        GPIO_FALSE,
                        GPIO_CHANNEL_PB1,
                        GPIO_INPUT,
                        GPIO_PIN_INITIAL_HIGH
                },
                {
                        GPIO_FALSE,
                        GPIO_CHANNEL_PB2,
                        GPIO_INPUT,
                        GPIO_PIN_INITIAL_HIGH
                },
                {
                        GPIO_FALSE,
                        GPIO_CHANNEL_PB3,
                        GPIO_INPUT,
                        GPIO_PIN_INITIAL_HIGH
                },
                {
                        GPIO_FALSE,
                        GPIO_CHANNEL_PB4,
                        GPIO_INPUT,
                        GPIO_PIN_INITIAL_HIGH
                },
                {
                        GPIO_FALSE,
                        GPIO_CHANNEL_PB5,
                        GPIO_OUTPUT,
                        GPIO_PIN_INITIAL_LOW
                },
                {
                        GPIO_FALSE,
                        GPIO_CHANNEL_PB6,
                        GPIO_INPUT,
                        GPIO_PIN_INITIAL_HIGH
                },
                {
                        GPIO_FALSE,
                        GPIO_CHANNEL_PB7,
                        GPIO_INPUT,
                        GPIO_PIN_INITIAL_HIGH
                },

                /* PORT C */
                {
                        GPIO_FALSE,
                        GPIO_CHANNEL_PC0,
                        GPIO_INPUT,
                        GPIO_PIN_INITIAL_HIGH
                },
                {
                        GPIO_FALSE,
                        GPIO_CHANNEL_PC1,
                        GPIO_INPUT,
                        GPIO_PIN_INITIAL_HIGH
                },
                {
                        GPIO_FALSE,
                        GPIO_CHANNEL_PC2,
                        GPIO_INPUT,
                        GPIO_PIN_INITIAL_HIGH
                },
                {
                        GPIO_FALSE,
                        GPIO_CHANNEL_PC3,
                        GPIO_INPUT,
                        GPIO_PIN_INITIAL_HIGH
                },
                {
                        GPIO_FALSE,
                        GPIO_CHANNEL_PC4,
                        GPIO_INPUT,
                        GPIO_PIN_INITIAL_HIGH
                },
                {
                        GPIO_FALSE,
                        GPIO_CHANNEL_PC5,
                        GPIO_INPUT,
                        GPIO_PIN_INITIAL_HIGH
                },
                {
                        GPIO_FALSE,
                        GPIO_CHANNEL_PC6,
                        GPIO_INPUT,
                        GPIO_PIN_INITIAL_HIGH
                },

                /* PORT D */
                {
                        GPIO_FALSE,
                        GPIO_CHANNEL_PD0,
                        GPIO_INPUT,
                        GPIO_PIN_INITIAL_HIGH
                },
                {
                        GPIO_FALSE,
                        GPIO_CHANNEL_PD1,
                        GPIO_INPUT,
                        GPIO_PIN_INITIAL_HIGH
                },
                {
                        GPIO_FALSE,
                        GPIO_CHANNEL_PD2,
                        GPIO_INPUT,
                        GPIO_PIN_INITIAL_HIGH
                },
                {
                        GPIO_FALSE,
                        GPIO_CHANNEL_PD3,
                        GPIO_INPUT,
                        GPIO_PIN_INITIAL_HIGH
                },
                {
                        GPIO_FALSE,
                        GPIO_CHANNEL_PD4,
                        GPIO_INPUT,
                        GPIO_PIN_INITIAL_HIGH
                },
                {
                        GPIO_FALSE,
                        GPIO_CHANNEL_PD5,
                        GPIO_INPUT,
                        GPIO_PIN_INITIAL_HIGH
                },
                {
                        GPIO_FALSE,
                        GPIO_CHANNEL_PD6,
                        GPIO_INPUT,
                        GPIO_PIN_INITIAL_HIGH
                },
                {
                        GPIO_FALSE,
                        GPIO_CHANNEL_PD7,
                        GPIO_INPUT,
                        GPIO_PIN_INITIAL_HIGH
                }
        }
};





const void *Gpio_getlcfgdata(void)
{
    return ((const void *) &Gpio_initialConfiguration_s);
}
