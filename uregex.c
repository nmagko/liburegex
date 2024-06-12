/* UREGEX functions implementation.
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
 *    Unique Regular Expressions:                 <uregex.c>
 */
   
#define _GNU_SOURCE

#ifdef CYGWIN32
#define program_invocation_short_name "uregex.c"
#endif

#include <regex.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* eXtended malloc */
void *xmalloc (size_t size) {
  register void *value = malloc(size);
  if ( value == 0 ) {
    fprintf(stderr, "%s: xmalloc(), virtual memory exhausted\n", program_invocation_short_name);
    exit(EXIT_FAILURE);
  }
  return value;
}

/* eXtended realloc */
void *xrealloc(void *ptr, size_t size) {
  register void *value = realloc(ptr, size);
  if ( value == 0 ) {
    fprintf(stderr, "%s: xrealloc(), virtual memory exhausted\n", program_invocation_short_name);
    exit(EXIT_FAILURE);
  }
  return value;
}

/* save string into char pointer with automatic allocation */
char *savestring(const char *ptr) {
  size_t len = strlen(ptr);
  register char *value = (char *) xmalloc(len + 1);
  memcpy (value, ptr, len);
  value[len] = 0x00; /* NULL */
  return value;
}

/* eXtended regerror */
char *xregerror (int errcode, regex_t *crex) {
  size_t len = regerror(errcode, crex, NULL, 0);
  char *msg = xmalloc(len);
  regerror(errcode, crex, msg, len);
  return msg;
}

/* match with regular expression */
int m_rex (const char *strsrc, const char *pattern, const char *option) {
  regex_t *crex = xmalloc (sizeof(regex_t));
  unsigned int cflg = ( REG_EXTENDED | REG_NOSUB );
  int rval, i;

  int lenop = strlen ( option );

  for ( i = 0; i < lenop; i++ ) {
    switch ( option[i] ) {
      case 'i': cflg |= REG_ICASE;
                break;
      case 'n': cflg |= REG_NEWLINE;
    }
  }

  if ( ( rval = regcomp (crex, pattern, cflg) ) != 0 ) {
#ifdef REX_DEBUG
    fprintf(stderr, "m_rex->regcomp: %s\n", xregerror (rval, crex));
#endif
    free (crex);
    return 0;
  } else {
    if ( ( rval = regexec (crex, strsrc, 0, 0, 0) ) != 0 ) {
#ifdef REX_DEBUG
      fprintf(stderr, "m_rex->regexec: %s\n", xregerror (rval, crex));
#endif
      regfree (crex);
      free (crex);
      return 0;
    }
  }
  regfree (crex);
  free (crex);
  return 1;
}

/* search and replace private method for internal use only, not exported */
char **s_rex_pvt (regex_t *crex, const char *strsrc, const char *pattern, const char *replacement) {
  int rval;
  size_t mall = 1;
  regmatch_t mptr[mall];
  char *sreplace;
  char **aptr = xmalloc(sizeof(aptr) * 2);

  if ( ( rval = regexec (crex, strsrc, mall, mptr, 0) ) != 0 ) {
#ifdef REX_DEBUG
    fprintf(stderr, "s_rex_pvt->regexec: %s\n", xregerror (rval, crex));
#endif
    free (aptr);
    return NULL;
  }
#ifdef REX_DEBUG
  fprintf(stderr, "s_rex_pvt->regexec: comienza en %d y termina en %d\n", mptr->rm_so, mptr->rm_eo );
#endif
  sreplace = xmalloc(mptr->rm_so + strlen(replacement) + 1);
  memcpy(sreplace, strsrc, mptr->rm_so);
  sreplace[mptr->rm_so] = 0x00; /* NULL */
  strcat(sreplace, replacement);
  aptr[0] = xmalloc(strlen(sreplace) + 1);
  strcpy(aptr[0], sreplace);
  free(sreplace);
  strsrc += mptr->rm_eo;
  aptr[1] = xmalloc(strlen(strsrc) + 1);
  strcpy(aptr[1], strsrc);
#ifdef REX_DEBUG
  fprintf(stderr, "s_rex_pvt->aptr[0] = %d\n", strlen(aptr[0]));
  fprintf(stderr, "s_rex_pvt->aptr[1] = %d\n", strlen(aptr[1]));
#endif
  return aptr;
}

