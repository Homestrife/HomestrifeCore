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
	blockability = MID;
	horizontalDirectionBasedBlock = false;
	reversedHorizontalBlock = false;
	damage = 0;
	ownHitstop = 0;
	victimHitstop = 0;
	hitstun = 0;
	blockstun = 0;
	force.x = 0;
	force.y = 0;
	trips = false;
	resetHits = false;
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

int TerrainObjectHold::Define(XMLElement * definition, string defFileDirectory, list<HSTexture*> * textureRegistry, list<HSAudio*> * audioRegistry, SDL_AudioSpec * obtainedAudioSpec)
{
	if(int error = HSObjectHold::Define(definition, defFileDirectory, textureRegistry, audioRegistry, obtainedAudioSpec) != 0)
	{
		return error;
	}

	//get changeAttackBoxAttributes
	const char * caba = definition->Attribute("changeAttackBoxAttributes");
    string bString;
	if(caba != NULL)
	{
		bString.assign(caba);
		if(bString.compare("true") == 0) { changeAttackBoxAttributes = true; }
		else if(bString.compare("false") == 0) { changeAttackBoxAttributes = false; }
	}

	//get blockability
	const char * b = definition->Attribute("blockability");
	if(b != NULL)
	{
		bString.assign(b);
		if(bString.compare("UNBLOCKABLE") == 0) { blockability = UNBLOCKABLE; }
		else if(bString.compare("HIGH") == 0) { blockability = HIGH; }
		else if(bString.compare("LOW") == 0) { blockability = LOW; }
		else if(bString.compare("MID") == 0) { blockability = MID; }
	}

	//get horizontalDirectionBasedBlock
	const char * hdbb = definition->Attribute("horizontalDirectionBasedBlock");
	if(hdbb != NULL)
	{
		bString.assign(hdbb);
		if(bString.compare("true") == 0) { horizontalDirectionBasedBlock = true; }
		else if(bString.compare("false") == 0) { horizontalDirectionBasedBlock = false; }
	}

	//get reversedHorizontalBlock
	const char * rhb = definition->Attribute("reversedHorizontalBlock");
	if(rhb != NULL)
	{
		bString.assign(rhb);
		if(bString.compare("true") == 0) { reversedHorizontalBlock = true; }
		else if(bString.compare("false") == 0) { reversedHorizontalBlock = false; }
	}

	//get damage
	definition->QueryIntAttribute("damage", &damage);

	//get hitstop
	definition->QueryIntAttribute("ownHitstop", &ownHitstop);
	definition->QueryIntAttribute("victimHitstop", &victimHitstop);

	//get hitstun
	definition->QueryIntAttribute("hitstun", &hitstun);

	//get blockstun
	definition->QueryIntAttribute("blockstun", &blockstun);

	//get force
	definition->QueryFloatAttribute("forceX", &force.x);
	definition->QueryFloatAttribute("forceY", &force.y);

	//get trips
	const char * t = definition->Attribute("trips");
	if(t != NULL)
	{
		bString.assign(t);
		if(bString.compare("true") == 0) { trips = true; }
		else if(bString.compare("false") == 0) { trips = false; }
	}

	//get resetHits
	const char * r = definition->Attribute("resetHits");
	if(r != NULL)
	{
		bString.assign(r);
		if(bString.compare("true") == 0) { resetHits = true; }
		else if(bString.compare("false") == 0) { resetHits = false; }
	}

	//get the attack boxes
	XMLElement * attackBoxes;
	if((attackBoxes = definition->FirstChildElement("AttackBoxes")) != NULL)
	{
		for(XMLElement * j = attackBoxes->FirstChildElement(); j != NULL; j = j->NextSiblingElement())
		{
			//check the element name
			if(strcmp(j->Value(), "Box") != 0)
			{
				continue; //there's a mal-formed element in the hit box list
			}

			HSBox * newAttackBox = new HSBox();
			newAttackBox->nextBox = NULL;
					
			//add the hit box to the object
			if(int error = DefineAttackBox(newAttackBox, j) != 0)
			{
				return error; //there was an error adding the hit box
			}
		}
	}

	//get the hurt boxes
	XMLElement * hurtBoxes;
	if((hurtBoxes = definition->FirstChildElement("HurtBoxes")) != NULL)
	{
		for(XMLElement * j = hurtBoxes->FirstChildElement(); j != NULL; j = j->NextSiblingElement())
		{
			//check the element name
			if(strcmp(j->Value(), "Box") != 0)
			{
				continue; //there's a mal-formed element in the hurt box list
			}

			HSBox * newHurtBox = new HSBox();
			newHurtBox->nextBox = NULL;
					
			//add the hurt box to the object
			if(int error = DefineHurtBox(newHurtBox, j) != 0)
			{
				return error; //there was an error adding the hurt box
			}
		}
	}

	//loop through the hold's hit audio
	XMLElement * hitAudioList;
	if((hitAudioList = definition->FirstChildElement("HitAudioList")) != NULL)
	{
		for(XMLElement * k = hitAudioList->FirstChildElement(); k != NULL; k = k->NextSiblingElement())
		{
			//check the element name
			if(strcmp(k->Value(), "HitAudio") == 0)
			{
				//add the audio to the hold
				if(int error = AddHitAudio(k, defFileDirectory, audioRegistry, obtainedAudioSpec) != 0)
				{
					return error; //there was an error adding the audio
				}
			}
		}
	}

	//loop through the hold's blocked audio
	XMLElement * blockedAudioList;
	if((blockedAudioList = definition->FirstChildElement("BlockedAudioList")) != NULL)
	{
		for(XMLElement * k = blockedAudioList->FirstChildElement(); k != NULL; k = k->NextSiblingElement())
		{
			//check the element name
			if(strcmp(k->Value(), "BlockedAudio") == 0)
			{
				//add the audio to the hold
				if(int error = AddBlockedAudio(k, defFileDirectory, audioRegistry, obtainedAudioSpec) != 0)
				{
					return error; //there was an error adding the audio
				}
			}
		}
	}

	return 0;
}

