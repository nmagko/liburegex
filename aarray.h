/* Define ASSOCIATIVE ARRAY functions.
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
 *    Associative Array:                      <aarray.h>
 */
   
#define _GNU_SOURCE

#ifndef _AARRAY_H
#define _AARRAY_H

struct AArray {
  char **asoc_array;
  long iaa;
};

extern int put_aa (struct AArray *, const char *, const char *);

extern char *get_aa (const struct AArray *, const char *);

extern int delete_aa (struct AArray *, const char *);

extern char **keys_aa (const struct AArray *, long *);

extern char **values_aa (const struct AArray *, long *);

extern struct AArray *new_aa ();

extern void destroy_aa (struct AArray *);

extern void free_double_pointer (char **, long);

#endif /* !_AARRAY_H */

