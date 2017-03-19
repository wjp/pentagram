/*
Copyright (C) 2005 The Pentagram Team

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

#ifndef MANIPS_H_INCLUDED
#define MANIPS_H_INCLUDED

#include "RenderSurface.h"
#include "Texture.h"

template<class uintX> class Manip_Nat2Nat
{
public:
	static bool are_same() { return true; }
	static uintX copy (uintX src) { return src; }
	static void split(uintX src, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a) {
		UNPACK_RGBA8(src, r, g, b, a);
	}
	static uintX merge(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		return PACK_RGBA8(r, g, b, a);
	}
	static uint16_t to16bit(uintX src) { 
		uint8_t r,g,b;
		UNPACK_RGB(src,r,g,b);
		return (r>>3)|((g&0xFC)<<3)|((b&0xF8)<<8);
	}
};

class Manip_Nat2Nat_16
{
public:
	static bool are_same() { return true; }
	static uint16_t copy (uint16_t src) { return src; }
	static void split(uint16_t src, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a) {
		UNPACK_RGBA8(src, r, g, b, a);
	}
	static uint16_t merge(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		return PACK_RGBA8(r, g, b, a);
	}
	static uint16_t to16bit(uint16_t src) { 
		return src;
	}
};

class Manip_Nat2Nat_32
{
public:
	static bool are_same() { return true; }
	static uint32_t copy (uint32_t src) { return src; }
	static void split(uint32_t src, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a) {
		UNPACK_RGBA8(src, r, g, b, a);
	}
	static uint32_t merge(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		return PACK_RGBA8(r, g, b, a);
	}
	static uint16_t to16bit(uint32_t src) { 
		uint8_t r,g,b;
		UNPACK_RGB8(src,r,g,b);
		return (r>>3)|((g&0xFC)<<3)|((b&0xF8)<<8);
	}
};

template<class uintX> class Manip_Sta2Nat
{
public:
	static bool are_same() { return false; }
	static uintX copy (uint32_t src) { 
		return static_cast<uintX>(PACK_RGBA8( TEX32_R(src), TEX32_G(src), TEX32_B(src), TEX32_A(src) ));
	}
	static void split(uint32_t src, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a) {
		r = TEX32_R(src);
		g = TEX32_G(src);
		b = TEX32_B(src);
		a = TEX32_A(src);
	}
	static uintX merge(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		return PACK_RGBA8(r, g, b, a);
	}
	static uint16_t to16bit(uint32_t src) { 
		return (src>>3)|((src>>5)&0x7E0)|((src>>8)&0xF800);
	}
};

class Manip_Sta2Nat_16
{
public:
	static bool are_same() { return false; }
	static uint16_t copy (uint32_t src) { 
		return PACK_RGBA8( TEX32_R(src), TEX32_G(src), TEX32_B(src), TEX32_A(src) );
	}
	static void split(uint32_t src, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a) {
		r = TEX32_R(src);
		g = TEX32_G(src);
		b = TEX32_B(src);
		a = TEX32_A(src);
	}
	static uint16_t merge(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		return PACK_RGBA8(r, g, b, a);
	}
	static uint16_t to16bit(uint32_t src) { 
		return (src>>3)|((src>>5)&0x7E0)|((src>>8)&0xF800);
	}
};

class Manip_Sta2Nat_32
{
public:
	static bool are_same() { return false; }
	static uint32_t copy (uint32_t src) { 
		return PACK_RGBA8( TEX32_R(src), TEX32_G(src), TEX32_B(src), TEX32_A(src) );
	}
	static void split(uint32_t src, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a) {
		r = TEX32_R(src);
		g = TEX32_G(src);
		b = TEX32_B(src);
		a = TEX32_A(src);
	}
	static uint32_t merge(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		return PACK_RGBA8(r, g, b, a);
	}
	static uint16_t to16bit(uint32_t src) { 
		return (src>>3)|((src>>5)&0x7E0)|((src>>8)&0xF800);
	}
};

// Assumption, ABGR (but doesn't matter)
class Manip_32_A888
{
public:
	static bool are_same() { return true; }
	static uint32_t copy (uint32_t src) { return src; }
	static void split(uint32_t src, uint8_t &c0, uint8_t &c1, uint8_t &c2, uint8_t &a) {
		c0 = src; c1 = src>>8; c2 = src>>16; a = src>>24;
	}
	static uint32_t merge(uint8_t c0, uint8_t c1, uint8_t c2, uint8_t a) {
		return c0|(c1<<8)|(c2<<16)|(a<<24);
	}
	static uint16_t to16bit(uint32_t src) { 
		return (src>>3)|((src>>5)&0x7E0)|((src>>8)&0xF800);
	}
};

// Assumption, RGBA (but doesn't matter)
class Manip_32_888A
{
public:
	static bool are_same() { return true; }
	static uint32_t copy (uint32_t src) { return src; }
	static void split(uint32_t src, uint8_t &c0, uint8_t &c1, uint8_t &c2, uint8_t &a) {
		a = src; c2 = src>>8; c1 = src>>16; c0 = src>>24;
	}
	static uint32_t merge(uint8_t c0, uint8_t c1, uint8_t c2, uint8_t a) {
		return a|(c2<<8)|(c1<<16)|(c0<<24);
	}
	static uint16_t to16bit(uint32_t src) { 
		return (src>>27)|((src>>13)&0x7E0)|((src<<8)&0xF800);
	}
};

//
// Gamma Corrected Manips
//


class Manip_Nat2Nat_16_GC
{
public:
	static bool are_same() { return true; }
	static uint16_t copy (uint16_t src) { return src; }
	static void split(uint16_t src, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a) {
		UNPACK_RGBA8(src, r, g, b, a);
		r = RenderSurface::Gamma22toGamma10[r];
		g = RenderSurface::Gamma22toGamma10[g];
		b = RenderSurface::Gamma22toGamma10[b];
	}
	static uint16_t merge(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		return PACK_RGBA8(RenderSurface::Gamma10toGamma22[r], RenderSurface::Gamma10toGamma22[g], RenderSurface::Gamma10toGamma22[b], a);
	}
	static uint16_t to16bit(uint16_t src) { 
		return src;
	}
};

class Manip_Nat2Nat_32_GC
{
public:
	static bool are_same() { return true; }
	static uint32_t copy (uint32_t src) { return src; }
	static void split(uint32_t src, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a) {
		UNPACK_RGBA8(src, r, g, b, a);
		r = RenderSurface::Gamma22toGamma10[r];
		g = RenderSurface::Gamma22toGamma10[g];
		b = RenderSurface::Gamma22toGamma10[b];
	}
	static uint32_t merge(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		return PACK_RGBA8(RenderSurface::Gamma10toGamma22[r], RenderSurface::Gamma10toGamma22[g], RenderSurface::Gamma10toGamma22[b], a);
	}
	static uint16_t to16bit(uint32_t src) { 
		uint8_t r,g,b;
		UNPACK_RGB8(src,r,g,b);
		return (r>>3)|((g&0xFC)<<3)|((b&0xF8)<<8);
	}
};

class Manip_Sta2Nat_16_GC
{
public:
	static bool are_same() { return false; }
	static uint16_t copy (uint32_t src) { 
		return PACK_RGBA8( TEX32_R(src), TEX32_G(src), TEX32_B(src), TEX32_A(src) );
	}
	static void split(uint32_t src, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a) {
		r = RenderSurface::Gamma22toGamma10[TEX32_R(src)];
		g = RenderSurface::Gamma22toGamma10[TEX32_G(src)];
		b = RenderSurface::Gamma22toGamma10[TEX32_B(src)];
		a = TEX32_A(src);
	}
	static uint16_t merge(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		return PACK_RGBA8(RenderSurface::Gamma10toGamma22[r], RenderSurface::Gamma10toGamma22[g], RenderSurface::Gamma10toGamma22[b], a);
	}
	static uint16_t to16bit(uint32_t src) { 
		return (src>>3)|((src>>5)&0x7E0)|((src>>8)&0xF800);
	}
};

class Manip_Sta2Nat_32_GC
{
public:
	static bool are_same() { return false; }
	static uint32_t copy (uint32_t src) { 
		return PACK_RGBA8( TEX32_R(src), TEX32_G(src), TEX32_B(src), TEX32_A(src) );
	}
	static void split(uint32_t src, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a) {
		r = RenderSurface::Gamma22toGamma10[TEX32_R(src)];
		g = RenderSurface::Gamma22toGamma10[TEX32_G(src)];
		b = RenderSurface::Gamma22toGamma10[TEX32_B(src)];
		a = TEX32_A(src);
	}
	static uint32_t merge(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		return PACK_RGBA8(RenderSurface::Gamma10toGamma22[r], RenderSurface::Gamma10toGamma22[g], RenderSurface::Gamma10toGamma22[b], a);
	}
	static uint16_t to16bit(uint32_t src) { 
		return (src>>3)|((src>>5)&0x7E0)|((src>>8)&0xF800);
	}
};

// Assumption, ABGR (but doesn't matter)
class Manip_32_A888_GC
{
public:
	static bool are_same() { return true; }
	static uint32_t copy (uint32_t src) { return src; }
	static void split(uint32_t src, uint8_t &c0, uint8_t &c1, uint8_t &c2, uint8_t &a) {
		c0 = RenderSurface::Gamma22toGamma10[(src)&0xFF]; 
		c1 = RenderSurface::Gamma22toGamma10[(src>>8)&0xFF];
		c2 = RenderSurface::Gamma22toGamma10[(src>>16)&0xFF];
		a = src>>24;
	}
	static uint32_t merge(uint8_t c0, uint8_t c1, uint8_t c2, uint8_t a) {
		return RenderSurface::Gamma10toGamma22[c0]|(RenderSurface::Gamma10toGamma22[c1]<<8)|(RenderSurface::Gamma10toGamma22[c2]<<16)|(a<<24);
	}
	static uint16_t to16bit(uint32_t src) { 
		return (src>>3)|((src>>5)&0x7E0)|((src>>8)&0xF800);
	}
};

// Assumption, RGBA (but doesn't matter)
class Manip_32_888A_GC
{
public:
	static bool are_same() { return true; }
	static uint32_t copy (uint32_t src) { return src; }
	static void split(uint32_t src, uint8_t &c0, uint8_t &c1, uint8_t &c2, uint8_t &a) {
		a = src; 
		c2 = RenderSurface::Gamma22toGamma10[(src>>8)&0xFF]; 
		c1 = RenderSurface::Gamma22toGamma10[(src>>16)&0xFF]; 
		c0 = RenderSurface::Gamma22toGamma10[(src>>24)&0xFF];
	}
	static uint32_t merge(uint8_t c0, uint8_t c1, uint8_t c2, uint8_t a) {
		return a|(RenderSurface::Gamma10toGamma22[c2]<<8)|(RenderSurface::Gamma10toGamma22[c1]<<16)|(RenderSurface::Gamma10toGamma22[c0]<<24);
	}
	static uint16_t to16bit(uint32_t src) { 
		return (src>>27)|((src>>13)&0x7E0)|((src<<8)&0xF800);
	}
};


#endif //MANIPS_H_INCLUDED
