/*
 *  Copyright (C) 2003  Pentagram Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef XFORMBLEND_H
#define XFORMBLEND_H

#ifndef P_FASTCALL
#ifdef _MSC_VER
#define P_FASTCALL __fastcall
#elif (defined(WIN32) && defined(FASTCALL))
#define P_FASTCALL FASTCALL
#else
#define P_FASTCALL
#endif
#endif

typedef uint32 (P_FASTCALL * xformBlendFuncType)(uint32 col); 
extern const xformBlendFuncType		U8XFormFuncs[256];

uint32 P_FASTCALL BlendInvisible(uint32 src, uint32 dst); 

#endif