bool TerrainObjectHold::IsTerrainObjectHold()
{
	return true;
}

int TerrainObjectHold::DefineAttackBox(HSBox * newAttackBox, XMLElement * definition)
{
	if(int error = DefineBox(definition, newAttackBox) != 0)
	{
		return error; //there was an error defining the box
	}

	AddAttackBox(newAttackBox);

	return 0;
}

void TerrainObjectHold::AddAttackBox(HSBox * newAttackBox)
{
	if(firstAttackBox == NULL)
	{
		//just make the new hit box the first hit box
		firstAttackBox = newAttackBox;
		lastAttackBox = newAttackBox;
	}
	else
	{
		//put the new hit box at the end of the list
		lastAttackBox->nextBox = newAttackBox;
		lastAttackBox = newAttackBox;
	}
}

int TerrainObjectHold::DefineHurtBox(HSBox * newHurtBox, XMLElement * definition)
{
	if(int error = DefineBox(definition, newHurtBox) != 0)
	{
		return error; //there was an error defining the box
	}

	AddHurtBox(newHurtBox);

	return 0;
}

void TerrainObjectHold::AddHurtBox(HSBox * newHurtBox)
{
	if(firstHurtBox == NULL)
	{
		//just make the new hurt box the first hurt box
		firstHurtBox = newHurtBox;
		lastHurtBox = newHurtBox;
	}
	else
	{
		//put the new hurt box at the end of the list
		lastHurtBox->nextBox = newHurtBox;
		lastHurtBox = newHurtBox;
	}
}

int TerrainObjectHold::AddHitAudio(XMLElement * audio, string defFileDirectory, list<HSAudio*> * audioRegistry, SDL_AudioSpec * obtainedAudioSpec)
{
	//get the file path
	string audioFilePath = CreateAbsolutePath(defFileDirectory, audio->Attribute("hitAudioFilePath"));

	AudioInstance newAudInst;

	//see if the audio has already been loaded
	list<HSAudio*>::iterator arIt;
	bool audNotLoaded = true;
	for ( arIt=(*audioRegistry).begin(); arIt != (*audioRegistry).end(); arIt++)
	{
		if((*arIt)->audioFilePath.compare(audioFilePath) == 0)
		{
			//audio has been loaded
			newAudInst.hsAud = (*arIt);
			(*arIt)->usingCount++;
			audNotLoaded = false;
			break;
		}
	}

	if(audNotLoaded)
	{
		//audio hasn't been loaded before, so load it
		HSAudio * newAud = new HSAudio();
		newAud->usingCount = 1;
		newAud->audioFilePath = audioFilePath;

		if(int error = LoadWAVToAudio(newAud, obtainedAudioSpec) != 0) //load the audio
		{
			return error;
		}

		newAudInst.hsAud = newAud;
		audioRegistry->push_back(newAud);
	}
	
	newAudInst.delay = 0;
	audio->QueryIntAttribute("delay", &(newAudInst.delay));
	audio->QueryIntAttribute("percentage", &(newAudInst.percentage));
	audio->QueryBoolAttribute("exclusive", &(newAudInst.exclusive));
	audio->QueryBoolAttribute("usePercentage", &(newAudInst.usePercentage));
	hitAudioList.push_back(newAudInst);

	return 0;
}

