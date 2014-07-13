#include "objectManager.h"

list<CurrentAudioEntry*> currentAudio;

ObjectManager::ObjectManager()
{
	notDone = true;
	renderingThread = NULL;
	doRender = false;
	applyVideoSettings = false;
	centerCameraInstantly = false;
	matchCamera = false;
	loadTexturesAndPalettes = false;
	clearTexturesAndPalettes = false;
	renderingErrorCode = 0;

	texturesToDelete.clear();
	buffersToDelete.clear();
	vaosToDelete.clear();
	palettesToDelete.clear();

	vidInitLock = NULL;
	vidInitDone = NULL;

	renderLock = NULL;
	renderDone = NULL;

	processLock = NULL;
	processDone = NULL;
	
	fullScreen = false;
	fullScreenToApply = false;
	stretchScreen = false;
	stretchScreenToApply = false;
	windowedResolutionX = 0;
	windowedResolutionY = 0;
	windowedResolutionXToApply = 0;
	windowedResolutionYToApply = 0;
	fullscreenResolutionX = 0;
	fullscreenResolutionY = 0;
	fullscreenResolutionXToApply = 0;
	fullscreenResolutionYToApply = 0;
	needApplyWindowedResolutionForMenu = false;
	windowedResolutionXForMenu = 0;
	windowedResolutionYForMenu = 0;
	needApplyFullscreenResolutionForMenu = false;
	fullscreenResolutionXForMenu = 0;
	fullscreenResolutionYForMenu = 0;

	stageObjectsNeedSort = false;
	BGSpawnedObjectsNeedSort = false;
	fighterObjectsNeedSort = false;
	FGSpawnedObjectsNeedSort = false;
	HUDObjectsNeedSort = false;
	stageObjects.clear();
	BGSpawnedObjects.clear();
	fighterObjects.clear();
	FGSpawnedObjects.clear();
	HUDObjects.clear();
	textureRegistry.clear();
	paletteRegistry.clear();
	audioRegistry.clear();
	fontRegistry.clear();
	newObjectId = 1;
	openGL3 = false;
	obtainedAudioSpec = NULL;
	currentAudio.clear();
	loadingBackground = NULL;
	loadingFont = NULL;
	loadingText = NULL;
	menuManager = NULL;
	characterSelectManager = NULL;
	hudManager = NULL;

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		spawnPoints[i].x = 0;
		spawnPoints[i].y = 0;
		players[i] = NULL;
		focusObject[i] = NULL;
	}
}

bool HSObjectDepthSort(HSObject * first, HSObject * second)
{
	if(first->depth > second->depth) { return true; }
	return false;
}

bool HSOrderableSort(HSOrderable first, HSOrderable second)
{
	if(first.orderID < second.orderID) { return true; }
	return false;
}

bool HSOrderablePointerSort(HSOrderable * first, HSOrderable * second)
{
	if(first->orderID < second->orderID) { return true; }
	return false;
}

void ObjectManager::SortAllObjects()
{
	if(stageObjectsNeedSort)
	{
		stageObjects.sort(HSObjectDepthSort);
		stageObjectsNeedSort = false;
	}

	if(HUDObjectsNeedSort)
	{
		HUDObjects.sort(HSObjectDepthSort);
		HUDObjectsNeedSort = false;
	}
}

