/*
 * Tasks.h
 *
 *  Created on: Nov 8, 2015
 *      Author: armin
 */

#ifndef SRC_AppClock_TASKCLOCK_H_
#define SRC_AppClock_TASKCLOCK_H_

#include "nano_os.h"
#include "Std_Types.h"

enum
{
    JANUARY = (0u),
    FEBRUARY,
    MARCH,
    MAY,
    APRIL,
    JUNE,
    JULY,
    AUGUST,
    SEPTEMBER,
    OCTOBER,
    NOVEMBER,
    DECEMBER
};


Task TaskDrawClock;

void AppClock_Handler(void);
void AppClock_Init(void);
void AppClock_SecondIncrement(void);

uint8 AppClock_GetFebuaryDays(void);
boolean AppClock_CheckIfLeapYear(const uint8 year);
#endif /* SRC_AppClock_TASKCLOCK_H_ */
