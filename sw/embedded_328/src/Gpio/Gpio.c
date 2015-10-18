/*
 * changes:
 *  18.08.2014  Mr. L.  added Gpio_ReadPort(), Gpio_WritePort
 */

#include "Gpio.h"

#include "Gpio_Cfg.h"
#include "Gpio_Lcfg.h"
#include "Std_Types.h"

static const volatile Gpio_ConfigType *localConfig;

volatile const Gpio_RegisterAddresstype Gpio_RegisterAdress_as[MAX_NUM_OF_PORTS] =
{
        {
                (uint8*) GPIO_PORTB_ADDRESS,
                (uint8*) GPIO_DDRB_ADDRESS,
                (uint8*) GPIO_PINB_ADDRESS
        },
        {
                (uint8*) GPIO_PORTC_ADDRESS,
                (uint8*) GPIO_DDRC_ADDRESS,
                (uint8*) GPIO_PINC_ADDRESS
        },
        {
                (uint8*) GPIO_PORTD_ADDRESS,
                (uint8*) GPIO_DDRD_ADDRESS,
                (uint8*) GPIO_PIND_ADDRESS
        }
};


void Gpio_Init()
{
   uint8 forCounter1_ui8;
   Gpio_PinInOutType Gpio_LocalPinInOut;
   Gpio_PinInitialType Gpio_LocalPinInitial;
   Gpio_PortType port_e;
   Gpio_PinType pin_e;
   uint16 channel_ui16;

   localConfig = (const Gpio_ConfigType*) Gpio_getlcfgdata();

   for(forCounter1_ui8 = 0; forCounter1_ui8 < MAX_NUM_OF_PORTS * MAX_NUM_OF_PINS; forCounter1_ui8++)
   {
      if(localConfig->Gpio_PinConfig[forCounter1_ui8].Gpio_ConfireAllowed == GPIO_TRUE)
      {
         Gpio_LocalPinInOut = localConfig->Gpio_PinConfig[forCounter1_ui8].Gpio_PinInOut;
         Gpio_LocalPinInitial = localConfig->Gpio_PinConfig[forCounter1_ui8].Gpio_PinInitial;
         channel_ui16 = localConfig->Gpio_PinConfig[forCounter1_ui8].Gpio_Pin;
         port_e = (uint8)(channel_ui16 >> 8);
         pin_e = (uint8)((channel_ui16 & 0xFF));

         *(Gpio_RegisterAdress_as[port_e].Gpio_DirectionRegister_pui8) |= (uint8)(Gpio_LocalPinInOut << pin_e);
//         if(Gpio_LocalPinInOut == GPIO_OUTPUT)
//         {
        	 *(Gpio_RegisterAdress_as[port_e].Gpio_PortRegister_pui8) |= (uint8)(Gpio_LocalPinInitial << pin_e);
//         }
      }
   }
}


void Gpio_WriteChannel(Gpio_ChannelType channel, Gpio_PinState state)
{
   Gpio_PortType port_ui8 = (channel >> 8);
   Gpio_PinType pin_ui8 = (channel & 0xFF);

   if(state == GPIO_HIGH)
      *(Gpio_RegisterAdress_as[port_ui8].Gpio_PortRegister_pui8) |= (1 << pin_ui8);
   else
      *(Gpio_RegisterAdress_as[port_ui8].Gpio_PortRegister_pui8) &= ~(1 << pin_ui8);
}

void Gpio_ToggleChannel(Gpio_ChannelType channel)
{
   Gpio_PortType port_ui8 = (channel >> 8);
   Gpio_PinType pin_ui8 = (channel & 0xFF);

   *(Gpio_RegisterAdress_as[port_ui8].Gpio_PortRegister_pui8) ^= (uint8)(1 << pin_ui8);
}

Gpio_PinState Gpio_ReadChannel(Gpio_ChannelType channel)
{
   Gpio_PortType port_ui8 = (channel >> 8);
   Gpio_PinType pin_ui8 = (channel & 0xFF);

   return *(Gpio_RegisterAdress_as[port_ui8].Gpio_InputRgister_pui8) & (uint8)(1 << pin_ui8);
}


Gpio_PortState Gpio_ReadPort(Gpio_PortType port)
{
    return *(Gpio_RegisterAdress_as[port].Gpio_InputRgister_pui8);
}

void Gpio_WritePort(Gpio_PortType port, Gpio_PortState state)
{
    *(Gpio_RegisterAdress_as[port].Gpio_PortRegister_pui8) = (uint8) state;
}
