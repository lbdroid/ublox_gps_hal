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
 * Project: Windows 7 Driver
 *
 ******************************************************************************/
/*!
  \file
  \brief 
*/
/*******************************************************************************
 * $Id: datavar.h 63729 2012-11-29 09:08:13Z michael.ammann $
 * $HeadURL: http://10.64.255.33/GPS/SOFTWARE/PRODUCTS/Windows7Driver/Parser/database.h $
 ******************************************************************************/
#ifndef __DATAVAR_H__
#define __DATAVAR_H__

typedef struct {
	unsigned short wYear;
	unsigned short wMonth;
	unsigned short wDay;
	unsigned short wHour;
	unsigned short wMinute;
	unsigned long  lMicroseconds;
} TIMESTAMP;

class CVar
{
public:

	CVar()										{ memset(&m_value, 0, sizeof(m_value)); m_type = EMPTY; }
	void Empty(void)							{ memset(&m_value, 0, sizeof(m_value)); m_type = EMPTY; }
	bool IsEmpty(void) const 					{ return m_type == EMPTY; }
	__drv_floatUsed void Set(double d)			{ m_type = R8;		m_value.d = d; }
	__drv_floatUsed void Set(float f)			{ m_type = R4;		m_value.f = f; }
	void Set(char i)							{ m_type = I4;		m_value.i = i; }
	void Set(short i)							{ m_type = I4;		m_value.i = i; }
	void Set(int i)								{ m_type = I4;		m_value.i = i; }
	void Set(unsigned char u)					{ m_type = UI4;		m_value.u = u; }
	void Set(unsigned short u)					{ m_type = UI4;		m_value.u = u; }
	void Set(unsigned int u)					{ m_type = UI4;		m_value.u = u; }
	void Set(bool b)							{ m_type = BOOL;	m_value.b = b; }
	//lint -e{1746} parameter 'ts' in function 'CVar::Set(TIMESTAMP)' could be made const reference
	void Set(const TIMESTAMP ts)				{ m_type = TIME;	m_value.ts = ts; }
	bool IsSet(void) const						{ return m_type != EMPTY; }
	
	__drv_floatUsed bool Check(const double v) const
	{
		if (m_type == EMPTY)
			return true;
		double d;
		if (m_type == R8)
		{
			double err = v - m_value.d;
			return (-1e-13 < err) && (err < 1e13);
		}
		if (Get(d))
		{
			double err = v - d;
			return (-1e-13 < err) && (err < 1e13);
		}
		return false;
	}

	bool Check(const int v) const
	{
		if (m_type == EMPTY)
			return true;
		if (m_type == I4)
			return m_value.i == v;
		if (m_type == UI4)
			return (v >= 0) && (m_value.u == (unsigned int)v);
		//PARSER_ASSERT(0);
		int i;
		if (Get(i))
			return i == v;
		return false;
	}

	bool Check(const unsigned int v) const
	{
		if (m_type == EMPTY)
			return true;
		if (m_type == UI4)
			return m_value.u == v;
		if (m_type == I4)
			return (m_value.i >= 0) && ((unsigned int)m_value.i == v);
		//PARSER_ASSERT(0);
		unsigned int i;
		if (Get(i))
			return i == v;
		return false;
	}

	bool Get(TIMESTAMP& ts)	const 	
	{
		if (m_type == TIME) 
		{
			ts = m_value.ts;
			return true;
		}
		return false;
	}

	template<typename T>
	__drv_floatUsed bool Get(T& v) const		
	{ 
		if (m_type == R8) 
			v = (T)m_value.d;
		else if (m_type == I4) 
			v = (T)m_value.i;
		else if (m_type == UI4) 
			v = (T)m_value.u;
		else if (m_type == R4) 
			v = (T)m_value.f;
		else if (m_type == BOOL) 
			v = (T)m_value.b;
		else // if (type == EMPTY)
			return false;
		return true;
	}
	
protected:
	enum { EMPTY, R8, R4, I4, UI4, BOOL, TIME } m_type;
	union {
		double			d;
		float			f;
		int				i;
		unsigned int	u;
		bool			b;
		TIMESTAMP		ts; 
	} m_value;
};

#endif // __DATAVAR_H__

