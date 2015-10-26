/**
 * \file display.c
 * \brief This file contains the implementation of the display module.
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

/*--- Includes -------------------------------------------------------*/

#include "Ssd1306.h"

#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include "Dbg.h"
#include "Uart.h"


static void Ssd1306_CsSelect(void);
static void Ssd1306_CsDeselect(void);
static void Ssd1306_SendCommandMode(void);
static void Ssd1306_DataMode(void);
static void Ssd1306_SpiSend(uint8 data_ui8);

static void Ssd1306_SendData(uint8 cData);
static void Ssd1306_SendCommand(uint8 cData);
//static void Ssd1306_GoTo(uint8 col, uint8 page);
static void Ssd1306_DrawChar5x7(uint16 x, uint16 y, sint8 character);

/*--- Macros ---------------------------------------------------------*/

/** display RAM size */
#define DISPLAY_RAM_SIZE 1024U

/*--- Internal Constants ---------------------------------------------*/

/** The byte code array for the used font */
static const uint8 Font5x7[] PROGMEM = {
        0x00, 0x00, 0x00, 0x00, 0x00,   // (space)
        0x00, 0x00, 0x5F, 0x00, 0x00,   // !
        0x00, 0x07, 0x00, 0x07, 0x00,   // "
        0x14, 0x7F, 0x14, 0x7F, 0x14,   // #
        0x24, 0x2A, 0x7F, 0x2A, 0x12,   // $
        0x23, 0x13, 0x08, 0x64, 0x62,   // %
        0x36, 0x49, 0x55, 0x22, 0x50,   // &
        0x00, 0x05, 0x03, 0x00, 0x00,   // '
        0x00, 0x1C, 0x22, 0x41, 0x00,   // (
        0x00, 0x41, 0x22, 0x1C, 0x00,   // )
        0x08, 0x2A, 0x1C, 0x2A, 0x08,   // *
        0x08, 0x08, 0x3E, 0x08, 0x08,   // +
        0x00, 0x50, 0x30, 0x00, 0x00,   // ,
        0x08, 0x08, 0x08, 0x08, 0x08,   // -
        0x00, 0x60, 0x60, 0x00, 0x00,   // .
        0x20, 0x10, 0x08, 0x04, 0x02,   // /
        0x3E, 0x51, 0x49, 0x45, 0x3E,   // 0
        0x00, 0x42, 0x7F, 0x40, 0x00,   // 1
        0x42, 0x61, 0x51, 0x49, 0x46,   // 2
        0x21, 0x41, 0x45, 0x4B, 0x31,   // 3
        0x18, 0x14, 0x12, 0x7F, 0x10,   // 4
        0x27, 0x45, 0x45, 0x45, 0x39,   // 5
        0x3C, 0x4A, 0x49, 0x49, 0x30,   // 6
        0x01, 0x71, 0x09, 0x05, 0x03,   // 7
        0x36, 0x49, 0x49, 0x49, 0x36,   // 8
        0x06, 0x49, 0x49, 0x29, 0x1E,   // 9
        0x00, 0x36, 0x36, 0x00, 0x00,   // :
        0x00, 0x56, 0x36, 0x00, 0x00,   // ;
        0x00, 0x08, 0x14, 0x22, 0x41,   // <
        0x14, 0x14, 0x14, 0x14, 0x14,   // =
        0x41, 0x22, 0x14, 0x08, 0x00,   // >
        0x02, 0x01, 0x51, 0x09, 0x06,   // ?
        0x32, 0x49, 0x79, 0x41, 0x3E,   // @
        0x7E, 0x11, 0x11, 0x11, 0x7E,   // A
        0x7F, 0x49, 0x49, 0x49, 0x36,   // B
        0x3E, 0x41, 0x41, 0x41, 0x22,   // C
        0x7F, 0x41, 0x41, 0x22, 0x1C,   // D
        0x7F, 0x49, 0x49, 0x49, 0x41,   // E
        0x7F, 0x09, 0x09, 0x01, 0x01,   // F
        0x3E, 0x41, 0x41, 0x51, 0x32,   // G
        0x7F, 0x08, 0x08, 0x08, 0x7F,   // H
        0x00, 0x41, 0x7F, 0x41, 0x00,   // I
        0x20, 0x40, 0x41, 0x3F, 0x01,   // J
        0x7F, 0x08, 0x14, 0x22, 0x41,   // K
        0x7F, 0x40, 0x40, 0x40, 0x40,   // L
        0x7F, 0x02, 0x04, 0x02, 0x7F,   // M
        0x7F, 0x04, 0x08, 0x10, 0x7F,   // N
        0x3E, 0x41, 0x41, 0x41, 0x3E,   // O
        0x7F, 0x09, 0x09, 0x09, 0x06,   // P
        0x3E, 0x41, 0x51, 0x21, 0x5E,   // Q
        0x7F, 0x09, 0x19, 0x29, 0x46,   // R
        0x46, 0x49, 0x49, 0x49, 0x31,   // S
        0x01, 0x01, 0x7F, 0x01, 0x01,   // T
        0x3F, 0x40, 0x40, 0x40, 0x3F,   // U
        0x1F, 0x20, 0x40, 0x20, 0x1F,   // V
        0x7F, 0x20, 0x18, 0x20, 0x7F,   // W
        0x63, 0x14, 0x08, 0x14, 0x63,   // X
        0x03, 0x04, 0x78, 0x04, 0x03,   // Y
        0x61, 0x51, 0x49, 0x45, 0x43,   // Z
        0x00, 0x00, 0x7F, 0x41, 0x41,   // [
        0x02, 0x04, 0x08, 0x10, 0x20,   // "\"
        0x41, 0x41, 0x7F, 0x00, 0x00,   // ]
        0x04, 0x02, 0x01, 0x02, 0x04,   // ^
        0x40, 0x40, 0x40, 0x40, 0x40,   // _
        0x00, 0x01, 0x02, 0x04, 0x00,   // `
        0x20, 0x54, 0x54, 0x54, 0x78,   // a
        0x7F, 0x48, 0x44, 0x44, 0x38,   // b
        0x38, 0x44, 0x44, 0x44, 0x20,   // c
        0x38, 0x44, 0x44, 0x48, 0x7F,   // d
        0x38, 0x54, 0x54, 0x54, 0x18,   // e
        0x08, 0x7E, 0x09, 0x01, 0x02,   // f
        0x08, 0x14, 0x54, 0x54, 0x3C,   // g
        0x7F, 0x08, 0x04, 0x04, 0x78,   // h
        0x00, 0x44, 0x7D, 0x40, 0x00,   // i
        0x20, 0x40, 0x44, 0x3D, 0x00,   // j
        0x00, 0x7F, 0x10, 0x28, 0x44,   // k
        0x00, 0x41, 0x7F, 0x40, 0x00,   // l
        0x7C, 0x04, 0x18, 0x04, 0x78,   // m
        0x7C, 0x08, 0x04, 0x04, 0x78,   // n
        0x38, 0x44, 0x44, 0x44, 0x38,   // o
        0x7C, 0x14, 0x14, 0x14, 0x08,   // p
        0x08, 0x14, 0x14, 0x18, 0x7C,   // q
        0x7C, 0x08, 0x04, 0x04, 0x08,   // r
        0x48, 0x54, 0x54, 0x54, 0x20,   // s
        0x04, 0x3F, 0x44, 0x40, 0x20,   // t
        0x3C, 0x40, 0x40, 0x20, 0x7C,   // u
        0x1C, 0x20, 0x40, 0x20, 0x1C,   // v
        0x3C, 0x40, 0x30, 0x40, 0x3C,   // w
        0x44, 0x28, 0x10, 0x28, 0x44,   // x
        0x0C, 0x50, 0x50, 0x50, 0x3C,   // y
        0x44, 0x64, 0x54, 0x4C, 0x44,   // z
        0x00, 0x08, 0x36, 0x41, 0x00,   // {
        0x00, 0x00, 0x7F, 0x00, 0x00,   // |
        0x00, 0x41, 0x36, 0x08, 0x00,   // }
        0x08, 0x08, 0x2A, 0x1C, 0x08,   // ->
        0x08, 0x1C, 0x2A, 0x08, 0x08   // <-
};

