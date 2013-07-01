#include "main.h"

GLenum shader_vert;
GLenum shader_fragNonIndexed;
GLenum shader_fragIndexed;
GLenum shader_progNonIndexed;
GLenum shader_progIndexed;

Main::Main()
{
	currentWorkingDirectory = GetCurrentWorkingDirectory(NULL, 0);

	gameState = MAIN_MENU;
	mainMenuState = TOP;
	gameType = FREE_FOR_ALL;
	texCoordBufferID = 0;
	elementArrayBufferID = 0;
	gameObjects.clear();
	HUDObjects.clear();
	textureRegistry.clear();
	paletteRegistry.clear();
	audioRegistry.clear();
	newObjectId = 1;
	
	nonIndexedScaleLoc = 0;
	indexedScaleLoc = 0;
	nonIndexedPosOffsetLoc = 0;
	indexedPosOffsetLoc = 0;
	nonIndexedResolutionLoc = 0;
	indexedResolutionLoc = 0;
	nonIndexedResScaleLoc = 0;
	indexedResScaleLoc = 0;
	nonIndexedFocusPosLoc = 0;
	indexedFocusPosLoc = 0;
	nonIndexedZoomOutLoc = 0;
	indexedZoomOutLoc = 0;
	nonIndexedTexLoc = 0;
	indexedTexLoc = 0;
	paletteLoc = 0;
	openGL3 = false;
	notDone = true;
	surf_display = NULL;
	screenResolutionX = MAX_GAME_RESOLUTION_X;
	screenResolutionY = MAX_GAME_RESOLUTION_Y;
	gameResolutionX = screenResolutionX;
	gameResolutionY = screenResolutionY;
	fullScreen = false;

	obtainedAudioSpec = NULL;
	currentAudio.clear();

	zoomOut = 1;
	focusPos.x = 0;
	focusPos.y = 0;
	focusObjectOne = NULL;
	focusObjectTwo = NULL;

	lastFrameTicks = 0;
	frame = 0;

	playerToSetUp = -1;
	keyToSetUp = -1;
	currentSettingMapping = SETTING_UP;

	menuManager = NULL;
	loading = NULL;
	playerOne = NULL;
	playerTwo = NULL;
	playerThree = NULL;
	playerFour = NULL;
	wins = NULL;
	pressDesiredButton = NULL;
	readyOne = NULL;
	readyTwo = NULL;
	selectPaletteOne = NULL;
	selectPaletteTwo = NULL;
	selectPaletteLeftOne = NULL;
	selectPaletteLeftTwo = NULL;
	selectPaletteRightOne = NULL;
	selectPaletteRightTwo = NULL;

	//set up the default input states
	defaultInputs.bKeyUp.held = false; defaultInputs.bKeyUp.pressed = false; defaultInputs.bKeyUp.released = false;
	defaultInputs.bKeyDown.held = false; defaultInputs.bKeyDown.pressed = false; defaultInputs.bKeyDown.released = false;
	defaultInputs.bKeyLeft.held = false; defaultInputs.bKeyLeft.pressed = false; defaultInputs.bKeyLeft.released = false;
	defaultInputs.bKeyRight.held = false; defaultInputs.bKeyRight.pressed = false; defaultInputs.bKeyRight.released = false;
	defaultInputs.bKeyJump.held = false; defaultInputs.bKeyJump.pressed = false; defaultInputs.bKeyJump.released = false;
	defaultInputs.bKeyBlock.held = false; defaultInputs.bKeyBlock.pressed = false; defaultInputs.bKeyBlock.released = false;
	defaultInputs.bKeyLight.held = false; defaultInputs.bKeyLight.pressed = false; defaultInputs.bKeyLight.released = false;
	defaultInputs.bKeyHeavy.held = false; defaultInputs.bKeyHeavy.pressed = false; defaultInputs.bKeyHeavy.released = false;
	defaultInputs.bKeyStart.held = false; defaultInputs.bKeyStart.pressed = false; defaultInputs.bKeyStart.released = false;
	defaultInputs.bKeySelect.held = false; defaultInputs.bKeySelect.pressed = false; defaultInputs.bKeySelect.released = false;
	defaultInputs.keyMenuConfirm.held = false; defaultInputs.keyMenuConfirm.pressed = false; defaultInputs.keyMenuConfirm.released = false;
	defaultInputs.keyMenuBack.held = false; defaultInputs.keyMenuBack.pressed = false; defaultInputs.keyMenuBack.released = false;
	defaultInputs.bButtonUp.held = false; defaultInputs.bButtonUp.pressed = false; defaultInputs.bButtonUp.released = false;
	defaultInputs.bButtonDown.held = false; defaultInputs.bButtonDown.pressed = false; defaultInputs.bButtonDown.released = false;
	defaultInputs.bButtonLeft.held = false; defaultInputs.bButtonLeft.pressed = false; defaultInputs.bButtonLeft.released = false;
	defaultInputs.bButtonRight.held = false; defaultInputs.bButtonRight.pressed = false; defaultInputs.bButtonRight.released = false;
	defaultInputs.bButtonJump.held = false; defaultInputs.bButtonJump.pressed = false; defaultInputs.bButtonJump.released = false;
	defaultInputs.bButtonBlock.held = false; defaultInputs.bButtonBlock.pressed = false; defaultInputs.bButtonBlock.released = false;
	defaultInputs.bButtonLight.held = false; defaultInputs.bButtonLight.pressed = false; defaultInputs.bButtonLight.released = false;
	defaultInputs.bButtonHeavy.held = false; defaultInputs.bButtonHeavy.pressed = false; defaultInputs.bButtonHeavy.released = false;
	defaultInputs.bButtonStart.held = false; defaultInputs.bButtonStart.pressed = false; defaultInputs.bButtonStart.released = false;
	defaultInputs.bButtonSelect.held = false; defaultInputs.bButtonSelect.pressed = false; defaultInputs.bButtonSelect.released = false;
	defaultInputs.buttonMenuConfirm.held = false; defaultInputs.buttonMenuConfirm.pressed = false; defaultInputs.buttonMenuConfirm.released = false;
	defaultInputs.buttonMenuBack.held = false; defaultInputs.buttonMenuBack.pressed = false; defaultInputs.buttonMenuBack.released = false;
	defaultInputs.bStickUp.held = false; defaultInputs.bStickUp.pressed = false; defaultInputs.bStickUp.released = false;
	defaultInputs.bStickDown.held = false; defaultInputs.bStickDown.pressed = false; defaultInputs.bStickDown.released = false;
	defaultInputs.bStickLeft.held = false; defaultInputs.bStickLeft.pressed = false; defaultInputs.bStickLeft.released = false;
	defaultInputs.bStickRight.held = false; defaultInputs.bStickRight.pressed = false; defaultInputs.bStickRight.released = false;
	defaultInputs.bStickHardUp.held = false; defaultInputs.bStickHardUp.pressed = false; defaultInputs.bStickHardUp.released = false;
	defaultInputs.bStickHardDown.held = false; defaultInputs.bStickHardDown.pressed = false; defaultInputs.bStickHardDown.released = false;
	defaultInputs.bStickHardLeft.held = false; defaultInputs.bStickHardLeft.pressed = false; defaultInputs.bStickHardLeft.released = false;
	defaultInputs.bStickHardRight.held = false; defaultInputs.bStickHardRight.pressed = false; defaultInputs.bStickHardRight.released = false;
	defaultInputs.bHatUp.held = false; defaultInputs.bHatUp.pressed = false; defaultInputs.bHatUp.released = false;
	defaultInputs.bHatDown.held = false; defaultInputs.bHatDown.pressed = false; defaultInputs.bHatDown.released = false;
	defaultInputs.bHatLeft.held = false; defaultInputs.bHatLeft.pressed = false; defaultInputs.bHatLeft.released = false;
	defaultInputs.bHatRight.held = false; defaultInputs.bHatRight.pressed = false; defaultInputs.bHatRight.released = false;
	defaultInputs.prevInputState = NULL;
	defaultInputs.frame = 0;

	sticks.clear();
	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		playerLives[i] = 0;
		spawnPoints[i].x = 0;
		spawnPoints[i].y = 0;
		playerHUDs[i] = NULL;
		players[i] = NULL;
		ClearControls(i);
		InputStates * newHistory = new InputStates();
		*newHistory = defaultInputs;
		inputHistory[i] = newHistory;
		curInputs[i] = new InputStates();
		inputStateChange[i] = false;
		prevJoystickStates[i].hat = SDL_HAT_CENTERED;
		prevJoystickStates[i].stickHeldUp = false;
		prevJoystickStates[i].stickHeldDown = false;
		prevJoystickStates[i].stickHeldLeft = false;
		prevJoystickStates[i].stickHeldRight = false;
		prevJoystickStates[i].stickHeldHardUp = false;
		prevJoystickStates[i].stickHeldHardDown = false;
		prevJoystickStates[i].stickHeldHardLeft = false;
		prevJoystickStates[i].stickHeldHardRight = false;
	}
}

int Main::Execute()
{
	int error = 0;
	if((error = Initialize()) != 0)
	{
		return error;
	}

	UpdateLog("Game initialization complete.", false);

	SDL_Event curEvent;

	UpdateLog("Starting main game loop.", false);

	while(notDone)
	{
		int thisFrameTicks = SDL_GetTicks();
		if(thisFrameTicks - lastFrameTicks >= 1000 / TARGET_FPS)
		{
			lastFrameTicks = thisFrameTicks;
			if((error = AdvanceHolds()) != 0) { return error; }
			if((error = Event(&curEvent)) != 0) { return error; }
			if((error = Update()) != 0) { return error; }
			if((error = Collide()) != 0) { return error; }
			if((error = SpawnObjects()) != 0) { return error; }
			if((error = PlayAudio()) != 0) { return error; }
			if((error = DeleteObjects()) != 0) { return error; }
			if((error = Render()) != 0) { return error; }

			frame++;
		}
	}

	UpdateLog("Main game loop exited.", false);

	Cleanup();

	UpdateLog("Cleanup complete.", false);

	return 0;
}

int Main::SetBestGameResolution()
{
	//get the current screen resolution
	const SDL_VideoInfo * vidInfo = SDL_GetVideoInfo();
	screenResolutionX = vidInfo->current_w;
	screenResolutionY = vidInfo->current_h;
	gameResolutionX = MAX_GAME_RESOLUTION_X;
	gameResolutionY = MAX_GAME_RESOLUTION_Y;

	//if the current screen resolution is larger than or equal to the max possible resolution, then just use the max resolution
	if(screenResolutionX >= MAX_GAME_RESOLUTION_X && screenResolutionY >= MAX_GAME_RESOLUTION_Y)
	{
		return 0;
	}

	//pick a viable 16:9 resolution that's equal to or smaller than the screen resolution
	while(gameResolutionX > screenResolutionX || gameResolutionY > screenResolutionY)
	{
		gameResolutionX -= GAME_ASPECT_RATIO_X;
		gameResolutionY -= GAME_ASPECT_RATIO_Y;

		if(gameResolutionX < MIN_GAME_RESOLUTION_X || gameResolutionY < MIN_GAME_RESOLUTION_Y)
		{
			UpdateLog("Screen resolution too small.", true);
			return -1;
		}
	}

	return 0;
}

void AudioCallback(void *unused, Uint8 *stream, int len)
{
    Uint32 amount;

	list<CurrentAudioEntry*> toRemove;

	list<CurrentAudioEntry*>::iterator caIt;
	for ( caIt=currentAudio.begin(); caIt != currentAudio.end(); caIt++)
	{
		amount = ((*caIt)->aud->length - (*caIt)->curPosition);
        if ( amount > len )
		{
            amount = len;
        }
		SDL_MixAudio(stream, &(*caIt)->aud->data[(*caIt)->curPosition], amount, SDL_MIX_MAXVOLUME);
		(*caIt)->curPosition += amount;

		if((*caIt)->curPosition >= (*caIt)->aud->length)
		{
			toRemove.push_back((*caIt));
		}
	}

	//remove entries that need to be removed, if any
	for ( caIt=toRemove.begin(); caIt != toRemove.end(); caIt++)
	{
		currentAudio.remove((*caIt));
		delete((*caIt));
	}
}

int Main::Initialize()
{
	int error = 0;
	if((error = SDL_Init(SDL_INIT_EVERYTHING)) != 0)
	{
		return error;
	}

	UpdateLog("SDL Initialized.", false);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,        8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,      8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,       8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,      8);
 
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,      16);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,        32);
 
	SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,    8);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,    8);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,    8);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE,    8);
 
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  2);

	SDL_ShowCursor(SDL_DISABLE);

	UpdateLog("SDL attributes set.", false);

	//load configuration
	if(int error = LoadConfig() != 0)
	{
		return error;
	}

	UpdateLog("Configuration loaded.", false);

	//set up SDL screen
	if(int error = SetFullScreen(fullScreen) != 0)
	{
		return error;
	}

	////set up audio
	SDL_AudioSpec *desired; 

	/* Allocated audio specs */ 
	desired=(SDL_AudioSpec *)malloc(sizeof(SDL_AudioSpec)); 
	obtainedAudioSpec=(SDL_AudioSpec *)malloc(sizeof(SDL_AudioSpec)); 

	/* Set desired format */ 
	desired->freq=22050; 
	desired->format=AUDIO_S16; 
	desired->channels=1;
	desired->samples=1024; 
	desired->callback=AudioCallback; 
	desired->userdata=NULL; 

	/* Open the audio device */ 
	int blah = SDL_OpenAudio(desired, obtainedAudioSpec);
	if ( blah < 0 )
	{ 
		UpdateLog("Failed to open audio device.", false);
		return -1; //problem opening the audio
	} 
	free(desired); 

	//let audio start running
	SDL_PauseAudio(0);

	//make sure we start at the beginning!
	if(int i = ChangeGameState(MAIN_MENU) != 0) { return i; }

	//get joysticks
	for(int i = 0; i < SDL_NumJoysticks(); i++)
	{
		SDL_Joystick * stick = SDL_JoystickOpen(i);
		sticks.push_back(stick);
	}

	return 0;
}

int Main::LoadDefinition(string defFilePath, list<HSObject*> * objects, HSObject ** returnValue)
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
		UpdateLog("XML file is not Homestrife definition file: " + defFilePath, true);
		return -1; //this isn't an HSObjects definition file
	}

	//loop through all the object definitions in the xml file
	for(XMLElement * i = file->RootElement()->FirstChildElement(); i != NULL; i = i->NextSiblingElement())
	{
		//get the object type
		const char * objectType = i->Value();

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

		//execute the object's local definition code
		//string currentWorkingDirectory = GetCurrentWorkingDirectory(NULL, 0);
		//string defFileDirectory = CreateAbsolutePath(currentWorkingDirectory, defFilePath);
		//defFileDirectory = defFileDirectory.substr(0, defFileDirectory.find_last_of("\\"));
		string defFileDirectory = defFilePath.substr(0, defFilePath.find_last_of("\\"));
		if(int error = newObject->Define(i, defFileDirectory, &textureRegistry, &paletteRegistry, &audioRegistry, obtainedAudioSpec, openGL3) != 0)
		{
			return error; //there was an error defining the object
		}

		//give the new object a unique id
		newObject->id = newObjectId;
		newObjectId++;

		//add the object to the main list of objects
		if(objects != NULL)
		{
			objects->push_back(newObject);
		}

		//save the last object created as the return value
		if(returnValue != NULL)
		{
			*returnValue = newObject;
		}

		//run through all the object's holds and create any spawn objects found
		HSObject * spawnObject = NULL;
		HSObjectHold * curHold = newObject->firstHold;
		while(curHold != NULL)
		{
			list<SpawnObject>::iterator spawnIt;
			for(spawnIt = curHold->spawnObjects.begin(); spawnIt != curHold->spawnObjects.end(); spawnIt++)
			{
				if(int error = LoadDefinition(spawnIt->defFilePath, NULL, &spawnObject) != 0) { return error; }
				spawnObject->curHold = spawnObject->firstHold;
				spawnIt->object = spawnObject;
				spawnObject = NULL;
			}

			curHold = curHold->nextListHold;
		}

		if(newObject->IsHUD())
		{
			HUD * hud = (HUD*)newObject;
			//load the meters and counters
			if(int error = LoadDefinition(hud->healthUnderMeterFilePath, objects, &(hud->healthUnderMeter)) != 0) { return error; }
			hud->healthUnderMeter->curHold = hud->healthUnderMeter->firstHold;

			if(int error = LoadDefinition(hud->healthMeterFilePath, objects, &(hud->healthMeter)) != 0) { return error; }
			hud->healthMeter->curHold = hud->healthMeter->firstHold;

			if(int error = LoadDefinition(hud->counterFilePath, objects, &(hud->onesCounter)) != 0) { return error; }
			hud->onesCounter->curHold = hud->onesCounter->firstHold;

			if(int error = LoadDefinition(hud->counterFilePath, objects, &(hud->tensCounter)) != 0) { return error; }
			hud->tensCounter->curHold = hud->tensCounter->firstHold;
		}
	}

	delete file;

	return 0;
}

int Main::Update()
{
	if(gameState == MAIN_MENU)
	{
		if(int i = UpdateMainMenu() != 0) { return i; }
	}
	else if(gameState == CHARACTER_SELECT)
	{
		if(int i = UpdateCharacterSelect() != 0) { return i; }
	}

	list<HSObject*>::iterator objIt;
	for ( objIt=HUDObjects.begin(); objIt != HUDObjects.end(); objIt++)
	{
		if(int error = (*objIt)->Update() != 0)
		{
			return error; //there was an error updating the object
		}
	}

	if(gameState == MATCH && matchState != IN_PROGRESS) { return 0; }

	for ( objIt=gameObjects.begin(); objIt != gameObjects.end(); objIt++)
	{
		if(int error = (*objIt)->Update() != 0)
		{
			return error; //there was an error updating the object
		}
	}

	return 0;
}

int Main::Collide()
{
	if(gameState == MATCH && matchState != IN_PROGRESS) { return 0; }

	//first handle terrain collisions
	list<HSObject*>::iterator objIt = gameObjects.begin();
	for ( objIt=gameObjects.begin(); objIt != gameObjects.end(); objIt++)
	{
		//pass the location of the list of game objects into each object's CollideTerrain function
		if(int error = (*objIt)->CollideTerrain(&gameObjects) != 0)
		{
			return error; //there was an error doing terrain collisions
		}
	}

	//next, handle collisions between fighter terrain boxes
	for (objIt=gameObjects.begin(); objIt != gameObjects.end(); objIt++)
	{
		if((*objIt)->IsFighter())
		{
			if(int error = ((Fighter*)*objIt)->CollideFighters(&gameObjects) != 0)
			{
				return error; //there was an error doing terrain collisions
			}
		}
	}
	
	//now, handle all attack/hurt box collision
	for ( objIt=gameObjects.begin(); objIt != gameObjects.end(); objIt++)
	{
		//pass the location of the list of game objects into each object's CollideAttack function
		if(int error = (*objIt)->CollideAttack(&gameObjects) != 0)
		{
			return error; //there was an error doing Hit/Hurt collisions
		}
	}

	//finally, apply all the results of the attack collision phase
	for ( objIt=gameObjects.begin(); objIt != gameObjects.end(); objIt++)
	{
		(*objIt)->ApplyAttackResults();
	}

	if(gameState == MATCH)
	{
		if(int error = CollideMatch()!= 0) { return error; }
	}

	return 0;
}

int Main::SpawnObjects()
{
	list<HSObject*>::iterator objIt;
	for ( objIt=gameObjects.begin(); objIt != gameObjects.end(); objIt++)
	{
		list<SpawnObject*> spawnObjects = (*objIt)->GetSpawnObjects();

		list<SpawnObject*>::iterator spawnIt;
		for(spawnIt = spawnObjects.begin(); spawnIt != spawnObjects.end(); spawnIt++)
		{
			if((*spawnIt)->object == NULL) { continue; }

			int hFlip = 1;
			if((*objIt)->hFlip) { hFlip = -1; }

			HSObject * spawnObject = (*spawnIt)->object->Clone();
			spawnObject->hFlip = (*objIt)->hFlip;
			spawnObject->parent = (*objIt);
			spawnObject->pos.x = (*objIt)->pos.x + (*spawnIt)->parentOffset.x * hFlip;
			spawnObject->pos.y = (*objIt)->pos.y + (*spawnIt)->parentOffset.y;
			spawnObject->prevPos.x = spawnObject->pos.x;
			spawnObject->prevPos.y = spawnObject->pos.y;
			spawnObject->vel.x = (*spawnIt)->vel.x * hFlip;
			spawnObject->vel.y = (*spawnIt)->vel.y;
			spawnObject->followParent = (*spawnIt)->followParent;
			if(spawnObject->IsTerrainObject())
			{
				((TerrainObject*)spawnObject)->collideParent = (*spawnIt)->collideParent;
			}
			spawnObject->ChangeHold(spawnObject->firstHold);
			spawnObject->id = newObjectId;
			newObjectId++;

			gameObjects.push_back(spawnObject);
		}
	}

	return 0;
}

