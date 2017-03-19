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

#ifndef BUTTONWIDGET_H_INCLUDED
#define BUTTONWIDGET_H_INCLUDED

#include "Gump.h"
#include "FrameID.h"

class ButtonWidget : public Gump
{
public:
	// p_dynamic_class stuff
	ENABLE_RUNTIME_CLASSTYPE();

	ButtonWidget();
	ButtonWidget(int X, int Y, std::string txt, bool gamefont, int font,
				 uint32_t mouseOverBlendCol = 0, int width = 0, int height = 0,
				 int32_t layer = LAYER_NORMAL);
	ButtonWidget(int X, int Y, FrameID frame_up, FrameID frame_down,
				 bool mouseOver = false, int32_t layer = LAYER_NORMAL);
	virtual ~ButtonWidget(void);

	virtual void InitGump(Gump* newparent, bool take_focus=true);
	virtual uint16_t TraceObjId(int mx, int my);
	virtual bool PointOnGump(int mx, int my);

	virtual Gump* OnMouseDown(int button, int mx, int my);
	virtual void OnMouseUp(int  button, int mx, int my);
	virtual void OnMouseClick(int button, int mx, int my);
	virtual void OnMouseDouble(int button, int mx, int my);

	virtual void OnMouseOver();
	virtual void OnMouseLeft();

	//! return the textwidget's vlead, or 0 for an image button
	int getVlead();

	//void SetShapeDown(Shape *_shape, uint32_t _framenum);
	//void SetShapeUp(Shape *_shape, uint32_t _framenum);

	enum Message
	{
		BUTTON_CLICK		= 0,
		BUTTON_UP			= 1,
		BUTTON_DOUBLE		= 2
	};

protected:
	Shape *shape_up;
	uint32_t framenum_up;
	Shape *shape_down;
	uint32_t framenum_down;
	uint16_t textwidget;
	uint32_t mouseOverBlendCol;
	bool mouseOver;
	int origw, origh;

public:
	bool loadData(IDataSource* ids, uint32_t version);
protected:
	virtual void saveData(ODataSource* ods);
};

#endif // BUTTONWIDGET_H_INCLUDED
