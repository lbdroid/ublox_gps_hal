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
  \brief  time utility library

  Module for time functions, header
*/
/*******************************************************************************
 * $Id: ubx_cfg.h 63615 2012-11-27 10:12:42Z andrea.foni $
 ******************************************************************************/
#ifndef __UBX_CFG_H__
#define __UBX_CFG_H__

class CCfg
{
public:
    CCfg();
	~CCfg();
	void load(const char* name);
	int get(const char* item, int def)const ;
	const char* get(const char* item, const char* def) const;
protected:
	enum { MAX_ITEM = 100 };
	int m_num;
	char* m_itemName[MAX_ITEM];
	char* m_itemData[MAX_ITEM];
};

#endif /* __UBX_CFG_H__ */