int ObjectManager::LoadDefinition(string defFilePath, list<HSObject*> * objects, HSObject ** returnValue)
{
	//get the XML structure from the file
	XMLDocument * file = new XMLDocument();
	if(int error = file->LoadFile(defFilePath.data()) != 0)
	{
		stringstream sstm;
		sstm << "Error loading definition file. Code: " << error << " File: " << defFilePath;
		UpdateLog(sstm.str(), true);
		return error; //couldn't load the file
	}

	if(strcmp(file->RootElement()->Value(), "HSObjects") != 0)
	{
		UpdateLog("XML file is not Homestrife object definition file: " + defFilePath, true);
		return -1; //this isn't an HSObjects definition file
	}

	XMLElement * root = file->RootElement();

	//loop through the individual objects
	for(XMLElement * objDef = root->FirstChildElement(); objDef != NULL; objDef = objDef->NextSiblingElement())
	{
		//get the object type
		const char * objectType = objDef->Value();

		//create a new object of the appropriate type
		HSObject * newObject;
		if(strcmp(objectType, "HSObject") == 0)
		{
			newObject = new HSObject();
		}
		else if(strcmp(objectType, "TerrainObject") == 0)
		{
			newObject = new TerrainObject();
		}
		else if(strcmp(objectType, "PhysicsObject") == 0)
		{
			newObject = new PhysicsObject();
		}
		else if(strcmp(objectType, "Fighter") == 0)
		{
			newObject = new Fighter();
		}
		else
		{
			continue; //invalid object type
		}

		newObject->definitionFilePath = defFilePath;

		//get the parts of the object definition
		XMLElement * holdsDef = NULL;
		XMLElement * eventHoldsDef = NULL;
		XMLElement * terrainBoxesDef = NULL;
		XMLElement * uprightTerrainBoxDef = NULL;
		XMLElement * crouchingTerrainBoxDef = NULL;
		XMLElement * proneTerrainBoxDef = NULL;
		XMLElement * compactTerrainBoxDef = NULL;
		XMLElement * palettesDef = NULL;
		for(XMLElement * partDef = objDef->FirstChildElement(); partDef != NULL; partDef = partDef->NextSiblingElement())
		{
			if(strcmp(partDef->Value(), "Holds") == 0) { holdsDef = partDef; }
			else if(strcmp(partDef->Value(), "EventHolds") == 0) { eventHoldsDef = partDef; }
			else if(strcmp(partDef->Value(), "TerrainBoxes") == 0) { terrainBoxesDef = partDef; }
			else if(strcmp(partDef->Value(), "UprightTerrainBox") == 0) { uprightTerrainBoxDef = partDef; }
			else if(strcmp(partDef->Value(), "CrouchingTerrainBox") == 0) { crouchingTerrainBoxDef = partDef; }
			else if(strcmp(partDef->Value(), "ProneTerrainBox") == 0) { proneTerrainBoxDef = partDef; }
			else if(strcmp(partDef->Value(), "CompactTerrainBox") == 0) { compactTerrainBoxDef = partDef; }
			else if(strcmp(partDef->Value(), "Palettes") == 0) { palettesDef = partDef; }
		}

		//get object attributes
		objDef->QueryUnsignedAttribute("lifetime", &newObject->lifetime);

		if(newObject->IsTerrainObject())
		{
			TerrainObject * newTObject = (TerrainObject*)newObject;

			objDef->QueryFloatAttribute("bounce", &newTObject->bounce);
			objDef->QueryBoolAttribute("canBeJumpedThrough", &newTObject->canBeJumpedThrough);
			objDef->QueryFloatAttribute("friction", &newTObject->friction);
			objDef->QueryIntAttribute("health", &newTObject->health);
			objDef->QueryBoolAttribute("takesTerrainDamage", &newTObject->takesTerrainDamage);
			objDef->QueryBoolAttribute("fragile", &newTObject->fragile);

			if(terrainBoxesDef != NULL)
			{
				for(XMLElement * terrainBoxDef = terrainBoxesDef->FirstChildElement(); terrainBoxDef != NULL; terrainBoxDef = terrainBoxDef->NextSiblingElement())
				{
					if(strcmp(terrainBoxDef->Value(), "TerrainBox") != 0)
					{
						continue;
					}

					HSBox * newTerrainBox = new HSBox();
					
					if(int error = DefineBox(terrainBoxDef, newTerrainBox) != 0)
					{
						return error; //there was an error defining the box
					}

					if(newTObject->firstTerrainBox == NULL)
					{
						//just make the new hit box the first hit box
						newTObject->firstTerrainBox = newTerrainBox;
						newTObject->lastTerrainBox = newTerrainBox;
					}
					else
					{
						//put the new hit box at the end of the list
						newTObject->lastTerrainBox->nextBox = newTerrainBox;
						newTObject->lastTerrainBox = newTerrainBox;
					}
				}
			}
		}

		if(newObject->IsPhysicsObject())
		{
			PhysicsObject * newPObject = (PhysicsObject*)newObject;

			objDef->QueryBoolAttribute("falls", &newPObject->falls);
			objDef->QueryFloatAttribute("mass", &newPObject->mass);
			objDef->QueryFloatAttribute("maxFallSpeed", &newPObject->maxFallSpeed);
		}

		if(newObject->IsFighter())
		{
			Fighter * newFighter = (Fighter*)newObject;
			
			objDef->QueryIntAttribute("airActions", &newFighter->airActions);
			objDef->QueryFloatAttribute("airControlAccel", &newFighter->airControlAccel);
			objDef->QueryIntAttribute("backwardAirDashDuration", &newFighter->backwardAirDashDuration);
			objDef->QueryFloatAttribute("backwardAirDashSpeed", &newFighter->backwardAirDashSpeed);
			objDef->QueryIntAttribute("forwardAirDashDuration", &newFighter->forwardAirDashDuration);
			objDef->QueryFloatAttribute("forwardAirDashSpeed", &newFighter->forwardAirDashSpeed);
			objDef->QueryFloatAttribute("jumpSpeed", &newFighter->jumpSpeed);
			objDef->QueryFloatAttribute("maxAirControlSpeed", &newFighter->maxAirControlSpeed);
			objDef->QueryFloatAttribute("runSpeed", &newFighter->runSpeed);
			objDef->QueryFloatAttribute("stepHeight", &newFighter->stepHeight);
			objDef->QueryFloatAttribute("walkSpeed", &newFighter->walkSpeed);

			HSBox * newBox = new HSBox();
			DefineBox(uprightTerrainBoxDef, newBox);
			newFighter->firstUprightTerrainBox = newBox;

			newBox = new HSBox();
			DefineBox(crouchingTerrainBoxDef, newBox);
			newFighter->firstCrouchingTerrainBox = newBox;

			newBox = new HSBox();
			DefineBox(proneTerrainBoxDef, newBox);
			newFighter->firstProneTerrainBox = newBox;

			newBox = new HSBox();
			DefineBox(compactTerrainBoxDef, newBox);
			newFighter->firstCompactTerrainBox = newBox;
		}

		//get palettes
		newObject->useTGAPalettes = true;
		if(palettesDef != NULL)
		{
			for(XMLElement * paletteDef = palettesDef->FirstChildElement(); paletteDef != NULL; paletteDef = paletteDef->NextSiblingElement())
			{
				if(strcmp(paletteDef->Value(), "Palette") != 0)
				{
					continue;
				}

				newObject->useTGAPalettes = false;

				string palFilePath = "";
				if(paletteDef->Attribute("path") != NULL) { palFilePath = paletteDef->Attribute("path"); }
				PaletteInstance newPalInst;
				
				//see if the palette has already been loaded
				bool alreadyLoaded = false;
				list<HSPalette*>::iterator plIt;
				for ( plIt=paletteRegistry.begin(); plIt != paletteRegistry.end(); plIt++)
				{
					if((*plIt)->paletteFilePath.compare(palFilePath) == 0)
					{
						newPalInst.hsPal = (*plIt);
						(*plIt)->usingCount++;
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
					newPal->textureID = 0;

					//if(int error = LoadHSPToPalette(newPal) != 0) //load the texture
					//{
					//	return error;
					//}

					paletteRegistry.push_back(newPal);
					newPalInst.hsPal = newPal;
				}

				if(paletteDef->Attribute("name") != NULL) { newPalInst.name = paletteDef->Attribute("name"); }
				paletteDef->QueryUnsignedAttribute("id", &newPalInst.orderID);
				newObject->palettes.push_back(newPalInst);
			}

			newObject->palettes.sort(HSOrderableSort);
		}

		newObject->SetPalette(0);

		//get holds
		for(XMLElement * holdDef = holdsDef->FirstChildElement(); holdDef != NULL; holdDef = holdDef->NextSiblingElement())
		{
			if(strcmp(holdDef->Value(), "Hold") != 0)
			{
				continue;
			}

			//create a new hold of the appropriate type
			HSObjectHold * newHold;
			if(strcmp(objectType, "HSObject") == 0)
			{
				newHold = new HSObjectHold();
			}
			else if(strcmp(objectType, "TerrainObject") == 0)
			{
				newHold = new TerrainObjectHold();
			}
			else if(strcmp(objectType, "PhysicsObject") == 0)
			{
				newHold = new PhysicsObjectHold();
			}
			else if(strcmp(objectType, "Fighter") == 0)
			{
				newHold = new FighterHold();
			}

			//get the hold's attributes
			holdDef->QueryUnsignedAttribute("id", &(newHold->id));
			holdDef->QueryUnsignedAttribute("nextHoldId", &(newHold->nextHoldId));
			holdDef->QueryUnsignedAttribute("duration", &(newHold->duration));
			holdDef->QueryBoolAttribute("overwriteVelocity", &(newHold->overwriteVelocity));
			holdDef->QueryFloatAttribute("velocityX", &(newHold->velocity.x));
			holdDef->QueryFloatAttribute("velocityY", &(newHold->velocity.y));
			holdDef->QueryFloatAttribute("repositionX", &(newHold->reposition.x));
			holdDef->QueryFloatAttribute("repositionY", &(newHold->reposition.y));

			//get the event holds
			unsigned int eventHoldId = 0;
			if(eventHoldsDef != NULL)
			{
				eventHoldsDef->QueryUnsignedAttribute("lifetimeDeath", &eventHoldId);
				if(newHold->id == eventHoldId) { newObject->hsObjectEventHolds.lifetimeDeath = newHold; }
			}

			if(newHold->IsTerrainObjectHold())
			{
				TerrainObjectHold * newTOHold = (TerrainObjectHold*)newHold;

				//get changeAttackBoxAttributes
				const char * caba = holdDef->Attribute("changeAttackBoxAttributes");
				string bString;
				if(caba != NULL)
				{
					bString.assign(caba);
					if(bString.compare("true") == 0) { newTOHold->changeAttackBoxAttributes = true; }
					else if(bString.compare("false") == 0) { newTOHold->changeAttackBoxAttributes = false; }
				}

				//get blockability
				const char * b = holdDef->Attribute("blockability");
				if(b != NULL)
				{
					bString.assign(b);
					if(bString.compare("UNBLOCKABLE") == 0) { newTOHold->blockability = UNBLOCKABLE; }
					else if(bString.compare("HIGH") == 0) { newTOHold->blockability = HIGH; }
					else if(bString.compare("LOW") == 0) { newTOHold->blockability = LOW; }
					else if(bString.compare("MID") == 0) { newTOHold->blockability = MID; }
				}

				//get horizontalDirectionBasedBlock
				const char * hdbb = holdDef->Attribute("horizontalDirectionBasedBlock");
				if(hdbb != NULL)
				{
					bString.assign(hdbb);
					if(bString.compare("true") == 0) { newTOHold->horizontalDirectionBasedBlock = true; }
					else if(bString.compare("false") == 0) { newTOHold->horizontalDirectionBasedBlock = false; }
				}

				//get reversedHorizontalBlock
				const char * rhb = holdDef->Attribute("reversedHorizontalBlock");
				if(rhb != NULL)
				{
					bString.assign(rhb);
					if(bString.compare("true") == 0) { newTOHold->reversedHorizontalBlock = true; }
					else if(bString.compare("false") == 0) { newTOHold->reversedHorizontalBlock = false; }
				}

				//get damage
				holdDef->QueryIntAttribute("damage", &newTOHold->damage);

				//get hitstop
				holdDef->QueryIntAttribute("ownHitstop", &newTOHold->ownHitstop);
				holdDef->QueryIntAttribute("victimHitstop", &newTOHold->victimHitstop);

				//get hitstun
				holdDef->QueryIntAttribute("hitstun", &newTOHold->hitstun);

				//get blockstun
				holdDef->QueryIntAttribute("blockstun", &newTOHold->blockstun);

				//get force
				holdDef->QueryFloatAttribute("forceX", &newTOHold->force.x);
				holdDef->QueryFloatAttribute("forceY", &newTOHold->force.y);

				//get trips
				const char * t = holdDef->Attribute("trips");
				if(t != NULL)
				{
					bString.assign(t);
					if(bString.compare("true") == 0) { newTOHold->trips = true; }
					else if(bString.compare("false") == 0) { newTOHold->trips = false; }
				}

				//get resetHits
				const char * r = holdDef->Attribute("resetHits");
				if(r != NULL)
				{
					bString.assign(r);
					if(bString.compare("true") == 0) { newTOHold->resetHits = true; }
					else if(bString.compare("false") == 0) { newTOHold->resetHits = false; }
				}

				//get the event holds
				if(eventHoldsDef != NULL)
				{
					TerrainObject * newTObject = (TerrainObject*)newObject;
				
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("healthDeath", &eventHoldId);
					if(newHold->id == eventHoldId) { newTObject->terrainEventHolds.healthDeath = newTOHold; }
				}
			}

			if(newHold->IsPhysicsObjectHold())
			{
				PhysicsObjectHold * newPOHold = (PhysicsObjectHold*)newHold;

				//get changePhysicsAttributes
				const char * cpa = holdDef->Attribute("changePhysicsAttributes");
				string bString;
				if(cpa != NULL)
				{
					bString.assign(cpa);
					if(bString.compare("true") == 0) { newPOHold->changePhysicsAttributes = true; }
					else if(bString.compare("false") == 0) { newPOHold->changePhysicsAttributes = false; }
				}
				
				//get ignoreGravity
				const char * ig = holdDef->Attribute("ignoreGravity");
				if(ig != NULL)
				{
					bString.assign(ig);
					if(bString.compare("true") == 0) { newPOHold->ignoreGravity = true; }
					else if(bString.compare("false") == 0) { newPOHold->ignoreGravity = false; }
				}
			}

			if(newHold->IsFighterHold())
			{
				FighterHold * newFHold = (FighterHold*)newHold;

				//get changeFighterAttributes
				const char * cfa = holdDef->Attribute("changeFighterAttributes");
				string bString;
				if(cfa != NULL)
				{
					bString.assign(cfa);
					if(bString.compare("true") == 0) { newFHold->changeFighterAttributes = true; }
					else if(bString.compare("false") == 0) { newFHold->changeFighterAttributes = false; }
				}

				//get disableAirControl
				const char * dac = holdDef->Attribute("disableAirControl");
				if(dac != NULL)
				{
					bString.assign(dac);
					if(bString.compare("true") == 0) { newFHold->disableAirControl = true; }
					else if(bString.compare("false") == 0) { newFHold->disableAirControl = false; }
				}

				//get endAirDash
				const char * ead = holdDef->Attribute("endAirDash");
				if(ead != NULL)
				{
					bString.assign(ead);
					if(bString.compare("true") == 0) { newFHold->endAirDash = true; }
					else if(bString.compare("false") == 0) { newFHold->endAirDash = false; }
				}

				//get changeCancels
				const char * cc = holdDef->Attribute("changeCancels");
				if(cc != NULL)
				{
					bString.assign(cc);
					if(bString.compare("true") == 0) { newFHold->changeCancels = true; }
					else if(bString.compare("false") == 0) { newFHold->changeCancels = false; }
				}

				//get cancels
				const char * dc = holdDef->Attribute("dashCancel");
				if(dc != NULL)
				{
					bString.assign(dc);
					if(bString.compare("ANY_TIME") == 0) { newFHold->cancels.dash = ANY_TIME; }
					else if(bString.compare("AFTER_HIT_OR_BLOCK") == 0) { newFHold->cancels.dash = AFTER_HIT_OR_BLOCK; }
					else if(bString.compare("AFTER_HIT") == 0) { newFHold->cancels.dash = AFTER_HIT; }
					else if(bString.compare("AFTER_BLOCK") == 0) { newFHold->cancels.dash = AFTER_BLOCK; }
					else if(bString.compare("NEVER") == 0) { newFHold->cancels.dash = NEVER; }
				}
				const char * jc = holdDef->Attribute("jumpCancel");
				if(jc != NULL)
				{
					bString.assign(jc);
					if(bString.compare("ANY_TIME") == 0) { newFHold->cancels.jump = ANY_TIME; }
					else if(bString.compare("AFTER_HIT_OR_BLOCK") == 0) { newFHold->cancels.jump = AFTER_HIT_OR_BLOCK; }
					else if(bString.compare("AFTER_HIT") == 0) { newFHold->cancels.jump = AFTER_HIT; }
					else if(bString.compare("AFTER_BLOCK") == 0) { newFHold->cancels.jump = AFTER_BLOCK; }
					else if(bString.compare("NEVER") == 0) { newFHold->cancels.jump = NEVER; }
				}
				const char * lnc = holdDef->Attribute("lightNeutralCancel");
				if(lnc != NULL)
				{
					bString.assign(lnc);
					if(bString.compare("ANY_TIME") == 0) { newFHold->cancels.lightNeutral = ANY_TIME; }
					else if(bString.compare("AFTER_HIT_OR_BLOCK") == 0) { newFHold->cancels.lightNeutral = AFTER_HIT_OR_BLOCK; }
					else if(bString.compare("AFTER_HIT") == 0) { newFHold->cancels.lightNeutral = AFTER_HIT; }
					else if(bString.compare("AFTER_BLOCK") == 0) { newFHold->cancels.lightNeutral = AFTER_BLOCK; }
					else if(bString.compare("NEVER") == 0) { newFHold->cancels.lightNeutral = NEVER; }
				}
				const char * lfc = holdDef->Attribute("lightForwardCancel");
				if(lfc != NULL)
				{
					bString.assign(lfc);
					if(bString.compare("ANY_TIME") == 0) { newFHold->cancels.lightForward = ANY_TIME; }
					else if(bString.compare("AFTER_HIT_OR_BLOCK") == 0) { newFHold->cancels.lightForward = AFTER_HIT_OR_BLOCK; }
					else if(bString.compare("AFTER_HIT") == 0) { newFHold->cancels.lightForward = AFTER_HIT; }
					else if(bString.compare("AFTER_BLOCK") == 0) { newFHold->cancels.lightForward = AFTER_BLOCK; }
					else if(bString.compare("NEVER") == 0) { newFHold->cancels.lightForward = NEVER; }
				}
				const char * luc = holdDef->Attribute("lightUpCancel");
				if(luc != NULL)
				{
					bString.assign(luc);
					if(bString.compare("ANY_TIME") == 0) { newFHold->cancels.lightUp = ANY_TIME; }
					else if(bString.compare("AFTER_HIT_OR_BLOCK") == 0) { newFHold->cancels.lightUp = AFTER_HIT_OR_BLOCK; }
					else if(bString.compare("AFTER_HIT") == 0) { newFHold->cancels.lightUp = AFTER_HIT; }
					else if(bString.compare("AFTER_BLOCK") == 0) { newFHold->cancels.lightUp = AFTER_BLOCK; }
					else if(bString.compare("NEVER") == 0) { newFHold->cancels.lightUp = NEVER; }
				}
				const char * ldc = holdDef->Attribute("lightDownCancel");
				if(ldc != NULL)
				{
					bString.assign(ldc);
					if(bString.compare("ANY_TIME") == 0) { newFHold->cancels.lightDown = ANY_TIME; }
					else if(bString.compare("AFTER_HIT_OR_BLOCK") == 0) { newFHold->cancels.lightDown = AFTER_HIT_OR_BLOCK; }
					else if(bString.compare("AFTER_HIT") == 0) { newFHold->cancels.lightDown = AFTER_HIT; }
					else if(bString.compare("AFTER_BLOCK") == 0) { newFHold->cancels.lightDown = AFTER_BLOCK; }
					else if(bString.compare("NEVER") == 0) { newFHold->cancels.lightDown = NEVER; }
				}
				const char * lbc = holdDef->Attribute("lightBackwardCancel");
				if(lbc != NULL)
				{
					bString.assign(lbc);
					if(bString.compare("ANY_TIME") == 0) { newFHold->cancels.lightBackward = ANY_TIME; }
					else if(bString.compare("AFTER_HIT_OR_BLOCK") == 0) { newFHold->cancels.lightBackward = AFTER_HIT_OR_BLOCK; }
					else if(bString.compare("AFTER_HIT") == 0) { newFHold->cancels.lightBackward = AFTER_HIT; }
					else if(bString.compare("AFTER_BLOCK") == 0) { newFHold->cancels.lightBackward = AFTER_BLOCK; }
					else if(bString.compare("NEVER") == 0) { newFHold->cancels.lightBackward = NEVER; }
				}
				const char * lqc = holdDef->Attribute("lightQCFCancel");
				if(lqc != NULL)
				{
					bString.assign(lqc);
					if(bString.compare("ANY_TIME") == 0) { newFHold->cancels.lightQCF = ANY_TIME; }
					else if(bString.compare("AFTER_HIT_OR_BLOCK") == 0) { newFHold->cancels.lightQCF = AFTER_HIT_OR_BLOCK; }
					else if(bString.compare("AFTER_HIT") == 0) { newFHold->cancels.lightQCF = AFTER_HIT; }
					else if(bString.compare("AFTER_BLOCK") == 0) { newFHold->cancels.lightQCF = AFTER_BLOCK; }
					else if(bString.compare("NEVER") == 0) { newFHold->cancels.lightQCF = NEVER; }
				}
				const char * hnc = holdDef->Attribute("heavyNeutralCancel");
				if(hnc != NULL)
				{
					bString.assign(hnc);
					if(bString.compare("ANY_TIME") == 0) { newFHold->cancels.heavyNeutral = ANY_TIME; }
					else if(bString.compare("AFTER_HIT_OR_BLOCK") == 0) { newFHold->cancels.heavyNeutral = AFTER_HIT_OR_BLOCK; }
					else if(bString.compare("AFTER_HIT") == 0) { newFHold->cancels.heavyNeutral = AFTER_HIT; }
					else if(bString.compare("AFTER_BLOCK") == 0) { newFHold->cancels.heavyNeutral = AFTER_BLOCK; }
					else if(bString.compare("NEVER") == 0) { newFHold->cancels.heavyNeutral = NEVER; }
				}
				const char * hfc = holdDef->Attribute("heavyForwardCancel");
				if(hfc != NULL)
				{
					bString.assign(hfc);
					if(bString.compare("ANY_TIME") == 0) { newFHold->cancels.heavyForward = ANY_TIME; }
					else if(bString.compare("AFTER_HIT_OR_BLOCK") == 0) { newFHold->cancels.heavyForward = AFTER_HIT_OR_BLOCK; }
					else if(bString.compare("AFTER_HIT") == 0) { newFHold->cancels.heavyForward = AFTER_HIT; }
					else if(bString.compare("AFTER_BLOCK") == 0) { newFHold->cancels.heavyForward = AFTER_BLOCK; }
					else if(bString.compare("NEVER") == 0) { newFHold->cancels.heavyForward = NEVER; }
				}
				const char * huc = holdDef->Attribute("heavyUpCancel");
				if(huc != NULL)
				{
					bString.assign(huc);
					if(bString.compare("ANY_TIME") == 0) { newFHold->cancels.heavyUp = ANY_TIME; }
					else if(bString.compare("AFTER_HIT_OR_BLOCK") == 0) { newFHold->cancels.heavyUp = AFTER_HIT_OR_BLOCK; }
					else if(bString.compare("AFTER_HIT") == 0) { newFHold->cancels.heavyUp = AFTER_HIT; }
					else if(bString.compare("AFTER_BLOCK") == 0) { newFHold->cancels.heavyUp = AFTER_BLOCK; }
					else if(bString.compare("NEVER") == 0) { newFHold->cancels.heavyUp = NEVER; }
				}
				const char * hdc = holdDef->Attribute("heavyDownCancel");
				if(hdc != NULL)
				{
					bString.assign(hdc);
					if(bString.compare("ANY_TIME") == 0) { newFHold->cancels.heavyDown = ANY_TIME; }
					else if(bString.compare("AFTER_HIT_OR_BLOCK") == 0) { newFHold->cancels.heavyDown = AFTER_HIT_OR_BLOCK; }
					else if(bString.compare("AFTER_HIT") == 0) { newFHold->cancels.heavyDown = AFTER_HIT; }
					else if(bString.compare("AFTER_BLOCK") == 0) { newFHold->cancels.heavyDown = AFTER_BLOCK; }
					else if(bString.compare("NEVER") == 0) { newFHold->cancels.heavyDown = NEVER; }
				}
				const char * hbc = holdDef->Attribute("heavyBackwardCancel");
				if(hbc != NULL)
				{
					bString.assign(hbc);
					if(bString.compare("ANY_TIME") == 0) { newFHold->cancels.heavyBackward = ANY_TIME; }
					else if(bString.compare("AFTER_HIT_OR_BLOCK") == 0) { newFHold->cancels.heavyBackward = AFTER_HIT_OR_BLOCK; }
					else if(bString.compare("AFTER_HIT") == 0) { newFHold->cancels.heavyBackward = AFTER_HIT; }
					else if(bString.compare("AFTER_BLOCK") == 0) { newFHold->cancels.heavyBackward = AFTER_BLOCK; }
					else if(bString.compare("NEVER") == 0) { newFHold->cancels.heavyBackward = NEVER; }
				}
				const char * hqc = holdDef->Attribute("heavyQCFCancel");
				if(hqc != NULL)
				{
					bString.assign(hqc);
					if(bString.compare("ANY_TIME") == 0) { newFHold->cancels.heavyQCF = ANY_TIME; }
					else if(bString.compare("AFTER_HIT_OR_BLOCK") == 0) { newFHold->cancels.heavyQCF = AFTER_HIT_OR_BLOCK; }
					else if(bString.compare("AFTER_HIT") == 0) { newFHold->cancels.heavyQCF = AFTER_HIT; }
					else if(bString.compare("AFTER_BLOCK") == 0) { newFHold->cancels.heavyQCF = AFTER_BLOCK; }
					else if(bString.compare("NEVER") == 0) { newFHold->cancels.heavyQCF = NEVER; }
				}

				//get the event holds
				if(eventHoldsDef != NULL)
				{
					Fighter * newFighter = (Fighter*)newObject;
				
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("standing", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.standing = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("turn", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.turn = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("walk", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.walk = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("walking", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.walking = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("walkingTurn", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.walkingTurn = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("run", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.run = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("running", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.running = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("runningTurn", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.runningTurn = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("runningStop", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.runningStop = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("crouch", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.crouch = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("crouching", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.crouching = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("crouchingTurn", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.crouchingTurn = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("stand", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.stand = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("jumpNeutralStart", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.jumpNeutralStart = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("jumpNeutralStartAir", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.jumpNeutralStartAir = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("jumpNeutralRising", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.jumpNeutralRising = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("jumpNeutralFall", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.jumpNeutralFall = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("jumpNeutralFalling", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.jumpNeutralFalling = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("jumpNeutralLand", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.jumpNeutralLand = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("jumpBackwardRising", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.jumpBackwardRising = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("jumpBackwardFall", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.jumpBackwardFall = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("airDashForward", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.airDashForward = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("airDashBackward", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.airDashBackward = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("blockHigh", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.blockHigh = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("blockLow", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.blockLow = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("blockAir", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.blockAir = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("hitstunLightHighStanding", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.hitstunLightHighStanding = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("hitstunLightMidStanding", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.hitstunLightMidStanding = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("hitstunLightLowStanding", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.hitstunLightLowStanding = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("hitstunLightMidCrouching", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.hitstunLightMidCrouching = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("hitstunLightLowCrouching", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.hitstunLightLowCrouching = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("hitstunLightAir", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.hitstunLightAir = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("attackLightNeutralGround", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.attackLightNeutralGround = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("attackLightDownGround", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.attackLightDownGround = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("attackLightUpGround", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.attackLightUpGround = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("attackLightForwardGround", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.attackLightForwardGround = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("attackLightQCFGround", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.attackLightQCFGround = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("attackLightNeutralAir", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.attackLightNeutralAir = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("attackLightDownAir", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.attackLightDownAir = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("attackLightUpAir", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.attackLightUpAir = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("attackLightForwardAir", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.attackLightForwardAir = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("attackLightBackwardAir", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.attackLightBackwardAir = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("attackLightQCFAir", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.attackLightQCFAir = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("attackHeavyNeutralGround", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.attackHeavyNeutralGround = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("attackHeavyDownGround", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.attackHeavyDownGround = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("attackHeavyUpGround", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.attackHeavyUpGround = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("attackHeavyForwardGround", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.attackHeavyForwardGround = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("attackHeavyQCFGround", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.attackHeavyQCFGround = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("attackHeavyNeutralAir", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.attackHeavyNeutralAir = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("attackHeavyDownAir", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.attackHeavyDownAir = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("attackHeavyUpAir", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.attackHeavyUpAir = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("attackHeavyForwardAir", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.attackHeavyForwardAir = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("attackHeavyBackwardAir", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.attackHeavyBackwardAir = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("attackHeavyQCFAir", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.attackHeavyQCFAir = newFHold; }
	
					eventHoldId = 0;
					eventHoldsDef->QueryUnsignedAttribute("knockout", &eventHoldId);
					if(newHold->id == eventHoldId) { newFighter->fighterEventHolds.knockout = newFHold; }
				}
			}
					
			//iterate through the parts of the hold definition
			for(XMLElement * holdPartDef = holdDef->FirstChildElement(); holdPartDef != NULL; holdPartDef = holdPartDef->NextSiblingElement())
			{
				if(strcmp(holdPartDef->Value(), "Textures") == 0)
				{
					//get textures
					for(XMLElement * texDef = holdPartDef->FirstChildElement(); texDef != NULL; texDef = texDef->NextSiblingElement())
					{
						if(strcmp(texDef->Value(), "Texture") != 0)
						{
							continue;
						}

						string textureFilePath = "";
						if(texDef->Attribute("textureFilePath") != NULL) { textureFilePath = texDef->Attribute("textureFilePath"); }

						TextureInstance newTexInst;

						//see if the texture has already been loaded
						list<HSTexture*>::iterator trIt;
						bool texNotLoaded = true;
						for ( trIt=textureRegistry.begin(); trIt != textureRegistry.end(); trIt++)
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
							newTex->ownPalette = NULL;
							newTex->textureSlices.clear();
							newTex->useTGAPalette = newObject->useTGAPalettes;

							//if(int error = LoadTGAToTexture(newTex, openGL3, newObject->useTGAPalettes) != 0) //load the texture
							//{
							//	return error;
							//}

							newTexInst.hsTex = newTex;
							textureRegistry.push_back(newTex);
						}
	
						newTexInst.offset.x = 0;
						newTexInst.offset.y = 0;
						newTexInst.depth = 0;
						newTexInst.hScale = 1;
						newTexInst.vScale = 1;
						texDef->QueryFloatAttribute("offsetX", &(newTexInst.offset.x));
						texDef->QueryFloatAttribute("offsetY", &(newTexInst.offset.y));
						texDef->QueryIntAttribute("depth", &(newTexInst.depth));
						newHold->textures.push_back(newTexInst);
					}
				}
				else if(strcmp(holdPartDef->Value(), "AudioList") == 0)
				{
					//get audio
					for(XMLElement * audioDef = holdPartDef->FirstChildElement(); audioDef != NULL; audioDef = audioDef->NextSiblingElement())
					{
						if(strcmp(audioDef->Value(), "Audio") != 0)
						{
							continue;
						}

						//get the file path
						string audioFilePath = "";
						if(audioDef->Attribute("audioFilePath") != NULL) { audioFilePath = audioDef->Attribute("audioFilePath"); }

						AudioInstance newAudInst;

						//see if the audio has already been loaded
						list<HSAudio*>::iterator arIt;
						bool audNotLoaded = true;
						for ( arIt=audioRegistry.begin(); arIt != audioRegistry.end(); arIt++)
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
							audioRegistry.push_back(newAud);
						}
	
						newAudInst.delay = 0;
						audioDef->QueryIntAttribute("delay", &(newAudInst.delay));
						audioDef->QueryBoolAttribute("exclusive", &(newAudInst.exclusive));
						audioDef->QueryBoolAttribute("usePercentage", &(newAudInst.usePercentage));
						audioDef->QueryIntAttribute("percentage", &(newAudInst.percentage));
						newHold->audioList.push_back(newAudInst);
					}
				}
				else if(strcmp(holdPartDef->Value(), "SpawnObjects") == 0)
				{
					//get spawn objects
					for(XMLElement * spawnDef = holdPartDef->FirstChildElement(); spawnDef != NULL; spawnDef = spawnDef->NextSiblingElement())
					{
						if(strcmp(spawnDef->Value(), "SpawnObject") != 0)
						{
							continue;
						}

						//get the file path
						string spawnObjectFilePath = "";
						if(spawnDef->Attribute("definitionFilePath") != NULL) { spawnObjectFilePath = spawnDef->Attribute("definitionFilePath"); }

						SpawnObject newSpawnObject;
						newSpawnObject.parent = newObject;
						spawnDef->QueryIntAttribute("delay", &(newSpawnObject.delay));
						spawnDef->QueryIntAttribute("number", &(newSpawnObject.number));
						spawnDef->QueryBoolAttribute("followParent", &(newSpawnObject.followParent));
						spawnDef->QueryBoolAttribute("collideParent", &(newSpawnObject.collideParent));
						spawnDef->QueryFloatAttribute("parentOffsetX", &(newSpawnObject.parentOffset.x));
						spawnDef->QueryFloatAttribute("parentOffsetY", &(newSpawnObject.parentOffset.y));
						spawnDef->QueryFloatAttribute("velocityX", &(newSpawnObject.vel.x));
						spawnDef->QueryFloatAttribute("velocityY", &(newSpawnObject.vel.y));
						newSpawnObject.defFilePath = spawnObjectFilePath;

						//call LoadDefinition() recursively to load the spawn object. Pass in NULL for the
						//game object list, so the spawn object doesn't get added to it.
						LoadDefinition(spawnObjectFilePath, NULL, &(newSpawnObject.object));
						newSpawnObject.object->parent = newObject;

						newHold->spawnObjects.push_back(newSpawnObject);
					}
				}

				//only handle terrain object holds from here
				if(!newHold->IsTerrainObjectHold()) { continue; }

				TerrainObjectHold * newTOHold = (TerrainObjectHold*)newHold;

				if(strcmp(holdPartDef->Value(), "AttackBoxes") == 0)
				{
					//get attack boxes
					for(XMLElement * boxDef = holdPartDef->FirstChildElement(); boxDef != NULL; boxDef = boxDef->NextSiblingElement())
					{
						if(strcmp(boxDef->Value(), "Box") != 0)
						{
							continue;
						}

						HSBox * newAttackBox = new HSBox();
						newAttackBox->nextBox = NULL;
					
						if(int error = DefineBox(boxDef, newAttackBox) != 0)
						{
							return error; //there was an error defining the box
						}

						if(newTOHold->firstAttackBox == NULL)
						{
							//just make the new hit box the first hit box
							newTOHold->firstAttackBox = newAttackBox;
							newTOHold->lastAttackBox = newAttackBox;
						}
						else
						{
							//put the new hit box at the end of the list
							newTOHold->lastAttackBox->nextBox = newAttackBox;
							newTOHold->lastAttackBox = newAttackBox;
						}
					}
				}
				else if(strcmp(holdPartDef->Value(), "HurtBoxes") == 0)
				{
					//get attack boxes
					for(XMLElement * boxDef = holdPartDef->FirstChildElement(); boxDef != NULL; boxDef = boxDef->NextSiblingElement())
					{
						if(strcmp(boxDef->Value(), "Box") != 0)
						{
							continue;
						}

						HSBox * newHurtBox = new HSBox();
						newHurtBox->nextBox = NULL;
					
						if(int error = DefineBox(boxDef, newHurtBox) != 0)
						{
							return error; //there was an error defining the box
						}

						if(newTOHold->firstHurtBox == NULL)
						{
							//just make the new hit box the first hit box
							newTOHold->firstHurtBox = newHurtBox;
							newTOHold->lastHurtBox = newHurtBox;
						}
						else
						{
							//put the new hit box at the end of the list
							newTOHold->lastHurtBox->nextBox = newHurtBox;
							newTOHold->lastHurtBox = newHurtBox;
						}
					}
				}
				else if(strcmp(holdPartDef->Value(), "HitAudioList") == 0)
				{
					//get hit audio
					for(XMLElement * hitAudioDef = holdPartDef->FirstChildElement(); hitAudioDef != NULL; hitAudioDef = hitAudioDef->NextSiblingElement())
					{
						if(strcmp(hitAudioDef->Value(), "HitAudio") != 0)
						{
							continue;
						}

						//get the file path
						string audioFilePath = "";
						if(hitAudioDef->Attribute("hitAudioFilePath") != NULL) { audioFilePath = hitAudioDef->Attribute("hitAudioFilePath"); }

						AudioInstance newAudInst;

						//see if the audio has already been loaded
						list<HSAudio*>::iterator arIt;
						bool audNotLoaded = true;
						for ( arIt=audioRegistry.begin(); arIt != audioRegistry.end(); arIt++)
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
							audioRegistry.push_back(newAud);
						}
	
						newAudInst.delay = 0;
						hitAudioDef->QueryIntAttribute("delay", &(newAudInst.delay));
						hitAudioDef->QueryIntAttribute("percentage", &(newAudInst.percentage));
						hitAudioDef->QueryBoolAttribute("exclusive", &(newAudInst.exclusive));
						hitAudioDef->QueryBoolAttribute("usePercentage", &(newAudInst.usePercentage));
						newTOHold->hitAudioList.push_back(newAudInst);
					}
				}
				else if(strcmp(holdPartDef->Value(), "BlockedAudioList") == 0)
				{
					//get blocked audio
					for(XMLElement * blockedAudioDef = holdPartDef->FirstChildElement(); blockedAudioDef != NULL; blockedAudioDef = blockedAudioDef->NextSiblingElement())
					{
						if(strcmp(blockedAudioDef->Value(), "BlockedAudio") != 0)
						{
							continue;
						}

						//get the file path
						string audioFilePath = "";
						if(blockedAudioDef->Attribute("blockedAudioFilePath") != NULL) { audioFilePath = blockedAudioDef->Attribute("blockedAudioFilePath"); }

						AudioInstance newAudInst;

						//see if the audio has already been loaded
						list<HSAudio*>::iterator arIt;
						bool audNotLoaded = true;
						for ( arIt=audioRegistry.begin(); arIt != audioRegistry.end(); arIt++)
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
							audioRegistry.push_back(newAud);
						}
	
						newAudInst.delay = 0;
						blockedAudioDef->QueryIntAttribute("delay", &(newAudInst.delay));
						blockedAudioDef->QueryIntAttribute("percentage", &(newAudInst.percentage));
						blockedAudioDef->QueryBoolAttribute("exclusive", &(newAudInst.exclusive));
						blockedAudioDef->QueryBoolAttribute("usePercentage", &(newAudInst.usePercentage));
						newTOHold->blockedAudioList.push_back(newAudInst);
					}
				}
			}

			newObject->numHolds++;

			//add the hold to the object
			if(newObject->firstHold == NULL)
			{
				//just make the new hit box the first hit box
				newObject->firstHold = newHold;
				newObject->lastHold = newHold;
			}
			else
			{
				//put the new hit box at the end of the list
				newObject->lastHold->nextListHold = newHold;
				newObject->lastHold = newHold;
			}
		}

		if(newObject->firstHold == NULL)
		{
			//object has no holds! forget it
			UpdateLog("Object has no holds: " + defFilePath, false);
			delete newObject;
			return 0;
		}

		//set the next holds
		for(HSObjectHold * i = newObject->firstHold; i != NULL; i = i->nextListHold)
		{
			if(i->nextHoldId > 0)
			{
				for(HSObjectHold * j = newObject->firstHold; j != NULL; j = j->nextListHold)
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

		if(objects != NULL)
		{
			newObject->id = newObjectId;
			newObjectId++;
			//to keep things from exploding...
			newObject->ChangeHold(newObject->firstHold);

			objects->push_back(newObject);

			if(objects == &stageObjects)
			{
				stageObjectsNeedSort = true;
			}
			else if(objects == &HUDObjects)
			{
				HUDObjectsNeedSort = true;
			}
		}

		if(returnValue != NULL)
		{
			*returnValue = newObject;
		}
	}

	delete file;

	return 0;
}

int ObjectManager::LoadStage(string defFilePath)
{
	//get the XML structure from the file
	XMLDocument * file = new XMLDocument();
	if(int error = file->LoadFile(defFilePath.data()) != 0)
	{
		stringstream sstm;
		sstm << "Error loading definition file. Code: " << error << " File: " << defFilePath;
		UpdateLog(sstm.str(), true);
		return error; //couldn't load the file
	}

	if(strcmp(file->RootElement()->Value(), "HSStage") != 0)
	{
		UpdateLog("XML file is not Homestrife stage definition file: " + defFilePath, true);
		return -1; //this isn't an HSObjects definition file
	}

	XMLElement * root = file->RootElement();

	root->QueryFloatAttribute("width", &(stageSize.x));
	root->QueryFloatAttribute("height", &(stageSize.y));
	root->QueryFloatAttribute("gravity", &(gravity));

	if(stageSize.x < 1920) { stageSize.x = 1920; }
	if(stageSize.y < 1080) { stageSize.y = 1080; }

	//loop through all the sections of the stage definition
	for(XMLElement * i = root->FirstChildElement(); i != NULL; i = i->NextSiblingElement())
	{
		//get the section
		const char * section = i->Value();

		if(strcmp("SpawnPoints", section) == 0)
		{
			for(XMLElement * j = i->FirstChildElement(); j != NULL; j = j->NextSiblingElement())
			{
				if(strcmp("SpawnPoint", j->Value()) != 0)
				{
					continue;
				}
				
				int spPlayer;
				j->QueryIntAttribute("player", &(spPlayer));

				if(spPlayer < 0) { spPlayer = 0; }
				if(spPlayer >= MAX_PLAYERS) { spPlayer = MAX_PLAYERS - 1; }

				j->QueryFloatAttribute("posX", &(spawnPoints[spPlayer].x));
				j->QueryFloatAttribute("posY", &(spawnPoints[spPlayer].y));
			}
		}
		else if(strcmp("Objects", section) == 0)
		{
			for(XMLElement * j = i->FirstChildElement(); j != NULL; j = j->NextSiblingElement())
			{
				if(strcmp("Object", j->Value()) != 0)
				{
					continue;
				}

				HSObject * newObject;
				const char * defFilePath = j->Attribute("defFilePath");

				LoadDefinition(defFilePath, &stageObjects, &newObject);

				j->QueryFloatAttribute("posX", &(newObject->pos.x));
				j->QueryFloatAttribute("posY", &(newObject->pos.y));
				j->QueryFloatAttribute("depth", &(newObject->depth));
			}
		}
	}

	delete file;

	return 0;
}

int ObjectManager::LoadHSMenuCollection(string defFilePath, HSMenu ** returnValue)
{
	//get the XML structure from the file
	XMLDocument * file = new XMLDocument();
	if(int error = file->LoadFile(defFilePath.data()) != 0)
	{
		stringstream sstm;
		sstm << "Error loading definition file. Code: " << error << " File: " << defFilePath;
		UpdateLog(sstm.str(), true);
		return error; //couldn't load the file
	}

	if(strcmp(file->RootElement()->Value(), "HSMenuCollection") != 0)
	{
		UpdateLog("XML file is not Homestrife menu collection definition file: " + defFilePath, true);
		return -1; //this isn't an HSObjects definition file
	}
	
	XMLElement * root = file->RootElement();

	HSVect2D menuPos;
	menuPos.x = 0;
	menuPos.y = 0;
	string topMenuDefFilePath = "";

	if(root->Attribute("topMenuDefFilePath") != NULL) { topMenuDefFilePath = root->Attribute("topMenuDefFilePath"); }
	if(topMenuDefFilePath.empty())
	{
		UpdateLog("Menu collectiion has no top menu definition file path: " + topMenuDefFilePath, true);
		return -1;
	}

	root->QueryFloatAttribute("menuPosX", &menuPos.x);
	root->QueryFloatAttribute("menuPosY", &menuPos.y);

	HSMenu * topMenu = NULL;

	if(int error = LoadHSMenu(topMenuDefFilePath, menuPos, &topMenu) != 0) { return error; }

	if(returnValue != NULL)
	{
		*returnValue = topMenu;
	}

	return 0;
}

int ObjectManager::LoadHSMenu(string defFilePath, HSVect2D menuPos, HSMenu ** returnValue)
{
	//get the XML structure from the file
	XMLDocument * file = new XMLDocument();
	if(int error = file->LoadFile(defFilePath.data()) != 0)
	{
		stringstream sstm;
		sstm << "Error loading definition file. Code: " << error << " File: " << defFilePath;
		UpdateLog(sstm.str(), true);
		return error; //couldn't load the file
	}

	if(strcmp(file->RootElement()->Value(), "HSMenu") != 0)
	{
		UpdateLog("XML file is not Homestrife menu definition file: " + defFilePath, true);
		return -1; //this isn't an HSObjects definition file
	}
	
	XMLElement * root = file->RootElement();

	string titleJustificationString = "";
	TextJustification titleJustification = JUSTIFICATION_LEFT;
	string itemTextJustificationString = "";
	TextJustification itemTextJustification = JUSTIFICATION_LEFT;

	if(root->Attribute("titleJustification") != NULL) { titleJustificationString = root->Attribute("titleJustification"); }
	if(titleJustificationString.compare("RIGHT") == 0) { titleJustification = JUSTIFICATION_RIGHT; }
	else if(titleJustificationString.compare("CENTER") == 0) { titleJustification = JUSTIFICATION_CENTER; }

	if(root->Attribute("itemTextJustification") != NULL) { itemTextJustificationString = root->Attribute("itemTextJustification"); }
	if(itemTextJustificationString.compare("RIGHT") == 0) { itemTextJustification = JUSTIFICATION_RIGHT; }
	else if(itemTextJustificationString.compare("CENTER") == 0) { itemTextJustification = JUSTIFICATION_CENTER; }

	HSFont * titleFont = NULL;
	string titleFontDefFilePath = "";
	if(root->Attribute("titleFontDefFilePath") != NULL) { titleFontDefFilePath = root->Attribute("titleFontDefFilePath"); }
	if(!titleFontDefFilePath.empty())
	{
		if(int error = LoadHSFont(titleFontDefFilePath, &titleFont) != 0) { return error; }
	}

	HSFont * itemFont = NULL;
	string itemFontDefFilePath = "";
	if(root->Attribute("itemFontDefFilePath") != NULL) { itemFontDefFilePath = root->Attribute("itemFontDefFilePath"); }
	if(int error = LoadHSFont(itemFontDefFilePath, &itemFont) != 0) { return error; }
	
	HSObject * cursorToClone;
	string cursorDefFilePath = "";
	if(root->Attribute("cursorDefFilePath") != NULL) { cursorDefFilePath = root->Attribute("cursorDefFilePath"); }
	if(cursorDefFilePath.empty())
	{
		UpdateLog("Menu has no cursor definition file path: " + cursorDefFilePath, true);
		return -1;
	}
		
	if(int error = LoadDefinition(cursorDefFilePath, NULL, &cursorToClone) != 0) { return error; }
	
	HSObject * background = NULL;
	string backgroundDefFilePath = "";
	if(root->Attribute("backgroundDefFilePath") != NULL) { backgroundDefFilePath = root->Attribute("backgroundDefFilePath"); }
	if(backgroundDefFilePath.empty())
	{
		UpdateLog("Menu has no background definition file path: " + backgroundDefFilePath, true);
		return -1;
	}
		
	if(int error = LoadDefinition(backgroundDefFilePath, &HUDObjects, &background) != 0) { return error; }

	HSMenu * newMenu = new HSMenu(titleFont);
	newMenu->justification = titleJustification;
	newMenu->background = background;
	newMenu->background->depth = MENU_BACKGROUND_DEPTH;
	newMenu->cursorToClone = cursorToClone;
	newMenu->cursorToClone->depth = MENU_CURSOR_DEPTH;
	newMenu->itemHeight = itemFont->charHeight;
	newMenu->depth = MENU_TITLE_DEPTH;

	float menuOffsetX = 0;
	float menuOffsetY = 0;
	float backgroundOffsetX = 0;
	float backgroundOffsetY = 0;
	int itemPalette = 0;
	root->QueryFloatAttribute("titleSeparation", &newMenu->titleSeparation);
	root->QueryFloatAttribute("cursorWidth", &newMenu->cursorWidth);
	root->QueryIntAttribute("titlePalette", &newMenu->palette);
	root->QueryIntAttribute("itemPalette", &itemPalette);
	root->QueryFloatAttribute("cursorSeparation", &newMenu->cursorSeparation);
	root->QueryFloatAttribute("itemSeparation", &newMenu->itemSeparation);
	root->QueryFloatAttribute("menuOffsetX", &menuOffsetX);
	root->QueryFloatAttribute("menuOffsetY", &menuOffsetY);
	root->QueryFloatAttribute("backgroundOffsetX", &backgroundOffsetX);
	root->QueryFloatAttribute("backgroundOffsetY", &backgroundOffsetY);

	newMenu->pos.x = menuPos.x + menuOffsetX;
	newMenu->pos.y = menuPos.y + menuOffsetY;
	newMenu->background->pos.x = menuPos.x + menuOffsetX + backgroundOffsetX;
	newMenu->background->pos.y = menuPos.y + menuOffsetY + backgroundOffsetY;

	if(root->Attribute("titleText") != NULL) { newMenu->titleText = root->Attribute("titleText"); }

	float chooserKeySettingOffsetX = 0;
	root->QueryFloatAttribute("chooserKeySettingOffsetX", &chooserKeySettingOffsetX);

	newMenu->RepositionCursor();

	//loop through all the sections of the menu definition
	for(XMLElement * i = root->FirstChildElement(); i != NULL; i = i->NextSiblingElement())
	{
		//get the section
		const char * section = i->Value();

		if(strcmp("MenuItem", section) == 0)
		{
			itemFont = NULL;
			if(int error = LoadHSFont(itemFontDefFilePath, &itemFont) != 0) { return error; }
			HSMenuItem * newItem = new HSMenuItem(itemFont);
			newItem->justification = itemTextJustification;
			newItem->parentMenu = newMenu;
			newItem->palette = itemPalette;

			string function = "";
			if(i->Attribute("function") != NULL) { function = i->Attribute("function"); }

			bool keyConfigItem = false;

			newItem->function = NO_MENU_FUNCTION;
			if(function.compare("BACK") == 0) { newItem->function = BACK; }
			else if(function.compare("QUIT_GAME") == 0) { newItem->function = QUIT_GAME; }
			else if(function.compare("RESUME_MATCH") == 0) { newItem->function = RESUME_MATCH; }
			else if(function.compare("QUIT_MATCH") == 0) { newItem->function = QUIT_MATCH; }
			else if(function.compare("VERSUS") == 0) { newItem->function = VERSUS; }
			else if(function.compare("OPTIONS") == 0) { newItem->function = OPTIONS; }
			else if(function.compare("FREE_FOR_ALL") == 0) { newItem->function = FREE_FOR_ALL; }
			else if(function.compare("FREE_FOR_ALL_2_PLAYERS") == 0) { newItem->function = FREE_FOR_ALL_2_PLAYERS; }
			else if(function.compare("FREE_FOR_ALL_3_PLAYERS") == 0) { newItem->function = FREE_FOR_ALL_3_PLAYERS; }
			else if(function.compare("FREE_FOR_ALL_4_PLAYERS") == 0) { newItem->function = FREE_FOR_ALL_4_PLAYERS; }
			else if(function.compare("VIDEO") == 0) { newItem->function = VIDEO; }
			else if(function.compare("KEY_CONFIG") == 0) { newItem->function = KEY_CONFIG; }
			else if(function.compare("KEY_CONFIG_PLAYER_1") == 0) { newItem->function = KEY_CONFIG_PLAYER_1; }
			else if(function.compare("KEY_CONFIG_PLAYER_2") == 0) { newItem->function = KEY_CONFIG_PLAYER_2; }
			else if(function.compare("KEY_CONFIG_PLAYER_3") == 0) { newItem->function = KEY_CONFIG_PLAYER_3; }
			else if(function.compare("KEY_CONFIG_PLAYER_4") == 0) { newItem->function = KEY_CONFIG_PLAYER_4; }
			else if(function.compare("FULL_SCREEN") == 0) { newItem->function = FULL_SCREEN; }
			else if(function.compare("STRETCH_SCREEN") == 0) { newItem->function = STRETCH_SCREEN; }
			else if(function.compare("FULL_SCREEN_RESOLUTION") == 0) { newItem->function = FULL_SCREEN_RESOLUTION; }
			else if(function.compare("WINDOWED_RESOLUTION") == 0) { newItem->function = WINDOWED_RESOLUTION; }
			else if(function.compare("APPLY_VIDEO_SETTINGS") == 0) { newItem->function = APPLY_VIDEO_SETTINGS; }
			else if(function.compare("KEY_CONFIG_UP") == 0) { newItem->function = KEY_CONFIG_UP; keyConfigItem = true; }
			else if(function.compare("KEY_CONFIG_DOWN") == 0) { newItem->function = KEY_CONFIG_DOWN; keyConfigItem = true; }
			else if(function.compare("KEY_CONFIG_LEFT") == 0) { newItem->function = KEY_CONFIG_LEFT; keyConfigItem = true; }
			else if(function.compare("KEY_CONFIG_RIGHT") == 0) { newItem->function = KEY_CONFIG_RIGHT; keyConfigItem = true; }
			else if(function.compare("KEY_CONFIG_LIGHT_ATTACK") == 0) { newItem->function = KEY_CONFIG_LIGHT_ATTACK; keyConfigItem = true; }
			else if(function.compare("KEY_CONFIG_HEAVY_ATTACK") == 0) { newItem->function = KEY_CONFIG_HEAVY_ATTACK; keyConfigItem = true; }
			else if(function.compare("KEY_CONFIG_JUMP") == 0) { newItem->function = KEY_CONFIG_JUMP; keyConfigItem = true; }
			else if(function.compare("KEY_CONFIG_BLOCK") == 0) { newItem->function = KEY_CONFIG_BLOCK; keyConfigItem = true; }
			else if(function.compare("KEY_CONFIG_PAUSE") == 0) { newItem->function = KEY_CONFIG_PAUSE; keyConfigItem = true; }
			else if(function.compare("KEY_CONFIG_MENU_CONFIRM") == 0) { newItem->function = KEY_CONFIG_MENU_CONFIRM; keyConfigItem = true; }
			else if(function.compare("KEY_CONFIG_MENU_BACK") == 0) { newItem->function = KEY_CONFIG_MENU_BACK; keyConfigItem = true; }
			
			if(i->Attribute("itemText") != NULL) { newItem->itemText = i->Attribute("itemText"); }

			i->QueryUnsignedAttribute("order", &newItem->orderID);
			
			newItem->depth = MENU_ITEM_DEPTH;

			newItem->child = NULL;
			string childMenuDefFilePath = "";
			if(i->Attribute("childMenuDefFilePath") != NULL) { childMenuDefFilePath = i->Attribute("childMenuDefFilePath"); }
			if(!childMenuDefFilePath.empty())
			{
				HSMenu * childMenu;

				LoadHSMenu(childMenuDefFilePath, menuPos, &childMenu);

				childMenu->parentMenuItem = newItem;
				newItem->child = childMenu;
			}

			newItem->chooser = NULL;
			string chooserDefFilePath = "";
			if(i->Attribute("chooserDefFilePath") != NULL) { chooserDefFilePath = i->Attribute("chooserDefFilePath"); }
			if(!chooserDefFilePath.empty())
			{
				MenuChooser * chooser;
				
				HSFont * chooserFont = NULL;
				if(int error = LoadHSFont(itemFontDefFilePath, &chooserFont) != 0) { return error; }
				LoadMenuChooser(chooserDefFilePath, chooserFont, &chooser);

				chooser->parentMenuItem = newItem;

				switch(newItem->function)
				{
				case FULL_SCREEN:
					if(fullScreen) { chooser->SetByChoiceFunction(YES); }
					else { chooser->SetByChoiceFunction(NO); }
					break;
				case STRETCH_SCREEN:
					if(stretchScreen) { chooser->SetByChoiceFunction(YES); }
					else { chooser->SetByChoiceFunction(NO); }
					break;
				case FULL_SCREEN_RESOLUTION:
					if(fullscreenResolutionX == 1920) { chooser->SetByChoiceFunction(RES1920X1080); }
					else if(fullscreenResolutionX == 1680) { chooser->SetByChoiceFunction(RES1680X945); }
					else if(fullscreenResolutionX == 1600) { chooser->SetByChoiceFunction(RES1600X900); }
					else if(fullscreenResolutionX == 1440) { chooser->SetByChoiceFunction(RES1440X810); }
					else if(fullscreenResolutionX == 1400) { chooser->SetByChoiceFunction(RES1400X787); }
					else if(fullscreenResolutionX == 1366) { chooser->SetByChoiceFunction(RES1366X768); }
					else if(fullscreenResolutionX == 1360) { chooser->SetByChoiceFunction(RES1360X765); }
					else if(fullscreenResolutionX == 1280) { chooser->SetByChoiceFunction(RES1280X720); }
					else if(fullscreenResolutionX == 1152) { chooser->SetByChoiceFunction(RES1152X648); }
					else if(fullscreenResolutionX == 1024) { chooser->SetByChoiceFunction(RES1024X576); }
					else if(fullscreenResolutionX == 800) { chooser->SetByChoiceFunction(RES800X450); }
					else if(fullscreenResolutionX == 640) { chooser->SetByChoiceFunction(RES640X360); }
					break;
				case WINDOWED_RESOLUTION:
					if(windowedResolutionX == 1920) { chooser->SetByChoiceFunction(RES1920X1080); }
					else if(windowedResolutionX == 1680) { chooser->SetByChoiceFunction(RES1680X945); }
					else if(windowedResolutionX == 1600) { chooser->SetByChoiceFunction(RES1600X900); }
					else if(windowedResolutionX == 1440) { chooser->SetByChoiceFunction(RES1440X810); }
					else if(windowedResolutionX == 1400) { chooser->SetByChoiceFunction(RES1400X787); }
					else if(windowedResolutionX == 1366) { chooser->SetByChoiceFunction(RES1366X768); }
					else if(windowedResolutionX == 1360) { chooser->SetByChoiceFunction(RES1360X765); }
					else if(windowedResolutionX == 1280) { chooser->SetByChoiceFunction(RES1280X720); }
					else if(windowedResolutionX == 1152) { chooser->SetByChoiceFunction(RES1152X648); }
					else if(windowedResolutionX == 1024) { chooser->SetByChoiceFunction(RES1024X576); }
					else if(windowedResolutionX == 800) { chooser->SetByChoiceFunction(RES800X450); }
					else if(windowedResolutionX == 640) { chooser->SetByChoiceFunction(RES640X360); }
					break;
				}

				newItem->chooser = chooser;
			}

			newItem->currentKeySetting = NULL;
			if(keyConfigItem)
			{
				MenuKeySetting * keysetting;
				
				HSFont * keySettingFont = NULL;
				if(int error = LoadHSFont(itemFontDefFilePath, &keySettingFont) != 0) { return error; }
				CreateMenuKeySetting(keySettingFont, &keysetting);

				keysetting->parentMenuItem = newItem;

				newItem->currentKeySetting = keysetting;
			}

			newMenu->items.push_back(newItem);
		}
	}

	newMenu->items.sort(HSOrderablePointerSort);

	delete file;
	
	HSVect2D textPos;
	textPos.x = newMenu->pos.x + newMenu->cursorWidth + newMenu->cursorSeparation;
	textPos.y = newMenu->pos.y + newMenu->titleSeparation + newMenu->itemHeight;

	list<HSMenuItem*>::iterator itIt;
	for(itIt = newMenu->items.begin(); itIt != newMenu->items.end(); itIt++)
	{
		(*itIt)->pos.x = textPos.x;
		(*itIt)->pos.y = textPos.y;

		if((*itIt)->chooser != NULL)
		{
			(*itIt)->chooser->pos.x = textPos.x + chooserKeySettingOffsetX;
			(*itIt)->chooser->pos.y = textPos.y;
		}

		if((*itIt)->currentKeySetting != NULL)
		{
			(*itIt)->currentKeySetting->pos.x = textPos.x + chooserKeySettingOffsetX;
			(*itIt)->currentKeySetting->pos.y = textPos.y;
		}

		textPos.y += itemFont->charHeight + newMenu->itemSeparation;
	}

	if(returnValue != NULL)
	{
		*returnValue = newMenu;
	}

	return 0;
}

int ObjectManager::LoadMenuChooser(string defFilePath, HSFont * font, MenuChooser ** returnValue)
{
	//get the XML structure from the file
	XMLDocument * file = new XMLDocument();
	if(int error = file->LoadFile(defFilePath.data()) != 0)
	{
		stringstream sstm;
		sstm << "Error loading definition file. Code: " << error << " File: " << defFilePath;
		UpdateLog(sstm.str(), true);
		return error; //couldn't load the file
	}

	if(strcmp(file->RootElement()->Value(), "HSChooser") != 0)
	{
		UpdateLog("XML file is not Homestrife chooser definition file: " + defFilePath, true);
		return -1; //this isn't an HSObjects definition file
	}

	XMLElement * root = file->RootElement();

	MenuChooser * newChooser = new MenuChooser(font);

	//loop through all the sections of the font definition
	for(XMLElement * i = root->FirstChildElement(); i != NULL; i = i->NextSiblingElement())
	{
		//get the section
		const char * section = i->Value();

		if(strcmp("ChooserItem", section) != 0) { continue; }

		MenuChooserItem * newItem = new MenuChooserItem();
		newItem->parentChooser = newChooser;
		
		i->QueryUnsignedAttribute("order", &newItem->orderID);
		string function = "";
		if(i->Attribute("function") != NULL) { function = i->Attribute("function"); }
		
		newItem->function = NO_CHOOSER_FUNCTION;
		if(function.compare("YES") == 0) { newItem->function = YES; newItem->itemText = "Yes"; }
		else if(function.compare("NO") == 0) { newItem->function = NO; newItem->itemText = "No"; }
		else if(function.compare("RES640X360") == 0) { newItem->function = RES640X360; newItem->itemText = "640x360"; }
		else if(function.compare("RES800X450") == 0) { newItem->function = RES800X450; newItem->itemText = "800x450"; }
		else if(function.compare("RES1024X576") == 0) { newItem->function = RES1024X576; newItem->itemText = "1024x576"; }
		else if(function.compare("RES1152X648") == 0) { newItem->function = RES1152X648; newItem->itemText = "1152x648"; }
		else if(function.compare("RES1280X720") == 0) { newItem->function = RES1280X720; newItem->itemText = "1280x720"; }
		else if(function.compare("RES1360X765") == 0) { newItem->function = RES1360X765; newItem->itemText = "1360x765"; }
		else if(function.compare("RES1366X768") == 0) { newItem->function = RES1366X768; newItem->itemText = "1366x768"; }
		else if(function.compare("RES1400X787") == 0) { newItem->function = RES1400X787; newItem->itemText = "1400x787"; }
		else if(function.compare("RES1440X810") == 0) { newItem->function = RES1440X810; newItem->itemText = "1440x810"; }
		else if(function.compare("RES1600X900") == 0) { newItem->function = RES1600X900; newItem->itemText = "1600x900"; }
		else if(function.compare("RES1680X945") == 0) { newItem->function = RES1680X945; newItem->itemText = "1680x945"; }
		else if(function.compare("RES1920X1080") == 0) { newItem->function = RES1920X1080; newItem->itemText = "1920x1080"; }

		newChooser->items.push_back(newItem);
	}

	delete file;

	newChooser->items.sort(HSOrderablePointerSort);

	if(returnValue != NULL)
	{
		*returnValue = newChooser;
	}

	return 0;
}

int ObjectManager::CreateMenuKeySetting(HSFont * font, MenuKeySetting ** returnValue)
{
	MenuKeySetting * newKeySetting = new MenuKeySetting(font);

	//keyboard
	MenuKeySettingItem * keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_BACKSPACE; keySettingItem->itemText = "Backspace"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_TAB; keySettingItem->itemText = "Tab"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_CLEAR; keySettingItem->itemText = "Clear"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_PAUSE; keySettingItem->itemText = "Pause"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_SPACE; keySettingItem->itemText = "Space"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_EXCLAIM; keySettingItem->itemText = "!"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_QUOTEDBL; keySettingItem->itemText = "\""; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_HASH; keySettingItem->itemText = "#"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_DOLLAR; keySettingItem->itemText = "$"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_AMPERSAND; keySettingItem->itemText = "&"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_QUOTE; keySettingItem->itemText = "'"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_LEFTPAREN; keySettingItem->itemText = "("; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_RIGHTPAREN; keySettingItem->itemText = ")"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_ASTERISK; keySettingItem->itemText = "*"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_PLUS; keySettingItem->itemText = "+"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_COMMA; keySettingItem->itemText = ","; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_MINUS; keySettingItem->itemText = "-"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_PERIOD; keySettingItem->itemText = "."; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_SLASH; keySettingItem->itemText = "/"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_0; keySettingItem->itemText = "0"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_1; keySettingItem->itemText = "1"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_2; keySettingItem->itemText = "2"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_3; keySettingItem->itemText = "3"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_4; keySettingItem->itemText = "4"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_5; keySettingItem->itemText = "5"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_6; keySettingItem->itemText = "6"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_7; keySettingItem->itemText = "7"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_8; keySettingItem->itemText = "8"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_9; keySettingItem->itemText = "9"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_COLON; keySettingItem->itemText = ":"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_SEMICOLON; keySettingItem->itemText = ";"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_LESS; keySettingItem->itemText = "<"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_EQUALS; keySettingItem->itemText = "="; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_GREATER; keySettingItem->itemText = ">"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_QUESTION; keySettingItem->itemText = "?"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_AT; keySettingItem->itemText = "@"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_LEFTBRACKET; keySettingItem->itemText = "["; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_BACKSLASH; keySettingItem->itemText = "\\"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_RIGHTBRACKET; keySettingItem->itemText = "]"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_CARET; keySettingItem->itemText = "^"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_UNDERSCORE; keySettingItem->itemText = "_"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_BACKQUOTE; keySettingItem->itemText = "`"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_a; keySettingItem->itemText = "A"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_b; keySettingItem->itemText = "B"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_c; keySettingItem->itemText = "C"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_d; keySettingItem->itemText = "D"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_e; keySettingItem->itemText = "E"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_f; keySettingItem->itemText = "F"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_g; keySettingItem->itemText = "G"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_h; keySettingItem->itemText = "H"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_i; keySettingItem->itemText = "I"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_j; keySettingItem->itemText = "J"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_k; keySettingItem->itemText = "K"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_l; keySettingItem->itemText = "L"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_m; keySettingItem->itemText = "M"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_n; keySettingItem->itemText = "N"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_o; keySettingItem->itemText = "O"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_p; keySettingItem->itemText = "P"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_q; keySettingItem->itemText = "Q"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_r; keySettingItem->itemText = "R"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_s; keySettingItem->itemText = "S"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_t; keySettingItem->itemText = "T"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_u; keySettingItem->itemText = "U"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_v; keySettingItem->itemText = "V"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_w; keySettingItem->itemText = "W"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_x; keySettingItem->itemText = "X"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_y; keySettingItem->itemText = "Y"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_z; keySettingItem->itemText = "Z"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_DELETE; keySettingItem->itemText = "Delete"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_KP_0; keySettingItem->itemText = "Numpad 0"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_KP_1; keySettingItem->itemText = "Numpad 1"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_KP_2; keySettingItem->itemText = "Numpad 2"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_KP_3; keySettingItem->itemText = "Numpad 3"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_KP_4; keySettingItem->itemText = "Numpad 4"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_KP_5; keySettingItem->itemText = "Numpad 5"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_KP_6; keySettingItem->itemText = "Numpad 6"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_KP_7; keySettingItem->itemText = "Numpad 7"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_KP_8; keySettingItem->itemText = "Numpad 8"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_KP_9; keySettingItem->itemText = "Numpad 9"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_KP_PERIOD; keySettingItem->itemText = "Numpad ."; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_KP_DIVIDE; keySettingItem->itemText = "Numpad /"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_KP_MULTIPLY; keySettingItem->itemText = "Numpad *"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_KP_MINUS; keySettingItem->itemText = "Numpad -"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_KP_PLUS; keySettingItem->itemText = "Numpad +"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_KP_EQUALS; keySettingItem->itemText = "Numpad ="; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_INSERT; keySettingItem->itemText = "Insert"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_HOME; keySettingItem->itemText = "Home"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_END; keySettingItem->itemText = "End"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_PAGEUP; keySettingItem->itemText = "Page Up"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_PAGEDOWN; keySettingItem->itemText = "Page Down"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_RSHIFT; keySettingItem->itemText = "Right Shift"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_LSHIFT; keySettingItem->itemText = "Left Shift"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_RCTRL; keySettingItem->itemText = "Right Ctrl"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_LCTRL; keySettingItem->itemText = "Left Ctrl"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_RALT; keySettingItem->itemText = "Right Alt"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_KEY;
	keySettingItem->keySetting.keycode = SDLK_LALT; keySettingItem->itemText = "Left Alt"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	//Joystick 0 buttons
	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 0; keySettingItem->keySetting.joystickMapping.button = 0; keySettingItem->itemText = "Joy 0 Button 0";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 0; keySettingItem->keySetting.joystickMapping.button = 1; keySettingItem->itemText = "Joy 0 Button 1";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 0; keySettingItem->keySetting.joystickMapping.button = 2; keySettingItem->itemText = "Joy 0 Button 2";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 0; keySettingItem->keySetting.joystickMapping.button = 3; keySettingItem->itemText = "Joy 0 Button 3";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 0; keySettingItem->keySetting.joystickMapping.button = 4; keySettingItem->itemText = "Joy 0 Button 4";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 0; keySettingItem->keySetting.joystickMapping.button = 5; keySettingItem->itemText = "Joy 0 Button 5";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 0; keySettingItem->keySetting.joystickMapping.button = 6; keySettingItem->itemText = "Joy 0 Button 6";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 0; keySettingItem->keySetting.joystickMapping.button = 7; keySettingItem->itemText = "Joy 0 Button 7";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 0; keySettingItem->keySetting.joystickMapping.button = 8; keySettingItem->itemText = "Joy 0 Button 8";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 0; keySettingItem->keySetting.joystickMapping.button = 9; keySettingItem->itemText = "Joy 0 Button 9";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 0; keySettingItem->keySetting.joystickMapping.button = 10; keySettingItem->itemText = "Joy 0 Button 10";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 0; keySettingItem->keySetting.joystickMapping.button = 11; keySettingItem->itemText = "Joy 0 Button 11";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 0; keySettingItem->keySetting.joystickMapping.button = 12; keySettingItem->itemText = "Joy 0 Button 12";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 0; keySettingItem->keySetting.joystickMapping.button = 13; keySettingItem->itemText = "Joy 0 Button 13";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 0; keySettingItem->keySetting.joystickMapping.button = 14; keySettingItem->itemText = "Joy 0 Button 14";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 0; keySettingItem->keySetting.joystickMapping.button = 15; keySettingItem->itemText = "Joy 0 Button 15";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 0; keySettingItem->keySetting.joystickMapping.button = 16; keySettingItem->itemText = "Joy 0 Button 16";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 0; keySettingItem->keySetting.joystickMapping.button = 17; keySettingItem->itemText = "Joy 0 Button 17";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 0; keySettingItem->keySetting.joystickMapping.button = 18; keySettingItem->itemText = "Joy 0 Button 18";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 0; keySettingItem->keySetting.joystickMapping.button = 19; keySettingItem->itemText = "Joy 0 Button 19";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	//Joystick 1 buttons
	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 1; keySettingItem->keySetting.joystickMapping.button = 0; keySettingItem->itemText = "Joy 1 Button 0";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 1; keySettingItem->keySetting.joystickMapping.button = 1; keySettingItem->itemText = "Joy 1 Button 1";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 1; keySettingItem->keySetting.joystickMapping.button = 2; keySettingItem->itemText = "Joy 1 Button 2";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 1; keySettingItem->keySetting.joystickMapping.button = 3; keySettingItem->itemText = "Joy 1 Button 3";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 1; keySettingItem->keySetting.joystickMapping.button = 4; keySettingItem->itemText = "Joy 1 Button 4";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 1; keySettingItem->keySetting.joystickMapping.button = 5; keySettingItem->itemText = "Joy 1 Button 5";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 1; keySettingItem->keySetting.joystickMapping.button = 6; keySettingItem->itemText = "Joy 1 Button 6";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 1; keySettingItem->keySetting.joystickMapping.button = 7; keySettingItem->itemText = "Joy 1 Button 7";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 1; keySettingItem->keySetting.joystickMapping.button = 8; keySettingItem->itemText = "Joy 1 Button 8";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 1; keySettingItem->keySetting.joystickMapping.button = 9; keySettingItem->itemText = "Joy 1 Button 9";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 1; keySettingItem->keySetting.joystickMapping.button = 10; keySettingItem->itemText = "Joy 1 Button 10";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 1; keySettingItem->keySetting.joystickMapping.button = 11; keySettingItem->itemText = "Joy 1 Button 11";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 1; keySettingItem->keySetting.joystickMapping.button = 12; keySettingItem->itemText = "Joy 1 Button 12";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 1; keySettingItem->keySetting.joystickMapping.button = 13; keySettingItem->itemText = "Joy 1 Button 13";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 1; keySettingItem->keySetting.joystickMapping.button = 14; keySettingItem->itemText = "Joy 1 Button 14";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 1; keySettingItem->keySetting.joystickMapping.button = 15; keySettingItem->itemText = "Joy 1 Button 15";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 1; keySettingItem->keySetting.joystickMapping.button = 16; keySettingItem->itemText = "Joy 1 Button 16";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 1; keySettingItem->keySetting.joystickMapping.button = 17; keySettingItem->itemText = "Joy 1 Button 17";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 1; keySettingItem->keySetting.joystickMapping.button = 18; keySettingItem->itemText = "Joy 1 Button 18";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 1; keySettingItem->keySetting.joystickMapping.button = 19; keySettingItem->itemText = "Joy 1 Button 19";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	//Joystick 2 buttons
	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 2; keySettingItem->keySetting.joystickMapping.button = 0; keySettingItem->itemText = "Joy 2 Button 0";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 2; keySettingItem->keySetting.joystickMapping.button = 1; keySettingItem->itemText = "Joy 2 Button 1";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 2; keySettingItem->keySetting.joystickMapping.button = 2; keySettingItem->itemText = "Joy 2 Button 2";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 2; keySettingItem->keySetting.joystickMapping.button = 3; keySettingItem->itemText = "Joy 2 Button 3";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 2; keySettingItem->keySetting.joystickMapping.button = 4; keySettingItem->itemText = "Joy 2 Button 4";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 2; keySettingItem->keySetting.joystickMapping.button = 5; keySettingItem->itemText = "Joy 2 Button 5";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 2; keySettingItem->keySetting.joystickMapping.button = 6; keySettingItem->itemText = "Joy 2 Button 6";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 2; keySettingItem->keySetting.joystickMapping.button = 7; keySettingItem->itemText = "Joy 2 Button 7";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 2; keySettingItem->keySetting.joystickMapping.button = 8; keySettingItem->itemText = "Joy 2 Button 8";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 2; keySettingItem->keySetting.joystickMapping.button = 9; keySettingItem->itemText = "Joy 2 Button 9";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 2; keySettingItem->keySetting.joystickMapping.button = 10; keySettingItem->itemText = "Joy 2 Button 10";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 2; keySettingItem->keySetting.joystickMapping.button = 11; keySettingItem->itemText = "Joy 2 Button 11";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 2; keySettingItem->keySetting.joystickMapping.button = 12; keySettingItem->itemText = "Joy 2 Button 12";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 2; keySettingItem->keySetting.joystickMapping.button = 13; keySettingItem->itemText = "Joy 2 Button 13";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 2; keySettingItem->keySetting.joystickMapping.button = 14; keySettingItem->itemText = "Joy 2 Button 14";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 2; keySettingItem->keySetting.joystickMapping.button = 15; keySettingItem->itemText = "Joy 2 Button 15";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 2; keySettingItem->keySetting.joystickMapping.button = 16; keySettingItem->itemText = "Joy 2 Button 16";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 2; keySettingItem->keySetting.joystickMapping.button = 17; keySettingItem->itemText = "Joy 2 Button 17";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 2; keySettingItem->keySetting.joystickMapping.button = 18; keySettingItem->itemText = "Joy 2 Button 18";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 2; keySettingItem->keySetting.joystickMapping.button = 19; keySettingItem->itemText = "Joy 2 Button 19";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	//Joystick 3 buttons
	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 3; keySettingItem->keySetting.joystickMapping.button = 0; keySettingItem->itemText = "Joy 3 Button 0";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 3; keySettingItem->keySetting.joystickMapping.button = 1; keySettingItem->itemText = "Joy 3 Button 1";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 3; keySettingItem->keySetting.joystickMapping.button = 2; keySettingItem->itemText = "Joy 3 Button 2";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 3; keySettingItem->keySetting.joystickMapping.button = 3; keySettingItem->itemText = "Joy 3 Button 3";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 3; keySettingItem->keySetting.joystickMapping.button = 4; keySettingItem->itemText = "Joy 3 Button 4";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 3; keySettingItem->keySetting.joystickMapping.button = 5; keySettingItem->itemText = "Joy 3 Button 5";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 3; keySettingItem->keySetting.joystickMapping.button = 6; keySettingItem->itemText = "Joy 3 Button 6";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 3; keySettingItem->keySetting.joystickMapping.button = 7; keySettingItem->itemText = "Joy 3 Button 7";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 3; keySettingItem->keySetting.joystickMapping.button = 8; keySettingItem->itemText = "Joy 3 Button 8";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 3; keySettingItem->keySetting.joystickMapping.button = 9; keySettingItem->itemText = "Joy 3 Button 9";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 3; keySettingItem->keySetting.joystickMapping.button = 10; keySettingItem->itemText = "Joy 3 Button 10";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 3; keySettingItem->keySetting.joystickMapping.button = 11; keySettingItem->itemText = "Joy 3 Button 11";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 3; keySettingItem->keySetting.joystickMapping.button = 12; keySettingItem->itemText = "Joy 3 Button 12";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 3; keySettingItem->keySetting.joystickMapping.button = 13; keySettingItem->itemText = "Joy 3 Button 13";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 3; keySettingItem->keySetting.joystickMapping.button = 14; keySettingItem->itemText = "Joy 3 Button 14";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 3; keySettingItem->keySetting.joystickMapping.button = 15; keySettingItem->itemText = "Joy 3 Button 15";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 3; keySettingItem->keySetting.joystickMapping.button = 16; keySettingItem->itemText = "Joy 3 Button 16";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 3; keySettingItem->keySetting.joystickMapping.button = 17; keySettingItem->itemText = "Joy 3 Button 17";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 3; keySettingItem->keySetting.joystickMapping.button = 18; keySettingItem->itemText = "Joy 3 Button 18";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_BUTTON;
	keySettingItem->keySetting.joystickMapping.joystick = 3; keySettingItem->keySetting.joystickMapping.button = 19; keySettingItem->itemText = "Joy 3 Button 19";
	keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	//joystick hats
	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_HAT;
	keySettingItem->keySetting.hat = 0; keySettingItem->itemText = "Joy 0 Hat"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_HAT;
	keySettingItem->keySetting.hat = 1; keySettingItem->itemText = "Joy 1 Hat"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_HAT;
	keySettingItem->keySetting.hat = 2; keySettingItem->itemText = "Joy 2 Hat"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_HAT;
	keySettingItem->keySetting.hat = 3; keySettingItem->itemText = "Joy 3 Hat"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	//joystick sticks
	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_STICK;
	keySettingItem->keySetting.stick = 0; keySettingItem->itemText = "Joy 0 Stick"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_STICK;
	keySettingItem->keySetting.stick = 1; keySettingItem->itemText = "Joy 1 Stick"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_STICK;
	keySettingItem->keySetting.stick = 2; keySettingItem->itemText = "Joy 2 Stick"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_STICK;
	keySettingItem->keySetting.stick = 3; keySettingItem->itemText = "Joy 3 Stick"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	//other
	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_ENTER;
	keySettingItem->itemText = "Enter desired key"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);
	
	keySettingItem = new MenuKeySettingItem(); keySettingItem->keySetting.type = KEY_SETTING_NONE;
	keySettingItem->itemText = "NO SETTING"; keySettingItem->parentKeySetting = newKeySetting;
	newKeySetting->items.push_back(keySettingItem);

	newKeySetting->items.sort(HSOrderablePointerSort);
	newKeySetting->depth = MENU_CHOOSER_DEPTH;

	if(returnValue != NULL)
	{
		*returnValue = newKeySetting;
	}

	return 0;
}

int ObjectManager::LoadHSFont(string defFilePath, HSFont ** returnValue, bool useRegistry)
{
	if(useRegistry)
	{
		list<HSFont*>::iterator fontItr;
		for(fontItr = fontRegistry.begin(); fontItr != fontRegistry.end(); fontItr++)
		{
			if((*fontItr)->fontFilePath.compare(defFilePath) == 0)
			{
				if(returnValue != NULL)
				{
					*returnValue = (*fontItr);
				}

				return 0;
			}
		}
	}

	//get the XML structure from the file
	XMLDocument * file = new XMLDocument();
	if(int error = file->LoadFile(defFilePath.data()) != 0)
	{
		stringstream sstm;
		sstm << "Error loading definition file. Code: " << error << " File: " << defFilePath;
		UpdateLog(sstm.str(), true);
		return error; //couldn't load the file
	}

	if(strcmp(file->RootElement()->Value(), "HSFont") != 0)
	{
		UpdateLog("XML file is not Homestrife font definition file: " + defFilePath, true);
		return -1; //this isn't an HSObjects definition file
	}

	XMLElement * root = file->RootElement();

	HSFont * newFont = new HSFont();
	newFont->fontFilePath = defFilePath;
	float spaceWidth;

	root->QueryFloatAttribute("charHeight", &newFont->charHeight);
	root->QueryFloatAttribute("charSeparation", &newFont->charSeparation);
	root->QueryFloatAttribute("spaceWidth", &spaceWidth);

	HSCharacter space;
	space.character = NULL;
	space.charWidth = spaceWidth;

	newFont->characters.space = space;

	XMLElement * palettesDef = NULL;
	for(XMLElement * partDef = root->FirstChildElement(); partDef != NULL; partDef = partDef->NextSiblingElement())
	{
		if(strcmp(partDef->Value(), "Palettes") == 0) { palettesDef = partDef; }
	}

	list<PaletteInstance> paletteInstances;
	paletteInstances.clear();

	if(palettesDef != NULL)
	{
		for(XMLElement * paletteDef = palettesDef->FirstChildElement(); paletteDef != NULL; paletteDef = paletteDef->NextSiblingElement())
		{
			if(strcmp(paletteDef->Value(), "Palette") != 0)
			{
				continue;
			}

			string palFilePath = "";
			if(paletteDef->Attribute("path") != NULL) { palFilePath = paletteDef->Attribute("path"); }
			PaletteInstance newPalInst;
				
			//see if the palette has already been loaded
			bool alreadyLoaded = false;
			list<HSPalette*>::iterator plIt;
			for ( plIt=paletteRegistry.begin(); plIt != paletteRegistry.end(); plIt++)
			{
				if((*plIt)->paletteFilePath.compare(palFilePath) == 0)
				{
					newPalInst.hsPal = (*plIt);
					(*plIt)->usingCount++;
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
				newPal->textureID = 0;

				//if(int error = LoadHSPToPalette(newPal) != 0) //load the texture
				//{
				//	return error;
				//}

				paletteRegistry.push_back(newPal);
				newPalInst.hsPal = newPal;
			}

			if(paletteDef->Attribute("name") != NULL) { newPalInst.name = paletteDef->Attribute("name"); }
			paletteDef->QueryUnsignedAttribute("id", &newPalInst.orderID);
			paletteInstances.push_back(newPalInst);
		}
	}

	//loop through all the sections of the font definition
	for(XMLElement * i = root->FirstChildElement(); i != NULL; i = i->NextSiblingElement())
	{
		//get the section
		const char * section = i->Value();

		if(strcmp("lcA", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.lcA, paletteInstances) != 0) { return error; } }
		else if(strcmp("lcB", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.lcB, paletteInstances) != 0) { return error; } }
		else if(strcmp("lcC", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.lcC, paletteInstances) != 0) { return error; } }
		else if(strcmp("lcD", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.lcD, paletteInstances) != 0) { return error; } }
		else if(strcmp("lcE", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.lcE, paletteInstances) != 0) { return error; } }
		else if(strcmp("lcF", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.lcF, paletteInstances) != 0) { return error; } }
		else if(strcmp("lcG", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.lcG, paletteInstances) != 0) { return error; } }
		else if(strcmp("lcH", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.lcH, paletteInstances) != 0) { return error; } }
		else if(strcmp("lcI", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.lcI, paletteInstances) != 0) { return error; } }
		else if(strcmp("lcJ", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.lcJ, paletteInstances) != 0) { return error; } }
		else if(strcmp("lcK", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.lcK, paletteInstances) != 0) { return error; } }
		else if(strcmp("lcL", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.lcL, paletteInstances) != 0) { return error; } }
		else if(strcmp("lcM", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.lcM, paletteInstances) != 0) { return error; } }
		else if(strcmp("lcN", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.lcN, paletteInstances) != 0) { return error; } }
		else if(strcmp("lcO", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.lcO, paletteInstances) != 0) { return error; } }
		else if(strcmp("lcP", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.lcP, paletteInstances) != 0) { return error; } }
		else if(strcmp("lcQ", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.lcQ, paletteInstances) != 0) { return error; } }
		else if(strcmp("lcR", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.lcR, paletteInstances) != 0) { return error; } }
		else if(strcmp("lcS", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.lcS, paletteInstances) != 0) { return error; } }
		else if(strcmp("lcT", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.lcT, paletteInstances) != 0) { return error; } }
		else if(strcmp("lcU", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.lcU, paletteInstances) != 0) { return error; } }
		else if(strcmp("lcV", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.lcV, paletteInstances) != 0) { return error; } }
		else if(strcmp("lcW", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.lcW, paletteInstances) != 0) { return error; } }
		else if(strcmp("lcX", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.lcX, paletteInstances) != 0) { return error; } }
		else if(strcmp("lcY", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.lcY, paletteInstances) != 0) { return error; } }
		else if(strcmp("lcZ", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.lcZ, paletteInstances) != 0) { return error; } }
		else if(strcmp("ucA", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.ucA, paletteInstances) != 0) { return error; } }
		else if(strcmp("ucB", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.ucB, paletteInstances) != 0) { return error; } }
		else if(strcmp("ucC", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.ucC, paletteInstances) != 0) { return error; } }
		else if(strcmp("ucD", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.ucD, paletteInstances) != 0) { return error; } }
		else if(strcmp("ucE", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.ucE, paletteInstances) != 0) { return error; } }
		else if(strcmp("ucF", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.ucF, paletteInstances) != 0) { return error; } }
		else if(strcmp("ucG", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.ucG, paletteInstances) != 0) { return error; } }
		else if(strcmp("ucH", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.ucH, paletteInstances) != 0) { return error; } }
		else if(strcmp("ucI", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.ucI, paletteInstances) != 0) { return error; } }
		else if(strcmp("ucJ", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.ucJ, paletteInstances) != 0) { return error; } }
		else if(strcmp("ucK", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.ucK, paletteInstances) != 0) { return error; } }
		else if(strcmp("ucL", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.ucL, paletteInstances) != 0) { return error; } }
		else if(strcmp("ucM", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.ucM, paletteInstances) != 0) { return error; } }
		else if(strcmp("ucN", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.ucN, paletteInstances) != 0) { return error; } }
		else if(strcmp("ucO", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.ucO, paletteInstances) != 0) { return error; } }
		else if(strcmp("ucP", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.ucP, paletteInstances) != 0) { return error; } }
		else if(strcmp("ucQ", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.ucQ, paletteInstances) != 0) { return error; } }
		else if(strcmp("ucR", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.ucR, paletteInstances) != 0) { return error; } }
		else if(strcmp("ucS", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.ucS, paletteInstances) != 0) { return error; } }
		else if(strcmp("ucT", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.ucT, paletteInstances) != 0) { return error; } }
		else if(strcmp("ucU", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.ucU, paletteInstances) != 0) { return error; } }
		else if(strcmp("ucV", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.ucV, paletteInstances) != 0) { return error; } }
		else if(strcmp("ucW", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.ucW, paletteInstances) != 0) { return error; } }
		else if(strcmp("ucX", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.ucX, paletteInstances) != 0) { return error; } }
		else if(strcmp("ucY", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.ucY, paletteInstances) != 0) { return error; } }
		else if(strcmp("ucZ", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.ucZ, paletteInstances) != 0) { return error; } }
		else if(strcmp("num0", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.num0, paletteInstances) != 0) { return error; } }
		else if(strcmp("num1", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.num1, paletteInstances) != 0) { return error; } }
		else if(strcmp("num2", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.num2, paletteInstances) != 0) { return error; } }
		else if(strcmp("num3", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.num3, paletteInstances) != 0) { return error; } }
		else if(strcmp("num4", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.num4, paletteInstances) != 0) { return error; } }
		else if(strcmp("num5", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.num5, paletteInstances) != 0) { return error; } }
		else if(strcmp("num6", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.num6, paletteInstances) != 0) { return error; } }
		else if(strcmp("num7", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.num7, paletteInstances) != 0) { return error; } }
		else if(strcmp("num8", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.num8, paletteInstances) != 0) { return error; } }
		else if(strcmp("num9", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.num9, paletteInstances) != 0) { return error; } }
		else if(strcmp("exclamation", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.exclamation, paletteInstances) != 0) { return error; } }
		else if(strcmp("ampersand", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.ampersand, paletteInstances) != 0) { return error; } }
		else if(strcmp("parenLeft", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.parenLeft, paletteInstances) != 0) { return error; } }
		else if(strcmp("parenRight", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.parenRight, paletteInstances) != 0) { return error; } }
		else if(strcmp("backslash", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.backslash, paletteInstances) != 0) { return error; } }
		else if(strcmp("forwardslash", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.forwardslash, paletteInstances) != 0) { return error; } }
		else if(strcmp("colon", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.colon, paletteInstances) != 0) { return error; } }
		else if(strcmp("quoteSingle", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.quoteSingle, paletteInstances) != 0) { return error; } }
		else if(strcmp("quoteDouble", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.quoteDouble, paletteInstances) != 0) { return error; } }
		else if(strcmp("comma", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.comma, paletteInstances) != 0) { return error; } }
		else if(strcmp("period", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.period, paletteInstances) != 0) { return error; } }
		else if(strcmp("question", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.question, paletteInstances) != 0) { return error; } }
		else if(strcmp("lessThan", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.lessThan, paletteInstances) != 0) { return error; } }
		else if(strcmp("greaterThan", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.greaterThan, paletteInstances) != 0) { return error; } }
		else if(strcmp("at", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.at, paletteInstances) != 0) { return error; } }
		else if(strcmp("hash", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.hash, paletteInstances) != 0) { return error; } }
		else if(strcmp("dollar", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.dollar, paletteInstances) != 0) { return error; } }
		else if(strcmp("percent", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.percent, paletteInstances) != 0) { return error; } }
		else if(strcmp("carat", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.carat, paletteInstances) != 0) { return error; } }
		else if(strcmp("asterisk", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.asterisk, paletteInstances) != 0) { return error; } }
		else if(strcmp("minus", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.minus, paletteInstances) != 0) { return error; } }
		else if(strcmp("underscore", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.underscore, paletteInstances) != 0) { return error; } }
		else if(strcmp("plus", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.plus, paletteInstances) != 0) { return error; } }
		else if(strcmp("equals", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.equals, paletteInstances) != 0) { return error; } }
		else if(strcmp("bracketLeft", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.bracketLeft, paletteInstances) != 0) { return error; } }
		else if(strcmp("bracketRight", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.bracketRight, paletteInstances) != 0) { return error; } }
		else if(strcmp("semicolon", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.semicolon, paletteInstances) != 0) { return error; } }
		else if(strcmp("quoteBack", section) == 0) { if(int error = LoadHSCharacter(i, &newFont->characters.quoteBack, paletteInstances) != 0) { return error; } }
	}

	delete file;

	if(returnValue != NULL)
	{
		*returnValue = newFont;
	}
	
	if(useRegistry)
	{
		fontRegistry.push_back(newFont);
	}

	return 0;
}

