/**
 * \file uart.c
 * @brief This file contains the implementation of the UART module.
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

#include "Uart.h"
#include "Uart_Cfg.h"
#include "Uart_Lcfg.h"
#include <avr/interrupt.h>
#include <util/delay.h>

static Uart_RxCallbackType      Uart_UartRxIntCallback_pv[UART_MAX_NUM_OF_UARTS];
static Uart_TxCallbackType      Uart_UartTxIntCallback_pv[UART_MAX_NUM_OF_UARTS];
static Uart_ConfigType        *LcfgConfigPtr_ps;

volatile const Uart_RegisterAddressType Uart_RegisterAdress_as[UART_MAX_NUM_OF_UARTS] =
{
        {
                (uint8*) UART_UDR0_ADDRESS,
                (uint8*) UART_UCSR0A_ADDRESS,
                (uint8*) UART_UCSR0B_ADDRESS,
                (uint8*) UART_UCSR0C_ADDRESS,
                (uint8*) UART_UBRR0L_ADDRESS,
                (uint8*) UART_UBRR0H_ADDRESS
        },
#if UART_NUMBER_OF_UARTS >= 2
        {
                (uint8*) UART_UDR0_ADDRESS,
                (uint8*) UART_UCSR0A_ADDRESS,
                (uint8*) UART_UCSR0B_ADDRESS,
                (uint8*) UART_UCSR0C_ADDRESS,
                (uint8*) UART_UBRR0L_ADDRESS,
                (uint8*) UART_UBRR0H_ADDRESS
        },
#endif
#if UART_NUMBER_OF_UARTS >= 3
        {
                (uint8*) UART_UDR0_ADDRESS,
                (uint8*) UART_UCSR0A_ADDRESS,
                (uint8*) UART_UCSR0B_ADDRESS,
                (uint8*) UART_UCSR0C_ADDRESS,
                (uint8*) UART_UBRR0L_ADDRESS,
                (uint8*) UART_UBRR0H_ADDRESS
        },
#endif
#if UART_NUMBER_OF_UARTS >= 4
        {
                (uint8*) UART_UDR0_ADDRESS,
                (uint8*) UART_UCSR0A_ADDRESS,
                (uint8*) UART_UCSR0B_ADDRESS,
                (uint8*) UART_UCSR0C_ADDRESS,
                (uint8*) UART_UBRR0L_ADDRESS,
                (uint8*) UART_UBRR0H_ADDRESS
        }
#endif
};

#define USART_BAUDRATE 57600
#define BAUD_PRESCALE ((( F_CPU / ( USART_BAUDRATE * 16UL))) - 1)
void Uart_Init(void)
{
    Uart_HwUnitType HwUnitCounter = UART_HWUNIT_0;

    uint32                     Uart_CpuFrequency_ui32;
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

    uint16 Baudrate_ui16;

    LcfgConfigPtr_ps = (Uart_ConfigType*)Uart_GetLcfgData();

    Uart_CpuFrequency_ui32 = LcfgConfigPtr_ps->Uart_CpuFrequency_ui32;
    for(HwUnitCounter = UART_HWUNIT_0; HwUnitCounter < UART_MAX_NUM_OF_UARTS; HwUnitCounter++)
    {
        Uart_HwUnit_e                       = LcfgConfigPtr_ps->Uart[HwUnitCounter].Uart_HwUnit_e;
        Uart_Baudrate_ui32                  = LcfgConfigPtr_ps->Uart[HwUnitCounter].Uart_Baudrate_ui32;
        Uart_CharacterSize_e                = LcfgConfigPtr_ps->Uart[HwUnitCounter].Uart_CharacterSize_e;
        Uart_Parity_e                       = LcfgConfigPtr_ps->Uart[HwUnitCounter].Uart_Parity_e;
        Uart_StopBits_e                     = LcfgConfigPtr_ps->Uart[HwUnitCounter].Uart_StopBits_e;
        Uart_DoubleTxSpeed_e                = LcfgConfigPtr_ps->Uart[HwUnitCounter].Uart_DoubleTxSpeed_e;
        Uart_RxEnabled_e                    = LcfgConfigPtr_ps->Uart[HwUnitCounter].Uart_RxEnabled_e;
        Uart_TxEnabled_e                    = LcfgConfigPtr_ps->Uart[HwUnitCounter].Uart_TxEnabled_e;
        Uart_RxInterruptEnable_e            = LcfgConfigPtr_ps->Uart[HwUnitCounter].Uart_RxInterruptEnable_e;
        Uart_TxInterruptEnable_e            = LcfgConfigPtr_ps->Uart[HwUnitCounter].Uart_TxInterruptEnable_e;
        Uart_UartRxIntCallback_pv[HwUnitCounter] = LcfgConfigPtr_ps->Uart[HwUnitCounter].Uart_RxIntCallback_pv;
        Uart_UartTxIntCallback_pv[HwUnitCounter] = LcfgConfigPtr_ps->Uart[HwUnitCounter].Uart_TxIntCallback_pv;

        /* Uart Baudrate */
        if(Uart_DoubleTxSpeed_e == UART_DOUBLE_TX_SPEED_ENABLED)
        {
            Baudrate_ui16 = 2 * ((uint32)(Uart_CpuFrequency_ui32 + (uint32)(Uart_Baudrate_ui32 * 8)) / ((uint32)(Uart_Baudrate_ui32 * 16)) - 1);
        }
        else
        {
            Baudrate_ui16 = ((uint32)(Uart_CpuFrequency_ui32 + (uint32)(Uart_Baudrate_ui32 * 8)) / ((uint32)(Uart_Baudrate_ui32 * 16)) - 1);
        }

        *(Uart_RegisterAdress_as[HwUnitCounter].Uart_BaudrateHighRegister_pui8) = ((Baudrate_ui16 >> 8u) & 0xFF);
        *(Uart_RegisterAdress_as[HwUnitCounter].Uart_BaudrateLowRegister_pui8)  = (Baudrate_ui16 & 0xFF);

        *(Uart_RegisterAdress_as[HwUnitCounter].Uart_ControlStatusARegister_pui8) = (Uart_DoubleTxSpeed_e << UART_U2X_BIT_POSITION_UI8);

        *(Uart_RegisterAdress_as[HwUnitCounter].Uart_ControlStatusBRegister_pui8) = \
                  (Uart_RxInterruptEnable_e << UART_RXCIE_BIT_POSITION_UI8) \
                | (Uart_TxInterruptEnable_e << UART_TXCIE_BIT_POSITION_UI8) \
                | (Uart_RxEnabled_e << UART_RXEN_BIT_POSITION_UI8) \
                | (Uart_TxEnabled_e << UART_TXEN_BIT_POSITION_UI8);


        *(Uart_RegisterAdress_as[HwUnitCounter].Uart_ControlStatusCRegister_pui8) = \
                  (Uart_StopBits_e << UART_USBS_BIT_POSITION_UI8) \
                | (Uart_Parity_e << UART_UPM0_BIT_POSITION_UI8);

        if(Uart_CharacterSize_e == UART_CHAR_5_BIT)
        {
            *(Uart_RegisterAdress_as[HwUnitCounter].Uart_ControlStatusCRegister_pui8) |= (0x00u << UART_UCSZ0_BIT_POSITION_UI8);
        }
        else if(Uart_CharacterSize_e == UART_CHAR_6_BIT)
        {
            *(Uart_RegisterAdress_as[HwUnitCounter].Uart_ControlStatusCRegister_pui8) |= (0x01u << UART_UCSZ0_BIT_POSITION_UI8);
        }
        else if(Uart_CharacterSize_e == UART_CHAR_7_BIT)
        {
            *(Uart_RegisterAdress_as[HwUnitCounter].Uart_ControlStatusCRegister_pui8) |= (0x02u << UART_UCSZ0_BIT_POSITION_UI8);
        }
        else if(Uart_CharacterSize_e == UART_CHAR_8_BIT)
        {
            *(Uart_RegisterAdress_as[HwUnitCounter].Uart_ControlStatusCRegister_pui8) |= (0x03u << UART_UCSZ0_BIT_POSITION_UI8);
        }
        else /* if(Uart_CharacterSize_e == UART_CHAR_9_BIT)*/
        {
            *(Uart_RegisterAdress_as[HwUnitCounter].Uart_ControlStatusCRegister_pui8) |= (0x03u << UART_UCSZ0_BIT_POSITION_UI8);
            *(Uart_RegisterAdress_as[HwUnitCounter].Uart_ControlStatusBRegister_pui8) |= (1u << UART_UCSZ2_BIT_POSITION_UI8);
        }
    }
}


