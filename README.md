### UNIQUE REGULAR EXPRESSIONS LIBRARY

### FUNCTIONS REFERENCE

This document is brief information about the uregex library functions.

### INTRODUCTION

Liburegex is a group of functions written in C for matching,
translating, splitting, replacing, and joining text. The functions are
defined in the header file `uregex.h'. It uses extended POSIX regular
expressions.

If you like `sed' or `perl' processing text then the uregex library is
for you.

What do you have to know? Just the basic sed scripting syntax to make
patterns for searching. It's an interface for manipulating regular
expressions without thinking about complex structures and many stages.

### 1. HANDLING REGULAR EXPRESSIONS WITH FUNCTIONS

int m_rex (const char *src, const char *pattern, const char *opt)

  This function is for pattern matching in the _src_ source pointer with
  options defined in the _opt_ string where each character represents an
  option.
  
  The possible _opt_ options are:
  
   i Case-insensitive searching.
   
   n Text processing line by line using carry left as delimiter.

  It returns 1 if there is a coincidence and 0 if not.
  
    if ( m_rex ( src_text, "^test", "in" ) ) printf ("founded\n");

char *s_rex (const char *src, const char *pattern, const char
*replacement, const char *opt)
	     
char *as_rex (char *src, const char *pattern, const char *replacement,
              const char *opt)

  It searches _pattern_ inside _src_ text pointer and replaces it with
  _replacement_ string.
  
  The possible _opt_ options are:
  
   i Case-insensitive searching.
   
   n Text processing line by line using carry left as delimiter.
   
   g All occurrences will be replaced.

  The s_rex function returns the new text-allocated pointer with
  substitutions by keeping _src_ unmodified.

    char *new_text = s_rex (src_text, "^\\w", "", "gn");
  
  The as_rex function returns the new text-allocated pointer and free
  _src_ pointer automatically. It's useful if you want to make
  substitutions directly.

    /* removing alphanumeric occurrences */
    
    src_text = as_rex (src_text, "\\w", "", "g");
    
char *y_rex (const char *src, const char *search, const char
*replacement, const char *opt);
	     
char *ay_rex (char *src, const char *search, const char *replacement,
              const char *opt);

  It translates character by character all occurrences found in _src_
  text pointer by _search_ list with corresponding characters in the
  _replacement_ list.

  The possible _opt_ options are:
  
   c It looks for characters in _search_ as a complement.
   
   d Any character specified in _search_ list will be changed by
     _replacement_ doing the inverse with option c.
	
   s All sequences translated to the same character are squashed down to
     one instance of the character.
  
  The y_rex function returns the new text-allocated pointer with
  translations by keeping _src_ unmodified.
  
    /* removing vowels */
    
    char *new_text1 = y_rex (src_text, "aeiou", "", "d");

    /* removing consonants */
    
    char *new_text2 = y_rex (src_text, "aeiou", "", "cd");
    
  The ay_rex function returns the new text-allocated pointer and free
  _src_ pointer automatically. It's useful if you want to make
  translations directly.
  
    /* lower case */
    
    src_text = ay_rex (src_text, "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
                       "abcdefghijklmnopqrstuvwxyz", "");
		       
    /* vowels to upper case */
    
    src_text = ay_rex (src_text, "aeiou", "AEIOU", "");
    
  There are some macros defined as tr_rex and atr_rex that are the same
  like y_rex and ay_rex respectively.
    
char **split (const char *pattern, const char *opt, const char *src, int
*isa)

  This function allocates and splits the _src_ pointer text into an
  array of texts (double pointer) and returns it. It has to be free with
  the free_double_pointer method defined in `aarray.h' when it is not
  useful anymore.
  
  Any text that fits _pattern_ will be taken as a delimiter. A delimiter
  may be longer than just one character and may be represented by
  extended and complex regular expressions in _pattern_.
  
  The possible _opt_ options are:
  
   i Case-insensitive delimiter searching.

  The full process of finding a delimiter always reads the whole text
  and updates the _isa_ parameter with the count of how many strings
  will be returned in the double-pointer.
  
    char **strarray = split ( ":", "i", text, &count_strarray );

char *join (char *separator, ...)

  This function puts together a list of strings into a single text
  joining strings by _separator_ returning the allocated text
  pointer. The last parameter has to be NULL.

    char *jointext = join (":", "user", "password", "uid", "gid",
                           "comment", "home", "shell", NULL);

char *joindp (char *separator, char **dpstrings, int countdp)

  This function puts together a list of strings in double-pointer
  _dpstrings_ into a single text joining strings by _separator_ and
  returning the allocated text pointer. The countdp saves how many
  strings of _dpstrings_ were joined.
  
    char *jointext = joindp (":", values, values_count);

### 2. THE ASSOCIATIVE ARRAY FUNCTIONS

It is a set of functions for handling associative array structure
(AArray). This structure associates two strings as a key and its
value. The structure and functions are defined in the header file
`aarray.h'.

