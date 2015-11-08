/*
 * TaskUart.c
 *
 *  Created on: Nov 8, 2015
 *      Author: armin
 */
#include "Uart.h"

void Uart0_RxCallback(uint16 data)
{
    Uart_WriteCharacter(UART_HWUNIT_0, data);
}
