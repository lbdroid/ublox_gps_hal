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
  \brief  GPS global IF

*/
/*******************************************************************************
 * $Id: gps_thread.h 63615 2012-11-27 10:12:42Z andrea.foni $
 ******************************************************************************/

#ifndef __GPS_THREAD_H__
#define __GPS_THREAD_H__

#include <pthread.h>
#include "ubx_moduleIf.h"

///////////////////////////////////////////////////////////////////////////////
// Definitions & Types

typedef  enum {
	CMD_NONE,
	CMD_START_SI,
	CMD_STOP_SI,
	CMD_START_NI,
	CMD_STOP_NI
#ifndef ANDROID_BUILD
    , CMD_EXIT
#endif    
} THREAD_CMDS;

typedef enum
{
	GPS_UNKNOWN,
	GPS_STARTED,
	GPS_STOPPING,
	GPS_STOPPED
} GPS_THREAD_STATES;

typedef struct {										//!< Main Gps (control) thread data structure containing
														//!< thread state & communication info
	GPS_THREAD_STATES       gpsState;					//!< Main state of Gps driver
	int                     cmdPipes[2];				//!< Handles to command communication pipes to main thread
	int                     cmdResult;					//!< Communication command result field
	pthread_cond_t          threadCmdCompleteCond;		//!< Mutex used in thread synchronisation
	pthread_mutex_t         threadCmdCompleteMutex;		//!< Condition used in thread synchronisation
	int64_t                 stoppingTimeoutMs;			//!< Timeout value when 'stopping'
	int						clientCount;				//!< Count of how many 'clients' are using the driver
														//!< Should the framework + number of NI sessions active
	pthread_mutex_t			threadDataAccessMutex;		//!< Mutex to control the access to data in this structure
} ControlThreadInfo;

typedef void (* requestStart)(void* pContext);		//!< Function prototype for 'Start' request event hander
typedef void (* requestStop)(void* pContext);		//!< Function prototype for 'Stop' request event handler

typedef struct 							//!< Event handler interface
{
    requestStart requestStart_cb;		//!< 'Start' request event handler
    requestStop requestStop_cb;			//!< 'Stop' request event handler
} GpsControlEventInterface;

///////////////////////////////////////////////////////////////////////////////
// API

void ubx_thread(void *pThreadData);

void gps_state_inject_time(GpsUtcTime timeUtcGps, int64_t timeReference, int uncertainty);
void gps_state_inject_location(double latitude, double longitude, float accuracy);
void gps_state_delete_aiding_data(GpsAidingData flags);
void gps_state_set_interval(uint32_t min_interval);
void gps_state_agps_injectData(const char* data, int length);

void controlThreadInfoInit(ControlThreadInfo* pControlThreadInfo);
void controlThreadInfoRelease(ControlThreadInfo* pControlThreadInfo);
bool controlThreadInfoSendCmd(ControlThreadInfo* pControlThreadInfo, THREAD_CMDS cmd);
void controlThreadInfoSetIF(ControlThreadInfo* pControlThreadInfo, CGpsIf* pInterface);

#ifndef UNDEBUG
extern pthread_t g_gpsDrvMainThread;
#endif

#endif /* __GPS_THREAD_H__ */
