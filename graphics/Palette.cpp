/*
Copyright (C) 2003 The Pentagram team

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

#include "pent_include.h"

#include "Palette.h"
#include "IDataSource.h"

void Palette::load(IDataSource& ds, const xformBlendFuncType *xff)
{
	xform_funcs = xff;

	ds.read(palette, 768);

	// convert from 0-63 to 0-255 palette
	for (int i = 0; i < 256; i++)
	{
		palette[i*3] = (palette[i*3]*255)/63;
		palette[i*3+1] = (palette[i*3+1]*255)/63;
		palette[i*3+2] = (palette[i*3+2]*255)/63;
	}
}