/*--- Internal Data --------------------------------------------------*/

static uint8 disp_ram[1024];

/*--- External Function Definitions ----------------------------------*/
/**
 * \brief Deselect the chip select pin for the graphic display.
 */
static inline void Ssd1306_CsDeselect(void)
{
    Gpio_WriteChannel(SSD1306_CS_PIN, STD_HIGH);
}

/**
 * \brief Activate the chip select pin for the graphic display.
 */
static inline void Ssd1306_CsSelect(void)
{
    Gpio_WriteChannel(SSD1306_CS_PIN, STD_LOW);
}

/**
 * \brief Set the display SPI communication to data mode.
 */
static inline void Ssd1306_DataMode(void)
{
    Gpio_WriteChannel(SSD1306_DC_PIN, STD_HIGH);
}

/**
 * \brief Set the display SPI communication to command mode.
 */
static inline void Ssd1306_SendCommandMode(void)
{
    Gpio_WriteChannel(SSD1306_DC_PIN, STD_LOW);
}

static void Ssd1306_SpiSend(uint8 data_ui8)
{
    Spi_SendData(data_ui8, SSD1306_CS_PIN, SPI_CS_ACTIVE_LOW, 1, 1);
}


/**
 * \brief Sends a 8 bit command to the display controller.
 */
