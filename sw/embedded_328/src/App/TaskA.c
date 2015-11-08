/*
 * TaskA.c
 *
 *  Created on: Nov 8, 2015
 *      Author: armin
 */
#include "TaskA.h"
#include "Uart.h"

void TaskA_Handler(void)
{
    while (1)
    {
        Uart_WriteString(UART_HWUNIT_0, "A\r\n");
        _delay_ms(500);
    }
}
