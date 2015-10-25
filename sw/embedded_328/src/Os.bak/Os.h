/*---------------------------------------------------------------------
 * Os.h (origininal: nano_os.h)
 *
 * Copyright (c) 2010 Tobias Wiese - hacker-tobi@gmx.net
 * Copyright (c) 2015 Armin Schlegel - armin.schlegel@gmx.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *------------------------------------------------------------------------
 */

//Changes
//25-11-10: Added settings for 8535 by Leo www.mikrocontroller.net
//28-12-10: Added settings for Mega32 by Uwe www.mikrocontroller.net
//13-03-11: Added RTOS Task Switching Idea by Rolf www.mikrocontroller.net
//14-03-11: Added support for AtTiny861 by Uwe S. www.mikrocontroller.net
//15-03-11: Added support for Atmega8/Atmega16
//25-10-15: Adapted for standardized SW calls by Armin Schlegel

//+++++++++++++++++++++GLOBAL DEFINE+++++++++++++++++++
//
//#include "Os_TaskControl.h"
//#include "Os_Idle.h"
//#include "Os_Kernel.h"
//#include "Os_Memory.h"
//#include "Os_Message.h"
//#include "Os_Semaphore.h"

#include "Std_Types.h"
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

// undefine to save memory if signal & wait are not needed
//#define TASK_USE_SLEEP

// undefine to save memory if task annihilate is not needed
//#define TASK_USE_ANNIHILATE

// undefine to save memory if message queue is not needed
//#define TASK_USE_MESSAGE

// undefine to save memory if semaphore is not needed
//#define TASK_USE_SEMAPHORE

// undefine to save memory if memory management is not needed
//#define TASK_USE_MEM

// supported devices
#if defined (__AVR_ATmega168__)
#define RAMSTART 	0x0100
#define HEAPSTART	0x02FF
#define TIMER_REGISTER	TCCR0B
#define TIMER_ENABLE	TIMSK0
#define TIMER_VECTOR	TIMER0_OVF_vect
#define CLK1		_BV(CS00)
#define CLK1024		_BV(CS02) |_BV(CS00)

#elif defined (__AVR_ATmega16__)
#define RAMSTART   	0x60
#define HEAPSTART  	0x025F
#define TIMER_REGISTER  TCCR0
#define TIMER_ENABLE  	TIMSK
#define TIMER_VECTOR  	TIMER0_OVF_vect
#define CLK1    	_BV(CS00)
#define CLK1024    	_BV(CS02) |_BV(CS00)

#elif defined (__AVR_ATmega32__)
#define RAMSTART   	0x60
#define HEAPSTART  	0x0460
#define TIMER_REGISTER  TCCR0
#define TIMER_ENABLE  	TIMSK
#define TIMER_VECTOR  	TIMER0_OVF_vect
#define CLK1    	_BV(CS00)
#define CLK1024    	_BV(CS02) |_BV(CS00)

#elif defined (__AVR_ATmega8__)
#define RAMSTART   	0x60
#define HEAPSTART  	0x025F
#define TIMER_REGISTER  TCCR0
#define TIMER_ENABLE  	TIMSK
#define TIMER_VECTOR  	TIMER0_OVF_vect
#define CLK1    	_BV(CS00)
#define CLK1024    	_BV(CS02) |_BV(CS00)

#elif defined (__AVR_ATmega88__)
#define RAMSTART   	0x0100
#define HEAPSTART  	0x02FF
#define TIMER_REGISTER  TCCR0B
#define TIMER_ENABLE  	TIMSK0
#define TIMER_VECTOR  	TIMER0_OVF_vect
#define CLK1    	_BV(CS00)
#define CLK1024    	_BV(CS02) |_BV(CS00)

#elif defined (__AVR_ATmega328P__)
#ifndef RAMSTART
#define RAMSTART   	0x0100
#endif
#ifndef HEAPSTART
#define HEAPSTART  	0x02FF
#endif
#define TIMER_REGISTER  TCCR0B
#define TIMER_ENABLE  	TIMSK0
#define TIMER_VECTOR  	TIMER0_OVF_vect
#define CLK1    	_BV(CS00)
#define CLK1024    	_BV(CS02) |_BV(CS00)

#elif defined (__AVR_AT90s8535__)
#define RAMSTART   	0x060
#define HEAPSTART  	0x015F
#define TIMER_REGISTER  TCCR0
#define TIMER_ENABLE  	TIMSK
#define TIMER_VECTOR  	TIMER0_OVF_vect
#define CLK1    	_BV(CS00)
#define CLK1024    	_BV(CS02) |_BV(CS00)

#elif defined (__AVR_ATtiny861__)
#define RAMSTART   	0x0100
#define HEAPSTART  	0x01FF
#define TIMER_REGISTER  TCCR0B
#define TIMER_ENABLE  	TIMSK
#define TIMER_VECTOR  	TIMER0_OVF_vect
#define TCNT0		TCNT0L	//TCNT0 is a 16-bit-timer in here
#define CLK1    	_BV(CS00)
#define CLK1024    	_BV(CS02) |_BV(CS00)

#else
#error "Device not supported by nano_os. See nano_os.h for details about supported devices."

#endif

//some values

#define TRUE 1
#define FALSE 0
#define ERR 255

//number of tasks in the target system (incl. the idle task)

#define TASKS 3

//Stack size for each task in byte. 
//Basic calculation: 32 + size of all variables in the task with the biggest RAM amount in byte. 
//Often 64 or 96 is a good idea.

#define STACK_SIZE 96

//makros

