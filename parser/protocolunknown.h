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
/*!
  \file
  \brief 
*/
/*******************************************************************************
 * $Id: protocolunknown.h 63667 2012-11-28 12:01:32Z andrea.foni $
 ******************************************************************************/
#ifndef __PROTOCOLUNKNOWN_H__
#define __PROTOCOLUNKNOWN_H__

#include "protocol.h"


class CProtocolUnknown : public CProtocol
{
public:
	int Parse(unsigned char* pBuffer, int iSize);
	void Process(unsigned char* pBuffer, int iSize, CDatabase* pDatabase);
	PROTOCOL_t GetType(void) { return UNKNOWN; }
};


#endif //__PROTOCOLUNKNOWN_H__