int Main::Render()
{
	//adjust the camera
 
	//get the current screen resolution
	if(zoomOut < 1) { zoomOut = 1; }

	float targetZoomOut = 1;
	HSVect2D targetFocusPos;
	targetFocusPos.x = 0;
	targetFocusPos.y = 0;
	if(focusObjectOne != NULL && focusObjectTwo != NULL)
	{
		//get the location between the two objects
		HSVect2D posActualOne;
		posActualOne.x = focusObjectOne->pos.x;
		posActualOne.y = focusObjectOne->pos.y;
		HSVect2D posActualTwo;
		posActualTwo.x = focusObjectTwo->pos.x;
		posActualTwo.y = focusObjectTwo->pos.y;

		if(focusObjectOne->IsFighter())
		{
			Fighter * f = (Fighter *)focusObjectOne;
			posActualOne.x = posActualOne.x + f->firstUprightTerrainBox->offset.x + f->firstUprightTerrainBox->width / 2;
			posActualOne.y = posActualOne.y + f->firstUprightTerrainBox->offset.y + f->firstUprightTerrainBox->height / 2;
		}

		if(focusObjectTwo->IsFighter())
		{
			Fighter * f = (Fighter *)focusObjectTwo;
			posActualTwo.x = posActualTwo.x + f->firstUprightTerrainBox->offset.x + f->firstUprightTerrainBox->width / 2;
			posActualTwo.y = posActualTwo.y + f->firstUprightTerrainBox->offset.y + f->firstUprightTerrainBox->height / 2;
		}

		HSVect2D posDiff;
		posDiff.x = posActualTwo.x - posActualOne.x;
		posDiff.y = posActualTwo.y - posActualOne.y;

		targetFocusPos.x = posActualOne.x + posDiff.x / 2;
		targetFocusPos.y = posActualOne.y + posDiff.y / 2;

		//get the zoom
		float xZoomOut = 1;
		float yZoomOut = 1;
		if(abs(posDiff.x) + ZOOM_BOUNDARY_X_THRESHOLD * 2 > MAX_GAME_RESOLUTION_X)
		{
			float minRes = MAX_GAME_RESOLUTION_X;
			float neededRes = abs(posDiff.x) + ZOOM_BOUNDARY_X_THRESHOLD * 2;
			xZoomOut = neededRes / minRes;
		}
		if(abs(posDiff.y) + ZOOM_BOUNDARY_Y_THRESHOLD * 2 > MAX_GAME_RESOLUTION_Y)
		{
			float minRes = MAX_GAME_RESOLUTION_Y;
			float neededRes = abs(posDiff.y) + ZOOM_BOUNDARY_Y_THRESHOLD * 2;
			yZoomOut = neededRes / minRes;
		}
		if(xZoomOut > yZoomOut) { targetZoomOut = xZoomOut; }
		else { targetZoomOut = yZoomOut; }

		if(targetZoomOut < 1) { targetZoomOut = 1; }
	}
	else if(focusObjectOne != NULL)
	{
		targetFocusPos.x = focusObjectOne->pos.x;
		targetFocusPos.y = focusObjectOne->pos.y;

		if(focusObjectOne->IsFighter())
		{
			//get the centerpoint of upright terrain box
			Fighter * f = (Fighter *)focusObjectOne;
			targetFocusPos.x = targetFocusPos.x + f->firstUprightTerrainBox->offset.x + f->firstUprightTerrainBox->width / 2;
			targetFocusPos.y = targetFocusPos.y + f->firstUprightTerrainBox->offset.y + f->firstUprightTerrainBox->height / 2;
		}
	}
	else if(focusObjectTwo != NULL)
	{
		targetFocusPos.x = focusObjectTwo->pos.x;
		targetFocusPos.y = focusObjectTwo->pos.y;

		if(focusObjectTwo->IsFighter())
		{
			//get the centerpoint of upright terrain box
			Fighter * f = (Fighter *)focusObjectTwo;
			targetFocusPos.x = targetFocusPos.x + f->firstUprightTerrainBox->offset.x + f->firstUprightTerrainBox->width / 2;
			targetFocusPos.y = targetFocusPos.y + f->firstUprightTerrainBox->offset.y + f->firstUprightTerrainBox->height / 2;
		}
	}

	if(focusPos.x != targetFocusPos.x || focusPos.y != targetFocusPos.y)
	{
		HSVect2D posDiff;
		posDiff.x = targetFocusPos.x - focusPos.x;
		posDiff.y = targetFocusPos.y - focusPos.y;

		HSVectComp diffLength = sqrt(pow(posDiff.x, 2) + pow(posDiff.y, 2));
		if(abs(diffLength) <= PAN_SPEED)
		{
			//distance is shorter than pan speed, so just move to the target position
			focusPos.x = targetFocusPos.x;
			focusPos.y = targetFocusPos.y;
			zoomOut = targetZoomOut;
		}
		else
		{
			HSVect2D diffNormal;
			diffNormal.x = posDiff.x / diffLength;
			diffNormal.y = posDiff.y / diffLength;

			HSVect2D posChange;
			posChange.x = diffNormal.x * PAN_SPEED;
			posChange.y = diffNormal.y * PAN_SPEED;

			focusPos.x += posChange.x;
			focusPos.y += posChange.y;

			HSVectComp changeLength = sqrt(pow(posChange.x, 2) + pow(posChange.y, 2));
			float zoomRatio = changeLength / diffLength;

			float zoomDiff = targetZoomOut - zoomOut;
			zoomOut += zoomDiff * zoomRatio;
		}
	}
	else
	{
		zoomOut = targetZoomOut;
	}

	//float left = focusPos.x - ((MAX_GAME_RESOLUTION_X * zoomOut) / 2);
	//float right = focusPos.x + ((MAX_GAME_RESOLUTION_X * zoomOut) / 2);
	//float bottom = focusPos.y + ((MAX_GAME_RESOLUTION_Y * zoomOut) / 2);
	//float top = focusPos.y - ((MAX_GAME_RESOLUTION_Y * zoomOut) / 2);

	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glOrtho(left, right, bottom, top, 1, -1);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	
	glUseProgram(shader_progIndexed);
	glUniform2f(indexedFocusPosLoc, focusPos.x, focusPos.y);
	glUniform1f(indexedZoomOutLoc, zoomOut);
	
	glUseProgram(shader_progNonIndexed);
	glUniform2f(nonIndexedFocusPosLoc, focusPos.x, focusPos.y);
	glUniform1f(nonIndexedZoomOutLoc, zoomOut);

	glUseProgram(0);
	
	//render objects
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glEnableClientState(GL_INDEX_ARRAY);

	list<HSObject*>::iterator objIt;
	for ( objIt=gameObjects.begin(); objIt != gameObjects.end(); objIt++)
	{
		if(!(*objIt)->visible) { continue; }

		list<TextureInstance>::iterator texIt;
		for ( texIt=(*objIt)->curHold->textures.begin(); texIt != (*objIt)->curHold->textures.end(); texIt++)
		{
			RenderTexture((*objIt), (*texIt));
		}
	}

	//left = MAX_GAME_RESOLUTION_X / -2;
	//right = MAX_GAME_RESOLUTION_X / 2;
	//bottom = MAX_GAME_RESOLUTION_Y / 2;
	//top = MAX_GAME_RESOLUTION_Y / -2;

	//render HUD
	//glPushMatrix();
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glOrtho(left, right, bottom, top, 1, -1);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	
	glUseProgram(shader_progIndexed);
	glUniform2f(indexedFocusPosLoc, 0.0f, 0.0f);
	glUniform1f(indexedZoomOutLoc, 1.0f);
	
	glUseProgram(shader_progNonIndexed);
	glUniform2f(nonIndexedFocusPosLoc, 0.0f, 0.0f);
	glUniform1f(nonIndexedZoomOutLoc, 1.0f);

	glUseProgram(0);

	for ( objIt=HUDObjects.begin(); objIt != HUDObjects.end(); objIt++)
	{
		if(!(*objIt)->visible) { continue; }

		list<TextureInstance>::iterator texIt;
		for ( texIt=(*objIt)->curHold->textures.begin(); texIt != (*objIt)->curHold->textures.end(); texIt++)
		{
			RenderTexture((*objIt), (*texIt));
		}
	}

	//glPopMatrix();

	//glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//glDisableClientState(GL_INDEX_ARRAY);

	SDL_GL_SwapBuffers();

	GLenum glError = glGetError();
	if(glError != GL_NO_ERROR)
	{
		string glErrorString = "OpenGL error in Render().";
		UpdateLog(glErrorString, true);
	}

	return 0;
}

int Main::RenderTexture(HSObject * obj, TextureInstance tex)
{
	//get the actual texture offset, based on the object's current hFlip
	float hScale = tex.hScale;
	float vScale = tex.vScale;
	if(obj->hFlip)
	{
		hScale *= -1;
	}

	//get the offset
	float offsetX = obj->pos.x;
	if(obj->hFlip)
	{
		offsetX += tex.offset.x * -1;
	}
	else
	{
		offsetX += tex.offset.x;
	}
	float offsetY = obj->pos.y + tex.offset.y;

	//set up the texture
	if(tex.hsTex->indexed)
	{
		glUseProgram(shader_progIndexed);
		glUniform2f(indexedPosOffsetLoc, offsetX, offsetY);
		glUniform2f(indexedScaleLoc, hScale, vScale);
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(indexedTexLoc, 0);
		glBindTexture(GL_TEXTURE_2D, tex.hsTex->textureID);
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		//set up the palette
		glActiveTexture(GL_TEXTURE1);
		glUniform1i(paletteLoc, 1);
		if(obj->palette == NULL || obj->useTGAPalettes)
		{
			glBindTexture(GL_TEXTURE_2D, tex.hsTex->ownPalette->textureID);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, obj->palette->textureID);
		}
		glPixelStorei(GL_PACK_ALIGNMENT, 4);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}
	else
	{
		glUseProgram(shader_progNonIndexed);
		glUniform2f(nonIndexedPosOffsetLoc, offsetX, offsetY);
		glUniform2f(nonIndexedScaleLoc, hScale, vScale);
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(nonIndexedTexLoc, 0);
		glBindTexture(GL_TEXTURE_2D, tex.hsTex->textureID);
		glPixelStorei(GL_PACK_ALIGNMENT, 4);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}
	
	//glLoadIdentity();
	//glTranslatef(offsetX, offsetY, 0.0);
	//glScalef(hScale, vScale, 1.0);
	
	if(openGL3)
	{
		//let's just try some cheap old shit again
		//glBindBuffer(GL_ARRAY_BUFFER, tex.hsTex->bufferID);
		//glVertexPointer(3, GL_FLOAT, 0, (void*)0);

		//get the vertex array set up
		glBindVertexArray(tex.hsTex->vaoID);
		
		//glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferID);
		//glTexCoordPointer(2, GL_FLOAT, 0, (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBufferID);
	}
	else
	{
		GLenum positionLoc;

		if(tex.hsTex->indexed)
		{
			positionLoc = positionLocIndexed;
		}
		else
		{
			positionLoc = positionLocNonIndexed;
		}

		glBindBuffer(GL_ARRAY_BUFFER, tex.hsTex->bufferID);
		glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//draw stuff
	//glDrawArrays(GL_QUADS, 0, 4);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0);

	//unbind stuff
	if(openGL3)
	{
		glBindVertexArray(0);
	}

	glUseProgram(0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	return 0;
}

int Main::PlayAudio()
{
	if(gameState == MATCH && matchState != IN_PROGRESS) { return 0; }

	list<HSObject*>::iterator objIt;
	for ( objIt=gameObjects.begin(); objIt != gameObjects.end(); objIt++)
	{
		list<AudioInstance*> audio = (*objIt)->GetAudio();

		list<AudioInstance*>::iterator audIt;
		for ( audIt=audio.begin(); audIt != audio.end(); audIt++)
		{
			if((*audIt)->exclusive)
			{
				list<CurrentAudioEntry*> toRemove;

				//search for other exclusive sounds that orginated from this object
				list<CurrentAudioEntry*>::iterator curAudIt;
				for(curAudIt = currentAudio.begin(); curAudIt != currentAudio.end(); curAudIt++)
				{
					if((*curAudIt)->exclusive && (*curAudIt)->sourceObj == (*objIt))
					{
						toRemove.push_back((*curAudIt));
					}
				}

				//remove entries that need to be removed, if any
				for ( curAudIt=toRemove.begin(); curAudIt != toRemove.end(); curAudIt++)
				{
					currentAudio.remove((*curAudIt));
					delete((*curAudIt));
				}
			}

			CurrentAudioEntry * newEntry = new CurrentAudioEntry();
			newEntry->aud = (*audIt)->hsAud;
			newEntry->curPosition = 0;
			newEntry->sourceObj = (*objIt);
			newEntry->exclusive = (*audIt)->exclusive;

			SDL_LockAudio();
			currentAudio.push_back(newEntry);
			SDL_UnlockAudio();
		}
	}

	return 0;
}

int Main::DeleteObjects()
{
	list<HSObject*>::iterator objIt = gameObjects.begin();
	while (objIt != gameObjects.end())
	{
		if((*objIt)->toDelete)
		{
			ClearSpecificObject(*objIt);
			objIt = gameObjects.erase(objIt);
		}
		else
		{
			objIt++;
		}
	}

	return 0;
}

int Main::Cleanup()
{
	ClearAllObjects();

	if(shader_vert != 0)
	{
		glDeleteShader(shader_vert);
		shader_vert = 0;
	}
	if(shader_fragNonIndexed != 0)
	{
		glDeleteShader(shader_fragNonIndexed);
		shader_fragNonIndexed = 0;
	}
	if(shader_fragIndexed != 0)
	{
		glDeleteShader(shader_fragIndexed);
		shader_fragIndexed = 0;
	}
	if(shader_progNonIndexed != 0)
	{
		glDeleteProgram(shader_progNonIndexed);
		shader_progNonIndexed = 0;
	}
	if(shader_progIndexed != 0)
	{
		glDeleteProgram(shader_progIndexed);
		shader_progIndexed = 0;
	}

	list<SDL_Joystick*>::iterator it2;
	for ( it2=sticks.begin(); it2 != sticks.end(); it2++)
	{
		SDL_JoystickClose(*it2);
	}
	sticks.clear();

	SDL_Quit();

	return 0;
}

int Main::ClearAllObjects()
{
	SDL_LockAudio();

	list<HSObject*>::iterator objIt;
	for ( objIt=gameObjects.begin(); objIt != gameObjects.end(); objIt++)
	{
		delete (*objIt);
	}

	gameObjects.clear();
	
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

	focusObjectOne = NULL;
	focusObjectTwo = NULL;

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		players[i] = NULL;
		playerHUDs[i] = NULL;
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
	selectPaletteOne = NULL;
	selectPaletteTwo = NULL;
	selectPaletteLeftOne = NULL;
	selectPaletteLeftTwo = NULL;
	selectPaletteRightOne = NULL;
	selectPaletteRightTwo = NULL;

	return 0;
}

int Main::ClearSpecificObject(HSObject* object)
{
	if(focusObjectOne == object) { focusObjectOne = NULL; }
	if(focusObjectTwo == object) { focusObjectTwo = NULL; }

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(players[i] == object) { players[i] = NULL; }
		if(playerHUDs[i] == object) { playerHUDs[i] = NULL; }
	}

	delete object;

	return 0;
}

int Main::ChangeGameState(GameState newState)
{
	ClearAllObjects();

	gameState = newState;

	StartLoading();

	switch(newState)
	{
	case MAIN_MENU:
		if(int i = InitializeMainMenu() != 0) { return i; }
		break;
	case CHARACTER_SELECT:
		if(int i = InitializeCharacterSelect() != 0) { return i; }
		break;
	case MATCH:
		if(int i = InitializeMatch() != 0) { return i; }
		break;
	}

	if(focusObjectOne != NULL && focusObjectTwo != NULL)
	{
		//get the location between the two objects
		HSVect2D posActualOne;
		posActualOne.x = focusObjectOne->pos.x;
		posActualOne.y = focusObjectOne->pos.y;
		HSVect2D posActualTwo;
		posActualTwo.x = focusObjectTwo->pos.x;
		posActualTwo.y = focusObjectTwo->pos.y;

		if(focusObjectOne->IsFighter())
		{
			Fighter * f = (Fighter *)focusObjectOne;
			posActualOne.x = posActualOne.x + f->firstUprightTerrainBox->offset.x + f->firstUprightTerrainBox->width / 2;
			posActualOne.y = posActualOne.y + f->firstUprightTerrainBox->offset.y + f->firstUprightTerrainBox->height / 2;
		}

		if(focusObjectTwo->IsFighter())
		{
			Fighter * f = (Fighter *)focusObjectTwo;
			posActualTwo.x = posActualTwo.x + f->firstUprightTerrainBox->offset.x + f->firstUprightTerrainBox->width / 2;
			posActualTwo.y = posActualTwo.y + f->firstUprightTerrainBox->offset.y + f->firstUprightTerrainBox->height / 2;
		}

		HSVect2D posDiff;
		posDiff.x = posActualTwo.x - posActualOne.x;
		posDiff.y = posActualTwo.y - posActualOne.y;

		focusPos.x = posActualOne.x + posDiff.x / 2;
		focusPos.y = posActualOne.y + posDiff.y / 2;

		//get the zoom
		float xZoomOut = 1;
		float yZoomOut = 1;
		if(abs(posDiff.x) + ZOOM_BOUNDARY_X_THRESHOLD * 2 > MAX_GAME_RESOLUTION_X)
		{
			float minRes = MAX_GAME_RESOLUTION_X;
			float neededRes = abs(posDiff.x) + ZOOM_BOUNDARY_X_THRESHOLD * 2;
			xZoomOut = neededRes / minRes;
		}
		if(abs(posDiff.y) + ZOOM_BOUNDARY_Y_THRESHOLD * 2 > MAX_GAME_RESOLUTION_Y)
		{
			float minRes = MAX_GAME_RESOLUTION_Y;
			float neededRes = abs(posDiff.y) + ZOOM_BOUNDARY_Y_THRESHOLD * 2;
			yZoomOut = neededRes / minRes;
		}
		if(xZoomOut > yZoomOut) { zoomOut = xZoomOut; }
		else { zoomOut = yZoomOut; }

		if(zoomOut < 1) { zoomOut = 1; }
	}
	else if(focusObjectOne != NULL)
	{
		focusPos.x = focusObjectOne->pos.x;
		focusPos.y = focusObjectOne->pos.y;

		if(focusObjectOne->IsFighter())
		{
			//get the centerpoint of upright terrain box
			Fighter * f = (Fighter *)focusObjectOne;
			focusPos.x = focusPos.x + f->firstUprightTerrainBox->offset.x + f->firstUprightTerrainBox->width / 2;
			focusPos.y = focusPos.y + f->firstUprightTerrainBox->offset.y + f->firstUprightTerrainBox->height / 2;
		}
	}

	EndLoading();

	return 0;
}

int Main::StartLoading()
{
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\loading\\loading.xml"), &HUDObjects, &loading) != 0) { return error; }
	loading->curHold = loading->firstHold;
	loading->pos.x = -90;
	loading->pos.y = -18;

	//refresh the screen
	return Render();
}

int Main::EndLoading()
{
	//get rid of the loading graphic, which is guaranteed to be in the front
	HUDObjects.pop_front();
	delete loading;
	loading = NULL;

	return 0;
}

