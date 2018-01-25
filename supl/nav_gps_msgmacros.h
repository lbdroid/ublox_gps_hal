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
  \brief  GPS message macros

  The macros defined herein are used to extract data from the transmitted
  GPS message.
*/
/*******************************************************************************
 * $Id: nav_gps_msgmacros.h 63615 2012-11-27 10:12:42Z andrea.foni $
 * $HeadURL: http://svn.u-blox.ch/GPS/SOFTWARE/hmw/android/release/release_2.0/supl/nav_gps_msgmacros.h $
 ******************************************************************************/

#ifndef __NAV_GPS_MSGMACROS_H__
#define __NAV_GPS_MSGMACROS_H__ //!< multiple inclusion guard

#include "std_macros.h"

/*! \name GPS Subframe Bits Extract Macros 

	These macros extract bits from an array and converts 
	the result into a U4 or I4. The GPS subframe word has
	the following format. 

	\verbatim
	MSB                                               LSB
	+------------+----------------------+---------------+
	| 2 Pad Bits |   24   Data   Bits   | 6 Parity Bits |
	+------------+----------------------+---------------+
	\endverbatim

	The offset is the offset into the subframe including 
	the parity bits. The number of bits to extract is the 
	number of databits excluding the parity.

	@{
*/

//! Unsigned Bits Extract from a GPS Subframe format
/**
	
	The macro performs the following steps:
	- get the first word and shift the bits to the MSBs
	- check if we have to add another word
	- if so load it and attach it to the other bits
	- shift back the bits to the LSBs
	
	\note The bits that are to be extract must not span more than two data words.

	\param pkAdr IN a pointer to the array.
	\param ofs   IN the bit offset in the array.
	\param num   IN the number of bits to extract.
	\return the composed word.
*/
#define GPSSFRU4DW(pkAdr, ofs, num)  					\
		( (U4)( ( (pkAdr[(ofs) / 30] >> 6)				\
				  << (((ofs) % 30) + 8) ) | 			\
				( (24 >= ((ofs) % 30 + (num))) ? 0 : 	\
				  ( ((U4)(pkAdr)[(ofs) / 30 + 1] << 2)	\
				    >> (24 - ((ofs) % 30)) ) )			\
			  ) >> (32 - (num)) )

//! Unsigned Bits Extract from a GPS Subframe format
/**
	
	The macro performs the following steps:
	- get the word and shift the bits to the MSBs
	- shift back the bits to the LSBs
	
	\note The bits that are to be extract must not span more than one data word.

	\param pkAdr IN a pointer to the array.
	\param ofs   IN the bit offset in the array.
	\param num   IN the number of bits to extract.
	\return the composed word.
*/
#define GPSSFRU4SW(pkAdr, ofs, num)  					\
		( (U4)( (pkAdr[(ofs) / 30] >> 6)				\
				  << (((ofs) % 30) + 8)  				\
			  ) >> (32 - (num)) )

//! Signed Bits Extract from a GPS Subframe format
/**
	
	The macro performs the following steps:
	- get the first word and shift the bits to the MSBs
	- check if we have to add another word
	- if so load it and attach it to the other bits
	- shift back the bits to the LSBs the sign is extended
	
	\note The bits that are to be extract must not span more than two data words

	\param pkAdr IN a pointer to the array.
	\param ofs   IN the bit offset in the array.
	\param num   IN the number of bits to extract.
	\return the composed word.
*/
#define GPSSFRI4DW(pkAdr, ofs, num)  					\
		( (I4)( ( (pkAdr[(ofs) / 30] >> 6)				\
				  << (((ofs) % 30) + 8) ) | 			\
				( (24 >= ((ofs) % 30 + (num))) ? 0 : 	\
				  ( ((U4)(pkAdr)[(ofs) / 30 + 1] << 2)	\
				    >> (24 - ((ofs) % 30)) ) )			\
			  ) >> (32 - (num)) )

//! Signed Bits Extract from a GPS Subframe format
/**
	
	The macro performs the following steps:
	- get the word and shift the bits to the MSBs
	- shift back the bits to the LSBs the sign is extended
	
	\note The bits that are to be extract must not span more than one data word

	\param pkAdr IN a pointer to the array.
	\param ofs   IN the bit offset in the array.
	\param num   IN the number of bits to extract.
	\return the composed word.
*/
#define GPSSFRI4SW(pkAdr, ofs, num)  					\
		( (I4)( (pkAdr[(ofs) / 30] >> 6)				\
				  << (((ofs) % 30) + 8) 				\
			  ) >> (32 - (num)) )

//! get transmission tow from HOW
/*! The Hand-over word (HOW) transmitted in every GPS subframe contains
	a truncated time of week (TOW) in ZCount units (1.5s). This macro
	extracts this TOW and converts in to seconds in the week.
	
	\verbatim
    tow = ttow                      * 2^2                 * 1.5 
          (truncated tow from HOW)  (missing 2 LSB bits)  (ZCount -> s)
    \endverbatim
*/
#define HOWTOW(pkAdr) (GPSSFRU4SW(pkAdr, 30, 17) * 6)

//! Bits Pack for a GPS Subframe format (single word)
/**
	Reverse function of #GPSSFRI4SW and #GPSSFRU4SW.

	\note The bits that are to be packed must not span more than one data word

	\param pkAdr INOUT 	a pointer to the array.
	\param ofs   IN 	the bit offset in the array.
	\param num   IN 	the number of bits to extract.
	\param word  IN 	the word to set
*/
#define GPSSFRSW_R(pkAdr, ofs, num, word)  										\
		pkAdr[(ofs) / 30] = 													\
			(pkAdr[(ofs) / 30] & ~(MASK(num) << (30 - ((ofs) % 30) - (num)))) | \
			((((U4)(word)) & MASK(num)) << (30 - ((ofs) % 30) - (num)));

//! Bits Pack for a GPS Subframe format	(double word)
/**
	Reverse function of #GPSSFRI4DW and #GPSSFRU4DW.
	
	\param pkAdr INOUT 	a pointer to the array.
	\param ofs   IN 	the bit offset in the array.
	\param num   IN 	the number of bits to extract.
	\param word  IN 	the word to set
*/ 
#define GPSSFRDW_R(pkAdr, ofs, num, word)  								\
		GPSSFRSW_R(pkAdr, (ofs),  										\
				  (24 - MIN(24,(ofs) % 30)),							\
				  (U4)(word) >> ((num) - (24 - MIN(24,((ofs) % 30)))));	\
		GPSSFRSW_R(pkAdr, (ofs) + (30 - ((ofs) % 30)), 					\
				  (num) - (24 - MIN(24,(ofs) % 30)),					\
				  word);

//! pack transmission tow to HOW
/*! 
	reverse function of #HOWTOW.
*/
#define HOWTOW_R(pkAdr,word) GPSSFRSW_R(pkAdr, 30, 17, (word)/6)

//@}


#endif	// __NAV_GPS_MSGMACROS_H__
