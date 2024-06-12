/* long test program for liburegex
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

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <uregex.h>
#include <aarray.h>

int main (void) {
  char *orig_text, *text, *new_text;
  char **keys, **values, **strarray;
  long keys_count, values_count, ic;
  int count_strarray;
  struct AArray *my_aa;
  
  char *jointext = join (":", "user", "password", "uid", "gid", "comment", "home", "shell", NULL);
  printf ("-*-*-*- join -*-*-*-\n%s\n", jointext);
  
  orig_text = savestring(
    "  The licenses for most software are designed to take away your\n"
    "freedom to share and change it.  By contrast, the GNU General Public\n"
    "License is intended to guarantee your freedom to share and change free\n"
    "software--to make sure the software is free for all its users.  This\n"
    "General Public License applies to most of the Free Software\n"
    "Foundation's software and to any other program whose authors commit to\n"
    "using it.  (Some other Free Software Foundation software is covered by\n"
    "the GNU Library General Public License instead.)  You can apply it to\n"
    "your programs, too.\n"
  );
  text = savestring(orig_text);
  
  if ( m_rex ( text, "^[ \t]*th", "in" ) ) printf ("-*-*-*- m_rex -*-*-*-\nmatch\n");
  
  /* removes alphanumerics from start, line to line */
  new_text = s_rex (text, "^\\w", "", "gn");
  printf ("-*-*-*- s_rex (removes alphanumeris from start, line to line) -*-*-*-\n%s\n", new_text);
  free (new_text);
  
  /* removes alphanumerics occurrence */
  text = as_rex (text, "\\w", "", "g");
  printf ("-*-*-*- as_rex (removes alphanumerics) -*-*-*-\n%s\n", text);
  
  free(text); text = savestring(orig_text);
  
  /* removes vowels */
  new_text = y_rex (text, "aeiou", "", "d");
  printf ("-*-*-*- y_rex (removes vowels) -*-*-*-\n%s\n", new_text);
  free (new_text);
  
  /* removes non vowels */
  new_text = y_rex (text, "aeiou", "", "cd");
  printf ("-*-*-*- y_rex (removes non vowels) -*-*-*-\n%s\n", new_text);
  free (new_text);
  
  /* to lower case */
  text = ay_rex (text, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", "abcdefghijklmnopqrstuvwxyz", "");
  printf ("-*-*-*- ay_rex (to lower case) -*-*-*-\n%s\n", text);
  
  /* vowels to upper case */
  text = ay_rex (text, "aeiou", "AEIOU", "");
  printf ("-*-*-*- ay_rex (vowels to upper case) -*-*-*-\n%s\n", text);
  
  my_aa = new_aa();

  put_aa (my_aa, "year", "2006");
  put_aa (my_aa, "clave", "valor");
  put_aa (my_aa, "0003", "a number?");

  printf ("-*-*-*- get_aa -*-*-*-\nThe value of `year' key is %s\n", get_aa (my_aa, "year"));

  delete_aa (my_aa, "year");

  keys = keys_aa (my_aa, &keys_count);
  printf ("-*-*-*- keys_aa -*-*-*-\n");
  for (ic = 0; ic < keys_count; ic ++)
    printf ("%s\n", keys[ic]);

  values = values_aa (my_aa, &values_count);
  printf ("-*-*-*- values_aa -*-*-*-\n");
  for (ic = 0; ic < values_count; ic ++)
    printf ("%s\n", values[ic]);

  strarray = split ( ":", "i", jointext, &count_strarray );
  printf ("-*-*-*- split -*-*-*-\n");
  for (ic = 0; ic < count_strarray; ic ++)
    printf ("%s\n", strarray[ic]);

  free(jointext);
  jointext = joindp (":", values, values_count);
  printf ("-*-*-*- join -*-*-*-\n%s\n", jointext);
  
  free_double_pointer (values, values_count);

  destroy_aa (my_aa);
  
  exit (EXIT_SUCCESS);
}