struct AArray

  This is the data type used to represent a namespace of an associative
  array. It has the following members:
  
  char **asoc_array
  
    It is used internally and defines double pointers where even strings
    are the keys and odd strings are their values.

  long iaa
  
    It is used internally and represents the count of key-value pairs.
  
struct AArray *new_aa ()

  This method initializes and returns a new AArray structure pointer. It
  has to be free with the destroy_aa method when it isn't needed
  anymore.
  
    struct AArray *my_aa = new_aa();

int put_aa (struct AArray *addr, const char *key, const char *value)

  It appends a key-value pair into the initialized AArray _addr_. If the
  key already exists it overwrites the value with a new one. If the key
  does not exist it allocates a new entry with malloc and increments the
  iaa value into AArray _addr_ structure.
  
    put_aa (my_aa, "year", "2006");
    
    put_aa (my_aa, "0003", "it is a number");
    
    put_aa (my_aa, "fuck off", "bad word");

char *get_aa (const struct AArray *addr, const char *key)

  This function returns a string pointer to the value of the key. If the
  key doesn't exist and returns a NULL pointer.
  
    char *val = get_aa (my_aa, "year");

int delete_aa (struct AArray *addr, const char *key)

  It gets rid key-value pair from AArray _addr_ structure and returns 1
  if it is successful and 0 if not.
  
    delete_aa (my_aa, "year");

char **keys_aa (const struct AArray *addr, long *isa)

  It returns a list of strings containing all keys from the AArray
  _addr_ structure and updates the _isa_ value with how many keys were
  allocated in the double pointer. Double pointer has to be free with
  the free_double_pointer method when it is not needed anymore.
  
    long keys_count;
    
    char **keys = keys_aa (my_aa, &keys_count);

char **values_aa (const struct AArray *addr, long *isa)

  It returns a list of strings containing all values of the AArray
  _addr_ structure and updates the _isa_ value with how many keys were
  allocated in the double-pointer. Double pointer has to be free with
  the free_double_pointer method when it is not needed anymore.
  
    long values_count;
    
    char **values = values_aa (my_aa, &values_count);

void free_double_pointer (char **daddr, long isa)

  It gets rid of double pointer _daddr_ from memory as big as _isa_
  value.
  
    free_double_pointer (values, values_count);

void destroy_aa (struct AArray *addr)

  It gets rid of AArray _addr_ structure. The key-value pairs will be
  clear from memory.
  
    destroy_aa (my_aa);

### 3. UNIQUE CONFIGURATION READER

This is a function for reading simple configuration files that have a
syntax like `key = value' or `key: value' in plain text files. Blank
lines and characters after # ; or // will be ignored.

struct AArray *get_config (const char *path)

  This function is defined in the header file `uconfi.h'. It initializes
  and returns a pointer of the AArray structure containing the key-value
  pairs from the configuration file. This AArray structure has to be
  used with Associative Array Functions. The structure AArray has to be
  passed to destroy_aa function when it is not needed anymore.
  
    struct AArray *my_config = get_config ("/etc/test.conf");

### FREE DOCUMENTATION LICENSE

Copyright (C) 2006,2016 Victor Clodoaldo Salas Pumacayo.

  Permission is granted to copy, distribute, and/or modify this document
  under the terms of the GNU Free Documentation License, Version 1.3 or
  any later version published by the Free Software Foundation; with no
  Invariant Sections, no Front-Cover Texts, and no Back-Cover Texts. A
  copy of the license is included in the section entitled "GNU Free
  Documentation License".
