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

#ifndef CURRENTMAP_H
#define CURRENTMAP_H

#include <list>
#include "intrinsics.h"

class Map;
class Item;
class UCList;
class TeleportEgg;
class EggHatcherProcess;
class IDataSource;
class ODataSource;

#define MAP_NUM_CHUNKS	64

class CurrentMap
{
	friend class World;
public:
	CurrentMap();
	~CurrentMap();

	void clear();
	void writeback();
	void loadMap(Map* map);

	//! sets the currently loaded map, without any processing.
	//! (Should only be used for loading.)
	void setMap(Map* map) { current_map = map; }

	//! Get the map number of the CurrentMap
	uint32_t getNum() const;

	unsigned int getChunkSize() const { return mapChunkSize; }

	//! Add an item to the beginning of the item list
	void addItem(Item* item);

	//! Add an item to the end of the item list
	void addItemToEnd(Item* item);

	void removeItemFromList(Item* item, int32_t oldx, int32_t oldy);
	void removeItem(Item* item);

	//! Update the fast area for the cameras position
	void updateFastArea(int32_t from_x, int32_t from_y, int32_t from_z, int32_t to_x, int32_t to_y, int32_t to_z);

	//! search an area for items matching a loopscript
	//! \param itemlist the list to return objids in
	//! \param loopscript the script to check items against
	//! \param scriptsize the size (in bytes) of the loopscript
	//! \param item the item around which you want to search, or 0.
	//!             if item is 0, search around (x,y)
	//! \param range the (square) range to search
	//! \param recurse if true, search in containers too
	//! \param x x coordinate of search center if item is 0.
	//! \param y y coordinate of search center if item is 0.
	void areaSearch(UCList* itemlist, const uint8_t* loopscript,
					uint32_t scriptsize, Item* item, uint16_t range, bool recurse,
					int32_t x=0, int32_t y=0);

	// Surface search: Search above and below an item.
	void surfaceSearch(UCList* itemlist, const uint8_t* loopscript,
					uint32_t scriptsize, Item* item, bool above, bool below,
					bool recurse=false);

	// Surface search: Search above and below an item.
	void surfaceSearch(UCList* itemlist, const uint8_t* loopscript,
					uint32_t scriptsize, ObjId id,
					int32_t origin[3], int32_t dims[2],
					bool above, bool below, bool recurse=false);

	// Collision detection. Returns true if the box [x,y,z]-[x-xd,y-yd,z+zd]
	// does not collide with any solid items.
	// Additionally, if support is not NULL, *support is set to the item
	// supporting the given box, or 0 if it isn't supported.
	// If under_roof is not NULL, *roof is set to the roof item with the lowest
	// z coordinate that's over the box, or 0 if there is no roof above box.
	// Ignores collisions which were already occurring at the start position.
	// NB: isValidPosition doesn't consider item 'item'.
	bool isValidPosition(int32_t x, int32_t y, int32_t z,
						 int32_t startx, int32_t starty, int32_t startz,
						 int xd, int yd, int zd, uint32_t shapeflags,
						 ObjId item,
						 Item** support=0, ObjId* roof=0);

	// Note that this version of isValidPosition does not look for start
	// position collisions.
	bool isValidPosition(int32_t x, int32_t y, int32_t z,
						 int xd, int yd, int zd, uint32_t shapeflags,
						 ObjId item,
						 Item** support=0, ObjId* roof=0);

	// Note that this version of isValidPosition can not take 'flipped'
	// into account!
	bool isValidPosition(int32_t x, int32_t y, int32_t z, uint32_t shape,
						 ObjId item, Item** support=0, ObjId* roof=0);

	//! Scan for a valid position for item in directions orthogonal to movedir
	bool scanForValidPosition(int32_t x, int32_t y, int32_t z, Item* item,
							  int movedir, bool wantsupport,
							  int32_t& tx, int32_t& ty, int32_t& tz);

