#
# UREGEX Makefile
# Copyright (C) 2005-2016  Victor C. Salas P. (aka nmag) <nmagko@gmail.com>
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# How can make install library when I'm not root?
# LD_INCLUDE_PATH=../../j/s/include LD_LIBRARY_PATH=../../j/s/sbin make install
#

CC        = gcc
COPTS     = 
CCFLAGS   = -O2 -fPIC -Wall $(COPTS) -c 
OBJECTDIR = .
PREFIX    = .
INSTALL   = install
MAKE      = make
LN        = ln
RM        = rm
MKARCH    = $(shell uname -m)
MKUSER    = $(shell whoami)
MKUID     = $(shell id -u)
MKGID     = $(shell id -g)

ifeq ($(MKUSER), root)
 LDCONFIG = ldconfig
 INCDIR  = /usr/include
 ifeq ($(MKARCH), x86_64)
  LIBDIR  = $(shell if [ -d /usr/lib64 ]; then echo /usr/lib64; else echo /usr/lib; fi)
 else
  LIBDIR  = /usr/lib
 endif
else
 ifndef LD_INCLUDE_PATH
  $(error You are not root, you have to set `LD_INCLUDE_PATH' include target)
 else
  INCDIR  = $(LD_INCLUDE_PATH)
 endif
 ifndef LD_LIBRARY_PATH
  $(error You are not root, you have to set `LD_LIBRARY_PATH' library target)
 else
  LIBDIR  = $(LD_LIBRARY_PATH)
 endif
 LDCONFIG = if [ ! -e liburegex.so.0 ] && [ -e liburegex.so.0.0.1 ]; then $(LN) -s liburegex.so.0.0.1 liburegex.so.0; fi
endif

all: lib

lib:
	$(CC) $(CCFLAGS) $(OBJECTDIR)/aarray.c
	$(CC) $(CCFLAGS) $(OBJECTDIR)/uregex.c
	$(CC) $(CCFLAGS) $(OBJECTDIR)/uconfi.c
	$(CC) -shared -o liburegex.so.0.0.1 -Wl,-soname,liburegex.so.0 uregex.o aarray.o uconfi.o

clean:
	$(RM) -f *~
	$(RM) -f *.o
	$(RM) -f lib*.so.*
	cd samples && $(MAKE) clean

install:
	$(INSTALL) -o $(MKUID) -g $(MKGID) -m 0644 liburegex.so.0.0.1 $(LIBDIR)/
	$(INSTALL) -o $(MKUID) -g $(MKGID) -m 0644 uregex.h $(INCDIR)/
	$(INSTALL) -o $(MKUID) -g $(MKGID) -m 0644 uconfi.h $(INCDIR)/
	$(INSTALL) -o $(MKUID) -g $(MKGID) -m 0644 aarray.h $(INCDIR)/
	cd $(LIBDIR) && $(LDCONFIG) && $(LN) -s liburegex.so.0.0.1 liburegex.so

uninstall:
	$(RM) -f $(LIBDIR)/liburegex.so*
	$(RM) -f $(INCDIR)/aarray.h
	$(RM) -f $(INCDIR)/uconfi.h
	$(RM) -f $(INCDIR)/uregex.h
	cd $(LIBDIR) &&	$(LDCONFIG)

sample:
	cd samples && $(MAKE)
