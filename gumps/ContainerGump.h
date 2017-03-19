/*
 *  Copyright (C) 2003-2005  The Pentagram Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef CONTAINERGUMP_H
#define CONTAINERGUMP_H

#include "ItemRelativeGump.h"

class Shape;
class Container;

class ContainerGump : public ItemRelativeGump
{
public:
	ENABLE_RUNTIME_CLASSTYPE();

	ContainerGump();
	ContainerGump(Shape* shape, uint32_t framenum, uint16_t owner,
				  uint32_t _Flags = FLAG_DRAGGABLE, int32_t layer = LAYER_NORMAL);
	virtual ~ContainerGump(void);

	void setItemArea(Pentagram::Rect* itemarea_) { itemarea = *itemarea_; }

	// Close the gump
	virtual void Close(bool no_del = false);

	// Init the gump, call after construction
	virtual void InitGump(Gump* newparent, bool take_focus=true);

	// Paint the Gump
	virtual void PaintThis(RenderSurface*, int32_t lerp_factor, bool scaled);

	// Trace a click, and return ObjId
	virtual uint16_t TraceObjId(int mx, int my);

	// Get the location of an item in the gump (coords relative to this).
	// Returns false on failure.
	virtual bool GetLocationOfItem(uint16_t itemid, int &gx, int &gy,
								   int32_t lerp_factor = 256);


	virtual bool StartDraggingItem(Item* item, int mx, int my);
	virtual bool DraggingItem(Item* item, int mx, int my);
	virtual void DraggingItemLeftGump(Item* item);
	virtual void StopDraggingItem(Item* item, bool moved);
	virtual void DropItem(Item* item, int mx, int my);

	virtual Gump* OnMouseDown(int button, int mx, int my);
	virtual void OnMouseClick(int button, int mx, int my);
	virtual void OnMouseDouble(int button, int mx, int my);

	bool loadData(IDataSource* ids, uint32_t version);
protected:
	virtual void saveData(ODataSource* ods);

	virtual void GetItemLocation(int32_t lerp_factor);

	virtual Container* getTargetContainer(Item* item, int mx, int my);

	void getItemCoords(Item* item, int32_t& itemx, int32_t& itemy);

	Pentagram::Rect itemarea;

	bool display_dragging;
	uint32_t dragging_shape;
	uint32_t dragging_frame;
	uint32_t dragging_flags;
	int32_t dragging_x, dragging_y;
};

#endif
