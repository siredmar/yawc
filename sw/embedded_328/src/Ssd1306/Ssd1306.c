/**
*  COPYRIGHT: Armin Schlegel
*  \file tft.h
*  \brief Display Graphic Driver
*
*  This header file contains all tft specific datatypes and defines.
*
*******************************************************************************/

/* ***************************** includes < > ******************************* */

/* ***************************** includes " " ******************************* */
#include "Ssd1306.h"

#include "Ssd1306_Cfg.h"
#include "Ssd1306_Lcfg.h"
#include "Gpio.h"

#include "Platform_Types.h"
#include <util/delay.h>


/* ***************************** # defines ********************************** */

/* ***************************** modul global data ************************** */

/* ***************************** modul global functions ********************* */

/*  \brief  Sends the parameters of an command to the display controller
*
*  \param [in]  data_ui16 = data to be sent
*  \param [out] ---
*  \return      ---
*/
static void Ssd1306_sendData
(
   uint16 data_ui16
)
{
	Ssd1306_selectRS();
#if DEBUG_TFT_ENABLED
   printf("(DBG): Ssd1306_sendData:\t\t0x%04X\n", data_ui16);
#endif

   /* TODO */
   //dio_writePortGroup((uint8)data_ui16);
//   dio_writePortGroup_8((uint8)data_ui16);
	Ssd1306_deSelectWR();
	Ssd1306_selectWR();
}

/*  \brief  Sends the pixel data to the display controller
*
*  \param [in]  data_ui16 = data to be sent
*  \param [out] ---
*  \return      ---
*/
void Ssd1306_sendPixelData
(
   uint16 data_ui16
)
{
   Ssd1306_selectRS();
#if DEBUG_TFT_ENABLED
   printf("(DBG): Ssd1306_sendPixelData:\t\t0x%04X\n", data_ui16);
#endif

   /* TODO */
//   dio_writePortGroup((uint16)data_ui16);

	Ssd1306_deSelectWR();
	Ssd1306_selectWR();
}

/**  \brief Sends a command to the display controller
*
*  \param [in]  data_ui16 = Data to send
*  \param [out] ---
*  \return      ---
*/
static void Ssd1306_sendCommand
(
   uint16 data_ui16
)
{
	Ssd1306_deSelectRS();
#if DEBUG_TFT_ENABLED
	printf("(DBG): Ssd1306_sendCommand:\t\t0x%04X\n", data_ui16);
#endif
	/* TODO */
//	dio_writePortGroup_8((uint8)data_ui16);
	//dio_writePortGroup((uint16)data_ui16);
	Ssd1306_deSelectWR();
	Ssd1306_selectWR();
}

/*  \brief Sets register on TFT display controller
*
*  \param [in]  Ssd1306_register_ui16 = tft Controler register address
*  \param [in]  Ssd1306_registerValue_ui16 = Register value to write
*  \param [in]  Ssd1306_registerParameters_ui16 = Parameters count to write
*  \param [out] ---
*  \return      ---
*/
static void Ssd1306_setDisplayRegister
(
   uint16 Ssd1306_register_ui16,
   uint16 Ssd1306_registerValue_ui16,
   uint16 Ssd1306_registerParameters_ui16
)
{
   /* First send Command if it is != NOCMD */
   if(Ssd1306_register_ui16 != NOCMD)
   {
      Ssd1306_sendCommand(Ssd1306_register_ui16);
   }
   /* Then send or continue sending Parameter to tft controler if needed */
   if(Ssd1306_registerParameters_ui16 != NOPARAM)
   {
      Ssd1306_sendData(Ssd1306_registerValue_ui16);
   }
}

/*  \brief Waits x ms
*
*  \param [in]  waitTime_ui16 = Wait time in ms
*  \param [out] ---
*  \return      ---
*/
static void Ssd1306_waitXms
(
   uint16 waitTime_ui16
)
{
	uint16 tmp_ui16 = 0U;
	for(tmp_ui16 = 0U; tmp_ui16 < waitTime_ui16; tmp_ui16++)
	{
		_delay_us(1000U);
	}
}


