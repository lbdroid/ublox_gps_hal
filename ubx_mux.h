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
	\brief MUX message definitions

*/
/*******************************************************************************
 * $Id: ubx_mux.h 63615 2012-11-27 10:12:42Z andrea.foni $
 ******************************************************************************/

#ifndef __UBX_MUX_H__
#define __UBX_MUX_H__

#define UDP_MUX_PACKET_HEADER   'X'

//! policies for the flags 
//@{
#define CLIENT_NMEA_ENABLED     0x01          //!< enable the receiving of NMEA messages
#define CLIENT_UBX_ENABLED      0x02          //!< enable the receiving of UBX messages
#define CLIENT_MUX_ENABLED      0x04          //!< enable the receiving of MUX messages
#define CLIENT_TRASPARENT_MODE  0x08          //!< the client will receive all the traffic
//@}

#define UDP_MUX_PING            0x00
#define UDP_MUX_START_SESSION   0x01
#define UDP_MUX_NUM_CLIENT      0x02
#define UDP_MUX_END_SESSION     0x03

#endif //__UBX_MUX_H__
