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

#include "pent_include.h"
#include "CreateItemProcess.h"
#include "ItemFactory.h"
#include "Item.h"

#include "IDataSource.h"
#include "ODataSource.h"

// p_dynamic_class stuff 
DEFINE_RUNTIME_CLASSTYPE_CODE(CreateItemProcess,Process);

CreateItemProcess::CreateItemProcess()
	: Process()
{

}

CreateItemProcess::CreateItemProcess(uint32_t shape_, uint32_t frame_,
									 uint16_t quality_, uint16_t flags_,
									 uint16_t npcnum_, uint16_t mapnum_,
									 uint32_t extendedflags_,
									 int32_t x_, int32_t y_, int32_t z_)
	: shape(shape_), frame(frame_), quality(quality_), flags(flags_),
	  npcnum(npcnum_), mapnum(mapnum_), extendedflags(extendedflags_),
	  x(x_), y(y_), z(z_)
{

}

CreateItemProcess::~CreateItemProcess(void)
{

}

void CreateItemProcess::run()
{
	Item *item = ItemFactory::createItem(shape, frame, quality, flags,
										 npcnum, mapnum, extendedflags, true);
	item->move(x,y,z);

	result = item->getObjId();

	terminate();
}

void CreateItemProcess::saveData(ODataSource* ods)
{
	Process::saveData(ods);

	ods->write4(shape);
	ods->write4(frame);
	ods->write2(quality);
	ods->write2(flags);
	ods->write2(npcnum);
	ods->write2(mapnum);
	ods->write4(extendedflags);
	ods->write4(static_cast<uint32_t>(x));
	ods->write4(static_cast<uint32_t>(y));
	ods->write4(static_cast<uint32_t>(z));
}

bool CreateItemProcess::loadData(IDataSource* ids, uint32_t version)
{
	if (!Process::loadData(ids, version)) return false;

	shape = ids->read4();
	frame = ids->read4();
	quality = ids->read2();
	flags = ids->read2();
	npcnum = ids->read2();
	mapnum = ids->read2();
	extendedflags = ids->read4();
	x = static_cast<int32_t>(ids->read4());
	y = static_cast<int32_t>(ids->read4());
	z = static_cast<int32_t>(ids->read4());
	return true;
}
