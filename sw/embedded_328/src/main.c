#include "Std_Types.h"
#include "Uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include "Gpio.h"
#include "Dbg.h"

void Uart0_RxCallback(uint16 data)
{
    Uart_WriteCharacter(UART_HWUNIT_0, data);
}


void UartHelloTask(void)
{
    Uart_WriteString(UART_HWUNIT_0, "Hello ");
}

void UartWorldTask(void)
{
    Uart_WriteString(UART_HWUNIT_0, "World! ");
}

int main()
{
    uint8 cnt = 0;
    uint8 string[30];

    Uart_Init();
    Os_Init();

    Uart_WriteString(UART_HWUNIT_0, "System started...\n\r");
    Dbg_ReadRegister(UART_HWUNIT_0, "MCUSR (0x54): ", 0x54);
    sei(); /* Enable the interrupts */
    Os_Start();
    while(1)
    {
//        Dbg_ReadRegister(UART_HWUNIT_0, "USCR0A (0xC0): ", 0xC0);
//        Dbg_ReadRegister(UART_HWUNIT_0, "USCR0B (0xC1): ", 0xC1);
//        Dbg_ReadRegister(UART_HWUNIT_0, "USCR0C (0xC2): ", 0xC2);
//
//        _delay_ms(1000);
//        sprintf(string, "Cnt: %d\n\r", cnt++);
//        Uart_WriteString(UART_HWUNIT_0, string);
    }
    return 0;
}
