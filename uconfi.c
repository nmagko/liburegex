/* Unique CONFIg function implementation.
 * Copyright (C) 2005-2016  Victor Clodoaldo Salas Pumacayo (aka nmag)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *    Victor Clodoaldo Salas Pumacayo <nmag.ko@gmail.com>,
 *    Victor Clodoaldo Salas Pumacayo <vsalas@member.fsf.org>
 */

/*
 *    Unique Config:                          <uconfi.c>
 */

#define _GNU_SOURCE

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "aarray.h"
#include "uregex.h"

#define UCONFILL 512               /* Unique CONFIg Line Length */

/* Obtain configuration from file and put it into Asociative Array
 * Structure (AArray).
 */
struct AArray *get_config (const char *filename) {
  int fdes, ele_strarray;
  FILE *fstream;
  char *gs = (char *) xmalloc (UCONFILL);
  struct AArray *config = new_aa();
  char **strarray;
  if ( ( fdes = open(filename, O_RDONLY) ) == -1 ) {
    printf ("open %s, failed.\n", filename);
    free(gs);
    exit(EXIT_FAILURE);
  }
  if ( ( fstream = fdopen(fdes, "r") ) != NULL ) {
    while ( fgets(gs, UCONFILL - 1, fstream) != NULL ) {
      gs[UCONFILL - 1] = 0x00; /* NULL */
      /* getting data from config file */
      gs = as_rex (gs, "(#|;|//).*$", "", "");
      gs = as_rex (gs, "[ \t\r\n\'\"]", "", "g");
      gs = as_rex (gs, "(%20)+", " ", "g");
      if ( m_rex (gs, "[:=]", "") ) {
        strarray = split ("(:|=|:=)+", "", gs, &ele_strarray);
        put_aa(config, strarray[0], strarray[1]);
        free_double_pointer(strarray, ele_strarray);
      }
      /* reallocationg gs memory, because s_rex reallocate them */
      gs = (char *) xrealloc (gs, UCONFILL);
    }
    fclose(fstream);
  } else {
    printf("reading %s, failed.\n", filename);
    free(gs);
    close(fdes);
    exit(EXIT_FAILURE);
  }
  free(gs);
  close(fdes);
  return(config);
}

