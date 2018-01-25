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
  \brief  Local UDP Port Handling

  This file provides the implementation of local ports. A master (i.e. what is 
  implemented here) broadcasts data it has received to local clients which wait 
  for UDP traffic on a defined port.

  If clients talk, the traffic is received in here and delivered to the 
  upper layers.

  Connection handling, keep-alive  timeouting is also being done in here
*/
/*******************************************************************************
 * $Id: ubx_udpServer.h 63615 2012-11-27 10:12:42Z andrea.foni $
 ******************************************************************************/

#ifndef __UBX_UDPSERVERL_H__
#define __UBX_UDPSERVERL_H__

#include <arpa/inet.h>
#include <fcntl.h>

#ifndef MAXUDPCONN
#define MAXUDPCONN 2 //!< Maximum number of UDP connected clients
#endif

#define SESSION_TIMEOUT    4            //<! in seconds is the timeout for UDP connection
#define SESSION_TIMEOUT_MS (4 * 1000)   //<! in milliseconds is the timeout for UDP connection


class CUdpServer
{
public:
    CUdpServer()
    {
        m_fd = -1;
        m_slaveStatus = 0;
        memset(m_udpConn, 0, sizeof(m_udpConn));
    };
    ~CUdpServer(){};

    int openLocalPort(int port);
    int recvPort(char * pBuf, int buflen);
    void checkPort(int slaveOpen);
    void sendPort(const unsigned char * pBuf, int len);

    bool fdSet(fd_set &rfds, int &rMaxFd) const 
    {
        if (m_fd <= 0)
            return false;
        if ((m_fd + 1) > rMaxFd)
            rMaxFd = m_fd + 1;
        FD_SET(m_fd, &rfds);
        return true;
//lint -e{1764} suppress "could be declared const ref"
    };

    void closeUdp()
    {
        close(m_fd);
		m_fd = -1;
    };

    bool fdIsSet(fd_set &rfds) const
    {
        if ((m_fd > 0) && (FD_ISSET(m_fd, &rfds)))
            return true;
        return false;
//lint -e{1764} suppress "could be declared const ref"
    };

private:
    int m_fd;	// file descriptor for UDP socket
	
    int findConnection (struct sockaddr_in si_other) const;
    int findEmptySlot (void) const;
    int activeConnections (void) const;

    typedef struct udpConn_s 
    {
        int64_t seen;
        struct sockaddr_in conn;
        unsigned int flags;
    } udpConn_t;

	// Array where all connection-related info is stored:
    udpConn_t m_udpConn[MAXUDPCONN];

    int m_slaveStatus;
};

#endif /* __UBX_UDPSERVERL_H__ */
