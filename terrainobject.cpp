#include "terrainobject.h"

/////////////////////
//TerrainObjectHold//
/////////////////////

TerrainObjectHold::TerrainObjectHold() : HSObjectHold()
{
	firstAttackBox = NULL;
	lastAttackBox = NULL;
	firstHurtBox = NULL;
	lastHurtBox = NULL;
	hitAudioList.clear();
	changeAttackBoxAttributes = false;
	hitLevel = HIT_MID;
	blockability = BLOCKABLE;
	invulnerability = INVULN_NONE;
	horizontalDirectionBasedBlock = false;
	reversedHorizontalBlock = false;
	damage = 0;
	overrideOwnHitstop = false;
	ownHitstop = 0;
	overrideVictimHitstop = false;
	victimHitstop = 0;
	hitstun = 0;
	blockstun = 0;
	force.x = 0;
	force.y = 0;
	trips = false;
	resetHits = false;
	changeHurtBoxAttributes = false;
}

TerrainObjectHold::~TerrainObjectHold()
{
	HSBox * boxToDelete = firstAttackBox;
	HSBox * nextBoxToDelete;

	while(boxToDelete != NULL)
	{
		nextBoxToDelete = boxToDelete->nextBox;

		delete boxToDelete;

		boxToDelete = nextBoxToDelete;
	}
	
	boxToDelete = firstHurtBox;

	while(boxToDelete != NULL)
	{
		nextBoxToDelete = boxToDelete->nextBox;

		delete boxToDelete;

		boxToDelete = nextBoxToDelete;
	}
}

bool TerrainObjectHold::IsTerrainObjectHold()
{
	return true;
}

/////////////////
//TerrainObject//
/////////////////

TerrainObject::TerrainObject() : HSObject()
{
	firstTerrainBox = NULL;
	lastTerrainBox = NULL;
	collides = true;
	canBeJumpedThrough = false;
	health = 0;
	curHealth = health;
	lives = 0;
	bounce = 0;
	friction = 1;
	takesTerrainDamage = false;
	fragile = false;

	terrainEventHolds.healthDeath = NULL;

	ResetAttackResults();

	victims.clear();

	numHitThisFrame = 0;
	numBlockedByThisFrame = 0;
}

TerrainObject::~TerrainObject()
{
	HSBox * boxToDelete = firstTerrainBox;
	HSBox * nextBoxToDelete;

	while(boxToDelete != NULL)
	{
		nextBoxToDelete = boxToDelete->nextBox;

		delete boxToDelete;

		boxToDelete = nextBoxToDelete;
	}


}

int TerrainObject::Event(InputStates * inputHistory, int frame)
{
	return 0;
}

int TerrainObject::Update()
{
	return HSObject::Update();
}

bool TerrainObject::AdvanceHold(HSObjectHold* hold)
{
	if(HSObject::AdvanceHold(hold))
	{
		//we can be confident that this is actually a TerrainObjectHold
		TerrainObjectHold * tHold = (TerrainObjectHold*)curHold;

		if(tHold->changeAttackBoxAttributes)
		{
			//get the new statistics
			hitAudioList = tHold->hitAudioList;
			blockedAudioList = tHold->blockedAudioList;
			hitLevel = tHold->hitLevel;
			blockability = tHold->blockability;
			horizontalDirectionBasedBlock = tHold->horizontalDirectionBasedBlock;
			reversedHorizontalBlock = tHold->reversedHorizontalBlock;
			damage = tHold->damage;
			hitstun = tHold->hitstun;
			blockstun = tHold->blockstun;
			force.x = tHold->force.x;
			force.y = tHold->force.y;
			trips = tHold->trips;

			//reset victims
			if(tHold->resetHits) { victims.clear(); }
		}

		if(tHold->changeHurtBoxAttributes)
		{
			invulnerability = tHold->invulnerability;
		}

		if(tHold->overrideOwnHitstop)
		{
			ownHitstop = tHold->ownHitstop;
		}
		else
		{
			ownHitstop = DEFAULT_HITSTOP;
		}

		if(tHold->overrideVictimHitstop)
		{
			victimHitstop = tHold->victimHitstop;
		}
		else
		{
			victimHitstop = DEFAULT_HITSTOP;
		}

		return true;
	}
	
	return false;
}

bool TerrainObject::ChangeHold(HSObjectHold * hold)
{
	return HSObject::ChangeHold(hold);
}

HSObjectHold * TerrainObject::GetDefaultHold()
{
	victims.clear();
	hitAudioList.clear();
	hitLevel = HIT_MID;
	blockability = BLOCKABLE;
	invulnerability = INVULN_NONE;
	horizontalDirectionBasedBlock = false;
	reversedHorizontalBlock = false;
	damage = 0;
	hitstun = 0;
	blockstun = 0;
	force.x = 0;
	force.y = 0;
	trips = false;

	return HSObject::GetDefaultHold();
}