int Main::InitializeMainMenu()
{
	//load all main menu elements and place them in HUDObjects
	HSObject * newObject;
	
	Menu * playerKeyConfigMenu = new Menu(MAIN_MENU_HEADER_HEIGHT, MAIN_MENU_CURSOR_WIDTH, MAIN_MENU_ITEM_HEIGHT, MAIN_MENU_ITEM_SPACING);
	playerKeyConfigMenu->pos.x = (MAX_GAME_RESOLUTION_X / -2) + MAIN_MENU_ITEM_SPACING;
	playerKeyConfigMenu->pos.y = (MAX_GAME_RESOLUTION_Y / -2) + MAIN_MENU_ITEM_SPACING;

	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\cursor\\cursor.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerKeyConfigMenu->SetCursor(newObject);
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\player\\player1.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerKeyConfigMenu->SetHeader(newObject);
	playerOne = newObject;
	newObject->visible = false;
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\player\\player2.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerTwo = newObject;
	newObject->visible = false;
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\player\\player3.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerThree = newObject;
	newObject->visible = false;
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\player\\player4.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerFour = newObject;
	newObject->visible = false;
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\up\\up.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerKeyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\down\\down.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerKeyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\left\\left.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerKeyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\right\\right.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerKeyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\lightAttack\\lightAttack.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerKeyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\heavyAttack\\heavyAttack.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerKeyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\jump\\jump.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerKeyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\block\\block.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerKeyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\pause\\pause.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerKeyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\menuConfirm\\menuConfirm.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerKeyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\menuBack\\menuBack.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerKeyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\back\\back.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerKeyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject));
	
	Menu * videoMenu = new Menu(MAIN_MENU_HEADER_HEIGHT, MAIN_MENU_CURSOR_WIDTH, MAIN_MENU_ITEM_HEIGHT, MAIN_MENU_ITEM_SPACING);
	videoMenu->pos.x = (MAX_GAME_RESOLUTION_X / -2) + MAIN_MENU_ITEM_SPACING;
	videoMenu->pos.y = (MAX_GAME_RESOLUTION_Y / -2) + MAIN_MENU_ITEM_SPACING;

	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\cursor\\cursor.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	videoMenu->SetCursor(newObject);
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\video\\video.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	videoMenu->SetHeader(newObject);
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\toggleFullscreen\\toggleFullscreen.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	videoMenu->AddItem(menuManager->MakeMenuItem(newObject));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\back\\back.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	videoMenu->AddItem(menuManager->MakeMenuItem(newObject));
	
	Menu * keyConfigMenu = new Menu(MAIN_MENU_HEADER_HEIGHT, MAIN_MENU_CURSOR_WIDTH, MAIN_MENU_ITEM_HEIGHT, MAIN_MENU_ITEM_SPACING);
	keyConfigMenu->pos.x = (MAX_GAME_RESOLUTION_X / -2) + MAIN_MENU_ITEM_SPACING;
	keyConfigMenu->pos.y = (MAX_GAME_RESOLUTION_Y / -2) + MAIN_MENU_ITEM_SPACING;

	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\cursor\\cursor.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	keyConfigMenu->SetCursor(newObject);
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\keyConfig\\keyConfig.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	keyConfigMenu->SetHeader(newObject);
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\player\\player1.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	keyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject, playerKeyConfigMenu));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\player\\player2.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	keyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject, playerKeyConfigMenu));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\player\\player3.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	keyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject, playerKeyConfigMenu));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\player\\player4.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	keyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject, playerKeyConfigMenu));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\back\\back.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	keyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject));

	Menu * versusMenu = new Menu(MAIN_MENU_HEADER_HEIGHT, MAIN_MENU_CURSOR_WIDTH, MAIN_MENU_ITEM_HEIGHT, MAIN_MENU_ITEM_SPACING);
	versusMenu->pos.x = (MAX_GAME_RESOLUTION_X / -2) + MAIN_MENU_ITEM_SPACING;
	versusMenu->pos.y = (MAX_GAME_RESOLUTION_Y / -2) + MAIN_MENU_ITEM_SPACING;

	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\cursor\\cursor.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	versusMenu->SetCursor(newObject);
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\versus\\versus.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	versusMenu->SetHeader(newObject);

	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\freeForAll\\freeForAll.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	versusMenu->AddItem(menuManager->MakeMenuItem(newObject));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\back\\back.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	versusMenu->AddItem(menuManager->MakeMenuItem(newObject));
	
	Menu * optionsMenu = new Menu(MAIN_MENU_HEADER_HEIGHT, MAIN_MENU_CURSOR_WIDTH, MAIN_MENU_ITEM_HEIGHT, MAIN_MENU_ITEM_SPACING);
	optionsMenu->pos.x = (MAX_GAME_RESOLUTION_X / -2) + MAIN_MENU_ITEM_SPACING;
	optionsMenu->pos.y = (MAX_GAME_RESOLUTION_Y / -2) + MAIN_MENU_ITEM_SPACING;

	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\cursor\\cursor.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	optionsMenu->SetCursor(newObject);
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\options\\options.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	optionsMenu->SetHeader(newObject);
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\video\\video.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	optionsMenu->AddItem(menuManager->MakeMenuItem(newObject, videoMenu));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\keyConfig\\keyConfig.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	optionsMenu->AddItem(menuManager->MakeMenuItem(newObject, keyConfigMenu));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\back\\back.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	optionsMenu->AddItem(menuManager->MakeMenuItem(newObject));

	Menu * mainMenu = new Menu(MAIN_MENU_HEADER_HEIGHT, MAIN_MENU_CURSOR_WIDTH, MAIN_MENU_ITEM_HEIGHT, MAIN_MENU_ITEM_SPACING);
	mainMenu->pos.x = (MAX_GAME_RESOLUTION_X / -2) + MAIN_MENU_ITEM_SPACING;
	mainMenu->pos.y = (MAX_GAME_RESOLUTION_Y / -2) + MAIN_MENU_ITEM_SPACING;

	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\cursor\\cursor.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	mainMenu->SetCursor(newObject);
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\mainMenu\\mainMenu.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	mainMenu->SetHeader(newObject);
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\versus\\versus.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	mainMenu->AddItem(menuManager->MakeMenuItem(newObject, versusMenu));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\options\\options.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	mainMenu->AddItem(menuManager->MakeMenuItem(newObject, optionsMenu));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\quit\\quitGame.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	mainMenu->AddItem(menuManager->MakeMenuItem(newObject, optionsMenu));

	menuManager = new MenuManager(mainMenu);
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\pressDesiredButton\\pressDesiredButton.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	pressDesiredButton = newObject;
	newObject->visible = false;

	ChangeMainMenuState(TOP);

	return 0;
}

int Main::ChangeMainMenuState(MainMenuState newState)
{
	mainMenuState = newState;

	switch(newState)
	{
	case TOP:
		break;
	case VERSUS:
		break;
	case OPTIONS:
		break;
	case VIDEO:
		break;
	case KEY_CONFIG:
		break;
	case PLAYER_KEY_CONFIG:
		pressDesiredButton->visible = false;
		switch(playerToSetUp)
		{
		case 0:
			menuManager->SetHeader(playerOne);
			break;
		case 1:
			menuManager->SetHeader(playerTwo);
			break;
		case 2:
			menuManager->SetHeader(playerThree);
			break;
		case 3:
			menuManager->SetHeader(playerFour);
			break;
		}
		break;
	case INPUT_KEY:
		pressDesiredButton->pos.x = menuManager->GetCursorPos().x + PRESS_DESIRED_BUTTON_OFFSET_X;
		pressDesiredButton->pos.y = menuManager->GetCursorPos().y;
		pressDesiredButton->visible = true;
		break;
	}

	return 0;
}

int Main::EventMainMenu(InputStates * inputHistory, int frame)
{
	if(inputHistory->frame == frame && (inputHistory->bButtonDown.pressed || inputHistory->bHatDown.pressed || inputHistory->bKeyDown.pressed || inputHistory->bStickDown.pressed))
	{
		menuManager->CursorNext();
	}
	else if(inputHistory->frame == frame && (inputHistory->bButtonUp.pressed || inputHistory->bHatUp.pressed || inputHistory->bKeyUp.pressed || inputHistory->bStickUp.pressed))
	{
		menuManager->CursorPrev();
	}

	if(inputHistory->frame == frame && (inputHistory->buttonMenuConfirm.pressed || inputHistory->keyMenuConfirm.pressed))
	{
		switch(mainMenuState)
		{
		case TOP:
			switch(menuManager->GetCursorIndex())
			{
			case 0:
				menuManager->ToChild();
				ChangeMainMenuState(VERSUS);
				break;
			case 1:
				menuManager->ToChild();
				ChangeMainMenuState(OPTIONS);
				break;
			case 2:
				Exit();
				break;
			}
			break;
		case VERSUS:
			switch(menuManager->GetCursorIndex())
			{
			case 0:
				gameType = FREE_FOR_ALL;
				if(int i = ChangeGameState(CHARACTER_SELECT) != 0) { return i; }
				break;
			case 1:
				menuManager->ToParent();
				ChangeMainMenuState(TOP);
				break;
			}
			break;
		case OPTIONS:
			switch(menuManager->GetCursorIndex())
			{
			case 0:
				menuManager->ToChild();
				ChangeMainMenuState(VIDEO);
				break;
			case 1:
				menuManager->ToChild();
				ChangeMainMenuState(KEY_CONFIG);
				break;
			case 2:
				menuManager->ToParent();
				ChangeMainMenuState(TOP);
				break;
			}
			break;
		case VIDEO:
			switch(menuManager->GetCursorIndex())
			{
			case 0:
				if(fullScreen)
				{
					SetFullScreen(false);
				}
				else
				{
					SetFullScreen(true);
				}
				SaveConfig();
				break;
			case 1:
				menuManager->ToParent();
				ChangeMainMenuState(OPTIONS);
				break;
			}
			break;
		case KEY_CONFIG:
			switch(menuManager->GetCursorIndex())
			{
			case 0:
			case 1:
			case 2:
			case 3:
				playerToSetUp = menuManager->GetCursorIndex();
				menuManager->ToChild();
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
				break;
			case 4:
				menuManager->ToParent();
				ChangeMainMenuState(OPTIONS);
				break;
			}
			break;
		case PLAYER_KEY_CONFIG:
			switch(menuManager->GetCursorIndex())
			{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
				keyToSetUp = menuManager->GetCursorIndex();
				ChangeMainMenuState(INPUT_KEY);
				break;
			case 11:
				playerToSetUp = -1;
				menuManager->ToParent();
				ChangeMainMenuState(KEY_CONFIG);
				break;
			}
			break;
		}
	}
	else if(inputHistory->frame == frame && (inputHistory->buttonMenuBack.pressed || inputHistory->keyMenuBack.pressed))
	{
		switch(mainMenuState)
		{
		case TOP:
			Exit();
			break;
		case VERSUS:
			menuManager->ToParent();
			ChangeMainMenuState(TOP);
			break;
		case OPTIONS:
			menuManager->ToParent();
			ChangeMainMenuState(TOP);
			break;
		case VIDEO:
			menuManager->ToParent();
			ChangeMainMenuState(OPTIONS);
			break;
		case KEY_CONFIG:
			menuManager->ToParent();
			ChangeMainMenuState(OPTIONS);
			break;
		case PLAYER_KEY_CONFIG:
			playerToSetUp = -1;
			menuManager->ToParent();
			ChangeMainMenuState(KEY_CONFIG);
			break;
		}
	}

	return 0;
}

int Main::UpdateMainMenu()
{
	return 0;
}

int Main::InitializeCharacterSelect()
{
	//demo characters
	HSObject * newObject;
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\characters\\john\\johnEgbertDemo.xml"), &gameObjects, &newObject) != 0) { return error; }
	newObject->pos.x = CHAR_SELECT_PLAYER_ONE_POS_X;
	newObject->pos.y = CHAR_SELECT_PLAYER_POS_Y;
	newObject->curHold = newObject->firstHold;
	focusObjectOne = newObject;
	players[0] = newObject;
	selectedCharacters[0] = "data\\characters\\john\\John Egbert.xml";
	selectedPalettes[0] = players[0]->GetPalette();

	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\characters\\john\\johnEgbertDemo.xml"), &gameObjects, &newObject) != 0) { return error; }
	newObject->pos.x = CHAR_SELECT_PLAYER_TWO_POS_X;
	newObject->pos.y = CHAR_SELECT_PLAYER_POS_Y;
	newObject->curHold = newObject->firstHold;
	newObject->hFlip = true;
	focusObjectTwo = newObject;
	players[1] = newObject;
	selectedCharacters[1] = "data\\characters\\john\\John Egbert.xml";
	selectedPalettes[1] = players[1]->GetPalette();

	//hud
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\characterSelect\\characterSelect.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->pos.x = CHARACTER_SELECT_POS_X;
	newObject->pos.y = CHARACTER_SELECT_POS_Y;
	newObject->curHold = newObject->firstHold;
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\player\\player1.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->pos.x = PLAYER_ONE_POS_X;
	newObject->pos.y = PLAYER_POS_Y;
	newObject->curHold = newObject->firstHold;
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\player\\player2.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->pos.x = PLAYER_TWO_POS_X;
	newObject->pos.y = PLAYER_POS_Y;
	newObject->curHold = newObject->firstHold;
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\selectPalette\\selectPalette.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->pos.x = SELECT_PALETTE_ONE_POS_X;
	newObject->pos.y = SELECT_PALETTE_POS_Y;
	newObject->curHold = newObject->firstHold;
	selectPaletteOne = newObject;
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\leftArrow\\leftArrow.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->pos.x = SELECT_PALETTE_ONE_POS_X - 55;
	newObject->pos.y = SELECT_PALETTE_POS_Y;
	newObject->curHold = newObject->firstHold;
	selectPaletteLeftOne = newObject;
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\rightArrow\\rightArrow.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->pos.x = SELECT_PALETTE_ONE_POS_X + 305;
	newObject->pos.y = SELECT_PALETTE_POS_Y;
	newObject->curHold = newObject->firstHold;
	selectPaletteRightOne = newObject;
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\ready\\ready.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->pos.x = READY_ONE_POS_X;
	newObject->pos.y = SELECT_PALETTE_POS_Y;
	newObject->curHold = newObject->firstHold;
	readyOne = newObject;
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\selectPalette\\selectPalette.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->pos.x = SELECT_PALETTE_TWO_POS_X;
	newObject->pos.y = SELECT_PALETTE_POS_Y;
	newObject->curHold = newObject->firstHold;
	selectPaletteTwo = newObject;
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\leftArrow\\leftArrow.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->pos.x = SELECT_PALETTE_TWO_POS_X - 55;
	newObject->pos.y = SELECT_PALETTE_POS_Y;
	newObject->curHold = newObject->firstHold;
	selectPaletteLeftTwo = newObject;
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\rightArrow\\rightArrow.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->pos.x = SELECT_PALETTE_TWO_POS_X + 305;
	newObject->pos.y = SELECT_PALETTE_POS_Y;
	newObject->curHold = newObject->firstHold;
	selectPaletteRightTwo = newObject;
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\ready\\ready.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->pos.x = READY_TWO_POS_X;
	newObject->pos.y = SELECT_PALETTE_POS_Y;
	newObject->curHold = newObject->firstHold;
	readyTwo = newObject;

	ChangeCharacterSelectState(PLAYERS_SELECTING);

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		ChangeCharacterSelectPlayerState(SELECTING_PALETTE, i);
	}

	return 0;
}

int Main::ChangeCharacterSelectState(CharacterSelectState newState)
{
	characterSelectState = newState;

	return 0;
}

int Main::ChangeCharacterSelectPlayerState(CharacterSelectPlayerState newState, int player)
{
	characterSelectPlayerState[player] = newState;

	switch(characterSelectPlayerState[player])
	{
	case SELECTING_PALETTE:
		switch(player)
		{
		case 0:
			readyOne->visible = false;
			selectPaletteLeftOne->visible = true;
			selectPaletteRightOne->visible = true;
			selectPaletteOne->visible = true;
			break;
		case 1:
			readyTwo->visible = false;
			selectPaletteLeftTwo->visible = true;
			selectPaletteRightTwo->visible = true;
			selectPaletteTwo->visible = true;
			break;
		}
		break;
	case READY:
		switch(player)
		{
		case 0:
			readyOne->visible = true;
			selectPaletteLeftOne->visible = false;
			selectPaletteRightOne->visible = false;
			selectPaletteOne->visible = false;
			break;
		case 1:
			readyTwo->visible = true;
			selectPaletteLeftTwo->visible = false;
			selectPaletteRightTwo->visible = false;
			selectPaletteTwo->visible = false;
			break;
		}
		break;
	}

	return 0;
}

int Main::EventCharacterSelect(InputStates * inputHistory, int frame, int player)
{
	if(inputHistory->frame == frame && (inputHistory->bButtonLeft.pressed || inputHistory->bHatLeft.pressed || inputHistory->bKeyLeft.pressed || inputHistory->bStickLeft.pressed))
	{
		switch(characterSelectState)
		{
		case PLAYERS_SELECTING:
			switch(characterSelectPlayerState[player])
			{
			case SELECTING_PALETTE:
				if(players[player] != NULL)
				{
					players[player]->PrevPalette();
					selectedPalettes[player] = players[player]->GetPalette();
				}
				break;
			case READY:
				break;
			}
			break;
		}
	}
	else if(inputHistory->frame == frame && (inputHistory->bButtonRight.pressed || inputHistory->bHatRight.pressed || inputHistory->bKeyRight.pressed || inputHistory->bStickRight.pressed))
	{
		switch(characterSelectState)
		{
		case PLAYERS_SELECTING:
			switch(characterSelectPlayerState[player])
			{
			case SELECTING_PALETTE:
				if(players[player] != NULL)
				{
					players[player]->NextPalette();
					selectedPalettes[player] = players[player]->GetPalette();
				}
				break;
			case READY:
				break;
			}
			break;
		}
	}

	if(inputHistory->frame == frame && (inputHistory->buttonMenuConfirm.pressed || inputHistory->keyMenuConfirm.pressed))
	{
		switch(characterSelectState)
		{
		case PLAYERS_SELECTING:
			switch(characterSelectPlayerState[player])
			{
			case SELECTING_PALETTE:
				ChangeCharacterSelectPlayerState(READY, player);
				break;
			case READY:
				break;
			}
			break;
		}
	}
	else if(inputHistory->frame == frame && (inputHistory->buttonMenuBack.pressed || inputHistory->keyMenuBack.pressed))
	{
		switch(characterSelectState)
		{
		case PLAYERS_SELECTING:
			switch(characterSelectPlayerState[player])
			{
			case SELECTING_PALETTE:
				if(int i = ChangeGameState(MAIN_MENU) != 0) { return i; }
				break;
			case READY:
				ChangeCharacterSelectPlayerState(SELECTING_PALETTE, player);
				break;
			}
			break;
		}
	}

	return 0;
}

int Main::UpdateCharacterSelect()
{
	if(characterSelectPlayerState[0] == READY && characterSelectPlayerState[1] == READY)
	{
		if(int i = ChangeGameState(MATCH) != 0) { return i; }
	}

	return 0;
}

int Main::InitializeMatch()
{
	//load stage/background
	HSObject * newObject;
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\stages\\lowas\\LOWAS.xml"), &gameObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	newObject->pos.x = 3250;
	newObject->pos.y = 2700;

	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\stages\\lowas\\LeftPlatform.xml"), &gameObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	newObject->pos.x = 1785;
	newObject->pos.y = 3365;

	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\stages\\lowas\\RightPlatform.xml"), &gameObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	newObject->pos.x = 3838;
	newObject->pos.y = 3036;

	spawnPoints[0].x = 2315;
	spawnPoints[0].y = 2502;
	spawnPoints[1].x = 4432;
	spawnPoints[1].y = 2100;

	//load characters
	HSObject * fighterOne;
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, selectedCharacters[0]), &gameObjects, &fighterOne) != 0) { return error; }
	fighterOne->pos.x = spawnPoints[0].x;
	fighterOne->pos.y = spawnPoints[0].y;
	players[0] = fighterOne;
	fighterOne->SetPalette(selectedPalettes[0]);
	((Fighter*)fighterOne)->state = STANDING;
	((Fighter*)fighterOne)->curHealth = ((Fighter*)fighterOne)->health;
	fighterOne->ChangeHold(((Fighter*)fighterOne)->fighterEventHolds.standing);
	focusObjectOne = fighterOne;
	
	HSObject * fighterTwo;
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, selectedCharacters[1]), &gameObjects, &fighterTwo) != 0) { return error; }
	fighterTwo->pos.x = spawnPoints[1].x;
	fighterTwo->pos.y = spawnPoints[1].y;
	players[1] = fighterTwo;
	fighterTwo->SetPalette(selectedPalettes[1]);
	if(selectedCharacters[1] == selectedCharacters[0] && selectedPalettes[1] == selectedPalettes[0]) { fighterTwo->NextPalette(); }
	((Fighter*)fighterTwo)->state = STANDING;
	((Fighter*)fighterTwo)->facing = LEFT;
	((Fighter*)fighterTwo)->curHealth = ((Fighter*)fighterTwo)->health;
	fighterTwo->ChangeHold(((Fighter*)fighterTwo)->fighterEventHolds.standing);
	focusObjectTwo = fighterTwo;

	//load HUD
	HSObject * newHUD;
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\TestHUD\\johnHud.xml"), &HUDObjects, &newHUD) != 0) { return error; }
	playerHUDs[0] = (HUD*)newHUD;
	newHUD->curHold = newHUD->firstHold;
	((HUD*)newHUD)->pos.x = (MAX_GAME_RESOLUTION_X / -2) + 20;
	((HUD*)newHUD)->pos.y = (MAX_GAME_RESOLUTION_Y / -2) + 20;

	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\TestHUD\\johnHud.xml"), &HUDObjects, &newHUD) != 0) { return error; }
	playerHUDs[1] = (HUD*)newHUD;
	newHUD->curHold = newHUD->firstHold;
	((HUD*)newHUD)->pos.x = (MAX_GAME_RESOLUTION_X / 2) - 560;
	((HUD*)newHUD)->pos.y = (MAX_GAME_RESOLUTION_Y / -2) + 20;

	//load pause menu
	Menu * playerKeyConfigMenu = new Menu(MAIN_MENU_HEADER_HEIGHT, MAIN_MENU_CURSOR_WIDTH, MAIN_MENU_ITEM_HEIGHT, MAIN_MENU_ITEM_SPACING);
	playerKeyConfigMenu->pos.x = -382;
	playerKeyConfigMenu->pos.y = -352;

	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\cursor\\cursor.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerKeyConfigMenu->SetCursor(newObject);
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\player\\player1.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerKeyConfigMenu->SetHeader(newObject);
	playerOne = newObject;
	newObject->visible = false;
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\player\\player2.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerTwo = newObject;
	newObject->visible = false;
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\player\\player3.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerThree = newObject;
	newObject->visible = false;
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\player\\player4.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerFour = newObject;
	newObject->visible = false;
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\up\\up.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerKeyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\down\\down.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerKeyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\left\\left.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerKeyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\right\\right.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerKeyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\lightAttack\\lightAttack.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerKeyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\heavyAttack\\heavyAttack.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerKeyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\jump\\jump.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerKeyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\block\\block.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerKeyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\pause\\pause.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerKeyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\menuConfirm\\menuConfirm.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerKeyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\menuBack\\menuBack.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerKeyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\back\\back.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	playerKeyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject));
	
	Menu * keyConfigMenu = new Menu(MAIN_MENU_HEADER_HEIGHT, MAIN_MENU_CURSOR_WIDTH, MAIN_MENU_ITEM_HEIGHT, MAIN_MENU_ITEM_SPACING);
	keyConfigMenu->pos.x = -382;
	keyConfigMenu->pos.y = -352;

	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\cursor\\cursor.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	keyConfigMenu->SetCursor(newObject);
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\keyConfig\\keyConfig.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	keyConfigMenu->SetHeader(newObject);
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\player\\player1.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	keyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject, playerKeyConfigMenu));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\player\\player2.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	keyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject, playerKeyConfigMenu));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\player\\player3.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	keyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject, playerKeyConfigMenu));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\player\\player4.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	keyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject, playerKeyConfigMenu));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\back\\back.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	keyConfigMenu->AddItem(menuManager->MakeMenuItem(newObject));
	
	Menu * pauseMenu = new Menu(MAIN_MENU_HEADER_HEIGHT, MAIN_MENU_CURSOR_WIDTH, MAIN_MENU_ITEM_HEIGHT, MAIN_MENU_ITEM_SPACING);
	pauseMenu->pos.x = -382;
	pauseMenu->pos.y = -352;

	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\cursor\\cursor.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	pauseMenu->SetCursor(newObject);
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\pause\\pause.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	pauseMenu->SetHeader(newObject);
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\resume\\resumeMatch.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	pauseMenu->AddItem(menuManager->MakeMenuItem(newObject));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\keyConfig\\keyConfig.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	pauseMenu->AddItem(menuManager->MakeMenuItem(newObject, keyConfigMenu));
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\quit\\quitMatch.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	pauseMenu->AddItem(menuManager->MakeMenuItem(newObject));

	menuManager = new MenuManager(pauseMenu);
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\pressDesiredButton\\pressDesiredButton.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	pressDesiredButton = newObject;
	newObject->visible = false;

	//load win text
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\player\\player1.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	newObject->visible = false;
	newObject->pos.x = -82;
	newObject->pos.y = -45;
	playerOne = newObject;
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\player\\player2.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	newObject->visible = false;
	newObject->pos.x = -82;
	newObject->pos.y = -45;
	playerTwo = newObject;
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\player\\player3.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	newObject->visible = false;
	newObject->pos.x = -82;
	newObject->pos.y = -45;
	playerThree = newObject;
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\player\\player4.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	newObject->visible = false;
	newObject->pos.x = -82;
	newObject->pos.y = -45;
	playerFour = newObject;
	
	if(int error = LoadDefinition(CreateAbsolutePath(currentWorkingDirectory, "data\\hud\\MainMenuGUI\\wins\\wins.xml"), &HUDObjects, &newObject) != 0) { return error; }
	newObject->curHold = newObject->firstHold;
	newObject->visible = false;
	newObject->pos.x = -53;
	newObject->pos.y = 10;
	wins = newObject;

	ChangeMatchState(IN_PROGRESS);

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		playerLives[i] = 3;
		ChangeMatchPlayerState(ACTIVE, i);
	}

	ChangePauseMenuState(PAUSE_TOP);

	return 0;
}

