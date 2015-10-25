/**
* COPYRIGHT : GNU GPL V2 -
*
* This is the main header of the Os
*
* Author: Armin Schlegel
* based on project eRTK (https://github.com/eRTK/eRTK)
* Creation date: 2015-09-21
* Last change: 2015-09-21
*
* Moduleversion: 0.0.1
****************************************************************************** */

#ifndef OS_H_
#define OS_H_
#include <stddef.h>
#include <stdint.h>
#include <Os_Cfg.h>
#include <Std_Types.h>

//typedef enum
//{
//    OS_TASK_PRIO_0 = (0u),
//
//
//};

typedef uint8 Os_TaskPrioType;
typedef uint8 Os_TaskIdType;
typedef uint8 Os_TaskTimerType;

typedef enum
{
    OS_SEMAPHORE_0 = (0u),
    OS_SEMAPHORE_1,
    OS_SEMAPHORE_2,
    OS_SEMAPHORE_3,
    OS_SEMAPHORE_4,
    OS_SEMAPHORE_5,
    OS_SEMAPHORE_6,
    OS_SEMAPHORE_7,
    OS_SEMAPHORE_8,
    OS_SEMAPHORE_9,
    OS_MAX_NUM_OF_SEMAPHORE
} Os_SemaphoreType;

//gruende fuer einen toten bueffel
typedef enum
{
    OS_SYS_NOTASK = (0u),
    OS_SYS_NULLPTR,
    OS_SYS_NULLTIMER,
    OS_SYS_OVERLOAD,
    OS_SYS_VERIFY,
    OS_SYS_STACKOVERFLOW,
    OS_SYS_UNKNOWN
} Os_ErrorHookType;


typedef enum
{
    OS_TASK_SCHEDULE_PLANNED = (0u),
    OS_TASK_SCHEDULE_IMMEDIATELY
} Os_ScheduleImmediatyleType;


//der task control block
typedef struct
{
    void (*TaskHandler )(uint16 param0, void *param1);
    Os_TaskPrioType TaskPrio;
    uint16 param0;
    void * param1;
} Os_TaskConfigType;

/* struktur task control block */
typedef struct Os_TaskControlBlockType {    /* queue mit ordnungskriterium task prioritaet 255..0 */
    struct Os_TaskControlBlockType * NextTask;   /* 1. element immer verkettungszeiger !!! */
    struct Os_TaskControlBlockType * PreviousTask; /* vorgaenger in liste */
    Os_TaskIdType                    TaskId;          /* index aus der reihenfolge der statischen deklaration */
    Os_TaskPrioType                  TaskPrio;         /* 0..255 */
    Os_TaskTimerType                 TaskTimer;        /* timeout counter mit system tick aufloesung */
#ifdef OS_DEBUG
    uint16 param0;
    void * param1;
#endif
} Os_TaskControlBlockType;

union
{
    volatile uint8 timer8[4];
    volatile uint16 timer16[2];
    volatile uint32 timer32;
} Os_TimerStruct;

union
{
    uint32 ui32;
    uint8 ui8[4];
    void (*TaskPointer)(uint16 param0, void *param1);
    void (*IdleTaskPointer)(void);
    void (*ErrorHookPointer)(Os_ErrorHookType);
} TaskAddress;

//extern const Os_TaskConfigType Os_TaskConfig[OS_MAX_NUMBER_OF_TASKS]; //das soll in der anwendung definiert sein

void __attribute__ ((naked)) eRTK_scheduler(void); // start der hoechstprioren ready task, notfalls idle

//eRTK funktionsinterface
void          Os_Init(void);                  //initialisieren der datenstrukturen
void          Os_Idle(void);
//void          Os_TimerInit(void);            //system timer initialisieren
void          Os_Start(void);                    //start der hoechstprioren ready task, notfalls idle
uint32         Os_GetTimer32(void);             //systemzeit 1000Hz in 8 Bit
Os_TaskIdType Os_GetTaskId(void);                //holen der eigenen task id
void          Os_SetReady(Os_TaskIdType TaskId);     //task fuer bereit erklaeren
void          Os_SetSuspended(Os_TaskIdType TaskId); //task suspendieren
void          Os_WaitUntil(uint32 then);   //warte auf den zeitpunkt
void          Os_SleepMs(uint16 ms);       //warte eine gewisse zeit
void          Os_GetSema(Os_SemaphoreType semaid);  //Warten bis Semaphore frei ist und danach besetzen
void          Os_FreeSema(Os_SemaphoreType semaid);
void          Os_SemaphoreInit(void);
void          Os_SuspendAndWait(uint8 timeout);    //Task suspendieren fuer eine gewisse zeit
void          Os_ChangeTaskPriority(Os_TaskIdType TaskId, Os_TaskPrioType prio, Os_ScheduleImmediatyleType schedule);
void          Os_Scheduler(void);

//allgemeine fehlerbehandlung, kann je nach bedarf umgelenkt werden
void Os_ErrorHook(Os_ErrorHookType error);              //allgemeine fehler routine

#endif


