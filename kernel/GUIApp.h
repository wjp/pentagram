/*
Copyright (C) 2002-2004 The Pentagram team

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

#ifndef GUIAPP_H
#define GUIAPP_H

#include <vector>
#include <stack>
#include <list>

#include "SDL_events.h"
#include "intrinsics.h"
#include "Args.h"

#include "CoreApp.h"
#include "Mouse.h"

class Game;
class Gump;
class ConsoleGump;
class GameMapGump;
class ScalerGump;
class InverterGump;
class RenderSurface;
class PaletteManager;
class GameData;
class World;
class ObjectManager;
class FontManager;
class HIDManager;
class MidiDriver;
class AvatarMoverProcess;
class IDataSource;
class ODataSource;

// Hack alert
struct HWMouseCursor;
#if defined(WIN32) && defined(I_AM_COLOURLESS_EXPERIMENTING_WITH_HW_CURSORS)
#include <windows.h>
#endif

class GUIApp : public CoreApp
{
public:
	ENABLE_RUNTIME_CLASSTYPE();
	
	GUIApp(int argc, const char* const* argv);
	virtual ~GUIApp();
	
	static GUIApp* get_instance()
		{ return p_dynamic_cast<GUIApp*>(application); }
	
	void startup();

	virtual void run();
	virtual void handleEvent(const SDL_Event& event);
	
	virtual void paint();
	virtual bool isPainting() { return painting; }
	
	
	INTRINSIC(I_getCurrentTimerTick);
	INTRINSIC(I_setAvatarInStasis);
	INTRINSIC(I_getAvatarInStasis);
	INTRINSIC(I_getTimeInGameHours);
	INTRINSIC(I_getTimeInMinutes);
	INTRINSIC(I_getTimeInSeconds);
	INTRINSIC(I_setTimeInGameHours);
	INTRINSIC(I_makeAvatarACheater);

	void setAvatarInStasis(bool stat) { avatarInStasis = stat; }
	bool isAvatarInStasis() const { return avatarInStasis; }
	void toggleAvatarInStasis() { avatarInStasis = !avatarInStasis; }
	bool isPaintEditorItems() const { return paintEditorItems; }
	void togglePaintEditorItems() { paintEditorItems = !paintEditorItems; }
	bool isShowTouchingItems() const { return showTouching; }
	void toggleShowTouchingItems() { showTouching = !showTouching; }

	uint32 getGameTimeInSeconds();
	
	GameMapGump* getGameMapMapGump() { return gameMapGump; }
	ConsoleGump* getConsoleGump() { return consoleGump; }
	Gump* getDesktopGump() { return desktopGump; }
	Gump* getGump(uint16 gumpid);

	//! add a gump to the right core gump (such as desktopgump)
	//! \param gump the gump to be added
	void addGump(Gump* gump);

	AvatarMoverProcess* getAvatarMoverProcess() { return avatarMoverProcess; }

	//! save a game
	//! \param filename the file to save to
	//! \return true if succesful
	bool saveGame(std::string filename, bool ignore_modals=false);

	//! load a game
	//! \param filename the savegame to load
	//! \return true if succesful.
	bool loadGame(std::string filename);

	//! start a new game
	//! \return true if succesful.
	bool newGame();

	MidiDriver* getMidiDriver() const { return midi_driver; }

	//! get mouse cursor length. 0 = short, 1 = medium, 2 = long
	int getMouseLength(int mx, int my);

	//! get mouse cursor direction. 0 = up, 1 = up-right, 2 = right, etc...
	int getMouseDirection(int mx, int my);

	//! get current mouse cursor location
	void getMouseCoords(int& mx, int& my) { mx = mouseX; my = mouseY; }

	bool isMouseDown(MouseButton button);

	enum MouseCursor {
		MOUSE_NORMAL = 0,
		MOUSE_NONE = 1,
		MOUSE_TARGET = 2,
		MOUSE_PENTAGRAM = 3,
		MOUSE_HAND = 4,
		MOUSE_QUILL = 5,
		MOUSE_MAGGLASS = 6,
		MOUSE_CROSS = 7
	};

	//! set the current mouse cursor
	void setMouseCursor(MouseCursor cursor);

	//! flash the red cross mouse cursor for a brief while
	void flashCrossCursor();

	//! push the current mouse cursor to the stack
	void pushMouseCursor();

	//! pop the last mouse cursor from the stack
	void popMouseCursor();

	//! Enter gump text mode (aka SDL Unicode keyhandling)
	void enterTextMode(Gump *);

	//! Leave gump text mode (aka SDL Unicode keyhandling)
	void leaveTextMode(Gump *);

protected:
	virtual void DeclareArgs();

private:
	uint32 save_count;

	//! write savegame info (time, ..., game-specifics)
	void writeSaveInfo(ODataSource* ods);

	//! save CoreApp/GUIApp data
	void save(ODataSource* ods);

	//! load CoreApp/GUIApp data
	bool load(IDataSource* ids, uint32 version);
	
	//! reset engine (including World, UCMachine, a.o.)
	void resetEngine();

	//! create core gumps (DesktopGump, GameMapGump, ConsoleGump, ...)
	void setupCoreGumps();

	// full system
	Game* game;

	ObjectManager* objectmanager;
	HIDManager* hidmanager;
	UCMachine* ucmachine;
	RenderSurface *screen;
	PaletteManager *palettemanager;
	GameData *gamedata;
	World *world;
	FontManager* fontmanager;
	
	Gump			*desktopGump;
	ConsoleGump		*consoleGump;
	GameMapGump		*gameMapGump;
	ScalerGump		*scalerGump;
	InverterGump	*inverterGump;

	AvatarMoverProcess* avatarMoverProcess;
	
	// called depending upon command line arguments
	void GraphicSysInit(); // starts the graphics subsystem
	void LoadConsoleFont(); // loads the console font
	
	void handleDelayedEvents();
	
	// Various dependancy flags
	bool runGraphicSysInit;
	bool runSDLInit;
	
	// Timing stuff
	sint32 lerpFactor;		//!< Interpolation factor for this frame (0-256)
	bool inBetweenFrame;	//!< Set true if we are doing an inbetween frame
	
	bool frameSkip;			//!< Set to true to enable frame skipping (default false)
	bool frameLimit;		//!< Set to true to enable frame limiting (default true)
	bool interpolate;		//!< Set to true to enable interpolation (default true)
	sint32 animationRate;	//!< The animation rate. Affects all processes! (default 100)
	
	// Sort of Camera Related Stuff, move somewhere else
	
	bool avatarInStasis;    //!< If this is set to true, Avatar can't move, 
	                        //!< nor can Avatar start more usecode
	bool paintEditorItems;  //!< If true, paint items with the SI_EDITOR flag
	
	bool painting;			//!< Set true when painting

	bool showTouching;			//!< If true, highlight items touching Avatar

	int mouseX, mouseY;

	//! get the current mouse frame
	int getMouseFrame();
	std::stack<MouseCursor> cursors;
	int flashingcursor; // time (SDL_GetTicks) mouse started flashing, or -1

	// Hack alert
	HWMouseCursor			*hwcursors;	// Shape frames converted into system specific format.

#if defined(WIN32) && defined(I_AM_COLOURLESS_EXPERIMENTING_WITH_HW_CURSORS)
	void					CreateHWCursors();
	static LRESULT CALLBACK	myWindowProc( HWND, UINT, WPARAM, LPARAM );

#endif

	static void	conAutoPaint(void);

	// mouse input state
	MButton mouseButton[NUM_MOUSEBUTTONS+1];

	uint16 mouseOverGump;

	enum DraggingState {
		DRAG_NOT = 0,
		DRAG_OK = 1,
		DRAG_INVALID = 2,
		DRAG_TEMPFAIL = 3
	} dragging;
	ObjId dragging_objid;
	uint16 dragging_item_startgump;
	uint16 dragging_item_lastgump;

	int dragging_offsetX, dragging_offsetY;
	unsigned int inversion;
public:
	void setDraggingOffset(int x, int y)
		{ dragging_offsetX = x; dragging_offsetY = y; }
	void getDraggingOffset(int& x, int& y)
		{ x = dragging_offsetX; y = dragging_offsetY; }

	unsigned int getInversion() const { return inversion; }
	void setInversion(unsigned int i) { inversion = i & 0xFFFF; }
	bool isInverted() { return ( inversion >= 0x4000 && inversion < 0xC000 ); }

private:
	void startDragging(int mx, int my);
	void moveDragging(int mx, int my);
	void stopDragging(int mx, int my);

	sint32 timeOffset;
	bool has_cheated;

public:
	bool hasCheated() const { return has_cheated; }
	void makeCheater() { has_cheated = true; }

private:

	// Should probably go somewhere else.... perhaps in some 'audio manager'
	MidiDriver *midi_driver;
	int			midi_volume;
	void		init_midi();
	void		deinit_midi();
	static void	sdlAudioCallback(void *userdata, Uint8 *stream, int len);
	
	std::list<ObjId>	textmodes;		//!< Gumps that want text mode

	// Load and save games from arbitrary filenames from the console
	static void			ConCmd_saveGame(const Console::ArgsType &args, const Console::ArgvType &argv);	//!< "GUIApp::saveGame <filename>" console command
	static void			ConCmd_loadGame(const Console::ArgsType &args, const Console::ArgvType &argv);	//!< "GUIApp::loadGame <filename>" console command
	static void			ConCmd_newGame(const Console::ArgsType &args, const Console::ArgvType &argv);	//!< "GUIApp::newGame" console command

	static void			ConCmd_quit(const Console::ArgsType &args, const Console::ArgvType &argv);		//!< "quit" console command

	// This should be a console variable once they are implemented
	bool				drawRenderStats;
	static void			ConCmd_drawRenderStats(const Console::ArgsType &args, const Console::ArgvType &argv);		//!< "GUIApp::drawRenderStats" console command

	bool				ttfoverrides;
};

#endif
