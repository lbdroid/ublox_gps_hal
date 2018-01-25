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
  \brief  ULP message manager interface

  Interface description of the module for managing SUPL messagges  sent 
  from the SET to the SPL
*/
/*******************************************************************************
 * $Id: upldecod.h 63615 2012-11-27 10:12:42Z andrea.foni $
 * $HeadURL: http://svn.u-blox.ch/GPS/SOFTWARE/hmw/android/release/release_2.0/supl/upldecod.h $
 ******************************************************************************/

#ifndef __UPLDECOD_H__
#define __UPLDECOD_H__

#include "ULP-PDU.h"

///////////////////////////////////////////////////////////////////////////////
// Functions
struct ULP_PDU *uplDecode(const char *pBuffer, int size);
int extractSid(const struct ULP_PDU *pMsg);

#endif /* __UPLDECOD_H__ */
