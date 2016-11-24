/*
 * Gpt.h
 *
 *  Created on: Nov 6, 2015
 *      Author: armin
 */

#ifndef SRC_GPT_GPT_H_
#define SRC_GPT_GPT_H_

#include <avr/io.h>
#include "Std_Types.h"
#include "Gpt_Cfg.h"

#define GPT_CALLBACK_NULL_PTR ((void*)0)
typedef void (*Gpt_CallbackType)(void);

typedef enum
{
    TIMER_8BIT_0 = (0u),
    TIMER_8BIT_1,
    TIMER_16BIT_0,
    GPT_MAX_NUM_OF_TIMERS
} Gpt_TimerType;

typedef enum
{
    TIMER_UNUSED = (0u),
    TIMER_USED
} Gpt_TimerUsedType;

typedef enum
{
    PRESCALER_TIMER_OFF = (0U),
    PRESCALER_1,
    PRESCALER_8,
    PRESCALER_64,
    PRESCALER_256,
    PRESCALER_1024,
    NUMBER_OF_PRESCALERS
} Gpt_PrescalerType;

typedef struct
{
    Gpt_TimerUsedType   TimerUsed_e;
    Gpt_TimerType       TimerType_e;
    uint16              TimerDelayMs_ui16;
    uint16              TimerModuloCounter_ui16;
    Gpt_CallbackType    Gpt_Callback_pv;
} Gpt_ConfigTimerType;


typedef struct
{
    Gpt_ConfigTimerType Timer[GPT_MAX_NUM_OF_TIMERS];
} Gpt_ConfigType;



typedef struct
{
    uint16 TCNT_Reg;
    Gpt_PrescalerType prescaler;
    uint8 classifier;
//    float32 Rest;
//    uint16 RestCounter;
    uint16 RestCounterLimit;
//    sint8 RestCounterUpDown;
} Gpt_TimerCalcType;


void Gpt_Init(void);
Std_ReturnType Gpt_TimerStart(Gpt_TimerType timer);
Std_ReturnType Gpt_TimerStop(Gpt_TimerType timer);

#endif /* SRC_GPT_GPT_H_ */
