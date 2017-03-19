/*
Copyright (C) 2003-2004 The Pentagram Team

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

#ifndef SHAPE_H
#define SHAPE_H

#include <vector>

class ShapeFrame;
namespace Pentagram { struct Palette; struct Rect; }
struct ConvertShapeFormat;
class IDataSource;

class Shape 
{
public:
	// Parse data, create frames.
	// NB: Shape uses data without copying it. It is deleted on destruction
	// If format is not specified it will be autodetected
	Shape(const uint8_t* data, uint32_t size, const ConvertShapeFormat *format,
		const uint16_t flexId, const uint32_t shapenum);
	Shape(IDataSource *src, const ConvertShapeFormat *format);
	virtual ~Shape();
	void setPalette(const Pentagram::Palette* pal) { palette = pal; }
	const Pentagram::Palette* getPalette() const { return palette; }

	uint32_t frameCount() const { return static_cast<uint32_t>(frames.size()); }

	//! Returns the dimensions of all frames combined
	//! (w,h) = size of smallest rectangle covering all frames
	//! (x,y) = coordinates of origin relative to top-left point of rectangle
	void getTotalDimensions(int32_t& w, int32_t& h, int32_t& x, int32_t& y) const;

	ShapeFrame* getFrame(unsigned int frame)
		{ if (frame < frames.size()) return frames[frame]; else return 0; }
		
	void getShapeId(uint16_t & flexId, uint32_t & shapenum);

	// This will detect the format of a shape
	static const ConvertShapeFormat *DetectShapeFormat(const uint8_t* data, uint32_t size);
	static const ConvertShapeFormat *DetectShapeFormat(IDataSource *ds, uint32_t size);

	ENABLE_RUNTIME_CLASSTYPE();

	ENABLE_CUSTOM_MEMORY_ALLOCATION();

protected:

	// This will load a u8 style shape 'optimized'.
	void LoadU8Format(const uint8_t* data, uint32_t size, const ConvertShapeFormat* format);

	// This will load a pentagram style shape 'optimized'.
	void LoadPentagramFormat(const uint8_t* data, uint32_t size, const ConvertShapeFormat* format);

	// This will load any sort of shape via a ConvertShapeFormat struct
	// Crusader shapes must be loaded this way
	void LoadGenericFormat(const uint8_t* data, uint32_t size, const ConvertShapeFormat* format);

	std::vector<ShapeFrame*> frames;

	const Pentagram::Palette* palette;

	const uint8_t* data;
	uint32_t size;
	const uint16_t flexId;
	const uint32_t shapenum;
};


#endif
