/*
 *  Copyright (C) 2003-2004  The Pentagram Team
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

#ifndef GAMEMAPGUMP_H_INCLUDED
#define GAMEMAPGUMP_H_INCLUDED

#include "Gump.h"
#include <vector>

class ItemSorter;
class CameraProcess;

class GameMapGump : public Gump
{
protected:
	ItemSorter		*display_list;

public:
	ENABLE_RUNTIME_CLASSTYPE();

	GameMapGump();
	GameMapGump(int x, int y, int w, int h);
	virtual ~GameMapGump();

	virtual void		PaintThis(RenderSurface *surf, int32_t lerp_factor, bool scaled);

	void				GetCameraLocation(int32_t& x, int32_t& y, int32_t& z,
										  int lerp_factor=256);

	// Trace a click, and return ObjId (parent coord space)
	virtual uint16_t		TraceObjId(int mx, int my);

	// Trace a click, return ObjId, and the coordinates of the mouse click (gump coord space)
	virtual uint16_t		TraceCoordinates(int mx, int my, int32_t coords[3],
										 int offsetx = 0, int offsety = 0,
										 Item* item = 0);

	// Get the location of an item in the gump (coords relative to this).
	// Returns false on failure
	virtual bool		GetLocationOfItem(uint16_t itemid, int &gx, int &gy,
										  int32_t lerp_factor = 256);

	virtual bool		StartDraggingItem(Item* item, int mx, int my);
	virtual bool		DraggingItem(Item* item, int mx, int my);
	virtual void		DraggingItemLeftGump(Item* item);
	virtual void		StopDraggingItem(Item* item, bool moved);
	virtual void		DropItem(Item* item, int mx, int my);

	virtual Gump *		OnMouseDown(int button, int mx, int my);
	virtual void		OnMouseUp(int button, int mx, int my);
	virtual void		OnMouseClick(int button, int mx, int my);
	virtual void		OnMouseDouble(int button, int mx, int my);

	void IncSortOrder(int count);

	bool loadData(IDataSource* ids, uint32_t version);

	static void			SetHighlightItems(bool highlight) { highlightItems = highlight; }
	static bool			isHighlightItems() { return highlightItems; }

	static void ConCmd_toggleHighlightItems(const Console::ArgvType &argv);
	static void ConCmd_dumpMap(const Console::ArgvType &argv);

	static void ConCmd_incrementSortOrder(const Console::ArgvType &argv);
	static void ConCmd_decrementSortOrder(const Console::ArgvType &argv);

	virtual void		RenderSurfaceChanged();

protected:
	virtual void saveData(ODataSource* ods);

	bool display_dragging;
	uint32_t dragging_shape;
	uint32_t dragging_frame;
	uint32_t dragging_flags;
	int32_t dragging_pos[3];

	static bool highlightItems;

};

#endif //GameMapGUMP_H_INCLUDED