int ObjectManager::LoadHSCharacter(XMLElement * xml, HSCharacter * hsChar, list<PaletteInstance> paletteInstances)
{
	if(hsChar == NULL) { return 0; }

	xml->QueryFloatAttribute("charWidth", &hsChar->charWidth);

	HSObject * newObject;
	string defFilePath = "";
	if(xml->Attribute("defFilePath") != NULL) { defFilePath = xml->Attribute("defFilePath"); }

	if(defFilePath.empty()) { return 0; }

	if(int error = LoadDefinition(defFilePath, NULL, &newObject) != 0) { return error; }

	//add the font palettes to the character object
	list<PaletteInstance>::iterator piItr;
	for(piItr = paletteInstances.begin(); piItr != paletteInstances.end(); piItr++)
	{
		newObject->palettes.push_back(*piItr);
	}
	newObject->palettes.sort(HSOrderableSort);
	newObject->SetPalette(0);

	hsChar->character = newObject;

	return 0;
}

int ObjectManager::LoadLoadingScreen(string defFilePath)
{
	//load loading screen
	//get the XML structure from the file
	XMLDocument * file = new XMLDocument();
	if(int error = file->LoadFile(defFilePath.data()) != 0)
	{
		stringstream sstm;
		sstm << "Error loading definition file. Code: " << error << " File: " << defFilePath;
		UpdateLog(sstm.str(), true);
		return error; //couldn't load the file
	}

	if(strcmp(file->RootElement()->Value(), "HSLoadingScreen") != 0)
	{
		UpdateLog("XML file is not Homestrife loading screen definition file: " + defFilePath, true);
		return -1;
	}

	XMLElement * root = file->RootElement();
	
	TextJustification loadingTextJustification = JUSTIFICATION_LEFT;
	string backgroundDefFilePath = "";
	float backgroundPosX = 0;
	float backgroundPosY = 0;
	string loadingTextFontFilePath = "";
	float loadingTextPosX = 0;
	float loadingTextPosY = 0;
	int loadingTextPalette = 0;

	backgroundDefFilePath = root->Attribute("backgroundDefFilePath");
	root->QueryFloatAttribute("backgroundPosX", &backgroundPosX);
	root->QueryFloatAttribute("backgroundPosY", &backgroundPosY);
	loadingTextFontFilePath = root->Attribute("loadingTextFontFilePath");
	root->QueryFloatAttribute("loadingTextPosX", &loadingTextPosX);
	root->QueryFloatAttribute("loadingTextPosY", &loadingTextPosY);
	root->QueryIntAttribute("loadingTextPalette", &loadingTextPalette);

	string loadingTextJustificationString = "";
	if(root->Attribute("loadingTextJustification") != NULL) { loadingTextJustificationString = root->Attribute("loadingTextJustification"); }
	if(loadingTextJustificationString.compare("RIGHT") == 0) { loadingTextJustification = JUSTIFICATION_RIGHT; }
	else if(loadingTextJustificationString.compare("CENTER") == 0) { loadingTextJustification = JUSTIFICATION_CENTER; }

	if(int error = LoadHSFont(loadingTextFontFilePath, &loadingFont, false) != 0) { return error; }
	if(int error = LoadDefinition(backgroundDefFilePath, &HUDObjects, &loadingBackground) != 0) { return error; }
	loadingBackground->pos.x = backgroundPosX;
	loadingBackground->pos.y = backgroundPosY;
	loadingBackground->depth = LOADING_BACKGROUND_DEPTH;

	loadingText = new HSText(loadingFont);
	loadingText->pos.x = loadingTextPosX;
	loadingText->pos.y = loadingTextPosY;
	loadingText->depth = LOADING_TEXT_DEPTH;
	loadingText->justification = loadingTextJustification;
	loadingText->palette = loadingTextPalette;

	return 0;
}

