/*
 *  Copyright (C) 2004-2005  The Pentagram Team
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

#ifndef SHAPEVIEWERGUMP_H
#define SHAPEVIEWERGUMP_H

#include "ModalGump.h"

#include <vector>

class ShapeArchive;


class ShapeViewerGump : public ModalGump
{
public:
	ENABLE_RUNTIME_CLASSTYPE();

	ShapeViewerGump();
	ShapeViewerGump(int width, int height,
					std::vector<std::pair<std::string,ShapeArchive*> >& flexes,
					uint32_t _Flags = 0, int32_t layer = LAYER_MODAL);
	virtual ~ShapeViewerGump(void);

	virtual void PaintThis(RenderSurface*, int32_t lerp_factor, bool scaled);

	virtual bool OnKeyDown(int key, int mod);
	virtual bool OnTextInput(int unicode);

	static void U8ShapeViewer();
	static void ConCmd_U8ShapeViewer(const Console::ArgvType &argv);

	bool loadData(IDataSource* ids);
protected:
	virtual void saveData(ODataSource* ods);

	std::vector<std::pair<std::string,ShapeArchive*> > flexes;	
	unsigned int curflex;
	ShapeArchive* flex;
	uint32_t curshape;
	uint32_t curframe;

	uint32_t background;

	int32_t shapew,shapeh,shapex,shapey;
};

#endif
