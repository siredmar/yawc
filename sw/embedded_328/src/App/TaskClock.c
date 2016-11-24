/*
 * Tasks.c
 *
 *  Created on: Nov 8, 2015
 *      Author: armin
 */

#include "TaskClock.h"
#include "u8g.h"
#include "Uart.h"

static uint8 daysOfMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static void AppClock_DrawClock(uint8 sec, uint8 min, uint8 hour, uint8 day, uint8 month, uint8 year);
static void AppClock_ClearScreen(void);

u8g_t u8g;
volatile uint8 second = 50;
volatile uint8 minute = 59;
volatile uint8 hour = 23;
volatile uint8 day = 31;
volatile uint8 month = 12;
volatile uint8 year = 15;
volatile uint8 AppClock_ClockChanged_ui8;
//volatile uint8 AppClock_DisplayUpdatePossible_ui8;

static void AppClock_DrawClock(uint8 sec, uint8 min, uint8 hour, uint8 day, uint8 month, uint8 year)
{
    uint8 str[9];

    u8g_DrawFrame(&u8g, 15, 5, 99, 50);

    u8g_SetFont(&u8g, u8g_font_helvR18);
    sprintf(str, "%.2d:%.2d:%.2d", hour, min, sec);
    u8g_DrawStr(&u8g, 20, 30, str);

    u8g_SetFont(&u8g, u8g_font_helvR14);
    sprintf(str, "%.2d.%.2d.%.2d", day, month, year);
    u8g_DrawStr(&u8g, 30, 50, str);
}

static void AppClock_ClearScreen(void)
{
    u8g_FirstPage(&u8g);
    do
    {
    } while ( u8g_NextPage(&u8g) );
}

void AppClock_Init(void)
{
    AppClock_ClearScreen();
    AppClock_ClockChanged_ui8 = TRUE;
//    AppClock_DisplayUpdatePossible_ui8 = TRUE;
    AppClock_CheckIfLeapYear(year);
}

void AppClock_SecondIncrement(void)
{
    second++;
    Uart_WriteString(UART_HWUNIT_0, "II: Second Increment");
    AppClock_ClockChanged_ui8 = TRUE;
    if(second > 59)
    {
        second = 0;
        minute++;
    }
    if(minute > 59)
    {
        minute = 0;
        hour++;
    }
    if(hour > 23)
    {
        hour = 0;
        day++;
    }
    if (day > daysOfMonth[month - 1])
    {
        day = 1;
        month++;
    }
    if (month > 12)
    {
        month = 1;
        year++;
        AppClock_CheckIfLeapYear(year);
    }
}



void AppClock_Handler(void)
{
    while (1)
    {
        if(AppClock_ClockChanged_ui8 == TRUE)
        {
            AppClock_ClockChanged_ui8 = FALSE;
//            if(AppClock_DisplayUpdatePossible_ui8 == TRUE)
//            {
//                AppClock_DisplayUpdatePossible_ui8 = FALSE;
                Uart_WriteString(UART_HWUNIT_0, "II: Display update\n\r");
                u8g_FirstPage(&u8g);
                do
                {
                    AppClock_DrawClock(second, minute, hour, day, month, year);
                } while ( u8g_NextPage(&u8g) );
//                AppClock_DisplayUpdatePossible_ui8 = TRUE;
//            }
        }
    }
}


uint8 AppClock_GetFebuaryDays(void)
{
    uint16 longYear = 2000 + year;

    if((longYear % 400 == 0) || (longYear % 4 == 0 && longYear % 100 != 0))
    {
        return 29;
    }
    else
    {
        return 28;
    }
}

boolean AppClock_CheckIfLeapYear(const uint8 year)
{
    uint16 longYear = 2000 + year;

    if((longYear % 400 == 0) || (longYear % 4 == 0 && longYear % 100 != 0))
    {
        daysOfMonth[FEBRUARY] = 29;
        return TRUE;
    }
    else
    {
        daysOfMonth[FEBRUARY] = 28;
        return FALSE;
    }
}


