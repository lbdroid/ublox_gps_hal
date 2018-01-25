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
  \brief  Agps interface implementation

*/
/*******************************************************************************
 * $Id: ubx_agpsIf.h 63615 2012-11-27 10:12:42Z andrea.foni $
 ******************************************************************************/
#ifndef __UBX_AGPSIF_H__
#define __UBX_AGPSIF_H__

#include "std_inc.h"

class CAgpsIf
{
public:
	CAgpsIf();
	~CAgpsIf();
    static const void *getIf() { return &s_interface; }
	// operations
	static CAgpsIf* getInstance(void);
	
#ifdef SUPL_ENABLED	
	void getSuplServerInfo(char** ppSuplServerAddress, int *pSuplPort);
	void setCertificateFileName(const char* pCert) 
	{ 
		if (m_certificateFileName)
		{
			free(m_certificateFileName);
			m_certificateFileName = NULL;
		}
		m_certificateFileName = pCert ? strdup(pCert) : NULL;
	};
	const char* getCertificateFileName(void) const { return m_certificateFileName; };
	bool isTlsActive(void) const { return (m_certificateFileName != NULL); };
#endif

protected:
	// interface
 	static void init(AGpsCallbacks* callbacks);
	static int dataConnOpen(const char* apn);
	static int dataConnClosed(void);
	static int dataConnFailed(void);
	static int setServer(AGpsType type, const char* hostname, int port);
	
	// variables	
    static const AGpsInterface s_interface;
	AGpsCallbacks m_callbacks;
	bool m_ready;
	
	// implementation (server name list)
	int numServer(AGpsType type) const;
	#define NUM_AGPS_SERVERS	16
	#define MAX_HOSTNAME		256
	typedef struct AGPS_SERVER_DATA_s
	{ 
		AGpsType type;
		char hostname[MAX_HOSTNAME];
		int port;
	} AGPS_SERVER_DATA_t;
	AGPS_SERVER_DATA_t		m_agpsServers[NUM_AGPS_SERVERS];
	int						m_agpsServersCnt;
	
#ifdef SUPL_ENABLED		
	char* m_certificateFileName;
#endif
};

#endif /* __UBX_AGPSIF_H__ */
