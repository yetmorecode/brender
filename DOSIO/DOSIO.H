/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: dosio.h 2.2 1995/09/28 13:54:55 sam Exp $
 * $Locker:  $
 *
 * Definitions for DOS I/O library
 */
#ifndef _DOSIO_H_
#define _DOSIO_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * DosClockRead Ticks per second
 */
#define BR_DOS_CLOCK_RATE 1192755L

/*
 * Mouse button mask
 */
#define	BR_MSM_BUTTONL 1
#define	BR_MSM_BUTTONR 2
#define	BR_MSM_BUTTONM 4

#ifdef __cplusplus
};
#endif

#ifndef EVENTQ_H_
#include "eventq.h"
#endif

#ifndef _KEYBOARD_H_
#include "keyboard.h"
#endif

#ifndef _DOS_P_H_
#include "dos_p.h"
#endif

#ifndef _DOS_X_H_
#include "dos_x.h"
#endif

#endif
