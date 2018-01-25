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
  \brief  RIL interface implementation

*/
/*******************************************************************************
 * $Id: ubx_rilIf.cpp 64292 2012-12-18 08:02:17Z michael.ammann $
 ******************************************************************************/

#include "ubx_rilIf.h"
#include "string.h"
#include <sys/socket.h>
#include <net/if.h>
#include <sys/ioctl.h>

#include "ubxgpsstate.h"

#ifdef SUPL_ENABLED
#include "suplSMmanager.h"

#endif

///////////////////////////////////////////////////////////////////////////////
// Definitions & Types
#define RIL_NO_NETWORK	(-1)

///////////////////////////////////////////////////////////////////////////////
// Static data
static CRilIf s_myIf;	//!< Private instance of the CRilIf class - 'singleton' 

 
const AGpsRilInterface CRilIf::s_interface = 		//!< RIL interface jump table
{
    size:                           sizeof(AGpsRilInterface),
    init:                           CRilIf::init,
    set_ref_location:               CRilIf::setRefLocation,
    set_set_id:                     CRilIf::setSetId,
    ni_message:                     CRilIf::niMessage,
    update_network_state:           CRilIf::updateNetworkState,
#if (PLATFORM_SDK_VERSION >= 14 /* =4.0 */)
    update_network_availability:    CRilIf::updateNetworkAvailability,
#endif
#ifdef CONTROL_PLANE_AGPS
	agps_assist_message:			CRilIf::agpsAssistMessage,
#endif
};

///////////////////////////////////////////////////////////////////////////////
//! Constructor for CRilIf class
CRilIf::CRilIf() 
{ 
	m_ready = false; 
	memset(&m_refLoc, 0, sizeof(m_refLoc));
	strcpy(m_setidImsi, "");
	strcpy(m_setidMsisdn, "");
	m_networkType = RIL_NO_NETWORK;
    sem_init(&sem, 0, 0);
#if (PLATFORM_SDK_VERSION >= 14 /* =4.0 */)
	m_available = 0;
#else
	m_available = 1;			// Always true for Android < 4.0
#endif
    memset(&m_callbacks, 0, sizeof(m_callbacks));
} 

///////////////////////////////////////////////////////////////////////////////
//! Retrieve singleton class instance
/*!
  \return	: Pointer to singleton CRilIf class instance
*/
CRilIf* CRilIf::getInstance()
{
	return &s_myIf;
}

///////////////////////////////////////////////////////////////////////////////
//! RIL interface 'init' function implementation
/*! Framework calls this function on initialisation of the gps driver
  \param callbacks	: Pointer to jump table implementing RIM API into framework
*/
void CRilIf::init(AGpsRilCallbacks* callbacks)
{
    if (s_myIf.m_ready)
        LOGE("CRilIf::%s : already initialized", __FUNCTION__);
    LOGV("CRilIf::%s :", __FUNCTION__);
    s_myIf.m_callbacks = *callbacks;
	s_myIf.m_ready = true;
//lint -e{818} remove Pointer parameter 'callbacks' (line 94) could be declared as pointing to const
}

