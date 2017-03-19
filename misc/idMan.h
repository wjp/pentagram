/*
Copyright (C) 2003-2013 The Pentagram team

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

#ifndef IDMAN_H
#define IDMAN_H

class IDataSource;
class ODataSource;

#include <vector>

//
// idMan. Used to allocate and keep track of unused ids.
// Supposed to be used by Kernel and World for pID and ObjId
//
// The idMan itself uses a nifty linked list that is also an array.
// As such it has the advantages of both. Adds and removals are fast,
// As is checking to see if an ID is used.
//
// idMan works as a LILO (last in last out) for id recycling. So an id that has
// been cleared will not be used until all other currently unused ids have been
// allocated.
//

class idMan
{
	uint16_t 		begin;			//!< start of the available range of IDs
	uint16_t 		end;			//!< current end of the range
	uint16_t 		max_end;		//!< end of the available range
	uint16_t 		startcount;		//!< number of IDs to make available initially

	uint16_t 		usedcount;		//!< number of IDs currently in use

	std::vector<uint16_t> ids;	//!< the 'next' field in a list of free IDs
	uint16_t		first;			//!< the first ID in the free list
	uint16_t		last;			//!< the last ID in the last list
public:
	//! \param begin start of the range of available IDs
	//! \param max_end end of the range of available IDs
	//! \param startcount number of IDs to make available initially (0 = all)
	idMan(uint16_t begin, uint16_t max_end, uint16_t startcount=0);
	~idMan();

	//! check if this idMan is full
	bool		isFull() const
		{ return first == 0 && end >= max_end; }

	//! clear all IDs, reset size to the startcount, and set max_end to new_max
	void		clearAll(uint16_t new_max = 0);

	//! get a free ID
	//! \return a free ID, or 0 if none are available
	uint16_t		getNewID();

	//! mark a given ID as used, expanding if necessary.
	//! Note: reserveID is O(n), so don't use too often.
	//! \return false if the ID was already used or is out of range
	bool		reserveID(uint16_t id);

	//! release an id
	void		clearID(uint16_t id);

	//! check if an ID is in use
	bool		isIDUsed(uint16_t id) const
		{ return id >= begin && id <= end && ids[id] == 0 && id != last; }

	//! increase the maximum size
	//! Note: this shouldn't be used in normal circumstances.
	//!       It exists for dumpMap currently. If that is rewritten not
	//!       to need more than 32768 object IDs, this function should be
	//!       deleted.
	void		setNewMax(uint16_t max_end) { this->max_end = max_end; }

	void save(ODataSource* ods);
	bool load(IDataSource* ids, uint32_t version);

private:
	//! double the amount of available IDs (up to the maximum passed 
	//! to the constructor)
	void expand();
};

#endif //IDMAN_H