int ObjectManager::LoadCharacterSelect(string defFilePath, string pcFilePath, string psFilePath, string pmFilePath)
{
	if(characterSelectManager == NULL) { return -1; }

	//load character select
	//get the XML structure from the file
	XMLDocument * file = new XMLDocument();
	if(int error = file->LoadFile(defFilePath.data()) != 0)
	{
		stringstream sstm;
		sstm << "Error loading definition file. Code: " << error << " File: " << defFilePath;
		UpdateLog(sstm.str(), true);
		return error; //couldn't load the file
	}

	if(strcmp(file->RootElement()->Value(), "HSCharacterSelect") != 0)
	{
		UpdateLog("XML file is not Homestrife character select definition file: " + defFilePath, true);
		return -1;
	}

	XMLElement * root = file->RootElement();

	//loop through all the sections of the character select
	for(XMLElement * i = root->FirstChildElement(); i != NULL; i = i->NextSiblingElement())
	{
		if(strcmp(i->Value(), "CharacterSelect") == 0)
		{
			CharacterSelect * cs = new CharacterSelect();
			characterSelectManager->characterSelect = cs;

			HSFont * titleFont = NULL;
			TextJustification titleJustification = JUSTIFICATION_LEFT;
			float titlePosX = 0;
			float titlePosY = 0;
			float backgroundPosX = 0;
			float backgroundPosY = 0;
			float gridPosX = 0;
			float gridPosY = 0;
			int gridRows = 1;
			int gridColumns = 1;
			float panelWidth = 0;
			float panelHeight = 0;
			float panelBorderOffsetX = 0;
			float panelBorderOffsetY = 0;
			float panelImageOffsetX = 0;
			float panelImageOffsetY = 0;
			float portraitBorderOffsetX = 0;
			float portraitBorderOffsetY = 0;
			int titlePalette = 0;
			int characterNamePalette = 0;
			int instructionsPalette = 0;
	
			string backgroundFilePath = i->Attribute("backgroundFilePath");
			i->QueryFloatAttribute("backgroundPosX", &backgroundPosX);
			i->QueryFloatAttribute("backgroundPosY", &backgroundPosY);
			string characterSelectTitle = i->Attribute("titleText");
			string selectCharacterText = i->Attribute("selectCharacterText");
			string selectPaletteText = i->Attribute("selectPaletteText");
			string readyText = i->Attribute("readyText");
			string titleFontFilePath = i->Attribute("titleFontFilePath");
			i->QueryFloatAttribute("titlePosX", &titlePosX);
			i->QueryFloatAttribute("titlePosY", &titlePosY);
			i->QueryFloatAttribute("gridPosX", &gridPosX);
			i->QueryFloatAttribute("gridPosY", &gridPosY);
			i->QueryIntAttribute("gridRows", &gridRows);
			i->QueryIntAttribute("gridColumns", &gridColumns);
			i->QueryFloatAttribute("panelWidth", &panelWidth);
			i->QueryFloatAttribute("panelHeight", &panelHeight);
			i->QueryFloatAttribute("panelBorderOffsetX", &panelBorderOffsetX);
			i->QueryFloatAttribute("panelBorderOffsetY", &panelBorderOffsetY);
			i->QueryFloatAttribute("panelImageOffsetX", &panelImageOffsetX);
			i->QueryFloatAttribute("panelImageOffsetY", &panelImageOffsetY);
			string panelBorderFilePath = i->Attribute("panelBorderFilePath");
			i->QueryFloatAttribute("portraitBorderOffsetX", &portraitBorderOffsetX);
			i->QueryFloatAttribute("portraitBorderOffsetY", &portraitBorderOffsetY);
			string characterNameFontFilePath = i->Attribute("characterNameFontFilePath");
			string instructionsFontFilePath = i->Attribute("instructionsFontFilePath");
			i->QueryIntAttribute("titlePalette", &titlePalette);
			i->QueryIntAttribute("characterNamePalette", &characterNamePalette);
			i->QueryIntAttribute("instructionsPalette", &instructionsPalette);

			string titleJustificationString = "";
			if(i->Attribute("titleJustification") != NULL) { titleJustificationString = i->Attribute("titleJustification"); }
			if(titleJustificationString.compare("RIGHT") == 0) { titleJustification = JUSTIFICATION_RIGHT; }
			else if(titleJustificationString.compare("CENTER") == 0) { titleJustification = JUSTIFICATION_CENTER; }
	
			HSObject * newObject;
			if(int error = LoadDefinition(backgroundFilePath, &HUDObjects, &newObject) != 0) { return error; }
			newObject->pos.x = backgroundPosX;
			newObject->pos.y = backgroundPosY;
			newObject->depth = CHARACTER_SELECT_BACKGROUND_DEPTH;

			if(int error = LoadHSFont(titleFontFilePath, &titleFont) != 0) { return error; }

			cs->title = new HSText(titleFont);
			cs->title->justification = titleJustification;
			cs->title->pos.x = titlePosX;
			cs->title->pos.y = titlePosY;
			cs->title->depth = CHARACTER_SELECT_TITLE_DEPTH;
			cs->title->palette = titlePalette;
			cs->characterSelectBackground = newObject;
			cs->characterSelectTitle = characterSelectTitle;
			cs->selectCharacterText = selectCharacterText;
			cs->selectPaletteText = selectPaletteText;
			cs->readyText = readyText;
			cs->gridPos.x = gridPosX;
			cs->gridPos.y = gridPosY;
			cs->rows = gridRows;
			cs->columns = gridColumns;
			cs->panelWidth = panelWidth;
			cs->panelHeight = panelHeight;
			cs->panelBorderOffset.x = panelBorderOffsetX;
			cs->panelBorderOffset.y = panelBorderOffsetY;
			cs->panelImageOffset.x = panelImageOffsetX;
			cs->panelImageOffset.y = panelImageOffsetY;
			cs->panelBorderDepth = CHARACTER_SELECT_PANEL_BORDER_DEPTH;
			cs->panelImageDepth = CHARACTER_SELECT_PANEL_DEPTH;
			cs->portraitDepth = CHARACTER_SELECT_PORTRAIT_DEPTH;

			//look for the character portraits element
			for(XMLElement * j = i->FirstChildElement(); j != NULL; j = j->NextSiblingElement())
			{
				if(strcmp(j->Value(), "CharacterSelectPlayers") != 0)
				{
					continue;
				}

				//loop through the character portraits
				for(XMLElement * k = j->FirstChildElement(); k != NULL; k = k->NextSiblingElement())
				{
					if(strcmp(k->Value(), "CharacterSelectPlayer") != 0)
					{
						continue;
					}

					int playerNum;
					float portraitPosX;
					float portraitPosY;
					float characterNamePosX;
					float characterNamePosY;
					float instructionsPosX;
					float instructionsPosY;
					HSFont * characterNameFont;
					TextJustification characterNameJustification = JUSTIFICATION_LEFT;
					HSFont * instructionsFont;
					TextJustification instructionJustification = JUSTIFICATION_LEFT;

					k->QueryIntAttribute("player", &playerNum);
					k->QueryFloatAttribute("portraitPosX", &portraitPosX);
					k->QueryFloatAttribute("portraitPosY", &portraitPosY);
					k->QueryFloatAttribute("characterNamePosX", &characterNamePosX);
					k->QueryFloatAttribute("characterNamePosY", &characterNamePosY);
					k->QueryFloatAttribute("instructionsPosX", &instructionsPosX);
					k->QueryFloatAttribute("instructionsPosY", &instructionsPosY);
					string cursorFilePath = k->Attribute("cursorFilePath");
					string portraitBorderFilePath = k->Attribute("portraitBorderFilePath");

					string characterNameJustificationString = "";
					if(k->Attribute("characterNameJustification") != NULL) { characterNameJustificationString = k->Attribute("characterNameJustification"); }
					if(characterNameJustificationString.compare("RIGHT") == 0) { characterNameJustification = JUSTIFICATION_RIGHT; }
					else if(characterNameJustificationString.compare("CENTER") == 0) { characterNameJustification = JUSTIFICATION_CENTER; }

					string instructionJustificationString = "";
					if(k->Attribute("instructionsJustification") != NULL) { instructionJustificationString = k->Attribute("instructionsJustification"); }
					if(instructionJustificationString.compare("RIGHT") == 0) { instructionJustification = JUSTIFICATION_RIGHT; }
					else if(instructionJustificationString.compare("CENTER") == 0) { instructionJustification = JUSTIFICATION_CENTER; }

					if(playerNum < 0 || playerNum >= MAX_PLAYERS) { continue; }

					if(int error = LoadHSFont(characterNameFontFilePath, &characterNameFont) != 0) { return error; }
					if(int error = LoadHSFont(instructionsFontFilePath, &instructionsFont) != 0) { return error; }

					CharacterSelectCursor * newCursor = new CharacterSelectCursor();
					if(int error = LoadDefinition(cursorFilePath, &HUDObjects, &newObject) != 0) { return error; }
					newCursor->cursorImage = newObject;
					newCursor->currentPanel = 0;
					newCursor->cursorImage->depth = CHARACTER_SELECT_CURSOR_DEPTH;
				
					if(int error = LoadDefinition(portraitBorderFilePath, &HUDObjects, &newObject) != 0) { return error; }
					newObject->pos.x = portraitPosX + portraitBorderOffsetX;
					newObject->pos.y = portraitPosY + portraitBorderOffsetY;
					newObject->depth = CHARACTER_SELECT_PORTRAIT_BORDER_DEPTH;
				
					cs->portraitPos[playerNum].x = portraitPosX;
					cs->portraitPos[playerNum].y = portraitPosY;
					cs->cursors[playerNum] = newCursor;
					cs->portraitBorders[playerNum] = newObject;
					cs->characterName[playerNum] = new HSText(characterNameFont);
					cs->characterName[playerNum]->justification = characterNameJustification;
					cs->characterName[playerNum]->pos.x = characterNamePosX;
					cs->characterName[playerNum]->pos.y = characterNamePosY;
					cs->characterName[playerNum]->depth = CHARACTER_SELECT_NAME_DEPTH;
					cs->characterName[playerNum]->palette = characterNamePalette;
					cs->portraitInstructions[playerNum] = new HSText(instructionsFont);
					cs->portraitInstructions[playerNum]->justification = instructionJustification;
					cs->portraitInstructions[playerNum]->pos.x = instructionsPosX;
					cs->portraitInstructions[playerNum]->pos.y = instructionsPosY;
					cs->portraitInstructions[playerNum]->depth = CHARACTER_SELECT_INSTRUCTIONS_DEPTH;
					cs->portraitInstructions[playerNum]->palette = instructionsPalette;
				}
			}
			
			if(int error = LoadPlayableCharacters(pcFilePath, panelBorderFilePath) != 0) { return error; }
		}
		else if(strcmp(i->Value(), "StageSelect") == 0)
		{
			StageChooser * sc = new StageChooser();
			characterSelectManager->stageChooser = sc;

			string titleText = "";
			float titlePosX = 0;
			float titlePosY = 0;
			float backgroundPosX = 0;
			float backgroundPosY = 0;
			string titleFontFilePath = "";
			HSFont * titleFont = NULL;
			TextJustification titleJustification = JUSTIFICATION_LEFT;
			string backgroundFilePath = "";
			HSObject * background = NULL;
			float previewPosX = 0;
			float previewPosY = 0;
			float previewBorderOffsetX = 0;
			float previewBorderOffsetY = 0;
			float stageNamePosX = 0;
			float stageNamePosY = 0;
			string stageNameFontFilePath = "";
			HSFont * stageNameFont = NULL;
			TextJustification stageNameJustification = JUSTIFICATION_LEFT;
			string previewBorderFilePath = "";
			HSObject * previewBorder = NULL;
			int titlePalette = 0;
			int stageNamePalette = 0;

			titleText = i->Attribute("titleText");
			i->QueryFloatAttribute("titlePosX", &titlePosX);
			i->QueryFloatAttribute("titlePosY", &titlePosY);
			i->QueryFloatAttribute("backgroundPosX", &backgroundPosX);
			i->QueryFloatAttribute("backgroundPosY", &backgroundPosY);
			titleFontFilePath = i->Attribute("titleFontFilePath");
			backgroundFilePath = i->Attribute("backgroundFilePath");
			i->QueryFloatAttribute("previewPosX", &previewPosX);
			i->QueryFloatAttribute("previewPosY", &previewPosY);
			i->QueryFloatAttribute("previewBorderOffsetX", &previewBorderOffsetX);
			i->QueryFloatAttribute("previewBorderOffsetY", &previewBorderOffsetY);
			i->QueryFloatAttribute("stageNamePosX", &stageNamePosX);
			i->QueryFloatAttribute("stageNamePosY", &stageNamePosY);
			stageNameFontFilePath = i->Attribute("stageNameFontFilePath");
			previewBorderFilePath = i->Attribute("previewBorderFilePath");
			i->QueryIntAttribute("titlePalette", &titlePalette);
			i->QueryIntAttribute("stageNamePalette", &stageNamePalette);

			string titleJustificationString = "";
			if(i->Attribute("titleJustification") != NULL) { titleJustificationString = i->Attribute("titleJustification"); }
			if(titleJustificationString.compare("RIGHT") == 0) { titleJustification = JUSTIFICATION_RIGHT; }
			else if(titleJustificationString.compare("CENTER") == 0) { titleJustification = JUSTIFICATION_CENTER; }

			string stageNameJustificationString = "";
			if(i->Attribute("stageNameJustification") != NULL) { stageNameJustificationString = i->Attribute("stageNameJustification"); }
			if(stageNameJustificationString.compare("RIGHT") == 0) { stageNameJustification = JUSTIFICATION_RIGHT; }
			else if(stageNameJustificationString.compare("CENTER") == 0) { stageNameJustification = JUSTIFICATION_CENTER; }

			if(int error = LoadHSFont(titleFontFilePath, &titleFont) != 0) { return error; }
			if(int error = LoadHSFont(stageNameFontFilePath, &stageNameFont) != 0) { return error; }
			if(int error = LoadDefinition(previewBorderFilePath, &HUDObjects, &previewBorder) != 0) { return error; }
			previewBorder->pos.x = previewPosX + previewBorderOffsetX;
			previewBorder->pos.y = previewPosY + previewBorderOffsetY;
			previewBorder->depth = STAGE_SELECT_PREVIEW_BORDER_DEPTH;
			if(int error = LoadDefinition(backgroundFilePath, &HUDObjects, &background) != 0) { return error; }
			background->pos.x = backgroundPosX;
			background->pos.y = backgroundPosY;
			background->depth = STAGE_SELECT_BACKGROUND_DEPTH;
			
			sc->imagePos.x = previewPosX;
			sc->imagePos.y = previewPosY;
			sc->imageDepth = STAGE_SELECT_PREVIEW_DEPTH;
			sc->stageSelectTitle = titleText;
			sc->title = new HSText(titleFont);
			sc->title->palette = titlePalette;
			sc->title->justification = titleJustification;
			sc->title->pos.x = titlePosX;
			sc->title->pos.y = titlePosY;
			sc->title->depth = STAGE_SELECT_TITLE_DEPTH;
			sc->background = background;
			sc->stageName = new HSText(stageNameFont);
			sc->stageName->palette = stageNamePalette;
			sc->stageName->justification = stageNameJustification;
			sc->stageName->pos.x = stageNamePosX;
			sc->stageName->pos.y = stageNamePosY;
			sc->stageName->depth = STAGE_SELECT_NAME_DEPTH;
			sc->previewBorder = previewBorder;

			if(int error = LoadPlayableStages(psFilePath) != 0) { return error; }
		}
		else if(strcmp(i->Value(), "MusicSelect") == 0)
		{
			string titleText = "";
			float titlePosX = 0;
			float titlePosY = 0;
			float backgroundPosX = 0;
			float backgroundPosY = 0;
			string titleFontFilePath = "";
			HSFont * titleFont = NULL;
			TextJustification titleJustification = JUSTIFICATION_LEFT;
			string backgroundFilePath = "";
			HSObject * background = NULL;
			float musicNamePosX = 0;
			float musicNamePosY = 0;
			string musicNameFontFilePath = "";
			HSFont * musicNameFont = NULL;
			TextJustification musicNameJustification = JUSTIFICATION_LEFT;
			int titlePalette = 0;
			int musicNamePalette = 0;

			titleText = i->Attribute("titleText");
			i->QueryFloatAttribute("titlePosX", &titlePosX);
			i->QueryFloatAttribute("titlePosY", &titlePosY);
			i->QueryFloatAttribute("backgroundPosX", &backgroundPosX);
			i->QueryFloatAttribute("backgroundPosY", &backgroundPosY);
			titleFontFilePath = i->Attribute("titleFontFilePath");
			backgroundFilePath = i->Attribute("backgroundFilePath");
			i->QueryFloatAttribute("musicNamePosX", &musicNamePosX);
			i->QueryFloatAttribute("musicNamePosY", &musicNamePosY);
			musicNameFontFilePath = i->Attribute("musicNameFontFilePath");
			i->QueryIntAttribute("titlePalette", &titlePalette);
			i->QueryIntAttribute("musicNamePalette", &musicNamePalette);

			string titleJustificationString = "";
			if(i->Attribute("titleJustification") != NULL) { titleJustificationString = i->Attribute("titleJustification"); }
			if(titleJustificationString.compare("RIGHT") == 0) { titleJustification = JUSTIFICATION_RIGHT; }
			else if(titleJustificationString.compare("CENTER") == 0) { titleJustification = JUSTIFICATION_CENTER; }

			string musicNameJustificationString = "";
			if(i->Attribute("titleJustification") != NULL) { musicNameJustificationString = i->Attribute("titleJustification"); }
			if(musicNameJustificationString.compare("RIGHT") == 0) { musicNameJustification = JUSTIFICATION_RIGHT; }
			else if(musicNameJustificationString.compare("CENTER") == 0) { musicNameJustification = JUSTIFICATION_CENTER; }

			if(int error = LoadHSFont(titleFontFilePath, &titleFont) != 0) { return error; }
			if(int error = LoadHSFont(musicNameFontFilePath, &musicNameFont) != 0) { return error; }
			if(int error = LoadDefinition(backgroundFilePath, &HUDObjects, &background) != 0) { return error; }
			background->pos.x = backgroundPosX;
			background->pos.y = backgroundPosY;
			background->depth = MUSIC_SELECT_BACKGROUND_DEPTH;

			MusicChooser * mc = new MusicChooser(musicNameFont);
			mc->palette = musicNamePalette;
			mc->justification = musicNameJustification;
			characterSelectManager->musicChooser = mc;

			mc->pos.x = musicNamePosX;
			mc->pos.y = musicNamePosY;
			mc->depth = MUSIC_SELECT_NAME_DEPTH;
			mc->musicSelectTitle = titleText;
			mc->title = new HSText(titleFont);
			mc->title->palette = titlePalette;
			mc->title->justification = titleJustification;
			mc->title->pos.x = titlePosX;
			mc->title->pos.y = titlePosY;
			mc->title->depth = MUSIC_SELECT_TITLE_DEPTH;
			mc->background = background;

			if(int error = LoadPlayableMusic(pmFilePath) != 0) { return error; }
		}
	}

	return 0;
}

