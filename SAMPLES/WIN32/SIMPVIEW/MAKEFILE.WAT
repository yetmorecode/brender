# Copyright (c) 1993 Argonaut Software Ltd. All rights reserved.
#
# $Id: makefile.wat 1.1 1995/01/23 16:45:43 sam Exp $
# $Locker:  $
#
# Build WIN32 Application with Watcom C/C++ 10
#
# NB: Make sure the INCLUDE path references "...\watcom\h\nt" not
# "...\watcom\h\win" 
#
TARGET=simpview.exe

BASE_DIR=p:\brender\v1_2\test_a

# Watcom, Fixed, Release
#
LIB_TYPE=wxrr
LIB_TYPE_NB=wrr
LIB_TYPE_EX=wnwrr
BASED=FIXED

CC=wcc386
CFLAGS=-omaxnet -zp4 -5r &
 -bt=nt&
 -DBASED_$(BASED)=1&
 -DNDEBUG&
 -D_WINDOWS&
 -DWIN32&
 -I$(BASE_DIR)\inc

# Object files
#
OBJS_C=&
 buffer.obj&
 dispatch.obj&
 app.obj&
 world.obj&
 main.obj&

OBJS=$(OBJS_C) $(OBJS_ASM)

# Link libraries
#
LIBS=&
	$(BASE_DIR)/lib/brfw$(LIB_TYPE).lib&
	$(BASE_DIR)/lib/brfm$(LIB_TYPE).lib&
	$(BASE_DIR)/lib/brdb$(LIB_TYPE).lib&
	$(BASE_DIR)/lib/brmt$(LIB_TYPE).lib&
	$(BASE_DIR)/lib/brpm$(LIB_TYPE_NB).lib&
	$(BASE_DIR)/lib/dosio$(LIB_TYPE_NB).lib&
	$(BASE_DIR)/lib/brst$(LIB_TYPE_NB).lib&
	$(BASE_DIR)/lib/hst$(LIB_TYPE_EX).lib&

# Default rules
#
# Cope with long command lines
#
.c.obj:
	set _ARGS=$(CFLAGS) -fo=$@ $<
	$(CC) @_ARGS

.SUFFIXES: .res .rc

.rc.res:
	wrc -bt=nt -r -DNDEBUG -fo=$@ $<

$(TARGET): $(OBJS) wlink.rsp app.res
	wlink @wlink.rsp
	wrc app.res $(TARGET)

# Link response file
#
wlink.rsp: makefile.wat
	echo name $(TARGET) >wlink.rsp
	echo system nt_win >>wlink.rsp
	echo option caseexact,stack=64k >>wlink.rsp
	for %i in ($(OBJS)) do echo file %i >>wlink.rsp
	for %i in ($(LIBS)) do echo library %i >>wlink.rsp

# Dependencies
#
app.obj : app.h dispatch.h world.h buffer.h resource.h app.c
buffer.obj : resource.h buffer.h app.h buffer.c
dispatch.obj : dispatch.h dispatch.c
main.obj : app.h dispatch.h world.h buffer.h resource.h main.c
world.obj : dispatch.h world.h resource.h mattab.h buffer.h world.c

