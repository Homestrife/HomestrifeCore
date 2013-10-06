#include "objectManager.h"

list<CurrentAudioEntry*> currentAudio;

ObjectManager::ObjectManager()
{
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
	newObjectId = 1;
	openGL3 = false;
	obtainedAudioSpec = NULL;
	currentAudio.clear();
	menuManager = NULL;

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		playerHUDs[i] = NULL;
		players[i] = NULL;
		focusObject[i] = NULL;
		characterList[i].clear();
		selectedCharacters[i].defFilePath = "";
		selectedCharacters[i].demoObject = NULL;
		selectedPalettes[i] = 0;
	}
	stageList.clear();
	selectedStage.defFilePath = "";
	selectedStage.demoObject = NULL;

	loading = NULL;
	menuManager = NULL;
	playerOne = NULL;
	playerTwo = NULL;
	playerThree = NULL;
	playerFour = NULL;
	wins = NULL;
	pressDesiredButton = NULL;
	readyOne = NULL;
	readyTwo = NULL;
	readyThree = NULL;
	readyFour = NULL;
	selectCharacterOne = NULL;
	selectCharacterTwo = NULL;
	selectCharacterThree = NULL;
	selectCharacterFour = NULL;
	selectPaletteOne = NULL;
	selectPaletteTwo = NULL;
	selectPaletteThree = NULL;
	selectPaletteFour = NULL;
	selectPaletteLeftOne = NULL;
	selectPaletteLeftTwo = NULL;
	selectPaletteLeftThree = NULL;
	selectPaletteLeftFour = NULL;
	selectPaletteRightOne = NULL;
	selectPaletteRightTwo = NULL;
	selectPaletteRightThree = NULL;
	selectPaletteRightFour = NULL;
}

bool ObjectSort(HSObject * first, HSObject * second)
{
	if(first->depth > second->depth) { return true; }
	return false;
}

bool PaletteSort(PaletteInstance first, PaletteInstance second)
{
	if(first.orderID < second.orderID) { return true; }
	return false;
}

