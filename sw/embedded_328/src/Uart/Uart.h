/**
 * \file uart.h
 * \brief This file contains the interface of the UART module.
 *
 * Copyright (C) 2015  Armin Schlegel
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _UART_H_
#define _UART_H_

/*--- Includes -------------------------------------------------------*/

#include <inttypes.h>
#include <avr/io.h>
#include "Std_Types.h"
#include "Uart_Cfg.h"

/** The maximum size of a string that is sent via UART */
#define UART_CALLBACK_NULL_PTR ((void*)0)

typedef void (*Uart_RxCallbackType)(uint16);
typedef void (*Uart_TxCallbackType)(void);


typedef enum
{
    UART_RECEPTION_DISABLED = 0U,
    UART_RECEPTION_ENABLED
} Uart_RxEnableType;

typedef enum
{
    UART_TRANSMISSION_DISABLED = 0U,
    UART_TRANSMISSION_ENABLED
} Uart_TxEnableType;

typedef enum
{
    UART_RX_INTERRUPT_DISABLED = 0U,
    UART_RX_INTERRUPT_ENABLED
} Uart_RxInterruptEnableType;

typedef enum
{
   UART_TX_INTERRUPT_DISABLED = 0U,
   UART_TX_INTERRUPT_ENABLED
} Uart_TxInterruptEnableType;

typedef enum
{
   UART_PARITY_DISABLED = (0U),
   UART_EVEN_PARITY,
   UART_ODD_PARITY
} Uart_ParityModeType;

typedef enum
{
   UART_STOP_BITS_1 = (0U),
   UART_STOP_BITS_2
} Uart_StopBitsType;

typedef enum
{
   UART_DOUBLE_TX_SPEED_DISABLED = (0U),
   UART_DOUBLE_TX_SPEED_ENABLED
} Uart_DoubleTxSpeedType;

typedef enum
{
    UART_CHAR_5_BIT = (0u),
    UART_CHAR_6_BIT,
    UART_CHAR_7_BIT,
    UART_CHAR_8_BIT,
    UART_CHAR_9_BIT
} Uart_CharacterSizeType;

typedef enum
{
    UART_HWUNIT_0 = 0u,
#if UART_NUMBER_OF_UARTS >= 2
    UART_HWUNIT_1,
#endif
#if UART_NUMBER_OF_UARTS >= 3
    UART_HWUNIT_2,
#endif
#if UART_NUMBER_OF_UARTS >= 4
    UART_HWUNIT_3,
#endif
    UART_MAX_NUM_OF_UARTS
} Uart_HwUnitType;


//typedef struct
//{
//    uint8* Uart_DataRegister_pui8;
//    uint8* Uart_ControlStatusARegister_pui8;
//    uint8* Uart_ControlStatusBRegister_pui8;
//    uint8* Uart_ControlStatusCRegister_pui8;
//    uint8* Uart_BaudrateLowRegister_pui8;
//    uint8* Uart_BaudrateHighRegister_pui8;
//} Uart_HwUnitRegisterAdressType;


typedef struct
{
//    Uart_HwUnitRegisterAdressType   HwUnit[UART_MAX_NUM_OF_UARTS];
    uint8* Uart_DataRegister_pui8;
    uint8* Uart_ControlStatusARegister_pui8;
    uint8* Uart_ControlStatusBRegister_pui8;
    uint8* Uart_ControlStatusCRegister_pui8;
    uint8* Uart_BaudrateLowRegister_pui8;
    uint8* Uart_BaudrateHighRegister_pui8;
} Uart_RegisterAddressType;

typedef struct
{
    Uart_HwUnitType            Uart_HwUnit_e;
    uint32                     Uart_Baudrate_ui32;
    Uart_CharacterSizeType     Uart_CharacterSize_e;
    Uart_ParityModeType        Uart_Parity_e;
    Uart_StopBitsType          Uart_StopBits_e;
    Uart_DoubleTxSpeedType     Uart_DoubleTxSpeed_e;
    Uart_RxEnableType          Uart_RxEnabled_e;
    Uart_TxEnableType          Uart_TxEnabled_e;
    Uart_RxInterruptEnableType Uart_RxInterruptEnable_e;
    Uart_TxInterruptEnableType Uart_TxInterruptEnable_e;
    Uart_RxCallbackType        Uart_RxIntCallback_pv;
    Uart_TxCallbackType        Uart_TxIntCallback_pv;
}Uart_HwUnitConfigType;


typedef struct
{
    uint32 Uart_CpuFrequency_ui32;
    Uart_HwUnitConfigType   Uart[UART_MAX_NUM_OF_UARTS];
} Uart_ConfigType;

/*--- Function Prototypes --------------------------------------------*/

void Uart_Init(void);
void Uart_WriteCharacter(Uart_HwUnitType HwUnit, uint16 data);
void Uart_WriteString(Uart_HwUnitType unit, const uint8 *s);
void Uart0_RxCallback(uint16 data);

#endif /* #ifndef _UART_H_ */

