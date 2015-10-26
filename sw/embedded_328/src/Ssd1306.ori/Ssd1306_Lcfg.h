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
#include "../Ssd1306.ori/Ssd1306.h"

/* ***************************** # defines ********************************** */
#define NOCMD     (0xFFFFU)
#define NOPARAM   (0U)
#define PARAM     (1U)
/* ***************************** modul global data ************************** */

/* ***************************** global data ******************************** */


const void *Ssd1306_GetLcfgData(void);

