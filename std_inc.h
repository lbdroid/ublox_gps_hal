/*******************************************************************************
 *
 * Copyright (C) u-blox AG 
 * u-blox AG, Thalwil, Switzerland
 *
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose without fee is hereby granted, provided that this entire notice
 * is included in all copies of any software which is or includes a copy
 * or modification of this software and in all copies of the supporting
 * documentation for such software.
 *
 * THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY. IN PARTICULAR, NEITHER THE AUTHOR NOR U-BLOX MAKES ANY
 * REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
 * OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
 *
 *******************************************************************************
 *
 * Project: PE_ANS
 *
 ******************************************************************************/
// $Id: std_inc.h 64754 2013-01-11 15:12:44Z michael.ammann $

#ifndef __STD_INC_H__
#define __STD_INC_H__

#include <hardware/gps.h>
#include "std_types.h"
#include "ubx_log.h"

#if (PLATFORM_SDK_VERSION > 8 /* >2.2 */)
 #define IF_ANDROID23(...) __VA_ARGS__
#else
 #define IF_ANDROID23(...) 
#endif

#endif // __STD_INC_H__
