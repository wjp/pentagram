/*
 *	LoopScriptNodes.h -
 *
 *  Copyright (C) 2002-2003 The Pentagram Team
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

#ifndef LOOPSCRIPTNODES_H
#define LOOPSCRIPTNODES_H

#include "GenericNodes.h"


class LoopScriptNode : public Node
{
	public:
		LoopScriptNode(const uint32_t opcode, const uint32_t offset, const uint32_t newScriptTok)
			: Node(opcode, offset, Type(Type::T_INVALID)), scriptTok(newScriptTok)
			{
				assert(acceptOp(opcode, 0x74));
			};
		~LoopScriptNode() {};
		
		void print_unk(Console &o, const uint32_t isize) const;
		void print_asm(Console &o) const;
		void print_bin(ODequeDataSource &o) const;
		bool fold(DCUnit *unit, std::deque<Node *> &nodes);
		
		const uint32_t lsTok() const { return scriptTok; };

	protected:

	private:
		uint32_t scriptTok;
};

class LoopNode : public ColNode
{
	public:
		LoopNode() : ColNode() {};
		LoopNode(const uint32_t opcode, const uint32_t offset, const uint32_t newCurrObj, const uint32_t newStrSize, const uint32_t newSearchType)
			: ColNode(opcode, offset, Type(Type::T_VOID)), currObj(newCurrObj), strSize(newStrSize), searchType(newSearchType)
			{
				assert(acceptOp(opcode, 0x70));
			};
		~LoopNode() {};

		void print_unk(Console &o, const uint32_t isize) const;
		void print_asm(Console &o) const;
		void print_bin(ODequeDataSource &o) const;

		bool fold(DCUnit *unit, std::deque<Node *> &nodes);

	protected:

	private:
		uint32_t currObj;
		uint32_t strSize;
		uint32_t searchType;
};

class LoopNextNode : public Node
{
	public:
		LoopNextNode(const uint32_t opcode, const uint32_t offset)
			: Node(opcode, offset, Type(Type::T_INVALID))
			{
				assert(acceptOp(opcode, 0x73));
			};
		~LoopNextNode() {};
		
		void print_unk(Console &o, const uint32_t isize) const;
		void print_asm(Console &o) const;
		void print_bin(ODequeDataSource &o) const;
		bool fold(DCUnit *unit, std::deque<Node *> &nodes);
		
	protected:

	private:
};

#endif
