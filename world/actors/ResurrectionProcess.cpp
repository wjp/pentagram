/*
Copyright (C) 2004 The Pentagram team

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

#include "ResurrectionProcess.h"
#include "World.h"
#include "Actor.h"

#include "IDataSource.h"
#include "ODataSource.h"

// p_dynamic_cast stuff
DEFINE_RUNTIME_CLASSTYPE_CODE(ResurrectionProcess,Process);

ResurrectionProcess::ResurrectionProcess() : Process()
{

}

ResurrectionProcess::ResurrectionProcess(Actor* actor_)
{
	assert(actor_);
	item_num = actor_->getObjId();

	type = 0x229; // CONSTANT !
}

bool ResurrectionProcess::run(const uint32 framenum)
{
	Actor *a = World::get_instance()->getNPC(item_num);

	if (!a) {
		// actor gone... too late for resurrection now :-)
		terminate();
		return false;
	}

	if (!(a->getActorFlags() & Actor::ACT_DEAD)) {
		// not dead?
		terminate();
		return false;
	}

	if (a->getFlags() & Item::FLG_GUMP_OPEN) {
		// first close gump in case player is still rummaging through us
		a->closeGump();
	}

	a->clearActorFlag(Actor::ACT_WITHSTANDDEATH);
	a->clearActorFlag(Actor::ACT_DEAD);

	// reload stats
	if (!a->loadMonsterStats()) {
		perr << "ResurrectionProcess::run failed to reset stats for actor ("
			 << a->getShape() << ")." << std::endl;
	}

	// done
	terminate();

	return true;
}

void ResurrectionProcess::saveData(ODataSource* ods)
{
	ods->write2(1); //version
	Process::saveData(ods);
}

bool ResurrectionProcess::loadData(IDataSource* ids)
{
	uint16 version = ids->read2();
	if (version != 1) return false;
	if (!Process::loadData(ids)) return false;

	return true;
}