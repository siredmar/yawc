/*---------------------------------------------------------------------
 * idle.c
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
//*idle()                                            *
//*                                                  *
//*      !!!INTERNAL FUNCTION!!!                     *
//*                                                  *
//* description: idle task                           *
//* input: none                                      *
//* output: none                                     *
//****************************************************

void idle()
{
    while (1)
    {
        wdt_reset();
        set_sleep_mode(SLEEP_MODE_IDLE);
        TIMER_REGISTER = CLK1;
        TCNT0 = 0xFF;
        sleep_mode();
    }
} 
