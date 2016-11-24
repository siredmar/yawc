/*
 * Os_Init.c
 *
 *  Created on: Nov 8, 2015
 *      Author: armin
 */
#include "Uart.h"
#include "Gpio.h"
#include "Gpt.h"
#include "u8g.h"
#include "nano_os.h"
#include "TaskClock.h"
#include "TaskA.h"

extern u8g_t u8g;

void init_user_environment()
{
    Uart_Init();
    Gpio_Init();
    Gpt_Init();
    u8g_Init(&u8g, &u8g_dev_ssd1306_128x64_2x_i2c);
    AppClock_Init();
    Gpt_TimerStart(TIMER_16BIT_0);

    Uart_WriteString(UART_HWUNIT_0, "System started\n\r");
//    task_insert(&TaskA, TaskA_Handler, 20);
    task_insert(&TaskDrawClock, AppClock_Handler, 200);
    Uart_WriteString(UART_HWUNIT_0, "System up and running!\n\r");


}

void shutdown_user_environment()
{
    asm("nop");
}


