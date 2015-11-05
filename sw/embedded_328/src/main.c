#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <avr/interrupt.h>   // Interrupts and timers
#include <avr/wdt.h>     // Watchdog timer to repair lockups
#include <avr/sleep.h>
#include "Std_Types.h"
#include "Uart.h"
#include <stdio.h>
#include <util/delay.h>
#include "Gpio.h"
#include "Dbg.h"
#include "nano_os.h"
#include "u8g.h"

u8g_t u8g;

void TaskA_Handler();
void TaskB_Handler();
Task TaskA, TaskB;

void init_user_environment()
{
    Uart_WriteString(UART_HWUNIT_0, "System started...\n\r");

    task_insert(&TaskA, TaskA_Handler, 20);
    task_insert(&TaskB, TaskB_Handler, 10);
    Uart_WriteString(UART_HWUNIT_0, "System up and running...\n\r");
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
        Uart_WriteString(UART_HWUNIT_0, "A\r\n");
        _delay_ms(500);
    }
}
typedef struct
{
    uint8 m;//[3];
    uint8 h;//[3];
} ClockType;


ClockType CalcClock(void)
{
    ClockType returnValue;
    static uint8 m = 0;
    static uint8 h = 0;

    if(m++ > 58)
    {
        h++;
        m = 0;
    }
    if(h > 23)
    {
        h = 0;
    }
    returnValue.h = h;
    returnValue.m = m;
    return returnValue;
}

void draw(ClockType clock)
{
    uint8 str[6];

    u8g_SetFont(&u8g, u8g_font_osr29n);
    sprintf(str, "%.2d:%.2d", clock.h, clock.m);
    u8g_DrawStr(&u8g, 10, 50, str);
}

void TaskB_Handler()
{
    static uint8 DisplayDrawn = 1;
    ClockType clock;

    while (1)
    {
        if(DisplayDrawn == 1)
        {
            Uart_WriteString(UART_HWUNIT_0, "Disp Update\r\n");

            DisplayDrawn = 0;
            clock = CalcClock();
            u8g_FirstPage(&u8g);
            do
            {
                draw(clock);
            } while ( u8g_NextPage(&u8g) );
            u8g_Delay(50);
            DisplayDrawn = 1;
        }
        else
        {

        }
    }
}

int main()
{
    uint8 cnt = 0;
    Uart_Init();
    Gpio_Init();
    u8g_Init(&u8g, &u8g_dev_ssd1306_131x64_i2c);

    kernel();

    sei(); /* Enable the interrupts */
    while(1)
    {
        /* do nothing */
    }
    return 0;
}
