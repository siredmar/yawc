/**
 * \file uart.h
 * \brief This file contains the interface of the UART module.
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

#ifndef _UART_H_
#define _UART_H_

/*--- Includes -------------------------------------------------------*/

#include <inttypes.h>
#include <avr/io.h>
#include "Std_Types.h"

/** The maximum size of a string that is sent via UART */
#define MAXUARTSTR 250

typedef enum
{
   RECEPTION_DISABLED = 0U,
   RECEPTION_ENABLED
}Uart_rxenType;

typedef enum
{
   TRANSMISSION_DISABLED = 0U,
   TRANSMISSION_ENABLED
}Uart_txenType;

typedef enum
{
   INTERRUPT_DISABLED = 0U,
   INTERRUPT_ENABLED
}Uart_rxieType;

typedef enum
{
    UART_0 = 0u,
    UART_1,
    UART_2,
    UART_3
} Uart_hwUnitType;


/*--- Function Prototypes --------------------------------------------*/

void Uart_Init(Uart_hwUnitType unit, Uart_rxenType rxen, Uart_txenType txen, Uart_rxieType rxcie, uint32 Baudrate, uint8 u2x);
void Uart_Putc(Uart_hwUnitType unit, uint8 byte);
void Uart_Puts(Uart_hwUnitType unit, const uint8 *s);


#endif /* #ifndef _UART_H_ */

