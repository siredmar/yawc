/**
*  COPYRIGHT: Armin Schlegel
*  \file tft.h
*  \brief Display Graphic Driver
*
*  This header file contains all tft specific datatypes and defines.
*
*******************************************************************************/
#ifndef TFT_H
#define TFT_H

/* ***************************** includes < > ******************************* */

/* ***************************** includes " " ******************************* */
#include "../Ssd1306/Ssd1306_Cfg.h"
#include "../Ssd1306/Ssd1306_Lcfg.h"
#include "Std_Types.h"


/* Function definitions */
#define Ssd1306_selectRS()             Gpio_WriteChannel(TFT_RS_PIN_UI8, STD_HIGH)
#define Ssd1306_deSelectRS()           Gpio_WriteChannel(TFT_RS_PIN_UI8, STD_LOW)

#define Ssd1306_selectCS()             Gpio_WriteChannel(TFT_CS_PIN_UI8, STD_HIGH)
#define Ssd1306_deSelectCS()           Gpio_WriteChannel(TFT_CS_PIN_UI8, STD_LOW)

#define Ssd1306_selectWR()             Gpio_WriteChannel(TFT_WR_PIN_UI8, STD_HIGH)
#define Ssd1306_deSelectWR()           Gpio_WriteChannel(TFT_WR_PIN_UI8, STD_LOW)

#define Ssd1306_selectBacklight()      Gpio_WriteChannel(TFT_BACKLIGHT_PIN_UI8, STD_HIGH)
#define Ssd1306_deSelectBacklight()    Gpio_WriteChannel(TFT_BACKLIGHT_PIN_UI8, STD_LOW)

#define Ssd1306_selectReset()          Gpio_WriteChannel(TFT_RESET_PIN_UI8, STD_HIGH)
#define Ssd1306_deSelectReset()        Gpio_WriteChannel(TFT_RESET_PIN_UI8, STD_LOW)



/* ***************************** typedefs *********************************** */
typedef uint16 Ssd1306_ColorType;

/* ***************************** enums ************************************** */
typedef enum
{
   WHITE       = (0xFFFFU),
   BLACK       = (0x0000U),
   BLUE        = (0x001FU),
   BRED        = (0xF81FU),
   GRED 		   = (0xFFE0U),
   GBLUE		   = (0x07FFU),
   RED         = (0xF800U),
   MAGENTA     = (0xF81FU),
   GREEN       = (0x07E0U),
   CYAN        = (0x7FFFU),
   YELLOW      = (0xFFE0U),
   BROWN 		= (0xBC40U),
   BRRED 		= (0xFC07U),
   GRAY  		= (0x8430U),
}Ssd1306_stdColorsType;

/* ***************************** macros ************************************* */
/* ***************************** structs ************************************ */

/**
*  \brief init sequence description type
*
*  Struct for postbuild init sequence description
*/
typedef struct
{
   uint16 Ssd1306_registerAddress_ui16;
   uint16 Ssd1306_registerValue_ui16;
   uint16 Ssd1306_waitTimeAfterSend_ui16;
   uint16 Ssd1306_registerParameters_ui16;
}Ssd1306_initSequenceType;


/**
*  \brief init container
*
*  This type of data structure containing the initialization data for the TFT driver
*/
typedef struct
{
   Ssd1306_initSequenceType const    *Ssd1306_initSequence_ps;
   uint16                         Ssd1306_numberOfInitSeq_ui16;
}Ssd1306_configType;

/* ***************************** global data ******************************** */
extern const Ssd1306_configType Ssd1306_initialConfigTft_s;


/* ***************************** modul global prototypes ******************** */
/* ***************************** global functions *************************** */
/*
*  \brief Function initialize the Display Graphic Driver and the tft Display
*
*  \param [in]  initSequence = Pointer to init Sequence
*  \param [out] ---
*  \return      ---
*/
Std_ReturnType Ssd1306_init
(
	const Ssd1306_configType *ConfigPtr
);



/*
*  \brief Function clears whole screen with one color
*
*  \param [in]  color_ui16 =  Color Value
*  \param [out] ---
*  \return      ---
*/
Std_ReturnType Ssd1306_clearScreen
(
   Ssd1306_ColorType color_ui16
);

//void Ssd1306_drawStart();
//
//void Ssd1306_drawStop();

/*
*  \brief Function draw a filled Rectangle at x/y position
*
*  \param [in]  xStartPosition_ui16 = X Start position of rectangle
*  \param [in]  yStartPosition_ui16 = Y Start position of rectangle
*  \param [in]  rectangleWidth_ui16 = Width of rectangle in pixel
*  \param [in]  rectangleHeight_ui16 = Height of rectangle in pixel
*  \param [in]  rectangleColor_ui16 = Color of rectangle

*  \param [out] ---
*  \return      E_OK = Rectangle draw successfull
*               E_NOT_OK = Rectangle draw not successfull
*/
Std_ReturnType Ssd1306_drawPixel
(
   uint16 xPosition_ui16,
   uint16 yPosition_ui16,
   Ssd1306_ColorType rectangleColor_ui16
);

/*
*  \brief Function draw a filled Rectangle at x/y position
*
*  \param [in]  xStartPosition_ui16 = X Start position of rectangle
*  \param [in]  yStartPosition_ui16 = Y Start position of rectangle
*  \param [in]  rectangleWidth_ui16 = Width of rectangle in pixel
*  \param [in]  rectangleHeight_ui16 = Height of rectangle in pixel
*  \param [in]  rectangleColor_ui16 = Color of rectangle

*  \param [out] ---
*  \return      E_OK = Rectangle draw successfull
*               E_NOT_OK = Rectangle draw not successfull
*/
Std_ReturnType Ssd1306_drawRectangle
(
   uint16 xPosition_ui16,
   uint16 yPosition_ui16,
   uint16 rectangleWidth_ui16,
   uint16 rectangleHeight_ui16,
   Ssd1306_ColorType rectangleColor_ui16
);

void Ssd1306_setWindow
(
   uint16 startPositionX_ui16,
   uint16 startPositionY_ui16,
   uint16 endPositionX_ui16,
   uint16 endPositionY_ui16
);

void Ssd1306_sendPixelData
(
   uint16 data_ui16
);

void Ssd1306_drawStart();

void Ssd1306_drawStop();

#endif
