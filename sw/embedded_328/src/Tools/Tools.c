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

/*--- External Function Definitions ----------------------------------*/

/**
 * \brief Custom variant of the strtok() function from libc
 *
 * For a detailed description of this function and its parameters,
 * please have a look the concerning MAN pages (man strtok).
 */
char *Tools_StrtokREmpty(char *p_str, const char *p_delim, char **pp_save)
{
    /* *pp_save will save the pointer to the start of the next
     * unprocessed token or NULL if there are no more tokens */
    char *p_start = (p_str ? p_str : *pp_save);

    if (p_start) {
      /* look for start of next token, or NULL */
      *pp_save = strpbrk(p_start, p_delim);

      if (*pp_save) {
         /* delimiter found */
         **pp_save = '\0'; /* terminate current token */
         ++*pp_save;       /* skip delimiter on next call */
      }
   }
   /* return current token or NULL */
   return p_start;
}

