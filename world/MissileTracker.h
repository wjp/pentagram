/*
Copyright (C) 2007 The Pentagram team

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

#ifndef MISSILETRACKER_H
#define MISSILETRACKER_H

class Item;

class MissileTracker
{
public:
	MissileTracker(Item* item, int32_t sx, int32_t sy, int32_t sz,
				   int32_t tx, int32_t ty, int32_t tz,
				   int32_t speed, int32_t gravity);
	MissileTracker(Item* item, int32_t tx, int32_t ty, int32_t tz,
				   int32_t speed, int32_t gravity);
	~MissileTracker();

	bool isPathClear();

	void launchItem();

protected:
	void init(int32_t sx, int32_t sy, int32_t sz, int32_t speed);

private:
	ObjId objid;
	int32_t destx, desty, destz;
	int32_t speedx, speedy, speedz;
	int32_t gravity;
	int frames;
};

#endif