bool TerrainObject::IsTerrain()
{
	return true;
}

bool TerrainObject::IsTerrainObject()
{
	return true;
}

bool TerrainObject::AreRectanglesColliding(HSVect2D * boxOnePos, HSBox * boxOne, HSVect2D * boxTwoPos, HSBox * boxTwo)
{
	if(boxOnePos == NULL || boxOne == NULL || boxTwoPos == NULL || boxTwo == NULL) { return false; }

	float boxOneLeft = boxOnePos->x + boxOne->offset.x;
	float boxOneRight = boxOneLeft + boxOne->width;
	float boxOneTop = boxOnePos->y + boxOne->offset.y;
	float boxOneBottom = boxOneTop + boxOne->height;

	float boxTwoLeft = boxTwoPos->x + boxTwo->offset.x;
	float boxTwoRight = boxTwoLeft + boxTwo->width;
	float boxTwoTop = boxTwoPos->y + boxTwo->offset.y;
	float boxTwoBottom = boxTwoTop + boxTwo->height;

	if(boxOneLeft < boxTwoRight &&
		boxOneRight > boxTwoLeft &&
		boxOneTop < boxTwoBottom &&
		boxOneBottom > boxTwoTop)
	{
		return true;
	}

	return false;
}

void TerrainObject::ResetAttackResults()
{
	//reset all of the attack results to zero or null
	attackResults.timesStruck = 0;
	attackResults.didStrike = false;
	attackResults.hFlip = false;
	attackResults.damage = 0;
	attackResults.hitstop = 0;
	attackResults.hitstun = 0;
	attackResults.blockstun = 0;
	attackResults.force.x = 0;
	attackResults.force.y = 0;
	attackResults.hitLevel = HIT_MID;
	attackResults.blockability = BLOCKABLE;
	attackResults.IPSTriggered = false;
}

bool TerrainObject::HandleHurtCollision(TerrainObject * attacker)
{
	//get the effective hit level
	HitLevel effectiveHitLevel = attacker->hitLevel;
	if(attacker->hitLevel == HIT_STRICTLY_MID
		|| (attacker->hitLevel == HIT_HIGH && attacker->pos.y > pos.y)
		|| (attacker->hitLevel == HIT_LOW && attacker->pos.y < pos.y))
	{
		effectiveHitLevel = HIT_MID;
	}
	else if(attacker->blockability == HIT_MID)
	{
		if(attacker->pos.y < pos.y && attacker->vel.y >= 0)
		{
			effectiveHitLevel = HIT_HIGH;
		}
		else if(attacker->pos.y > pos.y && attacker->vel.y <= 0)
		{
			effectiveHitLevel = HIT_LOW;
		}
	}

	//handle invuln
	if(invulnerability == INVULN_FULL
		|| (effectiveHitLevel == HIT_HIGH && invulnerability == INVULN_HIGH)
		|| (effectiveHitLevel == HIT_LOW && invulnerability == INVULN_LOW))
	{
		return false;
	}

	//handle getting hit
	attackResults.timesStruck++;
	attackResults.damage += attacker->damage;
	attackResults.hitstop = attacker->victimHitstop;
	attacker->numHitThisFrame++;

	return true;
}

