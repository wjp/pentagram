/*
Copyright (C) 2002,2003 The Pentagram team

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

#ifndef KERNEL_H
#define KERNEL_H

#include <list>
#include <map>
#include <vector>

#include "intrinsics.h"

class Process;
class idMan;
class Object;
class Actor;

class Kernel {
public:
	Kernel();
	~Kernel();

	static Kernel* get_instance() { return kernel; }

	uint16 addProcess(Process *proc); // returns pid of new process
	void removeProcess(Process *proc);
	bool runProcesses(uint32 framenum);
	Process* getProcess(uint16 pid);

	void setNextProcess(Process *proc);

	// only UCProcesses; objid = 0 means any object, type = 6 means any type
	uint32 getNumProcesses(uint16 objid, uint16 processtype);
	void killProcesses(uint16 objid, uint16 processtype);
	void killObjectProcesses();

	void kernelStats();

	uint16 assignObjId(Object* obj);
	uint16 assignActorObjId(Actor* obj, uint16 id=0xFFFF);
	void clearObjId(uint16 objid);
	Object* getObject(uint16 objid) const;

	INTRINSIC(I_getNumProcesses);
	INTRINSIC(I_resetRef);
private:
	std::list<Process*> processes;
	idMan	*pIDs;

	std::list<Process*>::iterator current_process;

	std::vector<Object*> objects;
	idMan* objIDs;
	idMan* actorIDs;

	static Kernel* kernel;
};


#endif