int ObjectManager::LoadPlayableCharacters(string pcFilePath, string panelBorderFilePath)
{
	if(characterSelectManager == NULL || characterSelectManager->characterSelect == NULL) { return -1; }

	//load selectable characters
	//get the XML structure from the file
	XMLDocument * file = new XMLDocument();
	if(int error = file->LoadFile(pcFilePath.data()) != 0)
	{
		stringstream sstm;
		sstm << "Error loading definition file. Code: " << error << " File: " << pcFilePath;
		UpdateLog(sstm.str(), true);
		return error; //couldn't load the file
	}

	if(strcmp(file->RootElement()->Value(), "PlayableCharacters") != 0)
	{
		UpdateLog("XML file is not Homestrife character list definition file: " + pcFilePath, true);
		return -1;
	}

	XMLElement * root = file->RootElement();

	CharacterSelect * cs = characterSelectManager->characterSelect;

	string emptyPanelFilePath = "";
	string emptyPortraitFilePath = "";
	string emptyCharacterName = "";

	if(root->Attribute("emptyPanelFilePath") != NULL) { emptyPanelFilePath = root->Attribute("emptyPanelFilePath"); }
	if(root->Attribute("emptyPortraitFilePath") != NULL) { emptyPortraitFilePath = root->Attribute("emptyPortraitFilePath"); }
	if(root->Attribute("emptyCharacterName") != NULL) { emptyCharacterName = root->Attribute("emptyCharacterName"); }

	int entriesLoaded = 0;
	
	//loop through all the sections of the playable characters
	for(XMLElement * i = root->FirstChildElement(); i != NULL; i = i->NextSiblingElement())
	{
		if(strcmp(i->Value(), "Character") != 0)
		{
			continue;
		}

		int order = 0;
		HSObject * panel = NULL;
		HSObject * panelBorder = NULL;
		HSObject * portrait = NULL;

		i->QueryIntAttribute("order", &order);
		string characterName = i->Attribute("characterName");
		string defFilePath = i->Attribute("defFilePath");
		string hudIconDefFilePath = i->Attribute("hudIconDefFilePath");
		string panelFilePath = i->Attribute("panelFilePath");
		string portraitFilePath = i->Attribute("portraitFilePath");

		CharacterSelectPanel * newPanel = new CharacterSelectPanel();
		newPanel->orderID = order;

		if(int error = LoadDefinition(panelFilePath, &HUDObjects, &panel) != 0) { return error; }
		panel->depth = cs->panelImageDepth;
		newPanel->panelCharacterImage = panel;

		if(int error = LoadDefinition(panelBorderFilePath, &HUDObjects, &panelBorder) != 0) { return error; }
		panelBorder->depth = cs->panelBorderDepth;
		newPanel->panelBorderImage = panelBorder;

		cs->panels.push_back(newPanel);

		for(int j = 0; j < MAX_PLAYERS; j++)
		{
			if(int error = LoadDefinition(portraitFilePath, &HUDObjects, &portrait) != 0) { return error; }
			portrait->pos.x = cs->portraitPos[j].x;
			portrait->pos.y = cs->portraitPos[j].y;
			portrait->depth = cs->portraitDepth;
			
			CharacterSelectPortrait * newPortrait = new CharacterSelectPortrait();
			newPortrait->portraitImage = portrait;
			newPortrait->characterName = characterName;
			newPortrait->characterDefFilePath = defFilePath;
			newPortrait->characterIconDefFilePath = hudIconDefFilePath;

			cs->portraits[j].push_back(newPortrait);

			newPanel->portraitReference[j] = newPortrait;
		}

		entriesLoaded++;
	}

	//pad empty parts of the grid with dummy entries
	int order = 0;
	while(entriesLoaded < cs->columns * cs->rows)
	{
		//get the lowest available order number
		bool searchingForOrderNum = true;
		list<CharacterSelectPanel*>::iterator panItr;
		while(searchingForOrderNum)
		{
			searchingForOrderNum = false;
			for(panItr = cs->panels.begin(); panItr != cs->panels.end(); panItr++)
			{
				if(order == (*panItr)->orderID)
				{
					order++;
					searchingForOrderNum = true;
					break;
				}
			}
		}

		HSObject * panel = NULL;
		HSObject * panelBorder = NULL;
		HSObject * portrait = NULL;

		CharacterSelectPanel * newPanel = new CharacterSelectPanel();
		newPanel->orderID = order;

		if(!emptyPanelFilePath.empty())
		{
			if(int error = LoadDefinition(emptyPanelFilePath, &HUDObjects, &panel) != 0) { return error; }
			panel->depth = cs->panelImageDepth;
		}
		newPanel->panelCharacterImage = panel;

		if(int error = LoadDefinition(panelBorderFilePath, &HUDObjects, &panelBorder) != 0) { return error; }
		panelBorder->depth = cs->panelBorderDepth;
		newPanel->panelBorderImage = panelBorder;

		cs->panels.push_back(newPanel);

		for(int j = 0; j < MAX_PLAYERS; j++)
		{
			portrait = NULL;
			if(!emptyPortraitFilePath.empty())
			{
				if(int error = LoadDefinition(emptyPortraitFilePath, &HUDObjects, &portrait) != 0) { return error; }
				portrait->pos.x = cs->portraitPos[j].x;
				portrait->pos.y = cs->portraitPos[j].y;
				portrait->depth = cs->portraitDepth;
			}
			
			CharacterSelectPortrait * newPortrait = new CharacterSelectPortrait();
			newPortrait->portraitImage = portrait;
			newPortrait->characterName = emptyCharacterName;
			newPortrait->characterDefFilePath = "";
			newPortrait->characterIconDefFilePath = "";

			cs->portraits[j].push_back(newPortrait);

			newPanel->portraitReference[j] = newPortrait;
		}

		entriesLoaded++;
		order++;
	}

	//order the grid panels
	cs->panels.sort(HSOrderablePointerSort);

	//position the grid panels based on the order
	int row = 0;
	int column = 0;

	list<CharacterSelectPanel*>::iterator panItr;
	for(panItr = cs->panels.begin(); panItr != cs->panels.end(); panItr++)
	{
		if((*panItr)->panelCharacterImage != NULL)
		{
			(*panItr)->panelCharacterImage->pos.x = cs->gridPos.x + column * cs->panelWidth + cs->panelImageOffset.x;
			(*panItr)->panelCharacterImage->pos.y = cs->gridPos.y + row * cs->panelHeight + cs->panelImageOffset.y;
		}
		(*panItr)->panelBorderImage->pos.x = cs->gridPos.x + column * cs->panelWidth + cs->panelImageOffset.x + cs->panelBorderOffset.x;
		(*panItr)->panelBorderImage->pos.y = cs->gridPos.y + row * cs->panelHeight + cs->panelImageOffset.y + cs->panelBorderOffset.y;

		column++;
		if(column >= cs->columns)
		{
			column = 0;
			row++;
		}
	}

	return 0;
}