///////////////////////////////////////////////////////////////////////////////
//! RIL interface 'set_ref_location' implementation
/*! Framework calls this function when it has been instructed to give the gps 
    driver reference location information. Usually this when the driver has called
    the function 'request_refloc' on the AGPS framework interface.
  \param agps_reflocation	: Pointer to a framework defined agps reference 
                              location data structure
  \param sz_struct			: Size of the passed structure
*/
void CRilIf::setRefLocation(const AGpsRefLocation *agps_reflocation, size_t sz_struct)
{
	if (agps_reflocation->type == AGPS_REG_LOCATION_TYPE_MAC)
		LOGV("CRilIf::%s : size=%d type=%d(%s) -> mac=%02X-%02X-%02X-%02X-%02X-%02X", __FUNCTION__, 
				sz_struct, agps_reflocation->type, _LOOKUPSTR(agps_reflocation->type, AGpsRefLocation),
				agps_reflocation->u.mac.mac[0], agps_reflocation->u.mac.mac[1], 
				agps_reflocation->u.mac.mac[2], agps_reflocation->u.mac.mac[3], 
				agps_reflocation->u.mac.mac[4], agps_reflocation->u.mac.mac[5]);
	else if ((agps_reflocation->type == AGPS_REF_LOCATION_TYPE_GSM_CELLID) ||
			 (agps_reflocation->type == AGPS_REF_LOCATION_TYPE_UMTS_CELLID))
		LOGV("CRilIf::%s : size=%d type=%d(%s) -> type=%d mcc=%d mnc=%d lac=%d cid=%d", __FUNCTION__, 
				sz_struct, agps_reflocation->type, _LOOKUPSTR(agps_reflocation->type, AGpsRefLocation),
				agps_reflocation->u.cellID.type, // random, not filled by framework (random)
				agps_reflocation->u.cellID.mcc, agps_reflocation->u.cellID.mnc, 
				agps_reflocation->u.cellID.lac, // in 3g lac is discarded 
				agps_reflocation->u.cellID.cid);
	else
		LOGV("CRilIf::%s : size=%d type=%d(%s)", __FUNCTION__, 
				sz_struct, agps_reflocation->type, _LOOKUPSTR(agps_reflocation->type, AGpsRefLocation));
				
	s_myIf.m_refLoc = *agps_reflocation;	// make a copy

    // Produce the semaphore
    sem_post(&s_myIf.sem);
}

