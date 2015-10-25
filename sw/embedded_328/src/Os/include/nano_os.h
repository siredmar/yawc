/*---------------------------------------------------------------------
 * nano_os.h
 *
 * Copyright (c) 2010 Tobias Wiese - hacker-tobi@gmx.net
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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>     // Watchdog timer to repair lockups

//+++++++++++++++++++++GLOBAL DEFINE+++++++++++++++++++

/*  set in Makefile ! */
// undefine to save memory if signal & wait are not needed
#define TASK_USE_SLEEP

// undefine to save memory if task anhilate is not needed
#define TASK_USE_ANHILATE

// undefine to save memory if message queue is not needed
#define TASK_USE_MESSAGE

// undefine to save memory if semaphore is not needed
// #define TASK_USE_SEMAPHORE

// undefine to save memory if memory management is not needed
#define TASK_USE_MEM


// supported devices
#if defined (__AVR_ATmega168__)
#ifndef RAMSTART // avr-gcc 4.5.x define that
#define RAMSTART 	0x0100
#endif
#define HEAPSTART	0x02FF
#define TIMER_REGISTER	TCCR0B
#define TIMER_ENABLE	TIMSK0
#define TIMER_VECTOR	TIMER0_OVF_vect
#define CLK1		_BV(CS00)
#define CLK1024		_BV(CS02) |_BV(CS00)

#elif defined (__AVR_ATmega16__)
#ifndef RAMSTART // avr-gcc 4.5.x define that
#define RAMSTART   	0x60
#endif
#define HEAPSTART  	0x025F
#define TIMER_REGISTER  TCCR0
#define TIMER_ENABLE  	TIMSK
#define TIMER_VECTOR  	TIMER0_OVF_vect
#define CLK1    	_BV(CS00)
#define CLK1024    	_BV(CS02) |_BV(CS00)

#elif defined (__AVR_ATmega32__)
#ifndef RAMSTART // avr-gcc 4.5.x define that
#define RAMSTART   	0x60
#endif
#define HEAPSTART  	0x0460
#define TIMER_REGISTER  TCCR0
#define TIMER_ENABLE  	TIMSK
#define TIMER_VECTOR  	TIMER0_OVF_vect
#define CLK1    	_BV(CS00)
#define CLK1024    	_BV(CS02) |_BV(CS00)

#elif defined (__AVR_ATmega8__)
#ifndef RAMSTART // avr-gcc 4.5.x define that
#define RAMSTART   	0x60
#endif
#define HEAPSTART  	0x025F
#define TIMER_REGISTER  TCCR0
#define TIMER_ENABLE  	TIMSK
#define TIMER_VECTOR  	TIMER0_OVF_vect
#define CLK1    	_BV(CS00)
#define CLK1024    	_BV(CS02) |_BV(CS00)

#elif defined (__AVR_ATmega88__)
#ifndef RAMSTART // avr-gcc 4.5.x define that
#define RAMSTART   	0x0100
#endif
#define HEAPSTART  	0x02FF
#define TIMER_REGISTER  TCCR0B
#define TIMER_ENABLE  	TIMSK0
#define TIMER_VECTOR  	TIMER0_OVF_vect
#define CLK1    	_BV(CS00)
#define CLK1024    	_BV(CS02) |_BV(CS00)

#elif defined (__AVR_ATmega328P__)
#ifndef RAMSTART // avr-gcc 4.5.x define that
#define RAMSTART    0x0100
#endif
#define HEAPSTART   0x02FF
#define TIMER_REGISTER  TCCR0B
#define TIMER_ENABLE    TIMSK0
#define TIMER_VECTOR    TIMER0_OVF_vect
#define CLK1        _BV(CS00)
#define CLK1024     _BV(CS02) |_BV(CS00)