/* search and replace with regular expression */
char *s_rex (const char *strsrc, const char *pattern, const char *replacement, const char *option) {
  regex_t *crex = xmalloc (sizeof(regex_t));
  unsigned int cflg = ( REG_EXTENDED );
  int rval, i;
  int globbing = 0;
  char *source;
  char *sreplace = xmalloc(1);
  char **aptr;

  sreplace[0] = 0x00; /* NULL */

  int lenop = strlen ( option );

  for ( i = 0; i < lenop; i++ ) {
    switch ( option[i] ) {
      case 'i': cflg |= REG_ICASE;
                break;
      case 'n': cflg |= REG_NEWLINE;
                break;
      case 'g': globbing = 1;
    }
  }

  if ( ( rval = regcomp (crex, pattern, cflg) ) != 0 ) {
#ifdef REX_DEBUG
    fprintf(stderr, "s_rex->regcomp: %s\n", xregerror (rval, crex));
#endif
    free (sreplace);
    free (crex);
    return NULL;
  } else {
#ifdef REX_DEBUG
    fprintf(stderr, "s_rex->regcomp: %d subexpressions\n", crex->re_nsub );
#endif
    if ( globbing ) {
      source = xmalloc(strlen(strsrc) + 1);
      strcpy(source, strsrc);
      while( globbing ) {
#ifdef REX_DEBUG
        fprintf(stderr, " s_rex->source(g) = %d\n", strlen(source));
#endif
        aptr = s_rex_pvt (crex, source, pattern, replacement);
        if ( aptr == NULL ) {
          sreplace = xrealloc(sreplace, strlen(sreplace) + strlen(source) + 1);
          strcat(sreplace, source);
#ifdef REX_DEBUG
          fprintf(stderr, " s_rex->sreplace(g.null) = %d\n", strlen(sreplace));
#endif
          globbing = 0;
          free(source);
          regfree(crex);
          free(crex);
        } else if ( strlen(aptr[1]) == 0 ) {
          sreplace = xrealloc(sreplace, strlen(sreplace) + strlen(aptr[0]) + 1);
          strcat(sreplace, aptr[0]);
#ifdef REX_DEBUG
          fprintf(stderr, " s_rex->aptr[0](g.0) = %d\n", strlen(aptr[0]));
          fprintf(stderr, " s_rex->sreplace(g.0) = %d\n", strlen(sreplace));
#endif
          globbing = 0;
          free(source);
          free(aptr[0]);
          free(aptr[1]);
          free(aptr);
          regfree(crex);
          free(crex);
        } else {
          sreplace = xrealloc(sreplace, strlen(sreplace) + strlen(aptr[0]) + 1);
          strcat(sreplace, aptr[0]);
          source = xrealloc(source, strlen(aptr[1]) + 1);
          strcpy(source, aptr[1]);
#ifdef REX_DEBUG
          fprintf(stderr, " s_rex->sreplace(g) = %d\n", strlen(sreplace));
          fprintf(stderr, " s_rex->source(g) = %d\n", strlen(source));
#endif
          free(aptr[0]);
          free(aptr[1]);
          free(aptr);
        }
      }
    } else { /* no globbing */
      if ( ( aptr = s_rex_pvt (crex, strsrc, pattern, replacement) ) == NULL ) {
        regfree (crex);
        free (crex);
        sreplace = xrealloc(sreplace, strlen(strsrc) + 1);
        strcpy(sreplace, strsrc);
#ifdef REX_DEBUG
        fprintf(stderr, " s_rex->sreplace() = %d\n", strlen(sreplace));
#endif
      } else {
        regfree (crex);
        free (crex);
        sreplace = xrealloc(sreplace, strlen(aptr[0]) + strlen(aptr[1]) + 1);
        strcpy(sreplace, aptr[0]);
        strcat(sreplace, aptr[1]);
#ifdef REX_DEBUG
        fprintf(stderr, " s_rex->sreplace() = %d\n", strlen(sreplace));
#endif
        free(aptr[0]);
        free(aptr[1]);
        free(aptr);
      }
    }
  }
  return sreplace;
}

/* search and replace with regular expression plus automatic deallocation of source pointer */
char *as_rex (char *strsrc, const char *pattern, const char *replacement, const char *option) {
  char *strsrc_swp = strsrc;
  strsrc = s_rex (strsrc_swp, pattern, replacement, option);
  free (strsrc_swp);
  return strsrc;
}

/* yank/translator with regular expression */
char *y_rex (const char *strsrc, const char *pattern, const char *replacement, const char *option) {
  short complement = 0;
  short delete = 0;
  short squeze = 0;
  int es = strlen ( strsrc );
  int ep = strlen ( pattern );
  int er = strlen ( replacement );
  int lenop = strlen ( option );
  int i, j, y = 0;

  char *sreplace = xmalloc ( es + 1 );
  char lreplaced[2];
  lreplaced[1] = 0x00; /* NULL */

  for ( i = 0; i < lenop; i++ ) {
    switch ( option[i] ) {
      case 'c': complement = 1;
                break;
      case 'd': delete = 1;
                break;
      case 's': squeze = 1;
    }
  }

  for ( i = 0; i < es; i++ ) {
    short pointer = 0;
    short decrement = 0;
    lreplaced[0] = lreplaced[1];
    for ( j = 0; j < ep && strsrc[i] != pattern[j]; j++ );
    if ( ( j < ep && ! complement ) || ( j == ep && complement ) ) {
      if ( j >= er ) {
        pointer = er - 1;
        if ( delete ) decrement = 1;
      } else
        pointer = j;
      if ( pointer >= 0 ) {
        sreplace[y] = replacement[pointer];
        lreplaced[1] = replacement[pointer];
      } else {
        sreplace[y] = strsrc[i];
        lreplaced[1] = strsrc[i];
      }
    } else {
      sreplace[y] = strsrc[i];
      lreplaced[1] = 0x00; /* NULL */
    }

    if ( squeze && lreplaced[0] == lreplaced[1] && lreplaced[0] != 0x00 /* NULL */ && ! decrement )
      decrement = 1;

    y++;
    if ( decrement ) {
      decrement = 0;
      y--;
    }
  }
  sreplace[y] = 0x00; /* NULL */
  return sreplace;
}

