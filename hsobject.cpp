#include "hsobject.h"

/////////
//HSBox//
/////////

int DefineBox(XMLElement * definition, HSBox * newBox)
{
	if(definition->QueryFloatAttribute("height", &(newBox->height)) != XML_NO_ERROR)
	{
		UpdateLog("No height defined on new hitbox.", true);
		return -1; //no height defined
	}
	if(definition->QueryFloatAttribute("width", &(newBox->width)) != XML_NO_ERROR)
	{
		UpdateLog("No width defined on new hitbox.", true);
		return -1; //no width defined
	}

	newBox->offset.x = 0;
	definition->QueryFloatAttribute("offsetX", &(newBox->offset.x));

	newBox->offset.y = 0;
	definition->QueryFloatAttribute("offsetY", &(newBox->offset.y));

	/*newBox->isTriangle = false;
	definition->QueryBoolAttribute("isTriangle", &(newBox->isTriangle));
					
	newBox->rightAlign = false;
	newBox->bottomAlign = false;
	if(newBox->isTriangle)
	{
		definition->QueryBoolAttribute("rightAlign", &(newBox->rightAlign));

		definition->QueryBoolAttribute("bottomAlign", &(newBox->bottomAlign));
	}*/

	return 0;
}

////////////////
//HSObjectHold//
////////////////

HSObjectHold::HSObjectHold()
{
	id = 0;
	nextHoldId = 0;
	textures.clear();
	audioList.clear();
	spawnObjects.clear();
	nextHold = NULL;
	nextListHold = NULL;
	reposition.x = 0;
	reposition.y = 0;
	overwriteVelocity = false;
	velocity.x = 0;
	velocity.y = 0;

	duration = 0;
}

HSObjectHold::~HSObjectHold()
{
	list<SpawnObject>::iterator spawnItr;
	for(spawnItr = spawnObjects.begin(); spawnItr != spawnObjects.end(); spawnItr++)
	{
		delete spawnItr->object;
	}
}

bool HSObjectHold::IsTerrainObjectHold()
{
	return false;
}

bool HSObjectHold::IsPhysicsObjectHold()
{
	return false;
}

bool HSObjectHold::IsFighterHold()
{
	return false;
}

////////////
//HSObject//
////////////

HSObject::HSObject()
{
	id = 0;
	//player = -1;
	palettes.clear();
	palette = NULL;
	useTGAPalettes = true;
	firstHold = NULL;
	lastHold = NULL;
	curHold = NULL;
	parent = NULL;
	followParent = false;
	hFlip = false;
	visible = true;
	lifetime = 0;
	time = 0;
	holdTime = 0;
	toDelete = false;
	pos.x = 0;
	pos.y = 0;
	prevPos.x = 0;
	prevPos.y = 0;
	vel.x = 0;
	vel.y = 0;
	prevVel.x = 0;
	prevVel.y = 0;
	depth = 0;
	hsObjectEventHolds.lifetimeDeath = NULL;
	reposition.x;
	reposition.y;
	overwriteVelocity = false;
	holdVelocity.x = 0;
	holdVelocity.y = 0;

	objectsSpawned = false;
	audioPlayed = false;
}

HSObject::~HSObject()
{
	HSObjectHold * curHold = firstHold;
	HSObjectHold * nextHold;

	while(curHold != NULL)
	{
		nextHold = curHold->nextListHold;
		delete curHold;
		curHold = nextHold;
	}
}

void HSObject::PrevPalette()
{
	if(useTGAPalettes || palettes.empty()) { return; }

	if(palette == NULL) { palette = palettes.front().hsPal; return; }

	HSPalette * curPal = NULL;
	list<PaletteInstance>::iterator palIt;
	for ( palIt=palettes.begin(); palIt != palettes.end(); palIt++)
	{
		if(palette == palIt->hsPal)
		{
			if(curPal == NULL)
			{
				//the current palette is at the start of the list. go to the last one
				palette->usingCount--;
				palette = palettes.back().hsPal;
				palette->usingCount++;
				return;
			}

			palette = curPal;
			return;
		}

		curPal = palIt->hsPal;
	}
}

void HSObject::NextPalette()
{
	if(useTGAPalettes || palettes.empty()) { return; }

	if(palette == NULL) { palette = palettes.front().hsPal; return; }

	//switch to the palette following the current one
	bool curPalFound = false;
	bool nextPalSet = false;
	list<PaletteInstance>::iterator palIt;
	for ( palIt=palettes.begin(); palIt != palettes.end(); palIt++)
	{
		if(!curPalFound && palette->paletteFilePath.compare(palIt->hsPal->paletteFilePath) == 0)
		{
			curPalFound = true;
		}
		else if(curPalFound)
		{
			palette->usingCount--;
			palette = palIt->hsPal;
			palette->usingCount++;
			nextPalSet = true;
			break;
		}
	}

	if(!nextPalSet)
	{
		//the current palette must have been at the end of the list. go to the first one
		palette = palettes.front().hsPal;
	}
}

void HSObject::SetPalette(int index)
{
	if(useTGAPalettes || palettes.empty()) { return; }

	if(index < 0) { index = 0; }
	if(index >= palettes.size()) { index = palettes.size() - 1; }

	int cur = 0;
	list<PaletteInstance>::iterator palIt;
	for ( palIt=palettes.begin(); palIt != palettes.end(); palIt++)
	{
		if(cur == index)
		{
			if(palette != NULL) { palette->usingCount--; }
			palette = palIt->hsPal;
			palette->usingCount++;
			return;
		}

		cur++;
	}
}