int Main::ChangeMatchState(MatchState newState)
{
	matchState = newState;

	switch(newState)
	{
		case IN_PROGRESS:
			menuManager->Hide(true);
			break;
		case PAUSED:
			menuManager->Hide(false);
			break;
		case RESULTS:
			if(matchPlayerState[0] == ACTIVE)
			{
				playerOne->visible = true;
			}
			else if(matchPlayerState[1] == ACTIVE)
			{
				playerTwo->visible = true;
			}
			wins->visible = true;
			break;
	}

	return 0;
}

int Main::ChangeMatchPlayerState(MatchPlayerState newState, int player)
{
	matchPlayerState[player] = newState;

	switch(newState)
	{
		case ACTIVE:
			break;
		case LOST:
			playerLives[player] = 0;
			if(playerHUDs[player] != NULL)
			{
				playerHUDs[player]->SetHealthMeterValue(0);
				playerHUDs[player]->SetCounterValue(0);
			}
			if(players[player] == NULL) { break; }
			players[player]->visible = false;
			if(players[player]->IsTerrainObject())
			{
				((TerrainObject*)players[player])->curHealth = 0;
			}
			break;
	}

	return 0;
}

int Main::ChangePauseMenuState(PauseMenuState newState)
{
	pauseMenuState = newState;

	switch(newState)
	{
	case PAUSE_TOP:
		break;
	case PAUSE_KEY_CONFIG:
		break;
	case PAUSE_PLAYER_KEY_CONFIG:
		pressDesiredButton->visible = false;
		switch(playerToSetUp)
		{
		case 0:
			menuManager->SetHeader(playerOne);
			break;
		case 1:
			menuManager->SetHeader(playerTwo);
			break;
		case 2:
			menuManager->SetHeader(playerThree);
			break;
		case 3:
			menuManager->SetHeader(playerFour);
			break;
		}
		break;
	case PAUSE_INPUT_KEY:
		pressDesiredButton->pos.x = menuManager->GetCursorPos().x + PRESS_DESIRED_BUTTON_OFFSET_X;
		pressDesiredButton->pos.y = menuManager->GetCursorPos().y;
		pressDesiredButton->visible = true;
		break;
	}

	return 0;
}

int Main::EventMatch(InputStates * inputHistory, int frame, int player)
{
	switch(matchState)
	{
	case IN_PROGRESS:
		if(inputHistory->frame == frame && (inputHistory->bButtonStart.pressed || inputHistory->bKeyStart.pressed))
		{
			ChangeMatchState(PAUSED);
		}
		else if(players[player] != 0)
		{
			players[player]->Event(inputHistory, frame);
		}
		break;
	case PAUSED:
		if(inputHistory->frame == frame && (inputHistory->bButtonDown.pressed || inputHistory->bHatDown.pressed || inputHistory->bKeyDown.pressed || inputHistory->bStickDown.pressed))
		{
			menuManager->CursorNext();
		}
		else if(inputHistory->frame == frame && (inputHistory->bButtonUp.pressed || inputHistory->bHatUp.pressed || inputHistory->bKeyUp.pressed || inputHistory->bStickUp.pressed))
		{
			menuManager->CursorPrev();
		}

		if(inputHistory->frame == frame && (inputHistory->bButtonStart.pressed || inputHistory->bKeyStart.pressed))
		{
			ChangeMatchState(IN_PROGRESS);
		}
		else if(inputHistory->frame == frame && (inputHistory->buttonMenuConfirm.pressed || inputHistory->keyMenuConfirm.pressed))
		{
			switch(pauseMenuState)
			{
			case PAUSE_TOP:
				switch(menuManager->GetCursorIndex())
				{
				case 0:
					ChangeMatchState(IN_PROGRESS);
					break;
				case 1:
					menuManager->ToChild();
					ChangePauseMenuState(PAUSE_KEY_CONFIG);
					break;
				case 2:
					ChangeGameState(MAIN_MENU);
					break;
				}
				break;
			case PAUSE_KEY_CONFIG:
				switch(menuManager->GetCursorIndex())
				{
				case 0:
				case 1:
				case 2:
				case 3:
					playerToSetUp = menuManager->GetCursorIndex();
					menuManager->ToChild();
					ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
					break;
				case 4:
					menuManager->ToParent();
					ChangePauseMenuState(PAUSE_TOP);
					break;
				}
				break;
			case PAUSE_PLAYER_KEY_CONFIG:
				switch(menuManager->GetCursorIndex())
				{
				case 0:
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10:
					keyToSetUp = menuManager->GetCursorIndex();
					ChangePauseMenuState(PAUSE_INPUT_KEY);
					break;
				case 11:
					playerToSetUp = -1;
					menuManager->ToParent();
					ChangePauseMenuState(PAUSE_KEY_CONFIG);
					break;
				}
				break;
			}
		}
		else if(inputHistory->frame == frame && (inputHistory->buttonMenuBack.pressed || inputHistory->keyMenuBack.pressed))
		{
			switch(pauseMenuState)
			{
			case PAUSE_TOP:
				ChangeMatchState(IN_PROGRESS);
				break;
			case PAUSE_KEY_CONFIG:
				menuManager->ToParent();
				ChangePauseMenuState(PAUSE_TOP);
				break;
			case PAUSE_PLAYER_KEY_CONFIG:
				playerToSetUp = -1;
				menuManager->ToParent();
				ChangePauseMenuState(PAUSE_KEY_CONFIG);
				break;
			}
		}
		break;
	case RESULTS:
		if(inputHistory->frame == frame && (inputHistory->bButtonStart.pressed || inputHistory->bKeyStart.pressed
			|| inputHistory->buttonMenuConfirm.pressed || inputHistory->keyMenuConfirm.pressed || inputHistory->buttonMenuBack.pressed || inputHistory->keyMenuBack.pressed))
		{
			if(int error = ChangeGameState(CHARACTER_SELECT) != 0) { return error; }
		}
		break;
	}

	return 0;
}

int Main::UpdateMatch()
{
	return 0;
}

int Main::CollideMatch()
{
	int survivingPlayers = 0;

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(players[i] == NULL) { ChangeMatchPlayerState(LOST, i); continue; }

		if(matchPlayerState[i] == LOST) { continue; }

		if((!players[i]->IsTerrain() && players[i]->pos.y > 6000) || (players[i]->IsTerrainObject() && ((TerrainObject*)players[i])->curHealth <= 0 &&
			(!players[i]->IsFighter() || ((Fighter*)players[i])->state != KNOCKOUT && ((Fighter*)players[i])->state != KNOCKOUT_AIR)))
		{
			playerLives[i]--;

			if(playerLives[i] <= 0)
			{
				if(int error = ChangeMatchPlayerState(LOST, i) != 0) { return error; }
				players[i]->toDelete = true;
				continue;
			}

			players[i]->pos.x = spawnPoints[i].x;
			players[i]->pos.y = spawnPoints[i].y;
			players[i]->vel.x = 0;
			players[i]->vel.y = 0;

			if(players[i]->IsTerrainObject())
			{
				((TerrainObject*)players[i])->curHealth = ((TerrainObject*)players[i])->health;
			}

			if(players[i]->IsFighter())
			{
				((Fighter*)players[i])->ChangeHold(((Fighter*)players[i])->fighterEventHolds.jumpNeutralFall);
				((Fighter*)players[i])->state = JUMPING;
				((Fighter*)players[i])->falls = true;
			}
		}

		if(playerHUDs[i] != NULL)
		{
			float curHealth = ((TerrainObject*)players[i])->curHealth;
			float health = ((TerrainObject*)players[i])->health;
			playerHUDs[i]->SetHealthMeterValue(curHealth / health);
			playerHUDs[i]->SetCounterValue(playerLives[i]);
		}
		
		survivingPlayers++;
	}

	if(survivingPlayers <= 1)
	{
		if(int i = ChangeMatchState(RESULTS) != 0) { return i; }
	}

	return 0;
}

void Main::DefaultConfig()
{
	fullScreen = true;

	mappings[0].keyUp = SDLK_w;
	mappings[0].keyDown = SDLK_s;
	mappings[0].keyLeft = SDLK_a;
	mappings[0].keyRight = SDLK_d;
	mappings[0].keyJump = SDLK_j;
	mappings[0].keyLight = SDLK_h;
	mappings[0].keyHeavy = SDLK_n;
	mappings[0].keyBlock = SDLK_k;
	mappings[0].keyStart = SDLK_RSHIFT;
	mappings[0].keyMenuConfirm = mappings[0].keyLight;
	mappings[0].keyMenuBack = mappings[0].keyJump;

	mappings[1].keyUp = SDLK_UP;
	mappings[1].keyDown = SDLK_DOWN;
	mappings[1].keyLeft = SDLK_LEFT;
	mappings[1].keyRight = SDLK_RIGHT;
	mappings[1].keyJump = SDLK_KP5;
	mappings[1].keyLight = SDLK_KP4;
	mappings[1].keyHeavy = SDLK_KP1;
	mappings[1].keyBlock = SDLK_KP6;
	mappings[1].keyStart = SDLK_KP_PLUS;
	mappings[1].keyMenuConfirm = mappings[1].keyLight;
	mappings[1].keyMenuBack = mappings[1].keyJump;
}

int Main::LoadConfig()
{
	//get the XML structure from the file
	XMLDocument * file = new XMLDocument();
	if(int error = file->LoadFile("config.xml") != 0)
	{
		//file doesn't exist, so just set some defaults and then save the configuration
		DefaultConfig();
		return SaveConfig();
	}

	if(strcmp(file->RootElement()->Value(), "Configuration") != 0)
	{
		//file isn't formatted correctly, so just set some defaults and then save the configuration
		DefaultConfig();
		return SaveConfig();
	}

	//loop through all the parts of the configuration
	for(XMLElement * i = file->RootElement()->FirstChildElement(); i != NULL; i = i->NextSiblingElement())
	{
		//get the config section
		const char * configSec = i->Value();

		//create a new object of the appropriate type
		if(strcmp(configSec, "General") == 0)
		{
			//get fullscreen
			const char * fs = i->Attribute("fullscreen");
			string b;
			if(fs != NULL)
			{
				b.assign(fs);
				if(b.compare("true") == 0) { fullScreen = true; }
				else if(b.compare("false") == 0) { fullScreen = false; }
			}
		}
		else if(strcmp(configSec, "PlayerOne") == 0)
		{
			LoadPlayerConfig(i, 0);
		}
		else if(strcmp(configSec, "PlayerTwo") == 0)
		{
			LoadPlayerConfig(i, 1);
		}
		else if(strcmp(configSec, "PlayerThree") == 0)
		{
			LoadPlayerConfig(i, 2);
		}
		else if(strcmp(configSec, "PlayerFour") == 0)
		{
			LoadPlayerConfig(i, 3);
		}
	}

	return 0;
}

void Main::LoadPlayerConfig(XMLElement * config, int player)
{
	if(player < 0) { player = 0; }
	if(player > 3) { player = 3; }

	bool hatOrStick = false;
	if(config->Attribute("up") != NULL)
	{
		string buttonConfig = config->Attribute("up");
		if(!LoadToKeyConfig(buttonConfig, &mappings[player].keyUp))
		{
			if(!LoadToJoyButtonConfig(buttonConfig, &mappings[player].buttonUp))
			{
				if((hatOrStick = LoadToHatConfig(buttonConfig, &mappings[player].hat)) == false)
				{
					hatOrStick = LoadToStickConfig(buttonConfig, &mappings[player].stick);
				}
			}
		}
	}
	
	if(!hatOrStick && config->Attribute("down") != NULL)
	{
		string buttonConfig = config->Attribute("down");
		if(!LoadToKeyConfig(buttonConfig, &mappings[player].keyDown))
		{
			if(!LoadToJoyButtonConfig(buttonConfig, &mappings[player].buttonDown))
			{
				if((hatOrStick = LoadToHatConfig(buttonConfig, &mappings[player].hat)) == false)
				{
					hatOrStick = LoadToStickConfig(buttonConfig, &mappings[player].stick);
				}
			}
		}
	}
	
	if(!hatOrStick && config->Attribute("left") != NULL)
	{
		string buttonConfig = config->Attribute("left");
		if(!LoadToKeyConfig(buttonConfig, &mappings[player].keyLeft))
		{
			if(!LoadToJoyButtonConfig(buttonConfig, &mappings[player].buttonLeft))
			{
				if((hatOrStick = LoadToHatConfig(buttonConfig, &mappings[player].hat)) == false)
				{
					hatOrStick = LoadToStickConfig(buttonConfig, &mappings[player].stick);
				}
			}
		}
	}
	
	if(!hatOrStick && config->Attribute("right") != NULL)
	{
		string buttonConfig = config->Attribute("right");
		if(!LoadToKeyConfig(buttonConfig, &mappings[player].keyRight))
		{
			if(!LoadToJoyButtonConfig(buttonConfig, &mappings[player].buttonRight))
			{
				if((hatOrStick = LoadToHatConfig(buttonConfig, &mappings[player].hat)) == false)
				{
					hatOrStick = LoadToStickConfig(buttonConfig, &mappings[player].stick);
				}
			}
		}
	}
	
	if(config->Attribute("light") != NULL)
	{
		string buttonConfig = config->Attribute("light");
		if(!LoadToKeyConfig(buttonConfig, &mappings[player].keyLight))
		{
			LoadToJoyButtonConfig(buttonConfig, &mappings[player].buttonLight);
		}
	}
	
	if(config->Attribute("heavy") != NULL)
	{
		string buttonConfig = config->Attribute("heavy");
		if(!LoadToKeyConfig(buttonConfig, &mappings[player].keyHeavy))
		{
			LoadToJoyButtonConfig(buttonConfig, &mappings[player].buttonHeavy);
		}
	}
	
	if(config->Attribute("jump") != NULL)
	{
		string buttonConfig = config->Attribute("jump");
		if(!LoadToKeyConfig(buttonConfig, &mappings[player].keyJump))
		{
			LoadToJoyButtonConfig(buttonConfig, &mappings[player].buttonJump);
		}
	}
	
	if(config->Attribute("block") != NULL)
	{
		string buttonConfig = config->Attribute("block");
		if(!LoadToKeyConfig(buttonConfig, &mappings[player].keyBlock))
		{
			LoadToJoyButtonConfig(buttonConfig, &mappings[player].buttonBlock);
		}
	}
	
	if(config->Attribute("menuConfirm") != NULL)
	{
		string buttonConfig = config->Attribute("menuConfirm");
		if(!LoadToKeyConfig(buttonConfig, &mappings[player].keyMenuConfirm))
		{
			LoadToJoyButtonConfig(buttonConfig, &mappings[player].buttonMenuConfirm);
		}
	}
	
	if(config->Attribute("menuBack") != NULL)
	{
		string buttonConfig = config->Attribute("menuBack");
		if(!LoadToKeyConfig(buttonConfig, &mappings[player].keyMenuBack))
		{
			LoadToJoyButtonConfig(buttonConfig, &mappings[player].buttonMenuBack);
		}
	}
	
	if(config->Attribute("pause") != NULL)
	{
		string buttonConfig = config->Attribute("pause");
		if(!LoadToKeyConfig(buttonConfig, &mappings[player].keyStart))
		{
			LoadToJoyButtonConfig(buttonConfig, &mappings[player].buttonStart);
		}
	}
}

bool Main::LoadToKeyConfig(string config, SDLKey * key)
{
	if(config == "a") { *key = SDLK_a; return true; }
	else if(config == "b") { *key = SDLK_b; return true; }
	else if(config == "c") { *key = SDLK_c; return true; }
	else if(config == "d") { *key = SDLK_d; return true; }
	else if(config == "e") { *key = SDLK_e; return true; }
	else if(config == "f") { *key = SDLK_f; return true; }
	else if(config == "g") { *key = SDLK_g; return true; }
	else if(config == "h") { *key = SDLK_h; return true; }
	else if(config == "i") { *key = SDLK_i; return true; }
	else if(config == "j") { *key = SDLK_j; return true; }
	else if(config == "k") { *key = SDLK_k; return true; }
	else if(config == "l") { *key = SDLK_l; return true; }
	else if(config == "m") { *key = SDLK_m; return true; }
	else if(config == "n") { *key = SDLK_n; return true; }
	else if(config == "o") { *key = SDLK_o; return true; }
	else if(config == "p") { *key = SDLK_p; return true; }
	else if(config == "q") { *key = SDLK_q; return true; }
	else if(config == "r") { *key = SDLK_r; return true; }
	else if(config == "s") { *key = SDLK_s; return true; }
	else if(config == "t") { *key = SDLK_t; return true; }
	else if(config == "u") { *key = SDLK_u; return true; }
	else if(config == "v") { *key = SDLK_v; return true; }
	else if(config == "w") { *key = SDLK_w; return true; }
	else if(config == "x") { *key = SDLK_x; return true; }
	else if(config == "y") { *key = SDLK_y; return true; }
	else if(config == "z") { *key = SDLK_z; return true; }
	else if(config == "0") { *key = SDLK_0; return true; }
	else if(config == "1") { *key = SDLK_1; return true; }
	else if(config == "2") { *key = SDLK_2; return true; }
	else if(config == "3") { *key = SDLK_3; return true; }
	else if(config == "4") { *key = SDLK_4; return true; }
	else if(config == "5") { *key = SDLK_5; return true; }
	else if(config == "6") { *key = SDLK_6; return true; }
	else if(config == "7") { *key = SDLK_7; return true; }
	else if(config == "8") { *key = SDLK_8; return true; }
	else if(config == "9") { *key = SDLK_9; return true; }
	else if(config == "backspace") { *key = SDLK_BACKSPACE; return true; }
	else if(config == "tab") { *key = SDLK_TAB; return true; }
	else if(config == "clear") { *key = SDLK_CLEAR; return true; }
	//else if(config == "return") { *key = SDLK_RETURN; return true; }
	else if(config == "pause") { *key = SDLK_PAUSE; return true; }
	//else if(config == "escape") { *key = SDLK_ESCAPE; return true; }
	else if(config == "space") { *key = SDLK_SPACE; return true; }
	else if(config == "exclaim") { *key = SDLK_EXCLAIM; return true; }
	else if(config == "quotedbl") { *key = SDLK_QUOTEDBL; return true; }
	else if(config == "hash") { *key = SDLK_HASH; return true; }
	else if(config == "dollar") { *key = SDLK_DOLLAR; return true; }
	else if(config == "ampersand") { *key = SDLK_AMPERSAND; return true; }
	else if(config == "quote") { *key = SDLK_QUOTE; return true; }
	else if(config == "leftparen") { *key = SDLK_LEFTPAREN; return true; }
	else if(config == "rightparen") { *key = SDLK_RIGHTPAREN; return true; }
	else if(config == "asterisk") { *key = SDLK_ASTERISK; }
	else if(config == "plus") { *key = SDLK_PLUS; return true; }
	else if(config == "comma") { *key = SDLK_COMMA; return true; }
	else if(config == "minus") { *key = SDLK_MINUS; return true; }
	else if(config == "period") { *key = SDLK_PERIOD; return true; }
	else if(config == "slash") { *key = SDLK_SLASH; return true; }
	else if(config == "colon") { *key = SDLK_COLON; return true; }
	else if(config == "semicolon") { *key = SDLK_SEMICOLON; return true; }
	else if(config == "less") { *key = SDLK_LESS; return true; }
	else if(config == "equals") { *key = SDLK_EQUALS; return true; }
	else if(config == "greater") { *key = SDLK_GREATER; return true; }
	else if(config == "question") { *key = SDLK_QUESTION; return true; }
	else if(config == "at") { *key = SDLK_AT; return true; }
	else if(config == "leftbracket") { *key = SDLK_LEFTBRACKET; return true; }
	else if(config == "backslash") { *key = SDLK_BACKSLASH; return true; }
	else if(config == "rightbracket") { *key = SDLK_RIGHTBRACKET; return true; }
	else if(config == "caret") { *key = SDLK_CARET; return true; }
	else if(config == "underscore") { *key = SDLK_UNDERSCORE; return true; }
	else if(config == "backquote") { *key = SDLK_BACKQUOTE; return true; }
	else if(config == "delete") { *key = SDLK_DELETE; return true; }
	else if(config == "numpad_0") { *key = SDLK_KP0; return true; }
	else if(config == "numpad_1") { *key = SDLK_KP1; return true; }
	else if(config == "numpad_2") { *key = SDLK_KP2; return true; }
	else if(config == "numpad_3") { *key = SDLK_KP3; return true; }
	else if(config == "numpad_4") { *key = SDLK_KP4; return true; }
	else if(config == "numpad_5") { *key = SDLK_KP5; return true; }
	else if(config == "numpad_6") { *key = SDLK_KP6; return true; }
	else if(config == "numpad_7") { *key = SDLK_KP7; return true; }
	else if(config == "numpad_8") { *key = SDLK_KP8; return true; }
	else if(config == "numpad_9") { *key = SDLK_KP9; return true; }
	else if(config == "numpad_period") { *key = SDLK_KP_PERIOD; return true; }
	else if(config == "numpad_divide") { *key = SDLK_KP_DIVIDE; return true; }
	else if(config == "numpad_multiply") { *key = SDLK_KP_MULTIPLY; return true; }
	else if(config == "numpad_minus") { *key = SDLK_KP_MINUS; return true; }
	else if(config == "numpad_plus") { *key = SDLK_KP_PLUS; return true; }
	//else if(config == "numpad_enter") { *key = SDLK_KP_ENTER; return true; }
	else if(config == "numpad_equals") { *key = SDLK_KP_EQUALS; return true; }
	else if(config == "up") { *key = SDLK_UP; return true; }
	else if(config == "down") { *key = SDLK_DOWN; return true; }
	else if(config == "right") { *key = SDLK_RIGHT; return true; }
	else if(config == "left") { *key = SDLK_LEFT; return true; }
	else if(config == "insert") { *key = SDLK_INSERT; return true; }
	else if(config == "home") { *key = SDLK_HOME; return true; }
	else if(config == "end") { *key = SDLK_END; return true; }
	else if(config == "pageup") { *key = SDLK_PAGEUP; return true; }
	else if(config == "pagedown") { *key = SDLK_PAGEDOWN; return true; }
	else if(config == "rshift") { *key = SDLK_RSHIFT; return true; }
	else if(config == "lshift") { *key = SDLK_LSHIFT; return true; }
	else if(config == "rctrl") { *key = SDLK_RCTRL; return true; }
	else if(config == "lctrl") { *key = SDLK_LCTRL; return true; }
	else if(config == "ralt") { *key = SDLK_RALT; return true; }
	else if(config == "lalt") { *key = SDLK_LALT; return true; }

	return false;
}

