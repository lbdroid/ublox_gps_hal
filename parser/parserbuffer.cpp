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
 * $Id: parserbuffer.cpp 63738 2012-11-29 09:29:25Z michael.ammann $
 ******************************************************************************/
#include "stdafx.h"

#include "parserbuffer.h"

static const int BUFFER_SIZE = 0x10000; // 64KB Buffer 


//--------------------------------------------------------------------------------
CParserBuffer::CParserBuffer()
{
	mpRoot = NULL;
	mpProtocolUnknown = NULL;
	miSize = BUFFER_SIZE; 
	//lint -e{1732,1733} new in constructor for class 'CParserBuffer' which has no assignment/copy operator
	mpBuffer = new unsigned char[(size_t)miSize];
	miUsed = 0;
	miDone = 0;
}

//--------------------------------------------------------------------------------
CParserBuffer::~CParserBuffer()
{
	// free the buffer
	if (mpBuffer != NULL)
	{
		delete [] mpBuffer;
	}
	// free all protocols
	RegisterInfo* pTemp = mpRoot;
	while (pTemp != NULL)
	{
		RegisterInfo* pNext = pTemp->pNext;
		delete pTemp;
		pTemp = pNext;
	}
	mpRoot = NULL;
	mpProtocolUnknown = NULL; // no need to free it.
}

//--------------------------------------------------------------------------------
bool CParserBuffer::Empty(CProtocol* &pProtocol, unsigned char*& pData, int& iSize)
{
	pProtocol = mpProtocolUnknown;
	pData = &mpBuffer[miDone];
	iSize = miUsed - miDone;
	return iSize > 0;
}

//--------------------------------------------------------------------------------
void CParserBuffer::Compact()
{
	// compact if possible and some data is consumed
	PARSER_ASSERT(miDone <= miUsed);
	if ((miUsed > 0) && (miDone > 0))
	{
		if (miDone < miUsed)
		{
			miUsed -= miDone;
			memmove(mpBuffer, &mpBuffer[miDone], (size_t)miUsed);
		}
		else 
		{
			miUsed = 0;
		}
		miDone = 0;
	}
}	

//--------------------------------------------------------------------------------

void CParserBuffer::Remove(int iSize)
{
	PARSER_ASSERT(iSize > 0);
	PARSER_ASSERT(miDone + iSize <= miUsed);
	miDone += iSize;
	if ((miDone > 0) && (miDone == miUsed))
	{
		miDone = 0;
		miUsed = 0;
	}
}

					
bool CParserBuffer::Parse(CProtocol* &pProtocol, unsigned char* &pData, int &iSize)
{
	// search the buffer 
	for (int iStart = miDone; (iStart < miUsed); iStart ++)
	{
		// Loop through the Protocols
		RegisterInfo* pItem = mpRoot;
		while (pItem != NULL)
		{
			// Is there a message at this point of the buffer with this protocol?
			int iTemp = pItem->pProtocol->Parse(&mpBuffer[iStart], miUsed-iStart);
			if (iTemp == WAIT)
			{
				// the parser would like to wait
			
				// wait / there is a chance that we get the full message later
				if ((miDone > 0) || (miUsed < BUFFER_SIZE))
				{
					return false;
				}

				// unknown message was detected before our wait
				if (iStart > miDone)
				{
					pData = &mpBuffer[miDone];
					iSize = iStart - miDone;
					pProtocol = mpProtocolUnknown;
					return true;
				}
				//TRACE(_T("CParserBuffer::Parse Buffer Overrun (skip one unsigned char)\n"));
				// this must be considered as a not found`!!
				iTemp = NOT_FOUND;
			}
			if (iTemp == NOT_FOUND)
				pItem = pItem->pNext;
			else 
			{
				// we found a message, but does it start at the beginning or 
				// is there a unknown message before ?
				if (iStart == miDone)
				{
					// its our message
					pData = &mpBuffer[iStart];
					iSize = iTemp;
					pProtocol = pItem->pProtocol;
				}
				else 
				{
					//TRACE(_T("CParserBuffer::Parse Unknown Message\n"));
					// unknown before 
					pData = &mpBuffer[miDone];
					iSize = iStart - miDone;
					pProtocol = mpProtocolUnknown;
				}
				return true;
			}
		}
	}
	// check for a buffer overrun 
	if ((miDone == 0) && (miUsed == BUFFER_SIZE))
	{
		// we have detected a buffer overrun
		//TRACE(_T("CParserBuffer::Parse Buffer Overrun (full)\n"));
		pData = &mpBuffer[0];
		iSize = miUsed;
		pProtocol = mpProtocolUnknown;
		return true;
	}
	else
	{
		return false;
	}
}

//--------------------------------------------------------------------------------
bool CParserBuffer::Register(CProtocol* pProtocol)
{
	RegisterInfo** pNode = NULL;
	if (mpRoot == NULL)
	{
		pNode = &mpRoot;
	}
	else
	{
		// append to the end
		pNode = &mpRoot;
		while (*pNode)
		{
			if ((*pNode)->pProtocol == pProtocol)
			{
				//printf("d'oh\n");
				return true;
			}
			pNode = &(*pNode)->pNext;
		}
	}
	RegisterInfo* pItem = ATLTRY(new RegisterInfo);
	//lint -e{774} Boolean within 'if' always evaluates to True
	if ( pItem )
	{
		pItem->pProtocol = pProtocol;
		pItem->pNext = NULL;
		*pNode = pItem;
		return true;
	}
	return false;
}

