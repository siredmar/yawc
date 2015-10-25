/*---------------------------------------------------------------------
 * task_control.c
 *
 * Copyright (c) 2010 Tobias Wiese - hacker-tobi@gmx.net
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *------------------------------------------------------------------------
 */
#include <avr/sleep.h>
#include "include/nano_os.h"

//****************************************************
//*task_insert(Task *t,Function func_name,Prio prio) *
//*                                                  *
//* description: register a task                     *
//* input: Task *t: Pointer to Task                  *
//*        Function func_name: Function name         *
//*        Prio: Priority                            *
//*       stack_size: stack_size (0=Max.)            * 
//* output: TRUE(ok)/NULL(error)                     *
//****************************************************

uint8_t task_insert(Task *t, Function func_name, Priority prio)
{
    sregsys = SREG;
    cli();
    if (TASKS > MAXTASKS)
        return 0;
    signed char a = 0, b = (STACK_SIZE - 3);
    for (a=0; a<TASKS; a++)
    {
        if (!task_container[a])
        {
            t->ID = a;
            t->priority = prio;
            //set pointer to this task's stack section in RAM
            t->stack_section = (unsigned char*) stack_global + (a * STACK_SIZE);
            //clear this stack section
            unsigned char *stackp = (unsigned char *)(t->stack_section + STACK_SIZE - 3);
            while (b >= 0)
            {
                *stackp-- = 0;
                b--;
            }
            //init kontext: set the pc
            *(t->stack_section + STACK_SIZE - 2) = ((unsigned int)func_name)>>8;
            *(t->stack_section + STACK_SIZE - 1) = ((unsigned int)func_name)&0xff;
            //init kontext: set SP to t->stack_section
            t->sp = (uint8_t *)(t->stack_section + STACK_SIZE - 3 - 32);
            //init kontext: init sreg with 0
            t->sreg = 0;
            //set other stuff
#ifdef TASK_USE_SLEEP
            t->state = Active;
            t->laps = 0;
#endif
            task_container[a] = t;
            break;
        }
    }
    SREG = sregsys;
    return TRUE;
}

//****************************************************
//*task_remove(Task *t)                              *
//*                                                  *
//* description: delete a task                       *
//*                                                  *
//*           USE WITH CAUTION!                      *
//*                                                  *
//* input: Task *t = Handle to task                  *
//* output: TRUE(ok)/NULL(error)                     *
//****************************************************

#ifdef TASK_USE_ANHILATE
uint8_t task_remove(Task *t)
{
    sregsys=SREG;
    cli();

#ifdef TASK_USE_MEM
    uint8_t a,b;
#else
    uint8_t a;
#endif

    uint8_t ID = t->ID;
    if (ID == 0)
    {
        SREG=sregsys;
        return 0;
    }
    a=ID;
    //reset task values
    if (ID == active_task_id)
        active_task = 0;	//to avoid problems when deleting the current task
    task_container[a] = 0;		//this deletes the task

#ifdef TASK_USE_MEM
    //release task memory blocks
    //no reason to clear it here, because theres a clearing in memory_allocate_block()
    for (a=0;a<activeBlocks;a++)
    {
        if ((blocks+a)->owner == ID)
        {
            (blocks+a)->ptr=0;
            (blocks+a)->owner=0;
            for (b=a;b<(activeBlocks-1);b++)
                blocks[b]=blocks[b+1];
            activeBlocks--;
        }
    }
#endif

    if (ID != active_task_id)
    {
        //go back to parent task
        SREG=sregsys;
        return TRUE;
    }
    else
    {
        //switch to next task without going back to the deleted task if it's the parent task
        TIMER_REGISTER = CLK1;
        TCNT0 = 0xFF;
        active_task_id = -1;
        set_sleep_mode(SLEEP_MODE_IDLE);
        sei();
        sleep_mode();
    }
    return 0;
}
#endif

//****************************************************
//*task_schedule()                                   *
//*                                                  *
//*      !!!Internal Function!!!                     *
//*                                                  *
//* description: switch task / enable scheduling     *
//* input: none                                      *
//* output: none                                     *
//****************************************************

void task_schedule()
{
    cli();
    set_sleep_mode(SLEEP_MODE_IDLE);
    TCNT0 = 0xFF;
    TIMER_REGISTER = CLK1;
    TIMER_ENABLE |= _BV(TOIE0);
    sei();
    sleep_mode();
}

//****************************************************
//*task_wake(Task *t, uint8_t realtime)/task_sleep   *
//*                                                  *
//* description: sleep/wake a task                   *
//* input: Task                                      *
//* input uint8_t realtime (only task_wake):         * 
//*               1=switch immediately to this task  *
//*               0=leave scheduling unattended      *
//* output: TRUE(ok)/NULL(error)                     *
//****************************************************

#ifdef TASK_USE_SLEEP
uint8_t task_wake(Task *t, uint8_t realtime) 
{
    sregsys = SREG;
    cli();
    if (t->ID == 0 || t->state == Active)
    {
        SREG = sregsys;
        return 0;
    }
    if (realtime == 1)
        active_task_id = t->ID-1; //Switch to task t;
    t->state = Active;
    task_schedule();
    return 0;
}

// Task Signal - wait 
uint8_t task_sleep(Task *t)
{
    sregsys = SREG;
    cli();
    if (t->ID == 0 || t->state == Sleeping)
    {
        SREG = sregsys;
        return 0;
    }
    t->state = Sleeping;
    if (t->ID == active_task_id)
        task_schedule();
    else
    {
        SREG=sregsys;
        return TRUE;
    }
    return 0;
}

//****************************************************
//*task_sleep_for_laps(Task *t, uint8_t laps)        *
//*                                                  *
//* description: sleep a task for a number of laps   *
//* input: Task, number of laps                      *
//* output: TRUE(ok)/NULL(error)                     *
//****************************************************

uint8_t task_sleep_for_laps(Task *t, uint8_t laps)
{
    sregsys = SREG;
    cli();
    if (t->ID == 0)
    {
        SREG = sregsys;
        return 0;
    }
    t->state = SleepForLaps;
    t->laps = laps;
    if (t->ID == active_task_id)
        task_schedule();
    else
    {
        SREG = sregsys;
        return TRUE;
    }
    return 0;
}
#endif

//****************************************************
//*task_new_prio(Priority prio)                      *
//*                                                  *
//* description: set new priority for current Task   *
//* input: Priority (low/medium/high)                *
//* output: new TaskPriority                         *
//****************************************************

uint8_t task_new_prio(Priority prio) {return active_task->priority = prio;}

//****************************************************
//*task_switch(Task *t)			             *
//*                                                  *
//* description: switch to a task immediately        *
//* input: Task                                      *
//* output: TRUE(ok)/NULL(error)                     *
//****************************************************

uint8_t task_switch(Task *t)
{
    sregsys = SREG;
    cli();
#ifdef TASK_USE_SLEEP
    if (t->ID == 0 || t->state != Active)
#else
        if (t->ID == 0)
#endif  
        {
            SREG = sregsys;
            return 0;
        }
    active_task_id = t->ID-1; //Switch to task t;
    task_schedule();
    return 0;
}
