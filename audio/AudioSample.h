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
#ifndef AUDIOSAMPLE_H_INCLUDED
#define AUDIOSAMPLE_H_INCLUDED

class IDataSource;

namespace Pentagram {

class AudioSample
{
protected:
	uint32_t	sample_rate;
	uint32_t	bits;
	bool	stereo;
	int		frame_size;
	uint32_t	decompressor_size;
	uint32_t	length;

	uint32_t	buffer_size;
	uint8_t	*buffer;

public:
	AudioSample(uint8_t *buffer, uint32_t size);
	virtual ~AudioSample(void);

	inline uint32_t getRate() const { return sample_rate; }
	inline uint32_t getBits() const { return bits; }
	inline bool isStereo() const { return stereo; }
	inline uint32_t getFrameSize() const { return frame_size; }
	inline uint32_t getDecompressorDataSize() const { return decompressor_size; }

	//! get AudioSample length (in samples)
	inline uint32_t getLength() const { return length; }

	virtual void initDecompressor(void *DecompData) const = 0;
	virtual uint32_t decompressFrame(void *DecompData, void *samples) const = 0;
	virtual void rewind(void *DecompData) const = 0;
};

};

#endif //AUDIOSAMPLE_H_INCLUDED
