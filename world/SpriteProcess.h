/*
Copyright (C) 2003 The Pentagram team

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

#ifndef SPRITEPROCESS_H_INCLUDED
#define SPRITEPROCESS_H_INCLUDED

#include "Process.h"
#include "intrinsics.h"

//! Creates a Sprite. Animates it. Destroys it.
class SpriteProcess : public Process
{
	int		frame;
	int		first_frame;
	int		last_frame;
	int		repeats;
	int		delay;
	int		delay_counter;
public:
	// p_dynamic_class stuff
	ENABLE_RUNTIME_CLASSTYPE();

	//! SpriteProcess Constructor
	//! \param shape The shape to use
	//! \param frame The initial/first frame of the sprite animation
	//! \param last_frame The last frame of the sprite animation
	//! \param repeats The number of times to play the sprite animation
	//! \param delay The number of runs to wait before incrementing the frame
	//! \param x X coord of the sprite in the world
	//! \param y Y coord of the sprite in the world
	//! \param z Z coord of the sprite in the world
	SpriteProcess(int shape, int frame, int last_frame, 
					int repeats, int delay, int x, int y, int z);

	//! The SpriteProcess destructor
	virtual ~SpriteProcess(void);

	//! The SpriteProcess run function
	//! \param framenum The number of this frame of execution
	//! \return true if a screen repaint is required
	virtual bool run(const uint32 framenum);

	INTRINSIC(I_createSprite);
//	INTRINSIC(I_createSpriteEx);
};

#endif //SPRITEPROCESS_H_INCLUDED