list<AudioInstance*> TerrainObject::GetAudio()
{
	if(audioPlayed)
	{
		numHitThisFrame = 0;
		numBlockedByThisFrame = 0;
		return HSObject::GetAudio();
	}

	list<AudioInstance*> audio = HSObject::GetAudio();

	//hit sounds
	for(int i = 0; i < numHitThisFrame; i++)
	{
		list<AudioInstance*> chanceHitAudio;
		chanceHitAudio.clear();

		list<AudioInstance>::iterator audIt;
		for ( audIt=hitAudioList.begin(); audIt != hitAudioList.end(); audIt++)
		{
			if((*audIt).usePercentage)
			{
				chanceHitAudio.push_back(&(*audIt));
			}
			else
			{
				audio.push_back(&(*audIt));
			}
		}

		if(chanceHitAudio.size() > 0)
		{
			int maxRand = 0;
			int cur = 0;

			list<AudioInstance*>::iterator chanceAudIt;
			for ( chanceAudIt=chanceHitAudio.begin(); chanceAudIt != chanceHitAudio.end(); chanceAudIt++)
			{
				maxRand += (*chanceAudIt)->percentage;
			}

			if(maxRand < 100) { maxRand = 100; }

			int random = rand() % maxRand + 1;
		
			for ( chanceAudIt=chanceHitAudio.begin(); chanceAudIt != chanceHitAudio.end(); chanceAudIt++)
			{
				if(random <= cur + (*chanceAudIt)->percentage)
				{
					audio.push_back((*chanceAudIt));
					break;
				}

				cur += (*chanceAudIt)->percentage;
			}
		}
	}

	numHitThisFrame = 0;

	//blocked sounds
	for(int i = 0; i < numBlockedByThisFrame; i++)
	{
		list<AudioInstance*> chanceBlockedAudio;
		chanceBlockedAudio.clear();

		list<AudioInstance>::iterator audIt;
		for ( audIt=blockedAudioList.begin(); audIt != blockedAudioList.end(); audIt++)
		{
			if((*audIt).usePercentage)
			{
				chanceBlockedAudio.push_back(&(*audIt));
			}
			else
			{
				audio.push_back(&(*audIt));
			}
		}

		if(chanceBlockedAudio.size() > 0)
		{
			int maxRand = 0;
			int cur = 0;

			list<AudioInstance*>::iterator chanceAudIt;
			for ( chanceAudIt=chanceBlockedAudio.begin(); chanceAudIt != chanceBlockedAudio.end(); chanceAudIt++)
			{
				maxRand += (*chanceAudIt)->percentage;
			}

			if(maxRand < 100) { maxRand = 100; }

			int random = rand() % maxRand + 1;
		
			for ( chanceAudIt=chanceBlockedAudio.begin(); chanceAudIt != chanceBlockedAudio.end(); chanceAudIt++)
			{
				if(random <= cur + (*chanceAudIt)->percentage)
				{
					audio.push_back((*chanceAudIt));
					break;
				}

				cur += (*chanceAudIt)->percentage;
			}
		}
	}

	numBlockedByThisFrame = 0;

	return audio;
}

void TerrainObject::HandleAttackCollision(TerrainObject * targetObject)
{
	//first, let the target know it's been hit
	if(!targetObject->HandleHurtCollision(this)) { return; }

	//save the target so this object doesn't strike it again until the attack is over
	victims.push_back(targetObject->id);

	attackResults.hitstop = ownHitstop;
	attackResults.didStrike = true;
	
	if(fragile) { toDelete = true; }
}

bool TerrainObject::IsAttackBoxColliding(HSBox * ownBox, TerrainObject * targetObject, HSBox * targetBox)
{
	if(ownBox == NULL || targetBox == NULL || targetBox == NULL) { return false; } //a position or box is not given

	//get own box's change in position from the perspective of the target object
	HSVect2D posChange;
	posChange.x = (pos.x - prevPos.x) - (targetObject->pos.x - targetObject->prevPos.x);
	posChange.y = (pos.y - prevPos.y) - (targetObject->pos.y - targetObject->prevPos.y);

	//get the actual offsets of both boxes, based on rotation
	HSBox actualOwnBox;
	actualOwnBox.width = ownBox->width;
	actualOwnBox.height = ownBox->height;
	actualOwnBox.offset.x = ownBox->offset.x;
	actualOwnBox.offset.y = ownBox->offset.y;

	if(hFlip)
	{
		actualOwnBox.offset.x = (ownBox->offset.x + ownBox->width) * -1;
	}

	HSBox actualTargetBox;
	actualTargetBox.width = targetBox->width;
	actualTargetBox.height = targetBox->height;
	actualTargetBox.offset.x = targetBox->offset.x;
	actualTargetBox.offset.y = targetBox->offset.y;

	if(targetObject->hFlip)
	{
		actualTargetBox.offset.x = (targetBox->offset.x + targetBox->width) * -1;
	}

	//get the total bounding box of both positions
	HSBox totalBox;
	totalBox.width = actualOwnBox.width + abs(posChange.x);
	totalBox.height = actualOwnBox.height + abs(posChange.y);
	if(posChange.x < 0) { totalBox.offset.x = actualOwnBox.offset.x; }
	else { totalBox.offset.x = actualOwnBox.offset.x - abs(posChange.x); }
	if(posChange.y < 0) { totalBox.offset.y = actualOwnBox.offset.y; }
	else { totalBox.offset.y = actualOwnBox.offset.y - abs(posChange.y); }

	//see if the target box's rectangle falls anywhere in the total bounding box
	if(!AreRectanglesColliding(&pos, &totalBox, &(targetObject->pos), &actualTargetBox))
	{
		//the target box definitely isn't colliding with the total box at any point
		return false;
	}

	//target box falls within the total bounding box
	if(posChange.x == 0 || posChange.y == 0)
	{
		//box only moved on one or no axes, so there's definitely a collision
		return true;
	}

	//own box moves on both axes.
	//get the potential collision timeframe
	HSVect2D tMin, tMax;
	if(posChange.x < 0)
	{
		tMin.x = ((targetObject->pos.x + actualTargetBox.offset.x + actualTargetBox.width) - (pos.x + actualOwnBox.offset.x)) / posChange.x;
		tMax.x = ((targetObject->pos.x + actualTargetBox.offset.x) - (pos.x + actualOwnBox.offset.x + actualOwnBox.width)) / posChange.x;
	}
	else if(posChange.x > 0)
	{
		tMin.x = ((targetObject->pos.x + actualTargetBox.offset.x) - (pos.x + actualOwnBox.offset.x + actualOwnBox.width)) / posChange.x;
		tMax.x = ((targetObject->pos.x + actualTargetBox.offset.x + actualTargetBox.width) - (pos.x + actualOwnBox.offset.x)) / posChange.x;
	}
	if(posChange.y < 0)
	{
		tMin.y = ((targetObject->pos.y + actualTargetBox.offset.y + actualTargetBox.height) - (pos.y + actualOwnBox.offset.y)) / posChange.y;
		tMax.y = ((targetObject->pos.y + actualTargetBox.offset.y) - (pos.y + actualOwnBox.offset.y + actualOwnBox.height)) / posChange.y;
	}
	else if(posChange.y > 0)
	{
		tMin.y = ((targetObject->pos.y + actualTargetBox.offset.y) - (pos.y + actualOwnBox.offset.y + actualOwnBox.height)) / posChange.y;
		tMax.y = ((targetObject->pos.y + actualTargetBox.offset.y + actualTargetBox.height) - (pos.y + actualOwnBox.offset.y)) / posChange.y;
	}

	//compare the timeframes
	if(tMax.y < tMin.x || tMax.x < tMin.y)
	{
		//no collision
		return false;
	}

	//collision
	return true;
}

