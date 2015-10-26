#include "Std_Types.h"
#include "Uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include "Gpio.h"
#include "Dbg.h"
#include "Ssd1306.h"

#include <stdlib.h>
#include <avr/interrupt.h>   // Interrupts and timers
#include <avr/wdt.h>     // Watchdog timer to repair lockups
#include <avr/sleep.h>

#include "Os/include/nano_os.h"

void TaskA_Handler();
void TaskB_Handler();
void TaskC_Handler();
Task TaskA, TaskB, TaskC;

void init_user_environment()
{

    Uart_Init();
    Gpio_Init();
    Spi_Init();
    Ssd1306_Init();
    Ssd1306_ClearScreen();
    Ssd1306_PutPixel(5, 5, 1);
    Ssd1306_SendFrame();


    Uart_WriteString(UART_HWUNIT_0, "System started...\n\r");
    Dbg_ReadRegister(UART_HWUNIT_0, "MCUSR (0x54): ", 0x54);

    task_insert(&TaskA, TaskA_Handler, 100);
    task_insert(&TaskB, TaskB_Handler, 100);
    task_insert(&TaskC, TaskC_Handler, 100);
}

void shutdown_user_environment()
{
    asm("nop"); //only placebo
}

void Uart0_RxCallback(uint16 data)
{
    Uart_WriteCharacter(UART_HWUNIT_0, data);
}

void TaskA_Handler()
{
    while (1)
    {
        Uart_WriteString(UART_HWUNIT_0, "Task A\r\n");
        _delay_ms(500);
    }
}

//this is the display task for the hours
void TaskB_Handler()
{
    while (1)
    {
        Uart_WriteString(UART_HWUNIT_0, "Task B\r\n");
        _delay_ms(100);
    }
}

//this is the display task for the hours
void TaskC_Handler()
{
    volatile uint8 cnt = 0;
    sint8 string[30];
    while (1)
    {
        itoa(cnt++, string, 10);
        Uart_WriteString(UART_HWUNIT_0, string);
        Uart_WriteString(UART_HWUNIT_0, " Task C\r\n");
        _delay_ms(1000);
    }
}
int main()
{
    kernel();

    sei(); /* Enable the interrupts */
    while(1)
    {
        /* this should never be reached */
    }
    return 0;
}
