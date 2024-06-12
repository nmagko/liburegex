#
# samples Makefile
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
COPTS     = -I../
CCFLAGS   = -O2 -Wall $(COPTS) -o 
OBJECTDIR = .
PREFIX    = .
NESTEDOBJ = -luregex

all: sample

sample:
	$(CC) $(CCFLAGS) $(OBJECTDIR)/test $(OBJECTDIR)/test.c $(NESTEDOBJ)
	$(CC) $(CCFLAGS) $(OBJECTDIR)/ltest $(OBJECTDIR)/ltest.c $(NESTEDOBJ)

clean:
	rm -f *~
	rm -f *.o
	rm -f test ltest

