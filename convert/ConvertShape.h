/*
 *  Copyright (C) 2002, 2003 The Pentagram Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef CONVERTSHAPE_H
#define CONVERTSHAPE_H

class IDataSource;
class ODataSource;

// Convert shape C

/********** Update AutoShapeFormat in shapeconv/ShapeConv.cpp when changed! **********/
struct ConvertShapeFormat
{
	const char *		name;
													//	U8		U8 Gump	U8.SKF	Cru		Cru2D	Pent	Comp
	uint32_t				len_header;					//	6		6		2		6		6		8		11
	const char *		ident;						//  ""		""		"\2\0"	""		""		"PSHP"	""
	uint32_t				bytes_ident;				//	0		0		2		0		0		4		0
	uint32_t				bytes_special;				//	0		0		0		0		0		0		5
	uint32_t				bytes_header_unk;			//	4		4		0		4		4		0		4
	uint32_t				bytes_num_frames;			//	2		2		0		2		2		4		2

	uint32_t				len_frameheader;			//	6		6		0		8		8		8		6
	uint32_t				bytes_frame_offset;			//	3		3		0		3		3		4		4
	uint32_t				bytes_frameheader_unk;		//	1		2		0		2		2		0		0
	uint32_t				bytes_frame_length;			//	2		2		0		3		3		4		2
	uint32_t				bytes_frame_length_kludge;	//	0		8		0		0		0		0		0

	uint32_t				len_frameheader2;			//	18		18		10		28		20		20		10
	uint32_t				bytes_frame_unknown;		//	8		8		0		8		0		0		0
	uint32_t				bytes_frame_compression;	//	2		2		2		4		4		4		2
	uint32_t				bytes_frame_width;			//	2		2		2		4		4		4		2
	uint32_t				bytes_frame_height;			//	2		2		2		4		4		4		2
	uint32_t				bytes_frame_xoff;			//	2		2		2		4		4		4		2
	uint32_t				bytes_frame_yoff;			//	2		2		2		4		4		4		2

	uint32_t				bytes_line_offset;			//	2		2		2		4		4		4		0
	uint32_t				line_offset_absolute;		//	0		0		0		0		0		1		0
};

// ConvertShapeFrame structure

struct ConvertShapeFrame 
{
	uint8_t				header_unknown[2];

	uint8_t				unknown[8];
	uint32_t				compression;
	int32_t				width;
	int32_t				height;
	int32_t				xoff;
	int32_t				yoff;

	uint32_t				*line_offsets;		// Note these are offsets into rle_data

	int32_t				bytes_rle;			// Number of bytes of RLE Data
	uint8_t				*rle_data;

	void Free()
	{
		delete [] line_offsets;
		line_offsets = 0;

		delete [] rle_data;
		rle_data = 0;
	}

	void Read(IDataSource *source, const ConvertShapeFormat *csf, uint32_t frame_length);

	void ReadCmpFrame(IDataSource *source, const ConvertShapeFormat *csf, const uint8_t special[256], ConvertShapeFrame *prev);

	void GetPixels(uint8_t *buf, int32_t count, int32_t x, int32_t y);
};


// ConvertShape structure

class ConvertShape
{
	uint8_t				header_unknown[4];
	uint32_t				num_frames;
	ConvertShapeFrame	*frames;

public:
	ConvertShape() : num_frames(0), frames(0)
	{
	}

	~ConvertShape()
	{
		Free();
	}

	void Free()
	{
		if (frames)
			for(uint32_t i = 0; i < num_frames; ++i)
				frames[i].Free();
		
		delete [] frames;
		frames = 0;
		num_frames = 0;
	}


	void Read(IDataSource *source, const ConvertShapeFormat *csf, uint32_t real_len);
	void Write(ODataSource *source, const ConvertShapeFormat *csf, uint32_t &write_len);

	// This will check to see if a Shape is of a certain type. Return true if ok, false if bad
	static bool Check(IDataSource *source, const ConvertShapeFormat *csf, uint32_t real_len);

	// This will also check to see if a shape is of a certain type. However it won't check
	// the rle data, it only checks headers. Return true if ok, false if bad
	static bool CheckUnsafe(IDataSource *source, const ConvertShapeFormat *csf, uint32_t real_len);

	// Algorithmically calculate the number of frames
	static int CalcNumFrames(IDataSource *source, const ConvertShapeFormat *csf, uint32_t real_len, uint32_t start_pos);
};

// Shape format configuration for Pentagram format
extern const ConvertShapeFormat		PentagramShapeFormat;

#endif //CONVERTSHAPE_H
