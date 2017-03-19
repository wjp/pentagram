/*
Copyright (C) 2003-2007 The Pentagram team

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

#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <vector>
#include <queue>
#include <list>
#include "Animation.h"

class Actor;
class Item;

struct PathfindingState
{
	int32_t x, y, z;
	Animation::Sequence lastanim;
	uint32_t direction;
	bool flipped;
	bool firststep;
	bool combat;

	void load(Actor* actor);
	bool checkPoint(int32_t x_, int32_t y_, int32_t z_,int range);
	bool checkItem(Item* item, int xyRange, int zRange);
	bool checkHit(Actor* actor, Actor* target);
};

struct PathfindingAction
{
	Animation::Sequence action;
	uint32_t direction;
	uint32_t steps;
};

struct PathNode;

class PathNodeCmp {
public:
	bool operator()(PathNode* n1, PathNode* n2);
};

class Pathfinder
{
public:
	Pathfinder();
	~Pathfinder();

	void init(Actor* actor, PathfindingState* state=0);
	void setTarget(int32_t x, int32_t y, int32_t z);
	void setTarget(Item* item, bool hit=false);

	//! try to reach the target by pathfinding
	bool canReach();

	//! pathfind. If true, the found path is returned in path
	bool pathfind(std::vector<PathfindingAction>& path);

#ifdef DEBUG
	//! "visualDebug" console command
	static void ConCmd_visualDebug(const Console::ArgvType &argv);
	static ObjId visualdebug_actor;
#endif


protected:
	PathfindingState start;
	Actor* actor;
	int32_t targetx, targety, targetz;
	Item* targetitem;
	bool hitmode;
	int32_t expandtime;

	int32_t actor_xd,actor_yd,actor_zd;

	std::list<PathfindingState> visited;
	std::priority_queue<PathNode*,std::vector<PathNode*>,PathNodeCmp> nodes;

	std::list<PathNode*> nodelist;

	bool alreadyVisited(int32_t x, int32_t y, int32_t z);
	void newNode(PathNode* oldnode,PathfindingState& state,unsigned int steps);
	void expandNode(PathNode* node);
	unsigned int costHeuristic(PathNode* node);
	bool checkTarget(PathNode* node);
};

#endif
