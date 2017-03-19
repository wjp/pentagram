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

#ifndef MODALGUMP_H
#define MODALGUMP_H

#include "Gump.h"

class ModalGump : public Gump
{
public:
	ENABLE_RUNTIME_CLASSTYPE();

	ModalGump();
	ModalGump(int x, int y, int width, int height, uint16_t owner = 0,
			  uint32_t _Flags = 0, int32_t layer = LAYER_MODAL);

	~ModalGump();

	virtual void InitGump(Gump* newparent, bool take_focus=true);

	virtual void Close(bool no_del = false);

	virtual bool PointOnGump(int mx, int my);
	virtual Gump* FindGump(int mx, int my);
	virtual uint16_t TraceObjId(int mx, int my);

	virtual Gump* OnMouseDown(int button, int mx, int my);

	bool loadData(IDataSource* ids, uint32_t version);
protected:
	virtual void saveData(ODataSource* ods);
};

#endif
