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
  \brief  Some definitions which extend the standard C language

  
*/
/*******************************************************************************
 * $Id: std_lang_def.h 63615 2012-11-27 10:12:42Z andrea.foni $
 ******************************************************************************/

#ifndef __STD_LANG_DEF_H__
#define __STD_LANG_DEF_H__ //!< multiple inclusion guard


/*! \name Constants for Boolean Values ( L , #L1 , #L2 , #L4 ) */
//@{
#if !defined FALSE
#define FALSE		0 			//!< Boolean FALSE Value
#endif

#if !defined TRUE
#define TRUE		1 			//!< Boolean TRUE Value
#endif

#if !defined CONFIRMED
#define CONFIRMED	2			//!< Trilian CONFIRMED Value
#endif

#define GUESSED		3			//!< Guessed Value
//@}

#ifndef NULL
#define NULL 		((void*)0)	//!< Some standard librarys like ADS do not define NULL. 
#endif

/*! \name Dummy Argument Direction Declarator Constants */
//@{
#define IN                       //!< Dummy Input Argument Declarator
#define OUT                      //!< Dummy Output Argument Declarator
#define INOUT                    //!< Dummy Input/Output Argument Declarator
//@}

#endif // __STD_LANG_DEF_H__

