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
 ******************************************************************************/
#ifndef __STDAFX_H__
#define __STDAFX_H__

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//lint -save -e451 Header file repeatedly included but does not have a standard include guard
#include <stdarg.h>
//lint -restore

#ifndef UNIX_API
	#include <tchar.h>
	#include <crtdbg.h>
	#include "wtypes.h"
	#define PARSER_ASSERT(a) _ASSERT(a)
#else
	#include <time.h>
	
	#define __drv_floatUsed
	#define _T(a) a
	#define _TCHAR char
	#define _tfopen_s(a,b,c) (!(*(a) = fopen(b,c)))
	#define _tmain main
#endif

#ifndef ATLTRY
	#define ATLTRY(a) a
#endif


// Reference additional headers your program requires here

#endif // __STDAFX_H__