bool Main::LoadToJoyButtonConfig(string config, JoystickMapping * joyButton)
{
	if(config == "joy0_0") { (*joyButton).joystick = 0; (*joyButton).button = 0; return true; }
	else if(config == "joy0_1") { (*joyButton).joystick = 0; (*joyButton).button = 1; return true; }
	else if(config == "joy0_2") { (*joyButton).joystick = 0; (*joyButton).button = 2; return true; }
	else if(config == "joy0_3") { (*joyButton).joystick = 0; (*joyButton).button = 3; return true; }
	else if(config == "joy0_4") { (*joyButton).joystick = 0; (*joyButton).button = 4; return true; }
	else if(config == "joy0_5") { (*joyButton).joystick = 0; (*joyButton).button = 5; return true; }
	else if(config == "joy0_6") { (*joyButton).joystick = 0; (*joyButton).button = 6; return true; }
	else if(config == "joy0_7") { (*joyButton).joystick = 0; (*joyButton).button = 7; return true; }
	else if(config == "joy0_8") { (*joyButton).joystick = 0; (*joyButton).button = 8; return true; }
	else if(config == "joy0_9") { (*joyButton).joystick = 0; (*joyButton).button = 9; return true; }
	else if(config == "joy0_10") { (*joyButton).joystick = 0; (*joyButton).button = 10; return true; }
	else if(config == "joy0_11") { (*joyButton).joystick = 0; (*joyButton).button = 11; return true; }
	else if(config == "joy0_12") { (*joyButton).joystick = 0; (*joyButton).button = 12; return true; }
	else if(config == "joy0_13") { (*joyButton).joystick = 0; (*joyButton).button = 13; return true; }
	else if(config == "joy0_14") { (*joyButton).joystick = 0; (*joyButton).button = 14; return true; }
	else if(config == "joy0_15") { (*joyButton).joystick = 0; (*joyButton).button = 15; return true; }
	else if(config == "joy0_16") { (*joyButton).joystick = 0; (*joyButton).button = 16; return true; }
	else if(config == "joy0_17") { (*joyButton).joystick = 0; (*joyButton).button = 17; return true; }
	else if(config == "joy0_18") { (*joyButton).joystick = 0; (*joyButton).button = 18; return true; }
	else if(config == "joy0_19") { (*joyButton).joystick = 0; (*joyButton).button = 19; return true; }
	else if(config == "joy1_0") { (*joyButton).joystick = 1; (*joyButton).button = 0; return true; }
	else if(config == "joy1_1") { (*joyButton).joystick = 1; (*joyButton).button = 1; return true; }
	else if(config == "joy1_2") { (*joyButton).joystick = 1; (*joyButton).button = 2; return true; }
	else if(config == "joy1_3") { (*joyButton).joystick = 1; (*joyButton).button = 3; return true; }
	else if(config == "joy1_4") { (*joyButton).joystick = 1; (*joyButton).button = 4; return true; }
	else if(config == "joy1_5") { (*joyButton).joystick = 1; (*joyButton).button = 5; return true; }
	else if(config == "joy1_6") { (*joyButton).joystick = 1; (*joyButton).button = 6; return true; }
	else if(config == "joy1_7") { (*joyButton).joystick = 1; (*joyButton).button = 7; return true; }
	else if(config == "joy1_8") { (*joyButton).joystick = 1; (*joyButton).button = 8; return true; }
	else if(config == "joy1_9") { (*joyButton).joystick = 1; (*joyButton).button = 9; return true; }
	else if(config == "joy1_10") { (*joyButton).joystick = 1; (*joyButton).button = 10; return true; }
	else if(config == "joy1_11") { (*joyButton).joystick = 1; (*joyButton).button = 11; return true; }
	else if(config == "joy1_12") { (*joyButton).joystick = 1; (*joyButton).button = 12; return true; }
	else if(config == "joy1_13") { (*joyButton).joystick = 1; (*joyButton).button = 13; return true; }
	else if(config == "joy1_14") { (*joyButton).joystick = 1; (*joyButton).button = 14; return true; }
	else if(config == "joy1_15") { (*joyButton).joystick = 1; (*joyButton).button = 15; return true; }
	else if(config == "joy1_16") { (*joyButton).joystick = 1; (*joyButton).button = 16; return true; }
	else if(config == "joy1_17") { (*joyButton).joystick = 1; (*joyButton).button = 17; return true; }
	else if(config == "joy1_18") { (*joyButton).joystick = 1; (*joyButton).button = 18; return true; }
	else if(config == "joy1_19") { (*joyButton).joystick = 1; (*joyButton).button = 19; return true; }
	else if(config == "joy2_0") { (*joyButton).joystick = 2; (*joyButton).button = 0; return true; }
	else if(config == "joy2_1") { (*joyButton).joystick = 2; (*joyButton).button = 1; return true; }
	else if(config == "joy2_2") { (*joyButton).joystick = 2; (*joyButton).button = 2; return true; }
	else if(config == "joy2_3") { (*joyButton).joystick = 2; (*joyButton).button = 3; return true; }
	else if(config == "joy2_4") { (*joyButton).joystick = 2; (*joyButton).button = 4; return true; }
	else if(config == "joy2_5") { (*joyButton).joystick = 2; (*joyButton).button = 5; return true; }
	else if(config == "joy2_6") { (*joyButton).joystick = 2; (*joyButton).button = 6; return true; }
	else if(config == "joy2_7") { (*joyButton).joystick = 2; (*joyButton).button = 7; return true; }
	else if(config == "joy2_8") { (*joyButton).joystick = 2; (*joyButton).button = 8; return true; }
	else if(config == "joy2_9") { (*joyButton).joystick = 2; (*joyButton).button = 9; return true; }
	else if(config == "joy2_10") { (*joyButton).joystick = 2; (*joyButton).button = 10; return true; }
	else if(config == "joy2_11") { (*joyButton).joystick = 2; (*joyButton).button = 11; return true; }
	else if(config == "joy2_12") { (*joyButton).joystick = 2; (*joyButton).button = 12; return true; }
	else if(config == "joy2_13") { (*joyButton).joystick = 2; (*joyButton).button = 13; return true; }
	else if(config == "joy2_14") { (*joyButton).joystick = 2; (*joyButton).button = 14; return true; }
	else if(config == "joy2_15") { (*joyButton).joystick = 2; (*joyButton).button = 15; return true; }
	else if(config == "joy2_16") { (*joyButton).joystick = 2; (*joyButton).button = 16; return true; }
	else if(config == "joy2_17") { (*joyButton).joystick = 2; (*joyButton).button = 17; return true; }
	else if(config == "joy2_18") { (*joyButton).joystick = 2; (*joyButton).button = 18; return true; }
	else if(config == "joy2_19") { (*joyButton).joystick = 2; (*joyButton).button = 19; return true; }
	else if(config == "joy3_0") { (*joyButton).joystick = 3; (*joyButton).button = 0; return true; }
	else if(config == "joy3_1") { (*joyButton).joystick = 3; (*joyButton).button = 1; return true; }
	else if(config == "joy3_2") { (*joyButton).joystick = 3; (*joyButton).button = 2; return true; }
	else if(config == "joy3_3") { (*joyButton).joystick = 3; (*joyButton).button = 3; return true; }
	else if(config == "joy3_4") { (*joyButton).joystick = 3; (*joyButton).button = 4; return true; }
	else if(config == "joy3_5") { (*joyButton).joystick = 3; (*joyButton).button = 5; return true; }
	else if(config == "joy3_6") { (*joyButton).joystick = 3; (*joyButton).button = 6; return true; }
	else if(config == "joy3_7") { (*joyButton).joystick = 3; (*joyButton).button = 7; return true; }
	else if(config == "joy3_8") { (*joyButton).joystick = 3; (*joyButton).button = 8; return true; }
	else if(config == "joy3_9") { (*joyButton).joystick = 3; (*joyButton).button = 9; return true; }
	else if(config == "joy3_10") { (*joyButton).joystick = 3; (*joyButton).button = 10; return true; }
	else if(config == "joy3_11") { (*joyButton).joystick = 3; (*joyButton).button = 11; return true; }
	else if(config == "joy3_12") { (*joyButton).joystick = 3; (*joyButton).button = 12; return true; }
	else if(config == "joy3_13") { (*joyButton).joystick = 3; (*joyButton).button = 13; return true; }
	else if(config == "joy3_14") { (*joyButton).joystick = 3; (*joyButton).button = 14; return true; }
	else if(config == "joy3_15") { (*joyButton).joystick = 3; (*joyButton).button = 15; return true; }
	else if(config == "joy3_16") { (*joyButton).joystick = 3; (*joyButton).button = 16; return true; }
	else if(config == "joy3_17") { (*joyButton).joystick = 3; (*joyButton).button = 17; return true; }
	else if(config == "joy3_18") { (*joyButton).joystick = 3; (*joyButton).button = 18; return true; }
	else if(config == "joy3_19") { (*joyButton).joystick = 3; (*joyButton).button = 19; return true; }

	return false;
}

bool Main::LoadToHatConfig(string config, Uint8 * hat)
{
	if(config == "joy0_hat") { *hat = 0; return true; }
	else if(config == "joy1_hat") { *hat = 1; return true; }
	else if(config == "joy2_hat") { *hat = 2; return true; }
	else if(config == "joy3_hat") { *hat = 3; return true; }

	return false;
}

bool Main::LoadToStickConfig(string config, Uint8 * stick)
{
	if(config == "joy0_stick") { *stick = 0; return true; }
	else if(config == "joy1_stick") { *stick = 1; return true; }
	else if(config == "joy2_stick") { *stick = 2; return true; }
	else if(config == "joy3_stick") { *stick = 3; return true; }

	return false;
}

int Main::SaveConfig()
{
	//create a new xml structure
	XMLDocument * file = new XMLDocument();

	XMLElement * general = file->NewElement("General");
	if(fullScreen)
	{
		general->SetAttribute("fullscreen", "true");
	}
	else
	{
		general->SetAttribute("fullscreen", "false");
	}

	XMLElement * playerOne = file->NewElement("PlayerOne");
	SetPlayerConfig(playerOne, 0);

	XMLElement * playerTwo = file->NewElement("PlayerTwo");
	SetPlayerConfig(playerTwo, 1);

	XMLElement * playerThree = file->NewElement("PlayerThree");
	SetPlayerConfig(playerThree, 2);

	XMLElement * playerFour = file->NewElement("PlayerFour");
	SetPlayerConfig(playerFour, 3);

	XMLElement * config = file->NewElement("Configuration");
	config->InsertEndChild(general);
	config->InsertEndChild(playerOne);
	config->InsertEndChild(playerTwo);
	config->InsertEndChild(playerThree);
	config->InsertEndChild(playerFour);

	file->InsertEndChild(config);

	file->SaveFile("config.xml");

	return 0;
}

void Main::SetPlayerConfig(XMLElement * config, int player)
{
	if(GetKeyConfigString(mappings[player].keyUp) != "")
	{
		config->SetAttribute("up", GetKeyConfigString(mappings[player].keyUp).data());
	}
	else if(GetJoyButtonConfigString(mappings[player].buttonUp) != "")
	{
		config->SetAttribute("up", GetJoyButtonConfigString(mappings[player].buttonUp).data());
	}
	else if(GetHatConfigString(mappings[player].hat) != "")
	{
		config->SetAttribute("up", GetHatConfigString(mappings[player].hat).data());
	}
	else if(GetStickConfigString(mappings[player].stick) != "")
	{
		config->SetAttribute("up", GetStickConfigString(mappings[player].stick).data());
	}
	
	if(GetKeyConfigString(mappings[player].keyDown) != "")
	{
		config->SetAttribute("down", GetKeyConfigString(mappings[player].keyDown).data());
	}
	else if(GetJoyButtonConfigString(mappings[player].buttonDown) != "")
	{
		config->SetAttribute("down", GetJoyButtonConfigString(mappings[player].buttonDown).data());
	}
	else if(GetHatConfigString(mappings[player].hat) != "")
	{
		config->SetAttribute("down", GetHatConfigString(mappings[player].hat).data());
	}
	else if(GetStickConfigString(mappings[player].stick) != "")
	{
		config->SetAttribute("down", GetStickConfigString(mappings[player].stick).data());
	}
	
	if(GetKeyConfigString(mappings[player].keyLeft) != "")
	{
		config->SetAttribute("left", GetKeyConfigString(mappings[player].keyLeft).data());
	}
	else if(GetJoyButtonConfigString(mappings[player].buttonLeft) != "")
	{
		config->SetAttribute("left", GetJoyButtonConfigString(mappings[player].buttonLeft).data());
	}
	else if(GetHatConfigString(mappings[player].hat) != "")
	{
		config->SetAttribute("left", GetHatConfigString(mappings[player].hat).data());
	}
	else if(GetStickConfigString(mappings[player].stick) != "")
	{
		config->SetAttribute("left", GetStickConfigString(mappings[player].stick).data());
	}
	
	if(GetKeyConfigString(mappings[player].keyRight) != "")
	{
		config->SetAttribute("right", GetKeyConfigString(mappings[player].keyRight).data());
	}
	else if(GetJoyButtonConfigString(mappings[player].buttonRight) != "")
	{
		config->SetAttribute("right", GetJoyButtonConfigString(mappings[player].buttonRight).data());
	}
	else if(GetHatConfigString(mappings[player].hat) != "")
	{
		config->SetAttribute("right", GetHatConfigString(mappings[player].hat).data());
	}
	else if(GetStickConfigString(mappings[player].stick) != "")
	{
		config->SetAttribute("right", GetStickConfigString(mappings[player].stick).data());
	}
	
	if(GetKeyConfigString(mappings[player].keyLight) != "")
	{
		config->SetAttribute("light", GetKeyConfigString(mappings[player].keyLight).data());
	}
	else if(GetJoyButtonConfigString(mappings[player].buttonLight) != "")
	{
		config->SetAttribute("light", GetJoyButtonConfigString(mappings[player].buttonLight).data());
	}
	
	if(GetKeyConfigString(mappings[player].keyHeavy) != "")
	{
		config->SetAttribute("heavy", GetKeyConfigString(mappings[player].keyHeavy).data());
	}
	else if(GetJoyButtonConfigString(mappings[player].buttonHeavy) != "")
	{
		config->SetAttribute("heavy", GetJoyButtonConfigString(mappings[player].buttonHeavy).data());
	}
	
	if(GetKeyConfigString(mappings[player].keyJump) != "")
	{
		config->SetAttribute("jump", GetKeyConfigString(mappings[player].keyJump).data());
	}
	else if(GetJoyButtonConfigString(mappings[player].buttonJump) != "")
	{
		config->SetAttribute("jump", GetJoyButtonConfigString(mappings[player].buttonJump).data());
	}
	
	if(GetKeyConfigString(mappings[player].keyBlock) != "")
	{
		config->SetAttribute("block", GetKeyConfigString(mappings[player].keyBlock).data());
	}
	else if(GetJoyButtonConfigString(mappings[player].buttonBlock) != "")
	{
		config->SetAttribute("block", GetJoyButtonConfigString(mappings[player].buttonBlock).data());
	}
	
	if(GetKeyConfigString(mappings[player].keyMenuConfirm) != "")
	{
		config->SetAttribute("menuConfirm", GetKeyConfigString(mappings[player].keyMenuConfirm).data());
	}
	else if(GetJoyButtonConfigString(mappings[player].buttonMenuConfirm) != "")
	{
		config->SetAttribute("menuConfirm", GetJoyButtonConfigString(mappings[player].buttonMenuConfirm).data());
	}
	
	if(GetKeyConfigString(mappings[player].keyMenuBack) != "")
	{
		config->SetAttribute("menuBack", GetKeyConfigString(mappings[player].keyMenuBack).data());
	}
	else if(GetJoyButtonConfigString(mappings[player].buttonMenuBack) != "")
	{
		config->SetAttribute("menuBack", GetJoyButtonConfigString(mappings[player].buttonMenuBack).data());
	}
	
	if(GetKeyConfigString(mappings[player].keyStart) != "")
	{
		config->SetAttribute("pause", GetKeyConfigString(mappings[player].keyStart).data());
	}
	else if(GetJoyButtonConfigString(mappings[player].buttonStart) != "")
	{
		config->SetAttribute("pause", GetJoyButtonConfigString(mappings[player].buttonStart).data());
	}
}

