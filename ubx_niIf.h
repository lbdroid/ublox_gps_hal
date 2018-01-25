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
  \brief  Debug interface implementation

*/
/*******************************************************************************
 * $Id: ubx_niIf.h 64025 2012-12-07 07:56:42Z andrea.foni $
 ******************************************************************************/
#ifndef __UBX_NIIF_H__
#define __UBX_NIIF_H__

#include "std_inc.h"
#include <semaphore.h>


class CNiIf
{
public:
	CNiIf();
    static const void* getIf(void) { return &s_interface; }
	static CNiIf* getInstance(void);
	
	// callbacks
#ifdef SUPL_ENABLED
	static void request(GpsNiNotification* pNotification);
#endif
    sem_t sem;
    int m_cmd;

private:
	// interface
    static void init(GpsNiCallbacks* callbacks);
	static void respond(int notif_id, GpsUserResponseType user_response);

	// variables
    static const GpsNiInterface s_interface;
	GpsNiCallbacks m_callbacks;
	bool m_ready;
    pthread_t m_thread;
	
	//impelementation
	static void timoutThread(void *pThreadData);
};

#endif /* __UBX_DEBUGIF_H__ */
