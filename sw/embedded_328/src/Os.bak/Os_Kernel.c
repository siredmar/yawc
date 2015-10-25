/*---------------------------------------------------------------------
 * kernel.c
 *
 * Copyright (c) 2010 Tobias Wiese - hacker-tobi@gmx.net
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *------------------------------------------------------------------------
 */

#include "Os.h"
#include "Dbg.h"
#include "Uart.h"

Os_TaskType Os_Idletask;

void Os_Kernel(void)
{
    //Create an idle-task
    Os_TaskInsert(&Os_Idletask,Os_Idle,Low);
    Dbg_ReadRegister(UART_HWUNIT_0, "Test 1 ", NULL);
    //enable watchdog
    wdt_enable(WDTO_2S);
    Dbg_ReadRegister(UART_HWUNIT_0, "Test 2 ", NULL);
    //init user specific stuff
    Os_InitUserEnvironment();
    Dbg_ReadRegister(UART_HWUNIT_0, "Test 3 ", NULL);
    //start scheduler
    Os_TaskSchedule();
    Dbg_ReadRegister(UART_HWUNIT_0, "Test 4 ", NULL);

    while(1)
    {
        Dbg_ReadRegister(UART_HWUNIT_0, "Test 5 ", NULL);
        /* idle */
    }
}

//****************************************************
//*reset_system()                                    *
//*                                                  *
//* description: reset the system via watchdog       *
//* input: none                                      *
//* output: none                                     *
//****************************************************

void Os_ResetSystem(void)
{
    cli();
    wdt_enable(WDTO_1S);
    while(1);
}

//****************************************************
//*shutdown_system()                                 *
//*                                                  *
//* description: shutdown the system                 *
//* input: none                                      *
//* output: none                                     *
//****************************************************

void Os_ShutdownSystem(void)
{
    cli();
    wdt_disable();
    Os_ShutdownUserEnvironment();
    while(1);
}