string Main::GetKeyConfigString(SDLKey key)
{
	switch(key)
	{
	case SDLK_BACKSPACE: return "backspace"; break;
	case SDLK_TAB: return "tab"; break;
	case SDLK_CLEAR: return "clear"; break;
	//case SDLK_RETURN: return "return"; break;
	case SDLK_PAUSE: return "pause"; break;
	//case SDLK_ESCAPE: return "escape"; break;
	case SDLK_SPACE: return "space"; break;
	case SDLK_EXCLAIM: return "exclaim"; break;
	case SDLK_QUOTEDBL: return "quotedbl"; break;
	case SDLK_HASH: return "hash"; break;
	case SDLK_DOLLAR: return "dollar"; break;
	case SDLK_AMPERSAND: return "ampersand"; break;
	case SDLK_QUOTE: return "quote"; break;
	case SDLK_LEFTPAREN: return "leftparen"; break;
	case SDLK_RIGHTPAREN: return "rightparen"; break;
	case SDLK_ASTERISK: return "asterisk"; break;
	case SDLK_PLUS: return "plus"; break;
	case SDLK_COMMA: return "comma"; break;
	case SDLK_MINUS: return "minus"; break;
	case SDLK_PERIOD: return "period"; break;
	case SDLK_SLASH: return "slash"; break;
	case SDLK_0: return "0"; break;
	case SDLK_1: return "1"; break;
	case SDLK_2: return "2"; break;
	case SDLK_3: return "3"; break;
	case SDLK_4: return "4"; break;
	case SDLK_5: return "5"; break;
	case SDLK_6: return "6"; break;
	case SDLK_7: return "7"; break;
	case SDLK_8: return "8"; break;
	case SDLK_9: return "9"; break;
	case SDLK_COLON: return "colon"; break;
	case SDLK_SEMICOLON: return "semicolon"; break;
	case SDLK_LESS: return "less"; break;
	case SDLK_EQUALS: return "equals"; break;
	case SDLK_GREATER: return "greater"; break;
	case SDLK_QUESTION: return "question"; break;
	case SDLK_AT: return "at"; break;
	case SDLK_LEFTBRACKET: return "leftbracket"; break;
	case SDLK_BACKSLASH: return "backslash"; break;
	case SDLK_RIGHTBRACKET: return "rightbracket"; break;
	case SDLK_CARET: return "caret"; break;
	case SDLK_UNDERSCORE: return "underscore"; break;
	case SDLK_BACKQUOTE: return "backquote"; break;
	case SDLK_a: return "a"; break;
	case SDLK_b: return "b"; break;
	case SDLK_c: return "c"; break;
	case SDLK_d: return "d"; break;
	case SDLK_e: return "e"; break;
	case SDLK_f: return "f"; break;
	case SDLK_g: return "g"; break;
	case SDLK_h: return "h"; break;
	case SDLK_i: return "i"; break;
	case SDLK_j: return "j"; break;
	case SDLK_k: return "k"; break;
	case SDLK_l: return "l"; break;
	case SDLK_m: return "m"; break;
	case SDLK_n: return "n"; break;
	case SDLK_o: return "o"; break;
	case SDLK_p: return "p"; break;
	case SDLK_q: return "q"; break;
	case SDLK_r: return "r"; break;
	case SDLK_s: return "s"; break;
	case SDLK_t: return "t"; break;
	case SDLK_u: return "u"; break;
	case SDLK_v: return "v"; break;
	case SDLK_w: return "w"; break;
	case SDLK_x: return "x"; break;
	case SDLK_y: return "y"; break;
	case SDLK_z: return "z"; break;
	case SDLK_DELETE: return "delete"; break;
	case SDLK_KP0: return "numpad_0"; break;
	case SDLK_KP1: return "numpad_1"; break;
	case SDLK_KP2: return "numpad_2"; break;
	case SDLK_KP3: return "numpad_3"; break;
	case SDLK_KP4: return "numpad_4"; break;
	case SDLK_KP5: return "numpad_5"; break;
	case SDLK_KP6: return "numpad_6"; break;
	case SDLK_KP7: return "numpad_7"; break;
	case SDLK_KP8: return "numpad_8"; break;
	case SDLK_KP9: return "numpad_9"; break;
	case SDLK_KP_PERIOD: return "numpad_period"; break;
	case SDLK_KP_DIVIDE: return "numpad_divide"; break;
	case SDLK_KP_MULTIPLY: return "numpad_multiply"; break;
	case SDLK_KP_MINUS: return "numpad_minus"; break;
	case SDLK_KP_PLUS: return "numpad_plus"; break;
	//case SDLK_KP_ENTER: return "numpad_enter"; break;
	case SDLK_KP_EQUALS: return "numpad_equals"; break;
	case SDLK_UP: return "up"; break;
	case SDLK_DOWN: return "down"; break;
	case SDLK_LEFT: return "left"; break;
	case SDLK_RIGHT: return "right"; break;
	case SDLK_INSERT: return "insert"; break;
	case SDLK_HOME: return "home"; break;
	case SDLK_END: return "end"; break;
	case SDLK_PAGEUP: return "pageup"; break;
	case SDLK_PAGEDOWN: return "pagedown"; break;
	case SDLK_RSHIFT: return "rshift"; break;
	case SDLK_LSHIFT: return "lshift"; break;
	case SDLK_RCTRL: return "rctrl"; break;
	case SDLK_LCTRL: return "lctrl"; break;
	case SDLK_RALT: return "ralt"; break;
	case SDLK_LALT: return "lalt"; break;
	}

	return "";
}

string Main::GetJoyButtonConfigString(JoystickMapping joyButton)
{
	switch(joyButton.joystick)
	{
	case 0:
		switch(joyButton.button)
		{
		case 0: return "joy0_0"; break;
		case 1: return "joy0_1"; break;
		case 2: return "joy0_2"; break;
		case 3: return "joy0_3"; break;
		case 4: return "joy0_4"; break;
		case 5: return "joy0_5"; break;
		case 6: return "joy0_6"; break;
		case 7: return "joy0_7"; break;
		case 8: return "joy0_8"; break;
		case 9: return "joy0_9"; break;
		case 10: return "joy0_10"; break;
		case 11: return "joy0_11"; break;
		case 12: return "joy0_12"; break;
		case 13: return "joy0_13"; break;
		case 14: return "joy0_14"; break;
		case 15: return "joy0_15"; break;
		case 16: return "joy0_16"; break;
		case 17: return "joy0_17"; break;
		case 18: return "joy0_18"; break;
		case 19: return "joy0_19"; break;
		}
		break;
	case 1:
		switch(joyButton.button)
		{
		case 0: return "joy1_0"; break;
		case 1: return "joy1_1"; break;
		case 2: return "joy1_2"; break;
		case 3: return "joy1_3"; break;
		case 4: return "joy1_4"; break;
		case 5: return "joy1_5"; break;
		case 6: return "joy1_6"; break;
		case 7: return "joy1_7"; break;
		case 8: return "joy1_8"; break;
		case 9: return "joy1_9"; break;
		case 10: return "joy1_10"; break;
		case 11: return "joy1_11"; break;
		case 12: return "joy1_12"; break;
		case 13: return "joy1_13"; break;
		case 14: return "joy1_14"; break;
		case 15: return "joy1_15"; break;
		case 16: return "joy1_16"; break;
		case 17: return "joy1_17"; break;
		case 18: return "joy1_18"; break;
		case 19: return "joy1_19"; break;
		}
		break;
	case 2:
		switch(joyButton.button)
		{
		case 0: return "joy2_0"; break;
		case 1: return "joy2_1"; break;
		case 2: return "joy2_2"; break;
		case 3: return "joy2_3"; break;
		case 4: return "joy2_4"; break;
		case 5: return "joy2_5"; break;
		case 6: return "joy2_6"; break;
		case 7: return "joy2_7"; break;
		case 8: return "joy2_8"; break;
		case 9: return "joy2_9"; break;
		case 10: return "joy2_10"; break;
		case 11: return "joy2_11"; break;
		case 12: return "joy2_12"; break;
		case 13: return "joy2_13"; break;
		case 14: return "joy2_14"; break;
		case 15: return "joy2_15"; break;
		case 16: return "joy2_16"; break;
		case 17: return "joy2_17"; break;
		case 18: return "joy2_18"; break;
		case 19: return "joy2_19"; break;
		}
		break;
	case 3:
		switch(joyButton.button)
		{
		case 0: return "joy3_0"; break;
		case 1: return "joy3_1"; break;
		case 2: return "joy3_2"; break;
		case 3: return "joy3_3"; break;
		case 4: return "joy3_4"; break;
		case 5: return "joy3_5"; break;
		case 6: return "joy3_6"; break;
		case 7: return "joy3_7"; break;
		case 8: return "joy3_8"; break;
		case 9: return "joy3_9"; break;
		case 10: return "joy3_10"; break;
		case 11: return "joy3_11"; break;
		case 12: return "joy3_12"; break;
		case 13: return "joy3_13"; break;
		case 14: return "joy3_14"; break;
		case 15: return "joy3_15"; break;
		case 16: return "joy3_16"; break;
		case 17: return "joy3_17"; break;
		case 18: return "joy3_18"; break;
		case 19: return "joy3_19"; break;
		}
		break;
	}

	return "";
}

string Main::GetHatConfigString(Uint8 hat)
{
	switch(hat)
	{
	case 0: return "joy0_hat"; break;
	case 1: return "joy1_hat"; break;
	case 2: return "joy2_hat"; break;
	case 3: return "joy3_hat"; break;
	}

	return "";
}

string Main::GetStickConfigString(Uint8 stick)
{
	switch(stick)
	{
	case 0: return "joy0_stick"; break;
	case 1: return "joy1_stick"; break;
	case 2: return "joy2_stick"; break;
	case 3: return "joy3_stick"; break;
	}

	return "";
}

//animation and holds
int Main::AdvanceHolds()
{
	list<HSObject*>::iterator objIt;
	for ( objIt=HUDObjects.begin(); objIt != HUDObjects.end(); objIt++)
	{
		if(int error = (*objIt)->AdvanceHolds() != 0)
		{
			return error; //there was an error advancing holds
		}
	}

	if(gameState == MATCH && matchState != IN_PROGRESS) { return 0; }

	for ( objIt=gameObjects.begin(); objIt != gameObjects.end(); objIt++)
	{
		if(int error = (*objIt)->AdvanceHolds() != 0)
		{
			return error; //there was an error advancing holds
		}
	}

	return 0;
}

//event/input stuff
int Main::Event(SDL_Event* curEvent)
{
	//reset current inputs
	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		inputStateChange[i] = false;
		*curInputs[i] = defaultInputs;
		curInputs[i]->frame = frame;
	}

	//get current inputs
	while(SDL_PollEvent(curEvent))
	{
        HandleEvent(curEvent);
    }

	if(mainMenuState == INPUT_KEY) { return 0; } 

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(!inputStateChange[i]) { continue; }

		//set "held" for keyboard key presses
		curInputs[i]->bKeyUp.held = IsHeld(&curInputs[i]->bKeyUp, &inputHistory[i]->bKeyUp);
		curInputs[i]->bKeyDown.held = IsHeld(&curInputs[i]->bKeyDown, &inputHistory[i]->bKeyDown);
		curInputs[i]->bKeyLeft.held = IsHeld(&curInputs[i]->bKeyLeft, &inputHistory[i]->bKeyLeft);
		curInputs[i]->bKeyRight.held = IsHeld(&curInputs[i]->bKeyRight, &inputHistory[i]->bKeyRight);
		curInputs[i]->bKeyJump.held = IsHeld(&curInputs[i]->bKeyJump, &inputHistory[i]->bKeyJump);
		curInputs[i]->bKeyBlock.held = IsHeld(&curInputs[i]->bKeyBlock, &inputHistory[i]->bKeyBlock);
		curInputs[i]->bKeyLight.held = IsHeld(&curInputs[i]->bKeyLight, &inputHistory[i]->bKeyLight);
		curInputs[i]->bKeyHeavy.held = IsHeld(&curInputs[i]->bKeyHeavy, &inputHistory[i]->bKeyHeavy);
		curInputs[i]->bKeyStart.held = IsHeld(&curInputs[i]->bKeyStart, &inputHistory[i]->bKeyStart);
		curInputs[i]->bKeySelect.held = IsHeld(&curInputs[i]->bKeySelect, &inputHistory[i]->bKeySelect);
		curInputs[i]->keyMenuConfirm.held = IsHeld(&curInputs[i]->keyMenuConfirm, &inputHistory[i]->keyMenuConfirm);
		curInputs[i]->keyMenuBack.held = IsHeld(&curInputs[i]->keyMenuBack, &inputHistory[i]->keyMenuBack);
		
		//set "held" for joystick button presses
		curInputs[i]->bButtonUp.held = IsHeld(&curInputs[i]->bButtonUp, &inputHistory[i]->bButtonUp);
		curInputs[i]->bButtonDown.held = IsHeld(&curInputs[i]->bButtonDown, &inputHistory[i]->bButtonDown);
		curInputs[i]->bButtonLeft.held = IsHeld(&curInputs[i]->bButtonLeft, &inputHistory[i]->bButtonLeft);
		curInputs[i]->bButtonRight.held = IsHeld(&curInputs[i]->bButtonRight, &inputHistory[i]->bButtonRight);
		curInputs[i]->bButtonJump.held = IsHeld(&curInputs[i]->bButtonJump, &inputHistory[i]->bButtonJump);
		curInputs[i]->bButtonBlock.held = IsHeld(&curInputs[i]->bButtonBlock, &inputHistory[i]->bButtonBlock);
		curInputs[i]->bButtonLight.held = IsHeld(&curInputs[i]->bButtonLight, &inputHistory[i]->bButtonLight);
		curInputs[i]->bButtonHeavy.held = IsHeld(&curInputs[i]->bButtonHeavy, &inputHistory[i]->bButtonHeavy);
		curInputs[i]->bButtonStart.held = IsHeld(&curInputs[i]->bButtonStart, &inputHistory[i]->bButtonStart);
		curInputs[i]->bButtonSelect.held = IsHeld(&curInputs[i]->bButtonSelect, &inputHistory[i]->bButtonSelect);
		curInputs[i]->buttonMenuConfirm.held = IsHeld(&curInputs[i]->buttonMenuConfirm, &inputHistory[i]->buttonMenuConfirm);
		curInputs[i]->buttonMenuBack.held = IsHeld(&curInputs[i]->buttonMenuBack, &inputHistory[i]->buttonMenuBack);

		//set "held" for joystick hat presses
		curInputs[i]->bHatUp.held = IsHeld(&curInputs[i]->bHatUp, &inputHistory[i]->bHatUp);
		curInputs[i]->bHatDown.held = IsHeld(&curInputs[i]->bHatDown, &inputHistory[i]->bHatDown);
		curInputs[i]->bHatLeft.held = IsHeld(&curInputs[i]->bHatLeft, &inputHistory[i]->bHatLeft);
		curInputs[i]->bHatRight.held = IsHeld(&curInputs[i]->bHatRight, &inputHistory[i]->bHatRight);

		//set "held" for joystick stick movements
		curInputs[i]->bStickUp.held = IsHeld(&curInputs[i]->bStickUp, &inputHistory[i]->bStickUp);
		curInputs[i]->bStickDown.held = IsHeld(&curInputs[i]->bStickDown, &inputHistory[i]->bStickDown);
		curInputs[i]->bStickLeft.held = IsHeld(&curInputs[i]->bStickLeft, &inputHistory[i]->bStickLeft);
		curInputs[i]->bStickRight.held = IsHeld(&curInputs[i]->bStickRight, &inputHistory[i]->bStickRight);
		curInputs[i]->bStickHardUp.held = IsHeld(&curInputs[i]->bStickHardUp, &inputHistory[i]->bStickHardUp);
		curInputs[i]->bStickHardDown.held = IsHeld(&curInputs[i]->bStickHardDown, &inputHistory[i]->bStickHardDown);
		curInputs[i]->bStickHardLeft.held = IsHeld(&curInputs[i]->bStickHardLeft, &inputHistory[i]->bStickHardLeft);
		curInputs[i]->bStickHardRight.held = IsHeld(&curInputs[i]->bStickHardRight, &inputHistory[i]->bStickHardRight);

		//save the current inputs
		InputStates * newInputs = new InputStates();
		*newInputs = *curInputs[i];
		newInputs->prevInputState = inputHistory[i];
		inputHistory[i] = newInputs;

		//clean out the outdated input history
		/*int historyCount = 1;
		InputStates * is = inputHistory[i]->prevInputStates;
		InputStates * prevIs = NULL;
		while((is = is->prevInputStates) != NULL)
		{
			delete prevIs;
			historyCount++;
			if(historyCount > MAX_INPUT_HISTORY)
			{
				prevIs = is;
			}
		}

		delete prevIs;*/
	}
	
	//call event handling on players
	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(gameState == MAIN_MENU)
		{
			if(int error = EventMainMenu(inputHistory[i], frame) != 0) { return error; }
		}
		else if(gameState == CHARACTER_SELECT)
		{
			if(int error = EventCharacterSelect(inputHistory[i], frame, i) != 0) { return error; }
		}
		else if(gameState == MATCH)
		{
			if(int error = EventMatch(inputHistory[i], frame, i) != 0) { return error; }
		}
	}

	return 0;
}

bool Main::IsHeld(InputState * cur, InputState * prev)
{
	//set "held" to true if "pressed" is true OR if the previous inputs "held" true but the current "released" is not true
	if(cur->pressed == true || (cur->released == false && prev->held == true))
	{
		return true;
	}
	return false;
}

bool Main::IsPressed(InputState * cur, InputState * prev)
{
	//if "held" is true currently but wasn't previously, set "pressed" to true
	if(cur->held == true && prev->held == false)
	{
		return true;
	}
	return false;
}

bool Main::IsReleased(InputState * cur, InputState * prev)
{
	//if "held" is currently not true but was true perviously, set "released" to true
	if(cur->held == false && prev->held == true)
	{
		return true;
	}
	return false;
}

void Main::ClearControls(int player)
{
	if(player < 0 || player >= MAX_PLAYERS) { return; }
	mappings[player].buttonUp.button = JOYBUTTON_UNKNOWN;
	mappings[player].buttonUp.joystick = JOYSTICK_UNKNOWN;
	mappings[player].buttonDown.button = JOYBUTTON_UNKNOWN;
	mappings[player].buttonDown.joystick = JOYSTICK_UNKNOWN;
	mappings[player].buttonLeft.button = JOYBUTTON_UNKNOWN;
	mappings[player].buttonLeft.joystick = JOYSTICK_UNKNOWN;
	mappings[player].buttonRight.button = JOYBUTTON_UNKNOWN;
	mappings[player].buttonRight.joystick = JOYSTICK_UNKNOWN;
	mappings[player].buttonJump.button = JOYBUTTON_UNKNOWN;
	mappings[player].buttonJump.joystick = JOYSTICK_UNKNOWN;
	mappings[player].buttonLight.button = JOYBUTTON_UNKNOWN;
	mappings[player].buttonLight.joystick = JOYSTICK_UNKNOWN;
	mappings[player].buttonHeavy.button = JOYBUTTON_UNKNOWN;
	mappings[player].buttonHeavy.joystick = JOYSTICK_UNKNOWN;
	mappings[player].buttonBlock.button = JOYBUTTON_UNKNOWN;
	mappings[player].buttonBlock.joystick = JOYSTICK_UNKNOWN;
	mappings[player].buttonStart.button = JOYBUTTON_UNKNOWN;
	mappings[player].buttonStart.joystick = JOYSTICK_UNKNOWN;
	mappings[player].buttonMenuConfirm.button = JOYBUTTON_UNKNOWN;
	mappings[player].buttonMenuConfirm.joystick = JOYSTICK_UNKNOWN;
	mappings[player].buttonMenuBack.button = JOYBUTTON_UNKNOWN;
	mappings[player].buttonMenuBack.joystick = JOYSTICK_UNKNOWN;

	mappings[player].keyUp = SDLK_UNKNOWN;
	mappings[player].keyDown = SDLK_UNKNOWN;
	mappings[player].keyLeft = SDLK_UNKNOWN;
	mappings[player].keyRight = SDLK_UNKNOWN;
	mappings[player].keyJump = SDLK_UNKNOWN;
	mappings[player].keyLight = SDLK_UNKNOWN;
	mappings[player].keyHeavy = SDLK_UNKNOWN;
	mappings[player].keyBlock = SDLK_UNKNOWN;
	mappings[player].keyStart = SDLK_UNKNOWN;
	mappings[player].keyMenuConfirm = SDLK_UNKNOWN;
	mappings[player].keyMenuBack= SDLK_UNKNOWN;

	mappings[player].hat = JOYSTICK_UNKNOWN;
	mappings[player].stick = JOYSTICK_UNKNOWN;
}

void Main::ChangePlayerPalette(int player)
{
	HSPalette * curPalette = players[player]->palette;
	players[player]->palette = NULL;

	curPalette->usingCount--;
	if(curPalette->usingCount < 0) { curPalette->usingCount = 0; }

	list<HSPalette*>::iterator plIt;
	bool getNextPalette = false;
	for ( plIt=players[player]->palettes.begin(); plIt != players[player]->palettes.end(); plIt++)
	{
		if(getNextPalette && (*plIt)->usingCount <= 0)
		{
			players[player]->palette = (*plIt);
			(*plIt)->usingCount++;
			break;
		}
		else if((*plIt) == curPalette)
		{
			getNextPalette = true;
		}
	}
	
	if(players[player]->palette == NULL && !players[player]->palettes.empty())
	{
		players[player]->palette = players[player]->palettes.front();
		players[player]->palette->usingCount++;
	}
}

int Main::HandleEvent(SDL_Event* Event) 
{
    switch(Event->type) 
	{
        case SDL_ACTIVEEVENT: 
		{
            switch(Event->active.state) 
			{
                case SDL_APPMOUSEFOCUS: 
				{
                    if ( Event->active.gain )  MouseFocus();
                    else               MouseBlur();

                    break;
                }
                case SDL_APPINPUTFOCUS: 
				{
                    if ( Event->active.gain )  InputFocus();
                    else               InputBlur();

                    break;
                }
                case SDL_APPACTIVE: 
				{
                    if ( Event->active.gain )  Restore();
                    else               Minimize();

                    break;
                }
            }
            break;
        }

        case SDL_KEYDOWN: 
		{
			KeyDown(Event->key.keysym.sym,Event->key.keysym.mod,Event->key.keysym.unicode);
            break;
        }

        case SDL_KEYUP: 
		{
			KeyUp(Event->key.keysym.sym,Event->key.keysym.mod,Event->key.keysym.unicode);
            break;
        }

        case SDL_MOUSEMOTION: 
		{
			MouseMove(Event->motion.x,Event->motion.y,Event->motion.xrel,Event->motion.yrel,(Event->motion.state&SDL_BUTTON(SDL_BUTTON_LEFT))!=0,(Event->motion.state&SDL_BUTTON(SDL_BUTTON_RIGHT))!=0,(Event->motion.state&SDL_BUTTON(SDL_BUTTON_MIDDLE))!=0);
            break;
        }

        case SDL_MOUSEBUTTONDOWN: 
			{
            switch(Event->button.button) 
			{
                case SDL_BUTTON_LEFT: 
				{
					LButtonDown(Event->button.x,Event->button.y);
                    break;
                }
                case SDL_BUTTON_RIGHT: 
				{
					RButtonDown(Event->button.x,Event->button.y);
                    break;
                }
                case SDL_BUTTON_MIDDLE: 
				{
					MButtonDown(Event->button.x,Event->button.y);
                    break;
                }
            }
            break;
        }

        case SDL_MOUSEBUTTONUP: 
			{
            switch(Event->button.button) 
			{
                case SDL_BUTTON_LEFT: 
				{
					LButtonUp(Event->button.x,Event->button.y);
                    break;
                }
                case SDL_BUTTON_RIGHT: 
				{
					RButtonUp(Event->button.x,Event->button.y);
                    break;
                }
                case SDL_BUTTON_MIDDLE: 
				{
					MButtonUp(Event->button.x,Event->button.y);
                    break;
                }
            }
            break;
        }

        case SDL_JOYAXISMOTION: 
		{
			JoyAxis(Event->jaxis.which,Event->jaxis.axis,Event->jaxis.value);
            break;
        }

        case SDL_JOYBALLMOTION: 
		{
			JoyBall(Event->jball.which,Event->jball.ball,Event->jball.xrel,Event->jball.yrel);
            break;
        }

        case SDL_JOYHATMOTION: 
		{
			JoyHat(Event->jhat.which,Event->jhat.hat,Event->jhat.value);
            break;
        }
        case SDL_JOYBUTTONDOWN:
		{
			JoyButtonDown(Event->jbutton.which,Event->jbutton.button);
            break;
        }

        case SDL_JOYBUTTONUP:
		{
			JoyButtonUp(Event->jbutton.which,Event->jbutton.button);
            break;
        }

        case SDL_QUIT: 
		{
			Exit();
            break;
        }

        case SDL_SYSWMEVENT: 
		{
            //Ignore
            break;
        }

        case SDL_VIDEORESIZE: 
		{
			Resize(Event->resize.w,Event->resize.h);
            break;
        }

        case SDL_VIDEOEXPOSE: 
		{
			Expose();
            break;
        }

        default: 
		{
			User(Event->user.type,Event->user.code,Event->user.data1,Event->user.data2);
            break;
        }
    }

	return 0;
}



