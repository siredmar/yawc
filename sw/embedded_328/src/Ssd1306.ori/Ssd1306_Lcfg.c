/*
 * Ssd1306_lcfg.c
 *
 *  Created on: Nov 10, 2012
 *      Author: armin
 */
#include "../Ssd1306.ori/Ssd1306_Lcfg.h"

#include "../Ssd1306.ori/Ssd1306.h"



const Ssd1306_InitSequenceType Ssd1306_InitialConfigOled_s[] =
{
        // Turn display off
        {SSD1306_DISPLAYOFF, 0x00, 0U, NOPARAM},
        {SSD1306_SETDISPLAYCLOCKDIV, 0x80, 0U, PARAM},
        {SSD1306_SETMULTIPLEX, 0x3F, 0U, PARAM},
        {SSD1306_SETDISPLAYOFFSET, 0x00, 0U, PARAM},
        {SSD1306_SETSTARTLINE | 0x00, 0x00, 0U, NOPARAM},
        // We use internal charge pump

        {SSD1306_CHARGEPUMP, 0x14, 0U, PARAM},
        // Horizontal memory mode

        {SSD1306_MEMORYMODE, 0x00, 0U, PARAM},

        {SSD1306_SEGREMAP | 0x1, 0x00, 0U, NOPARAM},
        {SSD1306_COMSCANDEC, 0x00, 0U, NOPARAM},
        {SSD1306_SETCOMPINS, 0x12, 0U, PARAM},

        // Max contrast
        {SSD1306_SETCONTRAST, 0xCF, 0U, PARAM},

        {SSD1306_SETPRECHARGE, 0xF1, 0U, PARAM},

        {SSD1306_SETVCOMDETECT, 0x40, 0U, PARAM},

        {SSD1306_DISPLAYALLON_RESUME, 0x00, 0U, NOPARAM},

        // Non-inverted display
        {SSD1306_NORMALDISPLAY, 0x00, 0U, NOPARAM},

        // Turn display back on
        {SSD1306_DISPLAYON, 0x00, 0U, NOPARAM}
};

const Ssd1306_ConfigType Ssd1306_InitialConfigTft_s =
{
   Ssd1306_InitialConfigOled_s,		/* config struct for tft */
   16U 							    /* number of config entries is 44 */
};

const void *Ssd1306_GetLcfgData(void)
{
   return ((void*) &Ssd1306_InitialConfigTft_s);
}

