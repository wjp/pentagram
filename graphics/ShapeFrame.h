/*
Copyright (C) 2003 The Pentagram Team

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

#ifndef SHAPEFRAME_H
#define SHAPEFRAME_H

struct ConvertShapeFormat;
struct ConvertShapeFrame;

class ShapeFrame 
{
public:

	// parse data. 
	//
	// You will find this is quite similar to the ConvertShapeFrame except 
	// all the unknown crap is removed. It's designed to allow for painting
	// only, and for speed when loading.
	
	ShapeFrame(const uint8_t* data, uint32_t size, const ConvertShapeFormat* format=0,
		const uint8_t special[256]=0, ConvertShapeFrame *prev=0);
	~ShapeFrame();

	uint32_t				compressed;
	int32_t				width, height;
	int32_t				xoff, yoff;

	uint32_t				*line_offsets;		// Note these are offsets into rle_data
	const uint8_t			*rle_data;

	bool hasPoint(int32_t x, int32_t y) const;	// Check to see if a point is in the frame

	uint8_t getPixelAtPoint(int32_t x, int32_t y) const;	// Get the pixel at the point 

	void getConvertShapeFrame(ConvertShapeFrame &csf, bool need_bytes_rle=false);
protected:

	// This will load a u8 style shape 'optimized'.
	void LoadU8Format(const uint8_t* data, uint32_t size);

	// This will load a pentagram style shape 'optimized'.
	void LoadPentagramFormat(const uint8_t* data, uint32_t size);

	// This will load any sort of shape via a ConvertShapeFormat struct
	// Crusader shapes must be loaded this way
	void LoadGenericFormat(const uint8_t* data, uint32_t size, const ConvertShapeFormat* format);

	// This will load a u8-compressed shape
	void LoadU8CMPFormat(const uint8_t* data, uint32_t size, const ConvertShapeFormat* format, const uint8_t special[256], ConvertShapeFrame *prev);
};


#endif