void TerrainObject::IsCollidingWithObjectHurtBoxes(TerrainObject * targetObject)
{
	//loop through this object's attack boxes
	HSBox * selfAttackBox = ((TerrainObjectHold*)curHold)->firstAttackBox;
	TerrainObjectHold * targetHold = (TerrainObjectHold*)targetObject->curHold;
	HSBox * targetHurtBox;
	while(selfAttackBox != NULL)
	{
		//loop through the target's hurt boxes
		targetHurtBox = targetHold->firstHurtBox;
		while(targetHurtBox != NULL)
		{
			//check for collision
			if(IsAttackBoxColliding(selfAttackBox, targetObject, targetHurtBox))
			{
				//we only need one collision with a single box
				HandleAttackCollision(targetObject);
				return;
			}

			targetHurtBox = targetHurtBox->nextBox;
		}
		selfAttackBox = selfAttackBox->nextBox;
	}
}

void TerrainObject::IsCollidingWithAnyHurtBoxes(list<HSObject*> * gameObjects)
{
	list<HSObject*>::iterator objIt;
	for ( objIt=gameObjects->begin(); objIt != gameObjects->end(); objIt++)
	{
		//only check if the target isn't this object, if the target is at least a terrain object, if it collides with objects, and if the target has hurt boxes
		if((*objIt)->id != id && (*objIt)->IsTerrainObject() &&
			((TerrainObject*)*objIt)->collides && !(!((TerrainObject*)*objIt)->collideParent && (*objIt)->parent == this) && !(!collideParent && parent == (*objIt)) &&
			((TerrainObjectHold*)(*objIt)->curHold)->firstHurtBox != NULL)
		{
			//check to see if this object didn't already strike this target
			list<int>::iterator vicIt;
			bool alreadyVictim = false;
			for(vicIt=victims.begin(); vicIt != victims.end(); vicIt++)
			{
				if((*vicIt) == (*objIt)->id) { alreadyVictim = true; break; }
			}
			if(alreadyVictim) { continue; }
			IsCollidingWithObjectHurtBoxes((TerrainObject*)*objIt);
		}
	}
}

int TerrainObject::CollideAttack(list<HSObject*> * gameObjects)
{
	//check for collision with the hurt boxes of other objects, if this object's current hold has attack boxes
	if(collides && ((TerrainObjectHold*)curHold)->firstAttackBox != NULL)
	{
		IsCollidingWithAnyHurtBoxes(gameObjects);
	}

	return 0;
}

void TerrainObject::ApplyAttackResults()
{
	if(attackResults.timesStruck <= 0)
	{
		if(attackResults.didStrike)
		{
			curHitstop = attackResults.hitstop;
		}

		ResetAttackResults();
		return;
	}

	if(health > 0)
	{
		curHealth -= attackResults.damage;
	
		if(curHealth <= 0)
		{
			curHealth = 0;
			toDelete = true;
		}
		else if(curHealth > health) { curHealth = health; }
	}

	ResetAttackResults();
}