#elif defined (__AVR_AT90s8535__)
#ifndef RAMSTART // avr-gcc 4.5.x define that
#define RAMSTART   	0x060
#endif
#define HEAPSTART  	0x015F
#define TIMER_REGISTER  TCCR0
#define TIMER_ENABLE  	TIMSK
#define TIMER_VECTOR  	TIMER0_OVF_vect
#define CLK1    	_BV(CS00)
#define CLK1024    	_BV(CS02) |_BV(CS00)

#elif defined (__AVR_ATtiny861__)
#ifndef RAMSTART // avr-gcc 4.5.x define that
#define RAMSTART   	0x0100
#endif
#define HEAPSTART  	0x01FF
#define TIMER_REGISTER  TCCR0B
#define TIMER_ENABLE  	TIMSK
#define TIMER_VECTOR  	TIMER0_OVF_vect
#define TCNT0		TCNT0L	//TCNT0 is a 16-bit-timer in here
#define CLK1    	_BV(CS00)
#define CLK1024    	_BV(CS02) |_BV(CS00)

#else
#	error "Device not supported by nano_os. See nano_os.h for details about supported devices."
#endif

//some values

#define TRUE 1
#define FALSE 0
#define ERR 255

//number of tasks in the target system (incl. the idle task)

#ifndef TASKS
#define TASKS 4
#endif

//Stack size for each task in byte. 
//Basic calculation: 32 + size of all variables in the task with the biggest RAM amount in byte. 
//Often 64 or 96 is a good idea.

#ifndef STACK_SIZE
#define STACK_SIZE 96
#endif

//makros

#define TASK_ENTER_CRITICAL sreg=SREG;wdt_disable();cli()
#define TASK_LEAVE_CRITICAL sei();wdt_enable(WDTO_2S);SREG=sreg
#define MULTITASKING_DISABLE wdt_disable(); TIMER_ENABLE &= ~_BV(TOIE0)
#define MULTITASKING_ENABLE wdt_enable(WDTO_2S); TIMER_ENABLE |= _BV(TOIE0)
#define CONVERT_POINTER_TYPE_TO_MESSAGE(pointer) (void*)pointer
#define CONVERT_POINTER_TYPE_FROM_MESSAGE(message,type) (type)message.pointer

#define STATIC
//#define STATIC static

//+++++++++++++++++++++++++MAX TASKS++++++++++++++++++++
// max number of tasks - depending on ram amount
#define MAXTASKS ((RAMEND-RAMSTART-100)/(STACK_SIZE+2))

//+++++++++++++++++++TASK Function Dummy++++++++++++++++
typedef void (*Function)(void);

//+++++++++++++++++++TASK Priorities++++++++++++++++++++
// times & cycles at CLK/1024:
typedef enum Priority
{
    Low	= 0xff,	// 1024 cycles
    Medium	= 0xc0,	// 64*1024 cycles
    High	= 0x00	// 256*1024 cycles
} Priority;

//++++++++++++++++++TASK State++++++++++++++++++++++++++
#ifdef TASK_USE_SLEEP
typedef enum State
{
    Active = 0,
    Sleeping = 1,
    SleepForLaps = 2,
} State;
#endif

//+++++++++++++++++TASK Control Structure+++++++++++++++
typedef volatile struct Task
        {
    uint8_t ID;
    Priority priority;
#ifdef TASK_USE_SLEEP
    State state;
    uint8_t laps;
#endif
    unsigned char sreg;
    unsigned char *sp;
    unsigned char *stack_section;
        } Task;

        //+++++++++++++++++++++++SEMAPHORE+++++++++++++++++++++

#ifdef TASK_USE_SEMAPHORE

        typedef volatile struct Semaphor
                {
            volatile uint8_t owner;
                } Semaphor;

#endif

                //+++++++++++++++++++++MESSAGE QUEUE+++++++++++++++++++

