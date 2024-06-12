/* Define uregex functions.
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
 *    Unique Regular Expressions:               <uregex.h>
 */

#define _GNU_SOURCE

#ifndef _UREGEX_H
#define _UREGEX_H

/* Interface functions for generic tasks (allocation) */

extern void *xmalloc (size_t);

extern void *xrealloc (void *, size_t);

extern char *savestring (const char *);

/* Interface functions for regular expressions tasks */

extern int m_rex (const char *, const char *, const char *);

extern char *s_rex (const char *, const char *, const char *, const char *);
/* as_rex make an automatic free to first function parameter */
extern char *as_rex (char *, const char *, const char *, const char *);

extern char *y_rex (const char *, const char *, const char *, const char *);
/* ay_rex make an automatic free to first function parameter */
extern char *ay_rex (char *, const char *, const char *, const char *);

/* Macro-extended functions for translations tasks */

#define tr_rex(s1,s2,s3,s4) y_rex(s1,s2,s3,s4)

#define atr_rex(s1,s2,s3,s4) ay_rex(s1,s2,s3,s4)

/* Interface of extended functions for string's tasks */

extern char **split (const char *, const char *, const char *, int *);

extern char *join (char *, ...);

extern char *joindp (char *, char **, int);

#endif /* !_UREGEX_H */