///////////////////////////////////////////////////////////////////////////////
//! RIL interface 'set_set_id' implementation
/*! Framework calls this function when it has been instructed to give the gps 
    driver ID information. Usually this when the driver has called
    the function 'request_setid' on the AGPS framework interface.
  \param type	: Identifier type
  \param setid	: Pointer to test string identifier
*/
void CRilIf::setSetId(AGpsSetIDType type, const char* setid)
{
	if (!setid) setid = "";
    LOGV("CRilIf::%s : type=%d(%s) setid='%s'", __FUNCTION__, type, _LOOKUPSTR(type, AGpsSetIDType), setid);
	if (type == AGPS_SETID_TYPE_NONE)
	{
		// Do nothing
	}
	else
	{
		size_t len = strlen(setid);
		if (len > SETID_MAX_SIZE - 1)
		{
			LOGE("%s: Supplied setid too big '%s' (%i)", __FUNCTION__, setid, len);
		}
		else if (type == AGPS_SETID_TYPE_IMSI)
		{
			strncpy(s_myIf.m_setidImsi, setid, SETID_MAX_SIZE);
		}
		else if (type == AGPS_SETID_TYPE_MSISDN)
		{
			strncpy(s_myIf.m_setidMsisdn, setid, SETID_MAX_SIZE);
		}
		else
		{
			LOGE("%s: Unknown setid type %d '%s'", __FUNCTION__, type, setid);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//! RIL interface 'ni_message' implementation
/*! Framework calls this function when nit wants to pass a Network Initiated
    message to the Gps driver. The driver processes this as a Supl initiation.
  \param 	: Pointer to received message buffer
  \param 	: Size of buffer
*/
void CRilIf::niMessage(uint8_t *msg, size_t len)
{
	assert(pthread_self() != g_gpsDrvMainThread);		// On an arbitory thread
    LOGV("CRilIf::%s msg len %i:", __FUNCTION__, len);
#ifdef SUPL_ENABLED	
	// Assumption is that the msg from the network is a SUPL message
	if (msg)
	{
		suplHandleNetworkInitiatedAction((char*) msg, (int) len);
	}
#endif
//lint -e{818} remove Pointer parameter 'msg' (line 187) could be declared as pointing to const
}

///////////////////////////////////////////////////////////////////////////////
//! RIL interface 'updateNetworkState' implementation
/*! The framework calls this function when it wants to report the networking 
    state of the platform
  \param connected	: 1 if connected to a network, 0 if not.
  \param type		: Type of network.
  \param roaming	: 1 if on a roaming network, 0 if not.
  \param extra_info	: Pointer to extra info buffer.
*/
void CRilIf::updateNetworkState(int connected, int type, int roaming, const char* extra_info)
{
	LOGV("CRilIf::%s : connected=%d type=%d(%s) roaming=%d extra_info='%s'", __FUNCTION__,
         connected,
         type, _LOOKUPSTR((unsigned int) type, AgpsRilNetworkType),
         roaming,
         extra_info
        );

	s_myIf.m_networkType = connected ? type : RIL_NO_NETWORK;
}

#if (PLATFORM_SDK_VERSION >= 14 /* =4.0 */)
///////////////////////////////////////////////////////////////////////////////
//! RIL interface 'update_network_state' implementation
/*! Framework call this function when it wants to report additional information
    regarding the networking state of the platform
  \param avaiable	: 0 if network connection can not be used, 1 if it can
  \param apn		: Pointer to string containing Access Point Name
*/
void CRilIf::updateNetworkAvailability(int avaiable, const char* apn)
{
	LOGV("CRilIf::%s : avaiable=%d apn='%s'", __FUNCTION__,
         avaiable,
         apn
        );
	s_myIf.m_available = avaiable;
}
#endif

///////////////////////////////////////////////////////////////////////////////
//! Requests ID information from the framework
/*! Handles requests to call the framework to request ID information
    Implements 'phone faking' to set pretend ID parameters directly and avoids
    calling the framework which won't yield anything useful.
  \param setid	: Type of ID to request from framework
*/
void CRilIf::requestSetId(uint32_t flags) const
{
    if (!m_ready)
    {
        LOGE("CRilIf::%s : class not initialized", __FUNCTION__);
        return;
    }
    LOGV("CRilIf::%s : flags=%d(%s)", __FUNCTION__, flags, _LOOKUPSTRX(flags,AgpsRilRequestSetId));

#ifdef SUPL_ENABLED	
	if (CUbxGpsState::getInstance()->getFakePhone())
	{
		// Phone fake - Sets up ID parameters without calling framework
		LOGV("CRilIf::%s : Faking phone ID", __FUNCTION__);
		const char* setid = "";
		AGpsSetIDType type = AGPS_SETID_TYPE_NONE;
		if ((flags & AGPS_RIL_REQUEST_SETID_IMSI) == AGPS_RIL_REQUEST_SETID_IMSI)
		{
			type = AGPS_SETID_TYPE_IMSI;
#if 0
			setid = "2280215121973630";
#else
			setid = "460001831429979";
#endif
		}
		else if ((flags & AGPS_RIL_REQUEST_SETID_MSISDN) == AGPS_RIL_REQUEST_SETID_MSISDN)
		{
			type = AGPS_SETID_TYPE_MSISDN;
			setid = "380561234567";
		}
		setSetId(type, setid);
	}
	else
#endif
		m_callbacks.request_setid(flags);
}

///////////////////////////////////////////////////////////////////////////////
//! Request location reference information from the framework
/*! Handles requests to call the framework to request reference location information
    Implements 'phone faking' to set pretend ref location  parameters directly and avoids
    calling the framework which won't yield anything useful.
  \param flags	: Type of location information to request from the framework
*/
void CRilIf::requestRefLoc(uint32_t flags) const
{
    if (!m_ready) 
    {
        LOGE("CRilIf::%s : class not initialized", __FUNCTION__);
        return;
    }
	LOGV("CRilIf::%s : flags=%d(%s)", __FUNCTION__, flags, _LOOKUPSTRX(flags,AgpsRilRequestRefLoc));
	
#ifdef SUPL_ENABLED	
	if (CUbxGpsState::getInstance()->getFakePhone())
	{
		// Phone fake - Sets up reference location parameters without calling framework
		AGpsRefLocation refLoc;
		memset(&refLoc, 0, sizeof(refLoc));
		if ((flags & AGPS_RIL_REQUEST_REFLOC_CELLID) == AGPS_RIL_REQUEST_REFLOC_CELLID)
		{
#if 0
			refLoc.type = AGPS_REF_LOCATION_TYPE_GSM_CELLID;
			refLoc.u.cellID.type = 0/*random*/;
			refLoc.u.cellID.mcc = 230;
			refLoc.u.cellID.mnc = 120;
			refLoc.u.cellID.lac = 99;
			refLoc.u.cellID.cid = 123;
#elif 0
			// spreadtrum phone / shanghai
			refLoc.type = AGPS_REF_LOCATION_TYPE_UMTS_CELLID;
			refLoc.u.cellID.type = 0/*random*/;
			refLoc.u.cellID.mcc = 460;
			refLoc.u.cellID.mnc  = 0;
			refLoc.u.cellID.lac  = 0;
			refLoc.u.cellID.cid = 545;
#else
			// switzerland
			refLoc.type = AGPS_REF_LOCATION_TYPE_GSM_CELLID;
			refLoc.u.cellID.type = 0/*random*/;
			refLoc.u.cellID.mcc = 228;
			refLoc.u.cellID.mnc = 1;
			refLoc.u.cellID.lac = 1010;
			refLoc.u.cellID.cid = 20777;
			
			// Reigate (2G) - UK
//			refLoc.type = AGPS_REF_LOCATION_TYPE_GSM_CELLID;
//			refLoc.u.cellID.type = 0/*random*/;
//			refLoc.u.cellID.mcc = 234;
//			refLoc.u.cellID.mnc = 30;
//			refLoc.u.cellID.lac = 682;
//			refLoc.u.cellID.cid = 3612;

			// // Reigate alternative (2G) - UK
			// refLoc.type = AGPS_REF_LOCATION_TYPE_GSM_CELLID;
			// refLoc.u.cellID.type = 0/*random*/;
			// refLoc.u.cellID.mcc = 234;
			// refLoc.u.cellID.mnc = 15;
			// refLoc.u.cellID.lac = 142;
			// refLoc.u.cellID.cid = 24946;

			// Reigate alternative (2G) - UK
//			refLoc.type = AGPS_REF_LOCATION_TYPE_GSM_CELLID;
//			refLoc.u.cellID.type = 0/*random*/;
//			refLoc.u.cellID.mcc = 234;
//			refLoc.u.cellID.mnc = 33;
//			refLoc.u.cellID.lac = 118;
//			refLoc.u.cellID.cid = 18297;

			// Reigate (3G) - UK
//			refLoc.type = AGPS_REF_LOCATION_TYPE_UMTS_CELLID;
//			refLoc.u.cellID.type = 0/*random*/;
//			refLoc.u.cellID.mcc = 234;
//			refLoc.u.cellID.mnc = 30;
//			refLoc.u.cellID.lac = 0;
//			refLoc.u.cellID.cid = 9243701;
#endif
		}
		else if ((flags & AGPS_RIL_REQUEST_REFLOC_MAC) == AGPS_RIL_REQUEST_REFLOC_MAC)
		{
			// refLoc.type = AGPS_REG_LOCATION_TYPE_MAC;
			// refLoc.u.mac.mac[0] = 
			// refLoc.u.mac.mac[1] = 
			// refLoc.u.mac.mac[2] = 
			// refLoc.u.mac.mac[3] = 
			// refLoc.u.mac.mac[4] = 
			// refLoc.u.mac.mac[6] = 
		}
		setRefLocation(&refLoc, sizeof(refLoc));
	}
	else
#endif	
		m_callbacks.request_refloc(flags);
}

///////////////////////////////////////////////////////////////////////////////
//! Retrieves the IP address of this platform
/*! depending on the type of data network platform is connected to, retrieves
    the IP address of the corresponding networking adaptor
  \return	: IP address of this platform
*/
in_addr CRilIf::getClientIP(void)
{
	struct in_addr addr;
	memset(&addr, 0, sizeof(addr));
	
	switch(m_networkType)
	{
	case RIL_NO_NETWORK:
		addr = readIpAddress("eth0");
		break;
		
	case AGPS_RIL_NETWORK_TYPE_WIFI:
		addr = readIpAddress("wlan0");
		break;
	
	default:
		break;
	}

	LOGV("%s: %s (Type = %i)", __FUNCTION__, inet_ntoa(addr), m_networkType);
	
	return addr;
}	

///////////////////////////////////////////////////////////////////////////////
//! Retrieves the IP address assigned to a newtorking device
/*!
  \param pDeviceName	: Pointer to the name of the network device
  \return				: IP address of device
*/
in_addr CRilIf::readIpAddress(const char* pDeviceName)
{
	 int fd;
	 struct ifreq ifr;

	 memset(&ifr, 0, sizeof(ifr));
	 fd = socket(AF_INET, SOCK_DGRAM, 0);

	 /* I want to get an IPv4 IP address */
	 ifr.ifr_addr.sa_family = AF_INET;

	 /* I want IP address for device */
	 strncpy(ifr.ifr_name, pDeviceName, IFNAMSIZ - 1);
	 ioctl(fd, SIOCGIFADDR, &ifr);
	 close(fd);
	 
	 return ((struct sockaddr_in *) (void *) &ifr.ifr_addr)->sin_addr;
}

///////////////////////////////////////////////////////////////////////////////
//! Collect cell information
/*! Initiates calls to framework to retrieve all cell information

*/
void CRilIf::requestCellInfo(void)
{
	requestRefLoc(AGPS_RIL_REQUEST_REFLOC_MAC | AGPS_RIL_REQUEST_REFLOC_CELLID);

    // Be sure the reference location is got...
	struct timespec timeOut;
	timeOut.tv_sec = time(NULL) + REQUEST_REF_LOC_TIMEOUT;
	timeOut.tv_nsec = 0;
	
	sem_timedwait(&sem, &timeOut);	// Timeout needed because 'request' may not
									// cause corresponding 'set' if error occurs

	// First clear existing data as platform my have had SIM removed
	strcpy(m_setidImsi, "");
	strcpy(m_setidMsisdn, "");
	
	// Now request current cell info
	requestSetId(AGPS_RIL_REQUEST_SETID_IMSI);
	requestSetId(AGPS_RIL_REQUEST_SETID_MSISDN);
}

///////////////////////////////////////////////////////////////////////////////
//! Determine state of connection to data network
/*!
  \return	: true if connected to some kind of network, false if not
*/
bool CRilIf::isConnected(void) const
{
	if (isAvailable())
	{
		return (m_networkType != RIL_NO_NETWORK);
	}
	
	return false;
}

///////////////////////////////////////////////////////////////////////////////
//! determines if a SIMM card is present
/*!
  \return	: true if a SIMM is present, false if not
*/
bool CRilIf::isSimPresent(void) const
{
	return ((strcmp(m_setidImsi, "") != 0) || (strcmp(m_setidMsisdn, "") != 0));
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#ifdef CONTROL_PLANE_AGPS

#ifdef WIN32
 #pragma pack(push, 1)
 #define PACKED
#else
 #define PACKED __attribute__((packed))
#endif
	// this payload is UBX-AID-INI
	struct ini_ubx {
		I4 x : 32; I4 y : 32; I4 z : 32; U4 posacc : 32; 
		U4 tm_cfg: 16; U4 wn : 16; U4 itow : 32; I4 frac : 32; U4 tacc_ms : 32; U4 tacc_ns : 32;
		I4 clkd : 32; U4 clkd_acc : 32; U4 flags : 32;
	} PACKED;
	
	// this payload is UBX-AID-EPH (we dont char about the type of the bitfields as we never read them)
	struct eph_ubx { 
		U4 sv_id : 8;
		U4 pad : 24;
		U4 how : 32; 
		struct { // SF 1
			/* WORD  3 */ U4 iodc_msb : 2; U4 sv_health : 6; U4 sv_ura : 4;
						  U4 code_on_l2 : 2; U4 wn_oe : 10; U4 _pad3 : 8;
			/* WORD  4 */ U4 rsvd1 : 23; U4 l2_p_flag : 1; U4 _pad4 : 8;
			/* WORD  5 */ U4 rsvd2 : 24; U4 _pad5 : 8;
			/* WORD  6 */ U4 rsvd3 : 24; U4 _pad6 : 8;
			/* WORD  7 */ U4 t_gd : 8; U4 rsvd4 : 16; U4 _pad7 : 8;
			/* WORD  8 */ U4 t_oc : 16; U4 iodc_lsb : 8; U4 _pad8 : 8;
			/* WORD  9 */ U4 a_f1 : 16; U4 a_f2 : 8; U4 _pad9 : 8;
			/* WORD 10 */ U4 t : 2; U4 a_f0  : 22; U4 _pad10 : 8;
		} sf1;
		struct { // SF 2
			/* WORD  3 */ U4 c_rs : 16; U4 iode : 8; U4 _pad3 : 8;
			/* WORD  4 */ U4 m_0_msb : 8; U4 delta_n : 16; U4 _pad4 : 8;
			/* WORD  5 */ U4 m_0_lsb : 24; U4 _pad5 : 8;
			/* WORD  6 */ U4 e_msb : 8; U4 c_uc : 16; U4 _pad6 : 8;
			/* WORD  7 */ U4 e_lsb : 24; U4 _pad7 : 8;
			/* WORD  8 */ U4 a_sqrt_msb : 8; U4 c_us : 16; U4 _pad8 : 8;
			/* WORD  9 */ U4 a_sqrt_lsb : 24; U4 _pad9 : 8;
			/* WORD 10 */ U4 t : 2; U4 aodo : 5; U4 fit_flag : 1; U4 t_oe : 16; U4 _pad10 : 8;
		} sf2;
		struct { // SF 3
			/* WORD  3 */ U4 omega_0_msb : 8; U4 c_ic : 16; U4 _pad3 : 8;
			/* WORD  4 */ U4 omega_0_lsb : 24; U4 _pad4 : 8;
			/* WORD  5 */ U4 i_0_msb : 8; U4 c_is : 16; U4 _pad5 : 8;
			/* WORD  6 */ U4 i_0_lsb : 24; U4 _pad6 : 8;
			/* WORD  7 */ U4 w_msb : 8; U4 c_rc : 16; U4 _pad7 : 8;
			/* WORD  8 */ U4 w_lsb : 24; U4 _pad8 : 8;
			/* WORD  9 */ U4 omega_dot : 24; U4 _pad9 : 8;
			/* WORD 10 */ U4 t : 2; U4 idot : 14; U4 iode : 8; U4 _pad10 : 8;
		} sf3;
	} PACKED;
#ifdef WIN32
 #pragma pack(pop)
#endif	

void CRilIf::agpsAssistMessage(uint8_t *msg, size_t len)
{
	LOGV("CRilIf::%s : msg len=%d", __FUNCTION__, len);
	if (len != sizeof(agps_assist_t))
	{
		LOGV("CRilIf::%s : len=%d wrong %d expected", __FUNCTION__, len, sizeof(agps_assist_t));
		return;
	}
	// debugging;
	unsigned int k, j;
	char str[1024];
	// get the instance
	CUbxGpsState* pUbxGps = CUbxGpsState::getInstance();
	// here we do the conversion ... 
	agps_assist_t* p = (agps_assist_t*)msg;
	double tow = 0;
	int wn = 0;
	struct ini_ubx ini;
	memset(&ini, 0, sizeof(struct ini_ubx));
	if (p->valid & 0x0001) //reference time valid
	{
		double delay = 0.0;
		//struct timeval t;
		//gettimeofday(&t, 0);
		//delay =  (1e-6 * t.tv_usec + t.tv_sec) - 
		//				(1e-6 * assist.time.stamp.tv_usec + assist.time.stamp.tv_sec);
		wn = 1024 + p->reference_time.gps_Week;
		tow = (delay + 0.001 * p->reference_time.gps_tow_1msec);
		if (tow >= 7*24*60*60)
		{
			tow -= 7*24*60*60;
			wn++;
		}
		ini.wn = wn;
		ini.itow = (int)(tow*1000);
		ini.tacc_ms = 2*1000; /* 2 seconds */
		ini.flags |= 0x02;
		// debugging 
		for (k = 0, j = 0; j < sizeof(p->reference_time); j ++)
			k += sprintf(&str[k], "%02X ", ((unsigned char*) &p->reference_time)[j]);
		LOGV("CRilIf::%s: reference_time %s", __FUNCTION__,  str);
		// log message
		LOGV("CRilIf::%s: UBX-AID-INI wn=%d tow=%.6f acc=%.3f", __FUNCTION__, 
					wn, tow, 0.001 * ini.tacc_ms);
	}
	if (p->valid & 0x0002) //reference location valid
	{
		// lla to xyz conversion
		const double F  = 1.0/298.2572235630;
		const double A	= 6378137.0;
		const double B	= A * (1.0-F);
		const double pi = 3.1415926535898;
		const double d2r = pi/180.0;
		double lat =   pi/2 / (1<<23) * p->reference_location.mnellipsoidPointAltitudeEllipsoide.latitude * 
									   (p->reference_location.mnellipsoidPointAltitudeEllipsoide.latitudeSign ? -1/*south*/ : 1/*north*/);
		double lon = 2 * pi / (1<<24) * p->reference_location.mnellipsoidPointAltitudeEllipsoide.longitude;
		double alt = p->reference_location.mnellipsoidPointAltitudeEllipsoide.altitude *
					 (p->reference_location.mnellipsoidPointAltitudeEllipsoide.altitudeDirection ? -1/*depth*/ : 1/*height*/);
		double s = sin(lat);
		double c = cos(lat);
		double N =  (A * A) / sqrt(A * A * c * c + B * B * s * s);
		ini.x = (int)(100.0 * (N + alt) * c * cos(lon));
		ini.y = (int)(100.0 * (N + alt) * c * sin(lon));
		ini.z = (int)(100.0 * ((B * B) / (A * A) * N + alt) * s);
		ini.posacc = (int)(100.0 * 10.0 * (pow(1.1,			p->reference_location.mnellipsoidPointAltitudeEllipsoide.uncertaintySemiMajor) - 1));
		//ini.altacc = (int)(100.0 * 45.0 * (pow(1.0+0.025,	p->reference_location.mnellipsoidPointAltitudeEllipsoide.uncertaintyAltitude)  - 1));

		for (k = 0, j = 0; j < sizeof(p->reference_location); j ++)
			k += sprintf(&str[k], "%02X ", ((unsigned char*) &p->reference_location)[j]);
		LOGV("CRilIf::%s: reference_location %s", __FUNCTION__,  str);
		LOGV("CRilIf::%s: UBX-AID-INI lat=%.6f lon=%.6f alt=%.1f acc=%.2f x=%.2f y=%.2f z=%.2f", __FUNCTION__, 
					lat / d2r, lon / d2r, alt, 0.01 * ini.posacc, 
					0.01 * ini.x, 0.01 * ini.y, 0.01 * ini.z);
		ini.flags |= 0x01;
	}
	if (ini.flags)
	{
		pUbxGps->lock();
		pUbxGps->sendAidingData((const GPS_UBX_AID_INI_U5__t*)&ini);
		pUbxGps->unlock();
	}
	
	if (p->valid & 0x0008) //navigation_model valid
	{
		for (int n = 0; n < p->navigation_model.mnNavigationModelSatInfoList.NumNavigationModelSatInfo; n ++) 
		{
			mn_agps_NavigationModelSatInfoSingle_t* nm = 
				&p->navigation_model.mnNavigationModelSatInfoList.mnNavigationModelSatInfo[n];
			if (/*(nm->satID >= 0) &&*/ (nm->satID < 32) && 
				((nm->satelliteStatus == 0/*NS_NN = new SV, new nav model*/) || 
				 //(nm->satelliteStatus == 1/*ES_SN = existing SV, same nav model*/) || // Ephemeris optional 
				 (nm->satelliteStatus == 2/*ES_NN = existing SV, new nav model*/))) 
			{
				struct eph_ubx eph;
				/* convert to UBX */
				memset(&eph, 0, sizeof(struct eph_ubx));
				eph.sv_id = nm->satID + 1;
				eph.how = ((unsigned int)nm->mnephemerisParameter.t_oe << 11) / 6;
			
				eph.sf1.iodc_msb = nm->mnephemerisParameter.iodc >> 8;
				eph.sf1.sv_health = nm->mnephemerisParameter.satHealth;
				eph.sf1.sv_ura = nm->mnephemerisParameter.uraIndex;
				//eph.sf1.code_on_l2 = nm->mnephemerisParameter.codeOnL2; // not needed
				eph.sf1.wn_oe = 0/* set later */;
				//aubx.sf1.rsvd1 = nm->mnephemerisParameter.reserved1; // not needed
				//eph.sf1.rsvd2 = nm->mnephemerisParameter.reserved2; // not needed
				//eph.sf1.rsvd3 = nm->mnephemerisParameter.reserved3; // not needed
				//eph.sf1.l2_p_flag = nm->mnephemerisParameter.l2Pflag; // not needed
				//eph.sf1.rsvd4 = nm->mnephemerisParameter.reserved4; // not needed
				eph.sf1.t_gd = nm->mnephemerisParameter.t_GD;
				eph.sf1.t_oc = nm->mnephemerisParameter.t_oc;
				eph.sf1.iodc_lsb = nm->mnephemerisParameter.iodc;
				eph.sf1.a_f2 = nm->mnephemerisParameter.af2;
				eph.sf1.a_f1 = nm->mnephemerisParameter.af1;
				eph.sf1.a_f0 = nm->mnephemerisParameter.af0;
				// SF 2
				eph.sf2.c_rs = nm->mnephemerisParameter.c_rs;
				eph.sf2.iode = nm->mnephemerisParameter.iodc; 
				eph.sf2.m_0_msb = nm->mnephemerisParameter.m0 >> 24;
				eph.sf2.delta_n = nm->mnephemerisParameter.delta_n;
				eph.sf2.m_0_lsb = nm->mnephemerisParameter.m0;
				eph.sf2.e_msb = nm->mnephemerisParameter.e >> 24;
				eph.sf2.c_uc = nm->mnephemerisParameter.c_uc;
				eph.sf2.e_lsb = nm->mnephemerisParameter.e;
				eph.sf2.a_sqrt_msb = nm->mnephemerisParameter.a_Sqrt >> 24;
				eph.sf2.c_us = nm->mnephemerisParameter.c_us;
				eph.sf2.a_sqrt_lsb = nm->mnephemerisParameter.a_Sqrt;
				eph.sf2.fit_flag = nm->mnephemerisParameter.fitInterval;
				//eph.sf2.aodo = nm->mnephemerisParameter.aodo; // not needed	
				eph.sf2.t_oe = nm->mnephemerisParameter.t_oe;
				// SF 2
				eph.sf3.omega_0_msb = nm->mnephemerisParameter.omega0 >> 24;
				eph.sf3.c_ic = nm->mnephemerisParameter.c_ic;
				eph.sf3.omega_0_lsb = nm->mnephemerisParameter.omega0;
				eph.sf3.i_0_msb = nm->mnephemerisParameter.i0 >> 24;
				eph.sf3.c_is = nm->mnephemerisParameter.c_is;
				eph.sf3.i_0_lsb = nm->mnephemerisParameter.i0;
				eph.sf3.w_msb = nm->mnephemerisParameter.omega >> 24;
				eph.sf3.c_rc = nm->mnephemerisParameter.c_rc;
				eph.sf3.w_lsb = nm->mnephemerisParameter.omega;
				eph.sf3.omega_dot = nm->mnephemerisParameter.omegaDot;
				eph.sf3.idot = nm->mnephemerisParameter.iDot;
				eph.sf3.iode = nm->mnephemerisParameter.iodc;
	
				int toe = (eph.how >> 7) * 6;
				int tmp = (int)(tow - toe);
				eph.sf1.wn_oe = (tmp >  302400) ? wn + 1 :
								(tmp < -302400) ? wn - 1 : 
									 			  wn;
				// debugging
				for (k = 0, j = 0; j < sizeof(mn_agps_NavigationModelSatInfoSingle_t); j ++)
					k += sprintf(&str[k], "%02X ", ((unsigned char*) &nm)[j]);
				LOGV("CRilIf::%s: mnNavigationModelSatInfo[%d] %s", __FUNCTION__, n, str);
				// debugging 
				for (k = 0, j = 0; j < 8 * 3; j ++)
					k += sprintf(&str[k], "%06X ", ((unsigned int*) &eph.sf1)[j]);
				LOGV("CRilIf::%s: eph-AID-EPH sv=%2d how=%d data=%s", __FUNCTION__, eph.sv_id, (eph.how >> 7) * 6, str);
			
				pUbxGps->lock();
				pUbxGps->sendEph(&eph, sizeof(eph));
				pUbxGps->unlock();
			}
		}
	}
}
#endif