/* yank/translator with regular expression plus automatic deallocation of source pointer */
char *ay_rex (char *strsrc, const char *pattern, const char *replacement, const char *option) {
  char *strsrc_swp = strsrc;
  strsrc = y_rex (strsrc_swp, pattern, replacement, option);
  free (strsrc_swp);
  return strsrc;
}

/* split string in pointer of strings using regular expression pattern like deliminter */
char **split (const char *pattern, const char *option, const char *strsrc, int *isa) {
  regex_t *crex = xmalloc (sizeof(regex_t));
  char *replacement = xmalloc(1);
  unsigned int cflg = ( REG_EXTENDED );
  int rval, i;
  int globbing = 1;
  char *source;
  char **aptr;
  char **strings = xmalloc(sizeof(strings));
  int is = 0;
  
  replacement[0] = 0x00; /* NULL */
  
  int lenop = strlen ( option );
  
  for ( i = 0; i < lenop; i++ ) {
    if ( option[i] == 'i' ) {
      cflg |= REG_ICASE;
    }
  }
  
  if ( ( rval = regcomp (crex, pattern, cflg) ) != 0 ) {
#ifdef REX_DEBUG
    fprintf(stderr, "split->regcomp: %s\n", xregerror (rval, crex));
#endif
    free (replacement);
    free (strings[is]);
    free (strings);
    free (crex);
    return NULL;
  } else {
#ifdef REX_DEBUG
    fprintf(stderr, "split->regcomp: %d subexpressions\n", crex->re_nsub);
#endif
    source = xmalloc(strlen(strsrc) + 1);
    strcpy (source, strsrc);
    while ( globbing ) {
#ifdef REX_DEBUG
      fprintf(stderr, " split->source(g) = %d\n", strlen(source));
#endif
      aptr = s_rex_pvt (crex, source, pattern, replacement);
      if ( aptr == NULL ) {
        strings[is] = xmalloc(strlen(source) + 1);
        strcpy(strings[is], source);
#ifdef REX_DEBUG
        fprintf(stderr, " split->strings[is](g.null) = %d\n", strlen(strings[is]));
#endif
        is++;
        globbing = 0;
        free(replacement);
        free(source);
        regfree(crex);
        free(crex);
      } else if ( strlen(aptr[1]) == 0 ) {
        strings[is] = xmalloc(strlen(aptr[0]) + 1);
        strcpy(strings[is], aptr[0]);
#ifdef REX_DEBUG
        fprintf(stderr, " split->aptr[0](g.0) = %d\n", strlen(aptr[0]));
        fprintf(stderr, " split->strings[is](g.0) = %d\n", strlen(strings[is]));
#endif
        is++;
        globbing = 0;
        free(replacement);
        free(source);
        free(aptr[0]);
        free(aptr[1]);
        free(aptr);
        regfree(crex);
        free(crex);
      } else {
        strings[is] = xmalloc(strlen(aptr[0]) + 1);
        strcpy(strings[is], aptr[0]);
        source = xrealloc(source, strlen(aptr[1]) + 1);
        strcpy(source, aptr[1]);
#ifdef REX_DEBUG
        fprintf(stderr, " split->strings[is](g) = %d\n", strlen(strings[is]));
        fprintf(stderr, " split->source(g) = %d\n", strlen(source));
#endif
        is++;
        strings = xrealloc(strings, sizeof(strings) * (is + 1));
        free(aptr[0]);
        free(aptr[1]);
        free(aptr);
      }
    }
  }
  *isa = is;
  return strings;
}

/* join multiple strings with first string */
char *join (char *between, ...) {
  va_list strings;
  char *result, *vas, *swap;
  va_start (strings, between);
  result = (char *) malloc (1);
  result[0] = 0x00; /* NULL */
  if ( ( vas = va_arg (strings, char *) ) != NULL ) {
    free(result);
    asprintf (&result, "%s", vas);
    while ( ( vas = va_arg(strings, char *) ) != NULL ) {
      asprintf (&swap, "%s", result);
      free (result);
      asprintf (&result, "%s%s%s", swap, between, vas);
      free (swap);
    }
  }
  va_end (strings);
  return result;
}

/* join double pointer strings with first string */
char *joindp (char *between, char **dpstring, int countdp) {
  char *result, *swap;
  int icountdp;
  result = (char *) malloc (1);
  result[0] = 0x00; /* NULL */
  if ( countdp <= 0 ) return result;
  free(result);
  asprintf (&result, "%s", dpstring[0]);
  for ( icountdp = 1; icountdp < countdp; icountdp++ ) {
    asprintf (&swap, "%s", result);
    free (result);
    asprintf (&result, "%s%s%s", swap, between, dpstring[icountdp]);
    free (swap);
  }
  return result;
}
