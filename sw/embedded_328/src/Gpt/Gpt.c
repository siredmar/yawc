/*
 * Gpy.c
 *
 *  Created on: Nov 6, 2015
 *      Author: armin
 */
#include "Gpt.h"
#include "Gpt_Lcfg.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Dbg.h"
#include "Tools.h"

static Gpt_CallbackType Gpt_Callback_pv[GPT_MAX_NUM_OF_TIMERS];
static Gpt_ConfigType   *LcfgConfigPtr_ps;

//
static uint16 PrescalerArray[NUMBER_OF_PRESCALERS] = {0, 1, 8, 64, 256, 1024};
//typedef struct
//{
////    Gpt_HwUnitRegisterAdressType   HwUnit[Gpt_MAX_NUM_OF_UARTS];
//    uint8* Gpt_DataRegister_pui8;
//    uint8* Gpt_ControlStatusARegister_pui8;
//    uint8* Gpt_ControlStatusBRegister_pui8;
//    uint8* Gpt_ControlStatusCRegister_pui8;
//    uint8* Gpt_BaudrateLowRegister_pui8;
//    uint8* Gpt_BaudrateHighRegister_pui8;
//} Gpt_RegisterAddressType;
//
//volatile const Gpt_RegisterAddressType Gpt_RegisterAdress_as[GPT_MAX_NUM_OF_TIMERS] =
//{
//        {
//                (uint8*) Gpt_UDR0_ADDRESS,
//                (uint8*) Gpt_UCSR0A_ADDRESS,
//                (uint8*) Gpt_UCSR0B_ADDRESS,
//                (uint8*) Gpt_UCSR0C_ADDRESS,
//                (uint8*) Gpt_UBRR0L_ADDRESS,
//                (uint8*) Gpt_UBRR0H_ADDRESS
//        },
//        {
//                (uint8*) Gpt_UDR0_ADDRESS,
//                (uint8*) Gpt_UCSR0A_ADDRESS,
//                (uint8*) Gpt_UCSR0B_ADDRESS,
//                (uint8*) Gpt_UCSR0C_ADDRESS,
//                (uint8*) Gpt_UBRR0L_ADDRESS,
//                (uint8*) Gpt_UBRR0H_ADDRESS
//        },
//        {
//                (uint8*) Gpt_UDR0_ADDRESS,
//                (uint8*) Gpt_UCSR0A_ADDRESS,
//                (uint8*) Gpt_UCSR0B_ADDRESS,
//                (uint8*) Gpt_UCSR0C_ADDRESS,
//                (uint8*) Gpt_UBRR0L_ADDRESS,
//                (uint8*) Gpt_UBRR0H_ADDRESS
//        }
//};


Gpt_TimerCalcType Gpt_ConfigureTimer(Gpt_TimerType TimerType, uint16 targetMs)
{
    uint8 dbg_str[30];
    Gpt_TimerCalcType Timer;
    float32 TCNT_Temp_f32 = 0.0;
    uint8 prescalerCounter = 0;
    float32 TimerDecimals_f32 = 0.0;

    float32 TimerBoundary = 0.0;
    if(TimerType == TIMER_16BIT_0)
    {
        TimerBoundary = 65535.0;
    }
    else
    {
        TimerBoundary = 255.0;
    }

    for(prescalerCounter = PRESCALER_1; prescalerCounter <= PRESCALER_1024; prescalerCounter++)
    {
        Timer.prescaler = prescalerCounter;
        TCNT_Temp_f32  = (((float32)targetMs) * (float32)(F_CPU/1000) / (float32)PrescalerArray[prescalerCounter]) - 1;
//        printf("TCNT_Temp_f32: %f\n", TCNT_Temp_f32);
        TimerDecimals_f32 = (GetFloatDecimals(TCNT_Temp_f32));
        if(TCNT_Temp_f32 <= TimerBoundary)
//        if((TimerDecimals_f32 <= 0.25) && (TCNT_Temp_f32 <= TimerBoundary))
        {
//            printf("Decimals: %f\n", (GetFloatDecimals(TCNT_Temp_f32)));
            Timer.TCNT_Reg = (uint16)TCNT_Temp_f32;
            Timer.classifier = 1;
//            Timer.Rest = TimerDecimals_f32;
            Timer.RestCounterLimit = (uint16)(1.0f/TimerDecimals_f32);
//            Timer.RestCounterUpDown = 1;

//            printf("Timer.Rest: %f\n", Timer.Rest);
//            printf("Timer.RestCounterLimit: %d\n",  Timer.RestCounterLimit);
            break;
        }
//        else if((TimerDecimals_f32 >= 0.75) && (TCNT_Temp_f32 <= TimerBoundary))
//        {
//            printf("Decimals: %f\n", (GetFloatDecimals(TCNT_Temp_f32)));
//            Timer.TCNT_Reg = (uint16)TCNT_Temp_f32 + 1;
//            Timer.classifier = 1;
//            Timer.Rest = 1 - TimerDecimals_f32;
//            Timer.RestCounterLimit = (uint16)(1.0f/Timer.Rest);
//            Timer.RestCounterUpDown = -1;
//
//            printf("Timer.Rest: %f\n", Timer.Rest);
//            printf("Timer.RestCounterLimit: %d\n",  Timer.RestCounterLimit);
//            break;
//        }
        else
        {
//            printf("Decimals: %f\n", (GetFloatDecimals(TCNT_Temp_f32)));
            Timer.classifier = 0;
        }
    }
    Dbg_ReadVariableInteger("TCNT_Reg: ", Timer.TCNT_Reg);
    Dbg_ReadVariableInteger("prescaler: ", PrescalerArray[Timer.prescaler]);
    Dbg_ReadVariableInteger("classifier: ", (float32)Timer.classifier);
//    Dbg_ReadVariableFloat("Rest: ", (float32)Timer.Rest);
//    Dbg_ReadVariableFloat("RestCounter: ", (float32)Timer.RestCounter);
    Dbg_ReadVariableInteger("RestCounterLimit: ", (float32)Timer.RestCounterLimit);
//    Dbg_ReadVariableFloat("RestCounterUpDown: ", (float32)Timer.RestCounterUpDown);
    return Timer;
}

