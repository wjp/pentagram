/*
RenderSurface.h : RenderSurface Interface header

Copyright (C) 2002-2004 The Pentagram Team

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

#ifndef RENDERSURFACE_H
#define RENDERSURFACE_H

#include "GraphicsErrors.h"

struct Texture;
class Shape;
class ShapeFont;
struct FixedWidthFont;

namespace Pentagram
{
	struct Palette;
	struct Rect;
	class Scaler;
}

#define UNPACK_RGB8(pix,r,g,b) { r = (((pix)&RenderSurface::format.r_mask)>>RenderSurface::format.r_shift)<<RenderSurface::format.r_loss; g = (((pix)&RenderSurface::format.g_mask)>>RenderSurface::format.g_shift)<<RenderSurface::format.g_loss; b = (((pix)&RenderSurface::format.b_mask)>>RenderSurface::format.b_shift)<<RenderSurface::format.b_loss; }
#define PACK_RGB8(r,g,b) ((((r)>>RenderSurface::format.r_loss)<<RenderSurface::format.r_shift) | (((g)>>RenderSurface::format.g_loss)<<RenderSurface::format.g_shift) | (((b)>>RenderSurface::format.b_loss)<<RenderSurface::format.b_shift))
#define PACK_RGB16(r,g,b) ((((r)>>RenderSurface::format.r_loss16)<<RenderSurface::format.r_shift) | (((g)>>RenderSurface::format.g_loss16)<<RenderSurface::format.g_shift) | (((b)>>RenderSurface::format.b_loss16)<<RenderSurface::format.b_shift))

#define UNPACK_RGBA8(pix,r,g,b,a) { r = (((pix)&RenderSurface::format.r_mask)>>RenderSurface::format.r_shift)<<RenderSurface::format.r_loss; g = (((pix)&RenderSurface::format.g_mask)>>RenderSurface::format.g_shift)<<RenderSurface::format.g_loss; b = (((pix)&RenderSurface::format.b_mask)>>RenderSurface::format.b_shift)<<RenderSurface::format.b_loss; ; a = (((pix)&RenderSurface::format.a_mask)>>RenderSurface::format.a_shift)<<RenderSurface::format.a_loss; }
#define PACK_RGBA8(r,g,b,a) ((((r)>>RenderSurface::format.r_loss)<<RenderSurface::format.r_shift) | (((g)>>RenderSurface::format.g_loss)<<RenderSurface::format.g_shift) | (((b)>>RenderSurface::format.b_loss)<<RenderSurface::format.b_shift) | (((a)>>RenderSurface::format.a_loss)<<RenderSurface::format.a_shift))
#define PACK_RGBA16(r,g,b,a) ((((r)>>RenderSurface::format.r_loss16)<<RenderSurface::format.r_shift) | (((g)>>RenderSurface::format.g_loss16)<<RenderSurface::format.g_shift) | (((b)>>RenderSurface::format.b_loss16)<<RenderSurface::format.b_shift) | (((a)>>RenderSurface::format.a_loss16)<<RenderSurface::format.a_shift))

//
// RenderSurface
//
// Desc: The base abstact class for rendering in Pentagram
//
class RenderSurface
{
public:

	// Colour shifting values (should these all be uint32???)
	struct Format
	{
		uint32	s_bpp,    s_bytes_per_pixel;
		uint32	r_loss,   g_loss,   b_loss,   a_loss;
		uint32	r_loss16, g_loss16, b_loss16, a_loss16;
		uint32	r_shift,  g_shift,  b_shift,  a_shift;
		uint32	r_mask,   g_mask,   b_mask,   a_mask;
	};

	static Format format;

	static uint8 Gamma10toGamma22[256];
	static uint8 Gamma22toGamma10[256];

	//
	// TODO: Improve the way SDL Surfaces are created. Should be more versatile.
	//

	//! Create a standard RenderSurface
	static RenderSurface *SetVideoMode(uint32 width, uint32 height, uint32 bpp, bool fullscreen, bool use_opengl);

	//! Create a SecondaryRenderSurface with an associated Texture object
	static RenderSurface *CreateSecondaryRenderSurface(uint32 width, uint32 height);

	// Virtual Destructor
	virtual ~RenderSurface();


	//
	// Being/End Painting
	//

	//! Begin painting to the buffer. MUST BE CALLED BEFORE DOING ANYTHING TO THE SURFACE!
	// \note Can be called multiple times
	// \return Error Code on error. Check return code.....
	virtual ECode BeginPainting() = 0;

	//! Finish paining to the buffer. 
	// \note MUST BE CALLED FOR EACH CALL TO BeginPainting()
	// \return Error Code on error. Check return code.....
	virtual ECode EndPainting() = 0;

	//! Get the surface as a Texture. Only valid for SecondaryRenderSurfaces
	// \note Do not delete the texture. 
	// \note Do not assume anything about the contents of the Texture object.
	// \note It should only be used with Painting and Blitting methods.
	virtual Texture *GetSurfaceAsTexture() = 0;

	//
	// Surface Properties
	//

	//! Set the Origin of the Surface
	virtual void SetOrigin(sint32 x, sint32 y) = 0;

	//! Set the Origin of the Surface
	virtual void GetOrigin(sint32 &x, sint32 &y) const = 0;

	//! Get the Surface Dimensions
	virtual void GetSurfaceDims(Pentagram::Rect &) const = 0;

	//! Get Clipping Rectangle
	virtual void GetClippingRect(Pentagram::Rect &) const = 0;

	//! Set Clipping Rectangle
	virtual void SetClippingRect(const Pentagram::Rect &) = 0;

	//! Check Clipped. -1 if off screen, 0 if not clipped, 1 if clipped
	virtual sint16 CheckClipped(const Pentagram::Rect &) const = 0;

	//! Flip the surface
	virtual void SetFlipped(bool flipped) = 0;

	//! Has the render surface been flipped?
	virtual bool IsFlipped() const = 0;


	//
	// Surface Palettes
	//
	// TODO: Handle Ultima8 and Crusader Xforms
	//

	// Set The Surface Palette
	// virtual void SetPalette(uint8 palette[768]) = 0;

	// Set The Surface Palette to be the one used by another surface
	// TODO: virtual void SetPalette(RenderSurface &) = 0;

	// Get The Surface Palette
	// TODO: virtual void GetPalette(uint8 palette[768]) = 0;

	virtual void CreateNativePalette(Pentagram::Palette* palette) = 0;


	//
	// Surface Filling
	//

	//! Fill buffer (using a palette index)
	virtual void Fill8(uint8 index, sint32 sx, sint32 sy, sint32 w, sint32 h) = 0;

	//! Fill buffer (using a RGB colour)
	virtual void Fill32(uint32 rgb, sint32 sx, sint32 sy, sint32 w, sint32 h) = 0;

	//! Fill alpha channel
	virtual void FillAlpha(uint8 alpha, sint32 sx, sint32 sy, sint32 w, sint32 h) = 0;

	//! Fill the region doing alpha blending 
	virtual void FillBlended(uint32 rgba, sint32 sx, sint32 sy, sint32 w, sint32 h) = 0;

	//
	// The rule for painting methods:
	//
	// First arg are the source object to 'draw' with
	// Next args are any other required data to define the 'source'
	// Next args are the destination position
	//

	//
	// Basic Shape Painting
	//

	//! Paint a Shape
	// TODO: virtual void Paint(CachedShape* s, uint32 frame, sint32 x, sint32 y) = 0;
	virtual void Paint(Shape*s, uint32 frame, sint32 x, sint32 y, bool untformed_pal = false) = 0;

	//! Paint a Shape without clipping
	// TODO: virtual void PaintNoClip(CachedShape*s, uint32 frame, sint32 x, sint32 y) = 0;
	virtual void PaintNoClip(Shape*s, uint32 frame, sint32 x, sint32 y, bool untformed_pal = false) = 0;

	//! Paint a Translucent Shape. 
	// TODO: virtual void PaintTranslucent(CachedShape* s, uint32 frame, sint32 x, sint32 y) = 0;
	virtual void PaintTranslucent(Shape* s, uint32 frame, sint32 x, sint32 y, bool untformed_pal = false) = 0;

	//! Paint a Mirrored Shape
	// TODO: virtual void PaintMirrored(CachedShape* s, uint32 frame, sint32 x, sint32 y, bool trans = false) = 0;
	virtual void PaintMirrored(Shape* s, uint32 frame, sint32 x, sint32 y, bool trans = false, bool untformed_pal = false) = 0;

	//! Paint an Invisible Shape
	// TODO: virtual void PaintInvisible(CachedShape* s, uint32 frame, sint32 x, sint32 y, bool trans, bool mirrored) = 0;
	virtual void PaintInvisible(Shape* s, uint32 frame, sint32 x, sint32 y, bool trans, bool mirrored, bool untformed_pal = false) = 0;

	//! Paint a Highlighted Shape of using the 32 Bit Colour col32 (0xAARRGGBB Alpha is blend level)
	// TODO: virtual void PaintHighlight(CachedShape* s, uint32 frame, sint32 x, sint32 y, bool trans, bool mirrored, uint32 col32);
	virtual void PaintHighlight(Shape* s, uint32 frame, sint32 x, sint32 y, bool trans, bool mirrored, uint32 col32, bool untformed_pal = false) = 0;

	//! Paint a Invisible Highlighted Shape of using the 32 Bit Colour col32 (0xAARRGGBB Alpha is blend level)
	// TODO: virtual void PaintHighlightInvis(CachedShape* s, uint32 frame, sint32 x, sint32 y, bool trans, bool mirrored, uint32 col32);
	virtual void PaintHighlightInvis(Shape* s, uint32 frame, sint32 x, sint32 y, bool trans, bool mirrored, uint32 col32, bool untformed_pal = false) = 0;

	//! Paint a shape masked against destination alpha
	virtual void PaintMasked(Shape* s, uint32 framenum, sint32 x, sint32 y, bool trans = false, bool mirrored = false, uint32 col32=0, bool untformed_pal = false) = 0;

	//
	// Basic Line Drawing
	//
	
	// Draw a Line (using a palette index)
	// TODO: virtual void DrawLine8(uint8 index, sint32 sx, sint32 sy, sint32 ex, sint32 ey) = 0;

	// Draw a RGB Line
	virtual void DrawLine32(uint32 rgb, sint32 sx, sint32 sy, sint32 ex, sint32 ey) = 0;


	//
	// Basic Font Drawing
	//
	//! Draw FixedWidthFont
	virtual void PrintTextFixed(FixedWidthFont *, const char *text, int x, int y) = 0;

	//! Draw a fixed width character from a FixedWidthFont
	virtual void PrintCharFixed(FixedWidthFont *, int character, int x, int y) = 0;


	//
	// Basic Texture Blitting
	//

	//! Blit a region from a Texture (Alpha == 0 -> skipped)
	virtual void Blit(Texture *, sint32 sx, sint32 sy, sint32 w, sint32 h, sint32 dx, sint32 dy, bool alpha_blend=false) = 0;

	//! Blit a region from a Texture with a Colour blend (AlphaTex == 0 -> skipped. AlphaCol32 -> Blend Factors)
	virtual void FadedBlit(Texture *, sint32 sx, sint32 sy, sint32 w, sint32 h, sint32 dx, sint32 dy, uint32 col32, bool alpha_blend=false) = 0;

	//! Blit a region from a Texture with a Colour blend masked based on DestAlpha (AlphaTex == 0 || AlphaDest == 0 -> skipped. AlphaCol32 -> Blend Factors)
	virtual void MaskedBlit(Texture *, sint32 sx, sint32 sy, sint32 w, sint32 h, sint32 dx, sint32 dy, uint32 col32, bool alpha_blend=false) = 0;

	//! Blit a stretched region from a Texture (Alpha == 0 -> skipped???)
	virtual void StretchBlit(Texture *, sint32 sx, sint32 sy, sint32 sw, sint32 sh, sint32 dx, sint32 dy, sint32 dw, sint32 dh, bool bilinear = false, bool clampedges = false) = 0;

	//! Blit a region from a Texture using a scaler
	virtual bool ScalerBlit(Texture *, sint32 sx, sint32 sy, sint32 sw, sint32 sh, sint32 dx, sint32 dy, sint32 dw, sint32 dh, const Pentagram::Scaler *, bool clampedges = false) = 0;


	////////////////////////////////////////
	// TODO: Add in Abstract 3d code Here //
	////////////////////////////////////////
};

#endif
