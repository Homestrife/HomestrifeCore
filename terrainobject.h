#ifndef __TERRAINOBJECT_H_
#define __TERRAINOBJECT_H_

#include "hsobject.h"

enum Blockability
{
	UNBLOCKABLE, //unblockable!
	HIGH, //must be blocked high
	LOW, //must be blocked low
	MID //can be blocked high or low
};

class TerrainObjectHold : public HSObjectHold
{
public:
	//first of a list of all the hit boxes in this hold that can strike hurt boxes in other objects
	HSBox * firstAttackBox;
	HSBox * lastAttackBox;

	//first of a list of all the hitboxes in this hold that can be struck by the attack boxes of other objects
	HSBox * firstHurtBox;
	HSBox * lastHurtBox;

	//sounds to be played by this object when one of its attackboxes meets something else's hurtbox
	list<AudioInstance> hitAudioList; 

	//sounds to be played by this object when one of its attackboxes meets something else's hurtbox and is blocked
	list<AudioInstance> blockedAudioList; 

	//whether or not to actually change attack attributes this hold
	bool changeAttackBoxAttributes;
	
	//how the object must be blocked
	Blockability blockability;

	//whether or not this object must be blocked in a direction relative to its horizontal motion.
	//otherwise, it must be blocked based on its relative horizontal position
	bool horizontalDirectionBasedBlock;

	//whether or not this object must be blocked in the reverse horizontal direction
	bool reversedHorizontalBlock;

	//how much damage this object does when one of its attackboxes meets something else's hurtbox
	int damage;

	//how long this object is frozen after a hit/hurt impact
	int ownHitstop;

	//how long a hittable thing is frozen after a hit/hurt impact
	int victimHitstop;

	//for how long a hittable thing is stunned when hit by this object
	int hitstun;

	//for how long a hittable thing is stunned when it blocks this object
	int blockstun;

	//force imparted upon something this object hits. strong vertical forces can send grounded objects airborn
	HSVect2D force;

	//whether or not this attack knocks a standing opponent prone
	bool trips;

	//whether or not to clear out the victims list, if past attack boxes hit anything. (this allows multiple hits in one attack)
	bool resetHits;

	TerrainObjectHold();
	~TerrainObjectHold();

	virtual bool IsTerrainObjectHold();

protected:
};

struct AttackResults
{
	bool struck; //lets the object know it was actually struck
	bool didStrike; //lets the object know that it hit something
	bool hFlip;
	int damage; //this is cumulative
	int hitstop;
	int hitstun;
	int blockstun;
	HSVect2D force;
	Blockability blockability;
	bool IPSTriggered;
};

struct TerrainEventHolds
{
	TerrainObjectHold * healthDeath;
};

class TerrainObject : public HSObject
{
public:
	HSBox * firstTerrainBox; //the first of a list of hitboxes that only interact with other terrain boxes
	HSBox * lastTerrainBox;

	bool collides; //whether or not to involve this object in collisions
	bool collideParent; //whether or not this should collide with its parents attack/hurt boxes

	bool canBeJumpedThrough; //whether or not the object can be jumped through (but stood on top of) if it is terrain

	//how much damage this object can take
	int health;
	int curHealth;

	//a percentage (0.0 to 1.0) of how much velocity is retained in impacts
	float bounce;

	//a percentage (0.0 to 1.0) of how much velocity is removed when sliding or skipping across a surface
	float friction;

	//whether or not this object takes damage from blunt terrain impacts
	bool takesTerrainDamage;

	//whether or not this object is destroyed upon ANY impact
	bool fragile;

	//statistics defined by the last hold to change them. They automatically get reset to default values whenever a nextHold is null, or an event is triggered
	list<AudioInstance> hitAudioList;
	list<AudioInstance> blockedAudioList;
	Blockability blockability;
	bool horizontalDirectionBasedBlock;
	bool reversedHorizontalBlock;
	short damage;
	short ownHitstop;
	short victimHitstop;
	short hitstun;
	short blockstun;
	HSVect2D force;
	bool trips;

	int curHitstop;

	//all pending changes caused during the attack collision step
	AttackResults attackResults;

	//ids of other objects struck by this one's attacks
	list<int> victims;

	//number of objects struck by an attack box this frame
	int numHitThisFrame;
	int numBlockedByThisFrame;

	//the holds this object moves to upon particular events
	TerrainEventHolds terrainEventHolds;

	TerrainObject();
	~TerrainObject();
	
	virtual int Event(InputStates * inputHistory, int frame); //handle events
	virtual int Update();
	virtual int CollideAttack(list<HSObject*> * gameObjects);
	virtual void ApplyAttackResults();
	virtual void HandleHurtCollision(TerrainObject * attacker);
	virtual list<AudioInstance*> GetAudio(); //get any audio this object currently wishes to play

	virtual bool IsTerrain();
	virtual bool IsTerrainObject();
	
	virtual bool AdvanceHold(HSObjectHold * hold);
	virtual bool ChangeHold(HSObjectHold * hold);

protected:
	virtual void ResetAttackResults();
	virtual HSObjectHold * GetDefaultHold();

	//finds out if two rectangles are overlapping
	bool AreRectanglesColliding(HSVect2D * boxOnePos, HSBox * boxOne, HSVect2D * boxTwoPos, HSBox * boxTwo);

	//Applies the effects of one of this object's attack boxes striking another object's hurt box
	virtual void HandleAttackCollision(TerrainObject * targetObject);

	//checks whether the given attack box is colliding with the given hurt box
	bool IsAttackBoxColliding(HSBox * ownBox, TerrainObject * targetObject, HSBox * targetBox);

	//check this object against a particular object's hurt boxes
	void IsCollidingWithObjectHurtBoxes(TerrainObject * targetObject);

	//check this object against all other non-terrain objects
	void IsCollidingWithAnyHurtBoxes(list<HSObject*> * gameObjects);
};

#endif