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
 * $Id: protocolubx.h 63729 2012-11-29 09:08:13Z michael.ammann $
 ******************************************************************************/
#ifndef __PROTOCOLUBX_H__
#define __PROTOCOLUBX_H__

#include "protocol.h"

// ubx specific type definitions used 
#ifdef _MSC_VER
typedef __int8					I1;		//!< signed 1 byte integer
typedef __int16					I2;		//!< signed 2 byte integer
typedef __int32					I4;		//!< signed 4 byte integer
typedef __int64					I8;		//!< signed 8 byte integer
typedef unsigned __int8			U1;		//!< unsigned 1 byte integer
typedef unsigned __int16		U2;		//!< unsigned 2 byte integer
typedef unsigned __int32		U4;		//!< unsigned 4 byte integer
typedef unsigned __int64		U8;		//!< unsigned 8 byte integer
#else
#include <stdint.h>
typedef int8_t					I1; 	//!< signed 1 byte integer
typedef int16_t					I2; 	//!< signed 2 byte integer
typedef int32_t					I4; 	//!< signed 4 byte integer
typedef int64_t					I8;		//!< signed 8 byte integer
typedef uint8_t					U1; 	//!< unsigned 1 byte integer
typedef uint16_t				U2; 	//!< unsigned 2 byte integer
typedef uint32_t				U4; 	//!< unsigned 4 byte integer
typedef uint64_t				U8; 	//!< unsigned 8 byte integer
#endif
typedef char					CH; 	//!< character

#ifdef SUPL_ENABLED	
#include "ubx_messageDef.h"
#endif

class CProtocolUBX : public CProtocol
{
public:
	enum { 
		UBX_CHAR_SYNC0 = 0xB5 /* 'µ' */,
		UBX_CHAR_SYNC1 = 0x62 /* 'b' */,
		UBX_FRM_SIZE   = 8,
		UBX_MAX_SIZE   = 2*1024
	};

	int Parse(unsigned char* pBuffer, int iSize);
	static int ParseFunc(const unsigned char* pBuffer, int iSize);
	void __drv_floatUsed Process(unsigned char* pBuffer, int iSize, CDatabase* pDatabase);
	PROTOCOL_t GetType(void) { return UBX; }
    unsigned int NewMsg(U1 classId, U1 msgId, const void* pPayload, unsigned int iPayloadSize, unsigned char **ppMsg) const;

protected:
	void __drv_floatUsed ProcessNavSol(   const unsigned char* pBuffer, int iSize, CDatabase* pDatabase) const;
	void __drv_floatUsed ProcessNavPvt(   const unsigned char* pBuffer, int iSize, CDatabase* pDatabase) const;
	void __drv_floatUsed ProcessNavSvInfo(const unsigned char* pBuffer, int iSize, CDatabase* pDatabase) const;
#ifdef SUPL_ENABLED		
	void __drv_floatUsed ProcessRxmMeas(const unsigned char* pBuffer, int iSize, CDatabase* pDatabase) const;
#endif
	
	void __drv_floatUsed CheckSetTtag(CDatabase* pDatabase, U4 ttag) const;
};

#endif //__PROTOCOLUBX_H__
