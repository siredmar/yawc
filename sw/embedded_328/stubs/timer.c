#ifdef STUBS
#include <stdio.h>
#include <stdlib.h>

#define F_CPU 16020028UL
typedef unsigned char uint8;
typedef signed char sint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef float float32;


uint8 TCCR0;
uint8 TCNT0;

uint8 prescaler;
uint8 targetMilliseconds;

typedef enum
{
    PRESCALER_TIMER_OFF      = (0U),
    PRESCALER_0,
    PRESCALER_8,
    PRESCALER_64,
    PRESCALER_256,
    PRESCALER_1024,
    NUMBER_OF_PRESCALERS,
} Gpt_PrescalerType;

uint16 PrescalerArray[6] = {0, 1, 8, 64, 256, 1024};

typedef struct
{
    uint16 TCNT_Reg;
    Gpt_PrescalerType prescaler;
    uint8 classifier;
    float32 Rest;
    uint16 RestCounter;
    uint16 RestCounterLimit;
    sint8 RestCounterUpDown;
} Gpt_TimerCalcType;

typedef enum
{
    TIMER_8BIT_0 = (0u),
    TIMER_8BIT_1,
    TIMER_16BIT_0,
    GPT_MAX_NUM_OF_TIMERS
} Gpt_TimerType;

uint8 FloatContainsDecimals(float32 num)
{
    float32 intpart = (int)num;
    float32 decpart = (float32)((float32)num - (float32)intpart);
    if(decpart == 0.0f)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
float32 GetFloatDecimals(float32 num)
{
    float32 intpart = (uint32)num;
    float32 decpart = (float32)((float32)num - (float32)intpart);
    return decpart;
}


Gpt_TimerCalcType Gpt_ConfigureTimer(Gpt_TimerType TimerType, uint16 targetMs)
{
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

    for(prescalerCounter = PRESCALER_0; prescalerCounter <= PRESCALER_1024; prescalerCounter++)
//        for(prescalerCounter = PRESCALER_1024; prescalerCounter >= PRESCALER_0; prescalerCounter--)
    {
        Timer.prescaler = prescalerCounter;
        TCNT_Temp_f32  = (((float32)targetMs) * (float32)(F_CPU/1000) / (float32)PrescalerArray[prescalerCounter]) - 1;
        printf("TCNT_Temp_f32: %f\n", TCNT_Temp_f32);
        TimerDecimals_f32 = (GetFloatDecimals(TCNT_Temp_f32));
        if(TCNT_Temp_f32 <= TimerBoundary)
//        if((TimerDecimals_f32 <= 0.25) && (TCNT_Temp_f32 <= TimerBoundary))
        {
            printf("Decimals: %f\n", (GetFloatDecimals(TCNT_Temp_f32)));
            Timer.TCNT_Reg = (uint16)TCNT_Temp_f32;
            Timer.classifier = 1;
            Timer.Rest = TimerDecimals_f32;
            Timer.RestCounterLimit = (uint16)(1.0f/TimerDecimals_f32);
            Timer.RestCounterUpDown = 1;

            printf("Timer.Rest: %f\n", Timer.Rest);
            printf("Timer.RestCounterLimit: %d\n",  Timer.RestCounterLimit);
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
            printf("Decimals: %f\n", (GetFloatDecimals(TCNT_Temp_f32)));
            Timer.classifier = 0;
        }
    }
    printf("##########################\n");
    printf("TCNT_Reg: %d\n", (uint16)  Timer.TCNT_Reg);
    printf("prescaler: %d\n", (uint16) Timer.prescaler);
    printf("classifier: %d\n", (uint16)Timer.classifier);
    printf("Rest: %f\n", Timer.Rest);
    printf("RestCounterLimit: %d\n", (uint16)Timer.RestCounterLimit);
    printf("RestCounter: %d\n", (uint16)Timer.RestCounter);

    return Timer;
}

int main(void)
{
    Gpt_TimerCalcType timer;
    timer = Gpt_ConfigureTimer(TIMER_16BIT_0, 540);
    printf("timer.TCNT_Reg: %d\n", timer.TCNT_Reg);
    printf("timer.prescaler: %d\n", PrescalerArray[timer.prescaler]);
    printf("timer.classifier: %d\n", timer.classifier);
    return 0;
}
#endif
