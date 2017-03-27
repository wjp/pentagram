/*
RenderSurface.cpp : RenderSurface Interface source file

Copyright (C) 2002, 2003 The Pentagram Team

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

#include "pent_include.h"
#include "RenderSurface.h"
#include "SoftRenderSurface.h"
#include <SDL.h>
#include <cmath>

#if defined(WIN32) && defined(I_AM_COLOURLESS_EXPERIMENTING_WITH_D3D)
#include "D3D9SoftRenderSurface.h"
#endif

RenderSurface::Format	RenderSurface::format = {
	0,	0,
	0,	0,	0,	0,
	0,	0,	0,	0,
	0,	0,	0,	0,
	0,	0,	0,	0
};

uint8 RenderSurface::Gamma10toGamma22[256];
uint8 RenderSurface::Gamma22toGamma10[256];

//
// RenderSurface::SetVideoMode()
//
// Desc: Create a standard RenderSurface
// Returns: Created RenderSurface or 0
//

RenderSurface *RenderSurface::SetVideoMode(uint32 width,		// Width of desired mode
									uint32 height,		// Height of desired mode
									uint32 bpp,			// Bits Per Pixel of desired mode
									bool fullscreen,	// Fullscreen if true, Windowed if false
									bool use_opengl)	// Use OpenGL if true, Software if false
{
	// TODO: Add in OpenGL
	if (use_opengl)
	{
		pout << "OpenGL Mode not enabled" << std::endl;
		// TODO: Set Error Code
		return 0;
	}

	// check to make sure a 16 bit or 32 bit Mode has been requested
	if (bpp != 16 && bpp != 32)
	{
		pout << "Only 16 bit and 32 bit video modes supported" << std::endl;
		// TODO: Set Error Code
		return 0;
	}

	// SDL Flags to set
	uint32 flags = 0;

	// Specific Windowed code
	if (!fullscreen) 
	{
		// Use the BPP of the desktop
		//bpp = vinfo->vfmt->BitsPerPixel;

		// check to make sure we are in 16 bit or 32 bit
		if (bpp != 16 && bpp != 32)
		{
			pout << bpp << " bit windowed mode unsupported" << std::endl;
			// TODO: Set Error Code
			return 0;
		}
	}
	// Fullscreen Specific 
	else
	{
		// Enable Fullscreen
		flags |= SDL_WINDOW_FULLSCREEN;
	}


	SDL_Window *screen = SDL_CreateWindow("Pentagram",
	                          SDL_WINDOWPOS_UNDEFINED,
	                          SDL_WINDOWPOS_UNDEFINED,
	                          width, height,
	                          flags);
	
	if (!screen)
	{
		// TODO: Set Error Code
		return 0;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, 0);
	if (!renderer)
	{
		// TODO: Set Error Code
		return 0;
	}


	SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING, width, height);
	if (!texture)
	{
		// TODO: Set Error Code
		return 0;
	}

	// Now create the SoftRenderSurface
	RenderSurface *surf;

	if (bpp == 32) surf = new SoftRenderSurface<uint32>(renderer, texture, width, height, bpp);
	else surf = new SoftRenderSurface<uint16>(renderer, texture, width, height, bpp);

	// Initialize gamma correction tables
	for (int i = 0; i < 256; i++)
	{
		Gamma22toGamma10[i] = static_cast<uint8>(0.5 + (std::pow (i/255.0, 2.2/1.0) * 255.0));
		Gamma10toGamma22[i] = static_cast<uint8>(0.5 + (std::pow (i/255.0, 1.0/2.2) * 255.0));
	}

	return surf;
}

// Create a SecondaryRenderSurface with an associated Texture object
RenderSurface *RenderSurface::CreateSecondaryRenderSurface(uint32 width, uint32 height)
{
	// Now create the SoftRenderSurface
	RenderSurface *surf;

	// TODO: Change this
	if (format.s_bpp == 32) surf = new SoftRenderSurface<uint32>(width,height);
	else surf = new SoftRenderSurface<uint16>(width,height);
	return surf;
}

RenderSurface::~RenderSurface()
{
}
