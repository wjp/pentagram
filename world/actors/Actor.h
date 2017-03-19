/*
Copyright (C) 2003-2007 The Pentagram team

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

#ifndef ACTOR_H
#define ACTOR_H

#include "Container.h"
#include "intrinsics.h"
#include "Animation.h"

class ActorAnimProcess;
struct PathfindingState;
class CombatProcess;

class Actor : public Container
{
	friend class ActorAnimProcess;
	friend class AnimationTracker;
public:
	Actor();
	~Actor();

	int16_t getStr() const { return strength; }
	void setStr(int16_t str) { strength = str; }
	int16_t getDex() const { return dexterity; }
	void setDex(int16_t dex) { dexterity = dex; }
	int16_t getInt() const { return intelligence; }
	void setInt(int16_t int_) { intelligence = int_; }
	uint16_t getHP() const { return hitpoints; }
	void setHP(uint16_t hp) { hitpoints = hp; }
	int16_t getMana() const { return mana; }
	void setMana(int16_t mp) { mana = mp; }

	int16_t getMaxMana() const;
	uint16_t getMaxHP() const;

	bool isDead() const { return (actorflags & ACT_DEAD) != 0; }

	bool isInCombat() const { return (actorflags & ACT_INCOMBAT) != 0; }
	void toggleInCombat() {
		if (isInCombat()) clearInCombat(); else setInCombat();
	}

	CombatProcess* getCombatProcess();
	virtual void setInCombat();
	virtual void clearInCombat();

	uint16_t getAlignment() const { return alignment; }
	void setAlignment(uint16_t a) { alignment = a; }
	uint16_t getEnemyAlignment() const { return enemyalignment; }
	void setEnemyAlignment(uint16_t a) { enemyalignment = a; }

	Animation::Sequence getLastAnim() const { return lastanim; }
	void setLastAnim(Animation::Sequence anim) { lastanim = anim; }
	uint16_t getDir() const { return direction; }
	void setDir(uint16_t dir) { direction = dir; }
	int32_t getFallStart() const { return fallstart; }
	void setFallStart(int32_t z) { fallstart = z; }
	void setUnk0C(uint8_t b) { unk0C = b; }

	uint32_t getActorFlags() const { return actorflags; }
	void setActorFlag(uint32_t mask) { actorflags |= mask; }
	void clearActorFlag(uint32_t mask) { actorflags &= ~mask; }

	//! set stats from MonsterInfo (hp, dex, alignment, enemyalignment)
	//! \return true if a MonsterInfo struct was found, false otherwise
	bool loadMonsterStats();

	//! add treasure according to the TreasureInfo in the MonsterInfo
	//! \return true if a MonsterInfo struct was found, false otherwise
	bool giveTreasure();

	virtual void teleport(int mapnum, int32_t x, int32_t y, int32_t z);

	virtual bool removeItem(Item* item);

	//! \return the PID of the spawned usecode process if any (otherwise 0)
	uint16_t schedule(uint32_t time);

	bool setEquip(Item* item, bool checkwghtvol=false);
	uint16_t getEquip(uint32_t type);

	virtual uint32_t getArmourClass();
	virtual uint16_t getDefenseType();
	virtual int16_t getAttackingDex();
	virtual int16_t getDefendingDex();

	virtual uint16_t getDamageType();
	virtual int getDamageAmount();

	//! calculate the damage an attack against this Actor does.
	//! \param other the attacker (can be zero)
	//! \param damage base damage
	//! \param type damage type
	//! \return the amount of damage to be applied. Zero if attack missed.
	int calculateAttackDamage(uint16_t other, int damage, uint16_t type);

	//! receive a hit
	//! \param damage base damage (or zero to use attacker's default damage)
	//! \param type damage type (or zero to use attacker's default type)
	virtual void receiveHit(uint16_t other, int dir, int damage, uint16_t type);

	//! die
	//! \param damageType damage type that caused the death
	//! \return the process ID of the death animation
	virtual ProcId die(uint16_t damageType);

	//! kill all processes except those related to combat
	void killAllButCombatProcesses();

	//! kill all animation processes except those related to dying/falling
	//! \return PID of animprocess doing the falling (or getting up)
	ProcId killAllButFallAnims(bool death);

	//! check if NPCs are near which are in combat mode and hostile
	bool areEnemiesNear();

	//! starts an activity
	//! \return processID of process handling the activity or zero
	uint16_t cSetActivity(int activity);

	//! run the given animation
	//! \return the PID of the ActorAnimProcess
	uint16_t doAnim(Animation::Sequence anim, int dir, unsigned int steps=0);

	//! check if this actor has a specific animation
	bool hasAnim(Animation::Sequence anim);

	//! check if the given animation can be done from the location in state,
	//! without walking into things. If state is non-zero, and successful,
	//! state will be updated to after the animation. If unsuccessful,
	//! the contents of state are undefined.
	//! \param anim Action to try
	//! \param dir direction to walk in
	//! \param state the state to start from, or 0 to use the current state
	Animation::Result tryAnim(Animation::Sequence anim, int dir, unsigned int steps=0, PathfindingState* state=0);

	//! create an actor, assign objid, make it ethereal and load monster stats.
	static Actor* createActor(uint32_t shape, uint32_t frame);

	virtual uint16_t assignObjId(); // assign an NPC objid

	virtual void dumpInfo();

	bool loadData(IDataSource* ids, uint32_t version);

	// p_dynamic_cast stuff
	ENABLE_RUNTIME_CLASSTYPE();

	INTRINSIC(I_isNPC);
	INTRINSIC(I_getDir);
	INTRINSIC(I_getLastAnimSet);
	INTRINSIC(I_pathfindToItem);
	INTRINSIC(I_pathfindToPoint);
	INTRINSIC(I_getStr);
	INTRINSIC(I_getDex);
	INTRINSIC(I_getInt);
	INTRINSIC(I_getHp);
	INTRINSIC(I_getMana);
	INTRINSIC(I_getAlignment);
	INTRINSIC(I_getEnemyAlignment);
	INTRINSIC(I_setStr);
	INTRINSIC(I_setDex);
	INTRINSIC(I_setInt);
	INTRINSIC(I_setHp);
	INTRINSIC(I_setMana);
	INTRINSIC(I_setAlignment);
	INTRINSIC(I_setEnemyAlignment);
	INTRINSIC(I_getMap);
	INTRINSIC(I_teleport);
	INTRINSIC(I_doAnim);
	INTRINSIC(I_isInCombat);
	INTRINSIC(I_setInCombat);
	INTRINSIC(I_clrInCombat);
	INTRINSIC(I_setTarget);
	INTRINSIC(I_getTarget);
	INTRINSIC(I_isEnemy);
	INTRINSIC(I_isDead);
	INTRINSIC(I_setDead);
	INTRINSIC(I_clrDead);
	INTRINSIC(I_isImmortal);
	INTRINSIC(I_setImmortal);
	INTRINSIC(I_clrImmortal);
	INTRINSIC(I_isWithstandDeath);
	INTRINSIC(I_setWithstandDeath);
	INTRINSIC(I_clrWithstandDeath);
	INTRINSIC(I_isFeignDeath);
	INTRINSIC(I_setFeignDeath);
	INTRINSIC(I_clrFeignDeath);
	INTRINSIC(I_areEnemiesNear);
	INTRINSIC(I_isBusy);
	INTRINSIC(I_createActor);
	INTRINSIC(I_cSetActivity);
	INTRINSIC(I_setAirWalkEnabled);
	INTRINSIC(I_getAirWalkEnabled);
	INTRINSIC(I_schedule);
	INTRINSIC(I_getEquip);
	INTRINSIC(I_setEquip);

	enum ActorFlags {
		ACT_INVINCIBLE     = 0x000001, // flags from npcdata byte 0x1B
		ACT_ASCENDING      = 0x000002,
		ACT_DESCENDING     = 0x000004,
		ACT_ANIMLOCK       = 0x000008,

		ACT_FIRSTSTEP      = 0x000400, // flags from npcdata byte 0x2F
		ACT_INCOMBAT       = 0x000800,
		ACT_DEAD           = 0x001000,
		ACT_COMBATRUN      = 0x008000,
		
		ACT_AIRWALK        = 0x010000, // flags from npcdata byte 0x30
		ACT_IMMORTAL       = 0x040000,
		ACT_WITHSTANDDEATH = 0x080000,
		ACT_FEIGNDEATH     = 0x100000,
		ACT_STUNNED        = 0x200000,
		ACT_POISONED       = 0x400000,
		ACT_PATHFINDING    = 0x800000
	};

protected:
	virtual void saveData(ODataSource* ods);

	int16_t strength;
	int16_t dexterity;
	int16_t intelligence;
	uint16_t hitpoints;
	int16_t mana;

	uint16_t alignment, enemyalignment;

	Animation::Sequence lastanim;
	uint16_t animframe;
	uint16_t direction;

	int32_t fallstart;
	uint8_t unk0C; // unknown byte 0x0C from npcdata.dat

	uint32_t actorflags;
};


#endif