static void Ssd1306_SendCommand(uint8 cData)
{
    Ssd1306_CsSelect();
    Dbg_ReadRegister(UART_HWUNIT_0, "Ssd1306_Init a ", 0x00);
    Ssd1306_SendCommandMode();
    Dbg_ReadRegister(UART_HWUNIT_0, "Ssd1306_Init b ", 0x00);
    Ssd1306_SpiSend(cData);
    Dbg_ReadRegister(UART_HWUNIT_0, "Ssd1306_Init c ", 0x00);
    Ssd1306_CsDeselect();
    Dbg_ReadRegister(UART_HWUNIT_0, "Ssd1306_Init d ", 0x00);
}

/**
 * \brief Sends a 8 bit data block the display controller.
 */
static void Ssd1306_SendData(uint8 cData)
{
    Ssd1306_CsSelect();
    Ssd1306_DataMode();
    Ssd1306_SpiSend(cData);
    Ssd1306_CsDeselect();
}

/**
 * \brief Initializes the graphical display
 *
 * Within this function the EA DOGL-128-6 is initialized for single power
 * supply +3.3V
 */
void Ssd1306_Init(void)
{
    Dbg_ReadRegister(UART_HWUNIT_0, "Ssd1306_Init 1 ", 0x00);

    //Ssd1306_SendCommand(SSD1306_DISPLAYOFF);                    // 0xAE
    Dbg_ReadRegister(UART_HWUNIT_0, "Ssd1306_Init 2 ", 0x00);
    Ssd1306_SendCommand(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
    Ssd1306_SendCommand(0x80);                                  // the suggested ratio 0x80
    Ssd1306_SendCommand(SSD1306_SETMULTIPLEX);                  // 0xA8
    Ssd1306_SendCommand(0x3F);
    Ssd1306_SendCommand(SSD1306_SETDISPLAYOFFSET);              // 0xD3
    Ssd1306_SendCommand(0x0);                                   // no offset
    Ssd1306_SendCommand(SSD1306_SETSTARTLINE | 0x0);            // line #0
    Ssd1306_SendCommand(SSD1306_CHARGEPUMP);                    // 0x8D
    //    if (vccstate == SSD1306_EXTERNALVCC)
    //    {
    //        Ssd1306_SendCommand(0x10);
    //      }
    //    else
    //    {
    Ssd1306_SendCommand(0x14);
    //    }
    Ssd1306_SendCommand(SSD1306_MEMORYMODE);                    // 0x20
    Ssd1306_SendCommand(0x00);                                  // 0x0 act like ks0108
    Ssd1306_SendCommand(SSD1306_SEGREMAP | 0x1);
    Ssd1306_SendCommand(SSD1306_COMSCANDEC);
    Ssd1306_SendCommand(SSD1306_SETCOMPINS);                    // 0xDA
    Ssd1306_SendCommand(0x12);
    Ssd1306_SendCommand(SSD1306_SETCONTRAST);                   // 0x81
    //    if (vccstate == SSD1306_EXTERNALVCC)
    //    {
    //        Ssd1306_SendCommand(0x9F);
    //    }
    //    else
    //    {
    Ssd1306_SendCommand(0xCF);
    //    }
    Ssd1306_SendCommand(SSD1306_SETPRECHARGE);                  // 0xd9
    //    if (vccstate == SSD1306_EXTERNALVCC)
    //    {
    //        Ssd1306_SendCommand(0x22);
    //    }
    //    else
    //    {
    Ssd1306_SendCommand(0xF1);
    //    }
    Ssd1306_SendCommand(SSD1306_SETVCOMDETECT);                 // 0xDB
    Ssd1306_SendCommand(0x40);
    Ssd1306_SendCommand(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
    Ssd1306_SendCommand(SSD1306_NORMALDISPLAY);                 // 0xA6
}

/**
 * \brief Send a new frame to th ST-7565R display RAM
 *
 * A frame consists of 8 Pages. Each page consists of 8x128bit storage.
 * \code
 *    0                                             127
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-...-+-+-+
 * D0 | | | | | | | | | | | | | | | | | | | ... | | |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-...-+-+-+
 * D1 | | | | | | | | | | | | | | | | | | | ... | | |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-...-+-+-+
 * D2 | | | | | | | | | | | | | | | | | | | ... | | |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-...-+-+-+
 * D3 | | | | | | | | | | | | | | | | | | | ... | | |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-...-+-+-+
 * D4 | | | | | | | | | | | | | | | | | | | ... | | |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-...-+-+-+
 * D5 | | | | | | | | | | | | | | | | | | | ... | | |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-...-+-+-+
 * D6 | | | | | | | | | | | | | | | | | | | ... | | |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-...-+-+-+
 * D7 | | | | | | | | | | | | | | | | | | | ... | | |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-...-+-+-+
 * \endcode
 * \note The ST-7565R display controller provides a 64 * 132(!) bit RAM. But the
 *       actual display resolution of the EA DOGL128-6 is 128x64 pixel. Therefore
 *       only 128 columns are used within the internal RAM of the display controller.
 *
 * The virtual data representation is realized in linear order here.
 * \code
 *    |- disp_ram[0]                disp_ram[1023] -|
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-...-+-+-+
 *    | | | | | | | | | | | | | | | | | | | ... | | |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-...-+-+-+
 * \endcode
 * To avoid a waste of memory the data structure is organized as follows:
 * ARRAY_ELEMENT = 8bit * COLUMN + PAGE --> maximum array size of 1024 elements
 *
 * Every 8 bit value stored in disp_ram array represents a single column of one
 * page.
 */
void Ssd1306_SendFrame()
{
    uint8 page;
    uint8 column;

    Ssd1306_SendCommand(SSD1306_COLUMNADDR);
    Ssd1306_SendCommand(0);
    Ssd1306_SendCommand(DISP_WIDTH - 1);

    Ssd1306_SendCommand(SSD1306_PAGEADDR);
    Ssd1306_SendCommand(0);
    Ssd1306_SendCommand(7);

    for (page = 0U; page < 8U; page++)
    {
        for (column = 0U; column < 128U; column++)
        {
            Ssd1306_SendData(disp_ram[page + (column << 3U)]);
        }
    }
}

/**
 * \brief Set/Unset a single pixel on the display
 *
 * For choosing the correct entry within the data structure (disp_ram[])
 * first the concerning page of this pixel has to be determined. This
 * is done by dividing the Y coordinate by 8 (or better: do a right shift
 * of 3 bits). For choosing the entry in the array, the X coordinate multiplied
 * by 8 (or better: left shifted by 3) has to be added to the actual page number.
 * The exact bit that shall be set/unset is determined by using the bitmask
 * (y & 0x07). This selects the exact row of the respective memory page.
 *
 * \param x  X coordinate of the pixel
 * \param y  Y coordinate of the pixel
 */
void Ssd1306_PutPixel(uint8 x, uint8 y, int pixel_status)
{
    if (x < DISP_WIDTH && y < DISP_HEIGHT) {
        if (pixel_status == PIXEL_ON)
            disp_ram[(y >> 3) + (x << 3)] |= (1 << (y & 0x07));
        else
            disp_ram[(y >> 3) + (x << 3)] &= ~(1 << (y & 0x07));
    }
}

/**
 * \brief Turn every pixel off
 */
void Ssd1306_ClearRam(void)
{
    /* clears the local RAM but don't send it */
    uint16_t d;
    for (d = 0U; d < DISPLAY_RAM_SIZE; d++) {
        disp_ram[d] = 0x00;
    }
}

/**
 * \brief Clear the graphical display screen
 */
void Ssd1306_ClearScreen(void)
{
    Ssd1306_ClearRam();
    Ssd1306_SendFrame();
}
//
///**
// * \brief Positioning of the cursor at the provided column and page
// *
// * \param col the column at which the curser shall be positioned
// * \param page the RAM page at which the cursor shall be positioned
// */
//static void Ssd1306_GoTo(uint8 col, uint8 page)
//{
//   Ssd1306_SendCommand(DISPLAY_PAGE_ADDRESS);
//   Ssd1306_SendCommand(0);
//   Ssd1306_SendCommand();
//
//   Ssd1306_SendCommand(DISPLAY_COL_ADDRESS);
//   Ssd1306_SendCommand(col);
//   return;
//}

/**
 * \brief Draw a provided character at a specific position on the graphical display
 *
 * \param x the x coordinate where the character shall be placed
 * \param y the y coordinate where the character shall be placed
 * \param character the actual character that shall be printed
 */
static void Ssd1306_DrawChar5x7(uint16 x, uint16 y, sint8 character)
{
    uint8 fontIndex, i, j;
    const uint8 *addr = Font5x7;
    /* The 5x7 character set starts at the '!' character (ASCII index
     * number 33) so we subtract 32 from the ASCII character to find the
     * index in the 5x7 font table. */
    fontIndex = character - 32;
    /* If the index is out of bounds, bail out */
    if (fontIndex > 94)
        return;

    for (i = 0; i < FONT_WIDTH; i++) {
        for (j = 0; j < FONT_HEIGHT; j++) {
            /* Check if the bit/pixel is set, paint accoringly to
             * the screen */
            if (pgm_read_byte(addr + FONT_WIDTH * fontIndex + i) & (1 << j))
                Ssd1306_PutPixel(x, y + j, PIXEL_ON);
            else
                Ssd1306_PutPixel(x, y + j, PIXEL_OFF);
        }
        /* Move the LCD cursor through the font width as well */
        x++;
    }
}

/**
 * \brief Draw an entire string at a specific position on the graphical display
 *
 * \param x the x coordinate where the string shall be placed
 * \param y the y coordinate where the string shall be placed
 * \param str the pointer to the string that shall be printed, terminated by
 *            '\0'
 */
void Ssd1306_WriteString(uint16 x, uint16 y, const sint8* str)
{
    uint16 origin_X;

    origin_X = x;      //alte X Position beibehalten

    /* Continue through the string until we encounter a null character */

    while (*str != '\0') {
        /* If the character is a newline, then prepare our x and y
         * coordinates for the next character on the new line. */
        if (*str == '\n') {
            /* Reset x to its origin */
            x = origin_X;

            /* Move y one character down */
            y += FONT_HEIGHT + 1;

            str++;
            continue;
        }

        Ssd1306_DrawChar5x7(x, y, *str++);

        /* Add a 1-pixel spacing between the characters */
        x += FONT_WIDTH + 1;

        /* In case we are putting this character out of bounds,
         * move the character to the next line on the display */
        if ((x + FONT_WIDTH) > DISP_WIDTH) {
            /* Reset x to its origin */
            x = origin_X;

            /* Move y one character down */
            y += FONT_HEIGHT + 1;
        }
    }
}

/**
 * \brief Draw a bitmap picture to the graphical display.
 * \note The picture has to be stored within the interal EEPROM!
 * \param x the x coordinate where the picture shall be placed (upper left corner)
 * \param y the y coordinate where the string shall be placed (upper left corner)
 * \param str the address where the picture is stored (in internal EEPROM)
 */
void Ssd1306_DrawBmpFromFlash(uint8 x, uint8 y, uint8 *addr)
{
    uint8 width = pgm_read_byte(addr++);
    uint8 heigth = pgm_read_byte(addr++);
    uint16_t bitcnt;
    uint8 _x = 0;
    uint8 _y = 0;

    for (bitcnt = 0; bitcnt < (uint16_t) (heigth * width); bitcnt++) {
        if ((bitcnt % (width)) == 0) {
            _y++;
            _x = 0;
        } else {
            _x++;
        }
        if (pgm_read_byte(addr + bitcnt / 8) & (1 << (bitcnt % 8)))
            Ssd1306_PutPixel(_x + x, _y + y, PIXEL_ON);
        else
            Ssd1306_PutPixel(_x + x, _y + y, PIXEL_OFF);
    }
}

/**
 * \brief Print a line on the graphical display.
 *
 * \param x0 the starting point x-coordinate
 * \param y0 the starting point y-coordinate
 * \param x1 the ending point x-coordinate
 * \param y1 the ending point y-coordinate
 */
void Ssd1306_DrawLine(uint8 x0, uint8 y0, uint8 x1, uint8 y1)
{
    int8_t dy = y1 - y0;
    int8_t dx = x1 - x0;
    int8_t stepx, stepy;

    if (dy < 0) {
        dy = -dy;
        stepy = -1;
    } else {
        stepy = 1;
    }
    if (dx < 0) {
        dx = -dx;
        stepx = -1;
    } else {
        stepx = 1;
    }
    dy <<= 1;
    dx <<= 1;

    Ssd1306_PutPixel(x0, y0, 1);

    if (dx > dy) {
        int fraction = dy - (dx >> 1);

        while (x0 != x1) {
            if (fraction >= 0) {
                y0 += stepy;
                fraction -= dx;
            }
            x0 += stepx;
            fraction += dy;
            Ssd1306_PutPixel(x0, y0, PIXEL_ON);
        }
    } else {
        int fraction = dx - (dy >> PIXEL_ON);
        while (y0 != y1) {
            if (fraction >= 0) {
                x0 += stepx;
                fraction -= dy;
            }
            y0 += stepy;
            fraction += dx;
            Ssd1306_PutPixel(x0, y0, PIXEL_ON);
        }
    }
}

/**
 * \brief Print a horizontal line on the graphical display.
 *
 * \param x0 the starting point x-coordinate
 * \param y0 the starting point y-coordinate
 * \param length the length of the horizontal line
 * \param pixel_status defines whether the line shall be printed by enabling
 *                     (PIXEL_ON) or disabling (PIXEL_OFF) single pixels
 */
void Ssd1306_DrawHorizontalLine(uint8 x0, uint8 y0, uint8 length, uint8 pixel_status)
{
    uint8 cnt;

    for (cnt = x0; cnt <= length + x0; cnt++)
        Ssd1306_PutPixel(cnt, y0, pixel_status);
}

