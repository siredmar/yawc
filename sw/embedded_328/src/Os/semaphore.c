/*---------------------------------------------------------------------
 * semaphore.c
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

#ifdef TASK_USE_SEMAPHORE

//****************************************************
//*semaphore_init(Semaphor *s)                       *
//*                                                  *
//* description: init semaphores                     *
//* input: Pointer to Semaphore                      *
//* output: none                                     *
//****************************************************

void semaphore_init(Semaphor *s)
{
    s->owner = 0;
}

//****************************************************
//*semaphore_set(Semaphor *s)                        *
//*                                                  *
//* description: set semaphore                       *
//* input: Pointer to Semaphore                      *
//* output: TRUE(ok), NULL(error)                    *
//****************************************************

uint8_t semaphore_set(Semaphor *s)
{
    sregsys = SREG;
    cli();
    if (s->owner != 0)
    {
        SREG = sregsys;
        return 0;
    }
    s->owner = active_task_id ;
    SREG = sregsys;
    return TRUE;
}

//****************************************************
//*semaphore_free(Semaphor *s)                       *
//*                                                  *
//* description: free semaphore                      *
//* input: Pointer to Semaphore                      *
//* output: TRUE(ok), NULL(error)                    *
//****************************************************

uint8_t semaphore_free(Semaphor *s)
{
    sregsys = SREG;
    cli();
    if (s->owner != active_task_id)
    {
        SREG = sregsys;
        return 0;
    }
    s->owner = 0;
    SREG = sregsys;
    return TRUE;
}

//****************************************************
//*semaphore_read(Semaphor *s)                       *
//*                                                  *
//* description: read status of a semaphore          *
//* input: Pointer to Semaphore                      *
//* output: TRUE(free), NULL(error/non free)         *
//****************************************************

uint8_t semaphore_read(Semaphor *s) 
{
    sregsys = SREG;
    cli();
    if (s->owner)
    {
        SREG = sregsys;
        return 0;
    }
    SREG = sregsys;
    return TRUE;
}

//****************************************************
//*semaphore_wait(Semaphor *s)                       *
//*                                                  *
//* description: wait for a semaphore                *
//* input: Pointer to Semaphore                      *
//* output: TRUE(is free), NULL(error)               *
//****************************************************

uint8_t semaphore_wait(Semaphor *s) 
{ 
    //enable multitasking to avoid a deadrun
    wdt_enable(WDTO_2S);
    TIMER_ENABLE |= _BV(TOIE0);
    sei();
    while (s->owner != 0)
    {
        TIMER_REGISTER = CLK1; //to avoid unecessary wait states while we are waiting for the semaphore
        TCNT0=0xFF; //to switch to scheduler as soon as possible. scheduler will set this back to CLK1024
    }
    return TRUE;
} 

#endif