int ObjectManager::LoadPlayableStages(string psFilePath)
{
	if(characterSelectManager == NULL || characterSelectManager->stageChooser == NULL) { return -1; }

	//load selectable characters
	//get the XML structure from the file
	XMLDocument * file = new XMLDocument();
	if(int error = file->LoadFile(psFilePath.data()) != 0)
	{
		stringstream sstm;
		sstm << "Error loading definition file. Code: " << error << " File: " << psFilePath;
		UpdateLog(sstm.str(), true);
		return error; //couldn't load the file
	}

	if(strcmp(file->RootElement()->Value(), "PlayableStages") != 0)
	{
		UpdateLog("XML file is not Homestrife stage list definition file: " + psFilePath, true);
		return -1;
	}

	XMLElement * root = file->RootElement();

	StageChooser * sc = characterSelectManager->stageChooser;
	
	//loop through all the sections of the playable characters
	for(XMLElement * i = root->FirstChildElement(); i != NULL; i = i->NextSiblingElement())
	{
		if(strcmp(i->Value(), "Stage") != 0)
		{
			continue;
		}

		int order = 0;
		string stageName = "";
		string defFilePath = "";
		string previewFilePath = "";
		HSObject * preview = NULL;

		i->QueryIntAttribute("order", &order);
		stageName = i->Attribute("name");
		defFilePath = i->Attribute("defFilePath");
		previewFilePath = i->Attribute("previewFilePath");

		if(int error = LoadDefinition(previewFilePath, &HUDObjects, &preview) != 0) { return error; }
		preview->pos.x = sc->imagePos.x;
		preview->pos.y = sc->imagePos.y;
		preview->depth = sc->imageDepth;

		StageChooserItem * newItem = new StageChooserItem();

		newItem->orderID = order;
		newItem->stageName = stageName;
		newItem->stageDefFilePath = defFilePath;
		newItem->itemImage = preview;

		sc->items.push_back(newItem);
	}
	
	sc->items.sort(HSOrderablePointerSort);

	return 0;
}

int ObjectManager::LoadPlayableMusic(string pmFilePath)
{
	if(characterSelectManager == NULL || characterSelectManager->musicChooser == NULL) { return -1; }

	//load selectable characters
	//get the XML structure from the file
	XMLDocument * file = new XMLDocument();
	if(int error = file->LoadFile(pmFilePath.data()) != 0)
	{
		stringstream sstm;
		sstm << "Error loading definition file. Code: " << error << " File: " << pmFilePath;
		UpdateLog(sstm.str(), true);
		return error; //couldn't load the file
	}

	if(strcmp(file->RootElement()->Value(), "PlayableMusic") != 0)
	{
		UpdateLog("XML file is not Homestrife music list definition file: " + pmFilePath, true);
		return -1;
	}

	XMLElement * root = file->RootElement();

	MusicChooser * mc = characterSelectManager->musicChooser;
	
	//loop through all the sections of the playable characters
	for(XMLElement * i = root->FirstChildElement(); i != NULL; i = i->NextSiblingElement())
	{
		if(strcmp(i->Value(), "Music") != 0)
		{
			continue;
		}

		int order = 0;
		string musicName = "";
		string musicFilePath = "";

		i->QueryIntAttribute("order", &order);
		musicName = i->Attribute("name");
		musicFilePath = i->Attribute("musicFilePath");

		MusicChooserItem * newItem = new MusicChooserItem();

		newItem->orderID = order;
		newItem->itemText = musicName;
		newItem->musicFilePath = musicFilePath;
		
		mc->items.push_back(newItem);
	}

	mc->items.sort(HSOrderablePointerSort);

	return 0;
}

