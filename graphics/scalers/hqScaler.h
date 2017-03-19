/*
Copyright (C) 2005 The Pentagram Team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef HQSCALER_H_INCLUDED
#define HQSCALER_H_INCLUDED

#if defined(USE_HQ2X_SCALER) || defined(USE_HQ3X_SCALER) || defined(USE_HQ4X_SCALER)

#include "Scaler.h"

namespace Pentagram {

class hqScaler : public Scaler
{
public:
	hqScaler();

	virtual const uint32_t	ScaleBits() const;			//< bits for supported integer scaling
	virtual const bool		ScaleArbitrary() const;		//< supports arbitrary scaling of any degree 

	virtual const char *	ScalerName() const;			//< Name Of the Scaler (1 word)
	virtual const char *	ScalerDesc() const;			//< Desciption of the Scaler
	virtual const char *	ScalerCopyright() const;	//< Scaler Copyright info

private:
	static bool _Scale16Nat ( Texture *tex, int32_t sx, int32_t sy, int32_t sw, int32_t sh, 
					uint8_t* pixel, int32_t dw, int32_t dh, int32_t pitch, bool clamp_src);
	static bool _Scale16Sta ( Texture *tex, int32_t sx, int32_t sy, int32_t sw, int32_t sh, 
					uint8_t* pixel, int32_t dw, int32_t dh, int32_t pitch, bool clamp_src);

	static bool _Scale32Nat ( Texture *tex, int32_t sx, int32_t sy, int32_t sw, int32_t sh, 
					uint8_t* pixel, int32_t dw, int32_t dh, int32_t pitch, bool clamp_src);
	static bool _Scale32Sta ( Texture *tex, int32_t sx, int32_t sy, int32_t sw, int32_t sh, 
					uint8_t* pixel, int32_t dw, int32_t dh, int32_t pitch, bool clamp_src);
	static bool _Scale32_A888 ( Texture *tex, int32_t sx, int32_t sy, int32_t sw, int32_t sh, 
					uint8_t* pixel, int32_t dw, int32_t dh, int32_t pitch, bool clamp_src);
	static bool _Scale32_888A ( Texture *tex, int32_t sx, int32_t sy, int32_t sw, int32_t sh, 
					uint8_t* pixel, int32_t dw, int32_t dh, int32_t pitch, bool clamp_src);
};

extern const hqScaler hq_scaler;

};

#endif

#endif
