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
   #define TFT_WIDTH_UI16                   ((uint16)480U)
   #define TFT_HEIGHT_UI16                  ((uint16)272U)
#else
   #define TFT_WIDTH_UI16                   ((uint16)272U)
   #define TFT_HEIGHT_UI16                  ((uint16)480U)
#endif
#define TFT_ZERO_X                          ((uint16)0U)
#define TFT_ZERO_Y                          ((uint16)0U)
#define TFT_PIXELS_UI32                     ((uint32)130560U)

#define TFT_RS_PIN_UI8                      (GPIO_CHANNEL_PB0)
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

/* ***************************** enums ************************************** */

#endif /* TFT_CFG_H */
