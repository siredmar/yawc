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
#include "include/nano_os.h"

//++++++++++++++++++++GlOBAL VARIABLES+++++++++++++++++

STATIC volatile signed char active_task_id = -1;
#ifdef TASK_USE_MEM
STATIC volatile uint8_t activeBlocks = 0;
#endif
STATIC volatile Task *task_container[TASKS], *active_task = 0;
STATIC volatile uint8_t sreg = 0, sregsys = 0;
volatile unsigned char stack_global[TASKS*STACK_SIZE];
Task idletask;


STATIC void kernel()
{
    //Create an idle-task
    task_insert(&idletask, idle, Low);
    //enable watchdog
    wdt_enable(WDTO_2S);

    //init user specific stuff
    init_user_environment();
    //start scheduler
    task_schedule();

    while(1)
    {
        /* this should never been reached */
    }
}

//****************************************************
//*reset_system()                                    *
//*                                                  *
//* description: reset the system via watchdog       *
//* input: none                                      *
//* output: none                                     *
//****************************************************

void reset_system()
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

void shutdown_system()
{
    cli();
    wdt_disable();
    shutdown_user_environment();
    while(1);
}
