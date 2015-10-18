#include "Std_Types.h"
#include "Uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include "Gpio.h"

int main()
{
    uint8 cnt = 0;
    uint8 string[30];

    Uart_Init(UART_0, RECEPTION_ENABLED, TRANSMISSION_ENABLED, INTERRUPT_ENABLED, 115200UL, 1);

   sei(); /* Enable the interrupts */

   while(1)
   {
       sprintf(string, "Cnt: %d\n\r", cnt++);
       Uart_Puts(UART_0, string);
       _delay_ms(100);
   }
   return 0;
}
