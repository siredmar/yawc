/*---------------------------------------------------------------------
 * memory.c
 *
 * Copyright (c) 2010 Tobias Wiese - hacker-tobi@gmx.net
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *------------------------------------------------------------------------
 */

#include "Os.h"

#ifdef TASK_USE_MEM
//****************************************************
//* memory_get_free_blocks()                         *
//*                                                  *
//* description: check if memory is available        *
//* input: no                                        *
//* output: number of free memory blocks             *
//****************************************************

uint8 Os_MemoryGetFreeBlocks()
{
    return MAXBLOCKS - activeBlocks;
}

//****************************************************
//* memory_allocate_block(uint8 size)              *
//*                                                  *
//* description: allocate memory                     *
//* input: none		                             *
//* output: pointer to memory block or 0 (error)     *
//****************************************************

void* Os_MemoryAllocateBlock()
{
    sregsys = SREG;
    cli();
    if (activeBlocks >= MAXBLOCKS)
    {
        SREG = sregsys;
        return 0;
    }
    uint8 a;
    char* ptr;
    Os_MemBlockIntern* block = (blocks+activeBlocks);
    //insert block
    //set a pointer to the memory block
    block->ptr = data+(activeBlocks*BLOCKSIZE);
    ptr = (char*) block->ptr;
    //leave if theres not enough memory
    if (block->ptr == 0 || (uint16_t) block->ptr > HEAPSTART)
    {
        SREG = sregsys;
        return 0;
    }
    //clear memory
    for (a=0;a<BLOCKSIZE;a++)
        *(ptr+a) = 0;
    //owner and tasknumber
    block->owner = Os_ActiveTaskId;
    activeBlocks++;
    SREG = sregsys;
    //return pointer to memory block
    return (void*) block->ptr;
}

//****************************************************
//* free_memory_block(void* ptr)                     *
//*                                                  *
//* description: free allocated memory               *
//* input: pointer to allocated memory block         *
//* output: TRUE (ok) or ZERO (error)                *
//****************************************************

uint8 Os_MemoryFreeBlock(void* ptr)
{
    sregsys = SREG;
    cli();
    uint8 a,b;
    //search for correct memory block
    for (a=0;a<activeBlocks;a++)
    {
        if ((blocks+a)->ptr == ptr)
        {
            if ((blocks+a)->owner != Os_ActiveTaskId)
            {
                SREG = sregsys;
                return 0;
            }
            //if block is leased by parent task release it.
            //no reason to clear it here, because theres a clearing in memory_allocate_block()
            (blocks+a)->ptr=0;
            (blocks+a)->owner=0;
            //re-organize memory blocks
            for (b=a;b<(activeBlocks-1);b++)
                blocks[b]=blocks[b+1];
            activeBlocks--;
            SREG = sregsys;
            return TRUE;
        }
    }
    SREG = sregsys;
    return 0;
}
#endif