#ifdef TASK_USE_MESSAGE
#define MAXQUEUEDEPTH 2

                typedef volatile struct Queue
                        {
                    uint8_t item_size;
                    uint8_t queueDepth;
                    uint8_t number;
                    void* ptr;
                    unsigned char *message;
                        } Queue;

#endif

                        //+++++++++++++++++MEMORY MANAGEMENT+++++++++++++++++++
#ifdef TASK_USE_MEM
#define MAXBLOCKS 2
#define BLOCKSIZE 2

                        typedef volatile struct mem_block_intern
                                {
                            uint8_t owner;
                            volatile void* ptr;
                                } mem_block_intern;

                                extern STATIC volatile char data[MAXBLOCKS*BLOCKSIZE];
                                extern STATIC volatile mem_block_intern blocks[MAXBLOCKS];

#endif

                                //+++++++++++++++++++++++PROTOTYPE+++++++++++++++++++++

                                STATIC void kernel(void);
                                void TIMER_VECTOR(void) __attribute__ ( ( signal, naked ) ); //see scheduler.c
                                void idle(void);

                                STATIC void init_user_environment(void);
                                STATIC void shutdown_user_environment(void);

                                void reset_system(void);
                                void shutdown_system(void);

#ifdef TASK_USE_MESSAGE //see message.c
                                uint8_t queue_init(Queue *qp, uint8_t MessageQueueDepth, uint8_t item_size);
                                uint8_t queue_remove(Queue *qp);
                                uint8_t queue_exists(Queue *qp);
                                uint8_t queue_get_maximum_size(Queue *qp);
                                uint8_t queue_get_free_size(Queue *qp);
                                uint8_t queue_get_number_of_messages(Queue *qp);
                                uint8_t queue_wait_for_message(Queue *qp);
                                unsigned char queue_read_byte(Queue *qp, uint8_t position, uint8_t delete_message);
                                uint8_t queue_read_item(Queue *qp, unsigned char* targetpointer, uint8_t position, uint8_t delete_message);
                                void* queue_read_pointer (Queue *qp);
                                uint8_t queue_write_byte (Queue *qp, unsigned char message);
                                uint8_t queue_write_item(Queue *qp, void* startpointer);
                                uint8_t queue_write_pointer (Queue *qp, void* pointer);
#endif

#ifdef TASK_USE_SEMAPHORE //see semaphore.c 
                                void semaphore_init(Semaphor *s);
                                uint8_t semaphore_set(Semaphor *s);
                                uint8_t semaphore_free(Semaphor *s);
                                uint8_t semaphore_read(Semaphor *s);
                                uint8_t semaphore_wait(Semaphor *s);
#endif

#ifdef TASK_USE_MEM //see memory.c
                                uint8_t memory_get_free_blocks(void);
                                void* memory_allocate_block(void);
                                uint8_t memory_free_block(void* ptr);
#endif

                                //see task_control.c
                                uint8_t task_insert(Task *t, Function func_name, Priority prio);
                                void task_schedule(void);
                                uint8_t task_new_prio(Priority prio);
                                uint8_t task_switch(Task *t);

#ifdef TASK_USE_ANHILATE //see task_control.c
                                uint8_t task_remove(Task *t);
#endif

#ifdef TASK_USE_SLEEP //see task_control.c
                                uint8_t task_wake(Task *t, uint8_t realtime);
                                uint8_t task_sleep(Task *t);
                                uint8_t task_sleep_for_laps(Task *t, uint8_t laps);
#endif

                                //++++++++++++++++++++GlOBAL VARIABLES+++++++++++++++++

                                extern STATIC volatile signed char active_task_id;
#ifdef TASK_USE_MEM
                                extern STATIC volatile uint8_t activeBlocks;
#endif
                                extern STATIC volatile Task *task_container[TASKS], *active_task;
                                extern STATIC volatile uint8_t sreg, sregsys;
                                extern volatile unsigned char stack_global[TASKS*STACK_SIZE];
                                extern Task idletask;

