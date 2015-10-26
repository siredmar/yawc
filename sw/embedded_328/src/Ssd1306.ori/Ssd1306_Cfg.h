/**
*  COPYRIGHT: Armin Schlegel
*  \file tft.h
*  \brief Display Graphic Driver
*
*  This header file contains all tft specific datatypes and defines.
*
*******************************************************************************/
#ifndef TFT_CFG_H
#define TFT_CFG_H
/* ***************************** includes < > ******************************* */

/* ***************************** includes " " ******************************* */
#include "Gpio.h"
/* ***************************** typedefs *********************************** */
/* ***************************** defines ************************************ */
#define ROTATE 0

#if !ROTATE
   #define TFT_WIDTH_UI16                   ((uint16)128)
   #define TFT_HEIGHT_UI16                  ((uint16)64U)
#else
   #define TFT_WIDTH_UI16                   ((uint16)64)
   #define TFT_HEIGHT_UI16                  ((uint16)128U)
#endif
#define TFT_ZERO_X                          ((uint16)0U)
#define TFT_ZERO_Y                          ((uint16)0U)
#define TFT_PIXELS_UI16                     ((uint16)8192U)

#define TFT_DC_PIN_UI8                      (GPIO_CHANNEL_PB0)
#define TFT_WR_PIN_UI8                      (GPIO_CHANNEL_PB0)
#define TFT_BACKLIGHT_PIN_UI8               (GPIO_CHANNEL_PB0)
#define TFT_CS_PIN_UI8                      (GPIO_CHANNEL_PB0)
#define TFT_RESET_PIN_UI8                   (GPIO_CHANNEL_PB0)

#define Ssd1306_ChannelDB0                      (GPIO_CHANNEL_PB0)
#define Ssd1306_ChannelDB1                      (GPIO_CHANNEL_PB0)
#define Ssd1306_ChannelDB2                      (GPIO_CHANNEL_PB0)
#define Ssd1306_ChannelDB3                      (GPIO_CHANNEL_PB0)
#define Ssd1306_ChannelDB4                      (GPIO_CHANNEL_PB0)
#define Ssd1306_ChannelDB5                      (GPIO_CHANNEL_PB0)
#define Ssd1306_ChannelDB6                      (GPIO_CHANNEL_PB0)
#define Ssd1306_ChannelDB7                      (GPIO_CHANNEL_PB0)
#define Ssd1306_ChannelDB8                      (GPIO_CHANNEL_PB0)
#define Ssd1306_ChannelDB9                      (GPIO_CHANNEL_PB0)
#define Ssd1306_ChannelDB10                     (GPIO_CHANNEL_PB0)
#define Ssd1306_ChannelDB11                     (GPIO_CHANNEL_PB0)
#define Ssd1306_ChannelDB12                     (GPIO_CHANNEL_PB0)
#define Ssd1306_ChannelDB13                     (GPIO_CHANNEL_PB0)
#define Ssd1306_ChannelDB14                     (GPIO_CHANNEL_PB0)
#define Ssd1306_ChannelDB15                     (GPIO_CHANNEL_PB0)

#define HDP                                 (479U)           // 479
#define HT                                  (531U)           // 525
#define HPS                                 (43U)            // 40
#define LPS                                 (8U)             // 6
#define HPW                                 (10U)            // 39

#define VDP                                 (271U)           // 271
#define VT                                  (288U)           // 288
#define VPS                                 (12U)            // 12
#define FPS                                 (4U)             // 8
#define VPW                                 (10U)            // 11

/* TFT Controler Register defines */
#define TFT_RAM_REG_ADDR_UI16               ((uint16)0x002CU)
#define TFT_COL_START_ADDR2_REG_ADDR_UI16   ((uint16)0x002AU)
#define TFT_COL_START_ADDR1_REG_ADDR_UI16   ((uint16)0x0000U)
#define TFT_COL_END_ADDR2_REG_ADDR_UI16     ((uint16)0x0000U)
#define TFT_COL_END_ADDR1_REG_ADDR_UI16     ((uint16)0x0000U)
#define TFT_ROW_START_ADDR2_REG_ADDR_UI16   ((uint16)0x002BU)
#define TFT_ROW_START_ADDR1_REG_ADDR_UI16   ((uint16)0x0000U)
#define TFT_ROW_END_ADDR2_REG_ADDR_UI16     ((uint16)0x0000U)
#define TFT_ROW_END_ADDR1_REG_ADDR_UI16     ((uint16)0x0000U)



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
/* ***************************** enums ************************************** */

#endif /* TFT_CFG_H */
