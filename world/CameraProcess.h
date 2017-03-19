/*
Copyright (C) 2003-2004 The Pentagram team

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

#ifndef CAMERAPROCESS_H
#define CAMERAPROCESS_H

#include "Process.h"
#include "intrinsics.h"

//
// The camera process. This works in 3 ways
//
// It can be set to stay where it currently is
// It can be set to follow an item.
// It can be set to scroll to an item
// It can be set to stay at a location
//

class CameraProcess : public Process
{
public:
	CameraProcess();
	CameraProcess(uint16_t itemnum);							// Follow item/Do nothing
	CameraProcess(int32_t x, int32_t y, int32_t z);				// Goto location
	CameraProcess(int32_t x, int32_t y, int32_t z, int32_t time);	// Scroll to location

	virtual ~CameraProcess();

	// p_dynamic_cast stuff
	ENABLE_RUNTIME_CLASSTYPE();

	virtual void run();

	// You will notice that this isn't the same as how Item::GetLerped works
	void GetLerped(int32_t &x, int32_t &y, int32_t &z, int32_t factor, bool noupdate=false);

	//! Find the roof above the camera. 
	//! \param factor Interpolation factor for this frame
	//! \return 0 if no roof found, objid of roof if found
	uint16_t FindRoof(int32_t factor);

	INTRINSIC(I_setCenterOn);
	INTRINSIC(I_move_to);
	INTRINSIC(I_scrollTo);
	INTRINSIC(I_startQuake);
	INTRINSIC(I_stopQuake);

	static void				GetCameraLocation(int32_t &x, int32_t &y, int32_t &z);
	static CameraProcess*	GetCameraProcess() { return camera; }
	static uint16_t			SetCameraProcess(CameraProcess *);	// Set the current camera process. Adds process. Return PID
	static void				ResetCameraProcess();

	static void				SetEarthquake(int32_t e) { 
		earthquake = e; 
		if (!e)  eq_x = eq_y = 0;
	}

	void					ItemMoved();

	virtual void terminate();	// Terminate NOW!

	bool loadData(IDataSource* ids, uint32_t version);
private:
	virtual void saveData(ODataSource* ods);

	int32_t sx, sy, sz;
	int32_t ex, ey, ez;
	int32_t time;
	int32_t elapsed;
	uint16_t itemnum;

	int32_t last_framenum;

	static CameraProcess	*camera;
	static int32_t earthquake;
	static int32_t eq_x, eq_y;
};

#endif //CAMERAPROCESS_H
