/*
Copyright (C) 2005 The Pentagram team

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
#ifndef SONARCAUDIOSAMPLE_H_INCLUDED
#define SONARCAUDIOSAMPLE_H_INCLUDED

#include "AudioSample.h"

namespace Pentagram {

class SonarcAudioSample : public AudioSample
{
	struct SonarcDecompData {
		uint32_t		pos;
		uint32_t		sample_pos;
	};

	static bool	GeneratedOneTable;
	static int	OneTable[256];

	static void GenerateOneTable();

	static void decode_EC(int mode, int samplecount,
						const uint8_t* source, int sourcesize,
						uint8_t* dest);
	static void decode_LPC(int order, int nsamples,
						uint8_t* dest, const uint8_t* factors);
	static int audio_decode(const uint8_t* source, uint8_t* dest);

	uint32_t		src_offset;

public:
	SonarcAudioSample(uint8_t *buffer_, uint32_t size_);
	virtual ~SonarcAudioSample(void);

	virtual void initDecompressor(void *DecompData) const;
	virtual uint32_t decompressFrame(void *DecompData, void *samples) const;
	virtual void rewind(void *DecompData) const;
};

};

#endif
