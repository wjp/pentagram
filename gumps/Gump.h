/*
 *  Copyright (C) 2003-2007  The Pentagram Team
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

#ifndef GUMP_H_INCLUDED
#define GUMP_H_INCLUDED

#include "Object.h"
#include "Rect.h"
#include "FrameID.h"
#include <list>

class RenderSurface;
class Shape;
class Item;
class GumpNotifyProcess;

//
// Class Gump
//
// Desc: Base Gump Class that all other Gumps inherit from
//

class Gump : public Object
{
protected:

	friend class GumpList;

	uint16_t				owner;			// Owner item
	Gump *				parent;			// Parent gump
	int32_t				x, y;			// Gump's position in parent.
										// Always the upper left corner!

	Pentagram::Rect		dims;			// The dimensions/coord space of the gump
	uint32_t				flags;			// Gump flags
	int32_t				layer;			// gump ordering layer

	int32_t				index;			// 'Index'

	Shape				*shape;			// The gumps shape (always painted at 0,0)
	uint32_t				framenum;

	//! The Gump list for this gump. This will contain all child gumps,
	//! as well as all gump widgets. 
	std::list<Gump*>	children;		// List of all gumps
	Gump *				focus_child;	// The child that has focus

	uint16_t				notifier;		// Process to notify when we're closing
	uint32_t				process_result;	// Result for the notifier process

public:
	ENABLE_RUNTIME_CLASSTYPE();
	Gump();
	Gump(int x, int y, int width, int height, uint16_t owner = 0,
		 uint32_t _Flags = 0, int32_t layer = LAYER_NORMAL);
	virtual ~Gump();

public:

	virtual void				CreateNotifier();
	void						SetNotifyProcess(GumpNotifyProcess* proc);
	GumpNotifyProcess*			GetNotifyProcess();
	inline uint32_t				GetResult() { return process_result; }
	void						SetResult(uint32_t res) { process_result = res; }

	//! Set the Gump's shape/frame
	inline void					SetShape(Shape *_shape, uint32_t _framenum)
		{ shape = _shape; framenum = _framenum; }

	void						SetShape(FrameID frame, bool adjustsize=false);

	//! Set the Gump's frame
	inline void					SetFramenum(uint32_t _framenum)
		{ framenum = _framenum; }

	//! Init the gump and add it to parent; call after construction
	//! When newparent is 0, this will call GUIApp::addGump().
	//! \param newparent The Gump's new parent or 0.
	//! \param takefocus If true, set parent's focus_child to this
	virtual void				InitGump(Gump* newparent,bool take_focus=true);

	//! Find a gump of the specified type (this or child)
	//! \param t Type of gump to look for
	//! \param recursive Recursively search through children?
	//! \param no_inheritance Exactly this type, or is a subclass also allowed?
	//! \return the desired Gump, or NULL if not found
	virtual Gump *				FindGump(const RunTimeClassType& t,
										 bool recursive=true,
										 bool no_inheritance=false);

	//! Find a gump of the specified type (this or child)
	//! \param T Type of gump to look for
	//! \param recursive Recursively search through children?
	//! \param no_inheritance Exactly this type, or is a subclass also allowed?
	//! \return the desired Gump, or NULL if not found
	template<class T> Gump *	FindGump(bool recursive=true,
										 bool no_inheritance=false)
		{ return FindGump(T::ClassType, recursive, no_inheritance); }

	//! Find gump (this, child or NULL) at parent coordinates (mx,my)
	//! \return the Gump at these coordinates, or NULL if none
	virtual Gump *		FindGump(int mx, int my);

	//! Get the mouse cursor for position mx, my relative to parents position.
	//! If this gump doesn't want to set the cursor, the gump list will
	//! attempt to get the cursor shape from the next lower gump.
	//! \return true if this gump wants to set the cursor, false otherwise
	virtual bool		GetMouseCursor(int mx, int my, Shape &shape,
									   int32_t &frame);

	// Notify gumps the render surface changed.
	virtual void		RenderSurfaceChanged();

	//! Run the gump
	virtual void		run();

	//! Close item-dependent gumps (recursively).
	//! Called when there is a map change (so the gumps can self terminate
	//! among other things), or when backspace is pressed by the user.
	virtual void		CloseItemDependents(void);

	//! Paint the Gump (RenderSurface is relative to parent).
	//! Calls PaintThis and PaintChildren
	// \param surf The RenderSurface to paint to
	// \param lerp_factor The lerp_factor to paint at (0-256)
	// \param scaled Set if the gump is being drawn scaled. 
	virtual void		Paint(RenderSurface* surf, int32_t lerp_factor, bool scaled);

	//! Paint the unscaled compontents of the Gump with compositing (RenderSurface is relative to parent).
	//! Calls PaintComposited on self and PaintCompositing on children
	// \param surf The RenderSurface to paint to
	// \param lerp_factor The lerp_factor to paint at (0-256)
	// \param scalex Fixed point scaling factor for x coord
	// \param scaley Fixed point scaling factor for y coord
	virtual void		PaintCompositing(RenderSurface* surf, int32_t lerp_factor, int32_t scalex, int32_t scaley);

protected:

	//! Overloadable method to Paint just this Gump (RenderSurface is relative to this)
	// \param surf The RenderSurface to paint to
	// \param lerp_factor The lerp_factor to paint at (0-256)
	// \param scaled Set if the gump is being drawn scaled. 
	virtual void		PaintThis(RenderSurface* surf, int32_t lerp_factor, bool scaled);

	//! Paint the Gumps Children (RenderSurface is relative to this)
	// \param surf The RenderSurface to paint to
	// \param lerp_factor The lerp_factor to paint at (0-256)
	// \param scaled Set if the gump is being drawn scaled. 
	virtual void		PaintChildren(RenderSurface* surf, int32_t lerp_factor, bool scaled);

	//! Overloadable method to Paint just this gumps unscaled components that require compositing (RenderSurface is relative to parent).
	// \param surf The RenderSurface to paint to
	// \param lerp_factor The lerp_factor to paint at (0-256)
	// \param scalex Fixed point scaling factor for x coord
	// \param scaley Fixed point scaling factor for y coord
	virtual void		PaintComposited(RenderSurface* surf, int32_t lerp_factor, int32_t scalex, int32_t scaley);

	static inline int32_t ScaleCoord(int32_t c, int32_t factor) { return ((c*factor)+(1<<15))>>16; }
	static inline int32_t UnscaleCoord(int32_t c, int32_t factor) { return (c<<16)/factor; }

public:

	//! Close the gump
	//! \param no_del If true, do not delete after closing
	virtual void		Close(bool no_del = false);

	//! Check to see if a Gump is Closing
	bool				IsClosing() { return (flags&FLAG_CLOSING)!=0; }

	//! Move this gump
	virtual void		Move(int x_, int y_) { x = x_; y = y_; }

	//! Move this gump relative to its current position
	virtual void		MoveRelative(int x_, int y_) { x += x_; y += y_; }

	enum Position {
		CENTER = 1,
		TOP_LEFT = 2,
		TOP_RIGHT = 3,
		BOTTOM_LEFT = 4,
		BOTTOM_RIGHT = 5,
		TOP_CENTER = 6,
		BOTTOM_CENTER = 7
	};

	//! Moves this gump to a relative location on the parent gump
	// \param pos the postition on the parent gump
	// \param xoffset an offset from the position on the x-axis
	// \param yoffset an offset from the position on the y-axis
	virtual void		setRelativePosition(Position pos, int xoffset=0, int yoffset=0);

	//
	// Points and Coords
	//

	//! Get the dims
	virtual void		GetDims(Pentagram::Rect &d) { d = dims; }

	//! Set the dims
	virtual void		SetDims(const Pentagram::Rect &d) { dims = d; }

	//! Detect if a point is on the gump
	virtual bool		PointOnGump(int mx, int my);

	enum PointRoundDir {
		ROUND_TOPLEFT = 0,
		ROUND_BOTTOMRIGHT = 1
	};
	enum RectRoundDir {
		ROUND_INSIDE,
		ROUND_OUTSIDE
	};

	//! Convert a screen space point to a gump point
	virtual void		ScreenSpaceToGump(int &sx, int &sy,
										  PointRoundDir r = ROUND_TOPLEFT);

	//! Convert a gump point to a screen space point
	virtual void		GumpToScreenSpace(int &gx, int &gy,
										  PointRoundDir r = ROUND_TOPLEFT);

	//! Convert a parent relative point to a gump point
	virtual void		ParentToGump(int &px, int &py,
									 PointRoundDir r = ROUND_TOPLEFT);

	//! Convert a gump point to parent relative point
	virtual void		GumpToParent(int &gx, int &gy,
									 PointRoundDir r = ROUND_TOPLEFT);

	//! Transform a rectangle to screenspace from gumpspace
	virtual void		GumpRectToScreenSpace(int &gx,int &gy, int &gw,int &gh,
											  RectRoundDir r = ROUND_OUTSIDE);

	//! Transform a rectangle to gumpspace from screenspace
	virtual void		ScreenSpaceToGumpRect(int &sx,int &sy, int &sw,int &sh,
											 RectRoundDir r = ROUND_OUTSIDE);

	//! Trace a click, and return ObjId
	virtual uint16_t		TraceObjId(int mx, int my);

	//! Get the location of an item in the gump (coords relative to this).
	//! \return false on failure
	virtual bool		GetLocationOfItem(uint16_t itemid, int &gx, int &gy,
										  int32_t lerp_factor = 256);


	//
	// Some event handlers. In theory they 'should' be able to be mapped to
	// Usecode classes.
	//
	// mx and my are relative to parents position
	//
	// OnMouseDown returns the Gump that handled the Input, if it was handled.
	// The MouseUp,MouseDouble events will be sent to the same gump.
	//
	// OnMouseMotion works like OnMouseDown,
	// but independently of the other methods.
	//
	// Unhandled input will be passed down to the next lower gump.
	//
	// A mouse click on a gump will make it focus, IF it wants it.
	//
	
	// Return Gump that handled event
	virtual Gump *		OnMouseDown(int button, int mx, int my);
	virtual void		OnMouseUp(int  button, int mx, int my) { }
	virtual void		OnMouseClick(int button, int mx, int my) { }
	virtual void		OnMouseDouble(int button, int mx, int my) { }
	virtual Gump *		OnMouseMotion(int mx, int my);

	// OnMouseOver is only call when the mouse first passes over the gump
	// OnMouseLeft is call as the mouse leaves the gump.
	virtual void		OnMouseOver() { };
	virtual void		OnMouseLeft() { };

	// Keyboard input gets sent to the FocusGump. Or if there isn't one, it
	// will instead get sent to the default key handler. TextInput requires
	// that text mode be enabled. Return true if handled, false if not.
	// Default, returns false, unless handled by focus child
	virtual bool		OnKeyDown(int key, int mod);
	virtual bool		OnKeyUp(int key);
	virtual bool		OnTextInput(int unicode);

	// This is for detecting focus changes for keyboard input. Gets called true
	// when the this gump is being set as the focus focus gump. It is called
	// false when focus is being taken away.
	virtual void		OnFocus(bool /*gain*/) { }
	
	// Makes this gump the focus
	virtual void		MakeFocus();

	// Is this gump the focus?
	inline bool			IsFocus()
		{ return parent?parent->focus_child==this:false; }

	// Get the child in focus
	inline Gump *		GetFocusChild() { return focus_child; }

	// Find a new Child to be the focus
	void				FindNewFocusChild();


	//
	// Child gump related
	//

	//! Add a gump to the child list. 
	virtual void		AddChild(Gump *, bool take_focus = true);

	//! Remove a gump from the child list
	virtual void		RemoveChild(Gump *);

	//! Move child to front (within its layer)
	virtual void		MoveChildToFront(Gump *);

	//! Get the parent
	inline Gump *		GetParent() { return parent; }

	//! Get the root gump (or self)
	Gump *				GetRootGump();

	//! This function is used by our children to notifty us of 'something'
	//! Think of it as a generic call back function
	virtual void		ChildNotify(Gump *child, uint32_t message) { }
	void				SetIndex(int32_t i) { index = i; }
	int32_t				GetIndex() { return index; }

	// Dragging
	//! Called when a child gump starts to be dragged.
	//! \return false if the child isn't allowed to be dragged.
	virtual bool		StartDraggingChild(Gump* gump, int mx, int my);
	virtual void		DraggingChild(Gump* gump, int mx, int my);
	virtual void		StopDraggingChild(Gump* gump);

	//! This will be called when an item in this gump starts to be dragged.
	//! \return false if the item isn't allowed to be dragged.
	virtual bool		StartDraggingItem(Item* item, int mx, int my)
		{ return false; }

	//! Called when an item is being dragged over the gump.
	//! Note: this may be called on a different gump than StartDraggingItem.
	//! \return false if the item can't be dragged to this location.
	virtual bool		DraggingItem(Item* item, int mx, int my)
		{ return false; }

	//! Called when an item that was being dragged over the gump left the gump
	virtual void		DraggingItemLeftGump(Item* item) { }

	//! Called when a drag operation finished.
	//! This is called on the same gump that received StartDraggingItem
	//! \param moved If true, the item was actually dragged somewhere else.
	//!              If false, the drag was cancelled.
	virtual void		StopDraggingItem(Item* item, bool moved) { }

	//! Called when an item has been dropped on a gump.
	//! This is called after StopDraggingItem has been called, but possibly
	//! on a different gump.
	//! It's guaranteed that a gump will only receive a DropItem at a location
	//! if a DraggingItem there returned true.
	virtual void		DropItem(Item* item, int mx, int my) { }

