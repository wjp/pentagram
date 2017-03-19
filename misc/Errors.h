/*
Copyright (C) 2002 The Pentagram Team

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

#ifndef ERRORS_H_INCLUDED
#define ERRORS_H_INCLUDED


////////////////////////////////
//                            //
// Pentagram Base Error Codes //
//                            //
////////////////////////////////

// Base Error Code type
struct ECode
{
	int32_t	error;

	ECode() : error(0) { }
	ECode(int32_t _e) : error(_e) { }

	bool failed() const { return error != 0; }
	bool succeded() const { return error == 0; }

	ECode &operator = (int32_t &_e)
	{
		error = _e;
		return (*this);
	}

	ECode &operator = (ECode &_e)
	{
		error = _e.error;
		return (*this);
	}

	bool operator != (int32_t &_e)
	{
		return error != _e;
	}

	friend bool operator != (int32_t &_e, ECode &_e2)
	{
		return _e2.error != _e;
	}

	bool operator == (int32_t &_e)
	{
		return error == _e;
	}

	friend bool operator == (int32_t &_e, ECode &_e2)
	{
		return _e2.error == _e;
	}
};


//
// Helper Macros
//

// Checks to see if a return code is an error
#define P_FAILED(e) ((e)!=0)

// Checks to see if an error code indicates success
#define P_SUCCEDED(e) ((e)==0)


// 
// Error Code Bases
//

// Kernel Error Code Base
#define KERNEL_ERROR_BASE					(-0x10000)

// Usecode Error Code Base
#define UESCODE_ERROR_BASE					(-0x20000)

// FileSystem Error Code Base
#define FILESYS_ERROR_BASE					(-0x30000)

// Graphics Error Code Base
#define GRAPHICS_ERROR_BASE					(-0x40000)

// Gump Error Code Base
#define GUMPS_ERROR_BASE					(-0x50000)

// Convert Error Code Base
#define CONVERT_ERROR_BASE					(-0x60000)

// World Error Code Base
#define WORLD_ERROR_BASE					(-0x70000)

// Audio Error Code Base
#define AUDIO_ERROR_BASE					(-0x80000)

// Misc Error Code Base
#define MISC_ERROR_BASE						(-0x90000)


// 
// Basic Error Codes
//

// No Error
#define P_NO_ERROR							(0)

// Generic Error
#define P_GENERIC_ERROR						(-1)

// Undefined Error
#define P_UNDEFINED_ERROR					(-2)

// Memory Allocation Error
#define P_MEMORY_ALLOC_ERRO					(-3)


#endif // ERRORS_H_INCLUDED

