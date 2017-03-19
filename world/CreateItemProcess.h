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

#ifndef CREATEITEMPROCESS_H
#define CREATEITEMPROCESS_H

#include "Process.h"

class CreateItemProcess : public Process
{
public:
	// p_dynamic_class stuff
	ENABLE_RUNTIME_CLASSTYPE();

	CreateItemProcess();
	CreateItemProcess(uint32_t shape, uint32_t frame, uint16_t quality,
					  uint16_t flags, uint16_t npcnum, uint16_t mapnum,
					  uint32_t extendedflags, int32_t x, int32_t y, int32_t z);
	virtual ~CreateItemProcess(void);

	virtual void run();

	bool loadData(IDataSource* ids, uint32_t version);
protected:
	virtual void saveData(ODataSource* ods);

	uint32_t shape;
	uint32_t frame;
	uint16_t quality;
	uint16_t flags;
	uint16_t npcnum;
	uint16_t mapnum;
	uint32_t extendedflags;
	int32_t x,y,z;
};

#endif
