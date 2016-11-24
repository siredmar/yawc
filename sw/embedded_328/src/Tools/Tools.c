/**
 * \file tools.c
 * \brief This file contains the implementation of the tooling module.
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

#include "Tools.h"

uint8 FloatContainsDecimals(float32 num)
{
    float32 intpart = (uint32)num;
    float32 decpart = (float32)((float32)num - (float32)intpart);
    if(decpart == 0.0f)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

float32 GetFloatDecimals(float32 num)
{
    float32 intpart = (uint32)num;
    float32 decpart = (float32)((float32)num - (float32)intpart);
    return decpart;
}