int ObjectManager::LoadHUD(string defFilePath, bool shouldLoadForPlayer[MAX_PLAYERS])
{
	if(hudManager == NULL) { return -1; }

	//load match HUD
	//get the XML structure from the file
	XMLDocument * file = new XMLDocument();
	if(int error = file->LoadFile(defFilePath.data()) != 0)
	{
		stringstream sstm;
		sstm << "Error loading definition file. Code: " << error << " File: " << defFilePath;
		UpdateLog(sstm.str(), true);
		return error; //couldn't load the file
	}

	if(strcmp(file->RootElement()->Value(), "HSMatchHUD") != 0)
	{
		UpdateLog("XML file is not Homestrife match HUD definition file: " + defFilePath, true);
		return -1;
	}

	XMLElement * root = file->RootElement();

	string promptJustificationString = "";
	TextJustification promptJustification = JUSTIFICATION_LEFT;
	float promptPosX = 0;
	float promptPosY = 0;
	float promptBGPosX = 0;
	float promptBGPosY = 0;
	string promptFontFilePath = "";
	HSFont * promptFont = NULL;
	HSText * prompt = NULL;
	string readyText = "";
	string fightText = "";
	string winText = "";
	string hitsText = "";
	string promptBGDefFilePath = "";
	HSObject * promptBackground = NULL;
	float underMeterAdjustRate = 0;
	float healthMeterWidth = 0;
	int promptPalette = 0;
	int hitsPalette = 0;
	int livesPalette = 0;
		
	if(root->Attribute("promptJustification") != NULL) { promptJustificationString = root->Attribute("promptJustification"); }
	if(promptJustificationString.compare("RIGHT") == 0) { promptJustification = JUSTIFICATION_RIGHT; }
	else if(promptJustificationString.compare("CENTER") == 0) { promptJustification = JUSTIFICATION_CENTER; }

	root->QueryFloatAttribute("promptPosX", &promptPosX);
	root->QueryFloatAttribute("promptPosY", &promptPosY);
	root->QueryFloatAttribute("promptBGPosX", &promptBGPosX);
	root->QueryFloatAttribute("promptBGPosY", &promptBGPosY);
	if(root->Attribute("promptFontFilePath") != NULL) { promptFontFilePath = root->Attribute("promptFontFilePath"); }
	if(root->Attribute("readyText") != NULL) { readyText = root->Attribute("readyText"); }
	if(root->Attribute("fightText") != NULL) { fightText = root->Attribute("fightText"); }
	if(root->Attribute("winText") != NULL) { winText = root->Attribute("winText"); }
	if(root->Attribute("hitsText") != NULL) { hitsText = root->Attribute("hitsText"); }
	if(root->Attribute("promptBGDefFilePath") != NULL) { promptBGDefFilePath = root->Attribute("promptBGDefFilePath"); }
	root->QueryFloatAttribute("underMeterAdjustRate", &underMeterAdjustRate);
	root->QueryFloatAttribute("healthMeterWidth", &healthMeterWidth);
	root->QueryIntAttribute("promptPalette", &promptPalette);
	root->QueryIntAttribute("hitsPalette", &hitsPalette);
	root->QueryIntAttribute("livesPalette", &livesPalette);

	if(int error = LoadHSFont(promptFontFilePath, &promptFont) != 0) { return error; }
	prompt = new HSText(promptFont);
	prompt->palette = promptPalette;
	prompt->pos.x = promptPosX;
	prompt->pos.y = promptPosY;
	prompt->depth = MATCH_HUD_PROMPT_DEPTH;
	prompt->justification = promptJustification;

	if(!promptBGDefFilePath.empty())
	{
		if(int error = LoadDefinition(promptBGDefFilePath, &HUDObjects, &promptBackground) != 0) { return error; }
		promptBackground->pos.x = promptBGPosX;
		promptBackground->pos.y = promptBGPosY;
		promptBackground->depth = MATCH_HUD_PROMPT_BACKGROUND_DEPTH;
	}

	hudManager->prompt = prompt;
	hudManager->promptBackground = promptBackground;
	hudManager->readyText = readyText;
	hudManager->fightText = fightText;
	hudManager->winText = winText;
	
	for(XMLElement * i = root->FirstChildElement(); i != NULL; i = i->NextSiblingElement())
	{
		if(strcmp(i->Value(), "PlayerHUDs") != 0)
		{
			continue;
		}

		for(XMLElement * k = i->FirstChildElement(); k != NULL; k = k->NextSiblingElement())
		{
			if(strcmp(k->Value(), "PlayerHUD") != 0)
			{
				continue;
			}
		
			int player = 0;
			string livesJustificationString = "";
			TextJustification livesJustification = JUSTIFICATION_LEFT;
			string comboJustificationString = "";
			TextJustification comboJustification = JUSTIFICATION_LEFT;
			float HUDPositionX = 0;
			float HUDPositionY = 0;
			float characterIconOffsetX = 0;
			float characterIconOffsetY = 0;
			float healthMeterOffsetX = 0;
			float healthMeterOffsetY = 0;
			bool flipMeterHorizontally = false;
			float livesCounterOffsetX = 0;
			float livesCounterOffsetY = 0;
			float comboCounterOffsetX = 0;
			float comboCounterOffsetY = 0;
			float comboCounterBGOffsetX = 0;
			float comboCounterBGOffsetY = 0;
			string backgroundDefFilePath = "";
			HSObject * background = NULL;
			string healthMeterDefFilePath = "";
			HSObject * healthMeter = NULL;
			string healthUnderMeterDefFilePath = "";
			HSObject * healthUnderMeter = NULL;
			string healthMeterCoverDefFilePath = "";
			HSObject * healthMeterCover = NULL;
			string livesCounterFontFilePath = "";
			HSFont * livesCounterFont = NULL;
			HSText * livesCounter = NULL;
			string comboCounterFontFilePath = "";
			HSFont * comboCounterFont = NULL;
			HSText * comboCounter = NULL;
			string comboCounterBackgroundDefFilePath = "";
			HSObject * comboCounterBackground = NULL;
		
			k->QueryIntAttribute("player", &player);

			if(player < 0) { player = 0; }
			else if(player >= MAX_PLAYERS) { player = MAX_PLAYERS - 1; }

			//just leave the HUD null if the player isn't participating
			if(!shouldLoadForPlayer[player]) { continue; }
		
			if(k->Attribute("livesJustification") != NULL) { livesJustificationString = k->Attribute("livesJustification"); }
			if(livesJustificationString.compare("RIGHT") == 0) { livesJustification = JUSTIFICATION_RIGHT; }
			else if(livesJustificationString.compare("CENTER") == 0) { livesJustification = JUSTIFICATION_CENTER; }
		
			if(k->Attribute("comboJustification") != NULL) { comboJustificationString = k->Attribute("comboJustification"); }
			if(comboJustificationString.compare("RIGHT") == 0) { comboJustification = JUSTIFICATION_RIGHT; }
			else if(comboJustificationString.compare("CENTER") == 0) { comboJustification = JUSTIFICATION_CENTER; }
		
			k->QueryFloatAttribute("HUDPositionX", &HUDPositionX);
			k->QueryFloatAttribute("HUDPositionY", &HUDPositionY);
			k->QueryFloatAttribute("characterIconOffsetX", &characterIconOffsetX);
			k->QueryFloatAttribute("characterIconOffsetY", &characterIconOffsetY);
			k->QueryFloatAttribute("healthMeterOffsetX", &healthMeterOffsetX);
			k->QueryFloatAttribute("healthMeterOffsetY", &healthMeterOffsetY);
			k->QueryBoolAttribute("flipMeterHorizontally", &flipMeterHorizontally);
			k->QueryFloatAttribute("livesCounterOffsetX", &livesCounterOffsetX);
			k->QueryFloatAttribute("livesCounterOffsetY", &livesCounterOffsetY);
			k->QueryFloatAttribute("comboCounterOffsetX", &comboCounterOffsetX);
			k->QueryFloatAttribute("comboCounterOffsetY", &comboCounterOffsetY);
			k->QueryFloatAttribute("comboCounterBGOffsetX", &comboCounterBGOffsetX);
			k->QueryFloatAttribute("comboCounterBGOffsetY", &comboCounterBGOffsetY);
			if(k->Attribute("backgroundDefFilePath") != NULL) { backgroundDefFilePath = k->Attribute("backgroundDefFilePath"); }
			if(k->Attribute("healthMeterDefFilePath") != NULL) { healthMeterDefFilePath = k->Attribute("healthMeterDefFilePath"); }
			if(k->Attribute("healthUnderMeterDefFilePath") != NULL) { healthUnderMeterDefFilePath = k->Attribute("healthUnderMeterDefFilePath"); }
			if(k->Attribute("healthMeterCoverDefFilePath") != NULL) { healthMeterCoverDefFilePath = k->Attribute("healthMeterCoverDefFilePath"); }
			if(k->Attribute("livesCounterFontFilePath") != NULL) { livesCounterFontFilePath = k->Attribute("livesCounterFontFilePath"); }
			if(k->Attribute("hitsCounterFontFilePath") != NULL) { comboCounterFontFilePath = k->Attribute("hitsCounterFontFilePath"); }
			if(k->Attribute("comboCounterBackgroundDefFilePath") != NULL) { comboCounterBackgroundDefFilePath = k->Attribute("comboCounterBackgroundDefFilePath"); }

			if(int error = LoadDefinition(backgroundDefFilePath, &HUDObjects, &background) != 0) { return error; }
			background->pos.x = HUDPositionX;
			background->pos.y = HUDPositionY;
			background->depth = MATCH_HUD_DEPTH;

			if(int error = LoadDefinition(healthMeterDefFilePath, &HUDObjects, &healthMeter) != 0) { return error; }
			healthMeter->depth = MATCH_HUD_METER_DEPTH;

			if(int error = LoadDefinition(healthUnderMeterDefFilePath, &HUDObjects, &healthUnderMeter) != 0) { return error; }
			healthUnderMeter->depth = MATCH_HUD_UNDER_METER_DEPTH;

			if(!healthMeterCoverDefFilePath.empty())
			{
				if(int error = LoadDefinition(healthMeterCoverDefFilePath, &HUDObjects, &healthMeterCover) != 0) { return error; }
				healthMeterCover->depth = MATCH_HUD_METER_COVER_DEPTH;
			}
		
			healthMeter->pos.x = healthUnderMeter->pos.x = healthMeterCover->pos.x = HUDPositionX + healthMeterOffsetX;
			healthMeter->pos.y = healthUnderMeter->pos.y = healthMeterCover->pos.y = HUDPositionY + healthMeterOffsetY;

			if(int error = LoadHSFont(livesCounterFontFilePath, &livesCounterFont) != 0) { return error; }
			livesCounter = new HSText(livesCounterFont);
			livesCounter->palette = livesPalette;
			livesCounter->pos.x = HUDPositionX + livesCounterOffsetX;
			livesCounter->pos.y = HUDPositionY + livesCounterOffsetY;
			livesCounter->depth = MATCH_HUD_LIVES_DEPTH;
			livesCounter->justification = livesJustification;

			if(int error = LoadHSFont(comboCounterFontFilePath, &comboCounterFont) != 0) { return error; }
			comboCounter = new HSText(comboCounterFont);
			comboCounter->palette = hitsPalette;
			comboCounter->pos.x = HUDPositionX + comboCounterOffsetX;
			comboCounter->pos.y = HUDPositionY + comboCounterOffsetY;
			comboCounter->depth = MATCH_HUD_COMBO_COUNTER_DEPTH;
			comboCounter->justification = comboJustification;

			if(!comboCounterBackgroundDefFilePath.empty())
			{
				if(int error = LoadDefinition(comboCounterBackgroundDefFilePath, &HUDObjects, &comboCounterBackground) != 0) { return error; }
				comboCounterBackground->pos.x = HUDPositionX + comboCounterBGOffsetX;
				comboCounterBackground->pos.y = HUDPositionY + comboCounterBGOffsetY;
				comboCounterBackground->depth = MATCH_HUD_COMBO_COUNTER_BACKGROUND_DEPTH;
			}

			HUD * newHUD = new HUD();
			newHUD->characterIconPos.x = HUDPositionX + characterIconOffsetX;
			newHUD->characterIconPos.y = HUDPositionY + characterIconOffsetY;
			newHUD->background = background;
			newHUD->healthMeter = healthMeter;
			newHUD->healthUnderMeter = healthUnderMeter;
			newHUD->healthMeterCover = healthMeterCover;
			newHUD->underMeterAdjustRate = underMeterAdjustRate;
			newHUD->healthMeterWidth = healthMeterWidth;
			newHUD->flipMeterHorizontally = flipMeterHorizontally;
			newHUD->healthMeterPosX = healthMeter->pos.x;
			newHUD->livesCounter = livesCounter;
			newHUD->comboCounter = comboCounter;
			newHUD->comboCounterBackground = comboCounterBackground;
			newHUD->hitsText = hitsText;

			hudManager->HUDs[player] = newHUD;
		}
	}

	return 0;
}

void ObjectManager::UpdateMenu()
{
	if(menuManager == NULL) { return; }

	MenuManager * mm = menuManager;

	if(needApplyFullscreenResolutionForMenu)
	{
		mm->SetChoiceForItem(FULL_SCREEN_RESOLUTION, mm->GetResolutionChooserFunction(fullscreenResolutionXForMenu, fullscreenResolutionYForMenu));
		needApplyFullscreenResolutionForMenu = false;
	}

	if(needApplyWindowedResolutionForMenu)
	{
		mm->SetChoiceForItem(WINDOWED_RESOLUTION, mm->GetResolutionChooserFunction(windowedResolutionXForMenu, windowedResolutionYForMenu));
		needApplyWindowedResolutionForMenu = false;
	}
}

int ObjectManager::ApplyVideoSettings()
{
	if(menuManager == NULL) { return 0; }
	
	MenuChooserFunction fullscreenFunc = menuManager->GetChooserFuncByMenuFunc(FULL_SCREEN);
	MenuChooserFunction stretchScreenfunc = menuManager->GetChooserFuncByMenuFunc(STRETCH_SCREEN);
	MenuChooserFunction fullscreenResFunc = menuManager->GetChooserFuncByMenuFunc(FULL_SCREEN_RESOLUTION);
	MenuChooserFunction windowedResFunc = menuManager->GetChooserFuncByMenuFunc(WINDOWED_RESOLUTION);

	fullScreenToApply = menuManager->GetYesNoBoolean(fullscreenFunc);
	stretchScreenToApply = menuManager->GetYesNoBoolean(stretchScreenfunc);
	fullscreenResolutionXToApply = menuManager->GetResolutionXInt(fullscreenResFunc);
	fullscreenResolutionYToApply = menuManager->GetResolutionYInt(fullscreenResFunc);
	windowedResolutionXToApply = menuManager->GetResolutionXInt(windowedResFunc);
	windowedResolutionYToApply = menuManager->GetResolutionYInt(windowedResFunc);

	stretchScreen = stretchScreenToApply;
	fullscreenResolutionX = fullscreenResolutionXToApply;
	fullscreenResolutionY = fullscreenResolutionYToApply;
	windowedResolutionX = windowedResolutionXToApply;
	windowedResolutionY = windowedResolutionYToApply;

	applyVideoSettings = true;

	return 0;
}

int ObjectManager::CloneObject(SpawnObject * objectToClone, list<HSObject*> * objects, HSObject ** returnValue)
{
	if(objectToClone == NULL) { return 0; }

	HSObject * newObject = NULL;

	//clone the actual object
	CloneObject(objectToClone->object, objects, &newObject);

	//get a value from the input object's hFlip, to be applied to x-coordinate spawn object variables
	int hFlip = 1;
	if(objectToClone->parent != NULL && objectToClone->parent->hFlip) { hFlip = -1; }

	//apply the spawn object values
	if(objectToClone->parent != NULL)
	{
		newObject->hFlip = objectToClone->parent->hFlip;
	}
	newObject->pos.x += objectToClone->parentOffset.x * hFlip;
	newObject->pos.y += objectToClone->parentOffset.y;
	newObject->vel.x = objectToClone->vel.x * hFlip;
	newObject->vel.y = objectToClone->vel.y;
	newObject->followParent = objectToClone->followParent;
	if(newObject->IsTerrainObject())
	{
		((TerrainObject*)newObject)->collideParent = objectToClone->collideParent;
	}

	if(returnValue != NULL)
	{
		*returnValue = newObject;
	}

	return 0;
}

