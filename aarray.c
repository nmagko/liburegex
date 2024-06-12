/* ASSOCIATIVE ARRAY functions implementation.
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
 *    Associative Array:                      <aarray.c>
 */
   
#define _GNU_SOURCE

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

#include "uregex.h"

void fatal(char * msg) {
  perror(msg);
  kill(getpid(), SIGKILL); // equal to: raise(SIGKILL);
}

struct AArray {
  char **asoc_array;
  long iaa;
};

int put_aa (struct AArray *aarr, const char *keyaa, const char *valueaa) {
  long one_step = 0;
  long ikeyaa;
  for (ikeyaa = 0; ikeyaa < aarr->iaa; ikeyaa += 2) { /* exist? */
    if ( 0 == strcmp(aarr->asoc_array[ikeyaa], keyaa) ) { /* update */
      one_step = ikeyaa + 1;
      aarr->asoc_array[one_step] = xrealloc(aarr->asoc_array[one_step], strlen(valueaa) + 1);
      strcpy(aarr->asoc_array[one_step], valueaa);
      break;
    }
  }
  if ( one_step == 0 ) { /* append */
    aarr->asoc_array[aarr->iaa] = xmalloc(strlen(keyaa) + 1);
    strcpy(aarr->asoc_array[aarr->iaa], keyaa);
    aarr->iaa++;
    aarr->asoc_array[aarr->iaa] = xmalloc(strlen(valueaa) + 1);
    strcpy(aarr->asoc_array[aarr->iaa], valueaa);
    aarr->iaa++;
    aarr->asoc_array = xrealloc(aarr->asoc_array, sizeof(aarr->asoc_array) * (aarr->iaa + 2));
  }
  return(1);
}

char *get_aa (const struct AArray *aarr, const char *keyaa) {
  long one_step = 0;
  long ikeyaa;
  for (ikeyaa = 0; ikeyaa < aarr->iaa; ikeyaa += 2) { /* exist? */
    if ( 0 == strcmp(aarr->asoc_array[ikeyaa], keyaa) ) {
      one_step = ikeyaa + 1;
      break;
    }
  }
  if ( one_step == 0 )
    return NULL;
  return (aarr->asoc_array[one_step]); /* get data */
}

int delete_aa (struct AArray *aarr, const char *keyaa) {
  long juer = -1;
  long ikeyaa;
  for (ikeyaa = 0; ikeyaa < aarr->iaa; ikeyaa += 2) { /* exist? */
    if ( 0 == strcmp(aarr->asoc_array[ikeyaa], keyaa) ) {
      juer = ikeyaa;
      break;
    }
  }
  if ( juer == -1 ) {
    return(0);
  } else {
    aarr->asoc_array[juer] = xrealloc(aarr->asoc_array[juer], 1);
    strcpy(aarr->asoc_array[juer], "");
    return(1);
  }
}

char **keys_aa (const struct AArray *aarr, long *isa) {
  char **keyas = xmalloc(sizeof(keyas));
  long is = -1;
  long ikeyaa;
  for (ikeyaa = 0; ikeyaa < aarr->iaa; ikeyaa += 2) { /* exist? */
    if ( 0 != strcmp(aarr->asoc_array[ikeyaa], "") ) {
      keyas[is+1] = xmalloc(strlen(aarr->asoc_array[ikeyaa]) + 1);
      strcpy(keyas[is+1], aarr->asoc_array[ikeyaa]);
      is++;
      keyas = xrealloc(keyas, sizeof(keyas) * (is + 2));
    }
  }
  *isa = is + 1;
  return keyas;
}

char **values_aa (const struct AArray *aarr, long *isa) {
  char **valueas = xmalloc(sizeof(valueas));
  long is = -1;
  long ikeyaa;
  for (ikeyaa = 0; ikeyaa < aarr->iaa; ikeyaa += 2) { /* exist? */
    if ( 0 != strcmp(aarr->asoc_array[ikeyaa], "") ) {
      valueas[is+1] = xmalloc(strlen(aarr->asoc_array[ikeyaa+1]) + 1);
      strcpy(valueas[is+1], aarr->asoc_array[ikeyaa+1]);
      is++;
      valueas = xrealloc(valueas, sizeof(valueas) * (is + 2));
    }
  }
  *isa = is + 1;
  return valueas;
}

struct AArray *new_aa () { /* constructor */
  struct AArray *aarr = xmalloc(sizeof(struct AArray));
  aarr->asoc_array = xmalloc(sizeof(aarr->asoc_array) * 2);
  aarr->iaa = 0;
  return aarr;
}

void free_double_pointer (char **double_pointer, long isa) {
  /* freeing double pointer */
  long frisa;
  for ( frisa = 0; frisa < isa; frisa++ )
    free(double_pointer[frisa]);
  free(double_pointer);
}

void destroy_aa (struct AArray *aarr) {
  /* freeing data */
  free_double_pointer (aarr->asoc_array, aarr->iaa);
  /* freeing aarr */
  free (aarr);
}