void Uart_WriteCharacter(Uart_HwUnitType HwUnit, uint16 data)
{
    while (!(*(Uart_RegisterAdress_as[HwUnit].Uart_ControlStatusARegister_pui8) & (1 << UART_UDRE_BIT_POSITION_UI8)));
    if(LcfgConfigPtr_ps->Uart[HwUnit].Uart_CharacterSize_e == UART_CHAR_9_BIT)
    {
        *(Uart_RegisterAdress_as[HwUnit].Uart_ControlStatusBRegister_pui8) |= (((data >> 8) & 0x01) << UART_TXB8_BIT_POSITION_UI8);
    }

    *(Uart_RegisterAdress_as[HwUnit].Uart_DataRegister_pui8) = (data & 0xFF);
}

void Uart_WriteString(Uart_HwUnitType HwUnit, const uint8 *s)
{
   while (*s)
   {
      Uart_WriteCharacter(HwUnit, *s);
      s++;
   }
}

ISR(USART_RX_vect)
{
    uint16 data = 0u;

    if(LcfgConfigPtr_ps->Uart[UART_HWUNIT_0].Uart_CharacterSize_e == UART_CHAR_9_BIT)
    {
        data = ((((*(Uart_RegisterAdress_as[UART_HWUNIT_0].Uart_ControlStatusBRegister_pui8) >> UART_RXB8_BIT_POSITION_UI8) & 0x01)) << 8u) & 0x100;
    }
    data |= (*(Uart_RegisterAdress_as[UART_HWUNIT_0].Uart_DataRegister_pui8) & 0xFF);

    if(Uart_UartRxIntCallback_pv[UART_HWUNIT_0] != NULL)
    {
        (Uart_UartRxIntCallback_pv[UART_HWUNIT_0])(data);
    }
    else
    {
        /* do nothing */
    }
}

