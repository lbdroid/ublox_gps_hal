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
 * $Id: protocolnmea.h 63728 2012-11-29 09:00:48Z michael.ammann $
 ******************************************************************************/
#ifndef __PROTOCOLNMEA_H__
#define __PROTOCOLNMEA_H__

#include "protocol.h"

class CProtocolNMEA : public CProtocol
{
public:
	enum { 
		NMEA_CHAR_SYNC = 36 /* '$' */,
		NMEA_MAX_SIZE  = 82 /* this is the limit of the NMEA standard */,
		PUBX_MAX_SIZE  = 512
	};

	int Parse(unsigned char* pBuffer, int iSize); 
	static int ParseFunc(const unsigned char* pBuffer, int iSize); 
	void __drv_floatUsed Process(unsigned char* pBuffer, int iSize, CDatabase* pDatabase);
	PROTOCOL_t GetType(void) { return NMEA; }

protected:
	void __drv_floatUsed ProcessGBS(unsigned char* pBuffer, int iSize, CDatabase* pDatabase) const;
	void __drv_floatUsed ProcessGGA(unsigned char* pBuffer, int iSize, CDatabase* pDatabase) const;
	void __drv_floatUsed ProcessGLL(unsigned char* pBuffer, int iSize, CDatabase* pDatabase) const;
	void __drv_floatUsed ProcessGNS(unsigned char* pBuffer, int iSize, CDatabase* pDatabase) const;
	void __drv_floatUsed ProcessGRS(unsigned char* pBuffer, int iSize, CDatabase* pDatabase) const;
	void __drv_floatUsed ProcessGSA(unsigned char* pBuffer, int iSize, CDatabase* pDatabase) const;
	void __drv_floatUsed ProcessGST(unsigned char* pBuffer, int iSize, CDatabase* pDatabase) const;
	void __drv_floatUsed ProcessGSV(unsigned char* pBuffer, int iSize, CDatabase* pDatabase) const;
	void __drv_floatUsed ProcessRMC(unsigned char* pBuffer, int iSize, CDatabase* pDatabase) const;
	void __drv_floatUsed ProcessVTG(unsigned char* pBuffer, int iSize, CDatabase* pDatabase) const;
	void __drv_floatUsed ProcessZDA(unsigned char* pBuffer, int iSize, CDatabase* pDatabase) const;
	
	static __drv_floatUsed void SetLatLon(unsigned char* pBuffer, int iSize, CDatabase* pDatabase, int ix);
	static void SetStatus(unsigned char* pBuffer, int iSize, CDatabase* pDatabase, int ix);
	static void SetModeIndicator(unsigned char* pBuffer, int iSize, CDatabase* pDatabase, int ix);
	static __drv_floatUsed void CheckSetTime(unsigned char* pBuffer, int iSize, CDatabase* pDatabase, int ix);

	static int GetItemCount(unsigned char* pBuffer, int iSize);
	static const char* GetItem(int iIndex, unsigned char* pBuffer, int iSize);
	static char* FindPos(int iIndex, char* pStart, const char* pEnd);
	static bool __drv_floatUsed GetItem(int iIndex, unsigned char* pBuffer, int iSize, double& dValue);
	static bool GetItem(int iIndex, unsigned char* pBuffer, int iSize, int& iValue, int iBase = 10);
	static bool GetItem(int iIndex, unsigned char* pBuffer, int iSize, char& ch);
	static bool MatchChar(const char* string, char ch, int& i);
	static __drv_floatUsed double Limit360(double);
	static double CalcAngle(double d);
	static bool __drv_floatUsed CalcLon(char ch, double& d);
	static bool __drv_floatUsed CalcLat(char ch, double& d);
	static bool CalcTime(double d, int& h, int& m, double& s);
	static bool CalcDate(int d, int& day, int& mon, int& yr);
};

#endif //__PROTOCOLNMEA_H__