#define TASK_ENTER_CRITICAL sreg=SREG;wdt_disable();cli()
#define TASK_LEAVE_CRITICAL sei();wdt_enable(WDTO_2S);SREG=sreg
#define MULTITASKING_DISABLE wdt_disable(); TIMER_ENABLE &= ~_BV(TOIE0)
#define MULTITASKING_ENABLE wdt_enable(WDTO_2S); TIMER_ENABLE |= _BV(TOIE0)
#define CONVERT_POINTER_TYPE_TO_MESSAGE(pointer) (void*)pointer
#define CONVERT_POINTER_TYPE_FROM_MESSAGE(message,type) (type)message.pointer


//+++++++++++++++++++++++++MAX TASKS++++++++++++++++++++
// max number of tasks - depending on ram amount
#define MAXTASKS ((RAMEND-RAMSTART-100)/(STACK_SIZE+2))

//+++++++++++++++++++TASK Function Dummy++++++++++++++++
typedef enum
{
    Active = 0,
    Sleeping = 1,
    SleepForLaps = 2,
} Os_TaskState;

typedef enum
{
    Low = 0xff, // 1024 cycles
    Medium  = 0xc0, // 64*1024 cycles
    High    = 0x00  // 256*1024 cycles
} Os_TaskPriority;

typedef volatile struct
{
    uint8 ID;
    Os_TaskPriority priority;
#ifdef TASK_USE_SLEEP
    Os_TaskState state;
    uint8 laps;
#endif
    uint8 sreg;
    uint8 *sp;
    uint8 *stack_section;
} Os_TaskType;

typedef void (*Function)(void);

//+++++++++++++++++++TASK Priorities++++++++++++++++++++
// times & cycles at CLK/1024:


void TIMER_VECTOR(void) __attribute__ ( ( signal, naked ) ); //see scheduler.c


extern void Os_InitUserEnvironment(void);
extern void Os_ShutdownUserEnvironment(void);

void Os_Idle();
void Os_Kernel(void);
void Os_ResetSystem(void);
void Os_ShutdownSystem(void);

#ifdef TASK_USE_MEM
#define MAXBLOCKS 2
#define BLOCKSIZE 2

typedef volatile struct mem_block_intern
{
    uint8 owner;
    volatile void* ptr;
} Os_MemBlockIntern;

uint8 Os_MemoryGetFreeBlocks();
void* Os_MemoryAllocateBlock();
uint8 Os_MemoryFreeBlock(void* ptr);
#endif

#ifdef TASK_USE_MESSAGE

#define MAXQUEUEDEPTH 2

typedef volatile struct Queue
{
    uint8 item_size;
    uint8 queueDepth;
    uint8 number;
    void* ptr;
    uint8 *message;
} Os_QueueType;

uint8 Os_QueueInit(Os_QueueType *qp, uint8 MessageQueueDepth, uint8 item_size);
uint8 Os_QueueRemove(Os_QueueType *qp);
uint8 Os_QueueExists(Os_QueueType *qp);
uint8 Os_QueueGetMaximumSize(Os_QueueType *qp);
uint8 Os_QueueGetFreeSize(Os_QueueType *qp);
uint8 Os_QueueGetNumberOfMessages(Os_QueueType *qp);
uint8 Os_QueueWaitForMessage(Os_QueueType *qp);
uint8 Os_QueueReadByte(Os_QueueType *qp, uint8 position, uint8 delete_message);
uint8 Os_QueueReadItem(Os_QueueType *qp, uint8* targetpointer, uint8 position, uint8 delete_message);
void* Os_QueueReadPointer (Os_QueueType *qp);
uint8 Os_QueueWriteByte (Os_QueueType *qp, uint8 message);
uint8 Os_QueueWriteItem (Os_QueueType *qp, void* startpointer);
uint8 Os_QueueWritePointer (Os_QueueType *qp, void* pointer);
#endif

#ifdef TASK_USE_SEMAPHORE

typedef volatile struct Semaphor
{
    volatile uint8 owner;
} Os_SemaphorType;


void Os_SemaphoreInit(Os_SemaphorType *s);
uint8 Os_SemaphoreSet(Os_SemaphorType *s);
uint8 Os_SemaphoreFree(Os_SemaphorType *s);
uint8 Os_SemaphoreRead(Os_SemaphorType *s);
uint8 Os_SemaphoreWait(Os_SemaphorType *s);
#endif

uint8 Os_TaskInsert(Os_TaskType *t, Function func_name, Os_TaskPriority prio);

void Os_TaskSchedule();
uint8 Os_TaskNewPrio(Os_TaskPriority prio);
uint8 Os_TaskSwitch(Os_TaskType *t);

#ifdef TASK_USE_SLEEP
uint8 Os_TaskWake(Os_TaskType *t, uint8 realtime);
uint8 Os_TaskSleep(Os_TaskType *t);
uint8 Os_TaskSleepForLaps(Os_TaskType *t, uint8 laps);
#endif

#ifdef TASK_USE_ANNIHILATE //see task_control.c
uint8 Os_TaskRemove(Os_TaskType *t);
#endif

#ifdef TASK_USE_MEM
static volatile Os_MemBlockIntern blocks[MAXBLOCKS];
static volatile char data[MAXBLOCKS*BLOCKSIZE];
static volatile uint8 activeBlocks = 0;
#endif
//++++++++++++++++++++GlOBAL VARIABLES+++++++++++++++++

static volatile uint8 sreg = 0;
static volatile uint8 sregsys = 0;
static volatile Os_TaskType *Os_ActiveTask = 0;
static volatile Os_TaskType *Os_TaskContainer[TASKS];
static volatile signed char Os_ActiveTaskId;
volatile uint8 stack_global[TASKS*STACK_SIZE];