void Main::InputFocus() 
{
    
}

void Main::InputBlur() 
{
    
}

void Main::KeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) 
{
	if(mainMenuState == INPUT_KEY || pauseMenuState == PAUSE_INPUT_KEY)
	{
		switch(keyToSetUp)
		{
		case 0: //up
			mappings[playerToSetUp].keyUp = sym;
			mappings[playerToSetUp].stick = -1;
			mappings[playerToSetUp].hat = -1;
			mappings[playerToSetUp].buttonUp.joystick = -1;
			mappings[playerToSetUp].buttonUp.button = -1;
			keyToSetUp = -1;
			menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveConfig();
			break;
		case 1: //down
			mappings[playerToSetUp].keyDown = sym;
			mappings[playerToSetUp].stick = -1;
			mappings[playerToSetUp].hat = -1;
			mappings[playerToSetUp].buttonDown.joystick = -1;
			mappings[playerToSetUp].buttonDown.button = -1;
			keyToSetUp = -1;
			menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveConfig();
			break;
		case 2: //left
			mappings[playerToSetUp].keyLeft = sym;
			mappings[playerToSetUp].stick = -1;
			mappings[playerToSetUp].hat = -1;
			mappings[playerToSetUp].buttonLeft.joystick = -1;
			mappings[playerToSetUp].buttonLeft.button = -1;
			keyToSetUp = -1;
			menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveConfig();
			break;
		case 3: //right
			mappings[playerToSetUp].keyRight = sym;
			mappings[playerToSetUp].stick = -1;
			mappings[playerToSetUp].hat = -1;
			mappings[playerToSetUp].buttonRight.joystick = -1;
			mappings[playerToSetUp].buttonRight.button = -1;
			keyToSetUp = -1;
			menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveConfig();
			break;
		case 4: //light attack
			mappings[playerToSetUp].keyLight = sym;
			mappings[playerToSetUp].buttonLight.joystick = -1;
			mappings[playerToSetUp].buttonLight.button = -1;
			keyToSetUp = -1;
			menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveConfig();
			break;
		case 5: //heavy attack
			mappings[playerToSetUp].keyHeavy = sym;
			mappings[playerToSetUp].buttonHeavy.joystick = -1;
			mappings[playerToSetUp].buttonHeavy.button = -1;
			keyToSetUp = -1;
			menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveConfig();
			break;
		case 6: //jump
			mappings[playerToSetUp].keyJump = sym;
			mappings[playerToSetUp].buttonJump.joystick = -1;
			mappings[playerToSetUp].buttonJump.button = -1;
			keyToSetUp = -1;
			menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveConfig();
			break;
		case 7: //block
			mappings[playerToSetUp].keyBlock = sym;
			mappings[playerToSetUp].buttonBlock.joystick = -1;
			mappings[playerToSetUp].buttonBlock.button = -1;
			keyToSetUp = -1;
			menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveConfig();
			break;
		case 8: //pause
			mappings[playerToSetUp].keyStart = sym;
			mappings[playerToSetUp].buttonStart.joystick = -1;
			mappings[playerToSetUp].buttonStart.button = -1;
			keyToSetUp = -1;
			menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveConfig();
			break;
		case 9: //menu confirm
			mappings[playerToSetUp].keyMenuConfirm = sym;
			mappings[playerToSetUp].buttonMenuConfirm.joystick = -1;
			mappings[playerToSetUp].buttonMenuConfirm.button = -1;
			keyToSetUp = -1;
			menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveConfig();
			break;
		case 10: //menu back
			mappings[playerToSetUp].keyMenuBack = sym;
			mappings[playerToSetUp].buttonMenuBack.joystick = -1;
			mappings[playerToSetUp].buttonMenuBack.button = -1;
			keyToSetUp = -1;
			menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveConfig();
			break;
		}

		return;
	}

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(i == 0 && (SDLK_RETURN == sym || SDLK_KP_ENTER == sym))
		{
			curInputs[i]->keyMenuConfirm.pressed = true;
			inputStateChange[i] = true;
		}
		if(i == 0 && SDLK_ESCAPE == sym)
		{
			curInputs[i]->keyMenuBack.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].keyUp == sym)
		{
			curInputs[i]->bKeyUp.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].keyDown == sym)
		{
			curInputs[i]->bKeyDown.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].keyLeft == sym)
		{
			curInputs[i]->bKeyLeft.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].keyRight == sym)
		{
			curInputs[i]->bKeyRight.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].keyJump == sym)
		{
			curInputs[i]->bKeyJump.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].keyBlock == sym)
		{
			curInputs[i]->bKeyBlock.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].keyLight == sym)
		{
			curInputs[i]->bKeyLight.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].keyHeavy == sym)
		{
			curInputs[i]->bKeyHeavy.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].keyStart == sym)
		{
			curInputs[i]->bKeyStart.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].keySelect == sym)
		{
			curInputs[i]->bKeySelect.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].keyMenuConfirm == sym)
		{
			curInputs[i]->keyMenuConfirm.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].keyMenuBack == sym)
		{
			curInputs[i]->keyMenuBack.pressed = true;
			inputStateChange[i] = true;
		}
	}
}

void Main::KeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) 
{
	if(keyToSetUp > 0)
	{
		return;
	}

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(mappings[i].keyUp == sym)
		{
			curInputs[i]->bKeyUp.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].keyDown == sym)
		{
			curInputs[i]->bKeyDown.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].keyLeft == sym)
		{
			curInputs[i]->bKeyLeft.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].keyRight == sym)
		{
			curInputs[i]->bKeyRight.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].keyJump == sym)
		{
			curInputs[i]->bKeyJump.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].keyBlock == sym)
		{
			curInputs[i]->bKeyBlock.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].keyLight == sym)
		{
			curInputs[i]->bKeyLight.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].keyHeavy == sym)
		{
			curInputs[i]->bKeyHeavy.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].keyStart == sym)
		{
			curInputs[i]->bKeyStart.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].keySelect == sym)
		{
			curInputs[i]->bKeySelect.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].keyMenuConfirm == sym)
		{
			curInputs[i]->keyMenuConfirm.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].keyMenuBack == sym)
		{
			curInputs[i]->keyMenuBack.released = true;
			inputStateChange[i] = true;
		}
	}
}

void Main::MouseFocus() 
{

}

void Main::MouseBlur() 
{
    
}

void Main::MouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle) 
{
    
}

void Main::MouseWheel(bool Up, bool Down) 
{
    
}

void Main::LButtonDown(int mX, int mY) 
{
    
}

void Main::LButtonUp(int mX, int mY) 
{
    
}

void Main::RButtonDown(int mX, int mY) 
{
    
}

void Main::RButtonUp(int mX, int mY) 
{
    
}

void Main::MButtonDown(int mX, int mY) 
{
    
}

void Main::MButtonUp(int mX, int mY) 
{
    
}

void Main::JoyAxis(Uint8 which,Uint8 axis,Sint16 value) 
{
	if(mainMenuState == INPUT_KEY || pauseMenuState == PAUSE_INPUT_KEY)
	{
		if(abs(value) <= STICK_THRESHOLD) { return; }

		switch(keyToSetUp)
		{
		case 0: //up
			menuManager->CursorNext();
		case 1: //down
			menuManager->CursorNext();
		case 2: //left
			menuManager->CursorNext();
		case 3: //right
			mappings[playerToSetUp].stick = which;
			mappings[playerToSetUp].hat = -1;
			mappings[playerToSetUp].keyUp = SDLK_UNKNOWN;
			mappings[playerToSetUp].buttonUp.joystick = -1;
			mappings[playerToSetUp].buttonUp.button = -1;
			mappings[playerToSetUp].keyDown = SDLK_UNKNOWN;
			mappings[playerToSetUp].buttonDown.joystick = -1;
			mappings[playerToSetUp].buttonDown.button = -1;
			mappings[playerToSetUp].keyLeft = SDLK_UNKNOWN;
			mappings[playerToSetUp].buttonLeft.joystick = -1;
			mappings[playerToSetUp].buttonLeft.button = -1;
			mappings[playerToSetUp].keyRight = SDLK_UNKNOWN;
			mappings[playerToSetUp].buttonRight.joystick = -1;
			mappings[playerToSetUp].buttonRight.button = -1;
			menuManager->CursorNext();
			keyToSetUp = -1;
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveConfig();
			break;
		}

		return;
	}

    InputStates * tempInputs = NULL;
	PreviousJoystickStates * prevStickState = NULL;
	int player = -1;
	
	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(mappings[i].stick == which)
		{
			tempInputs = curInputs[i];
			prevStickState = &prevJoystickStates[i];
			player = i;
		}
	}

	if(tempInputs == NULL) { return; }

	if(axis == 0)
	{
		if(value < -STICK_THRESHOLD)
		{
			if(!prevStickState->stickHeldLeft) { tempInputs->bStickLeft.pressed = true; inputStateChange[player] = true; }
			prevStickState->stickHeldLeft = true;
		}
		else if(value > STICK_THRESHOLD)
		{
			if(!prevStickState->stickHeldRight) { tempInputs->bStickRight.pressed = true; inputStateChange[player] = true; }
			prevStickState->stickHeldRight = true;
		}
		else
		{
			if(prevStickState->stickHeldLeft) { tempInputs->bStickLeft.released = true; inputStateChange[player] = true; }
			if(prevStickState->stickHeldRight) { tempInputs->bStickRight.released = true; inputStateChange[player] = true; }
			prevStickState->stickHeldLeft = false;
			prevStickState->stickHeldRight = false;
		}

		if(value < -STICK_HARD_THRESHOLD)
		{
			if(!prevStickState->stickHeldHardLeft) { tempInputs->bStickHardLeft.pressed = true; inputStateChange[player] = true; }
			prevStickState->stickHeldHardLeft = true;
		}
		else if(value > STICK_HARD_THRESHOLD)
		{
			if(!prevStickState->stickHeldHardRight) { tempInputs->bStickHardRight.pressed = true; inputStateChange[player] = true; }
			prevStickState->stickHeldHardRight = true;
		}
		else
		{
			if(prevStickState->stickHeldHardLeft) { tempInputs->bStickHardLeft.released = true; inputStateChange[player] = true; }
			if(prevStickState->stickHeldHardRight) { tempInputs->bStickHardRight.released = true; inputStateChange[player] = true; }
			prevStickState->stickHeldHardLeft = false;
			prevStickState->stickHeldHardRight = false;
		}
	}
	else if(axis == 1)
	{
		if(value < -STICK_THRESHOLD)
		{
			if(!prevStickState->stickHeldUp) { tempInputs->bStickUp.pressed = true; inputStateChange[player] = true; }
			prevStickState->stickHeldUp = true;
		}
		else if(value > STICK_THRESHOLD)
		{
			if(!prevStickState->stickHeldDown) { tempInputs->bStickDown.pressed = true; inputStateChange[player] = true; }
			prevStickState->stickHeldDown = true;
		}
		else
		{
			if(prevStickState->stickHeldUp) { tempInputs->bStickUp.released = true; inputStateChange[player] = true; }
			if(prevStickState->stickHeldDown) { tempInputs->bStickDown.released = true; inputStateChange[player] = true; }
			prevStickState->stickHeldUp = false;
			prevStickState->stickHeldDown = false;
		}
		
		if(value < -STICK_HARD_THRESHOLD)
		{
			if(!prevStickState->stickHeldHardUp) { tempInputs->bStickHardUp.pressed = true; inputStateChange[player] = true; }
			prevStickState->stickHeldHardUp = true;
		}
		else if(value > STICK_HARD_THRESHOLD)
		{
			if(!prevStickState->stickHeldHardDown) { tempInputs->bStickHardDown.pressed = true; inputStateChange[player] = true; }
			prevStickState->stickHeldHardDown = true;
		}
		else
		{
			if(prevStickState->stickHeldHardUp) { tempInputs->bStickHardUp.released = true; inputStateChange[player] = true; }
			if(prevStickState->stickHeldHardDown) { tempInputs->bStickHardDown.released = true; inputStateChange[player] = true; }
			prevStickState->stickHeldHardUp = false;
			prevStickState->stickHeldHardDown = false;
		}
	}
}

void Main::JoyHat(Uint8 which,Uint8 hat,Uint8 value) 
{
	if(mainMenuState == INPUT_KEY || pauseMenuState == PAUSE_INPUT_KEY)
	{
		switch(keyToSetUp)
		{
		case 0: //up
			menuManager->CursorNext();
		case 1: //down
			menuManager->CursorNext();
		case 2: //left
			menuManager->CursorNext();
		case 3: //right
			mappings[playerToSetUp].hat = which;
			mappings[playerToSetUp].stick = -1;
			mappings[playerToSetUp].keyUp = SDLK_UNKNOWN;
			mappings[playerToSetUp].buttonUp.joystick = -1;
			mappings[playerToSetUp].buttonUp.button = -1;
			mappings[playerToSetUp].keyDown = SDLK_UNKNOWN;
			mappings[playerToSetUp].buttonDown.joystick = -1;
			mappings[playerToSetUp].buttonDown.button = -1;
			mappings[playerToSetUp].keyLeft = SDLK_UNKNOWN;
			mappings[playerToSetUp].buttonLeft.joystick = -1;
			mappings[playerToSetUp].buttonLeft.button = -1;
			mappings[playerToSetUp].keyRight = SDLK_UNKNOWN;
			mappings[playerToSetUp].buttonRight.joystick = -1;
			mappings[playerToSetUp].buttonRight.button = -1;
			menuManager->CursorNext();
			keyToSetUp = -1;
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveConfig();
			break;
		}

		return;
	}

    InputStates * tempInputs = NULL;
	PreviousJoystickStates * prevStickState;
	int player = -1;
	
	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(mappings[i].hat == which)
		{
			tempInputs = curInputs[i];
			prevStickState = &prevJoystickStates[i];
			player = i;
		}
	}

	if(tempInputs == NULL)
	{
		return;
	}

	if(value & SDL_HAT_UP)
	{
		if(!(prevStickState->hat & SDL_HAT_UP))
		{
			tempInputs->bHatUp.pressed = true;
			inputStateChange[player] = true;
		}
	}
	else
	{
		if(prevStickState->hat & SDL_HAT_UP)
		{
			tempInputs->bHatUp.released = true;
			inputStateChange[player] = true;
		}
	}

	if(value & SDL_HAT_DOWN)
	{
		if(!(prevStickState->hat & SDL_HAT_DOWN))
		{
			tempInputs->bHatDown.pressed = true;
			inputStateChange[player] = true;
		}
	}
	else
	{
		if(prevStickState->hat & SDL_HAT_DOWN)
		{
			tempInputs->bHatDown.released = true;
			inputStateChange[player] = true;
		}
	}

	if(value & SDL_HAT_LEFT)
	{
		if(!(prevStickState->hat & SDL_HAT_LEFT))
		{
			tempInputs->bHatLeft.pressed = true;
			inputStateChange[player] = true;
		}
	}
	else
	{
		if(prevStickState->hat & SDL_HAT_LEFT)
		{
			tempInputs->bHatLeft.released = true;
			inputStateChange[player] = true;
		}
	}

	if(value & SDL_HAT_RIGHT)
	{
		if(!(prevStickState->hat & SDL_HAT_RIGHT))
		{
			tempInputs->bHatRight.pressed = true;
			inputStateChange[player] = true;
		}
	}
	else
	{
		if(prevStickState->hat & SDL_HAT_RIGHT)
		{
			tempInputs->bHatRight.released = true;
			inputStateChange[player] = true;
		}
	}

	prevStickState->hat = value;
}

void Main::JoyButtonDown(Uint8 which,Uint8 button) 
{
	if(mainMenuState == INPUT_KEY || pauseMenuState == PAUSE_INPUT_KEY)
	{
		switch(keyToSetUp)
		{
		case 0: //up
			mappings[playerToSetUp].buttonUp.joystick = which; mappings[playerToSetUp].buttonUp.button = button;
			mappings[playerToSetUp].stick = -1;
			mappings[playerToSetUp].hat = -1;
			mappings[playerToSetUp].keyUp = SDLK_UNKNOWN;
			keyToSetUp = -1;
			menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveConfig();
			break;
		case 1: //down
			mappings[playerToSetUp].buttonDown.joystick = which; mappings[playerToSetUp].buttonDown.button = button;
			mappings[playerToSetUp].stick = -1;
			mappings[playerToSetUp].hat = -1;
			mappings[playerToSetUp].keyDown = SDLK_UNKNOWN;
			keyToSetUp = -1;
			menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveConfig();
			break;
		case 2: //left
			mappings[playerToSetUp].buttonLeft.joystick = which; mappings[playerToSetUp].buttonLeft.button = button;
			mappings[playerToSetUp].stick = -1;
			mappings[playerToSetUp].hat = -1;
			mappings[playerToSetUp].keyLeft = SDLK_UNKNOWN;
			keyToSetUp = -1;
			menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveConfig();
			break;
		case 3: //right
			mappings[playerToSetUp].buttonRight.joystick = which; mappings[playerToSetUp].buttonRight.button = button;
			mappings[playerToSetUp].stick = -1;
			mappings[playerToSetUp].hat = -1;
			mappings[playerToSetUp].keyRight = SDLK_UNKNOWN;
			keyToSetUp = -1;
			menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveConfig();
			break;
		case 4: //light attack
			mappings[playerToSetUp].buttonLight.joystick = which; mappings[playerToSetUp].buttonLight.button = button;
			mappings[playerToSetUp].keyLight = SDLK_UNKNOWN;
			keyToSetUp = -1;
			menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveConfig();
			break;
		case 5: //heavy attack
			mappings[playerToSetUp].buttonHeavy.joystick = which; mappings[playerToSetUp].buttonHeavy.button = button;
			mappings[playerToSetUp].keyHeavy = SDLK_UNKNOWN;
			keyToSetUp = -1;
			menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveConfig();
			break;
		case 6: //jump
			mappings[playerToSetUp].buttonJump.joystick = which; mappings[playerToSetUp].buttonJump.button = button;
			mappings[playerToSetUp].keyJump = SDLK_UNKNOWN;
			keyToSetUp = -1;
			menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveConfig();
			break;
		case 7: //block
			mappings[playerToSetUp].buttonBlock.joystick = which; mappings[playerToSetUp].buttonBlock.button = button;
			mappings[playerToSetUp].keyBlock = SDLK_UNKNOWN;
			keyToSetUp = -1;
			menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveConfig();
			break;
		case 8: //pause
			mappings[playerToSetUp].buttonStart.joystick = which; mappings[playerToSetUp].buttonStart.button = button;
			mappings[playerToSetUp].keyStart = SDLK_UNKNOWN;
			keyToSetUp = -1;
			menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveConfig();
			break;
		case 9: //menu confirm
			mappings[playerToSetUp].buttonMenuConfirm.joystick = which; mappings[playerToSetUp].buttonMenuConfirm.button = button;
			mappings[playerToSetUp].keyMenuConfirm = SDLK_UNKNOWN;
			keyToSetUp = -1;
			menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveConfig();
			break;
		case 10: //menu back
			mappings[playerToSetUp].buttonMenuBack.joystick = which; mappings[playerToSetUp].buttonMenuBack.button = button;
			mappings[playerToSetUp].keyMenuBack = SDLK_UNKNOWN;
			keyToSetUp = -1;
			menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveConfig();
			break;
		}

		return;
	}

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(mappings[i].buttonUp.joystick == which && mappings[i].buttonUp.button == button)
		{
			curInputs[i]->bButtonUp.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].buttonDown.joystick == which && mappings[i].buttonDown.button == button)
		{
			curInputs[i]->bButtonDown.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].buttonLeft.joystick == which && mappings[i].buttonLeft.button == button)
		{
			curInputs[i]->bButtonLeft.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].buttonRight.joystick == which && mappings[i].buttonRight.button == button)
		{
			curInputs[i]->bButtonRight.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].buttonJump.joystick == which && mappings[i].buttonJump.button == button)
		{
			curInputs[i]->bButtonJump.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].buttonBlock.joystick == which && mappings[i].buttonBlock.button == button)
		{
			curInputs[i]->bButtonBlock.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].buttonLight.joystick == which && mappings[i].buttonLight.button == button)
		{
			curInputs[i]->bButtonLight.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].buttonHeavy.joystick == which && mappings[i].buttonHeavy.button == button)
		{
			curInputs[i]->bButtonHeavy.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].buttonStart.joystick == which && mappings[i].buttonStart.button == button)
		{
			curInputs[i]->bButtonStart.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].buttonSelect.joystick == which && mappings[i].buttonSelect.button == button)
		{
			curInputs[i]->bButtonSelect.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].buttonMenuConfirm.joystick == which && mappings[i].buttonMenuConfirm.button == button)
		{
			curInputs[i]->buttonMenuConfirm.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].buttonMenuBack.joystick == which && mappings[i].buttonMenuBack.button == button)
		{
			curInputs[i]->buttonMenuBack.pressed = true;
			inputStateChange[i] = true;
		}
	}
}

