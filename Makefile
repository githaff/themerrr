# Themerrr - Theme Rereader Utility
# Copyright (C) 2012 Dmirty Lavnikevich
# Contact: haff@midgard.by
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

# This software uses parts of Qt and GTK+ source code which both are
# availabe under LGPL v.2 Their source code can be found at
# http://qt-project.org/ and http://www.gtk.org/ correspondingly.


CC=gcc
CFLAGS=-Wall -lX11

INCLUDE_DIR=include
SRC_DIR=src
BIN_DIR=bin


${BIN_DIR}/themerrr : ${BIN_DIR}/main.o ${BIN_DIR}/gtk.o ${BIN_DIR}/qt.o
	mkdir -p ${BIN_DIR}
	${CC} ${CFLAGS} -o ${BIN_DIR}/themerrr ${BIN_DIR}/main.o ${BIN_DIR}/gtk.o ${BIN_DIR}/qt.o

${BIN_DIR}/main.o : ${INCLUDE_DIR}/themerrr.h ${SRC_DIR}/main.c
	mkdir -p ${BIN_DIR}
	${CC} ${CFLAGS} -c -o ${BIN_DIR}/main.o ${SRC_DIR}/main.c -I${INCLUDE_DIR}

${BIN_DIR}/gtk.o : ${INCLUDE_DIR}/themerrr.h ${SRC_DIR}/gtk.c
	mkdir -p ${BIN_DIR}
	${CC} ${CFLAGS} -c -o ${BIN_DIR}/gtk.o ${SRC_DIR}/gtk.c -I${INCLUDE_DIR}

${BIN_DIR}/qt.o : ${INCLUDE_DIR}/themerrr.h ${SRC_DIR}/qt.c
	mkdir -p ${BIN_DIR}
	${CC} ${CFLAGS} -c -o ${BIN_DIR}/qt.o ${SRC_DIR}/qt.c -I${INCLUDE_DIR}


.PHONY : clean
clean : 
	rm -rf ${BIN_DIR}
