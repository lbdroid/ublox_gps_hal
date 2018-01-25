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
 * $Id: parserbuffer.h 63691 2012-11-28 13:54:07Z michael.ammann $
 ******************************************************************************/
#ifndef __PARSERBUFFER_H__
#define __PARSERBUFFER_H__

#include "protocol.h"

#ifndef PARSER_ASSERT
 #define PARSER_ASSERT(a)
#endif

class CParserBuffer
{
public:
	enum { WAIT = 0, NOT_FOUND = -1};
	
	CParserBuffer();
	virtual ~CParserBuffer();

	void Compact();
	bool Empty(CProtocol* &pProtocol, unsigned char*& pData, int& iSize);
	bool Parse(CProtocol* &pProtocol, unsigned char* &pData, int &iSize);
	bool Register(CProtocol* pProtocol);
	void RegisterUnknown(CProtocol* pProtocol);
	
	unsigned char* GetPointer();
	unsigned char* GetData();
	int GetSpace() const;
	int GetSize() const;
	void Append(int iSize);
	void Remove(int iSize);

protected:
	typedef struct RegisterInfo_s
	{
		CProtocol* pProtocol;
		struct RegisterInfo_s* pNext;
	} RegisterInfo;
	RegisterInfo* mpRoot;
	CProtocol* mpProtocolUnknown;
	unsigned char* mpBuffer;
	int miSize;
	int miUsed;
	int miDone;
};

inline unsigned char* CParserBuffer::GetPointer()
{
	return &mpBuffer[miUsed];
}

inline unsigned char* CParserBuffer::GetData()
{
	return &mpBuffer[miDone];
}
inline int CParserBuffer::GetSpace() const
{
	return miSize - miUsed;
}

inline int CParserBuffer::GetSize() const
{
	return miUsed - miDone;
}

inline void CParserBuffer::Append(int iSize)
{
	PARSER_ASSERT(iSize > 0);
	PARSER_ASSERT(miUsed + iSize <= miSize);
	miUsed += iSize;
}

inline void CParserBuffer::RegisterUnknown(CProtocol* pProtocol)
{
	mpProtocolUnknown = pProtocol;
}

#endif // __PARSERBUFFER_H__
