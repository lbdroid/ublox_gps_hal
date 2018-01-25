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
 * $Id: database.h 65048 2013-01-21 10:15:08Z jon.bowern $
 ******************************************************************************/
#ifndef __DATABASE_H__
#define __DATABASE_H__

#include "stdafx.h"
#include "datavar.h"

class CDatabase
{
public:
	
	enum 
	{ 
		MAX_SATELLITES_USED    = 28,		// GPS + GLONASS + SBAS
		MAX_SATELLITES_IN_VIEW = 36			// GPS + GLONASS + SBAS
	};

	typedef enum STATE_e
	{
		STATE_NO_DATA,
		STATE_REACQ,
		STATE_READY
	} STATE_t;

	typedef enum DATA_e
	{
		// these fields are taken from sensor api definitions
		//DATA_ADDRESS1
		//DATA_ADDRESS2
		//DATA_ALTITUDE_ANTENNA_SEALEVEL_METERS ???
		DATA_ALTITUDE_ELLIPSOID_ERROR_METERS,
		DATA_ALTITUDE_ELLIPSOID_METERS,
		DATA_ALTITUDE_SEALEVEL_ERROR_METERS,
		DATA_ALTITUDE_SEALEVEL_METERS,
		//DATA_CITY
		//DATA_COUNTRY_REGION
		DATA_DGPS_DATA_AGE,
		DATA_DIFFERENTIAL_REFERENCE_STATION_ID,
		DATA_ERROR_RADIUS_METERS,
		DATA_FIX_QUALITY,
		DATA_FIX_TYPE,
		DATA_GEOIDAL_SEPARATION,
		DATA_GPS_OPERATION_MODE,
		DATA_GPS_SELECTION_MODE,
		DATA_GPS_STATUS,
		DATA_HORIZONAL_DILUTION_OF_PRECISION,
		DATA_LATITUDE_DEGREES,
		DATA_LONGITUDE_DEGREES,
		DATA_MAGNETIC_HEADING_DEGREES,
		DATA_MAGNETIC_VARIATION,
		//DATA_NMEA_SENTENCE
		DATA_POSITION_DILUTION_OF_PRECISION,
		//DATA_POSTALCODE
		DATA_SATELLITES_IN_VIEW,
		DATA_SATELLITES_IN_VIEW_AZIMUTH,
#define DATA_SATELLITES_IN_VIEW_AZIMUTH_(ix)	(CDatabase::DATA_t)(CDatabase::DATA_SATELLITES_IN_VIEW_AZIMUTH + ix)
		DATA_SATELLITES_IN_VIEW_AZIMUTH_X		= MAX_SATELLITES_IN_VIEW + DATA_SATELLITES_IN_VIEW_AZIMUTH,
		DATA_SATELLITES_IN_VIEW_ELEVATION,
#define DATA_SATELLITES_IN_VIEW_ELEVATION_(ix)	(CDatabase::DATA_t)(CDatabase::DATA_SATELLITES_IN_VIEW_ELEVATION + ix)
		DATA_SATELLITES_IN_VIEW_ELEVATION_X		= MAX_SATELLITES_IN_VIEW + DATA_SATELLITES_IN_VIEW_ELEVATION,
//		DATA_SATELLITES_IN_VIEW_ID,
//#define DATA_SATELLITES_IN_VIEW_ID_(ix)			(CDatabase::DATA_t)(CDatabase::DATA_SATELLITES_IN_VIEW_ID + ix)
//		DATA_SATELLITES_IN_VIEW_ID_X			= MAX_SATELLITES_IN_VIEW + DATA_SATELLITES_IN_VIEW_ID,
		DATA_SATELLITES_IN_VIEW_PRNS,
#define DATA_SATELLITES_IN_VIEW_PRNS_(ix)		(CDatabase::DATA_t)(CDatabase::DATA_SATELLITES_IN_VIEW_PRNS + ix)
		DATA_SATELLITES_IN_VIEW_PRNS_X			= MAX_SATELLITES_IN_VIEW + DATA_SATELLITES_IN_VIEW_PRNS,
		DATA_SATELLITES_IN_VIEW_STN_RATIO,
#define DATA_SATELLITES_IN_VIEW_STN_RATIO_(ix)	(CDatabase::DATA_t)(CDatabase::DATA_SATELLITES_IN_VIEW_STN_RATIO + ix)
		DATA_SATELLITES_IN_VIEW_STN_RATIO_X		= MAX_SATELLITES_IN_VIEW + DATA_SATELLITES_IN_VIEW_STN_RATIO,
		DATA_SATELLITES_USED_COUNT,
		DATA_SATELLITES_USED_PRNS,
#define DATA_SATELLITES_USED_PRNS_(ix)			(CDatabase::DATA_t)(CDatabase::DATA_SATELLITES_USED_PRNS + ix)
		DATA_SATELLITES_USED_PRNS_X				= MAX_SATELLITES_USED	 + DATA_SATELLITES_USED_PRNS,
		DATA_SPEED_KNOTS,
		//DATA_STATE_PROVINCE
		DATA_TRUE_HEADING_DEGREES,
		DATA_VERTICAL_DILUTION_OF_PRECISION,

		// internal fields 
		DATA_DATE_YEAR, 
		DATA_DATE_MONTH,
		DATA_DATE_DAY,  
		DATA_TIME_HOUR,
		DATA_TIME_MINUTE,
		DATA_TIME_SECOND,
		DATA_UTC_TIMESTAMP,
		DATA_LOCAL_TIMESTAMP, 
		DATA_LOCALX_TIMESTAMP, 
		
		// proprietary ubx specific fields
		DATA_UBX_TTAG,
		DATA_UBX_GPSTIME_TOW,
		DATA_UBX_GPSTIME_WEEK,
		DATA_UBX_GPSFIXOK,
		DATA_UBX_GPSFIX,
		DATA_UBX_DGPS,
		DATA_UBX_POSITION_ECEF_X,
		DATA_UBX_POSITION_ECEF_Y,
		DATA_UBX_POSITION_ECEF_Z,
		DATA_UBX_POSITION_ECEF_ACCURACY,
		DATA_UBX_VELOCITY_ECEF_VX,
		DATA_UBX_VELOCITY_ECEF_VY,
		DATA_UBX_VELOCITY_ECEF_VZ,
		DATA_UBX_VELOCITY_ECEF_ACCURACY,
		DATA_UBX_SATELLITES_IN_VIEW_ORB_STA,
#define DATA_UBX_SATELLITES_IN_VIEW_ORB_STA_(ix)	(CDatabase::DATA_t)(CDatabase::DATA_UBX_SATELLITES_IN_VIEW_ORB_STA + ix)
		DATA_UBX_SATELLITES_IN_VIEW_ORB_STA_X		= MAX_SATELLITES_IN_VIEW + DATA_UBX_SATELLITES_IN_VIEW_ORB_STA,

#ifdef SUPL_ENABLED
		// SUPL MS-ASSIST properties
		DATA_UBX_GNSS_TOW,
		DATA_UBX_GNSS_DOP_CENTER,
		DATA_UBX_SATELLITES_IN_MEAS_COUNT,
		
		DATA_UBX_SATELLITES_IN_MEAS,
#define DATA_UBX_SATELLITES_IN_MEAS_(ix)			(CDatabase::DATA_t)(CDatabase::DATA_UBX_SATELLITES_IN_MEAS + ix)
		DATA_UBX_SATELLITES_IN_MEAS_X				= MAX_SATELLITES_IN_VIEW + DATA_UBX_SATELLITES_IN_MEAS,
		
		DATA_UBX_SATELLITES_IN_MEAS_CNO,
#define DATA_UBX_SATELLITES_IN_MEAS_CNO_(ix)		(CDatabase::DATA_t)(CDatabase::DATA_UBX_SATELLITES_IN_MEAS_CNO + ix)
		DATA_UBX_SATELLITES_IN_MEAS_CNO_X			= MAX_SATELLITES_IN_VIEW + DATA_UBX_SATELLITES_IN_MEAS_CNO,		
		
		DATA_UBX_SATELLITES_IN_MEAS_PRRMS,
#define DATA_UBX_SATELLITES_IN_MEAS_PRRMS_(ix)		(CDatabase::DATA_t)(CDatabase::DATA_UBX_SATELLITES_IN_MEAS_PRRMS + ix)
		DATA_UBX_SATELLITES_IN_MEAS_PRRMS_X			= MAX_SATELLITES_IN_VIEW + DATA_UBX_SATELLITES_IN_MEAS_PRRMS,
		
		DATA_UBX_SATELLITES_IN_MEAS_MULTIPATH_IND,
#define DATA_UBX_SATELLITES_IN_MEAS_MULTIPATH_IND_(ix)	(CDatabase::DATA_t)(CDatabase::DATA_UBX_SATELLITES_IN_MEAS_MULTIPATH_IND + ix)
		DATA_UBX_SATELLITES_IN_MEAS_MULTIPATH_IND_X 	= MAX_SATELLITES_IN_VIEW + DATA_UBX_SATELLITES_IN_MEAS_MULTIPATH_IND,
		
 		DATA_UBX_SATELLITES_IN_MEAS_REL_CODEPHASE,
#define DATA_UBX_SATELLITES_IN_MEAS_REL_CODEPHASE_(ix)	(CDatabase::DATA_t)(CDatabase::DATA_UBX_SATELLITES_IN_MEAS_REL_CODEPHASE + ix)
		DATA_UBX_SATELLITES_IN_MEAS_REL_CODEPHASE_X 	= MAX_SATELLITES_IN_VIEW + DATA_UBX_SATELLITES_IN_MEAS_REL_CODEPHASE,
     		
  		DATA_UBX_SATELLITES_IN_MEAS_DOPPLER,
#define DATA_UBX_SATELLITES_IN_MEAS_DOPPLER_(ix)		(CDatabase::DATA_t)(CDatabase::DATA_UBX_SATELLITES_IN_MEAS_DOPPLER + ix)
		DATA_UBX_SATELLITES_IN_MEAS_DOPPLER_X 			= MAX_SATELLITES_IN_VIEW + DATA_UBX_SATELLITES_IN_MEAS_DOPPLER,		
#endif

		// the following fields may not be set by the parser 
		DATA_PARSE,	_DATA_PARSE_X = DATA_PARSE - 1,

		// commit timestamp
		DATA_COMMIT_TIMESTAMP,
		// last good fields
		DATA_LASTGOOD_LATITUDE_DEGREES,
		DATA_LASTGOOD_LONGITUDE_DEGREES,
		DATA_LASTGOOD_ERROR_RADIUS_METERS,
		DATA_LASTGOOD_ALTITUDE_ELLIPSOID_ERROR_METERS,
		DATA_LASTGOOD_ALTITUDE_ELLIPSOID_METERS,
		DATA_LASTGOOD_TIMESTAMP,

		// total number of fields 
		DATA_NUM
	} DATA_t;

