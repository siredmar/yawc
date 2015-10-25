/*---------------------------------------------------------------------
 * scheduler.c
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

#define stack_push(r) asm volatile("push r"#r)
#define stack_pop(r) asm volatile("pop r"#r)

//****************************************************
//*scheduler()                                       *
//*                                                  *
//*      !!!INTERNAL FUNCTION!!!                     *
//*                                                  *
//* description: does the task switching             *
//* input: none                                      *
//* output: none                                     *
//****************************************************

ISR(TIMER_VECTOR)
{
    //save R0-31 to active_task->stack
    stack_push(31);
    stack_push(30);
    stack_push(29);
    stack_push(28);
    stack_push(27);
    stack_push(26);
    stack_push(25);
    stack_push(24);
    stack_push(23);
    stack_push(22);
    stack_push(21);
    stack_push(20);
    stack_push(19);
    stack_push(18);
    stack_push(17);
    stack_push(16);
    stack_push(15);
    stack_push(14);
    stack_push(13);
    stack_push(12);
    stack_push(11);
    stack_push(10);
    stack_push( 9);
    stack_push( 8);
    stack_push( 7);
    stack_push( 6);
    stack_push( 5);
    stack_push( 4);
    stack_push( 3);
    stack_push( 2);
    stack_push( 1);
    stack_push( 0);
    sregsys = SREG;
    //to avoid inconsistent state when we have a stack overrun/underrun reset the system via watchdog in this case
    if (active_task && (SP < (uint16_t) active_task->stack_section || SP >= (uint16_t) active_task->stack_section + STACK_SIZE))
    {
        wdt_enable(WDTO_1S);
        while(1);
    }
    else
    {
        TIMER_REGISTER = CLK1024;
        if (active_task_id >= 0)
        {
            //save SREG and SP
            active_task->sreg = sregsys;
            active_task->sp = (unsigned char *) (SP);
        }
        //switch to next task
        do
        {
            active_task_id++;
            if (active_task_id >= TASKS)
            {
                active_task_id = 0;
            }
            active_task = task_container[active_task_id];

#ifdef TASK_USE_SLEEP
            if (active_task && active_task->state == SleepForLaps)
            {
                if (active_task->laps > 0)
                    active_task->laps--;
                else
                    active_task->state=Active;
            }
#endif

        }

#ifdef TASK_USE_SLEEP
        while (!(active_task && active_task->state==Active));
#else
        while (!active_task);
#endif

        //restore this task (SREG, SP and R0...31)
        if (active_task)
        {
            TCNT0 = active_task->priority;
            SREG = active_task->sreg;
            SP = (unsigned int) (active_task->sp);
            stack_pop( 0);
            stack_pop( 1);
            stack_pop( 2);
            stack_pop( 3);
            stack_pop( 4);
            stack_pop( 5);
            stack_pop( 6);
            stack_pop( 7);
            stack_pop( 8);
            stack_pop( 9);
            stack_pop(10);
            stack_pop(11);
            stack_pop(12);
            stack_pop(13);
            stack_pop(14);
            stack_pop(15);
            stack_pop(16);
            stack_pop(17);
            stack_pop(18);
            stack_pop(19);
            stack_pop(20);
            stack_pop(21);
            stack_pop(22);
            stack_pop(23);
            stack_pop(24);
            stack_pop(25);
            stack_pop(26);
            stack_pop(27);
            stack_pop(28);
            stack_pop(29);
            stack_pop(30);
            stack_pop(31);
        }
    }
    asm volatile("reti");
}

