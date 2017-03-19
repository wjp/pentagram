/*
Copyright (C) 2004 The Pentagram Team

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

#ifndef FRAMEID_H
#define FRAMEID_H

class IDataSource;
class ODataSource;

struct FrameID
{
	uint16_t flexid;
	uint32_t shapenum;
	uint32_t framenum;

	FrameID() { }
	FrameID(uint16_t flex, uint32_t shape, uint32_t frame)
		: flexid(flex), shapenum(shape), framenum(frame)
		{ }

	void save(ODataSource* ods);
	bool load(IDataSource* ids);
};

#endif