	struct SweepItem {
		SweepItem(ObjId it, int32_t ht, int32_t et, bool touch,
				  bool touchfloor, bool block, uint8_t dir)
			: item(it), hit_time(ht), end_time(et), touching(touch),
			  touching_floor(touchfloor), blocking(block), dirs(dir) { }

		ObjId	item;		// Item that was hit

		//
		// The time values here are 'normalized' fixed point values
		// They range from 0 for the start of the move to 0x4000 for the end of
		// The move.
		//
		// Linear interpolate between the start and end positions using
		// hit_time to find where the moving item was when the hit occurs
		//

		int32_t	hit_time;	// if -1, already hitting when sweep started. 
		int32_t	end_time;	// if 0x4000, still hitting when sweep finished

		bool	touching;	// We are only touching (don't actually overlap)
		bool	touching_floor; // touching and directly below the moving item

		bool	blocking;	// This item blocks the moving item

		uint8_t	dirs; // Directions in which the item is being hit.
				      // Bitmask. Bit 0 is x, 1 is y, 2 is z.

		// Use this func to get the interpolated location of the hit
		void GetInterpolatedCoords(int32_t out[3], int32_t start[3], int32_t end[3])
		{
			for (int i = 0; i < 3; i++)
				out[i] = start[i] + ((end[i]-start[i])*(hit_time>=0?hit_time:0)+(end[i] > start[i] ? 0x2000 : -0x2000) )/0x4000;
		}
	};

	//! Perform a sweepTest for an item move
	//! \param start Start point to sweep from.
	//! \param end End point to sweep to.
	//! \param dims Bounding size of item to check.
	//! \param shapeflags shapeflags of item to check.
	//! \param item ObjId of the item being checked. This will allow item to
	//!             be skipped from being tested against. Use 0 for no item.
	//! \param solid_only If true, only test solid items.
	//! \param hit Pointer to a list to fill with items hit. Items are sorted
	//!            by SweepItem::hit_time
	//! \return false if no items were hit.
	//!         true if any items were hit.
	bool sweepTest(const int32_t start[3], const int32_t end[3],
				   const int32_t dims[3], uint32_t shapeflags,
				   ObjId item, bool solid_only, std::list<SweepItem> *hit);

	TeleportEgg* findDestination(uint16_t id);

	// Not allowed to modify the list. Remember to use const_iterator
	const std::list<Item*>* getItemList (int32_t gx, int32_t gy)
	{
		// CONSTANTS!
		if (gx < 0 || gy < 0 || gx >= MAP_NUM_CHUNKS || gy >= MAP_NUM_CHUNKS) 
			return 0;
		return &items[gx][gy];
	}

	bool isChunkFast(int32_t cx, int32_t cy)
	{
		// CONSTANTS!
		if (cx < 0 || cy < 0 || cx >= MAP_NUM_CHUNKS || cy >= MAP_NUM_CHUNKS) 
			return false;
		return (fast[cy][cx/32]&(1<<(cx&31))) != 0;
	}

	// A simple trace to find the top item at a specific xy point
	Item *traceTopItem(int32_t x, int32_t y, int32_t ztop, int32_t zbot, ObjId ignore, uint32_t shflags);

	// Set the entire map as being 'fast' 
	void setWholeMapFast();

	void save(ODataSource* ods);
	bool load(IDataSource* ids, uint32_t version);

	INTRINSIC(I_canExistAt);

private:
	void loadItems(std::list<Item*> itemlist, bool callCacheIn);
	void createEggHatcher();

	Map* current_map;

	// item lists. Lots of them :-)
	// items[x][y]
	std::list<Item*>** items;

	ProcId egghatcher;

	// Fast area bit masks -> fast[ry][rx/32]&(1<<(rx&31));
	uint32_t** fast;	
	int32_t fast_x_min, fast_y_min, fast_x_max, fast_y_max;

	int mapChunkSize;

	void setChunkFast(int32_t cx, int32_t cy);
	void unsetChunkFast(int32_t cx, int32_t cy);
};

#endif
