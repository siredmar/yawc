/*
 * Gpt_Lcfg.c
 *
 *  Created on: Nov 6, 2015
 *      Author: armin
 */

#include "Gpt_Lcfg.h"

#include "../App/TaskClock.h"
#include "Gpt.h"

const Gpt_ConfigType Gpt_InitialConfiguration_s =
{
        {
                {
                        TIMER_UNUSED,
                        TIMER_8BIT_0,
                        0,
                        0,
                        GPT_CALLBACK_NULL_PTR
                },
                {
                        TIMER_USED,
                        TIMER_8BIT_1,
                        2u, /* Timer Timebase */
                        500u,  /* Modulo counter */
                        AppClock_SecondIncrement
                },
                {
                        TIMER_UNUSED,
                        TIMER_16BIT_0,
                        2,
                        10,
                        GPT_CALLBACK_NULL_PTR//App_SecondIncrement
                }
        }
};





const void *Gpt_GetLcfgData(void)
{
    return ((const void *) &Gpt_InitialConfiguration_s);
}