int ObjectManager::CloneObject(HSObject * objectToClone, list<HSObject*> * objects, HSObject ** returnValue)
{
	if(objectToClone == NULL) { return 0; }

	HSObject * newObject;
	TerrainObject * toToClone;
	TerrainObject * newTO;
	PhysicsObject * poToClone;
	PhysicsObject * newPO;
	Fighter * fToClone;
	Fighter * newF;

	//make a new object of the same type as the input object
	if(objectToClone->IsFighter())
	{
		newObject = new Fighter();
		fToClone = (Fighter*)objectToClone;
		newF = (Fighter*)newObject;
		poToClone = (PhysicsObject*)objectToClone;
		newPO = (PhysicsObject*)newObject;
		toToClone = (TerrainObject*)objectToClone;
		newTO = (TerrainObject*)newObject;
	}
	else if(objectToClone->IsPhysicsObject())
	{
		newObject = new PhysicsObject();
		poToClone = (PhysicsObject*)objectToClone;
		newPO = (PhysicsObject*)newObject;
		toToClone = (TerrainObject*)objectToClone;
		newTO = (TerrainObject*)newObject;
	}
	else if(objectToClone->IsTerrainObject())
	{
		newObject = new TerrainObject();
		toToClone = (TerrainObject*)objectToClone;
		newTO = (TerrainObject*)newObject;
	}
	else
	{
		newObject = new HSObject();
	}

	//apply the input object's current values
	if(objectToClone->parent != NULL)
	{
		newObject->pos.x = objectToClone->parent->pos.x;
		newObject->pos.y = objectToClone->parent->pos.y;
	}
	newObject->prevPos.x = newObject->pos.x;
	newObject->prevPos.y = newObject->pos.y;
	newObject->parent = objectToClone->parent;

	//get all the input object's variables
	newObject->definitionFilePath = objectToClone->definitionFilePath;
	newObject->followParent = objectToClone->followParent;
	newObject->lifetime = objectToClone->lifetime;
	newObject->palette = objectToClone->palette;
	newObject->palettes = objectToClone->palettes;
	newObject->useTGAPalettes = objectToClone->useTGAPalettes;
	newObject->visible = objectToClone->visible;

	//get all the object's palettes
	list<PaletteInstance>::iterator palItr;
	for(palItr = objectToClone->palettes.begin(); palItr != objectToClone->palettes.end(); palItr++)
	{
		newObject->palettes.push_back(*palItr);
		palItr->hsPal->usingCount++;
	}

	newObject->SetPalette(objectToClone->GetPalette());

	if(objectToClone->IsTerrainObject())
	{
		newTO->bounce = toToClone->bounce;
		newTO->canBeJumpedThrough = toToClone->canBeJumpedThrough;
		newTO->collides = toToClone->collides;

		HSBox * curBox = toToClone->firstTerrainBox;
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

			if(newTO->firstTerrainBox == NULL)
			{
				//just make the new hit box the first hit box
				newTO->firstTerrainBox = newBox;
				newTO->lastTerrainBox = newBox;
			}
			else
			{
				//put the new hit box at the end of the list
				newTO->lastTerrainBox->nextBox = newBox;
				newTO->lastTerrainBox = newBox;
			}

			curBox = curBox->nextBox;
		}

		newTO->friction = toToClone->friction;
		newTO->health = toToClone->health;
	
		list<AudioInstance>::iterator audIt;
		for(audIt = toToClone->hitAudioList.begin(); audIt != toToClone->hitAudioList.end(); audIt++)
		{
			AudioInstance newInstance;
			newInstance.delay = audIt->delay;
			newInstance.exclusive = audIt->exclusive;
			newInstance.hsAud = audIt->hsAud;
			newInstance.percentage = audIt->percentage;
			newInstance.usePercentage = audIt->usePercentage;

			newTO->hitAudioList.push_back(newInstance);
		}

		newTO->takesTerrainDamage = toToClone->takesTerrainDamage;
		newTO->fragile = toToClone->fragile;
	}

	if(objectToClone->IsPhysicsObject())
	{
		newPO->mass = poToClone->mass;
		newPO->falls = poToClone->falls;
		newPO->maxFallSpeed = poToClone->maxFallSpeed;
		newPO->ignoreJumpThroughTerrain = poToClone->ignoreJumpThroughTerrain;
	}

	if(objectToClone->IsFighter())
	{
		HSBox * curBox = fToClone->firstUprightTerrainBox;
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

			if(newF->firstUprightTerrainBox == NULL)
			{
				//just make the new hit box the first hit box
				newF->firstUprightTerrainBox = newBox;
				newF->lastUprightTerrainBox = newBox;
			}
			else
			{
				//put the new hit box at the end of the list
				newF->lastUprightTerrainBox->nextBox = newBox;
				newF->lastUprightTerrainBox = newBox;
			}

			curBox = curBox->nextBox;
		}

		curBox = fToClone->firstCrouchingTerrainBox;
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

			if(newF->firstCrouchingTerrainBox == NULL)
			{
				//just make the new hit box the first hit box
				newF->firstCrouchingTerrainBox = newBox;
				newF->lastCrouchingTerrainBox = newBox;
			}
			else
			{
				//put the new hit box at the end of the list
				newF->lastCrouchingTerrainBox->nextBox = newBox;
				newF->lastCrouchingTerrainBox = newBox;
			}

			curBox = curBox->nextBox;
		}

		curBox = fToClone->firstProneTerrainBox;
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

			if(newF->firstProneTerrainBox == NULL)
			{
				//just make the new hit box the first hit box
				newF->firstProneTerrainBox = newBox;
				newF->lastProneTerrainBox = newBox;
			}
			else
			{
				//put the new hit box at the end of the list
				newF->lastProneTerrainBox->nextBox = newBox;
				newF->lastProneTerrainBox = newBox;
			}

			curBox = curBox->nextBox;
		}

		curBox = fToClone->firstCompactTerrainBox;
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

			if(newF->firstCompactTerrainBox == NULL)
			{
				//just make the new hit box the first hit box
				newF->firstCompactTerrainBox = newBox;
				newF->lastCompactTerrainBox = newBox;
			}
			else
			{
				//put the new hit box at the end of the list
				newF->lastCompactTerrainBox->nextBox = newBox;
				newF->lastCompactTerrainBox = newBox;
			}

			curBox = curBox->nextBox;
		}
	}

	//clone all of the object's holds
	HSObjectHold * holdToClone = objectToClone->firstHold;
	while(holdToClone != NULL)
	{
		//create a new hold
		HSObjectHold * newHold = new HSObjectHold();
		if(objectToClone->IsFighter())
		{
			newHold = new FighterHold();
		}
		else if(objectToClone->IsPhysicsObject())
		{
			newHold = new PhysicsObjectHold();
		}
		else if(objectToClone->IsTerrainObject())
		{
			newHold = new TerrainObjectHold();
		}
		else
		{
			newHold = new HSObjectHold();
		}

		//get all of the input object hold's attributes
		list<AudioInstance>::iterator audIt;
		for(audIt = holdToClone->audioList.begin(); audIt != holdToClone->audioList.end(); audIt++)
		{
			AudioInstance newInstance;
			newInstance.delay = audIt->delay;
			newInstance.exclusive = audIt->exclusive;
			newInstance.hsAud = audIt->hsAud;
			newInstance.percentage = audIt->percentage;
			newInstance.usePercentage = audIt->usePercentage;

			newHold->audioList.push_back(newInstance);
		}

		newHold->duration = holdToClone->duration;
		newHold->id = holdToClone->id;
		newHold->nextHoldId = holdToClone->nextHoldId;
	
		list<SpawnObject>::iterator spawnIt;
		for(spawnIt = holdToClone->spawnObjects.begin(); spawnIt != holdToClone->spawnObjects.end(); spawnIt++)
		{
			SpawnObject newInstance;
			newInstance.parent = newObject;
			newInstance.defFilePath = spawnIt->defFilePath;
			newInstance.delay = spawnIt->delay;
			newInstance.followParent = spawnIt->followParent;
			CloneObject(spawnIt->object, NULL, &newInstance.object);
			newInstance.parentOffset.x = spawnIt->parentOffset.x;
			newInstance.parentOffset.y = spawnIt->parentOffset.y;
			newInstance.vel.x = spawnIt->vel.x;
			newInstance.vel.y = spawnIt->vel.y;

			newHold->spawnObjects.push_back(newInstance);
		}

		list<TextureInstance>::iterator texIt;
		for(texIt = holdToClone->textures.begin(); texIt != holdToClone->textures.end(); texIt++)
		{
			TextureInstance newInstance;
			newInstance.depth = texIt->depth;
			newInstance.hScale = texIt->hScale;
			newInstance.hsTex = texIt->hsTex;
			newInstance.offset.x = texIt->offset.x;
			newInstance.offset.y = texIt->offset.y;
			newInstance.vScale = texIt->vScale;

			newHold->textures.push_back(newInstance);
		}

		//get event holds
		if(objectToClone->hsObjectEventHolds.lifetimeDeath != NULL && newHold->id == objectToClone->hsObjectEventHolds.lifetimeDeath->id) { newObject->hsObjectEventHolds.lifetimeDeath = newHold; }

		if(objectToClone->IsTerrainObject())
		{
			TerrainObjectHold * tohToClone = (TerrainObjectHold*)holdToClone;
			TerrainObjectHold * newTOH = (TerrainObjectHold*)newHold;

			newTOH->blockability = tohToClone->blockability;
			newTOH->ownHitstop = tohToClone->ownHitstop;
			newTOH->victimHitstop = tohToClone->victimHitstop;
			newTOH->blockstun = tohToClone->blockstun;
			newTOH->changeAttackBoxAttributes = tohToClone->changeAttackBoxAttributes;
			newTOH->damage = tohToClone->damage;
	
			HSBox * curBox = tohToClone->firstAttackBox;
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

				if(newTOH->firstAttackBox == NULL)
				{
					//just make the new hit box the first hit box
					newTOH->firstAttackBox = newBox;
					newTOH->lastAttackBox = newBox;
				}
				else
				{
					//put the new hit box at the end of the list
					newTOH->lastAttackBox->nextBox = newBox;
					newTOH->lastAttackBox = newBox;
				}

				curBox = curBox->nextBox;
			}
	
			curBox = tohToClone->firstHurtBox;
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

				if(newTOH->firstHurtBox == NULL)
				{
					//just make the new hit box the first hit box
					newTOH->firstHurtBox = newBox;
					newTOH->lastHurtBox = newBox;
				}
				else
				{
					//put the new hit box at the end of the list
					newTOH->lastHurtBox->nextBox = newBox;
					newTOH->lastHurtBox = newBox;
				}

				curBox = curBox->nextBox;
			}

			newTOH->force.x = tohToClone->force.x;
			newTOH->force.y = tohToClone->force.y;
	
			list<AudioInstance>::iterator audIt;
			for(audIt = tohToClone->hitAudioList.begin(); audIt != tohToClone->hitAudioList.end(); audIt++)
			{
				AudioInstance newInstance;
				newInstance.delay = audIt->delay;
				newInstance.exclusive = audIt->exclusive;
				newInstance.hsAud = audIt->hsAud;
				newInstance.percentage = audIt->percentage;
				newInstance.usePercentage = audIt->usePercentage;

				newTOH->hitAudioList.push_back(newInstance);
			}
	
			for(audIt = tohToClone->blockedAudioList.begin(); audIt != tohToClone->blockedAudioList.end(); audIt++)
			{
				AudioInstance newInstance;
				newInstance.delay = audIt->delay;
				newInstance.exclusive = audIt->exclusive;
				newInstance.hsAud = audIt->hsAud;
				newInstance.percentage = audIt->percentage;
				newInstance.usePercentage = audIt->usePercentage;

				newTOH->blockedAudioList.push_back(newInstance);
			}

			newTOH->hitstun = tohToClone->hitstun;
			newTOH->horizontalDirectionBasedBlock = tohToClone->horizontalDirectionBasedBlock;
			newTOH->reversedHorizontalBlock = tohToClone->reversedHorizontalBlock;
			newTOH->trips = tohToClone->trips;

			//get event holds
			if(toToClone->terrainEventHolds.healthDeath != NULL && newTOH->id == toToClone->terrainEventHolds.healthDeath->id) { newTO->terrainEventHolds.healthDeath = newTOH; }
		}

		if(objectToClone->IsPhysicsObject())
		{
			PhysicsObjectHold * pohToClone = (PhysicsObjectHold*)holdToClone;
			PhysicsObjectHold * newPOH = (PhysicsObjectHold*)newHold;


		}

		if(objectToClone->IsFighter())
		{
			FighterHold * fhToClone = (FighterHold*)holdToClone;
			FighterHold * newFH = (FighterHold*)newHold;
	
			//get event holds
			if(fToClone->fighterEventHolds.standing != NULL && newFH->id == fToClone->fighterEventHolds.standing->id) { newF->fighterEventHolds.standing = newFH; }
			if(fToClone->fighterEventHolds.turn != NULL && newFH->id == fToClone->fighterEventHolds.turn->id) { newF->fighterEventHolds.turn = newFH; }
			if(fToClone->fighterEventHolds.walk != NULL && newFH->id == fToClone->fighterEventHolds.walk->id) { newF->fighterEventHolds.walk = newFH; }
			if(fToClone->fighterEventHolds.walking != NULL && newFH->id == fToClone->fighterEventHolds.walking->id) { newF->fighterEventHolds.walking = newFH; }
			if(fToClone->fighterEventHolds.walkingTurn != NULL && newFH->id == fToClone->fighterEventHolds.walkingTurn->id) { newF->fighterEventHolds.walkingTurn = newFH; }
			if(fToClone->fighterEventHolds.run != NULL && newFH->id == fToClone->fighterEventHolds.run->id) { newF->fighterEventHolds.run = newFH; }
			if(fToClone->fighterEventHolds.running != NULL && newFH->id == fToClone->fighterEventHolds.running->id) { newF->fighterEventHolds.running = newFH; }
			if(fToClone->fighterEventHolds.runningStop != NULL && newFH->id == fToClone->fighterEventHolds.runningStop->id) { newF->fighterEventHolds.runningStop = newFH; }
			if(fToClone->fighterEventHolds.runningTurn != NULL && newFH->id == fToClone->fighterEventHolds.runningTurn->id) { newF->fighterEventHolds.runningTurn = newFH; }
			if(fToClone->fighterEventHolds.crouch != NULL && newFH->id == fToClone->fighterEventHolds.crouch->id) { newF->fighterEventHolds.crouch = newFH; }
			if(fToClone->fighterEventHolds.crouching != NULL && newFH->id == fToClone->fighterEventHolds.crouching->id) { newF->fighterEventHolds.crouching = newFH; }
			if(fToClone->fighterEventHolds.crouchingTurn != NULL && newFH->id == fToClone->fighterEventHolds.crouchingTurn->id) { newF->fighterEventHolds.crouchingTurn = newFH; }
			if(fToClone->fighterEventHolds.stand != NULL && newFH->id == fToClone->fighterEventHolds.stand->id) { newF->fighterEventHolds.stand = newFH; }
	
			if(fToClone->fighterEventHolds.jumpNeutralStart != NULL && newFH->id == fToClone->fighterEventHolds.jumpNeutralStart->id) { newF->fighterEventHolds.jumpNeutralStart = newFH; }
			if(fToClone->fighterEventHolds.jumpNeutralStartAir != NULL && newFH->id == fToClone->fighterEventHolds.jumpNeutralStartAir->id) { newF->fighterEventHolds.jumpNeutralStartAir = newFH; }
			if(fToClone->fighterEventHolds.jumpNeutralRising != NULL && newFH->id == fToClone->fighterEventHolds.jumpNeutralRising->id) { newF->fighterEventHolds.jumpNeutralRising = newFH; }
			if(fToClone->fighterEventHolds.jumpNeutralFall != NULL && newFH->id == fToClone->fighterEventHolds.jumpNeutralFall->id) { newF->fighterEventHolds.jumpNeutralFall = newFH; }
			if(fToClone->fighterEventHolds.jumpNeutralFalling != NULL && newFH->id == fToClone->fighterEventHolds.jumpNeutralFalling->id) { newF->fighterEventHolds.jumpNeutralFalling = newFH; }
			if(fToClone->fighterEventHolds.jumpNeutralLand != NULL && newFH->id == fToClone->fighterEventHolds.jumpNeutralLand->id) { newF->fighterEventHolds.jumpNeutralLand = newFH; }
			if(fToClone->fighterEventHolds.jumpBackwardRising != NULL && newFH->id == fToClone->fighterEventHolds.jumpBackwardRising->id) { newF->fighterEventHolds.jumpBackwardRising = newFH; }
			if(fToClone->fighterEventHolds.jumpBackwardFall != NULL && newFH->id == fToClone->fighterEventHolds.jumpBackwardFall->id) { newF->fighterEventHolds.jumpBackwardFall = newFH; }
	
			if(fToClone->fighterEventHolds.airDashForward != NULL && newFH->id == fToClone->fighterEventHolds.airDashForward->id) { newF->fighterEventHolds.airDashForward = newFH; }
			if(fToClone->fighterEventHolds.airDashBackward != NULL && newFH->id == fToClone->fighterEventHolds.airDashBackward->id) { newF->fighterEventHolds.airDashBackward = newFH; }
	
			if(fToClone->fighterEventHolds.blockHigh != NULL && newFH->id == fToClone->fighterEventHolds.blockHigh->id) { newF->fighterEventHolds.blockHigh = newFH; }
			if(fToClone->fighterEventHolds.blockLow != NULL && newFH->id == fToClone->fighterEventHolds.blockLow->id) { newF->fighterEventHolds.blockLow = newFH; }
			if(fToClone->fighterEventHolds.blockAir != NULL && newFH->id == fToClone->fighterEventHolds.blockAir->id) { newF->fighterEventHolds.blockAir = newFH; }
	
			if(fToClone->fighterEventHolds.hitstunLightHighStanding != NULL && newFH->id == fToClone->fighterEventHolds.hitstunLightHighStanding->id) { newF->fighterEventHolds.hitstunLightHighStanding = newFH; }
			if(fToClone->fighterEventHolds.hitstunLightMidStanding != NULL && newFH->id == fToClone->fighterEventHolds.hitstunLightMidStanding->id) { newF->fighterEventHolds.hitstunLightMidStanding = newFH; }
			if(fToClone->fighterEventHolds.hitstunLightLowStanding != NULL && newFH->id == fToClone->fighterEventHolds.hitstunLightLowStanding->id) { newF->fighterEventHolds.hitstunLightLowStanding = newFH; }
			if(fToClone->fighterEventHolds.hitstunLightMidCrouching != NULL && newFH->id == fToClone->fighterEventHolds.hitstunLightMidCrouching->id) { newF->fighterEventHolds.hitstunLightMidCrouching = newFH; }
			if(fToClone->fighterEventHolds.hitstunLightLowCrouching != NULL && newFH->id == fToClone->fighterEventHolds.hitstunLightLowCrouching->id) { newF->fighterEventHolds.hitstunLightLowCrouching = newFH; }
			if(fToClone->fighterEventHolds.hitstunLightAir != NULL && newFH->id == fToClone->fighterEventHolds.hitstunLightAir->id) { newF->fighterEventHolds.hitstunLightAir = newFH; }
	
			if(fToClone->fighterEventHolds.attackLightNeutralGround != NULL && newFH->id == fToClone->fighterEventHolds.attackLightNeutralGround->id) { newF->fighterEventHolds.attackLightNeutralGround = newFH; }
			if(fToClone->fighterEventHolds.attackLightDownGround != NULL && newFH->id == fToClone->fighterEventHolds.attackLightDownGround->id) { newF->fighterEventHolds.attackLightDownGround = newFH; }
			if(fToClone->fighterEventHolds.attackLightUpGround != NULL && newFH->id == fToClone->fighterEventHolds.attackLightUpGround->id) { newF->fighterEventHolds.attackLightUpGround = newFH; }
			if(fToClone->fighterEventHolds.attackLightForwardGround != NULL && newFH->id == fToClone->fighterEventHolds.attackLightForwardGround->id) { newF->fighterEventHolds.attackLightForwardGround = newFH; }
			if(fToClone->fighterEventHolds.attackLightQCFGround != NULL && newFH->id == fToClone->fighterEventHolds.attackLightQCFGround->id) { newF->fighterEventHolds.attackLightQCFGround = newFH; }
			if(fToClone->fighterEventHolds.attackLightNeutralAir != NULL && newFH->id == fToClone->fighterEventHolds.attackLightNeutralAir->id) { newF->fighterEventHolds.attackLightNeutralAir = newFH; }
			if(fToClone->fighterEventHolds.attackLightDownAir != NULL && newFH->id == fToClone->fighterEventHolds.attackLightDownAir->id) { newF->fighterEventHolds.attackLightDownAir = newFH; }
			if(fToClone->fighterEventHolds.attackLightUpAir != NULL && newFH->id == fToClone->fighterEventHolds.attackLightUpAir->id) { newF->fighterEventHolds.attackLightUpAir = newFH; }
			if(fToClone->fighterEventHolds.attackLightForwardAir != NULL && newFH->id == fToClone->fighterEventHolds.attackLightForwardAir->id) { newF->fighterEventHolds.attackLightForwardAir = newFH; }
			if(fToClone->fighterEventHolds.attackLightBackwardAir != NULL && newFH->id == fToClone->fighterEventHolds.attackLightBackwardAir->id) { newF->fighterEventHolds.attackLightBackwardAir = newFH; }
			if(fToClone->fighterEventHolds.attackLightQCFAir != NULL && newFH->id == fToClone->fighterEventHolds.attackLightQCFAir->id) { newF->fighterEventHolds.attackLightQCFAir = newFH; }
	
			if(fToClone->fighterEventHolds.attackHeavyNeutralGround != NULL && newFH->id == fToClone->fighterEventHolds.attackHeavyNeutralGround->id) { newF->fighterEventHolds.attackHeavyNeutralGround = newFH; }
			if(fToClone->fighterEventHolds.attackHeavyDownGround != NULL && newFH->id == fToClone->fighterEventHolds.attackHeavyDownGround->id) { newF->fighterEventHolds.attackHeavyDownGround = newFH; }
			if(fToClone->fighterEventHolds.attackHeavyUpGround != NULL && newFH->id == fToClone->fighterEventHolds.attackHeavyUpGround->id) { newF->fighterEventHolds.attackHeavyUpGround = newFH; }
			if(fToClone->fighterEventHolds.attackHeavyForwardGround != NULL && newFH->id == fToClone->fighterEventHolds.attackHeavyForwardGround->id) { newF->fighterEventHolds.attackHeavyForwardGround = newFH; }
			if(fToClone->fighterEventHolds.attackHeavyQCFGround != NULL && newFH->id == fToClone->fighterEventHolds.attackHeavyQCFGround->id) { newF->fighterEventHolds.attackHeavyQCFGround = newFH; }
			if(fToClone->fighterEventHolds.attackHeavyNeutralAir != NULL && newFH->id == fToClone->fighterEventHolds.attackHeavyNeutralAir->id) { newF->fighterEventHolds.attackHeavyNeutralAir = newFH; }
			if(fToClone->fighterEventHolds.attackHeavyDownAir != NULL && newFH->id == fToClone->fighterEventHolds.attackHeavyDownAir->id) { newF->fighterEventHolds.attackHeavyDownAir = newFH; }
			if(fToClone->fighterEventHolds.attackHeavyUpAir != NULL && newFH->id == fToClone->fighterEventHolds.attackHeavyUpAir->id) { newF->fighterEventHolds.attackHeavyUpAir = newFH; }
			if(fToClone->fighterEventHolds.attackHeavyForwardAir != NULL && newFH->id == fToClone->fighterEventHolds.attackHeavyForwardAir->id) { newF->fighterEventHolds.attackHeavyForwardAir = newFH; }
			if(fToClone->fighterEventHolds.attackHeavyBackwardAir != NULL && newFH->id == fToClone->fighterEventHolds.attackHeavyBackwardAir->id) { newF->fighterEventHolds.attackHeavyBackwardAir = newFH; }
			if(fToClone->fighterEventHolds.attackHeavyQCFAir != NULL && newFH->id == fToClone->fighterEventHolds.attackHeavyQCFAir->id) { newF->fighterEventHolds.attackHeavyQCFAir = newFH; }
		}

		//add the new hold to the new object
		if(newObject->firstHold == NULL)
		{
			//just make the new hit box the first hit box
			newObject->firstHold = newHold;
			newObject->lastHold = newHold;
		}
		else
		{
			//put the new hit box at the end of the list
			newObject->lastHold->nextListHold = newHold;
			newObject->lastHold = newHold;
		}

		newObject->numHolds++;

		holdToClone = holdToClone->nextListHold;
	}

	//set the next holds
	for(HSObjectHold * i = newObject->firstHold; i != NULL; i = i->nextListHold)
	{
		if(i->nextHoldId > 0)
		{
			for(HSObjectHold * j = newObject->firstHold; j != NULL; j = j->nextListHold)
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

	if(objects != NULL)
	{
		newObject->id = newObjectId;
		newObjectId++;
		newObject->ChangeHold(newObject->firstHold);

		objects->push_back(newObject);

		if(objects == &stageObjects)
		{
			stageObjectsNeedSort = true;
		}
		else if(objects == &HUDObjects)
		{
			HUDObjectsNeedSort = true;
		}
	}

	if(returnValue != NULL)
	{
		*returnValue = newObject;
	}

	return 0;
}

int ObjectManager::ClearAllObjects()
{
	SDL_LockAudio();
	
	list<HSObject*>::iterator objIt;
	for ( objIt=stageObjects.begin(); objIt != stageObjects.end(); objIt++)
	{
		delete (*objIt);
	}

	stageObjects.clear();
	
	for ( objIt=BGSpawnedObjects.begin(); objIt != BGSpawnedObjects.end(); objIt++)
	{
		delete (*objIt);
	}

	BGSpawnedObjects.clear();
	
	for ( objIt=fighterObjects.begin(); objIt != fighterObjects.end(); objIt++)
	{
		delete (*objIt);
	}

	fighterObjects.clear();
	
	for ( objIt=FGSpawnedObjects.begin(); objIt != FGSpawnedObjects.end(); objIt++)
	{
		delete (*objIt);
	}

	FGSpawnedObjects.clear();
	
	for ( objIt=HUDObjects.begin(); objIt != HUDObjects.end(); objIt++)
	{
		delete (*objIt);
	}

	HUDObjects.clear();

	list<HSTexture*>::iterator texIt;
	for ( texIt=textureRegistry.begin(); texIt != textureRegistry.end(); texIt++)
	{
		list<HSTextureSlice*>::iterator tsIt;
		for ( tsIt=(*texIt)->textureSlices.begin(); tsIt != (*texIt)->textureSlices.end(); tsIt++)
		{
			if((*tsIt)->bufferID != 0)
			{
				buffersToDelete.push_back((*tsIt)->bufferID);
				(*tsIt)->bufferID = 0;
			}
			if((*tsIt)->vaoID != 0)
			{
				vaosToDelete.push_back((*tsIt)->vaoID);
				(*tsIt)->vaoID = 0;
			}
			if((*tsIt)->textureID != 0)
			{
				texturesToDelete.push_back((*tsIt)->textureID);
				(*tsIt)->textureID = 0;
			}
			delete (*tsIt);
		}
		delete (*texIt);
	}

	textureRegistry.clear();

	list<HSPalette*>::iterator palIt;
	for ( palIt=paletteRegistry.begin(); palIt != paletteRegistry.end(); palIt++)
	{
		if((*palIt)->textureID != 0)
		{
			palettesToDelete.push_back((*palIt)->textureID);
			(*palIt)->textureID = 0;
		}
		delete (*palIt);
	}

	paletteRegistry.clear();

	clearTexturesAndPalettes = true;

	list<HSAudio*>::iterator audIt;
	for ( audIt=audioRegistry.begin(); audIt != audioRegistry.end(); audIt++)
	{
		free ((*audIt)->data);
		delete (*audIt);
	}

	audioRegistry.clear();

	list<CurrentAudioEntry*>::iterator curAudIt;
	for ( curAudIt=currentAudio.begin(); curAudIt != currentAudio.end(); curAudIt++)
	{
		delete (*curAudIt);
	}

	currentAudio.clear();
	
	SDL_UnlockAudio();

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		players[i] = NULL;
		focusObject[i] = NULL;
	}

	delete menuManager;
	delete characterSelectManager;
	delete hudManager;

	menuManager = NULL;
	characterSelectManager = NULL;
	hudManager = NULL;

	list<HSFont*>::iterator fontIt;
	for (fontIt = fontRegistry.begin(); fontIt != fontRegistry.end(); fontIt++)
	{
		delete *fontIt;
	}

	fontRegistry.clear();

	return 0;
}

int ObjectManager::ClearSpecificObject(HSObject* object)
{
	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(players[i] == object) { players[i] = NULL; }
		if(focusObject[i] == object) { focusObject[i] = NULL; }
	}

	delete object;

	return 0;
}