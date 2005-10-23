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

#include "pent_include.h"
#include "PentagramMenuGump.h"

#include "RenderSurface.h"
#include "GUIApp.h"
#include "GameWidget.h"
#include "SettingManager.h"

DEFINE_RUNTIME_CLASSTYPE_CODE(PentagramMenuGump,ModalGump);

PentagramMenuGump::PentagramMenuGump(int X, int Y, int Width, int Height) :
	ModalGump(X,Y,Width,Height)
{
	gameScrollPos = 0;
	gameScrollTarget = 0;
	gameScrollLastDelta = 0;
}

PentagramMenuGump::~PentagramMenuGump()
{

}

void PentagramMenuGump::InitGump(Gump* newparent, bool take_focus)
{
	ModalGump::InitGump(newparent, take_focus);

	Pentagram::istring game;

	GameWidget* g;
	int y = 50;

	std::vector<Pentagram::istring> games;
	// TODO: listGames() should probably be in CoreApp
	games = SettingManager::get_instance()->listGames();
	unsigned int index = 0;
	for (unsigned int i = 0; i < games.size(); ++i) {
		Pentagram::istring game = games[i];

		if (game == "pentagram") continue;

		g = new GameWidget(150, y, game);
		g->InitGump(this, false);
		g->SetIndex(index++);
		y += 114;
	}

	gamecount = index;
}

void PentagramMenuGump::PaintThis(RenderSurface *surf, sint32 lerp_factor)
{
#if 1
	// CHECKME: fast enough?
	for (unsigned int i = 0; i < 400; i+=4) {
		unsigned int r = (140 * i)/400;
		unsigned int gb = (21 * i)/400;
		uint32 col = 0xFF000000 + (r << 16) + (gb << 8) + gb;
		surf->Fill32(col, 0, i, 640, 4);
	}
#else
	surf->Fill32(0xFF440A0A, 0, 0, 640, 400);
#endif

	surf->Fill32(0xFFDCB95C, 18, 0, 90, 400);

	surf->Fill32(0xFFC11515, 200, 6, 340, 36);
}

void PentagramMenuGump::PaintChildren(RenderSurface *surf, sint32 lerp_factor)
{
	// Iterate all children
	std::list<Gump*>::iterator it = children.begin();
	std::list<Gump*>::iterator end = children.end();

	Pentagram::Rect game_clip_rect(0, 45, 640, 342);
	Pentagram::Rect cur_clip_rect;
	surf->GetClippingRect(cur_clip_rect);


	while (it != end)
	{
		Gump *g = *it;

		if (g->IsOfType<GameWidget>()) {
			surf->SetClippingRect(game_clip_rect);
			g->Move(150, 50 + 114*g->GetIndex() + gameScrollPos);
		}

		g->Paint(surf, lerp_factor);

		surf->SetClippingRect(cur_clip_rect);

		++it;
	}	
}


void PentagramMenuGump::ChildNotify(Gump *child, uint32 message)
{
	if (child->IsOfType<GameWidget>()) {

		GameWidget* gw = p_dynamic_cast<GameWidget*>(child);
		Pentagram::istring gamename = gw->getGameName();

		switch (message) {
		case GameWidget::GAME_PLAY:
			GUIApp::get_instance()->changeGame(gamename);
			break;
		case GameWidget::GAME_LOAD:
			break;
		case GameWidget::GAME_SETTINGS:
			break;
		case GameWidget::GAME_REMOVE:
			break;
		}
	}
}

bool PentagramMenuGump::Run(const uint32 framenum)
{
	int oldpos = gameScrollPos;

	if (gameScrollPos != gameScrollTarget) {
		int diff = gameScrollTarget - gameScrollPos;
		if (diff < 20 && diff > -20) {
			gameScrollPos = gameScrollTarget;
		} else {
			gameScrollPos += diff/3;
		}
	}

	gameScrollLastDelta = gameScrollPos - oldpos;

	return true;
}


bool PentagramMenuGump::OnKeyDown(int key, int mod)
{
	int delta = 0;

	if (key == SDLK_DOWN) {
		delta = -114;
	} else if (key == SDLK_UP) {
		delta = 114;
	}

	if (delta && gamecount > 3) {
		gameScrollTarget += delta;

		if (gameScrollTarget > 0)
			gameScrollTarget = 0;
		if (gameScrollTarget < -114*(gamecount-3))
			gameScrollTarget = -114*(gamecount-3);

		return true;
	}

	return false;
}