int HSObject::GetPalette()
{
	if(useTGAPalettes || palettes.empty()) { return -1; }

	int cur = 0;
	list<PaletteInstance>::iterator palIt;
	for ( palIt=palettes.begin(); palIt != palettes.end(); palIt++)
	{
		if(palette == palIt->hsPal) { return cur; }
		cur++;
	}

	return -1;
}

int HSObject::AdvanceHolds()
{
	objectsSpawned = false;
	audioPlayed = false;

	//handle holds
	if(curHold->duration > 0)
	{
		holdTime++;
		if(holdTime >= curHold->duration)
		{
			AdvanceHold(curHold->nextHold);
		}
	}

	return 0;
}

int HSObject::Event(InputStates * inputHistory, int frame)
{
	return 0;
}

int HSObject::Update()
{
	//save the current position
	prevPos.x = pos.x;
	prevPos.y = pos.y;

	//save the current velocity
	prevVel.x = vel.x;
	prevVel.y = vel.y;

	//reposition from the hold
	pos.x += reposition.x;
	pos.y += reposition.y;

	reposition.x = 0;
	reposition.y = 0;

	//move according to parent, if applicable
	if(parent != NULL && followParent)
	{
		pos.x += parent->pos.x - parent->prevPos.x;
		pos.y += parent->pos.y - parent->prevPos.y;
	}

	//overwrite velocity
	if(overwriteVelocity)
	{
		vel.x = holdVelocity.x;
		vel.y = holdVelocity.y;
	}

	overwriteVelocity = false;
	holdVelocity.x = 0;
	holdVelocity.y = 0;

	//move according to velocity
	pos.x += vel.x;
	pos.y += vel.y;

	//handle lifetime
	if(lifetime > 0)
	{
		time++;
		if(time >= lifetime)
		{
			toDelete = true;
		}
	}

	return 0;
}

bool HSObject::AdvanceHold(HSObjectHold* hold)
{
	bool result = true;
	holdTime = 0;
	curHold = hold;
	if(curHold == NULL)
	{
		curHold = GetDefaultHold();
		result = false;
	}

	reposition = curHold->reposition;
	overwriteVelocity = curHold->overwriteVelocity;
	holdVelocity = curHold->velocity;

	return result;
}

bool HSObject::ChangeHold(HSObjectHold* hold)
{
	return AdvanceHold(hold);
}

HSObjectHold * HSObject::GetDefaultHold()
{
	return firstHold;
}

int HSObject::CollideTerrain(list<HSObject*> * gameObjects)
{
	return 0;
}

int HSObject::CollideAttack(list<HSObject*> * gameObjects)
{
	return 0;
}

void HSObject::ApplyAttackResults()
{
	return;
}

void HSObject::HandleHurtCollision(HSObject * attacker)
{
	return;
}

list<SpawnObject*> HSObject::GetSpawnObjects()
{
	list<SpawnObject*> spawnObjects;
	spawnObjects.clear();

	if(objectsSpawned) { return spawnObjects; }

	list<SpawnObject>::iterator spawnIt;
	for ( spawnIt=curHold->spawnObjects.begin(); spawnIt != curHold->spawnObjects.end(); spawnIt++)
	{
		if(holdTime == (*spawnIt).delay)
		{
			for(int i = 0; i < (*spawnIt).number; i++)
			{
				spawnObjects.push_back(&(*spawnIt));
			}
		}
	}

	objectsSpawned = true;

	return spawnObjects;
}

list<AudioInstance*> HSObject::GetAudio()
{
	list<AudioInstance*> audio;
	audio.clear();

	if(audioPlayed) { return audio; }

	list<AudioInstance*> chanceAudio;
	chanceAudio.clear();

	list<AudioInstance>::iterator audIt;
	for ( audIt=curHold->audioList.begin(); audIt != curHold->audioList.end(); audIt++)
	{
		if(holdTime == (*audIt).delay)
		{
			if((*audIt).usePercentage)
			{
				chanceAudio.push_back(&(*audIt));
			}
			else
			{
				audio.push_back(&(*audIt));
			}
		}
	}

	if(chanceAudio.size() > 0)
	{
		int maxRand = 0;
		int cur = 0;

		list<AudioInstance*>::iterator chanceAudIt;
		for ( chanceAudIt=chanceAudio.begin(); chanceAudIt != chanceAudio.end(); chanceAudIt++)
		{
			maxRand += (*chanceAudIt)->percentage;
		}

		if(maxRand < 100) { maxRand = 100; }

		int random = rand() % maxRand + 1;
		
		for ( chanceAudIt=chanceAudio.begin(); chanceAudIt != chanceAudio.end(); chanceAudIt++)
		{
			if(random <= cur + (*chanceAudIt)->percentage)
			{
				audio.push_back((*chanceAudIt));
				break;
			}

			cur += (*chanceAudIt)->percentage;
		}
	}

	audioPlayed = true;

	return audio;
}

bool HSObject::IsHUD()
{
	return false;
}

bool HSObject::IsTerrain()
{
	return false;
}

bool HSObject::IsTerrainObject()
{
	return false;
}

bool HSObject::IsPhysicsObject()
{
	return false;
}

bool HSObject::IsFighter()
{
	return false;
}