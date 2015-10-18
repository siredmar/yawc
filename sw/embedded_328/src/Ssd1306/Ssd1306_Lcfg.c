/*
 * Ssd1306_lcfg.c
 *
 *  Created on: Nov 10, 2012
 *      Author: armin
 */
#include "../Ssd1306/Ssd1306_Lcfg.h"

const Ssd1306_initSequenceType Ssd1306_initialConfig4d3_s[] =
{
		{0x00E2, 0x002D, 				      0U, PARAM},		// PLL config
		{NOCMD,  0x0002, 				      0U, PARAM},
		{NOCMD,  0x0004, 				      1U, PARAM},

		{0x00E0, 0x0001, 				      1U, PARAM},		// PLL config - continued

		{0x00E0, 0x0003, 				      5U, PARAM},		// PLL config - continued

		{0x0001, 0x0000, 				      5U, NOPARAM},	// software reset

		{0x00E6, 0x0000, 				      0U, PARAM},		// LSHIFT freq
		{NOCMD,  0x00FF, 				      0U, PARAM},
		{NOCMD,  0x00BE, 				      1U, PARAM},

		{0x00B0, 0x0020, 				      1U, PARAM},		// set lcd mode
		{NOCMD,  0x0000, 				      1U, PARAM},
		{NOCMD,  ((HDP >> 8U) & 0x00FF), 1U, PARAM},
		{NOCMD,  (HDP & 0x00FF), 		   1U, PARAM},
		{NOCMD,  ((HDP >> 8U) & 0x00FF), 1U, PARAM},
		{NOCMD,  (VDP & 0x00FF), 		   1U, PARAM},
		{NOCMD,  0x0000, 				      5U, PARAM},

		{0x00B4, ((HT >> 8U) & 0x00FF),  1U, PARAM},		// horizontal period
		{NOCMD,  (HT & 0x00FF), 		   1U, PARAM},
		{NOCMD,  ((HPS >> 8U) & 0x00FF), 1U, PARAM},
		{NOCMD,  (HPS & 0x00FF), 		   1U, PARAM},
		{NOCMD,  HPW, 					      1U, PARAM},
		{NOCMD,  ((LPS >> 8U) & 0x00FF), 1U, PARAM},
		{NOCMD,  (LPS & 0x00FF), 		   1U, PARAM},
		{NOCMD,  0x0000, 				      1U, PARAM},

		{0x00B6, ((VT >> 8U) & 0x00FF),  1U, PARAM},		// vertical period
		{NOCMD,  (VT & 0x00FF), 		   1U, PARAM},
		{NOCMD,  (VPS >> 8U) & 0x00FF, 	1U, PARAM},
		{NOCMD,  (VPS & 0x00FF), 		   1U, PARAM},
		{NOCMD,  VPW, 					      1U, PARAM},
		{NOCMD,  ((FPS >> 8U) & 0x00FF), 1U, PARAM},
		{NOCMD,  (FPS & 0x00FF), 		   1U, PARAM},

		{0x0036, 0x0000, 				      1U, PARAM},		// rotation - landscape

		{0x00F0, 0x0003, 				      5U, PARAM},		// pixel data interface 16 bit    0b011 is 16 Bit

		{0x0029, 0x0000, 				      1U, NOPARAM},	// set display on

		{0x00BE, 0x0006, 				      1U, PARAM},		// pwm config
		{NOCMD,  0x00F0, 				      1U, PARAM},
		{NOCMD,  0x0001, 				      1U, PARAM},
		{NOCMD,  0x00F0, 				      1U, PARAM},
		{NOCMD,  0x0000, 				      1U, PARAM},
		{NOCMD,  0x0000, 				      1U, PARAM},

		{0x00D0, 0x000D, 				      1U, PARAM},		// pwm coffig continued

		{0x00B8, 0x0000, 				      1U, PARAM},		// gpio config
		{NOCMD,  0x0000, 				      1U, PARAM},

		{0x00BA, 0x0000, 				      1U, PARAM}		// gpio to 0x00
};

const Ssd1306_configType Ssd1306_initialConfigTft_s =
{
   Ssd1306_initialConfig4d3_s,		/* config struct for tft */
   44U 							/* number of config entries is 44 */
};

const void *Ssd1306_getLcfgData
(
   void
)
{
   return ((void*) &Ssd1306_initialConfigTft_s);
}