int TerrainObjectHold::AddBlockedAudio(XMLElement * audio, string defFileDirectory, list<HSAudio*> * audioRegistry, SDL_AudioSpec * obtainedAudioSpec)
{
	//get the file path
	string audioFilePath = CreateAbsolutePath(defFileDirectory, audio->Attribute("blockedAudioFilePath"));

	AudioInstance newAudInst;

	//see if the audio has already been loaded
	list<HSAudio*>::iterator arIt;
	bool audNotLoaded = true;
	for ( arIt=(*audioRegistry).begin(); arIt != (*audioRegistry).end(); arIt++)
	{
		if((*arIt)->audioFilePath.compare(audioFilePath) == 0)
		{
			//audio has been loaded
			newAudInst.hsAud = (*arIt);
			(*arIt)->usingCount++;
			audNotLoaded = false;
			break;
		}
	}

	if(audNotLoaded)
	{
		//audio hasn't been loaded before, so load it
		HSAudio * newAud = new HSAudio();
		newAud->usingCount = 1;
		newAud->audioFilePath = audioFilePath;

		if(int error = LoadWAVToAudio(newAud, obtainedAudioSpec) != 0) //load the audio
		{
			return error;
		}

		newAudInst.hsAud = newAud;
		audioRegistry->push_back(newAud);
	}
	
	newAudInst.delay = 0;
	audio->QueryIntAttribute("delay", &(newAudInst.delay));
	audio->QueryIntAttribute("percentage", &(newAudInst.percentage));
	audio->QueryBoolAttribute("exclusive", &(newAudInst.exclusive));
	audio->QueryBoolAttribute("usePercentage", &(newAudInst.usePercentage));
	blockedAudioList.push_back(newAudInst);

	return 0;
}

TerrainObjectHold * TerrainObjectHold::CreateHoldOfSameType()
{
	return new TerrainObjectHold();
}

