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

#ifndef UCMACHINE_H
#define UCMACHINE_H

#include <map>
#include <set>

#include "intrinsics.h"

class Process;
class UCProcess;
class ConvertUsecode;
class IDataSource;
class ODataSource;
class BitSet;
class UCList;
class idMan;

class UCMachine
{
public:
	UCMachine(Intrinsic *iset, unsigned int icount);
	~UCMachine();

	static UCMachine* get_instance() { return ucmachine; }

	void reset();

	void execProcess(UCProcess* proc);

	std::string& getString(uint16_t str);
	UCList* getList(uint16_t l);

	void freeString(uint16_t s);
	void freeStringList(uint16_t l);
	void freeList(uint16_t l);

	uint16_t duplicateString(uint16_t str);

	void usecodeStats();

	static uint32_t listToPtr(uint16_t l);
	static uint32_t stringToPtr(uint16_t s);
	static uint32_t stackToPtr(uint16_t pid, uint16_t offset);
	static uint32_t globalToPtr(uint16_t offset);
	static uint32_t objectToPtr(uint16_t objID);

	static uint16_t ptrToObject(uint32_t ptr);

	bool assignPointer(uint32_t ptr, const uint8_t* data, uint32_t size);
	bool dereferencePointer(uint32_t ptr, uint8_t* data, uint32_t size);

	void saveGlobals(ODataSource* ods);
	void saveStrings(ODataSource* ods);
	void saveLists(ODataSource* ods);

	bool loadGlobals(IDataSource* ids, uint32_t version);
	bool loadStrings(IDataSource* ids, uint32_t version);
	bool loadLists(IDataSource* ids, uint32_t version);

	INTRINSIC(I_true);
	INTRINSIC(I_dummyProcess);
	INTRINSIC(I_getName);
	INTRINSIC(I_urandom);
	INTRINSIC(I_rndRange);
	INTRINSIC(I_numToStr);
	INTRINSIC(I_getCurrentTimerTick);

protected:
	void loadIntrinsics(Intrinsic *i, unsigned int icount);

private:

	ConvertUsecode*	convuse;
	Intrinsic* intrinsics;
	unsigned int intrinsiccount;

	BitSet* globals;

	std::map<uint16_t, UCList*> listHeap;
	std::map<uint16_t, std::string> stringHeap;

	uint16_t assignString(const char* str);
	uint16_t assignList(UCList* l);

	idMan* listIDs;
	idMan* stringIDs;

	static UCMachine* ucmachine;

	static void		ConCmd_getGlobal(const Console::ArgvType &argv);
	static void		ConCmd_setGlobal(const Console::ArgvType &argv);


#ifdef DEBUG
	// tracing
	bool tracing_enabled;
	bool trace_all;
	bool trace_events;
	std::set<ObjId> trace_ObjIDs;
	std::set<ProcId> trace_PIDs;
	std::set<uint16_t> trace_classes;

	inline bool trace_show(ProcId pid, ObjId objid, uint16_t ucclass) {
		if (!tracing_enabled) return false;
		if (trace_all) return true;
		if (trace_ObjIDs.find(objid) != trace_ObjIDs.end()) return true;
		if (trace_PIDs.find(pid) != trace_PIDs.end()) return true;
		if (trace_classes.find(ucclass) != trace_classes.end()) return true;
		return false;
	}

public:
	bool trace_event() {
		return (tracing_enabled && (trace_all || trace_events));
	}

private:
	static void		ConCmd_tracePID(const Console::ArgvType &argv);
	static void		ConCmd_traceObjID(const Console::ArgvType &argv);
	static void		ConCmd_traceClass(const Console::ArgvType &argv);
	static void		ConCmd_traceAll(const Console::ArgvType &argv);
	static void		ConCmd_traceEvents(const Console::ArgvType &argv);
	static void		ConCmd_stopTrace(const Console::ArgvType &argv);
#endif
};

#endif
