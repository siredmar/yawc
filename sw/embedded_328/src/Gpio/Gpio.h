//#define _MMIO_BYTE(mem_addr) (mem_addr))
//#define __SFR_OFFSET 0x20
//#define _SFR_IO8(io_addr) _MMIO_BYTE((io_addr) + __SFR_OFFSET)
//
//
#include "Std_Types.h"

#ifndef GPIO_H
#define GPIO_H

#define MAX_NUM_OF_PORTS (3U)
#define MAX_NUM_OF_PINS  (8U)

typedef enum
{
   GPIO_CHANNEL_PB0 = 0x0000,
   GPIO_CHANNEL_PB1 = 0x0001,
   GPIO_CHANNEL_PB2 = 0x0002,
   GPIO_CHANNEL_PB3 = 0x0003,
   GPIO_CHANNEL_PB4 = 0x0004,
   GPIO_CHANNEL_PB5 = 0x0005,
   GPIO_CHANNEL_PB6 = 0x0006,
   GPIO_CHANNEL_PB7 = 0x0007,

   GPIO_CHANNEL_PC0 = 0x0100,
   GPIO_CHANNEL_PC1 = 0x0101,
   GPIO_CHANNEL_PC2 = 0x0102,
   GPIO_CHANNEL_PC3 = 0x0103,
   GPIO_CHANNEL_PC4 = 0x0104,
   GPIO_CHANNEL_PC5 = 0x0105,
   GPIO_CHANNEL_PC6 = 0x0106,

   GPIO_CHANNEL_PD0 = 0x0200,
   GPIO_CHANNEL_PD1 = 0x0201,
   GPIO_CHANNEL_PD2 = 0x0202,
   GPIO_CHANNEL_PD3 = 0x0203,
   GPIO_CHANNEL_PD4 = 0x0204,
   GPIO_CHANNEL_PD5 = 0x0205,
   GPIO_CHANNEL_PD6 = 0x0206,
   GPIO_CHANNEL_PD7 = 0x0207,
}Gpio_ChannelType;


typedef enum
{
   GPIO_PORTB = (0U),
   GPIO_PORTC,
   GPIO_PORTD,
}Gpio_PortType;

typedef enum
{
   GPIO_PIN0 = (0U),
   GPIO_PIN1,
   GPIO_PIN2,
   GPIO_PIN3,
   GPIO_PIN4,
   GPIO_PIN5,
   GPIO_PIN6,
   GPIO_PIN7
}Gpio_PinType;

typedef enum
{
   GPIO_FALSE = (0U),
   GPIO_TRUE
}Gpio_ConfigureAllowedType;

typedef enum
{
   GPIO_PIN_INITIAL_LOW = (0U),
   GPIO_PIN_INITIAL_HIGH
}Gpio_PinInitialType;

typedef enum
{
   GPIO_LOW = (0U),
   GPIO_HIGH
}Gpio_PinState;

typedef enum
{
   GPIO_INPUT = (0U),
   GPIO_OUTPUT
}Gpio_PinInOutType;

typedef struct
{
   Gpio_ConfigureAllowedType  Gpio_ConfireAllowed;
   Gpio_ChannelType           Gpio_Pin;
   Gpio_PinInOutType          Gpio_PinInOut;
   Gpio_PinInitialType        Gpio_PinInitial;
}Gpio_PinConfigType;

typedef struct
{
   Gpio_PinConfigType Gpio_PinConfig[MAX_NUM_OF_PINS * MAX_NUM_OF_PORTS];
   //Gpio_PortConfigType  Gpio_PortConfig[MAX_NUM_OF_PORTS];
}Gpio_ConfigType;

typedef struct
{
   uint8*       Gpio_PortRegister_pui8;
   uint8*       Gpio_DirectionRegister_pui8;
   uint8*       Gpio_InputRgister_pui8;
} Gpio_RegisterAddresstype;

//!!! added --->
//typedef struct
//{
//    Gpio_PinState   bit0 :1;
//    Gpio_PinState   bit1 :1;
//    Gpio_PinState   bit2 :1;
//    Gpio_PinState   bit3 :1;
//    Gpio_PinState   bit4 :1;
//    Gpio_PinState   bit5 :1;
//    Gpio_PinState   bit6 :1;
//    Gpio_PinState   bit7 :1;
//}Gpio_PortState;
typedef uint8 Gpio_PortState;
//!!! <--- added

void Gpio_Init(void);
void Gpio_WriteChannel(Gpio_ChannelType channel, Gpio_PinState state);
Gpio_PinState Gpio_ReadChannel(Gpio_ChannelType channel);
void Gpio_ToggleChannel(Gpio_ChannelType channel);

Gpio_PortState Gpio_ReadPort(Gpio_PortType);
void Gpio_WritePort(Gpio_PortType port, Gpio_PortState state);

#endif