Gpt_TimerCalcType Timer[GPT_MAX_NUM_OF_TIMERS];

void Gpt_Init(void)
{
    Gpt_TimerType TimerCounter = TIMER_8BIT_0;


    LcfgConfigPtr_ps = (Gpt_ConfigType*)Gpt_GetLcfgData();

    if(LcfgConfigPtr_ps->Timer[TIMER_8BIT_0].TimerUsed_e == TIMER_USED)
    {
        TIMSK0 &= ~(_BV(OCIE0A));               // disable interrupt
        TCCR0A = _BV(WGM01);                // set CTC mode

        Timer[TIMER_8BIT_0] = Gpt_ConfigureTimer(TIMER_8BIT_0, LcfgConfigPtr_ps->Timer[TIMER_8BIT_0].TimerDelayMs_ui16);
        if(Timer[TIMER_8BIT_0].classifier == 1)
        {
            Uart_WriteString(UART_HWUNIT_0, "II: CONFIGURED 8 BIT TIMER0\n\r");
            OCR0A = (uint8)Timer[TIMER_8BIT_0].TCNT_Reg;                      // set top value
            TCCR0B = (uint8)Timer[TIMER_8BIT_0].prescaler;                 // set prescaler
        }
        else
        {
            Uart_WriteString(UART_HWUNIT_0, "EE: FAILED TO INITIALIZE 8 BIT TIMER0\n\r");
        }
        TCNT0 = 0;                          // reset TCNT counter register
        TIMSK0 |= _BV(OCIE0A);              // enable interrupt
    }

    if(LcfgConfigPtr_ps->Timer[TIMER_8BIT_1].TimerUsed_e == TIMER_USED)
    {
        TIMSK2 &= ~(_BV(OCIE2A));
        TCCR2A = _BV(WGM21);
        ASSR = 0;

        Timer[TIMER_8BIT_1] = Gpt_ConfigureTimer(TIMER_8BIT_1, LcfgConfigPtr_ps->Timer[TIMER_8BIT_1].TimerDelayMs_ui16);
        if(Timer[TIMER_8BIT_1].classifier == 1)
        {
            Uart_WriteString(UART_HWUNIT_0, "II: CONFIGURED 8 BIT TIMER1\n\r");
            TCCR2B = (uint8)Timer[TIMER_8BIT_1].prescaler;
            OCR2A  = (uint8)Timer[TIMER_8BIT_1].TCNT_Reg;
        }
        else
        {
            Uart_WriteString(UART_HWUNIT_0, "EE: FAILED TO INITIALIZE 8 BIT TIMER1\n\r");
        }
        TCNT2 = 0;
        TIMSK2 |= _BV(OCIE2A);
    }

    if(LcfgConfigPtr_ps->Timer[TIMER_16BIT_0].TimerUsed_e == TIMER_USED)
    {
        TIMSK1 &= ~(_BV(OCIE1A));

        Timer[TIMER_16BIT_0] = Gpt_ConfigureTimer(TIMER_16BIT_0, LcfgConfigPtr_ps->Timer[TIMER_16BIT_0].TimerDelayMs_ui16);
        if(Timer[TIMER_16BIT_0].classifier == 1)
        {
            Uart_WriteString(UART_HWUNIT_0, "II: CONFIGURED 16 BIT TIMER0\n\r");
            Dbg_ReadVariableInteger("TCNT_Reg: ", (uint32)Timer[TIMER_16BIT_0].TCNT_Reg);
            Dbg_ReadVariableInteger("prescaler: ", (uint32)Timer[TIMER_16BIT_0].prescaler);
            TCCR1B = (uint8)Timer[TIMER_16BIT_0].prescaler | _BV(WGM12);
            OCR1A = (uint16)Timer[TIMER_16BIT_0].TCNT_Reg;
        }
        else
        {
            Uart_WriteString(UART_HWUNIT_0, "EE: FAILED TO INITIALIZE 16 BIT TIMER0\n\r");
            Dbg_ReadVariableInteger("TCNT_Reg: ", (uint32)Timer[TIMER_16BIT_0].TCNT_Reg);
            Dbg_ReadVariableInteger("prescaler: ", (uint32)Timer[TIMER_16BIT_0].prescaler);
        }
        TCCR1A = 0;
        TCNT1 = 0;
        TIMSK1 |= _BV(OCIE1A);
    }
}