	CDatabase(void);
	virtual ~CDatabase(void) {}
	//lint -e{1735} Virtual function has default parameter
	virtual CDatabase::STATE_t Commit(bool bClear = true);
	//lint -sem(CDatabase::Reset,initializer)
	virtual void AddMessage(unsigned char* pData, int iSize);
	void Reset(void);
	__drv_floatUsed bool CheckTime(int hour, int minute, double seconds) const;
	__drv_floatUsed bool CheckDate(int year, int month,  int day) const;
	static __drv_floatUsed double Degrees360(double val);
	static int ConvertPrn2NmeaSvid(int svid);
	
/*	template<typename T>
	__drv_floatUsed bool Get(DATA_t data, T &v)
	{
		if (data < DATA_NUM)
			return varO[data].Get(v);
		return false;
	}*/

	template<typename T>
	__drv_floatUsed void Set(DATA_t data, T v)
	{
		if (data < DATA_PARSE)
		{
#if defined(_DEBUG) && 1
			if (!varN[data].Check(v))
			{
				double d;
				varN[data].Get(d);
				//printf("Set(%d, %f) -> Overwrite %f\n", v, d);
			}
#endif
			varN[data].Set(v);
		}
	}

	template<typename T>
	__drv_floatUsed bool Check(DATA_t data, T v) const
	{
		if (data < DATA_PARSE)
			return varN[data].Check(v);
		return true;
	}

