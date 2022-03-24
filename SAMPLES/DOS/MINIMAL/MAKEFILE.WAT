# Copyright (c) 1993 Argonaut Software Ltd. All rights reserved.
#
# $Id: makefile.wat 1.7 1995/01/03 15:57:17 sam Exp $
# $Locker:  $
#
# Makefile for Watcom C using WMAKE
#
TARGET=minimal.exe

BASE_DIR=..\..\..

# Fixed, Release, Register
LIB_TYPE=wxrr
LIB_TYPE_NB=wrr
LIB_TYPE_EX=4gwrr
BASED=FIXED

## Float, Release, Register
#LIB_TYPE=wfrr
#LIB_TYPE_NB=wrr
#LIB_TYPE_EX=4gwrr
#BASED=FLOAT

CC=wcc386
CFLAGS=-omaxnet -zp4 -5r -fp5&
 -DBASED_$(BASED)=1&
 -I$(BASE_DIR)\inc&
 -I$(BASE_DIR)\dosio

OBJS=&
	minimal.obj&

LIBS=&
	$(BASE_DIR)/lib/brfm$(LIB_TYPE).lib&
	$(BASE_DIR)/lib/brdb$(LIB_TYPE).lib&
	$(BASE_DIR)/lib/brmt$(LIB_TYPE).lib&
	$(BASE_DIR)/lib/brfw$(LIB_TYPE).lib&
	$(BASE_DIR)/lib/brpm$(LIB_TYPE_NB).lib&
	$(BASE_DIR)/lib/dosio$(LIB_TYPE_NB).lib&
	$(BASE_DIR)/lib/brst$(LIB_TYPE_NB).lib&
	$(BASE_DIR)/lib/hst$(LIB_TYPE_EX).lib&

# Default rules
#
# Cope with long command lines
#
.c.obj:
	set _ARGS=$(CFLAGS) $*.c
	$(CC) @_ARGS

$(TARGET): $(OBJS) wlink.rsp
	wlink @wlink.rsp

# Link response file
#
wlink.rsp: makefile.wat
	echo name $(TARGET) >wlink.rsp
	echo system dos4g >>wlink.rsp
	echo option dosseg,caseexact,quiet,stack=64k >>wlink.rsp
	for %i in ($(OBJS)) do echo file %i >>wlink.rsp
	for %i in ($(LIBS)) do echo library %i >>wlink.rsp

