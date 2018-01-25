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
  \brief  ULP message decode

  Module for decoding SUPL messagges sent from the SLP to the SET
*/
/*******************************************************************************
 * $Id: upldecod.cpp 64618 2013-01-09 11:22:57Z jon.bowern $
 * $HeadURL: http://svn.u-blox.ch/GPS/SOFTWARE/hmw/android/release/release_2.0/supl/upldecod.cpp $
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <time.h>
#include <malloc.h>

#include "upldecod.h"
#include "uplsend.h"
#include "ubx_log.h"

///////////////////////////////////////////////////////////////////////////////
//! Decode a SUPL message
/*! Function for decoding an incoming SUPL message. it will accept the payload of the 
    message as argment, and it will return a pointer to the received structure. 
  \param pBuffer     : payload recieved
  \param size       : size of the payload
  \return             pointer to ULP PDU message
*/
struct ULP_PDU *uplDecode(const char *pBuffer, int size)
{
	asn_dec_rval_t rval;
    ULP_PDU_t *pMsg;

    pMsg = (ULP_PDU_t *) MC_CALLOC(sizeof(ULP_PDU_t),1);
    if (pMsg == NULL)
	{
        LOGE("%s: allocation error", __FUNCTION__);
	}

    rval = uper_decode_complete(0, 
                                &asn_DEF_ULP_PDU,
                                (void **) &pMsg,
                                pBuffer,
                                (unsigned int) size);

	logSupl(pMsg, true);
    switch(rval.code) {
    case RC_OK:
        LOGV("%s: succeed", __FUNCTION__);
        break;
    case RC_FAIL:
        LOGW("%s: Decoding failure... %zd", __FUNCTION__, rval.consumed);
        ASN_STRUCT_FREE(asn_DEF_ULP_PDU, pMsg);
        pMsg = NULL;
        break;
    case RC_WMORE:
        LOGW("%s: Want more? %zd", __FUNCTION__, rval.consumed);
        ASN_STRUCT_FREE(asn_DEF_ULP_PDU, pMsg);
        pMsg = NULL;
        break;
    }

    return pMsg;
}

///////////////////////////////////////////////////////////////////////////////
//! Extract the session ID
/*! Function for extracting the session ID from a Supl message
  \param pMsg : pointer to the Supl message
  \return Extracted session ID
*/
int extractSid(const struct ULP_PDU *pMsg)
{
    /* Verify the prence of the session ID field */
    if (pMsg->sessionID.setSessionID == NULL)
    {
        LOGW("%s: No session ID field... must be a SUPL INIT", __FUNCTION__);
        if ( pMsg->message.present != UlpMessage_PR_msSUPLINIT )
        {
            LOGE("%s: session ID not present AND not SUPL INIT!!!!", __FUNCTION__);
        }
        return 0;
    }

    /* returning the SET session ID, the one we consider for the session */
    return (pMsg->sessionID.setSessionID->sessionId);
}