void TerrainObjectHold::CopyAttributes(HSObjectHold * target)
{
	HSObjectHold::CopyAttributes(target);

	if(!target->IsTerrainObjectHold()) { return; }

	((TerrainObjectHold*)target)->blockability = blockability;
	((TerrainObjectHold*)target)->ownHitstop = ownHitstop;
	((TerrainObjectHold*)target)->victimHitstop = victimHitstop;
	((TerrainObjectHold*)target)->blockstun = blockstun;
	((TerrainObjectHold*)target)->changeAttackBoxAttributes = changeAttackBoxAttributes;
	((TerrainObjectHold*)target)->damage = damage;
	
	HSBox * curBox = firstAttackBox;
	while(curBox != NULL)
	{
		HSBox * newBox = new HSBox();
		newBox->bottomAlign = curBox->bottomAlign;
		newBox->height = curBox->height;
		newBox->isTriangle = curBox->isTriangle;
		newBox->offset.x = curBox->offset.x;
		newBox->offset.y = curBox->offset.y;
		newBox->rightAlign = curBox->rightAlign;
		newBox->width = curBox->width;

		((TerrainObjectHold*)target)->AddAttackBox(newBox);

		curBox = curBox->nextBox;
	}
	
	curBox = firstHurtBox;
	while(curBox != NULL)
	{
		HSBox * newBox = new HSBox();
		newBox->bottomAlign = curBox->bottomAlign;
		newBox->height = curBox->height;
		newBox->isTriangle = curBox->isTriangle;
		newBox->offset.x = curBox->offset.x;
		newBox->offset.y = curBox->offset.y;
		newBox->rightAlign = curBox->rightAlign;
		newBox->width = curBox->width;

		((TerrainObjectHold*)target)->AddHurtBox(newBox);

		curBox = curBox->nextBox;
	}

	((TerrainObjectHold*)target)->force.x = force.x;
	((TerrainObjectHold*)target)->force.y = force.y;
	
	list<AudioInstance>::iterator audIt;
	for(audIt = hitAudioList.begin(); audIt != hitAudioList.end(); audIt++)
	{
		AudioInstance newInstance;
		newInstance.delay = audIt->delay;
		newInstance.exclusive = audIt->exclusive;
		newInstance.hsAud = audIt->hsAud;
		newInstance.percentage = audIt->percentage;
		newInstance.usePercentage = audIt->usePercentage;

		((TerrainObjectHold*)target)->hitAudioList.push_back(newInstance);
	}

	((TerrainObjectHold*)target)->hitstun = hitstun;
	((TerrainObjectHold*)target)->horizontalDirectionBasedBlock = horizontalDirectionBasedBlock;
	((TerrainObjectHold*)target)->reversedHorizontalBlock = reversedHorizontalBlock;
	((TerrainObjectHold*)target)->trips = trips;
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

int TerrainObject::Define(XMLElement * definition, string defFileDirectory, list<HSTexture*> * textureRegistry, list<HSPalette*> * paletteRegistry, list<HSAudio*> * audioRegistry, SDL_AudioSpec * obtainedAudioSpec)
{
	if(int error = HSObject::Define(definition, defFileDirectory, textureRegistry, paletteRegistry, audioRegistry, obtainedAudioSpec) != 0)
	{
		return error;
	}

	//get the terrain boxes
	XMLElement * terrainBox;
	if((terrainBox = definition->FirstChildElement("TerrainBox")) != NULL)
	{
		HSBox * newTerrainBox = new HSBox();
		newTerrainBox->nextBox = NULL;
					
		//add the terrain box to the object
		if(int error = DefineTerrainBox(newTerrainBox, terrainBox) != 0)
		{
			return error; //there was an error adding the terrain box
		}
	}
	
	const char * cbjt = definition->Attribute("canBeJumpedThrough");
	string b;
	if(cbjt != NULL)
	{
		b.assign(cbjt);
		if(b.compare("true") == 0) { canBeJumpedThrough = true; }
		else if(b.compare("false") == 0) { canBeJumpedThrough = false; }
	}

	definition->QueryIntAttribute("health", &(health));

	definition->QueryFloatAttribute("bounce", &(bounce));
	if(bounce < 0) { bounce = 0; }
	if(bounce > 1) { bounce = 1; }

	definition->QueryFloatAttribute("friction", &(friction));
	if(friction < 0) { friction = 0; }
	if(friction > 1) { friction = 1; }
			
	const char * ttd = definition->Attribute("takesTerrainDamage");
	if(ttd != NULL)
	{
		b.assign(ttd);
		if(b.compare("true") == 0) { takesTerrainDamage = true; }
		else if(b.compare("false") == 0) { takesTerrainDamage = false; }
	}
			
	const char * f = definition->Attribute("fragile");
	if(f != NULL)
	{
		b.assign(f);
		if(b.compare("true") == 0) { fragile = true; }
		else if(b.compare("false") == 0) { fragile = false; }
	}

	return 0;
}

int TerrainObject::Event(InputStates * inputHistory, int frame)
{
	return 0;
}

int TerrainObject::Update()
{
	return HSObject::Update();
}

bool TerrainObject::ChangeHold(HSObjectHold* hold)
{
	if(HSObject::ChangeHold(hold))
	{
		//we can be confident that this is actually a TerrainObjectHold
		TerrainObjectHold * tHold = (TerrainObjectHold*)curHold;

		if(tHold->changeAttackBoxAttributes)
		{
			//get the new statistics
			hitAudioList = tHold->hitAudioList;
			blockedAudioList = tHold->blockedAudioList;
			blockability = tHold->blockability;
			horizontalDirectionBasedBlock = tHold->horizontalDirectionBasedBlock;
			reversedHorizontalBlock = tHold->reversedHorizontalBlock;
			damage = tHold->damage;
			ownHitstop = tHold->ownHitstop;
			victimHitstop = tHold->victimHitstop;
			hitstun = tHold->hitstun;
			blockstun = tHold->blockstun;
			force.x = tHold->force.x;
			force.y = tHold->force.y;
			trips = tHold->trips;

			//reset victims
			if(tHold->resetHits) { victims.clear(); }
		}

		return true;
	}
	
	return false;
}

HSObjectHold * TerrainObject::GetDefaultHold()
{
	victims.clear();
	hitAudioList.clear();
	blockability = MID;
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

TerrainObjectHold * TerrainObject::CreateNewHold()
{
	TerrainObjectHold * newHold = new TerrainObjectHold();

	return newHold;
}

TerrainObject * TerrainObject::CreateObjectOfSameType()
{
	return new TerrainObject();
}

void TerrainObject::CopyAttributes(HSObject * target)
{
	HSObject::CopyAttributes(target);

	if(!target->IsTerrainObject()) { return; }

	((TerrainObject*)target)->bounce = bounce;
	((TerrainObject*)target)->canBeJumpedThrough = canBeJumpedThrough;
	((TerrainObject*)target)->collides = collides;

	HSBox * curBox = firstTerrainBox;
	while(curBox != NULL)
	{
		HSBox * newBox = new HSBox();
		newBox->bottomAlign = curBox->bottomAlign;
		newBox->height = curBox->height;
		newBox->isTriangle = curBox->isTriangle;
		newBox->offset.x = curBox->offset.x;
		newBox->offset.y = curBox->offset.y;
		newBox->rightAlign = curBox->rightAlign;
		newBox->width = curBox->width;

		((TerrainObject*)target)->AddTerrainBox(newBox);

		curBox = curBox->nextBox;
	}

	((TerrainObject*)target)->friction = friction;
	((TerrainObject*)target)->health = health;
	
	list<AudioInstance>::iterator audIt;
	for(audIt = hitAudioList.begin(); audIt != hitAudioList.end(); audIt++)
	{
		AudioInstance newInstance;
		newInstance.delay = audIt->delay;
		newInstance.exclusive = audIt->exclusive;
		newInstance.hsAud = audIt->hsAud;
		newInstance.percentage = audIt->percentage;
		newInstance.usePercentage = audIt->usePercentage;

		((TerrainObject*)target)->hitAudioList.push_back(newInstance);
	}

	((TerrainObject*)target)->takesTerrainDamage = takesTerrainDamage;
	((TerrainObject*)target)->fragile = fragile;
}

void TerrainObject::CopyEventHold(HSObject * target, HSObjectHold * targetHold)
{
	HSObject::CopyEventHold(target, targetHold);

	if(!target->IsTerrainObject() || !targetHold->IsTerrainObjectHold()) { return; }

	TerrainObject * to = (TerrainObject*)target;
	TerrainObjectHold * toHold = (TerrainObjectHold*)targetHold;

	if(terrainEventHolds.healthDeath != NULL && toHold->id == terrainEventHolds.healthDeath->id) { to->terrainEventHolds.healthDeath = toHold; }
}

int TerrainObject::SaveEventHolds(HSObjectHold * hold, XMLElement * eventHolds)
{
	if(int error = HSObject::SaveEventHolds(hold, eventHolds) != 0)
	{
		return error;
	}

	//check the attribute names, and put the hold pointers into the proper event hold slots.
	unsigned int eventHoldId;
	eventHolds->QueryUnsignedAttribute("healthDeath", &eventHoldId);
	if(hold->id == eventHoldId) { terrainEventHolds.healthDeath = (TerrainObjectHold*)hold; }

	return 0;
}

int TerrainObject::DefineTerrainBox(HSBox * newTerrainBox, XMLElement * definition)
{
	if(int error = DefineBox(definition, newTerrainBox) != 0)
	{
		return error; //there was an error defining the box
	}

	AddTerrainBox(newTerrainBox);

	return 0;
}

void TerrainObject::AddTerrainBox(HSBox * newTerrainBox)
{
	if(firstTerrainBox == NULL)
	{
		//just make the new terrain box the first terrain box
		firstTerrainBox = newTerrainBox;
		lastTerrainBox = newTerrainBox;
	}
	else
	{
		//put the new terrain box at the end of the list
		lastTerrainBox->nextBox = newTerrainBox;
		lastTerrainBox = newTerrainBox;
	}
}

bool TerrainObject::AreRectanglesColliding(HSVect2D * boxOnePos, HSBox * boxOne, HSVect2D * boxTwoPos, HSBox * boxTwo)
{
	if(boxOnePos == NULL || boxOne == NULL || boxTwoPos == NULL || boxTwo == NULL) { return false; }

	if(boxOnePos->x + boxOne->offset.x < boxTwoPos->x + boxTwo->offset.x + boxTwo->width &&
		boxOnePos->x + boxOne->offset.x + boxOne->width > boxTwoPos->x + boxTwo->offset.x &&
		boxOnePos->y + boxOne->offset.y < boxTwoPos->y + boxTwo->offset.y + boxTwo->height &&
		boxOnePos->y + boxOne->offset.y + boxOne->height > boxTwoPos->y + boxTwo->offset.y)
	{
		return true;
	}

	return false;
}

void TerrainObject::ResetAttackResults()
{
	//reset all of the attack results to zero or null
	attackResults.struck = false;
	attackResults.didStrike = false;
	attackResults.hFlip = false;
	attackResults.damage = 0;
	attackResults.hitstop = 0;
	attackResults.hitstun = 0;
	attackResults.blockstun = 0;
	attackResults.force.x = 0;
	attackResults.force.y = 0;
	attackResults.blockability = MID;
}

void TerrainObject::HandleHurtCollision(TerrainObject * attacker)
{
	attackResults.struck = true;
	attackResults.damage += attacker->damage;
	attackResults.hitstop = attacker->victimHitstop;
	attacker->numHitThisFrame++;
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
	targetObject->HandleHurtCollision(this);

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
	if(!attackResults.struck)
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