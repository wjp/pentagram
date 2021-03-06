/*
 *  Copyright (C) 2004-2005 The Pentagram Team
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

#include "pent_include.h"

#include "WpnOvlayDat.h"

#include "IDataSource.h"
#include "WeaponOverlay.h"
#include "RawArchive.h"
#include "GameData.h"
#include "MainShapeArchive.h"
#include "AnimAction.h"

WpnOvlayDat::WpnOvlayDat()
{

}


WpnOvlayDat::~WpnOvlayDat()
{
	for (unsigned int i = 0; i < overlay.size(); i++)
		delete overlay[i];
	overlay.clear();
}

const AnimWeaponOverlay* WpnOvlayDat::getAnimOverlay(uint32 action) const
{
	if (action >= overlay.size()) return 0;
	return overlay[action];
}

const WeaponOverlayFrame* WpnOvlayDat::getOverlayFrame(uint32 action, int type,
													   int direction,
													   int frame) const
{
	if (action >= overlay.size()) return 0;
	if (!overlay[action]) return 0;
	return overlay[action]->getFrame(type, direction, frame);
}


void WpnOvlayDat::load(RawArchive *overlaydat)
{
	WeaponOverlayFrame f;

	MainShapeArchive* msf = GameData::get_instance()->getMainShapes();
	assert(msf);

	overlay.resize(overlaydat->getCount());

	for (unsigned int action = 0; action < overlay.size(); action++)
	{
		IDataSource* ds = overlaydat->get_datasource(action);
		overlay[action] = 0;

		if (ds && ds->getSize()) {
			// get Avatar's animation
			AnimAction* anim = msf->getAnim(1, action);
			if (!anim) {
				perr << "Skipping wpnovlay action " << action << " because animation doesn't exist." << std::endl;
				continue;
			}

			AnimWeaponOverlay* awo = new AnimWeaponOverlay;
			overlay[action] = awo;

			unsigned int animlength = anim->size;
			unsigned int dircount = anim->dircount;

			unsigned int typecount = ds->getSize() / (4*dircount*animlength);
			awo->overlay.resize(typecount);

			for (unsigned int type = 0; type < typecount; type++) {
				awo->overlay[type].dircount = dircount;
				awo->overlay[type].frames =
					new std::vector<WeaponOverlayFrame>[dircount];
				for (unsigned int dir = 0; dir < dircount; dir++) {
					awo->overlay[type].frames[dir].resize(animlength);
					for (unsigned int frame = 0; frame < animlength; frame++) {
						unsigned int offset = type*8*animlength
							+ dir*animlength + frame;
						ds->seek(4 * offset);
						f.xoff = ds->readXS(1);
						f.yoff = ds->readXS(1);
						f.frame = ds->read2();

						awo->overlay[type].frames[dir][frame] = f;	
					}
				}
			}
		}


		delete ds;
	}

}