#if UART_NUMBER_OF_UARTS >= 2
ISR(USART1_RX_vect)
{
    uint16 data;

    if(LcfgConfigPtr_ps->Uart[UART_HWUNIT_1].Uart_CharacterSize_e == UART_CHAR_9_BIT)
    {
        data = ((((*(Uart_RegisterAdress_as[UART_HWUNIT_1].Uart_ControlStatusBRegister_pui8) >> UART_RXB8_BIT_POSITION_UI8) & 0x01)) << 8u) & 0x100;
    }
    data =  (*(Uart_RegisterAdress_as[UART_HWUNIT_1].Uart_DataRegister_pui8) & 0xFF);

    if(Uart_UartRxIntCallback_pv[UART_HWUNIT_1] != NULL)
    {
        (Uart_UartRxIntCallback_pv[UART_HWUNIT_1])(data);
    }
    else
    {
        /* do nothing */
    }
}
#endif

#if UART_NUMBER_OF_UARTS >= 3
ISR(USART2_RX_vect)
{
    uint16 data;

    if(LcfgConfigPtr_ps->Uart[UART_HWUNIT_2].Uart_CharacterSize_e == UART_CHAR_9_BIT)
    {
        data = ((((*(Uart_RegisterAdress_as[UART_HWUNIT_2].Uart_ControlStatusBRegister_pui8) >> UART_RXB8_BIT_POSITION_UI8) & 0x01)) << 8u) & 0x100;
    }
    data =  (*(Uart_RegisterAdress_as[UART_HWUNIT_2].Uart_DataRegister_pui8) & 0xFF);

    if(Uart_UartRxIntCallback_pv[UART_HWUNIT_2] != NULL)
    {
        (Uart_UartRxIntCallback_pv[UART_HWUNIT_2])(data);
    }
    else
    {
        /* do nothing */
    }
}
#endif

#if UART_NUMBER_OF_UARTS >= 4
ISR(USART3_RX_vect)
{
    uint16 data;

    if(LcfgConfigPtr_ps->Uart[UART_HWUNIT_3].Uart_CharacterSize_e == UART_CHAR_9_BIT)
    {
        data = ((((*(Uart_RegisterAdress_as[UART_HWUNIT_3].Uart_ControlStatusBRegister_pui8) >> UART_RXB8_BIT_POSITION_UI8) & 0x01)) << 8u) & 0x100;
    }
    data =  (*(Uart_RegisterAdress_as[UART_HWUNIT_3].Uart_DataRegister_pui8) & 0xFF);

    if(Uart_UartRxIntCallback_pv[UART_HWUNIT_3] != NULL)
    {
        (Uart_UartRxIntCallback_pv[UART_HWUNIT_3])(data);
    }
    else
    {
        /* do nothing */
    }

}
#endif

ISR(USART_TX_vect)
{
    if(Uart_UartTxIntCallback_pv[UART_HWUNIT_0] != NULL)
    {
        (Uart_UartTxIntCallback_pv[UART_HWUNIT_0])();
    }
    else
    {
        /* do nothing */
    }
}

#if UART_NUMBER_OF_UARTS >= 2
ISR(USART1_TX_vect)
{
    if(Uart_UartTxIntCallback_pv[UART_HWUNIT_1] != NULL)
    {
        (Uart_UartTxIntCallback_pv[UART_HWUNIT_1])();
    }
    else
    {
        /* do nothing */
    }
}
#endif

#if UART_NUMBER_OF_UARTS >= 3
ISR(USART2_TX_vect)
{
    if(Uart_UartTxIntCallback_pv[UART_HWUNIT_2] != NULL)
    {
        (Uart_UartTxIntCallback_pv[UART_HWUNIT_2])();
    }
    else
    {
        /* do nothing */
    }
}
#endif

#if UART_NUMBER_OF_UARTS >= 4
ISR(USART3_TX_vect)
{
    if(Uart_UartTxIntCallback_pv[UART_HWUNIT_3] != NULL)
    {
        (Uart_UartTxIntCallback_pv[UART_HWUNIT_3])();
    }
    else
    {
        /* do nothing */
    }
}
#endif