void Main::JoyButtonUp(Uint8 which,Uint8 button) 
{
	if(keyToSetUp > 0)
	{
		return;
	}

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(mappings[i].buttonUp.joystick == which && mappings[i].buttonUp.button == button)
		{
			curInputs[i]->bButtonUp.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].buttonDown.joystick == which && mappings[i].buttonDown.button == button)
		{
			curInputs[i]->bButtonDown.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].buttonLeft.joystick == which && mappings[i].buttonLeft.button == button)
		{
			curInputs[i]->bButtonLeft.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].buttonRight.joystick == which && mappings[i].buttonRight.button == button)
		{
			curInputs[i]->bButtonRight.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].buttonJump.joystick == which && mappings[i].buttonJump.button == button)
		{
			curInputs[i]->bButtonJump.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].buttonBlock.joystick == which && mappings[i].buttonBlock.button == button)
		{
			curInputs[i]->bButtonBlock.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].buttonLight.joystick == which && mappings[i].buttonLight.button == button)
		{
			curInputs[i]->bButtonLight.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].buttonHeavy.joystick == which && mappings[i].buttonHeavy.button == button)
		{
			curInputs[i]->bButtonHeavy.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].buttonStart.joystick == which && mappings[i].buttonStart.button == button)
		{
			curInputs[i]->bButtonStart.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].buttonSelect.joystick == which && mappings[i].buttonSelect.button == button)
		{
			curInputs[i]->bButtonSelect.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].buttonSelect.joystick == which && mappings[i].buttonMenuConfirm.button == button)
		{
			curInputs[i]->buttonMenuConfirm.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].buttonSelect.joystick == which && mappings[i].buttonMenuBack.button == button)
		{
			curInputs[i]->buttonMenuBack.released = true;
			inputStateChange[i] = true;
		}
	}
}

void Main::JoyBall(Uint8 which,Uint8 ball,Sint16 xrel,Sint16 yrel) 
{
    
}

void Main::Minimize() 
{
    
}

void Main::Restore() 
{
    
}

void Main::Resize(int w,int h) 
{
    
}

void Main::Expose() 
{
    
}

void Main::Exit()
{
    notDone = false;
}

int Main::ToggleFullScreen()
{
	if(fullScreen) { return SetFullScreen(false); }
	else { return SetFullScreen(true); }
}

int Main::SetFullScreen(bool newFullScreen)
{
	fullScreen = newFullScreen;

	if(textureRegistry.size() > 0)
	{
		list<HSTexture*>::iterator trIt;
		for ( trIt=textureRegistry.begin(); trIt != textureRegistry.end(); trIt++)
		{
			glDeleteTextures(1, &(*trIt)->textureID);
			glDeleteBuffers(1, &(*trIt)->bufferID);

			(*trIt)->textureID = 0;
			(*trIt)->bufferID = 0;

			if((*trIt)->ownPalette != NULL)
			{
				glDeleteTextures(1, &(*trIt)->ownPalette->textureID);
				(*trIt)->ownPalette->textureID = 0;
			}
		}
	}

	if(paletteRegistry.size() > 0)
	{
		list<HSPalette*>::iterator palIt;
		for (palIt=paletteRegistry.begin(); palIt != paletteRegistry.end(); palIt++)
		{
			glDeleteTextures(1, &(*palIt)->textureID);

			(*palIt)->textureID = 0;
		}
	}

	if(shader_vert != 0)
	{
		glDeleteShader(shader_vert);
		shader_vert = 0;
	}
	if(shader_fragNonIndexed != 0)
	{
		glDeleteShader(shader_fragNonIndexed);
		shader_fragNonIndexed = 0;
	}
	if(shader_fragIndexed != 0)
	{
		glDeleteShader(shader_fragIndexed);
		shader_fragIndexed = 0;
	}
	if(shader_progNonIndexed != 0)
	{
		glDeleteProgram(shader_progNonIndexed);
		shader_progNonIndexed = 0;
	}
	if(shader_progIndexed != 0)
	{
		glDeleteProgram(shader_progIndexed);
		shader_progIndexed = 0;
	}

	if(texCoordBufferID != 0)
	{
		glDeleteBuffers(1, &texCoordBufferID);
		texCoordBufferID = 0;
	}

	if(elementArrayBufferID != 0)
	{
		glDeleteBuffers(1, &elementArrayBufferID);
		elementArrayBufferID = 0;
	}

	if(int error = SetBestGameResolution() != 0)
	{
		return error;
	}

	UpdateLog("Resolution set.", false);

	int options = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL;

	if(!fullScreen)
	{
		screenResolutionX = gameResolutionX;
		screenResolutionY = gameResolutionY;
	}
	else
	{
		options = options | SDL_FULLSCREEN;
	}

	if((surf_display = SDL_SetVideoMode(screenResolutionX, screenResolutionY, 32, options)) == NULL)
	{
		UpdateLog("Error setting SDL video mode.", true);
		return -1;
	}

	UpdateLog("SDL video mode set.", false);

	glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
	glClearDepth(1.0f);

	glViewport((screenResolutionX - gameResolutionX) / 2, (screenResolutionY - gameResolutionY) / 2, gameResolutionX, gameResolutionY);

	resolutionScale = (float)gameResolutionX / (float)MAX_GAME_RESOLUTION_X;
 
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
 
	//glOrtho(0, MAX_GAME_RESOLUTION_X, MAX_GAME_RESOLUTION_Y, 0, 1, -1);
 
	//glMatrixMode(GL_MODELVIEW);
 
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);
 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	SDL_GL_SwapBuffers();

	//get the current opengl version
	int glMajorVersion = 0;
	int glMinorVersion = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &glMajorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &glMinorVersion);

	//gl major/minor version doesn't work, so use an older method
	if(glMajorVersion == 0)
	{
		string version; version.assign((const char *)glGetString(GL_VERSION));

		if(version.length() >= 1)
		{
			string major = version.substr(0, 1);
			glMajorVersion = atoi(major.c_str());
		}

		if(version.length() >= 3)
		{
			string minor = version.substr(2, 1);
			glMinorVersion = atoi(minor.c_str());
		}
	}

	//we need to know this for texture loading and storage
	if(glMajorVersion >= 3)
	{
		openGL3 = true;
	}
	else
	{
		openGL3 = false;
	}

	stringstream sstm;
	sstm << "This machine is running OpenGL version " << glMajorVersion << "." << glMinorVersion << ".";
	UpdateLog(sstm.str(), false);

	if(glMajorVersion < 2)
	{
		UpdateLog("OpenGL version is older than 2.0.", true);
		return -1;
	}

	setupExtensions();
	if(shading_enabled == false)
	{
		UpdateLog("OpenGL shaders are not enabled.", true);
		return -1;
	}
	if(buffer_objects_enabled == false)
	{
		UpdateLog("OpenGL buffer objects are not enabled.", true);
		return -1;
	}
	if(default_array_objects_enabled == false)
	{
		UpdateLog("OpenGL default array object is not enabled.", true);
		return -1;
	}
	if(custom_array_objects_enabled == false && openGL3)
	{
		UpdateLog("OpenGL custom array objects are not enabled.", true);
		return -1;
	}

	UpdateLog("OpenGL shaders, buffer objects, and vertex array objects enabled.", false);

	//set up all the shader stuff
	shader_progNonIndexed = glCreateProgram();
	shader_progIndexed = glCreateProgram();
	shader_vert = glCreateShader(GL_VERTEX_SHADER);
	shader_fragNonIndexed = glCreateShader(GL_FRAGMENT_SHADER);
	shader_fragIndexed = glCreateShader(GL_FRAGMENT_SHADER);

	string vertShaderFileName;
	string fragNonIndexedShaderFileName;
	string fragIndexedShaderFileName;

	if(openGL3)
	{
		vertShaderFileName = "vertexShader3.glsl.txt";
		fragNonIndexedShaderFileName = "fragmentShaderNonIndexed3.glsl.txt";
		fragIndexedShaderFileName = "fragmentShaderIndexed3.glsl.txt";
	}
	else
	{
		vertShaderFileName = "vertexShader2.glsl.txt";
		fragNonIndexedShaderFileName = "fragmentShaderNonIndexed2.glsl.txt";
		fragIndexedShaderFileName = "fragmentShaderIndexed2.glsl.txt";
	}

	string shader_vert_source_string = loadSource(vertShaderFileName);

	if(shader_vert_source_string.compare("READ ERROR") == 0)
	{
		UpdateLog("Could not read vertex shader source file: " + vertShaderFileName, true);
		return -1;
	}

	string shader_fragNonIndexed_source_string = loadSource(fragNonIndexedShaderFileName);

	if(shader_fragNonIndexed_source_string.compare("READ ERROR") == 0)
	{
		UpdateLog("Could not read non-indexed fragment shader source file: " + fragNonIndexedShaderFileName, true);
		return -1;
	}

	string shader_fragIndexed_source_string = loadSource(fragIndexedShaderFileName);

	if(shader_fragIndexed_source_string.compare("READ ERROR") == 0)
	{
		UpdateLog("Could not read indexed fragment shader source file: " + fragIndexedShaderFileName, true);
		return -1;
	}

	//add some newlines so it compiles correctly
	unsigned int verPos = shader_vert_source_string.find("#version ", 0);
	if(verPos != string::npos) { shader_vert_source_string.insert(verPos + 12, "\n"); }

	verPos = shader_fragNonIndexed_source_string.find("#version ", 0);
	if(verPos != string::npos) { shader_fragNonIndexed_source_string.insert(verPos + 12, "\n"); }

	verPos = shader_fragIndexed_source_string.find("#version ", 0);
	if(verPos != string::npos) { shader_fragIndexed_source_string.insert(verPos + 12, "\n"); }

	//add more newlines after semicolons so warning/error positions are meaningful
	unsigned int scPos = shader_vert_source_string.find(";", 0);
	while(scPos != string::npos)
	{
		shader_vert_source_string.insert(scPos+1, "\n");
		scPos = shader_vert_source_string.find(";", scPos+1);
	}
	
	scPos = shader_fragNonIndexed_source_string.find(";", 0);
	while(scPos != string::npos)
	{
		shader_fragNonIndexed_source_string.insert(scPos+1, "\n");
		scPos = shader_fragNonIndexed_source_string.find(";", scPos+1);
	}
	
	scPos = shader_fragIndexed_source_string.find(";", 0);
	while(scPos != string::npos)
	{
		shader_fragIndexed_source_string.insert(scPos+1, "\n");
		scPos = shader_fragIndexed_source_string.find(";", scPos+1);
	}

	const char * shader_vert_source = shader_vert_source_string.data();
	const char * shader_fragNonIndexed_source = shader_fragNonIndexed_source_string.data();
	const char * shader_fragIndexed_source = shader_fragIndexed_source_string.data();

	int len = 0;
	len = shader_vert_source_string.length();
	glShaderSource(shader_vert, 1, (const GLcharARB**)&shader_vert_source, &len);
	len = shader_fragNonIndexed_source_string.length();
	glShaderSource(shader_fragNonIndexed, 1, (const GLcharARB**)&shader_fragNonIndexed_source, &len);
	len = shader_fragIndexed_source_string.length();
	glShaderSource(shader_fragIndexed, 1, (const GLcharARB**)&shader_fragIndexed_source, &len);

	//compile vertex shader
	string shaderError = "";
	GLint shaderStatus = GL_FALSE;
	shaderError = compileShader(shader_vert);
	glGetShaderiv(shader_vert, GL_COMPILE_STATUS, &shaderStatus);
	if(shaderStatus == GL_TRUE)
	{
		if(shaderError.empty())
		{
			UpdateLog("Vertex shader compiled successfully.", false);
		}
		else
		{
			UpdateLog("Vertex shader compiled successfully with message: " + shaderError, false);
		}
	}
	else if(shaderStatus == GL_FALSE)
	{
		if(shaderError.empty())
		{
			UpdateLog("Graphics device does not support GLSL shaders.", true);
		}
		else
		{
			UpdateLog("Error compiling vertex shader: " + shaderError, true);
		}
		return -1;
	}
	
	//compile non-indexed fragment shader
	shaderError = "";
	shaderStatus = GL_FALSE;
	shaderError = compileShader(shader_fragNonIndexed);
	glGetShaderiv(shader_fragNonIndexed, GL_COMPILE_STATUS, &shaderStatus);
	if(shaderStatus == GL_TRUE)
	{
		if(shaderError.empty())
		{
			UpdateLog("Non-Indexed fragment shader compiled successfully.", false);
		}
		else
		{
			UpdateLog("Non-Indexed fragment shader compiled successfully with message: " + shaderError, false);
		}
	}
	else if(shaderStatus == GL_FALSE)
	{
		if(shaderError.empty())
		{
			UpdateLog("Graphics device does not support GLSL shaders.", true);
		}
		else
		{
			UpdateLog("Error compiling non-indexed fragment shader: " + shaderError, true);
		}
		return -1;
	}
	
	//compile indexed fragment shader
	shaderError = "";
	shaderStatus = GL_FALSE;
	shaderError = compileShader(shader_fragIndexed);
	glGetShaderiv(shader_fragIndexed, GL_COMPILE_STATUS, &shaderStatus);
	if(shaderStatus == GL_TRUE)
	{
		if(shaderError.empty())
		{
			UpdateLog("Indexed fragment shader compiled successfully.", false);
		}
		else
		{
			UpdateLog("Indexed fragment shader compiled successfully with message: " + shaderError, false);
		}
	}
	else if(shaderStatus == GL_FALSE)
	{
		if(shaderError.empty())
		{
			UpdateLog("Graphics device does not support GLSL shaders.", true);
		}
		else
		{
			UpdateLog("Error compiling indexed fragment shader: " + shaderError, true);
		}
		return -1;
	}

	//link non-indexed program
	glAttachShader(shader_progNonIndexed, shader_vert);
	glAttachShader(shader_progNonIndexed, shader_fragNonIndexed);
	
	shaderError = "";
	shaderStatus = GL_FALSE;
	shaderError = linkProgram(shader_progNonIndexed);
	glGetProgramiv(shader_progNonIndexed, GL_LINK_STATUS, &shaderStatus);
	if(shaderStatus == GL_TRUE)
	{
		if(shaderError.empty())
		{
			UpdateLog("Non-Indexed shaders linked successfully.", false);
		}
		else
		{
			UpdateLog("Non-Indexed shaders linked successfully with message: " + shaderError, false);
		}
	}
	else if(shaderStatus == GL_FALSE)
	{
		if(shaderError.empty())
		{
			UpdateLog("Graphics device does not support GLSL shaders.", true);
		}
		else
		{
			UpdateLog("Error linking non-indexed shaders: " + shaderError, true);
		}
		return -1;
	}

	//link indexed program
	glAttachShader(shader_progIndexed, shader_vert);
	glAttachShader(shader_progIndexed, shader_fragIndexed);
	
	shaderError = "";
	shaderStatus = GL_FALSE;
	shaderError = linkProgram(shader_progIndexed);
	glGetProgramiv(shader_progIndexed, GL_LINK_STATUS, &shaderStatus);
	if(shaderStatus == GL_TRUE)
	{
		if(shaderError.empty())
		{
			UpdateLog("Indexed shaders linked successfully.", false);
		}
		else
		{
			UpdateLog("Indexed shaders linked successfully with message: " + shaderError, false);
		}
	}
	else if(shaderStatus == GL_FALSE)
	{
		if(shaderError.empty())
		{
			UpdateLog("Graphics device does not support GLSL shaders.", true);
		}
		else
		{
			UpdateLog("Error linking indexed shaders: " + shaderError, true);
		}
		return -1;
	}

	positionLocNonIndexed = glGetAttribLocation(shader_progNonIndexed, "position");
	positionLocIndexed = glGetAttribLocation(shader_progIndexed, "position");
	texCoordInLocNonIndexed = glGetAttribLocation(shader_progNonIndexed, "texCoordIn");
	texCoordInLocIndexed = glGetAttribLocation(shader_progIndexed, "texCoordIn");

	nonIndexedPosOffsetLoc = glGetUniformLocation(shader_progNonIndexed, "posOffset");
	indexedPosOffsetLoc = glGetUniformLocation(shader_progIndexed, "posOffset");
	nonIndexedScaleLoc = glGetUniformLocation(shader_progNonIndexed, "scale");
	indexedScaleLoc = glGetUniformLocation(shader_progIndexed, "scale");
	nonIndexedResolutionLoc = glGetUniformLocation(shader_progNonIndexed, "resolution");
	indexedResolutionLoc = glGetUniformLocation(shader_progIndexed, "resolution");
	nonIndexedResScaleLoc = glGetUniformLocation(shader_progNonIndexed, "resScale");
	indexedResScaleLoc = glGetUniformLocation(shader_progIndexed, "resScale");
	nonIndexedFocusPosLoc = glGetUniformLocation(shader_progNonIndexed, "focusPos");
	indexedFocusPosLoc = glGetUniformLocation(shader_progIndexed, "focusPos");
	nonIndexedZoomOutLoc = glGetUniformLocation(shader_progNonIndexed, "zoomOut");
	indexedZoomOutLoc = glGetUniformLocation(shader_progIndexed, "zoomOut");
	nonIndexedTexLoc = glGetUniformLocation(shader_progNonIndexed, "tex");
	indexedTexLoc = glGetUniformLocation(shader_progIndexed, "tex");
	paletteLoc = glGetUniformLocation(shader_progIndexed, "palette");

	glUseProgram(shader_progNonIndexed);
	glUniform2f(nonIndexedResolutionLoc, (float)gameResolutionX, (float)gameResolutionY);
	glUniform1f(nonIndexedResScaleLoc, resolutionScale);
	
	glUseProgram(shader_progIndexed);
	glUniform2f(indexedResolutionLoc, (float)gameResolutionX, (float)gameResolutionY);
	glUniform1f(indexedResScaleLoc, resolutionScale);

	glUseProgram(0);

	//set up some VBO stuff
	if(texCoordBufferID == 0)
	{
		GLfloat * texCoord = new GLfloat[8];
		texCoord[0] = 0.0f;	texCoord[1] = 1.0f;
		texCoord[2] = 1.0f;	texCoord[3] = 1.0f;
		texCoord[4] = 1.0f;	texCoord[5] = 0.0f;
		texCoord[6] = 0.0f;	texCoord[7] = 0.0f;

		glGenBuffers(1, &texCoordBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferID);
		glBufferData(GL_ARRAY_BUFFER, 8*sizeof(GLfloat), texCoord, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		delete[] texCoord;
	}

	if(elementArrayBufferID == 0)
	{
		GLubyte * eArray = new GLubyte[6];
		eArray[0] = 0; eArray[1] = 1; eArray[2] = 3;
		eArray[3] = 1; eArray[4] = 2; eArray[5] = 3;

		glGenBuffers(1, &elementArrayBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, elementArrayBufferID);
		glBufferData(GL_ARRAY_BUFFER, 6*sizeof(GLubyte), eArray, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		delete[] eArray;
	}

	if(!openGL3)
	{
		//bind universal things to the default vertex array object
		glEnableVertexAttribArray(positionLocNonIndexed);
		if(positionLocNonIndexed != positionLocIndexed)
		{
			glEnableVertexAttribArray(positionLocIndexed);
		}
		glEnableVertexAttribArray(texCoordInLocNonIndexed);
		if(texCoordInLocNonIndexed != texCoordInLocIndexed)
		{
			glEnableVertexAttribArray(texCoordInLocIndexed);
		}

		glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferID);
		glVertexAttribPointer(texCoordInLocNonIndexed, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		if(texCoordInLocNonIndexed != texCoordInLocIndexed)
		{
			glVertexAttribPointer(texCoordInLocIndexed, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBufferID);
	}

	if(textureRegistry.size() > 0)
	{
		//need to reload all textures
		list<HSTexture*>::iterator trIt;
		for ( trIt=textureRegistry.begin(); trIt != textureRegistry.end(); trIt++)
		{
			if(int error = LoadTGAToTexture((*trIt), openGL3, (*trIt)->ownPalette != NULL) != 0)
			{
				return error;
			}
		}
	}

	if(paletteRegistry.size() > 0)
	{
		//need to reload all palettes
		list<HSPalette*>::iterator palIt;
		for ( palIt=paletteRegistry.begin(); palIt != paletteRegistry.end(); palIt++)
		{
			if(int error = LoadHSPToPalette((*palIt)) != 0)
			{
				return error;
			}
		}
	}

	GLenum glError = glGetError();
	if(glError != GL_NO_ERROR)
	{
		string glErrorString = "OpenGL error in SetFullScreen().";
		UpdateLog(glErrorString, true);
	}

	return 0;
}

void Main::User(Uint8 type, int code, void* data1, void* data2) 
{
    string blah = "";
}

//main
int main( int argc, char* argv[] )
{
	HSLog = "";

	Main * theGame = new Main();
	int error = theGame->Execute();
	
	//output the error code, if there was one
	string errorText = "Application exited with no errors.\n\n";
	if(error != 0)
	{
		errorText = "An error occured while running the application. See log for details.\n\n";
	}

	errorText = errorText + "Log:\n" + HSLog;

	ofstream file;
	file.open("log.txt");
	file << errorText;
	file.close();

	return error;
}