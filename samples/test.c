/* test program for liburegex (include test.conf file)
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
 * gcc -O2 -Wall -luregex -o test test.c
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <uconfi.h>

int main (void) {
  char **key;
  long int k_lim;
  int idx;
  struct AArray *config = get_config("./test.conf");
  key = keys_aa(config, &k_lim);
  /* show keys and values */
  for ( idx = 0; idx < k_lim; idx++ )
    printf("%s=%s\n", key[idx], get_aa(config, key[idx]));
  destroy_aa(config);
  return (0);
}
