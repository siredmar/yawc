/*---------------------------------------------------------------------
 * message.c
 *
 * Copyright (c) 2010 Tobias Wiese - hacker-tobi@gmx.net
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *------------------------------------------------------------------------
 */

#include "Os_Message.h"
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#ifdef TASK_USE_MESSAGE

//***********************************************************************
//*queue_init(Queue *qp, uint8 MessageQueueDepth)  			*
//*                                 					*
//* description: init a message queue     				*
//* input: Queue *qp: Pointer to queue, MessageQueueDepth: depth in byte*
//* output: TRUE(ok), ZERO(error)                			*
//***********************************************************************

uint8 Os_QueueInit(Os_QueueType *qp, uint8 MessageQueueDepth, uint8 item_size)
{
    sregsys = SREG;
    cli();
    if (MessageQueueDepth == 0 || item_size == 0 || MessageQueueDepth > MAXQUEUEDEPTH)
    {
        SREG = sregsys;
        return 0;
    }
    //allocate memory for queue dynamically
    qp->message = (uint8*) malloc (MessageQueueDepth*item_size);
    if (qp->message == 0 || (uint16) qp->message > SP)
    {
        qp->queueDepth = 0;
        SREG = sregsys;
        return 0;
    }
    qp->queueDepth = MessageQueueDepth*item_size;
    qp->item_size = item_size;
    qp->ptr = 0;
    SREG = sregsys;
    return TRUE;
}

//****************************************************
//*queue_remove(Queue *qp)                         *
//*                                                  *
//* description: delete a queue                      *
//* input: Queue *qp: Pointer to queue               *
//* output:  TRUE(ok), ZERO(error)                   *
//****************************************************

uint8 Os_QueueRemove(Os_QueueType *qp)
{
    sregsys = SREG;
    cli();
    if (qp->message == 0)
    {
        SREG = sregsys;
        return 0;
    }
    //free memory and reset other values
    free ((void*) qp->message);
    qp->message = 0;
    qp->number = 0;
    qp->queueDepth = 0;
    SREG = sregsys;
    return TRUE;
}

//****************************************************
//*queue_exists(Queue *qp)                           *
//*                                                  *
//* description: check if a queue exists             *
//* input: Queue *qp: Pointer to queue               *
//* output: free places in the queue                 *
//****************************************************

uint8 Os_QueueExists(Os_QueueType *qp)
{
    if (qp->queueDepth)
        return TRUE;
    return 0;
}

//****************************************************
//*queue_get_maximum_size(Queue *qp)                 *
//*                                                  *
//* description: get the maximum queue size          *
//* input: Queue *qp: Pointer to queue               *
//* output:  maximum number of messages in the queue *
//****************************************************

uint8 Os_QueueGetMaximumSize(Os_QueueType *qp)
{
    return qp->queueDepth;
}

//****************************************************
//*queue_get_free_size(Queue *qp)                    *
//*                                                  *
//* description: get the free queue size             *
//* input: Pointer to queue                          *
//* output: free places in the queue                 *
//****************************************************

uint8 Os_QueueGetFreeSize(Os_QueueType *qp)
{
    return qp->queueDepth - qp->number;
}

//****************************************************
//*queue_get_number_of_messages(Queue *qp)           *
//*                                                  *
//* description: check if new message is in a queue  *
//* input: Queue *qp: Pointer to queue               *
//* output:  number of messages in the queue         *
//****************************************************

uint8 Os_QueueGetNumberOfMessages(Os_QueueType *qp)
{
    return qp->number;
}

//****************************************************
//*queue_wait_for_message(Queue *qp)                 *
//*                                                  *
//* description: wait for new message                *
//* input: Queue *qp: Pointer to queue               *
//* output: number of new message, ZERO (error)      *
//****************************************************

uint8 Os_QueueWaitForMessage(Os_QueueType *qp)
{
    uint8 number_old = qp->number;
    //enable multitasking to avoid a deadrun
    wdt_enable(WDTO_2S);
    TIMER_ENABLE |= _BV(TOIE0);
    sei();
    if (qp->queueDepth == 0)
        return 0;
    while (qp->number == number_old)
    {
        TIMER_REGISTER = CLK1; //to avoid unecessary wait states while we are waiting for a message
        TCNT0=0xFF; //to switch to scheduler as soon as possible. scheduler will set this back to CLK1024
    }
    return qp->number;
}

//***********************************************************************
//*queue_read_byte(Queue *qp, uint8 start, uint8 delete_message)	*
//*                                                  			*
//* description:read a byte in a 1-byte-item Queue      		*
//* input:	Queue *qp: Pointer to queue, 				*
//*		uint8 position: position(Message-Number) in the Queue,*,
//*		uint8 delete_message : 1=delete message after reading *
//* output:  	Message as uint8, ZERO (error) 			*
//*                                                  			*
//*		Works only with 1-byte-item Queues!		     	*
//*                                                  			*
//***********************************************************************

