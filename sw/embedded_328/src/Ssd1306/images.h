/**
 * \file images.h
 * \brief This file provides access to images, that can be printed on the display.
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

#ifndef IMAGES_H_
#define IMAGES_H_

/*--- Includes -------------------------------------------------------*/

#include <avr/pgmspace.h>

/*--- External Data --------------------------------------------------*/

extern unsigned char sd_locked[] PROGMEM;
extern unsigned char sd_unlocked[] PROGMEM;
extern unsigned char satellit[] PROGMEM ;
extern unsigned char no_sd_card[] PROGMEM;
extern unsigned char sd_card[] PROGMEM ;
extern unsigned char no_lock[] PROGMEM ;
extern unsigned char record_init[] PROGMEM;
extern unsigned char sd_rec_on[] PROGMEM ;
extern unsigned char sd_rec_pause[] PROGMEM ;
extern unsigned char sd_rec_stop[] PROGMEM ;
extern unsigned char tacho[] PROGMEM;
extern unsigned char height[] PROGMEM;
extern unsigned char compass[] PROGMEM;
extern unsigned char pole[] PROGMEM;
extern unsigned char cloud[] PROGMEM;
extern unsigned char logo[] PROGMEM;

#endif /* #ifndef _IMAGES_H_ */