Std_ReturnType Gpt_TimerStart(Gpt_TimerType timer)
{
    switch(timer)
    {
        case TIMER_8BIT_0:
            if(LcfgConfigPtr_ps->Timer[TIMER_8BIT_0].TimerUsed_e == TIMER_USED)
            {
                TCCR0B = Timer[TIMER_8BIT_0].prescaler;
            }
            break;
        case TIMER_8BIT_1:
            if(LcfgConfigPtr_ps->Timer[TIMER_8BIT_1].TimerUsed_e == TIMER_USED)
            {
                TCCR1B = Timer[TIMER_8BIT_1].prescaler | _BV(WGM12);
            }
            break;
        case TIMER_16BIT_0:
            if(LcfgConfigPtr_ps->Timer[TIMER_16BIT_0].TimerUsed_e == TIMER_USED)
            {
                TCCR2B = Timer[TIMER_16BIT_0].prescaler;
            }
            break;
        default:
            break;
    }
}

Std_ReturnType Gpt_TimerStop(Gpt_TimerType timer)
{
    switch(timer)
    {
        case TIMER_8BIT_0:
            if(LcfgConfigPtr_ps->Timer[TIMER_8BIT_0].TimerUsed_e == TIMER_USED)
            {
                TCCR0B = PRESCALER_TIMER_OFF;
            }
            break;
        case TIMER_8BIT_1:
            if(LcfgConfigPtr_ps->Timer[TIMER_8BIT_1].TimerUsed_e == TIMER_USED)
            {
                TCCR1B = PRESCALER_TIMER_OFF;
            }
            break;
        case TIMER_16BIT_0:
            if(LcfgConfigPtr_ps->Timer[TIMER_16BIT_0].TimerUsed_e == TIMER_USED)
            {
                TCCR2B = PRESCALER_TIMER_OFF;
            }
            break;
        default:
            break;
    }
}


ISR(TIMER0_COMPA_vect)
{
    static uint16 Period_ui16 = 0;
    if((Period_ui16 % Timer[TIMER_8BIT_0].RestCounterLimit) != 0)
    {
        Period_ui16++;
    }
    else
    {
        /* dont increment because we need to compensate oscillator */
    }

    if(Period_ui16 >= LcfgConfigPtr_ps->Timer[TIMER_8BIT_0].TimerModuloCounter_ui16)
    {
        Period_ui16 = 0;
        if(LcfgConfigPtr_ps->Timer[TIMER_8BIT_0].Gpt_Callback_pv != GPT_CALLBACK_NULL_PTR)
        {
            LcfgConfigPtr_ps->Timer[TIMER_8BIT_0].Gpt_Callback_pv();
        }
    }
}

ISR(TIMER1_COMPA_vect)
{
    static uint16 Period_ui16 = 0;
//    if((Period_ui16 % Timer[TIMER_16BIT_0].RestCounterLimit) != 0)
//    {
        Period_ui16++;
//    }
//    else
//    {
//        /* dont increment because we need to compensate oscillator */
//    }
    if(Period_ui16 >= LcfgConfigPtr_ps->Timer[TIMER_16BIT_0].TimerModuloCounter_ui16)
    {
        Period_ui16 = 0;
        if(LcfgConfigPtr_ps->Timer[TIMER_16BIT_0].Gpt_Callback_pv != GPT_CALLBACK_NULL_PTR)
        {
            LcfgConfigPtr_ps->Timer[TIMER_16BIT_0].Gpt_Callback_pv();
        }
    }
}

ISR(TIMER2_COMPA_vect)
{
    static uint16 Period_ui16 = 0;
    if((Period_ui16 % Timer[TIMER_8BIT_1].RestCounterLimit) != 0)
    {
        Period_ui16++;
    }
    else
    {
        /* dont increment because we need to compensate oscillator */
    }
    if(Period_ui16 >= LcfgConfigPtr_ps->Timer[TIMER_8BIT_1].TimerModuloCounter_ui16)
    {
        Period_ui16 = 0;
        if(LcfgConfigPtr_ps->Timer[TIMER_8BIT_1].Gpt_Callback_pv != GPT_CALLBACK_NULL_PTR)
        {
            LcfgConfigPtr_ps->Timer[TIMER_8BIT_1].Gpt_Callback_pv();
        }
    }
}