/*  \brief Set window with defined height and width window
*          will be drawn form top to bottom
*
*  \param [in]  startPositionX_ui16 = Top start X position window
*  \param [in]  startPositionY_ui16 = Top start Y position window
*  \param [in]  windowHeight_ui16 = Window height
*  \param [in]  windowWidth_ui16 = Window width
*  \param [out] ---
*  \return      ---
*/
void Ssd1306_setWindow
(
   uint16 startPositionX_ui16,
   uint16 startPositionY_ui16,
   uint16 endPositionX_ui16,
   uint16 endPositionY_ui16
)
{

   Ssd1306_deSelectCS();
   Ssd1306_setDisplayRegister(TFT_COL_START_ADDR2_REG_ADDR_UI16,((startPositionX_ui16 & 0xFF00) >> 8U), 1U);
   Ssd1306_setDisplayRegister(NOCMD,                            (startPositionX_ui16 & 0x00FF), 1U);
   Ssd1306_setDisplayRegister(NOCMD,                            ((endPositionX_ui16 & 0xFF00) >> 8U), 1U);
   Ssd1306_setDisplayRegister(NOCMD,                            (endPositionX_ui16 & 0x00FF), 1U);
   Ssd1306_setDisplayRegister(TFT_ROW_START_ADDR2_REG_ADDR_UI16,((startPositionY_ui16 & 0xFF00) >> 8U), 1U);
   Ssd1306_setDisplayRegister(NOCMD,                            (startPositionY_ui16 & 0x00FF), 1U);
   Ssd1306_setDisplayRegister(NOCMD,                            ((endPositionY_ui16 & 0xFF00) >> 8U), 1U);
   Ssd1306_setDisplayRegister(NOCMD,                            (endPositionY_ui16 & 0x00FF), 1U);
   Ssd1306_selectCS();
  // Ssd1306_sendCommand(TFT_RAM_REG_ADDR_UI16);
}

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
)
{

   uint16 forCounter_ui16 = 0U;
   Std_ReturnType returnValue = E_NOT_OK;

   Ssd1306_initSequenceType const *tmpInitSequence_ps;
   tmpInitSequence_ps = ConfigPtr->Ssd1306_initSequence_ps;

   /* Reset Display */
   Ssd1306_selectReset();
   /* Wait 50ms */
   Ssd1306_waitXms(5U);

   Ssd1306_deSelectReset();

   /* Wait 100ms */
   Ssd1306_waitXms(10U);

   /* Reset Display done */
   Ssd1306_selectReset();

   /* activate CS */
   Ssd1306_deSelectCS();

   /* Enable Backlight */
   Ssd1306_selectBacklight();

   /* Pull WR Pin high */
   Ssd1306_selectWR();

   Ssd1306_waitXms(20U);

   /* Init Sequence */
   for(forCounter_ui16 = 0U; forCounter_ui16 < ConfigPtr->Ssd1306_numberOfInitSeq_ui16; forCounter_ui16++)
   {
      /* Send Sequence */
      Ssd1306_setDisplayRegister((tmpInitSequence_ps + forCounter_ui16)->Ssd1306_registerAddress_ui16,
                             (tmpInitSequence_ps + forCounter_ui16)->Ssd1306_registerValue_ui16,
                             (tmpInitSequence_ps + forCounter_ui16)->Ssd1306_registerParameters_ui16);

      /* Wait X ms */
      Ssd1306_waitXms((tmpInitSequence_ps + forCounter_ui16)->Ssd1306_waitTimeAfterSend_ui16);

   }
   /* deactivate CS */
   Ssd1306_selectCS();

   returnValue = E_OK;
   return returnValue;
}

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
)
{
   Std_ReturnType returnValue = E_NOT_OK;
   Ssd1306_drawRectangle(0U, 0U, 480U, 272U, color_ui16);
   returnValue = E_OK;
   return returnValue;
}


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
)
{
   Std_ReturnType returnValue = E_NOT_OK;

   /* Set window size and position - modified for portrait view */
   Ssd1306_setWindow(xPosition_ui16, yPosition_ui16, xPosition_ui16+1, yPosition_ui16+1);

   Ssd1306_deSelectCS();

   Ssd1306_sendCommand(TFT_RAM_REG_ADDR_UI16);

   Ssd1306_sendPixelData(rectangleColor_ui16);

   Ssd1306_selectCS();

   returnValue = E_OK;

   return returnValue;
}

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
)
{
   Std_ReturnType returnValue = E_NOT_OK;
   uint32 forCounter_ui32 = 0U;
   uint16 endPositionX_ui16 = 0U;
   uint16 endPositionY_ui16 = 0U;
   uint32 pixelSizeOfRectangle_ui32 = 0U;

   endPositionX_ui16 = xPosition_ui16 + rectangleWidth_ui16;
   endPositionY_ui16 = yPosition_ui16 + rectangleHeight_ui16;

   /* secure border of display */
   if(endPositionX_ui16 > (TFT_WIDTH_UI16 - 1U))
   {
	   endPositionX_ui16 = TFT_WIDTH_UI16 - 1U;
   }
   else if(endPositionY_ui16 > (TFT_HEIGHT_UI16 - 1U))
   {
	   endPositionY_ui16 = TFT_HEIGHT_UI16 - 1U;
   }
   else
   {
      /* do nothing */
   }
   /* calculate pixel size */
   pixelSizeOfRectangle_ui32 = rectangleWidth_ui16 * rectangleHeight_ui16;

   Ssd1306_setWindow(xPosition_ui16, yPosition_ui16, endPositionX_ui16, endPositionY_ui16);

   /* activate CS */
   Ssd1306_deSelectCS();

   Ssd1306_deSelectRS();

   Ssd1306_sendCommand(TFT_RAM_REG_ADDR_UI16);

   Ssd1306_selectRS();

   for(forCounter_ui32 = 0U; forCounter_ui32 < pixelSizeOfRectangle_ui32; forCounter_ui32++)
   {
      Ssd1306_sendPixelData(rectangleColor_ui16);
   }
   Ssd1306_selectCS();

   returnValue = E_OK;

   return returnValue;
}


void Ssd1306_drawStart()
{
    Ssd1306_deSelectCS();
    Ssd1306_sendCommand(TFT_RAM_REG_ADDR_UI16);
}

void Ssd1306_drawStop()
{
    Ssd1306_selectCS();
}
