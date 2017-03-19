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

#ifndef ITEMSORTER_H
#define ITEMSORTER_H

class MainShapeArchive;
class Item;
class RenderSurface;
struct SortItem;

class ItemSorter
{
	MainShapeArchive	*shapes;
	RenderSurface	*surf;

	SortItem	*items;
	SortItem	*items_tail;
	SortItem	*items_unused;
	int32_t		sort_limit;

	int32_t		order_counter;

	int32_t		cam_sx, cam_sy;

public:
	ItemSorter();
	~ItemSorter();

	enum HitFace {
		X_FACE, Y_FACE, Z_FACE
	};

	// Begin creating the display list
	void BeginDisplayList(RenderSurface*,
						  int32_t camx, int32_t camy, int32_t camz);

	void AddItem(int32_t x, int32_t y, int32_t z, uint32_t shape_num, uint32_t frame_num, uint32_t item_flags, uint32_t ext_flags, uint16_t item_num=0);
	void AddItem(Item *);					// Add an Item. SetupLerp() MUST have been called

	void PaintDisplayList(bool item_highlight=false);				// Finishes the display list and Paints

	// Trace and find an object. Returns objid.
	// If face is non-NULL, also return the face of the 3d bbox (x,y) is on
	uint16_t Trace(int32_t x, int32_t y, HitFace* face = 0, bool item_highlight=false );

	void IncSortLimit() { sort_limit++; }
	void DecSortLimit() { if (sort_limit > 0) sort_limit--; }

private:
	bool PaintSortItem(SortItem	*);
	bool NullPaintSortItem(SortItem	*);
};


#endif //ITEMSORTER_H
