#include "hsobject.h"

list<string> GetStringParts(list<string> parts, string toSplit, string splitToken)
{
	if(toSplit.empty()) { return parts; }

	string part = toSplit.substr(0, toSplit.find_first_of(splitToken));
	parts.push_back(part);

	if(part.compare(toSplit) == 0) { return parts; }

	string remaining = toSplit.substr(toSplit.find_first_of(splitToken) + splitToken.length(), toSplit.length());
	parts = GetStringParts(parts, remaining, splitToken);

	return parts;
}

list<string> SplitString(string toSplit, string splitToken)
{
	list<string> parts;
	parts.clear();

	if(splitToken.empty())
	{
		parts.push_back(toSplit);
		return parts;
	}

	parts = GetStringParts(parts, toSplit, splitToken);

	return parts;
}

string CreateAbsolutePath(string baseDirectory, string relPath)
{
	list<string> baseDirectoryParts = SplitString(baseDirectory, "\\");
	list<string> relPathParts = SplitString(relPath, "\\");

	//get the number of double periods
	int doublePeriods = 0;
	list<string>::iterator stIt;
	for ( stIt=relPathParts.begin(); stIt != relPathParts.end(); stIt++)
	{
		if((*stIt).compare("..") == 0)
		{
			doublePeriods++;
		}
		else
		{
			break;
		}
	}

	//get base directory parts up to the double periods
	string absolutePath = "";
	int cur = 0;
	int max = baseDirectoryParts.size() - doublePeriods;
	for ( stIt=baseDirectoryParts.begin(); stIt != baseDirectoryParts.end(); stIt++)
	{
		if(cur > 0)
		{
			absolutePath += "\\";
		}
		absolutePath += (*stIt);
		cur++;
		if(cur >= max)
		{
			break;
		}
	}

	//get the relative path parts past the double periods
	cur = 0;
	max = relPathParts.size();
	for ( stIt=relPathParts.begin(); stIt != relPathParts.end(); stIt++)
	{
		if(cur >= doublePeriods)
		{
			absolutePath += "\\" + (*stIt);
		}
		cur++;
	}

	return absolutePath;
}

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

int HSObjectHold::Define(XMLElement * definition, string defFileDirectory, list<HSTexture*> * textureRegistry, list<HSAudio*> * audioRegistry, SDL_AudioSpec * obtainedAudioSpec, bool openGL3)
{
	//get the hold's settings
	if(definition->QueryUnsignedAttribute("id", &(id)) != XML_NO_ERROR)
	{
		UpdateLog("No ID defined on new hold.", true);
		return -1; //no id defined
	}
	definition->QueryUnsignedAttribute("nextHoldId", &(nextHoldId)); //nextHold can be undefined
	definition->QueryUnsignedAttribute("duration", &(duration)); //duration can be undefined

	//loop through the hold's textures
	XMLElement * textures;
	if((textures = definition->FirstChildElement("Textures")) != NULL)
	{
		for(XMLElement * k = textures->FirstChildElement(); k != NULL; k = k->NextSiblingElement())
		{
			//check the element name
			if(strcmp(k->Value(), "Texture") == 0)
			{
				//add the texture to the hold
				if(int error = AddTexture(k, defFileDirectory, textureRegistry, openGL3) != 0)
				{
					return error; //there was an error adding the texture
				}
			}
		}
	}

	//loop through the hold's audio
	XMLElement * audioList;
	if((audioList = definition->FirstChildElement("AudioList")) != NULL)
	{
		for(XMLElement * k = audioList->FirstChildElement(); k != NULL; k = k->NextSiblingElement())
		{
			//check the element name
			if(strcmp(k->Value(), "Audio") == 0)
			{
				//add the audio to the hold
				if(int error = AddAudio(k, defFileDirectory, audioRegistry, obtainedAudioSpec) != 0)
				{
					return error; //there was an error adding the audio
				}
			}
		}
	}

	//loop through the hold's spawn objects
	XMLElement * spawnList;
	if((spawnList = definition->FirstChildElement("SpawnObjects")) != NULL)
	{
		for(XMLElement * k = spawnList->FirstChildElement(); k != NULL; k = k->NextSiblingElement())
		{
			//check the element name
			if(strcmp(k->Value(), "SpawnObject") == 0)
			{
				//add the object to the spawn object list
				if(int error = AddSpawnObject(k, defFileDirectory, textureRegistry, audioRegistry, obtainedAudioSpec) != 0)
				{
					return error;
				}
			}
		}
	}

	return 0;
}

