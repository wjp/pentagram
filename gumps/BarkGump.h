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

#ifndef BARKGUMP_H_INCLUDED
#define BARKGUMP_H_INCLUDED

#include "ItemRelativeGump.h"

class BarkGump : public ItemRelativeGump
{
protected:
	std::string	barked;
	int32_t counter;
	ObjId textwidget;
	uint32_t speechshapenum;
	uint32_t speechlength;
	uint32_t totaltextheight;

public:
	ENABLE_RUNTIME_CLASSTYPE();

	BarkGump();
	BarkGump(uint16_t owner, std::string msg, uint32_t speechshapenum=0);
	virtual ~BarkGump(void);

	// Run the gump (decrement the counter)
	virtual void		run();

	// Got to the next page on mouse click
	virtual Gump *		OnMouseDown(int button, int mx, int my);

	// Init the gump, call after construction
	virtual void		InitGump(Gump* newparent, bool take_focus=true);

protected:
	//! show next text.
	//! returns false if no more text available
	bool NextText();

	int textdelay;

public:
	bool loadData(IDataSource* ids, uint32_t version);
protected:
	virtual void saveData(ODataSource* ods);
};

#endif //BARKGUMP_H_INCLUDED