public:

	//
	// Gump Flags
	//
	enum GumpFlags {
		FLAG_DRAGGABLE		= 0x01,		// When set, the gump can be dragged
		FLAG_HIDDEN			= 0x02,		// When set, the gump will not be drawn
		FLAG_CLOSING		= 0x04,		// When set, the gump is closing
		FLAG_CLOSE_AND_DEL	= 0x08,		// When set, the gump is closing and will be deleted
		FLAG_ITEM_DEPENDENT	= 0x10,		// When set, the gump will be deleted on MapChange
		FLAG_DONT_SAVE      = 0x20,		// When set, don't save this gump.
		                          		// Be very careful with this one!
		FLAG_CORE_GUMP      = 0x40,		// core gump (only children are saved)
		FLAG_KEEP_VISIBLE   = 0x80		// Keep this gump on-screen.
										// (only for ItemRelativeGumps)
	};

	inline bool			IsHidden()
		{ return (flags&FLAG_HIDDEN) || (parent && parent->IsHidden()); }
	bool				IsDraggable() { return flags&FLAG_DRAGGABLE; }
	virtual void		HideGump() { flags |= FLAG_HIDDEN; }
	virtual void		UnhideGump() { flags &= ~FLAG_HIDDEN; }

	bool mustSave(bool toplevel);

	//
	// Gump Layers
	//
	enum GumpLayers {
		LAYER_DESKTOP		= -16,		// Layer for Desktop 'bottom most'
		LAYER_GAMEMAP		= -8,		// Layer for the World Gump
		LAYER_NORMAL		= 0,		// Layer for Normal gumps
		LAYER_ABOVE_NORMAL	= 8,		// Layer for Always on top Gumps
		LAYER_MODAL         = 12,		// Layer for Modal Gumps
		LAYER_CONSOLE		= 16		// Layer for the console
	};

	bool loadData(IDataSource* ids, uint32_t version);
protected:
	virtual void saveData(ODataSource* ods);
};

#endif //GUMP_H_INCLUDED