HSObjectHold * HSObjectHold::Clone()
{
	HSObjectHold * clone = CreateHoldOfSameType();

	CopyAttributes(clone);

	return clone;
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

int HSObjectHold::AddTexture(XMLElement * texture, string defFileDirectory, list<HSTexture*> * textureRegistry, bool openGL3)
{
	//get the file path
	string textureFilePath = CreateAbsolutePath(defFileDirectory, texture->Attribute("textureFilePath"));

	TextureInstance newTexInst;

	//see if the texture has already been loaded
	list<HSTexture*>::iterator trIt;
	bool texNotLoaded = true;
	for ( trIt=(*textureRegistry).begin(); trIt != (*textureRegistry).end(); trIt++)
	{
		if((*trIt)->textureFilePath.compare(textureFilePath) == 0)
		{
			//texture has been loaded
			newTexInst.hsTex = (*trIt);
			(*trIt)->usingCount++;
			texNotLoaded = false;
			break;
		}
	}

	if(texNotLoaded)
	{
		//texture hasn't been loaded before, so load it
		HSTexture * newTex = new HSTexture();
		newTex->usingCount = 1;
		newTex->textureFilePath = textureFilePath;

		if(int error = LoadTGAToTexture(newTex, openGL3) != 0) //load the texture
		{
			return error;
		}

		newTexInst.hsTex = newTex;
		textureRegistry->push_back(newTex);
	}
	
	newTexInst.offset.x = 0;
	newTexInst.offset.y = 0;
	newTexInst.depth = 0;
	newTexInst.hScale = 1;
	newTexInst.vScale = 1;
	texture->QueryFloatAttribute("offsetX", &(newTexInst.offset.x));
	texture->QueryFloatAttribute("offsetY", &(newTexInst.offset.y));
	texture->QueryIntAttribute("depth", &(newTexInst.depth));
	textures.push_back(newTexInst);

	return 0;
}

int HSObjectHold::AddAudio(XMLElement * audio, string defFileDirectory, list<HSAudio*> * audioRegistry, SDL_AudioSpec * obtainedAudioSpec)
{
	//get the file path
	string audioFilePath = CreateAbsolutePath(defFileDirectory, audio->Attribute("audioFilePath"));

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
	audio->QueryBoolAttribute("exclusive", &(newAudInst.exclusive));
	audio->QueryBoolAttribute("usePercentage", &(newAudInst.usePercentage));
	audio->QueryIntAttribute("percentage", &(newAudInst.percentage));
	audioList.push_back(newAudInst);

	return 0;
}

int HSObjectHold::AddSpawnObject(XMLElement * spawnObject, string defFileDirectory, list<HSTexture*> * textureRegistry, list<HSAudio*> * audioRegistry, SDL_AudioSpec * obtainedAudioSpec)
{
	//get the file path
	string spawnObjectFilePath = CreateAbsolutePath(defFileDirectory, spawnObject->Attribute("definitionFilePath"));

	SpawnObject newObject;
	spawnObject->QueryIntAttribute("delay", &(newObject.delay));
	spawnObject->QueryIntAttribute("number", &(newObject.number));
	spawnObject->QueryBoolAttribute("followParent", &(newObject.followParent));
	spawnObject->QueryBoolAttribute("collideParent", &(newObject.collideParent));
	spawnObject->QueryFloatAttribute("parentOffsetX", &(newObject.parentOffset.x));
	spawnObject->QueryFloatAttribute("parentOffsetY", &(newObject.parentOffset.y));
	spawnObject->QueryFloatAttribute("velocityX", &(newObject.vel.x));
	spawnObject->QueryFloatAttribute("velocityY", &(newObject.vel.y));
	newObject.defFilePath = spawnObjectFilePath;

	//Main::LoadDefinition() will handle the actual loading of the object later
	newObject.object = NULL;

	spawnObjects.push_back(newObject);

	return 0;
}

HSObjectHold * HSObjectHold::CreateHoldOfSameType()
{
	return new HSObjectHold();
}

void HSObjectHold::CopyAttributes(HSObjectHold * target)
{
	list<AudioInstance>::iterator audIt;
	for(audIt = audioList.begin(); audIt != audioList.end(); audIt++)
	{
		AudioInstance newInstance;
		newInstance.delay = audIt->delay;
		newInstance.exclusive = audIt->exclusive;
		newInstance.hsAud = audIt->hsAud;
		newInstance.percentage = audIt->percentage;
		newInstance.usePercentage = audIt->usePercentage;

		target->audioList.push_back(newInstance);
	}

	target->duration = duration;
	target->id = id;
	target->nextHoldId = nextHoldId;
	
	list<SpawnObject>::iterator spawnIt;
	for(spawnIt = spawnObjects.begin(); spawnIt != spawnObjects.end(); spawnIt++)
	{
		SpawnObject newInstance;
		newInstance.defFilePath = spawnIt->defFilePath;
		newInstance.delay = spawnIt->delay;
		newInstance.followParent = spawnIt->followParent;
		newInstance.object = spawnIt->object->Clone();
		newInstance.parentOffset.x = spawnIt->parentOffset.x;
		newInstance.parentOffset.y = spawnIt->parentOffset.y;
		newInstance.vel.x = spawnIt->vel.x;
		newInstance.vel.y = spawnIt->vel.y;

		target->spawnObjects.push_back(newInstance);
	}

	list<TextureInstance>::iterator texIt;
	for(texIt = textures.begin(); texIt != textures.end(); texIt++)
	{
		TextureInstance newInstance;
		newInstance.depth = texIt->depth;
		newInstance.hScale = texIt->hScale;
		newInstance.hsTex = texIt->hsTex;
		newInstance.offset.x = texIt->offset.x;
		newInstance.offset.y = texIt->offset.y;
		newInstance.vScale = texIt->vScale;

		target->textures.push_back(newInstance);
	}
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
	hsObjectEventHolds.lifetimeDeath = NULL;

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

int HSObject::Define(XMLElement * definition, string defFileDirectory, list<HSTexture*> * textureRegistry, list<HSPalette*> * paletteRegistry, list<HSAudio*> * audioRegistry, SDL_AudioSpec * obtainedAudioSpec, bool openGL3)
{
	//get the HSObject's settings
	//get the lifetime
	definition->QueryUnsignedAttribute("lifetime", &(lifetime)); //lifetime can be undefined

	//get the specified palette
	//string palFilePath = "";
	//if(palette > 0)
	//{
	//	if(palette == 1)		{ palFilePath = definition->Attribute("palette1FilePath"); }
	//	else if(palette == 2)	{ palFilePath = definition->Attribute("palette2FilePath"); }
	//	else if(palette == 3)	{ palFilePath = definition->Attribute("palette3FilePath"); }
	//	else if(palette == 4)	{ palFilePath = definition->Attribute("palette4FilePath"); }
	//	else if(palette == 5)	{ palFilePath = definition->Attribute("palette5FilePath"); }
	//	else if(palette == 6)	{ palFilePath = definition->Attribute("palette6FilePath"); }
	//	else if(palette == 7)	{ palFilePath = definition->Attribute("palette7FilePath"); }
	//	else if(palette == 8)	{ palFilePath = definition->Attribute("palette8FilePath"); }
	//	else if(palette == 9)	{ palFilePath = definition->Attribute("palette9FilePath"); }
	//	else if(palette == 10)	{ palFilePath = definition->Attribute("palette10FilePath"); }

	//	//the file path assumes a starting point of the location of the definition. So add that on.
	//	if(!defFileDirectory.empty())
	//	{
	//		palFilePath = defFileDirectory + "\\" + palFilePath;
	//		
	//		//see if the palette has already been loaded
	//		bool alreadyLoaded = false;
	//		list<HSPalette*>::iterator plIt;
	//		for ( plIt=(*paletteRegistry).begin(); plIt != (*paletteRegistry).end(); plIt++)
	//		{
	//			if((*plIt)->paletteFilePath.compare(palFilePath) == 0)
	//			{
	//				this->palette = (*plIt);
	//				this->palette->usingCount++;
	//				alreadyLoaded = true;
	//			}
	//		}

	//		if(!alreadyLoaded)
	//		{
	//			//make a new palette
	//			HSPalette * newPal = new HSPalette();
	//			newPal->usingCount = 1;
	//			newPal->paletteFilePath = palFilePath;

	//			if(int error = LoadHSPToPalette(newPal) != 0) //load the texture
	//			{
	//				return error;
	//			}

	//			this->palette = newPal;
	//			paletteRegistry->push_back(newPal);
	//		}
	//	}
	//}

	//get every available palette for this object
	string palletesDirectory = defFileDirectory + "\\palettes\\*";
	HANDLE hFind = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA ffd;
	hFind = FindFirstFile(palletesDirectory.data(), &ffd);

	if(INVALID_HANDLE_VALUE != hFind)
	{
		do
		{
			//skip this if it's a directory
			if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) { continue; }

			string filename = ffd.cFileName;

			//skip this if it's not a .hsp
			if(filename.find(".hsp", 0) == string::npos) { continue; }

			useTGAPalettes = false;

			string palFilePath = defFileDirectory + "\\palettes\\" + filename;
			
			//see if the palette has already been loaded
			bool alreadyLoaded = false;
			list<HSPalette*>::iterator plIt;
			for ( plIt=(*paletteRegistry).begin(); plIt != (*paletteRegistry).end(); plIt++)
			{
				if((*plIt)->paletteFilePath.compare(palFilePath) == 0)
				{
					palettes.push_back((*plIt));
					alreadyLoaded = true;
					break;
				}
			}

			if(!alreadyLoaded)
			{
				//make a new palette
				HSPalette * newPal = new HSPalette();
				newPal->usingCount = 0;
				newPal->paletteFilePath = palFilePath;

				if(int error = LoadHSPToPalette(newPal) != 0) //load the texture
				{
					return error;
				}

				paletteRegistry->push_back(newPal);
				palettes.push_back(newPal);
			}
		}
		while(FindNextFile(hFind, &ffd) != 0);
	}
	
	SetPalette(0);

	//if(!useTGAPalettes)
	//{
	//	//get a palette that isn't used
	//	list<HSPalette*>::iterator plIt;
	//	for ( plIt=palettes.begin(); plIt != palettes.end(); plIt++)
	//	{
	//		if((*plIt)->usingCount <= 0)
	//		{
	//			palette = (*plIt);
	//			(*plIt)->usingCount++;
	//			break;
	//		}
	//	}

	//	//if all palettes are used, just get the first one
	//	if(palette == NULL && !palettes.empty())
	//	{
	//		palette = palettes.front();
	//		palette->usingCount++;
	//	}
	//}

	//loop through the object's holds
	XMLElement * holds;
	if((holds = definition->FirstChildElement("Holds")) == NULL)
	{
		UpdateLog("No holds defined on new object.", true);
		return -1; //no holds!
	}
	for(XMLElement * j = holds->FirstChildElement(); j != NULL; j = j->NextSiblingElement())
	{
		//check the element name
		if(strcmp(j->Value(), "Hold") != 0)
		{
			continue; //there's a mal-formed element in the hold list
		}

		//create a new hold
		HSObjectHold * newHold = CreateNewHold();
		newHold->nextHold = NULL;
		newHold->nextListHold = NULL;

		//execute the hold's local definition code
		if(int error = newHold->Define(j, defFileDirectory, textureRegistry, audioRegistry, obtainedAudioSpec, openGL3) != 0)
		{
			return error; //there was an error defining the hold
		}

		//save event hold pointers, if necessary
		XMLElement * eventHolds = definition->FirstChildElement("EventHolds");
		if((eventHolds = definition->FirstChildElement("EventHolds")) != NULL)
		{
			if(int error = SaveEventHolds(newHold, eventHolds) != 0)
			{
				return error; //there was an error defining the hold
			}
		}

		//add the new hold to the list of holds
		if(int error = AddHold(newHold) != 0)
		{
			return error; //there was an error adding the hold
		}
	}
	
	//link all the holds in this object
	if(int error = LinkHolds() != 0)
	{
		return error; //there was an error linking the holds
	}

	return 0;
}

HSObject * HSObject::Clone()
{
	HSObject * clone = CreateObjectOfSameType();

	CopyAttributes(clone);
	
	HSObjectHold * curHold = firstHold;
	while(curHold != NULL)
	{
		HSObjectHold * holdClone = curHold->Clone();
		CopyEventHold(clone, holdClone);

		clone->AddHold(holdClone);

		curHold = curHold->nextListHold;
	}

	clone->LinkHolds();

	return clone;
}

void HSObject::PrevPalette()
{
	if(useTGAPalettes || palettes.empty()) { return; }

	if(palette == NULL) { palette = palettes.front(); return; }

	HSPalette * curPal = NULL;
	list<HSPalette*>::iterator palIt;
	for ( palIt=palettes.begin(); palIt != palettes.end(); palIt++)
	{
		if(palette == (*palIt))
		{
			if(curPal == NULL)
			{
				//the current palette is at the start of the list. go to the last one
				palette->usingCount--;
				palette = palettes.back();
				palette->usingCount++;
				return;
			}

			palette = curPal;
			return;
		}

		curPal = *palIt;
	}
}

void HSObject::NextPalette()
{
	if(useTGAPalettes || palettes.empty()) { return; }

	if(palette == NULL) { palette = palettes.front(); return; }

	//switch to the palette following the current one
	bool curPalFound = false;
	bool nextPalSet = false;
	list<HSPalette*>::iterator palIt;
	for ( palIt=palettes.begin(); palIt != palettes.end(); palIt++)
	{
		if(!curPalFound && palette->paletteFilePath.compare((*palIt)->paletteFilePath) == 0)
		{
			curPalFound = true;
		}
		else if(curPalFound)
		{
			palette->usingCount--;
			palette = (*palIt);
			palette->usingCount++;
			nextPalSet = true;
			break;
		}
	}

	if(!nextPalSet)
	{
		//the current palette must have been at the end of the list. go to the first one
		palette = palettes.front();
	}
}

void HSObject::SetPalette(int index)
{
	if(useTGAPalettes || palettes.empty()) { return; }

	if(index < 0) { index = 0; }
	if(index >= palettes.size()) { index = palettes.size() - 1; }

	int cur = 0;
	list<HSPalette*>::iterator palIt;
	for ( palIt=palettes.begin(); palIt != palettes.end(); palIt++)
	{
		if(cur == index)
		{
			if(palette != NULL) { palette->usingCount--; }
			palette = *palIt;
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
	list<HSPalette*>::iterator palIt;
	for ( palIt=palettes.begin(); palIt != palettes.end(); palIt++)
	{
		if(palette == *palIt) { return cur; }
		cur++;
	}

	return -1;
}

HSObjectHold * HSObject::CreateNewHold()
{
	HSObjectHold * newHold = new HSObjectHold();

	return newHold;
}

HSObject * HSObject::CreateObjectOfSameType()
{
	return new HSObject();
}

void HSObject::CopyAttributes(HSObject * target)
{
	target->definitionFilePath = definitionFilePath;
	target->followParent = followParent;
	target->hFlip = hFlip;
	target->lifetime = lifetime;
	target->palette = palette;
	target->palettes = palettes;
	target->pos.x = pos.x; target->pos.y = pos.y;
	target->prevPos.x = pos.x; target->prevPos.y = pos.y;
	target->useTGAPalettes = useTGAPalettes;
	target->visible = visible;
}

void HSObject::CopyEventHold(HSObject * target, HSObjectHold * targetHold)
{
	if(hsObjectEventHolds.lifetimeDeath != NULL && targetHold->id == hsObjectEventHolds.lifetimeDeath->id) { target->hsObjectEventHolds.lifetimeDeath = targetHold; }
}

int HSObject::SaveEventHolds(HSObjectHold * hold, XMLElement * eventHolds)
{
	//check the attribute names, and put the hold pointers into the proper event hold slots.
	unsigned int eventHoldId;
	eventHolds->QueryUnsignedAttribute("lifetimeDeath", &eventHoldId);
	if(hold->id == eventHoldId) { hsObjectEventHolds.lifetimeDeath = (HSObjectHold*)hold; }

	return 0;
}

int HSObject::AddHold(HSObjectHold * newHold)
{
	if(firstHold == NULL)
	{
		//just make the new hold the first hold
		firstHold = newHold;
		lastHold = newHold;
	}
	else
	{
		//put the new hold at the end of the list
		lastHold->nextListHold = newHold;
		lastHold = newHold;
	}

	return 0;
}

int HSObject::LinkHolds()
{
	for(HSObjectHold * i = firstHold; i != NULL; i = i->nextListHold)
	{
		if(i->nextHoldId > 0)
		{
			for(HSObjectHold * j = firstHold; j != NULL; j = j->nextListHold)
			{
				if(j->id == i->nextHoldId)
				{
					i->nextHold = j;
					break;
				}
			}
			if(i->nextHold == NULL)
			{
				UpdateLog("Next hold not found when linking holds.", true);
				return -1; //the nextHold wasn't found
			}
		}
	}

	return 0;
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
			ChangeHold(curHold->nextHold);
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

	//move according to parent, if applicable
	if(parent != NULL && followParent)
	{
		pos.x += parent->pos.x - parent->prevPos.x;
		pos.y += parent->pos.y - parent->prevPos.y;
	}

	//move according to velocity
	pos.x += vel.x;
	pos.y += vel.y;

	prevVel.x = vel.x;
	prevVel.y = vel.y;

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

bool HSObject::ChangeHold(HSObjectHold* hold)
{
	holdTime = 0;
	curHold = hold;
	if(curHold == NULL)
	{
		curHold = GetDefaultHold();
		return false;
	}

	return true;
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