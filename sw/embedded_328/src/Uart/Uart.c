/**
 * \file uart.c
 * @brief This file contains the implementation of the UART module.
 *
 * Copyright (C) 2011  Armin Schlegel, Christian Eismann
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
#include <avr/interrupt.h>
#include <util/delay.h>

/*--- Macros ---------------------------------------------------------*/
/** Baudrate for UART communication */
//#define BAUD_UART_0 115200UL
//#define BAUD_UART_1 115200UL
//#define BAUD_UART_2 115200UL
//#define BAUD_UART_3 115200UL

///** Calculated UBRR value for high baud rate */
//#define UBRR_VAL_UART_0 ((F_CPU+BAUD_UART_0*8)/(BAUD_UART_0*16)-1)
//#define UBRR_VAL_UART_1 ((F_CPU+BAUD_UART_1*8)/(BAUD_UART_1*16)-1)
//#define UBRR_VAL_UART_2 ((F_CPU+BAUD_UART_2*8)/(BAUD_UART_2*16)-1)
//#define UBRR_VAL_UART_3 ((F_CPU+BAUD_UART_3*8)/(BAUD_UART_3*16)-1)

/**
 * @brief Initialize UART communication with given parameters rxen, txen, rxcie and BAUD_VAL_HIGH
 *
 * @param[in] unit   UART HW Unit
 * @param[in] rxen   enable receiving
 * @param[in] txen   enable transmitting
 * @param[in] rxcie  enable receiving interrupts
 */
void Uart_Init(Uart_hwUnitType unit, Uart_rxenType rxen, Uart_txenType txen, Uart_rxieType rxcie, uint32 Baudrate, uint8 u2x)
{
    uint8 UBRR_VAL;
    if(u2x == 1)
    {
        UBRR_VAL = 2*((F_CPU+Baudrate*8)/(Baudrate*16)-1);
    }
    else
    {
        UBRR_VAL = ((F_CPU+Baudrate*8)/(Baudrate*16)-1);
    }

    switch(unit)
   {
#if UART0_USED == TRUE
        case UART_0:
       {
           UCSR0A = (uint8)u2x << U2X0;
           UCSR0B = ((uint8)rxen << RXEN0) | ((uint8)rxcie << RXCIE0) | ((uint8)txen << TXEN0);
           UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
           UBRR0H = UBRR_VAL >> 8;
           UBRR0L = UBRR_VAL & 0xFF;
           break;
       }
#endif
#if UART1_USED == TRUE
       case UART_1:
       {
           UCSR1A = (uint8)u2x << U2X1;
           UCSR1B = ((uint8)rxen << RXEN1) | ((uint8)rxcie << RXCIE1) | ((uint8)txen << TXEN1);
           UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);
           UBRR1H = UBRR_VAL >> 8;
           UBRR1L = UBRR_VAL & 0xFF;
           break;
       }
#endif
#if UART2_USED == TRUE
       case UART_2:
       {
           UCSR2A = (uint8)u2x << U2X2;
           UCSR2B = ((uint8)rxen << RXEN2) | ((uint8)rxcie << RXCIE2) | ((uint8)txen << TXEN2);
           UCSR2C = (1 << UCSZ21) | (1 << UCSZ20);
           UBRR2H = UBRR_VAL >> 8;
           UBRR2L = UBRR_VAL & 0xFF;
           break;
       }
#endif
#if UART3_USED == TRUE
       case UART_3:
       {
           UCSR3A = (uint8)u2x << U2X3;
           UCSR3B = ((uint8)rxen << RXEN3) | ((uint8)rxcie << RXCIE3) | ((uint8)txen << TXEN3);
           UCSR3C = (1 << UCSZ31) | (1 << UCSZ30);
           UBRR3H = UBRR_VAL >> 8;
           UBRR3L = UBRR_VAL & 0xFF;
           break;
       }
#endif
       default:
       {
           break;
       }
   }

}


/**
 * @brief Sends a single character via UART
 *
 * @param[in] c byte to send
 */
void Uart_Putc(Uart_hwUnitType unit, uint8 byte)
{
    switch(unit)
    {
#if UART0_USED == TRUE
        case UART_0:
        {
            while (!(UCSR0A & (1 << UDRE0)));
            UDR0 = byte;         /* sende Zeichen */
            break;
        }
#endif
#if UART1_USED == TRUE
        case UART_1:
        {
            while (!(UCSR1A & (1 << UDRE1)));
            UDR1 = byte;         /* sende Zeichen */
            break;
        }
#endif
#if UART2_USED == TRUE
        case UART_2:
        {
            while (!(UCSR2A & (1 << UDRE2)));
            UDR2 = byte;         /* sende Zeichen */
            break;
        }
#endif
#if UART3_USED == TRUE
        case UART_3:
        {
            while (!(UCSR3A & (1 << UDRE3)));
            UDR3 = byte;         /* sende Zeichen */
            break;
        }
#endif
        default:
        {
            break;
        }
    }
}

/**
 * @brief Transmit string
 *
 * @param[in] s pointer to string to send
 */
void Uart_Puts(Uart_hwUnitType unit, const uint8 *s) {
   while (*s ) {
      Uart_Putc(unit, *s);
      s++;
   }
}


/**
 * @brief Interrupt Service Routine for receiving charakters via UART
 *
 */
#if UART0_USED == TRUE
ISR(USART0_RX_vect)
{
    Uart_Putc(UART_1, UDR0);
}
#endif

#if UART1_USED == TRUE
ISR(USART1_RX_vect)
{
    Uart_Putc(UART_0, UDR1);
}
#endif

#if UART2_USED == TRUE
ISR(Uart_Putc)
{

}
#endif

#if UART3_USED == TRUE
ISR(Uart_Putc)
{

}
#endif