uint8 Os_QueueReadByte(Os_QueueType *qp, uint8 position, uint8 delete_message)
{
    sregsys = SREG;
    cli();
    uint8 i;
    uint8 tempmsg;
    if (qp->number == 0 || qp->number <= position || qp->item_size != 1)
    {
        SREG = sregsys;
        return 0;
    }
    //read message
    tempmsg = qp->message[position];
    if (delete_message)
    {
        //re-organize message queue (FIFO)
        for (i=position;i<(qp->number)-1;i++)
            qp->message[i] = qp->message[i+1];
        qp->number--;
    }
    SREG = sregsys;
    return tempmsg;
}

//*******************************************************************************
//*queue_read_item(Queue *qp,  uint8* targetpointer, uint8 length)	*
//*                                                  				*	
//* description:read message in a queue and copy it to a target array		*
//* input:	Queue *qp: Pointer to queue,					* 
//*		uint8 targetpointer: Pointer to target array,		*
//*		uint8 position: position(Message-Number) in the Queue		*
//*		uint8 delete_message: 1=delete message after reading		*
//* output:  	ZERO (error), TRUE (ok)                   			*		
//*******************************************************************************

uint8 Os_QueueReadItem(Os_QueueType *qp, uint8* targetpointer, uint8 position, uint8 delete_message)
{
    sregsys = SREG;
    cli();
    uint8 i,j;
    position = position * qp->item_size;	// get correct message for the given entry.
    if (qp->number == 0 || qp->number <= position || targetpointer == 0)
    {
        SREG = sregsys;
        return 0;
    }
    //read messages
    for (i=position;i<(position+qp->item_size);i++)
    {
        if (delete_message)
        {
            *(targetpointer+(i-position)) = qp->message[position];
            //re-organize message queue (FIFO)
            for (j=position;j<(qp->number)-1;j++)
                qp->message[j] = qp->message[j+1];
            qp->number--;
        }
        else
            *(targetpointer+(i-position)) = qp->message[i];
    }
    SREG = sregsys;
    return TRUE;
}

//***********************************************
//*queue_read_pointer (Queue *qp) 		*
//*                                             *
//* description: read a pointer from a queue	*
//* input: Queue *qp: Pointer to queue          *
//* output:  Pointer                            *
//***********************************************

void* Os_QueueReadPointer (Os_QueueType *qp)
{
    return (void*) qp->ptr;
}

//****************************************************
//*queue_write_byte (Queue *qp, uint8 message)*
//*                                                  *
//* description: write byte as message to a queue    *
//* input: Queue *qp: Pointer to receiver queue      *
//*        uint8 message: byte value         *
//* output:  TRUE(ok), ZERO (error)                  *
//*                                                  *
//*	Works only with 1-byte-item Queues!	     *
//*                                                  *
//****************************************************

uint8 Os_QueueWriteByte (Os_QueueType *qp, uint8 message)
{
    sregsys = SREG;
    cli();
    uint8 mlen = qp->number;
    if (mlen > (qp->queueDepth - 1) || qp->item_size != 1)
    {
        SREG = sregsys;
        return 0;
    }
    //insert message
    qp->message[mlen]=message;
    qp->number++;
    SREG = sregsys;
    return TRUE;
}

//*******************************************************************
//*queue_write_item (Queue *qp, void* startpointer)		    *
//*                                                                 *
//* description: write word/array as message to queue               *
//* input: Queue *qp: Pointer to receiver queue                     *
//*        void* startpointer: pointer to begin of word/array       *
//*                                                                 *
//* Remember: word/array size must be item size of the queue!       *
//*                                                                 *
//* output:  TRUE(ok), ZERO (error)                                 *
//*******************************************************************

uint8 Os_QueueWriteItem (Os_QueueType *qp, void* startpointer)
{
    sregsys = SREG;
    cli();
    uint8 mlen = qp->number,i=0;
    uint8 length = qp->item_size;
    if (mlen + length > qp->queueDepth)
    {
        SREG = sregsys;
        return 0;
    }
    //insert message as 1-byte chunks
    for (i=0;i<length;i++)
    {
        qp->message[mlen+i]=*((uint8*)startpointer++);
        qp->number++;
    }
    SREG = sregsys;
    return TRUE;
}

//*******************************************************
//*queue_write_pointer (Queue *qp, void* pointer) 	*
//*                                                     *
//* description: write pointer to a queue               *
//* input: Queue *qp: Pointer to receiver queue         *
//*        void* Pointer: pointer to begin of word      *
//* output:  TRUE(ok), ZERO (error)                     *
//*******************************************************

uint8 Os_QueueWritePointer (Os_QueueType *qp, void* pointer)
{
    sregsys = SREG;
    cli();
    if (pointer == 0)
    {
        SREG = sregsys;
        return 0;
    }
    //insert pointer
    qp->ptr = pointer;
    SREG = sregsys;
    return TRUE;
}
#endif
