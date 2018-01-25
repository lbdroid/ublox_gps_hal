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
  \brief  Serial utility functions

  Utility for accessing the serial port in a easier way
*/
/*******************************************************************************
 * $Id: ubx_serial.h 63615 2012-11-27 10:12:42Z andrea.foni $
 ******************************************************************************/

#ifndef __UBX_SERIAL_H__
#define __UBX_SERIAL_H__

#include <fcntl.h>
#include <malloc.h>
#include <string.h>

#define BAUDRATE_TABLE_SIZE 7

class CSerialPort
{
public:
    CSerialPort()
    {
        m_fd = -1;
		m_i2c = false;
    };
    ~CSerialPort(){};

    bool openSerial(const char * pTty, int ttybaud, int blocksize);

    int setbaudrate(int ttybaud)
    {
        return settermios(ttybaud, -1);
    };

    int changeBaudrate(char * ptty, int * pBaudrate, const unsigned char * pBuf, int length);

    void baudrateIncrease(int *baudrate) const;

    int retrieveErrors() const;

    void closeSerial()
    {
        if (m_fd > 0)
            close(m_fd);
		m_fd = -1;
		m_i2c = false; 
    };

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

    bool fdIsSet(fd_set &rfds) const 
    {
        if ((m_fd > 0)  && FD_ISSET(m_fd, &rfds))
            return true;
        return false;
//lint -e{1764} suppress "could be declared const ref"
    };

    int readSerial(void *pBuffer, unsigned int size)
    {
        if (m_fd <= 0)
            return -1;
        return read(m_fd, pBuffer, size);
    };

    int writeSerial(const void *pBuffer, unsigned int size)
    {
		unsigned char* p;
        
		if (m_fd <= 0)
            return -1;
		if (!m_i2c)
	        return write(m_fd, pBuffer, size);
	
		p = (unsigned char*)malloc(size+1);
		if (!p)
			return 0;
		p[0] = 0xFF; // allways address the stream register
		memcpy(p+1, pBuffer, size);
		size = (unsigned int) write(m_fd, p, size+1);
		if (size > 0)
			size --;
		free(p);
		return (int) size;
	};

	bool isFdOpen(void) const 
	{
		return m_fd > 0;
	}
	
private:
    int m_fd;
	bool m_i2c;

    int settermios(int ttybaud, int blocksize);

    static const int s_baudrateTable[BAUDRATE_TABLE_SIZE];

	//! container of error counters
#if defined serial_icounter_struct
    static struct serial_icounter_struct s_einfo;
#endif

};

#endif /* __UBX_SERIAL_H__ */