void ObjectManager::SortAllObjects()
{
	if(stageObjectsNeedSort)
	{
		stageObjects.sort(ObjectSort);
		stageObjectsNeedSort = false;
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
		else if(strcmp(objectType, "HUD") == 0)
		{
			newObject = new HUD();
		}
		else
		{
			continue; //invalid object type
		}

		newObject->definitionFilePath = defFilePath;

		//get the parts of the object definition
		XMLElement * holdsDef = NULL;
		XMLElement * eventHoldsDef = NULL;
		XMLElement * terrainBoxDef = NULL;
		XMLElement * uprightTerrainBoxDef = NULL;
		XMLElement * crouchingTerrainBoxDef = NULL;
		XMLElement * proneTerrainBoxDef = NULL;
		XMLElement * compactTerrainBoxDef = NULL;
		XMLElement * palettesDef = NULL;
		for(XMLElement * partDef = objDef->FirstChildElement(); partDef != NULL; partDef = partDef->NextSiblingElement())
		{
			if(strcmp(partDef->Value(), "Holds") == 0) { holdsDef = partDef; }
			else if(strcmp(partDef->Value(), "EventHolds") == 0) { eventHoldsDef = partDef; }
			else if(strcmp(partDef->Value(), "TerrainBox") == 0) { terrainBoxDef = partDef; }
			else if(strcmp(partDef->Value(), "UprightTerrainBox") == 0) { uprightTerrainBoxDef = partDef; }
			else if(strcmp(partDef->Value(), "CrouchingTerrainBox") == 0) { crouchingTerrainBoxDef = partDef; }
			else if(strcmp(partDef->Value(), "ProneTerrainBox") == 0) { proneTerrainBoxDef = partDef; }
			else if(strcmp(partDef->Value(), "CompactTerrainBox") == 0) { compactTerrainBoxDef = partDef; }
			else if(strcmp(partDef->Value(), "Palettes") == 0) { palettesDef = partDef; }
		}

		//get object attributes
		objDef->QueryUnsignedAttribute("lifetime", &newObject->lifetime);

		if(newObject->IsHUD())
		{
			HUD * newHUD = (HUD*)newObject;
			
			objDef->QueryFloatAttribute("hudWidth", &newHUD->hudWidth);
			objDef->QueryFloatAttribute("hudHeight", &newHUD->hudHeight);
			objDef->QueryFloatAttribute("healthMeterOffsetX", &newHUD->healthMeterOffset.x);
			objDef->QueryFloatAttribute("healthMeterOffsetY", &newHUD->healthMeterOffset.y);
			objDef->QueryFloatAttribute("livesCounterOffsetX", &newHUD->livesCounterOffset.x);
			objDef->QueryFloatAttribute("livesCounterOffsetY", &newHUD->livesCounterOffset.y);
			objDef->QueryFloatAttribute("livesCounterDigitWidth", &newHUD->livesCounterDigitWidth);
			objDef->QueryFloatAttribute("livesCounterDigitSeparation", &newHUD->livesCounterDigitSeparation);
			objDef->QueryFloatAttribute("hitsCounterXDistanceFromSide", &newHUD->hitsCounterXDistanceFromSide);
			objDef->QueryFloatAttribute("hitsCounterYDistanceFromHUD", &newHUD->hitsCounterYDistanceFromHUD);
			objDef->QueryFloatAttribute("hitsCounterDigitWidth", &newHUD->hitsCounterDigitWidth);
			objDef->QueryFloatAttribute("hitsCounterDigitHeight", &newHUD->hitsCounterDigitHeight);
			objDef->QueryFloatAttribute("hitsCounterDigitSeparation", &newHUD->hitsCounterDigitSeparation);
			newHUD->healthMeterFilePath = objDef->Attribute("healthMeterFilePath");
			newHUD->healthUnderMeterFilePath = objDef->Attribute("healthUnderMeterFilePath");
			newHUD->livesCounterFilePath = objDef->Attribute("livesCounterFilePath");
			newHUD->hitsCounterFilePath = objDef->Attribute("hitsCounterFilePath");
		}

		if(newObject->IsTerrainObject())
		{
			TerrainObject * newTObject = (TerrainObject*)newObject;

			objDef->QueryFloatAttribute("bounce", &newTObject->bounce);
			objDef->QueryBoolAttribute("canBeJumpedThrough", &newTObject->canBeJumpedThrough);
			objDef->QueryFloatAttribute("friction", &newTObject->friction);
			objDef->QueryIntAttribute("health", &newTObject->health);
			objDef->QueryBoolAttribute("takesTerrainDamage", &newTObject->takesTerrainDamage);
			objDef->QueryBoolAttribute("fragile", &newTObject->fragile);

			HSBox * newBox = new HSBox();
			DefineBox(terrainBoxDef, newBox);
			newTObject->firstTerrainBox = newBox;
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

				string palFilePath = paletteDef->Attribute("path");
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

					if(int error = LoadHSPToPalette(newPal) != 0) //load the texture
					{
						return error;
					}

					paletteRegistry.push_back(newPal);
					newPalInst.hsPal = newPal;
				}

				newPalInst.name = paletteDef->Attribute("name");
				paletteDef->QueryUnsignedAttribute("id", &newPalInst.orderID);
				newObject->palettes.push_back(newPalInst);
			}

			newObject->palettes.sort(PaletteSort);
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
			else if(strcmp(objectType, "HUD") == 0)
			{
				newHold = new HUDHold();
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

				//get changeCancels
				const char * cc = holdDef->Attribute("changeCancels");
				string bString;
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

						string textureFilePath = texDef->Attribute("textureFilePath");

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
							newTex->bufferID = 0;
							newTex->vaoID = 0;

							if(int error = LoadTGAToTexture(newTex, openGL3, newObject->useTGAPalettes) != 0) //load the texture
							{
								return error;
							}

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
						string audioFilePath = audioDef->Attribute("audioFilePath");

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
						string spawnObjectFilePath = spawnDef->Attribute("definitionFilePath");

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
						string audioFilePath = hitAudioDef->Attribute("hitAudioFilePath");

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
						string audioFilePath = blockedAudioDef->Attribute("blockedAudioFilePath");

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
		}

		if(newObject->IsHUD())
		{
			HUD * hud = (HUD*)newObject;
			//load the meters and counters
			if(int error = LoadDefinition(hud->healthUnderMeterFilePath, objects, &(hud->healthUnderMeter)) != 0) { return error; }

			if(int error = LoadDefinition(hud->healthMeterFilePath, objects, &(hud->healthMeter)) != 0) { return error; }

			if(int error = LoadDefinition(hud->livesCounterFilePath, objects, &(hud->livesOnesCounter)) != 0) { return error; }

			if(int error = LoadDefinition(hud->livesCounterFilePath, objects, &(hud->livesTensCounter)) != 0) { return error; }

			if(int error = LoadDefinition(hud->hitsCounterFilePath, objects, &(hud->hitsOnesCounter)) != 0) { return error; }

			if(int error = LoadDefinition(hud->hitsCounterFilePath, objects, &(hud->hitsTensCounter)) != 0) { return error; }

			if(int error = LoadDefinition(hud->hitsCounterFilePath, objects, &(hud->hitsHundredsCounter)) != 0) { return error; }
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

int ObjectManager::LoadPlayableCharacters(bool loadPlayer[MAX_PLAYERS])
{
	//load selectable characters
	//get the XML structure from the file
	XMLDocument * file = new XMLDocument();
	if(int error = file->LoadFile("data\\characters\\playableCharacters.xml") != 0)
	{
		stringstream sstm;
		sstm << "Error loading definition file. Code: " << error << " File: data\\characters\\playableCharacters.xml";
		UpdateLog(sstm.str(), true);
		return error; //couldn't load the file
	}

	if(strcmp(file->RootElement()->Value(), "PlayableCharacters") != 0)
	{
		UpdateLog("XML file is not Homestrife character list definition file: data\\characters\\playableCharacters.xml", true);
		return -1;
	}

	XMLElement * root = file->RootElement();

	for(int j = 0; j < MAX_PLAYERS; j++)
	{
		selectedCharacters[j].defFilePath = "";
		selectedCharacters[j].demoObject = NULL;
		selectedPalettes[j] = 0;
	}

	//loop through all the sections of the playable characters
	HSObject * newObject;
	for(XMLElement * i = root->FirstChildElement(); i != NULL; i = i->NextSiblingElement())
	{
		if(strcmp(i->Value(), "Character") != 0)
		{
			continue;
		}

		const char * defFilePath = i->Attribute("defFilePath");
		const char * demoFilePath = i->Attribute("demoFilePath");

		for(int j = 0; j < MAX_PLAYERS; j++)
		{
			if(!loadPlayer[j]) { continue; }

			if(int error = LoadDefinition(demoFilePath, &fighterObjects, &newObject) != 0) { return error; }
			PlayableCharacter character;
			character.defFilePath = defFilePath;
			character.demoObject = newObject;
			characterList[j].push_back(character);
			newObject->visible = false;
			if(selectedCharacters[j].demoObject == NULL)
			{
				focusObject[j] = newObject;
				newObject->visible = true;
				selectedCharacters[j] = character;
			}
		}
	}

	return 0;
}

void ObjectManager::PreviousCharacter(int player)
{
	if(characterList[player].empty()) { return; }

	if(selectedCharacters[player].demoObject == NULL) { selectedCharacters[player] = characterList[player].front(); return; }

	PlayableCharacter curPC;
	curPC.defFilePath = "";
	curPC.demoObject = NULL;
	list<PlayableCharacter>::iterator pcIt;
	for ( pcIt=characterList[player].begin(); pcIt != characterList[player].end(); pcIt++)
	{
		if(selectedCharacters[player].demoObject == pcIt->demoObject)
		{
			if(curPC.demoObject == NULL)
			{
				//the current character is at the start of the list. go to the last one
				selectedPalettes[player] = 0;
				selectedCharacters[player].demoObject->visible = false;
				selectedCharacters[player] = characterList[player].back();
				focusObject[player] = selectedCharacters[player].demoObject;
				selectedCharacters[player].demoObject->visible = true;
				return;
			}
			
			selectedPalettes[player] = 0;
			selectedCharacters[player].demoObject->visible = false;
			selectedCharacters[player] = curPC;
			focusObject[player] = selectedCharacters[player].demoObject;
			selectedCharacters[player].demoObject->visible = true;
			return;
		}

		curPC = *pcIt;
	}
}

void ObjectManager::NextCharacter(int player)
{
	if(characterList[player].empty()) { return; }

	if(selectedCharacters[player].demoObject == NULL) { selectedCharacters[player] = characterList[player].front(); return; }

	//switch to the palette following the current one
	bool curCharFound = false;
	bool nextCharSet = false;
	list<PlayableCharacter>::iterator pcIt;
	for ( pcIt=characterList[player].begin(); pcIt != characterList[player].end(); pcIt++)
	{
		if(!curCharFound && selectedCharacters[player].demoObject == pcIt->demoObject)
		{
			curCharFound = true;
		}
		else if(curCharFound)
		{
			selectedCharacters[player].demoObject->visible = false;
			selectedPalettes[player] = 0;
			selectedCharacters[player] = *pcIt;
			focusObject[player] = selectedCharacters[player].demoObject;
			selectedCharacters[player].demoObject->visible = true;
			nextCharSet = true;
			break;
		}
	}

	if(!nextCharSet)
	{
		//the current character must have been at the end of the list. go to the first one
		selectedCharacters[player].demoObject->visible = false;
		selectedPalettes[player] = 0;
		selectedCharacters[player] = characterList[player].front();
		focusObject[player] = selectedCharacters[player].demoObject;
		selectedCharacters[player].demoObject->visible = true;
	}
}

int ObjectManager::LoadPlayableStages()
{
	//load selectable stages
	//get the XML structure from the file
	XMLDocument * file = new XMLDocument();
	if(int error = file->LoadFile("data\\stages\\playableStages.xml") != 0)
	{
		stringstream sstm;
		sstm << "Error loading definition file. Code: " << error << " File: data\\stages\\playableStages.xml";
		UpdateLog(sstm.str(), true);
		return error; //couldn't load the file
	}

	if(strcmp(file->RootElement()->Value(), "PlayableStages") != 0)
	{
		UpdateLog("XML file is not Homestrife stage list definition file: data\\stages\\playableStages.xml", true);
		return -1;
	}

	selectedStage.defFilePath = "";
	selectedStage.demoObject = NULL;

	XMLElement * root = file->RootElement();

	//loop through all the sections of the playable characters
	HSObject * newObject;
	for(XMLElement * i = root->FirstChildElement(); i != NULL; i = i->NextSiblingElement())
	{
		if(strcmp(i->Value(), "Stage") != 0)
		{
			continue;
		}

		const char * defFilePath = i->Attribute("defFilePath");
		const char * demoFilePath = i->Attribute("demoFilePath");

		if(int error = LoadDefinition(demoFilePath, &HUDObjects, &newObject) != 0) { return error; }
		PlayableStage stage;
		stage.defFilePath = defFilePath;
		stage.demoObject = newObject;
		stageList.push_back(stage);
		newObject->visible = false;
		if(selectedStage.demoObject == NULL)
		{
			selectedStage = stage;
		}
	}

	return 0;
}

void ObjectManager::PreviousStage()
{
	if(stageList.empty()) { return; }

	if(selectedStage.demoObject == NULL) { selectedStage = stageList.front(); return; }

	PlayableStage curPS;
	curPS.defFilePath = "";
	curPS.demoObject = NULL;
	list<PlayableStage>::iterator psIt;
	for ( psIt=stageList.begin(); psIt != stageList.end(); psIt++)
	{
		if(selectedStage.demoObject == psIt->demoObject)
		{
			if(curPS.demoObject == NULL)
			{
				//the current character is at the start of the list. go to the last one
				selectedStage.demoObject->visible = false;
				selectedStage = stageList.back();
				selectedStage.demoObject->visible = true;
				return;
			}
			
			selectedStage.demoObject->visible = false;
			selectedStage = curPS;
			selectedStage.demoObject->visible = true;
			return;
		}

		curPS = *psIt;
	}
}

void ObjectManager::NextStage()
{
	if(stageList.empty()) { return; }

	if(selectedStage.demoObject == NULL) { selectedStage = stageList.front(); return; }

	//switch to the palette following the current one
	bool curStageFound = false;
	bool nextStageSet = false;
	list<PlayableStage>::iterator psIt;
	for ( psIt=stageList.begin(); psIt != stageList.end(); psIt++)
	{
		if(!curStageFound && selectedStage.demoObject == psIt->demoObject)
		{
			curStageFound = true;
		}
		else if(curStageFound)
		{
			selectedStage.demoObject->visible = false;
			selectedStage = *psIt;
			selectedStage.demoObject->visible = true;
			nextStageSet = true;
			break;
		}
	}

	if(!nextStageSet)
	{
		//the current character must have been at the end of the list. go to the first one
		selectedStage.demoObject->visible = false;
		selectedStage = stageList.front();
		selectedStage.demoObject->visible = true;
	}
}

int ObjectManager::CloneObject(SpawnObject * objectToClone, list<HSObject*> * objects, HSObject ** returnValue)
{
	if(objectToClone == NULL) { return 0; }

	HSObject * newObject = NULL;

	//clone the actual object
	CloneObject(objectToClone->object, objects, &newObject);

	//get a value from the input object's hFlip, to be applied to x-coordinate spawn object variables
	int hFlip = 1;
	if(objectToClone->parent->hFlip) { hFlip = -1; }

	//apply the spawn object values
	newObject->hFlip = objectToClone->parent->hFlip;
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
	else if(objectToClone->IsHUD())
	{
		newObject = new HUD();
	}
	else
	{
		newObject = new HSObject();
	}

	//apply the input object's current values
	newObject->pos.x = objectToClone->parent->pos.x;
	newObject->pos.y = objectToClone->parent->pos.y;
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
		else if(objectToClone->IsHUD())
		{
			newHold = new HUDHold();
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
		if((*texIt)->bufferID != 0)
		{
			glDeleteBuffers(1, &(*texIt)->bufferID);
			(*texIt)->bufferID = 0;
		}
		if((*texIt)->vaoID != 0)
		{
			glDeleteVertexArrays(1, &(*texIt)->vaoID);
			(*texIt)->vaoID = 0;
		}
		if((*texIt)->textureID != 0)
		{
			glDeleteTextures(1, &(*texIt)->textureID);
			(*texIt)->textureID = 0;
		}
		delete (*texIt);
	}

	textureRegistry.clear();

	list<HSPalette*>::iterator palIt;
	for ( palIt=paletteRegistry.begin(); palIt != paletteRegistry.end(); palIt++)
	{
		if((*palIt)->textureID != 0)
		{
			glDeleteTextures(1, &(*palIt)->textureID);
			(*palIt)->textureID = 0;
		}
		delete (*palIt);
	}

	paletteRegistry.clear();

	list<HSAudio*>::iterator audIt;
	for ( audIt=audioRegistry.begin(); audIt != audioRegistry.end(); audIt++)
	{
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
		playerHUDs[i] = NULL;
		focusObject[i] = NULL;
		characterList[i].clear();
	}

	delete menuManager;

	loading = NULL;
	menuManager = NULL;
	playerOne = NULL;
	playerTwo = NULL;
	playerThree = NULL;
	playerFour = NULL;
	wins = NULL;
	pressDesiredButton = NULL;
	readyOne = NULL;
	readyTwo = NULL;
	readyThree = NULL;
	readyFour = NULL;
	selectCharacterOne = NULL;
	selectCharacterTwo = NULL;
	selectCharacterThree = NULL;
	selectCharacterFour = NULL;
	selectPaletteOne = NULL;
	selectPaletteTwo = NULL;
	selectPaletteThree = NULL;
	selectPaletteFour = NULL;
	selectPaletteLeftOne = NULL;
	selectPaletteLeftTwo = NULL;
	selectPaletteLeftThree = NULL;
	selectPaletteLeftFour = NULL;
	selectPaletteRightOne = NULL;
	selectPaletteRightTwo = NULL;
	selectPaletteRightThree = NULL;
	selectPaletteRightFour = NULL;

	return 0;
}

int ObjectManager::ClearSpecificObject(HSObject* object)
{
	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(players[i] == object) { players[i] = NULL; }
		if(playerHUDs[i] == object) { playerHUDs[i] = NULL; }
		if(focusObject[i] == object) { focusObject[i] = NULL; }
	}

	delete object;

	return 0;
}