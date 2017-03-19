/*
Copyright (C) 2002-2004 The Pentagram team

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

#ifndef BaseUCStack_H
#define BaseUCStack_H

class IDataSource;
class ODataSource;

// A little-endian stack for use with usecode
class BaseUCStack
{
protected:
	uint8_t* buf;
	uint8_t* buf_ptr;
	uint32_t size;
public:

	BaseUCStack(uint32_t len, uint8_t *b) : buf(b), size(len)
	{
		// stack grows downward, so start at the end of the buffer
		buf_ptr = buf + size;
	}
	virtual ~BaseUCStack() { }

	inline uint32_t getSize() const {
		return size;
	}

	inline uint32_t stacksize() const {
		return size - (buf_ptr - buf);
	}

	inline void addSP(const int32_t offset) {
		buf_ptr += offset;
	}

	inline unsigned int getSP() const {
		return static_cast<unsigned int>(buf_ptr - buf);
	}

	inline void setSP(unsigned int pos) {
		buf_ptr = buf + pos;
	}

	//
	// Push values to the stack
	//

	inline void push1(uint8_t val) {
		buf_ptr--;
		buf_ptr[0] = val;
	}
	
	inline void push2(uint16_t val) {
		buf_ptr-=2;
		buf_ptr[0] = static_cast<uint8_t>( val     & 0xFF);
		buf_ptr[1] = static_cast<uint8_t>((val>>8) & 0xFF);
	}
	inline void push4(uint32_t val) {
		buf_ptr-=4;
		buf_ptr[0] = static_cast<uint8_t>( val      & 0xFF);
		buf_ptr[1] = static_cast<uint8_t>((val>>8)  & 0xFF);
		buf_ptr[2] = static_cast<uint8_t>((val>>16) & 0xFF);
		buf_ptr[3] = static_cast<uint8_t>((val>>24) & 0xFF);
	}
	// Push an arbitrary number of bytes of 0
	inline void push0(const uint32_t size) { 
		buf_ptr -= size;
		std::memset (buf_ptr, 0, size);
	}
	// Push an arbitrary number of bytes
	inline void push(const uint8_t *in, const uint32_t size) { 
		buf_ptr -= size;
		std::memcpy (buf_ptr, in, size);
	}

	//
	// Pop values from the stack
	//

	inline uint16_t pop2() {
		uint8_t b0, b1;
		b0 = *buf_ptr++;
		b1 = *buf_ptr++;
		return (b0 | (b1 << 8));
	}
	inline uint32_t pop4() {
		uint8_t b0, b1, b2, b3;
		b0 = *buf_ptr++;
		b1 = *buf_ptr++;
		b2 = *buf_ptr++;
		b3 = *buf_ptr++;
		return (b0 | (b1<<8) | (b2<<16) | (b3<<24));
	}
	inline void pop(uint8_t *out, const uint32_t size) {
		std::memcpy(out, buf_ptr, size);
		buf_ptr += size;
	}

	//
	// Access a value from a location in the stacck
	//

	inline uint8_t access1(const uint32_t offset) const {
		return buf[offset];
	}
	inline uint16_t access2(const uint32_t offset) const {
		return (buf[offset] | (buf[offset+1] << 8));
	}
	inline uint32_t access4(const uint32_t offset) const {
		return buf[offset] | (buf[offset+1]<<8) |
			(buf[offset+2]<<16) | (buf[offset+3]<<24);
	}
	inline uint8_t* access(const uint32_t offset) {		
		return buf+offset;
	}
	inline uint8_t* access() {
		return buf_ptr;
	}

	//
	// Assign a value to a location in the stack
	//

	inline void assign1(const uint32_t offset, const uint8_t val) {
		const_cast<uint8_t*>(buf)[offset]   = static_cast<uint8_t>( val     & 0xFF);
	}
	inline void assign2(const uint32_t offset, const uint16_t val) {
		const_cast<uint8_t*>(buf)[offset]   = static_cast<uint8_t>( val     & 0xFF);
		const_cast<uint8_t*>(buf)[offset+1] = static_cast<uint8_t>((val>>8) & 0xFF);
	}
	inline void assign4(const uint32_t offset, const uint32_t val) {
		const_cast<uint8_t*>(buf)[offset]   = static_cast<uint8_t>( val      & 0xFF);
		const_cast<uint8_t*>(buf)[offset+1] = static_cast<uint8_t>((val>>8)  & 0xFF);
		const_cast<uint8_t*>(buf)[offset+2] = static_cast<uint8_t>((val>>16) & 0xFF);
		const_cast<uint8_t*>(buf)[offset+3] = static_cast<uint8_t>((val>>24) & 0xFF);
	}
	inline void assign(const uint32_t offset, const uint8_t *in, const uint32_t len)
	{
		std::memcpy (const_cast<uint8_t*>(buf)+offset, in, len);
	}
};

class DynamicUCStack : public BaseUCStack
{
public:
	DynamicUCStack(uint32_t len=0x1000) : BaseUCStack(len, new uint8_t[len]) { }
	virtual ~DynamicUCStack() { delete [] buf; }

#ifdef USE_DYNAMIC_UCSTACK
#define UCStack DynamicUCStack
	void save(ODataSource* ods);
	bool load(IDataSource* ids, uint32_t version);
#endif
};

#ifndef USE_DYNAMIC_UCSTACK
class UCStack : public BaseUCStack
{
	uint8_t	buf_array[0x1000];
public:
	UCStack() : BaseUCStack(0x1000, buf_array) { }
	virtual ~UCStack() { }

	void save(ODataSource* ods);
	bool load(IDataSource* ids, uint32_t version);
};
#endif

#endif