	typedef enum 
	{
		MSG_NMEA_GBS,
		MSG_NMEA_GGA,
		MSG_NMEA_GLL,
		MSG_NMEA_GNS,
		MSG_NMEA_GRS,
		MSG_NMEA_GSA,
		MSG_NMEA_GST,
		MSG_NMEA_GSV_1,
		MSG_NMEA_RMC,
		MSG_NMEA_VTG,
		MSG_NMEA_ZDA,
		MSG_UBX_NAVSOL,
		MSG_UBX_NAVPVT,
		MSG_UBX_SVINFO,
		// number of fields 
		MSG_NUM
	} MSG_t;

	/** Use this function from a message to make sure that a message 
		is only inserted once in the db. A new epoch will be automatically 
		created.
	*/
	__drv_floatUsed void MsgOnce(MSG_t msg);

private:
	__drv_floatUsed double CalcLeapSeconds(double dUtcSec) const;
protected:
	__drv_floatUsed void CompleteTimestamp(void);
	__drv_floatUsed void CompletePosition(void);
	__drv_floatUsed void CompleteVelocity(void);
	__drv_floatUsed void CompleteAltitude(void);
	__drv_floatUsed void CompleteHeading(void);
	
	virtual bool GetCurrentTimestamp(TIMESTAMP& /*ft*/)				{ return false; }
	
	virtual __drv_floatUsed int Printf(const char* /*pFmt*/, ...)	{ return 0; }
	void __drv_floatUsed Dump(const CVar* pVar);
	
	CVar	varN[DATA_PARSE];
	CVar	varO[DATA_NUM];
	bool    varM[MSG_NUM];
	STATE_t vasS;
};

#endif //__DATABASE_H__
