/*
Copyright (C) 2003-2005 The Pentagram team

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

#ifndef PATHFINDERPROCESS_H
#define PATHFINDERPROCESS_H

#include "Process.h"

#include "Pathfinder.h"
class Actor;

class PathfinderProcess : public Process
{
public:
	PathfinderProcess();
	PathfinderProcess(Actor* actor, ObjId item, bool hit=false);
	PathfinderProcess(Actor* actor, int32_t x, int32_t y, int32_t z);
	virtual ~PathfinderProcess();

	// p_dynamic_cast stuff
	ENABLE_RUNTIME_CLASSTYPE();

	virtual void run();
	virtual void terminate();

//	virtual void terminate();

	bool loadData(IDataSource* ids, uint32_t version);
protected:
	virtual void saveData(ODataSource* ods);

	int32_t targetx, targety, targetz;
	ObjId targetitem;
	bool hitmode;

	std::vector<PathfindingAction> path;
	unsigned int currentstep;
};


#endif
