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
/*!
  \file
  \brief  Xtra interface implementation

*/
/*******************************************************************************
 * $Id: ubx_xtraIf.h 63615 2012-11-27 10:12:42Z andrea.foni $
 ******************************************************************************/
#ifndef __UBX_XTRAIF_H__
#define __UBX_XTRAIF_H__

#include "std_inc.h"

class CXtraIf
{
public:
    CXtraIf();
	static const void *getIf() { return &s_interface; }
	// callbacks
	static void requestDownload(void);

private:
	// interface
 	static int init(GpsXtraCallbacks* callbacks);
	static int injectData(char* data, int length);
	
	// variables
	static const GpsXtraInterface s_interface;
	GpsXtraCallbacks m_callbacks;
    bool m_ready;
};

#endif /* __UBX_XTRAIF_H__ */
