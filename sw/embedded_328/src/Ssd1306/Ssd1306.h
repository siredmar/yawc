/**
 * \file display.h
 * \brief This file contains the interface of the display module.
 *
 * Copyright (C) 2011  Armin Schlegel, Christian Eismann
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

/*--- Includes -------------------------------------------------------*/

#include <stdio.h>
#include <inttypes.h>

/* F_CPU has to be defined before including util/delay.h */
#include <util/delay.h>
#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "Spi.h"
#include "Tools.h"


/*--- Macros ---------------------------------------------------------*/
#define SSD1306_DC_PIN                      (GPIO_CHANNEL_PC0)
#define SSD1306_CS_PIN                      (GPIO_CHANNEL_PC1)


#define PAGES (7u)
#define LINES (8u)
#define COLUMS (128u)


#define SSD1306_DEFAULT_ADDRESS     ((uint8)0x78)
#define SSD1306_SETCONTRAST         ((uint8)0x81)
#define SSD1306_DISPLAYALLON_RESUME ((uint8)0xA4)
#define SSD1306_DISPLAYALLON        ((uint8)0xA5)
#define SSD1306_NORMALDISPLAY       ((uint8)0xA6)
#define SSD1306_INVERTDISPLAY       ((uint8)0xA7)
#define SSD1306_DISPLAYOFF          ((uint8)0xAE)
#define SSD1306_DISPLAYON           ((uint8)0xAF)
#define SSD1306_SETDISPLAYOFFSET    ((uint8)0xD3)
#define SSD1306_SETCOMPINS          ((uint8)0xDA)
#define SSD1306_SETVCOMDETECT       ((uint8)0xDB)
#define SSD1306_SETDISPLAYCLOCKDIV  ((uint8)0xD5)
#define SSD1306_SETPRECHARGE        ((uint8)0xD9)
#define SSD1306_SETMULTIPLEX        ((uint8)0xA8)
#define SSD1306_SETLOWCOLUMN        ((uint8)0x00)
#define SSD1306_SETHIGHCOLUMN       ((uint8)0x10)
#define SSD1306_SETSTARTLINE        ((uint8)0x40)
#define SSD1306_MEMORYMODE          ((uint8)0x20)
#define SSD1306_COLUMNADDR          ((uint8)0x21)
#define SSD1306_PAGEADDR            ((uint8)0x22)
#define SSD1306_COMSCANINC          ((uint8)0xC0)
#define SSD1306_COMSCANDEC          ((uint8)0xC8)
#define SSD1306_SEGREMAP            ((uint8)0xA0)
#define SSD1306_CHARGEPUMP          ((uint8)0x8D)
#define SSD1306_SWITCHCAPVCC        ((uint8)0x2)
#define SSD1306_NOP                 ((uint8)0xE3)

#define DISP_WIDTH   (128u)
#define DISP_HEIGHT  (64u)
#define PIXEL_ON (1u)
#define PIXEL_OFF (0u)
#define FONT_WIDTH   (5u)
#define FONT_HEIGHT  (7u)

/*--- External Function Prototypes -----------------------------------*/

void Ssd1306_Init(void);
void Ssd1306_ClearScreen(void);
void Ssd1306_ClearRam(void);
void Ssd1306_PutPixel(uint8 x, uint8 y, int pixel_status);
void Ssd1306_SendFrame(void);
void Ssd1306_WriteString(uint16 x, uint16 y, const sint8 *str);
void Ssd1306_DrawBmpFromFlash(uint8 x, uint8 y, uint8 *addr);
void Ssd1306_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void Ssd1306_DrawHorizontalLine(uint8_t x0, uint8_t y0, uint8_t length, uint8_t pixel_status);

#endif /* #ifndef _DISPLAY_H_ */

