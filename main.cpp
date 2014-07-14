#include "main.h"

GLenum shader_vert;
GLenum shader_fragNonIndexed;
GLenum shader_fragIndexed;
GLenum shader_progNonIndexed;
GLenum shader_progIndexed;

Main::Main()
{
	objectManager = new ObjectManager();
	renderingManager = new RenderingManager(objectManager);
	
	gameState = MAIN_MENU;
	gameType = GAME_TYPE_FREE_FOR_ALL;
	bindingKey = BINDING_NONE;

	lastFrameTicks = 0;
	frame = 0;

	needToPause = false;
	matchPromptTimer = 0;

	playerToSetUp = -1;

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

	UpdateLog("Starting main game loop.", false);

	while(objectManager->notDone)
	{
		if(objectManager->renderingErrorCode != 0) { return objectManager->renderingErrorCode; }

		int thisFrameTicks = SDL_GetTicks();
		if(thisFrameTicks - lastFrameTicks >= 1000 / TARGET_FPS)
		{
			lastFrameTicks = thisFrameTicks;

			if((error = AdvanceHolds()) != 0) { return error; }
			if((error = Event()) != 0) { return error; }
			if((error = Update()) != 0) { return error; }
			if((error = Collide()) != 0) { return error; }
			if((error = SpawnObjects()) != 0) { return error; }
			if((error = PlayAudio()) != 0) { return error; }
			if((error = DeleteObjects()) != 0) { return error; }
			//SDL_mutexV(objectManager->processLock);
			//SDL_CondSignal(objectManager->processDone);
			objectManager->doRender = true;
			if((error = renderingManager->Execute()) != 0) { return error; }
			//SDL_CondWait( objectManager->renderDone, objectManager->renderLock );

#if _DEBUG
			//DebugOutput();
#endif

			frame++;
		}

	}

	UpdateLog("Main game loop exited.", false);

	Cleanup();

	UpdateLog("Cleanup complete.", false);

	return 0;
}

void Main::DebugOutput()
{
#ifdef _WIN32 //This is only able to be done on Windows
	CONSOLE_SCREEN_BUFFER_INFO Info;
	GetConsoleScreenBufferInfo(GetStdHandle (STD_OUTPUT_HANDLE), &Info) ;
	SHORT Width = Info.srWindow.Right - Info.srWindow.Left + 1 ;
	for ( SHORT N = Info.srWindow.Top ; N <= Info.srWindow.Bottom ; ++N ) {
		DWORD Chars ;
		COORD Pos = { Info.srWindow.Left, N } ;
		FillConsoleOutputCharacter(GetStdHandle (STD_OUTPUT_HANDLE), ' ', Width, Pos, &Chars) ;
		FillConsoleOutputAttribute(GetStdHandle (STD_OUTPUT_HANDLE), ' ', Width, Pos, &Chars) ;
	}
	COORD TopLeft = { Info.srWindow.Left, Info.srWindow.Top } ;
	SetConsoleCursorPosition(GetStdHandle (STD_OUTPUT_HANDLE), TopLeft) ;

	cout << "====DEBUG INFO====" << endl;
	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(objectManager->players[i] == NULL) { continue; }

		cout << "---Player " << i << "---" << endl;

		if(objectManager->players[i]->IsFighter())
		{
			Fighter * f = (Fighter*)objectManager->players[i];

			cout << "Keyboard Left Pressed: " << (inputHistory[i]->bKeyLeft.pressed && inputHistory[i]->frame == frame) << endl;
			cout << "Keyboard Left Held: " << inputHistory[i]->bKeyLeft.held << endl;
			cout << "Keyboard Left Released: " << (inputHistory[i]->bKeyLeft.released && inputHistory[i]->frame == frame) << endl;
			cout << "Keyboard Right Pressed: " << (inputHistory[i]->bKeyRight.pressed && inputHistory[i]->frame == frame) << endl;
			cout << "Keyboard Right Held: " << inputHistory[i]->bKeyRight.held << endl;
			cout << "Keyboard Right Released: " << (inputHistory[i]->bKeyRight.released && inputHistory[i]->frame == frame) << endl;
		}

		cout << endl;
	}
	
	cout << endl;
#endif
}

void AudioCallback(void *unused, Uint8 *stream, int len)
{
	SDL_memset(stream, 0, len);

	if(currentAudio.empty()) { return; }

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
		delete (*caIt);
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
 
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  0);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  0);

	SDL_ShowCursor(SDL_DISABLE);

	UpdateLog("SDL attributes set.", false);

	//load configuration
	if(int error = LoadKeyConfig() != 0)
	{
		return error;
	}

	UpdateLog("Key configuration loaded.", false);

	//objectManager->vidInitLock = SDL_CreateMutex();
	//objectManager->vidInitDone = SDL_CreateCond();

	//objectManager->renderLock = SDL_CreateMutex();
	//objectManager->renderDone = SDL_CreateCond();

	//objectManager->processLock = SDL_CreateMutex();
	//objectManager->processDone = SDL_CreateCond();

	//SDL_mutexP(objectManager->vidInitLock);
	//SDL_mutexP(objectManager->renderLock);
	//SDL_mutexP(objectManager->processLock);
	
	//objectManager->renderingThread = SDL_CreateThread(RenderThreadControl, "render", objectManager);
	
	//SDL_CondWait( objectManager->vidInitDone, objectManager->vidInitLock );

	renderingManager->Initialize();

	//set up SDL screen
	//if(int error = InitializeGraphics() != 0)
	//{
	//	return error;
	//}

	////set up audio
	SDL_AudioSpec *desired; 

	/* Allocated audio specs */ 
	desired=(SDL_AudioSpec *)malloc(sizeof(SDL_AudioSpec)); 
	objectManager->obtainedAudioSpec=(SDL_AudioSpec *)malloc(sizeof(SDL_AudioSpec)); 

	/* Set desired format */ 
	desired->freq=22050; 
	desired->format=AUDIO_S16; 
	desired->channels=1;
	desired->samples=1024; 
	desired->callback=AudioCallback; 
	desired->userdata=NULL; 

	/* Open the audio device */ 
	int blah = SDL_OpenAudio(desired, objectManager->obtainedAudioSpec);
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
	else if(gameState == MATCH)
	{
		if(int i = UpdateMatch() != 0) { return i; }
	}

	list<HSObject*>::iterator objIt;
	for ( objIt=objectManager->HUDObjects.begin(); objIt != objectManager->HUDObjects.end(); objIt++)
	{
		if(int error = (*objIt)->Update() != 0)
		{
			return error; //there was an error updating the object
		}
	}

	if(gameState == MATCH && matchState == PAUSED) { return 0; }

	for ( objIt=objectManager->stageObjects.begin(); objIt != objectManager->stageObjects.end(); objIt++)
	{
		if(int error = (*objIt)->Update() != 0)
		{
			return error; //there was an error updating the object
		}
	}

	for ( objIt=objectManager->BGSpawnedObjects.begin(); objIt != objectManager->BGSpawnedObjects.end(); objIt++)
	{
		if(int error = (*objIt)->Update() != 0)
		{
			return error; //there was an error updating the object
		}
	}

	for ( objIt=objectManager->fighterObjects.begin(); objIt != objectManager->fighterObjects.end(); objIt++)
	{
		if(int error = (*objIt)->Update() != 0)
		{
			return error; //there was an error updating the object
		}
	}

	for ( objIt=objectManager->FGSpawnedObjects.begin(); objIt != objectManager->FGSpawnedObjects.end(); objIt++)
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
	if(gameState != MATCH || matchState == PAUSED) { return 0; }

	//first handle terrain collisions
	list<HSObject*>::iterator objIt = objectManager->BGSpawnedObjects.begin();
	for ( objIt=objectManager->BGSpawnedObjects.begin(); objIt != objectManager->BGSpawnedObjects.end(); objIt++)
	{
		if(int error = (*objIt)->CollideTerrain(&objectManager->stageObjects) != 0) { return error; }
	}

	for ( objIt=objectManager->fighterObjects.begin(); objIt != objectManager->fighterObjects.end(); objIt++)
	{
		if(int error = (*objIt)->CollideTerrain(&objectManager->stageObjects) != 0) { return error; }
	}

	for ( objIt=objectManager->FGSpawnedObjects.begin(); objIt != objectManager->FGSpawnedObjects.end(); objIt++)
	{
		if(int error = (*objIt)->CollideTerrain(&objectManager->stageObjects) != 0) { return error; }
	}

	//next, handle collisions between fighter terrain boxes
	for (objIt=objectManager->fighterObjects.begin(); objIt != objectManager->fighterObjects.end(); objIt++)
	{
		if((*objIt)->IsFighter())
		{
			if(int error = ((Fighter*)*objIt)->CollideFighters(&objectManager->fighterObjects) != 0) { return error; }
		}
	}
	
	//now, handle all attack/hurt box collision
	for ( objIt=objectManager->stageObjects.begin(); objIt != objectManager->stageObjects.end(); objIt++)
	{
		if(int error = (*objIt)->CollideAttack(&objectManager->stageObjects) != 0) { return error; }
		if(int error = (*objIt)->CollideAttack(&objectManager->BGSpawnedObjects) != 0) { return error; }
		if(int error = (*objIt)->CollideAttack(&objectManager->fighterObjects) != 0) { return error; }
		if(int error = (*objIt)->CollideAttack(&objectManager->FGSpawnedObjects) != 0) { return error; }
	}
	for ( objIt=objectManager->BGSpawnedObjects.begin(); objIt != objectManager->BGSpawnedObjects.end(); objIt++)
	{
		if(int error = (*objIt)->CollideAttack(&objectManager->stageObjects) != 0) { return error; }
		if(int error = (*objIt)->CollideAttack(&objectManager->BGSpawnedObjects) != 0) { return error; }
		if(int error = (*objIt)->CollideAttack(&objectManager->fighterObjects) != 0) { return error; }
		if(int error = (*objIt)->CollideAttack(&objectManager->FGSpawnedObjects) != 0) { return error; }
	}
	for ( objIt=objectManager->fighterObjects.begin(); objIt != objectManager->fighterObjects.end(); objIt++)
	{
		if(int error = (*objIt)->CollideAttack(&objectManager->stageObjects) != 0) { return error; }
		if(int error = (*objIt)->CollideAttack(&objectManager->BGSpawnedObjects) != 0) { return error; }
		if(int error = (*objIt)->CollideAttack(&objectManager->fighterObjects) != 0) { return error; }
		if(int error = (*objIt)->CollideAttack(&objectManager->FGSpawnedObjects) != 0) { return error; }
	}
	for ( objIt=objectManager->FGSpawnedObjects.begin(); objIt != objectManager->FGSpawnedObjects.end(); objIt++)
	{
		if(int error = (*objIt)->CollideAttack(&objectManager->stageObjects) != 0) { return error; }
		if(int error = (*objIt)->CollideAttack(&objectManager->BGSpawnedObjects) != 0) { return error; }
		if(int error = (*objIt)->CollideAttack(&objectManager->fighterObjects) != 0) { return error; }
		if(int error = (*objIt)->CollideAttack(&objectManager->FGSpawnedObjects) != 0) { return error; }
	}

	//finally, apply all the results of the attack collision phase
	for ( objIt=objectManager->stageObjects.begin(); objIt != objectManager->stageObjects.end(); objIt++) { (*objIt)->ApplyAttackResults(); }
	for ( objIt=objectManager->BGSpawnedObjects.begin(); objIt != objectManager->BGSpawnedObjects.end(); objIt++) { (*objIt)->ApplyAttackResults(); }
	list<HSObject*> fightersToBack;
	for ( objIt=objectManager->fighterObjects.begin(); objIt != objectManager->fighterObjects.end(); objIt++)
	{
		//save objectst that hit something this frame
		if((*objIt)->IsTerrainObject() && ((TerrainObject*)(*objIt))->attackResults.didStrike)
		{
			fightersToBack.push_back((*objIt));
		}
		(*objIt)->ApplyAttackResults();
	}
	//move objects that hit something this frame to the back of the list
	for ( objIt=fightersToBack.begin(); objIt != fightersToBack.end(); objIt++)
	{
		objectManager->fighterObjects.remove((*objIt));
		objectManager->fighterObjects.push_back((*objIt));
	}
	for ( objIt=objectManager->FGSpawnedObjects.begin(); objIt != objectManager->FGSpawnedObjects.end(); objIt++) { (*objIt)->ApplyAttackResults(); }

	if(int error = CollideMatch()!= 0) { return error; }

	return 0;
}

int Main::SpawnObjects()
{
	if(objectManager->menuManager != NULL)
	{
		if(int error = SpawnMenus(objectManager->menuManager->GetRoot()) != 0) { return error; }
	}

	if(objectManager->characterSelectManager != NULL)
	{
		if(int error = SpawnCharacterSelect(objectManager->characterSelectManager) != 0) { return error; }
	}

	if(objectManager->hudManager != NULL)
	{
		if(int error = SpawnHUD(objectManager->hudManager) != 0) { return error; }
	}

	objectManager->SortAllObjects();

	if(gameState == MATCH && matchState == PAUSED) { return 0; }

	list<HSObject*>::iterator objIt;
	for ( objIt=objectManager->stageObjects.begin(); objIt != objectManager->stageObjects.end(); objIt++)
	{
		list<SpawnObject*> spawnObjects = (*objIt)->GetSpawnObjects();

		list<SpawnObject*>::iterator spawnIt;
		for(spawnIt = spawnObjects.begin(); spawnIt != spawnObjects.end(); spawnIt++)
		{
			if(int error = objectManager->CloneObject((*spawnIt), &objectManager->FGSpawnedObjects) != 0) { return error; }
		}
	}
	for ( objIt=objectManager->BGSpawnedObjects.begin(); objIt != objectManager->BGSpawnedObjects.end(); objIt++)
	{
		list<SpawnObject*> spawnObjects = (*objIt)->GetSpawnObjects();

		list<SpawnObject*>::iterator spawnIt;
		for(spawnIt = spawnObjects.begin(); spawnIt != spawnObjects.end(); spawnIt++)
		{
			if(int error = objectManager->CloneObject((*spawnIt), &objectManager->FGSpawnedObjects) != 0) { return error; }
		}
	}
	for ( objIt=objectManager->fighterObjects.begin(); objIt != objectManager->fighterObjects.end(); objIt++)
	{
		list<SpawnObject*> spawnObjects = (*objIt)->GetSpawnObjects();

		list<SpawnObject*>::iterator spawnIt;
		for(spawnIt = spawnObjects.begin(); spawnIt != spawnObjects.end(); spawnIt++)
		{
			if(int error = objectManager->CloneObject((*spawnIt), &objectManager->FGSpawnedObjects) != 0) { return error; }
		}
	}
	for ( objIt=objectManager->FGSpawnedObjects.begin(); objIt != objectManager->FGSpawnedObjects.end(); objIt++)
	{
		list<SpawnObject*> spawnObjects = (*objIt)->GetSpawnObjects();

		list<SpawnObject*>::iterator spawnIt;
		for(spawnIt = spawnObjects.begin(); spawnIt != spawnObjects.end(); spawnIt++)
		{
			if(int error = objectManager->CloneObject((*spawnIt), &objectManager->FGSpawnedObjects) != 0) { return error; }
		}
	}

	return 0;
}

int Main::SpawnMenus(HSMenu * menu)
{
	if(menu == NULL) { return 0; }

	if(int error = SpawnText(menu) != 0) { return error; }

	if(menu->needToSpawnCursor)
	{
		if(int error = objectManager->CloneObject(menu->cursorToClone, &objectManager->HUDObjects, &menu->activeCursor) != 0) { return error; }
		menu->activeCursor->pos.x = menu->cursorToClone->pos.x;
		menu->activeCursor->pos.y = menu->cursorToClone->pos.y;
		menu->needToSpawnCursor = false;
	}

	if(!menu->itemNeedToSpawnText) { return 0; }

	list<HSMenuItem*>::iterator miIt;
	for(miIt = menu->items.begin(); miIt != menu->items.end(); miIt++)
	{
		if(int error = SpawnText((*miIt)) != 0) { return error; }

		if((*miIt)->childNeedToSpawnText)
		{
			if(int error = SpawnMenus((*miIt)->child) != 0) { return error; }
			(*miIt)->childNeedToSpawnText = false;
		}

		if((*miIt)->chooserOrKeySettingNeedToSpawnText)
		{
			if(int error = SpawnText((*miIt)->chooser) != 0) { return error; }
			if(int error = SpawnText((*miIt)->currentKeySetting) != 0) { return error; }
			(*miIt)->chooserOrKeySettingNeedToSpawnText = false;
		}
	}

	menu->itemNeedToSpawnText = false;

	return 0;
}

int Main::SpawnCharacterSelect(CharacterSelectManager * manager)
{
	if(manager == NULL) { return 0; }

	if(int error = SpawnText(manager->characterSelect->title) != 0) { return error; }

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(int error = SpawnText(manager->characterSelect->characterName[i]) != 0) { return error; }
		if(int error = SpawnText(manager->characterSelect->portraitInstructions[i]) != 0) { return error; }
	}

	if(int error = SpawnText(manager->stageChooser->title) != 0) { return error; }
	if(int error = SpawnText(manager->stageChooser->stageName) != 0) { return error; }
	
	if(int error = SpawnText(manager->musicChooser->title) != 0) { return error; }
	if(int error = SpawnText(manager->musicChooser) != 0) { return error; }

	return 0;
}

int Main::SpawnHUD(HUDManager * manager)
{
	if(manager == NULL) { return 0; }

	if(int error = SpawnText(manager->prompt) != 0) { return error; }

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(manager->HUDs[i] == NULL) { continue; }
		if(int error = SpawnText(manager->HUDs[i]->livesCounter) != 0) { return error; }
		if(int error = SpawnText(manager->HUDs[i]->comboCounter) != 0) { return error; }
	}

	return 0;
}

int Main::SpawnText(HSText * text)
{
	if(text == NULL || text->charListToClone.empty()) { return 0; }

	list<HSCharToClone>::iterator ctcIt;
	for(ctcIt = text->charListToClone.begin(); ctcIt != text->charListToClone.end(); ctcIt++)
	{
		if(ctcIt->character == NULL) { continue; }

		HSObject * newChar;
		if(objectManager->loadingLoadingScreen)
		{
			if(int error = objectManager->CloneObject(ctcIt->character, &objectManager->loadingScreenObjects, &newChar) != 0) { return error; }
		}
		else
		{
			if(int error = objectManager->CloneObject(ctcIt->character, &objectManager->HUDObjects, &newChar) != 0) { return error; }
		}
		newChar->pos.x = ctcIt->pos.x;
		newChar->pos.y = ctcIt->pos.y;
		newChar->depth = ctcIt->depth;
		newChar->SetPalette(ctcIt->palette);

		text->characterList.push_back(newChar);
	}

	text->charListToClone.clear();

	return 0;
}

int Main::PlayAudio()
{
	if(gameState == MATCH && matchState == PAUSED) { return 0; }

	PlayAudio(&objectManager->stageObjects);
	PlayAudio(&objectManager->BGSpawnedObjects);
	PlayAudio(&objectManager->fighterObjects);
	PlayAudio(&objectManager->FGSpawnedObjects);

	return 0;
}

int Main::PlayAudio(list<HSObject*> * objects)
{
	list<HSObject*>::iterator objIt;
	for ( objIt=objects->begin(); objIt != objects->end(); objIt++)
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
	DeleteObjects(&objectManager->HUDObjects);

	if(gameState == MATCH && matchState == PAUSED) { return 0; }

	DeleteObjects(&objectManager->stageObjects);
	DeleteObjects(&objectManager->BGSpawnedObjects);
	DeleteObjects(&objectManager->fighterObjects);
	DeleteObjects(&objectManager->FGSpawnedObjects);

	return 0;
}

int Main::DeleteObjects(list<HSObject*> * objects)
{
	list<HSObject*>::iterator objIt = objects->begin();
	while (objIt != objects->end())
	{
		if((*objIt)->toDelete)
		{
			objectManager->ClearSpecificObject(*objIt);
			objIt = objects->erase(objIt);
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
	objectManager->ClearAllObjects();
	objectManager->clearTexturesAndPalettes = true;
	renderingManager->Execute();

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

int Main::ChangeGameState(GameState newState)
{
	objectManager->ClearAllObjects();

	gameState = newState;

	objectManager->matchCamera = false;

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

	objectManager->SortAllObjects();

	objectManager->loadTexturesAndPalettes = true;

	EndLoading();

	return 0;
}

int Main::StartLoading()
{
	objectManager->loadingLoadingScreen = true;
	if(int error = objectManager->LoadLoadingScreen("data/hud/Loading/loadingScreen.xml") != 0) { return error; objectManager->loadingLoadingScreen = false; }
	objectManager->loadingText->SetText(objectManager->loadingString);
	SpawnText(objectManager->loadingText);

	objectManager->loadTexturesAndPalettes = true;
	objectManager->doRender = true;
	renderingManager->Execute();

	objectManager->loadingLoadingScreen = false;

	return 0;
}

int Main::EndLoading()
{
	if(int error = objectManager->ClearLoadingObjects() != 0) { return error; }

	return 0;
}

int Main::EventMenu(InputStates * inputHistory, int frame)
{
	if(inputHistory->frame == frame && (inputHistory->bButtonDown.pressed || inputHistory->bHatDown.pressed || inputHistory->bKeyDown.pressed || inputHistory->bStickDown.pressed))
	{
		objectManager->menuManager->CursorNext();
	}
	else if(inputHistory->frame == frame && (inputHistory->bButtonUp.pressed || inputHistory->bHatUp.pressed || inputHistory->bKeyUp.pressed || inputHistory->bStickUp.pressed))
	{
		objectManager->menuManager->CursorPrev();
	}
	
	if(inputHistory->frame == frame && (inputHistory->bButtonLeft.pressed || inputHistory->bHatLeft.pressed || inputHistory->bKeyLeft.pressed || inputHistory->bStickLeft.pressed))
	{
		HSMenuFunction function = objectManager->menuManager->GetCurrentFunction();

		switch(function)
		{
		case FULL_SCREEN:
		case STRETCH_SCREEN:
		case FULL_SCREEN_RESOLUTION:
		case WINDOWED_RESOLUTION:
			objectManager->menuManager->ChoicePrev();
			break;
		}
	}
	else if(inputHistory->frame == frame && (inputHistory->bButtonRight.pressed || inputHistory->bHatRight.pressed || inputHistory->bKeyRight.pressed || inputHistory->bStickRight.pressed))
	{
		HSMenuFunction function = objectManager->menuManager->GetCurrentFunction();

		switch(function)
		{
		case FULL_SCREEN:
		case STRETCH_SCREEN:
		case FULL_SCREEN_RESOLUTION:
		case WINDOWED_RESOLUTION:
			objectManager->menuManager->ChoiceNext();
			break;
		}
	}

	if(inputHistory->frame == frame && (inputHistory->buttonMenuConfirm.pressed || inputHistory->keyMenuConfirm.pressed))
	{
		HSMenuFunction function = objectManager->menuManager->GetCurrentFunction();

		switch(function)
		{
		case FREE_FOR_ALL:
			gameType = GAME_TYPE_FREE_FOR_ALL;
			if(int error = ChangeGameState(CHARACTER_SELECT) != 0) { return error; }
			break;
		case KEY_CONFIG_UP:
			bindingKey = BINDING_UP;
			objectManager->menuManager->SetKeyConfigEnterForItem(KEY_CONFIG_UP);
			break;
		case KEY_CONFIG_DOWN:
			bindingKey = BINDING_DOWN;
			objectManager->menuManager->SetKeyConfigEnterForItem(KEY_CONFIG_DOWN);
			break;
		case KEY_CONFIG_LEFT:
			bindingKey = BINDING_LEFT;
			objectManager->menuManager->SetKeyConfigEnterForItem(KEY_CONFIG_LEFT);
			break;
		case KEY_CONFIG_RIGHT:
			bindingKey = BINDING_RIGHT;
			objectManager->menuManager->SetKeyConfigEnterForItem(KEY_CONFIG_RIGHT);
			break;
		case KEY_CONFIG_LIGHT_ATTACK:
			bindingKey = BINDING_LIGHT;
			objectManager->menuManager->SetKeyConfigEnterForItem(KEY_CONFIG_LIGHT_ATTACK);
			break;
		case KEY_CONFIG_HEAVY_ATTACK:
			bindingKey = BINDING_HEAVY;
			objectManager->menuManager->SetKeyConfigEnterForItem(KEY_CONFIG_HEAVY_ATTACK);
			break;
		case KEY_CONFIG_JUMP:
			bindingKey = BINDING_JUMP;
			objectManager->menuManager->SetKeyConfigEnterForItem(KEY_CONFIG_JUMP);
			break;
		case KEY_CONFIG_BLOCK:
			bindingKey = BINDING_BLOCK;
			objectManager->menuManager->SetKeyConfigEnterForItem(KEY_CONFIG_BLOCK);
			break;
		case KEY_CONFIG_PAUSE:
			bindingKey = BINDING_PAUSE;
			objectManager->menuManager->SetKeyConfigEnterForItem(KEY_CONFIG_PAUSE);
			break;
		case KEY_CONFIG_MENU_CONFIRM:
			bindingKey = BINDING_CONFIRM;
			objectManager->menuManager->SetKeyConfigEnterForItem(KEY_CONFIG_MENU_CONFIRM);
			break;
		case KEY_CONFIG_MENU_BACK:
			bindingKey = BINDING_BACK;
			objectManager->menuManager->SetKeyConfigEnterForItem(KEY_CONFIG_MENU_BACK);
			break;
		case KEY_CONFIG_PLAYER_1:
			playerToSetUp = 0;
			objectManager->menuManager->ToChild();
			objectManager->menuManager->SetCurrentTitleText("Player 1 Key Config");
			SetMenuKeyConfig();
			break;
		case KEY_CONFIG_PLAYER_2:
			playerToSetUp = 1;
			objectManager->menuManager->ToChild();
			objectManager->menuManager->SetCurrentTitleText("Player 2 Key Config");
			SetMenuKeyConfig();
			break;
		case KEY_CONFIG_PLAYER_3:
			playerToSetUp = 2;
			objectManager->menuManager->ToChild();
			objectManager->menuManager->SetCurrentTitleText("Player 3 Key Config");
			SetMenuKeyConfig();
			break;
		case KEY_CONFIG_PLAYER_4:
			playerToSetUp = 3;
			objectManager->menuManager->ToChild();
			objectManager->menuManager->SetCurrentTitleText("Player 4 Key Config");
			SetMenuKeyConfig();
			break;
		case VIDEO:
			objectManager->menuManager->ToChild();
			SetMenuVideoSettings();
			break;
		case KEY_CONFIG:
		case VERSUS:
		case OPTIONS:
			objectManager->menuManager->ToChild();
			break;
		case FULL_SCREEN:
		case STRETCH_SCREEN:
		case FULL_SCREEN_RESOLUTION:
		case WINDOWED_RESOLUTION:
			objectManager->menuManager->ChoiceNext();
			break;
		case APPLY_VIDEO_SETTINGS:
			ApplyAndSaveVideoSettings();
			break;
		case BACK:
			objectManager->menuManager->ToParent();
			break;
		case RESUME_MATCH:
			needToPause = true;
			break;
		case QUIT_MATCH:
			if(int error = ChangeGameState(CHARACTER_SELECT) != 0) { return error; }
			break;
		case QUIT_GAME:
			Exit();
			break;
		}
	}
	else if(inputHistory->frame == frame && (inputHistory->buttonMenuBack.pressed || inputHistory->keyMenuBack.pressed))
	{
		HSMenuFunction function = objectManager->menuManager->GetCurrentFunction();

		switch(gameState)
		{
		case MAIN_MENU:
			switch(function)
			{
			case VERSUS:
			case OPTIONS:
			case QUIT_GAME:
				Exit();
				break;
			default:
				objectManager->menuManager->ToParent();
				break;
			}
			break;
		case MATCH:
			switch(function)
			{
			case RESUME_MATCH:
			case VIDEO:
			case KEY_CONFIG:
			case QUIT_MATCH:
				ChangeMatchState(IN_PROGRESS);
				break;
			default:
				objectManager->menuManager->ToParent();
				break;
			}
			break;
		}
	}

	return 0;
}

int Main::UpdateMenu()
{
	objectManager->UpdateMenu();

	return 0;
}

int Main::InitializeMainMenu()
{
	//load all main menu elements and place them in HUDObjects
	HSMenu * mainMenu;
	
	if(int error = objectManager->LoadHSMenuCollection("data/hud/Menus/mainMenuCollection.xml", &mainMenu) != 0) { return error; }

	objectManager->menuManager = new MenuManager(mainMenu);
	objectManager->menuManager->SetHidden(false);

	return 0;
}

int Main::EventMainMenu(InputStates * inputHistory, int frame)
{
	if(int error = EventMenu(inputHistory, frame) != 0) { return error; }

	return 0;
}

int Main::UpdateMainMenu()
{
	UpdateMenu();

	return 0;
}

int Main::InitializeCharacterSelect()
{
	objectManager->characterSelectManager = new CharacterSelectManager();
	
	if(int error = objectManager->LoadCharacterSelect("data/hud/Menus/Character Select/CharacterSelect.xml", "data/characters/playableCharacters.xml", "data/stages/playableStages.xml", "data/music/playableMusic.xml") != 0) { return error; }

	objectManager->characterSelectManager->ChangeCharacterSelectState(CHARACTERS_SELECT);

	return 0;
}

int Main::EventCharacterSelect(InputStates * inputHistory, int frame, int player)
{
	if(inputHistory->frame == frame && (inputHistory->bButtonUp.pressed || inputHistory->bHatUp.pressed || inputHistory->bKeyUp.pressed || inputHistory->bStickUp.pressed))
	{
		objectManager->characterSelectManager->PlayerUp(player);
	}
	else if(inputHistory->frame == frame && (inputHistory->bButtonDown.pressed || inputHistory->bHatDown.pressed || inputHistory->bKeyDown.pressed || inputHistory->bStickDown.pressed))
	{
		objectManager->characterSelectManager->PlayerDown(player);
	}

	if(inputHistory->frame == frame && (inputHistory->bButtonLeft.pressed || inputHistory->bHatLeft.pressed || inputHistory->bKeyLeft.pressed || inputHistory->bStickLeft.pressed))
	{
		objectManager->characterSelectManager->PlayerLeft(player);
	}
	else if(inputHistory->frame == frame && (inputHistory->bButtonRight.pressed || inputHistory->bHatRight.pressed || inputHistory->bKeyRight.pressed || inputHistory->bStickRight.pressed))
	{
		objectManager->characterSelectManager->PlayerRight(player);
	}

	if(inputHistory->frame == frame && (inputHistory->buttonMenuConfirm.pressed || inputHistory->keyMenuConfirm.pressed))
	{
		if(objectManager->characterSelectManager->PlayerConfirm(player))
		{
			objectManager->characterSelectChoices = objectManager->characterSelectManager->GetChoices();
			if(int error = ChangeGameState(MATCH) != 0) { return error; }
		}
	}
	else if(inputHistory->frame == frame && (inputHistory->buttonMenuBack.pressed || inputHistory->keyMenuBack.pressed))
	{
		if(objectManager->characterSelectManager->PlayerCancel(player))
		{
			ChangeGameState(MAIN_MENU);
		}
	}

	return 0;
}

int Main::UpdateCharacterSelect()
{

	return 0;
}

int Main::InitializeMatch()
{
	//load stage/background
	if(int error = objectManager->LoadStage(objectManager->characterSelectChoices.stageDefFilePath) != 0) { return error; }

	//load HUD
	objectManager->hudManager = new HUDManager();
	if(int error = objectManager->LoadHUD("data/hud/TestHUD/TestHUD.xml", objectManager->characterSelectChoices.participating) != 0) { return error; }

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(objectManager->characterSelectChoices.participating[i])
		{
			//load fighter
			HSObject * fighter;
			if(int error = objectManager->LoadDefinition(objectManager->characterSelectChoices.characterDefFilePaths[i], &objectManager->fighterObjects, &fighter) != 0) { return error; }
			fighter->pos.x = objectManager->spawnPoints[i].x;
			fighter->pos.y = objectManager->spawnPoints[i].y;
			objectManager->players[i] = fighter;
			fighter->SetPalette(objectManager->characterSelectChoices.characterPalettes[i]);
			((Fighter*)fighter)->state = STANDING;
			((Fighter*)fighter)->curHealth = ((Fighter*)fighter)->health;
			((Fighter*)fighter)->lives = STARTING_LIVES;
			fighter->ChangeHold(((Fighter*)fighter)->fighterEventHolds.standing);
			objectManager->focusObject[i] = fighter;

			if(i % 2 == 1)
			{
				((Fighter*)fighter)->facing = LEFT;
			}

			objectManager->hudManager->HUDs[i]->objectToTrack = (Fighter*)fighter;

			HSObject * hudIcon;
			if(int error = objectManager->LoadDefinition(objectManager->characterSelectChoices.characterIconDefFilePaths[i], &objectManager->HUDObjects, &hudIcon) != 0) { return error; }
			hudIcon->pos.x = objectManager->hudManager->HUDs[i]->characterIconPos.x;
			hudIcon->pos.y = objectManager->hudManager->HUDs[i]->characterIconPos.y;
			hudIcon->depth = MATCH_HUD_ICON_DEPTH;

			objectManager->hudManager->HUDs[i]->characterIcon = hudIcon;
		}
	}

	HSMenu * pauseMenu;

	if(int error = objectManager->LoadHSMenuCollection("data/hud/Menus/pauseMenuCollection.xml", &pauseMenu) !=0) { return error; }

	objectManager->menuManager = new MenuManager(pauseMenu);
	objectManager->menuManager->SetHidden(true);

	ChangeMatchState(INTRO);
	objectManager->hudManager->ChangePromptState(PROMPT_READY);
	objectManager->hudManager->SetVisible(true);
	matchPromptTimer = 0;

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		ChangeMatchPlayerState(ACTIVE, i);
	}

	objectManager->centerCameraInstantly = true;
	objectManager->matchCamera = true;

	return 0;
}

int Main::ChangeMatchState(MatchState newState)
{
	matchState = newState;

	switch(newState)
	{
		case IN_PROGRESS:
			objectManager->menuManager->SetHidden(true);
			break;
		case PAUSED:
			objectManager->menuManager->SetHidden(false);
			break;
		case RESULTS:
			objectManager->menuManager->SetHidden(true);
			objectManager->hudManager->ChangePromptState(PROMPT_WIN);
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
			break;
	}

	return 0;
}

int Main::EventMatch(InputStates * inputHistory, int frame, int player)
{
	switch(matchState)
	{
	case INTRO:
		break;
	case IN_PROGRESS:
		if(inputHistory->frame == frame && (inputHistory->bButtonStart.pressed || inputHistory->bKeyStart.pressed))
		{
			needToPause = true;
		}
		else if(objectManager->players[player] != 0)
		{
			objectManager->players[player]->Event(inputHistory, frame);
		}
		break;
	case PAUSED:
		if(int error = EventMenu(inputHistory, frame) != 0) { return error; }
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
	switch (matchState)
	{
	case INTRO:
		matchPromptTimer++;
		if(matchPromptTimer >= PROMPT_READY_DURATION) {
			ChangeMatchState(IN_PROGRESS);
			matchPromptTimer = 0;
			objectManager->hudManager->ChangePromptState(PROMPT_FIGHT);
		}
		break;
	case IN_PROGRESS:
		if(matchPromptTimer < PROMPT_FIGHT_DURATION)
		{
			matchPromptTimer++;
			if(matchPromptTimer >= PROMPT_READY_DURATION) {
				matchPromptTimer = 0;
				objectManager->hudManager->ChangePromptState(PROMPT_CLEAR);
			}
		}
		break;
	case PAUSED:
		break;
	case RESULTS:
		matchPromptTimer++;
		if(matchPromptTimer >= PROMPT_WIN_DURATION) {
			ChangeGameState(CHARACTER_SELECT);
			matchPromptTimer = 0;
		}
		break;
	}

	if(gameState == MATCH)
	{
		objectManager->hudManager->Refresh();
		UpdateMenu();
	}

	return 0;
}

int Main::CollideMatch()
{
	int survivingPlayers = 0;

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		HSObject * player = objectManager->players[i];

		if(player == NULL) { ChangeMatchPlayerState(LOST, i); continue; }

		if((!objectManager->players[i]->IsTerrain() && objectManager->players[i]->pos.y > (objectManager->stageSize.y / 2) + RINGOUT_BUFFER) || (objectManager->players[i]->IsTerrainObject() && ((TerrainObject*)objectManager->players[i])->curHealth <= 0 &&
			(!objectManager->players[i]->IsFighter() || ((Fighter*)objectManager->players[i])->state != KNOCKOUT && ((Fighter*)objectManager->players[i])->state != KNOCKOUT_AIR)))
		{

			player->pos.x = objectManager->spawnPoints[i].x;
			player->pos.y = objectManager->spawnPoints[i].y;
			player->vel.x = 0;
			player->vel.y = 0;

			if(objectManager->players[i]->IsTerrainObject())
			{
				((TerrainObject*)player)->curHealth = ((TerrainObject*)objectManager->players[i])->health;
				((TerrainObject*)player)->lives--;

				if(((TerrainObject*)player)->lives <= 0)
				{
					if(int error = ChangeMatchPlayerState(LOST, i) != 0) { return error; }
					player->toDelete = true;
					player = NULL;
					objectManager->hudManager->HUDs[i]->objectToTrack = NULL;
					continue;
				}
			}

			if(objectManager->players[i]->IsFighter())
			{
				((Fighter*)player)->ChangeHold(((Fighter*)objectManager->players[i])->fighterEventHolds.jumpNeutralFall);
				((Fighter*)player)->state = JUMPING;
				((Fighter*)player)->falls = true;
			}
		}

		if(matchPlayerState[i] == LOST) { continue; }
		
		survivingPlayers++;
	}

	if(survivingPlayers <= 1 && matchState != RESULTS)
	{
		if(int i = ChangeMatchState(RESULTS) != 0) { return i; }
		objectManager->hudManager->ChangePromptState(PROMPT_WIN);
		matchPromptTimer = 0;
	}

	objectManager->hudManager->Refresh();

	return 0;
}

void Main::DefaultKeyConfig()
{
	mappings[0].up.type = KEY_SETTING_KEY; mappings[0].up.keycode = SDLK_w;
	mappings[0].down.type = KEY_SETTING_KEY; mappings[0].down.keycode = SDLK_s;
	mappings[0].left.type = KEY_SETTING_KEY; mappings[0].left.keycode = SDLK_a;
	mappings[0].right.type = KEY_SETTING_KEY; mappings[0].right.keycode = SDLK_d;
	mappings[0].jump.type = KEY_SETTING_KEY; mappings[0].jump.keycode = SDLK_h;
	mappings[0].light.type = KEY_SETTING_KEY; mappings[0].light.keycode = SDLK_g;
	mappings[0].heavy.type = KEY_SETTING_KEY; mappings[0].heavy.keycode = SDLK_b;
	mappings[0].block.type = KEY_SETTING_KEY; mappings[0].block.keycode = SDLK_j;
	mappings[0].start.type = KEY_SETTING_KEY; mappings[0].start.keycode = SDLK_z;
	mappings[0].menuConfirm.type = KEY_SETTING_KEY; mappings[0].menuConfirm.keycode = mappings[0].light.keycode;
	mappings[0].menuBack.type = KEY_SETTING_KEY; mappings[0].menuBack.keycode = mappings[0].jump.keycode;
	
	mappings[1].up.type = KEY_SETTING_KEY; mappings[1].up.keycode = SDLK_p;
	mappings[1].down.type = KEY_SETTING_KEY; mappings[1].down.keycode = SDLK_SEMICOLON;
	mappings[1].left.type = KEY_SETTING_KEY; mappings[1].left.keycode = SDLK_l;
	mappings[1].right.type = KEY_SETTING_KEY; mappings[1].right.keycode = SDLK_QUOTE;
	mappings[1].jump.type = KEY_SETTING_KEY; mappings[1].jump.keycode = SDLK_KP_5;
	mappings[1].light.type = KEY_SETTING_KEY; mappings[1].light.keycode = SDLK_KP_4;
	mappings[1].heavy.type = KEY_SETTING_KEY; mappings[1].heavy.keycode = SDLK_KP_1;
	mappings[1].block.type = KEY_SETTING_KEY; mappings[1].block.keycode = SDLK_KP_6;
	mappings[1].start.type = KEY_SETTING_KEY; mappings[1].start.keycode = SDLK_KP_0;
	mappings[1].menuConfirm.type = KEY_SETTING_KEY; mappings[1].menuConfirm.keycode = mappings[1].light.keycode;
	mappings[1].menuBack.type = KEY_SETTING_KEY; mappings[1].menuBack.keycode = mappings[1].jump.keycode;
}

int Main::LoadKeyConfig()
{
	//get the XML structure from the file
	XMLDocument * file = new XMLDocument();
	if(int error = file->LoadFile("data/config/keyConfig.xml") != 0)
	{
		//file doesn't exist, so just set some defaults and then save the configuration
		DefaultKeyConfig();
		return SaveKeyConfig();
	}

	if(strcmp(file->RootElement()->Value(), "KeyConfiguration") != 0)
	{
		//file isn't formatted correctly, so just set some defaults and then save the configuration
		DefaultKeyConfig();
		return SaveKeyConfig();
	}

	//loop through all the parts of the configuration
	for(XMLElement * i = file->RootElement()->FirstChildElement(); i != NULL; i = i->NextSiblingElement())
	{
		//get the config section
		const char * configSec = i->Value();

		//create a new object of the appropriate type
		if(strcmp(configSec, "PlayerOne") == 0)
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
		mappings[player].up.type = KEY_SETTING_KEY;
		if(!LoadToKeyConfig(buttonConfig, &mappings[player].up.keycode))
		{
			mappings[player].up.type = KEY_SETTING_BUTTON;
			if(!LoadToJoyButtonConfig(buttonConfig, &mappings[player].up.joystickMapping))
			{
				mappings[player].up.type = KEY_SETTING_HAT;
				if((hatOrStick = LoadToHatConfig(buttonConfig, &mappings[player].up.hat)) == false)
				{
					mappings[player].up.type = KEY_SETTING_STICK;
					if((hatOrStick = LoadToStickConfig(buttonConfig, &mappings[player].up.stick)) == false)
					{
						mappings[player].up.type = KEY_SETTING_NONE;
					}
				}
			}
		}
	}
	
	if(!hatOrStick && config->Attribute("down") != NULL)
	{
		string buttonConfig = config->Attribute("down");
		mappings[player].down.type = KEY_SETTING_KEY;
		if(!LoadToKeyConfig(buttonConfig, &mappings[player].down.keycode))
		{
			mappings[player].down.type = KEY_SETTING_BUTTON;
			if(!LoadToJoyButtonConfig(buttonConfig, &mappings[player].down.joystickMapping))
			{
				mappings[player].down.type = KEY_SETTING_HAT;
				if((hatOrStick = LoadToHatConfig(buttonConfig, &mappings[player].down.hat)) == false)
				{
					mappings[player].down.type = KEY_SETTING_STICK;
					if((hatOrStick = LoadToStickConfig(buttonConfig, &mappings[player].down.stick)) == false)
					{
						mappings[player].down.type = KEY_SETTING_NONE;
					}
				}
			}
		}
	}
	
	if(!hatOrStick && config->Attribute("left") != NULL)
	{
		string buttonConfig = config->Attribute("left");
		mappings[player].left.type = KEY_SETTING_KEY;
		if(!LoadToKeyConfig(buttonConfig, &mappings[player].left.keycode))
		{
			mappings[player].left.type = KEY_SETTING_BUTTON;
			if(!LoadToJoyButtonConfig(buttonConfig, &mappings[player].left.joystickMapping))
			{
				mappings[player].left.type = KEY_SETTING_HAT;
				if((hatOrStick = LoadToHatConfig(buttonConfig, &mappings[player].left.hat)) == false)
				{
					mappings[player].left.type = KEY_SETTING_STICK;
					if((hatOrStick = LoadToStickConfig(buttonConfig, &mappings[player].left.stick)) == false)
					{
						mappings[player].left.type = KEY_SETTING_NONE;
					}
				}
			}
		}
	}
	
	if(!hatOrStick && config->Attribute("right") != NULL)
	{
		string buttonConfig = config->Attribute("right");
		mappings[player].right.type = KEY_SETTING_KEY;
		if(!LoadToKeyConfig(buttonConfig, &mappings[player].right.keycode))
		{
			mappings[player].right.type = KEY_SETTING_BUTTON;
			if(!LoadToJoyButtonConfig(buttonConfig, &mappings[player].right.joystickMapping))
			{
				mappings[player].right.type = KEY_SETTING_HAT;
				if((hatOrStick = LoadToHatConfig(buttonConfig, &mappings[player].right.hat)) == false)
				{
					mappings[player].right.type = KEY_SETTING_STICK;
					if((hatOrStick = LoadToStickConfig(buttonConfig, &mappings[player].right.stick)) == false)
					{
						mappings[player].right.type = KEY_SETTING_NONE;
					}
				}
			}
		}
	}
	
	if(config->Attribute("light") != NULL)
	{
		string buttonConfig = config->Attribute("light");
		mappings[player].light.type = KEY_SETTING_KEY;
		if(!LoadToKeyConfig(buttonConfig, &mappings[player].light.keycode))
		{
			mappings[player].light.type = KEY_SETTING_BUTTON;
			if(!LoadToJoyButtonConfig(buttonConfig, &mappings[player].light.joystickMapping))
			{
				mappings[player].light.type = KEY_SETTING_NONE;
			}
		}
	}
	
	if(config->Attribute("heavy") != NULL)
	{
		string buttonConfig = config->Attribute("heavy");
		mappings[player].heavy.type = KEY_SETTING_KEY;
		if(!LoadToKeyConfig(buttonConfig, &mappings[player].heavy.keycode))
		{
			mappings[player].heavy.type = KEY_SETTING_BUTTON;
			if(!LoadToJoyButtonConfig(buttonConfig, &mappings[player].heavy.joystickMapping))
			{
				mappings[player].heavy.type = KEY_SETTING_NONE;
			}
		}
	}
	
	if(config->Attribute("jump") != NULL)
	{
		string buttonConfig = config->Attribute("jump");
		mappings[player].jump.type = KEY_SETTING_KEY;
		if(!LoadToKeyConfig(buttonConfig, &mappings[player].jump.keycode))
		{
			mappings[player].jump.type = KEY_SETTING_BUTTON;
			if(!LoadToJoyButtonConfig(buttonConfig, &mappings[player].jump.joystickMapping))
			{
				mappings[player].jump.type = KEY_SETTING_NONE;
			}
		}
	}
	
	if(config->Attribute("block") != NULL)
	{
		string buttonConfig = config->Attribute("block");
		mappings[player].block.type = KEY_SETTING_KEY;
		if(!LoadToKeyConfig(buttonConfig, &mappings[player].block.keycode))
		{
			mappings[player].block.type = KEY_SETTING_BUTTON;
			if(!LoadToJoyButtonConfig(buttonConfig, &mappings[player].block.joystickMapping))
			{
				mappings[player].block.type = KEY_SETTING_NONE;
			}
		}
	}
	
	if(config->Attribute("menuConfirm") != NULL)
	{
		string buttonConfig = config->Attribute("menuConfirm");
		mappings[player].menuConfirm.type = KEY_SETTING_KEY;
		if(!LoadToKeyConfig(buttonConfig, &mappings[player].menuConfirm.keycode))
		{
			mappings[player].menuConfirm.type = KEY_SETTING_BUTTON;
			if(!LoadToJoyButtonConfig(buttonConfig, &mappings[player].menuConfirm.joystickMapping))
			{
				mappings[player].menuConfirm.type = KEY_SETTING_NONE;
			}
		}
	}
	
	if(config->Attribute("menuBack") != NULL)
	{
		string buttonConfig = config->Attribute("menuBack");
		mappings[player].menuBack.type = KEY_SETTING_KEY;
		if(!LoadToKeyConfig(buttonConfig, &mappings[player].menuBack.keycode))
		{
			mappings[player].menuBack.type = KEY_SETTING_BUTTON;
			if(!LoadToJoyButtonConfig(buttonConfig, &mappings[player].menuBack.joystickMapping))
			{
				mappings[player].menuBack.type = KEY_SETTING_NONE;
			}
		}
	}
	
	if(config->Attribute("pause") != NULL)
	{
		string buttonConfig = config->Attribute("pause");
		mappings[player].start.type = KEY_SETTING_KEY;
		if(!LoadToKeyConfig(buttonConfig, &mappings[player].start.keycode))
		{
			mappings[player].start.type = KEY_SETTING_BUTTON;
			if(!LoadToJoyButtonConfig(buttonConfig, &mappings[player].start.joystickMapping))
			{
				mappings[player].start.type = KEY_SETTING_NONE;
			}
		}
	}
}

bool Main::LoadToKeyConfig(string config, SDL_Keycode * key)
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
	else if(config == "numpad_0") { *key = SDLK_KP_0; return true; }
	else if(config == "numpad_1") { *key = SDLK_KP_1; return true; }
	else if(config == "numpad_2") { *key = SDLK_KP_2; return true; }
	else if(config == "numpad_3") { *key = SDLK_KP_3; return true; }
	else if(config == "numpad_4") { *key = SDLK_KP_4; return true; }
	else if(config == "numpad_5") { *key = SDLK_KP_5; return true; }
	else if(config == "numpad_6") { *key = SDLK_KP_6; return true; }
	else if(config == "numpad_7") { *key = SDLK_KP_7; return true; }
	else if(config == "numpad_8") { *key = SDLK_KP_8; return true; }
	else if(config == "numpad_9") { *key = SDLK_KP_9; return true; }
	else if(config == "numpad_period") { *key = SDLK_KP_PERIOD; return true; }
	else if(config == "numpad_divide") { *key = SDLK_KP_DIVIDE; return true; }
	else if(config == "numpad_multiply") { *key = SDLK_KP_MULTIPLY; return true; }
	else if(config == "numpad_minus") { *key = SDLK_KP_MINUS; return true; }
	else if(config == "numpad_plus") { *key = SDLK_KP_PLUS; return true; }
	//else if(config == "numpad_enter") { *key = SDLK_KP_ENTER; return true; }
	else if(config == "numpad_equals") { *key = SDLK_KP_EQUALS; return true; }
	//else if(config == "up") { *key = SDLK_UP; return true; }
	//else if(config == "down") { *key = SDLK_DOWN; return true; }
	//else if(config == "right") { *key = SDLK_RIGHT; return true; }
	//else if(config == "left") { *key = SDLK_LEFT; return true; }
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

void Main::SetMenuVideoSettings()
{
	if(objectManager->menuManager == NULL) { return; }

	MenuManager * mm = objectManager->menuManager;

	objectManager->menuManager->SetChoiceForItem(FULL_SCREEN, mm->GetYesNoChooserFunction(objectManager->fullScreen));
	objectManager->menuManager->SetChoiceForItem(STRETCH_SCREEN, mm->GetYesNoChooserFunction(objectManager->stretchScreen));
	objectManager->menuManager->SetChoiceForItem(FULL_SCREEN_RESOLUTION, mm->GetResolutionChooserFunction(objectManager->fullscreenResolutionX, objectManager->fullscreenResolutionY));
	objectManager->menuManager->SetChoiceForItem(WINDOWED_RESOLUTION, mm->GetResolutionChooserFunction(objectManager->windowedResolutionX, objectManager->windowedResolutionY));
}

void Main::SetMenuKeyConfig()
{
	if(objectManager->menuManager == NULL) { return; }

	InputMappings mapping = mappings[playerToSetUp];

	SetMenuKeyConfig(KEY_CONFIG_UP, mapping.up);
	SetMenuKeyConfig(KEY_CONFIG_DOWN, mapping.down);
	SetMenuKeyConfig(KEY_CONFIG_LEFT, mapping.left);
	SetMenuKeyConfig(KEY_CONFIG_RIGHT, mapping.right);
	SetMenuKeyConfig(KEY_CONFIG_LIGHT_ATTACK, mapping.light);
	SetMenuKeyConfig(KEY_CONFIG_HEAVY_ATTACK, mapping.heavy);
	SetMenuKeyConfig(KEY_CONFIG_JUMP, mapping.jump);
	SetMenuKeyConfig(KEY_CONFIG_BLOCK, mapping.block);
	SetMenuKeyConfig(KEY_CONFIG_PAUSE, mapping.start);
	SetMenuKeyConfig(KEY_CONFIG_MENU_CONFIRM, mapping.menuConfirm);
	SetMenuKeyConfig(KEY_CONFIG_MENU_BACK, mapping.menuBack);
}

void Main::SetMenuKeyConfig(HSMenuFunction function, KeySetting setting)
{
	switch(setting.type)
	{
	case KEY_SETTING_NONE:
		objectManager->menuManager->SetKeyConfigNoSettingForItem(function);
		break;
	case KEY_SETTING_KEY:
		objectManager->menuManager->SetKeyConfigByKeyForItem(function, setting.keycode);
		break;
	case KEY_SETTING_BUTTON:
		objectManager->menuManager->SetKeyConfigByButtonForItem(function, setting.joystickMapping.joystick, setting.joystickMapping.button);
		break;
	case KEY_SETTING_HAT:
		objectManager->menuManager->SetKeyConfigByHatForItem(function, setting.hat);
		break;
	case KEY_SETTING_STICK:
		objectManager->menuManager->SetKeyConfigByStickForItem(function, setting.stick);
		break;
	}
}

void Main::ApplyAndSaveVideoSettings()
{
	if(objectManager->menuManager == NULL) { return; }

	MenuManager * mm = objectManager->menuManager;

	//apply the changes
	objectManager->ApplyVideoSettings();

	//save the settings
	MenuChooserFunction fullscreenFunc = mm->GetChooserFuncByMenuFunc(FULL_SCREEN);
	MenuChooserFunction stretchScreenfunc = mm->GetChooserFuncByMenuFunc(STRETCH_SCREEN);
	MenuChooserFunction fullscreenResFunc = mm->GetChooserFuncByMenuFunc(FULL_SCREEN_RESOLUTION);
	MenuChooserFunction windowedResFunc = mm->GetChooserFuncByMenuFunc(WINDOWED_RESOLUTION);

	string fullscreenString = mm->GetYesNoConfigString(fullscreenFunc);
	string stretchScreenString = mm->GetYesNoConfigString(stretchScreenfunc);
	string fullscreenResXString = mm->GetResolutionXConfigString(fullscreenResFunc);
	string fullscreenResYString = mm->GetResolutionYConfigString(fullscreenResFunc);
	string windowedResXString = mm->GetResolutionXConfigString(windowedResFunc);
	string windowedResYString = mm->GetResolutionYConfigString(windowedResFunc);

	XMLDocument * file = new XMLDocument();

	XMLElement * general = file->NewElement("General");
	general->SetAttribute("fullscreen", fullscreenString.data());
	general->SetAttribute("stretchScreen", stretchScreenString.data());
	general->SetAttribute("fullscreenResolutionX", fullscreenResXString.data());
	general->SetAttribute("fullscreenResolutionY", fullscreenResYString.data());
	general->SetAttribute("windowedResolutionX", windowedResXString.data());
	general->SetAttribute("windowedResolutionY", windowedResYString.data());

	XMLElement * config = file->NewElement("VideoConfiguration");
	config->InsertEndChild(general);

	file->InsertEndChild(config);

	file->SaveFile("data/config/videoConfig.xml");
}

int Main::SaveKeyConfig()
{
	//create a new xml structure
	XMLDocument * file = new XMLDocument();

	XMLElement * playerOne = file->NewElement("PlayerOne");
	SavePlayerKeyConfig(playerOne, 0);

	XMLElement * playerTwo = file->NewElement("PlayerTwo");
	SavePlayerKeyConfig(playerTwo, 1);

	XMLElement * playerThree = file->NewElement("PlayerThree");
	SavePlayerKeyConfig(playerThree, 2);

	XMLElement * playerFour = file->NewElement("PlayerFour");
	SavePlayerKeyConfig(playerFour, 3);

	XMLElement * config = file->NewElement("KeyConfiguration");
	config->InsertEndChild(playerOne);
	config->InsertEndChild(playerTwo);
	config->InsertEndChild(playerThree);
	config->InsertEndChild(playerFour);

	file->InsertEndChild(config);

	file->SaveFile("data/config/keyConfig.xml");

	return 0;
}

void Main::SavePlayerKeyConfig(XMLElement * config, int player)
{
	config->SetAttribute("up", GetKeyConfigString(mappings[player].up).data());
	config->SetAttribute("down", GetKeyConfigString(mappings[player].down).data());
	config->SetAttribute("left", GetKeyConfigString(mappings[player].left).data());
	config->SetAttribute("right", GetKeyConfigString(mappings[player].right).data());
	config->SetAttribute("light", GetKeyConfigString(mappings[player].light).data());
	config->SetAttribute("heavy", GetKeyConfigString(mappings[player].heavy).data());
	config->SetAttribute("jump", GetKeyConfigString(mappings[player].jump).data());
	config->SetAttribute("block", GetKeyConfigString(mappings[player].block).data());
	config->SetAttribute("menuConfirm", GetKeyConfigString(mappings[player].menuConfirm).data());
	config->SetAttribute("menuBack", GetKeyConfigString(mappings[player].menuBack).data());
	config->SetAttribute("pause", GetKeyConfigString(mappings[player].start).data());
}

string Main::GetKeyConfigString(KeySetting setting)
{
	if(setting.type == KEY_SETTING_KEY)
	{
		return GetKeyConfigString(setting.keycode);
	}
	else if(setting.type == KEY_SETTING_BUTTON)
	{
		return GetJoyButtonConfigString(setting.joystickMapping);
	}
	else if(setting.type == KEY_SETTING_HAT)
	{
		return GetHatConfigString(setting.hat);
	}
	else if(setting.type == KEY_SETTING_STICK)
	{
		return GetStickConfigString(setting.stick);
	}

	return "";
}

string Main::GetKeyConfigString(SDL_Keycode key)
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
	case SDLK_KP_0: return "numpad_0"; break;
	case SDLK_KP_1: return "numpad_1"; break;
	case SDLK_KP_2: return "numpad_2"; break;
	case SDLK_KP_3: return "numpad_3"; break;
	case SDLK_KP_4: return "numpad_4"; break;
	case SDLK_KP_5: return "numpad_5"; break;
	case SDLK_KP_6: return "numpad_6"; break;
	case SDLK_KP_7: return "numpad_7"; break;
	case SDLK_KP_8: return "numpad_8"; break;
	case SDLK_KP_9: return "numpad_9"; break;
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
	if(gameState == MATCH && needToPause)
	{
		if(matchState == IN_PROGRESS)
		{
			ChangeMatchState(PAUSED);
		}
		else if(matchState == PAUSED)
		{
			ChangeMatchState(IN_PROGRESS);
		}

		needToPause = false;
	}

	list<HSObject*>::iterator objIt;
	for ( objIt=objectManager->HUDObjects.begin(); objIt != objectManager->HUDObjects.end(); objIt++)
	{
		if(int error = (*objIt)->AdvanceHolds() != 0)
		{
			return error; //there was an error advancing holds
		}
	}

	if(gameState == MATCH && matchState == PAUSED) { return 0; }

	for ( objIt=objectManager->stageObjects.begin(); objIt != objectManager->stageObjects.end(); objIt++)
	{
		if(int error = (*objIt)->AdvanceHolds() != 0) { return error; }
	}
	for ( objIt=objectManager->BGSpawnedObjects.begin(); objIt != objectManager->BGSpawnedObjects.end(); objIt++)
	{
		if(int error = (*objIt)->AdvanceHolds() != 0) { return error; }
	}
	for ( objIt=objectManager->fighterObjects.begin(); objIt != objectManager->fighterObjects.end(); objIt++)
	{
		if(int error = (*objIt)->AdvanceHolds() != 0) { return error; }
	}
	for ( objIt=objectManager->FGSpawnedObjects.begin(); objIt != objectManager->FGSpawnedObjects.end(); objIt++)
	{
		if(int error = (*objIt)->AdvanceHolds() != 0) { return error; }
	}

	return 0;
}

//event/input stuff
int Main::Event()
{
	//reset current inputs
	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		inputStateChange[i] = false;
		*curInputs[i] = defaultInputs;
		curInputs[i]->frame = frame;
	}

	//get current inputs
	SDL_Event curEvent;
	while(SDL_PollEvent(&curEvent))
	{
        HandleEvent(&curEvent);
    }

	if(bindingKey != BINDING_NONE) { return 0; } 

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

void Main::ClearStickForAllPlayers(Uint8 which)
{
	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(mappings[i].up.type == KEY_SETTING_STICK && mappings[i].up.stick == which)
		{
			mappings[i].up.type = KEY_SETTING_NONE;
			mappings[i].down.type = KEY_SETTING_NONE;
			mappings[i].left.type = KEY_SETTING_NONE;
			mappings[i].right.type = KEY_SETTING_NONE;
		}
	}
}

void Main::ClearHatForAllPlayers(Uint8 which)
{
	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(mappings[i].up.type == KEY_SETTING_HAT && mappings[i].up.hat == which)
		{
			mappings[i].up.type = KEY_SETTING_NONE;
			mappings[i].down.type = KEY_SETTING_NONE;
			mappings[i].left.type = KEY_SETTING_NONE;
			mappings[i].right.type = KEY_SETTING_NONE;
		}
	}
}

void Main::ClearButtonForAllPlayers(Uint8 which, Uint8 button, int playerToIgnore)
{
	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(playerToIgnore != i || (bindingKey != BINDING_CONFIRM && bindingKey != BINDING_BACK))
		{
			if(mappings[i].up.type == KEY_SETTING_BUTTON && mappings[i].up.joystickMapping.joystick == which && mappings[i].up.joystickMapping.button == button)
			{
				mappings[i].up.type = KEY_SETTING_NONE;
			}
			if(mappings[i].down.type == KEY_SETTING_BUTTON && mappings[i].down.joystickMapping.joystick == which && mappings[i].down.joystickMapping.button == button)
			{
				mappings[i].down.type = KEY_SETTING_NONE;
			}
			if(mappings[i].left.type == KEY_SETTING_BUTTON && mappings[i].left.joystickMapping.joystick == which && mappings[i].left.joystickMapping.button == button)
			{
				mappings[i].left.type = KEY_SETTING_NONE;
			}
			if(mappings[i].right.type == KEY_SETTING_BUTTON && mappings[i].right.joystickMapping.joystick == which && mappings[i].right.joystickMapping.button == button)
			{
				mappings[i].right.type = KEY_SETTING_NONE;
			}
			if(mappings[i].light.type == KEY_SETTING_BUTTON && mappings[i].light.joystickMapping.joystick == which && mappings[i].light.joystickMapping.button == button)
			{
				mappings[i].light.type = KEY_SETTING_NONE;
			}
			if(mappings[i].heavy.type == KEY_SETTING_BUTTON && mappings[i].heavy.joystickMapping.joystick == which && mappings[i].heavy.joystickMapping.button == button)
			{
				mappings[i].heavy.type = KEY_SETTING_NONE;
			}
			if(mappings[i].jump.type == KEY_SETTING_BUTTON && mappings[i].jump.joystickMapping.joystick == which && mappings[i].jump.joystickMapping.button == button)
			{
				mappings[i].jump.type = KEY_SETTING_NONE;
			}
			if(mappings[i].block.type == KEY_SETTING_BUTTON && mappings[i].block.joystickMapping.joystick == which && mappings[i].block.joystickMapping.button == button)
			{
				mappings[i].block.type = KEY_SETTING_NONE;
			}
			if(mappings[i].start.type == KEY_SETTING_BUTTON && mappings[i].start.joystickMapping.joystick == which && mappings[i].start.joystickMapping.button == button)
			{
				mappings[i].start.type = KEY_SETTING_NONE;
			}
		}
		
		if(playerToIgnore != i || bindingKey == BINDING_CONFIRM || bindingKey == BINDING_BACK)
		{
			if(mappings[i].menuConfirm.type == KEY_SETTING_BUTTON && mappings[i].menuConfirm.joystickMapping.joystick == which && mappings[i].menuConfirm.joystickMapping.button == button)
			{
				mappings[i].menuConfirm.type = KEY_SETTING_NONE;
			}
			if(mappings[i].menuBack.type == KEY_SETTING_BUTTON && mappings[i].menuBack.joystickMapping.joystick == which && mappings[i].menuBack.joystickMapping.button == button)
			{
				mappings[i].menuBack.type = KEY_SETTING_NONE;
			}
		}
	}
}

void Main::ClearKeyForAllPlayers(SDL_Keycode sym, int playerToIgnore)
{
	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(playerToIgnore != i || (bindingKey != BINDING_CONFIRM && bindingKey != BINDING_BACK))
		{
			if(mappings[i].up.type == KEY_SETTING_KEY && mappings[i].up.keycode == sym) { mappings[i].up.type = KEY_SETTING_NONE; }
			if(mappings[i].down.type == KEY_SETTING_KEY && mappings[i].down.keycode == sym) { mappings[i].down.type = KEY_SETTING_NONE; }
			if(mappings[i].left.type == KEY_SETTING_KEY && mappings[i].left.keycode == sym) { mappings[i].left.type = KEY_SETTING_NONE; }
			if(mappings[i].right.type == KEY_SETTING_KEY && mappings[i].right.keycode == sym) { mappings[i].right.type = KEY_SETTING_NONE; }
			if(mappings[i].light.type == KEY_SETTING_KEY && mappings[i].light.keycode == sym) { mappings[i].light.type = KEY_SETTING_NONE; }
			if(mappings[i].heavy.type == KEY_SETTING_KEY && mappings[i].heavy.keycode == sym) { mappings[i].heavy.type = KEY_SETTING_NONE; }
			if(mappings[i].jump.type == KEY_SETTING_KEY && mappings[i].jump.keycode == sym) { mappings[i].jump.type = KEY_SETTING_NONE; }
			if(mappings[i].block.type == KEY_SETTING_KEY && mappings[i].block.keycode == sym) { mappings[i].block.type = KEY_SETTING_NONE; }
			if(mappings[i].start.type == KEY_SETTING_KEY && mappings[i].start.keycode == sym) { mappings[i].start.type = KEY_SETTING_NONE; }
		}
		
		if(playerToIgnore != i || bindingKey == BINDING_CONFIRM || bindingKey == BINDING_BACK)
		{
			if(mappings[i].menuConfirm.type == KEY_SETTING_KEY && mappings[i].menuConfirm.keycode == sym) { mappings[i].menuConfirm.type = KEY_SETTING_NONE; }
			if(mappings[i].menuBack.type == KEY_SETTING_KEY && mappings[i].menuBack.keycode == sym) { mappings[i].menuBack.type = KEY_SETTING_NONE; }
		}
	}
}

void Main::ClearControls(int player)
{
	if(player < 0 || player >= MAX_PLAYERS) { return; }
	mappings[player].up.type = KEY_SETTING_NONE;
	mappings[player].down.type = KEY_SETTING_NONE;
	mappings[player].left.type = KEY_SETTING_NONE;
	mappings[player].right.type = KEY_SETTING_NONE;
	mappings[player].light.type = KEY_SETTING_NONE;
	mappings[player].heavy.type = KEY_SETTING_NONE;
	mappings[player].jump.type = KEY_SETTING_NONE;
	mappings[player].block.type = KEY_SETTING_NONE;
	mappings[player].start.type = KEY_SETTING_NONE;
	mappings[player].menuConfirm.type = KEY_SETTING_NONE;
	mappings[player].menuBack.type = KEY_SETTING_NONE;
}

void Main::ChangePlayerPalette(int player)
{
	HSPalette * curPalette = objectManager->players[player]->palette;
	objectManager->players[player]->palette = NULL;

	curPalette->usingCount--;
	if(curPalette->usingCount < 0) { curPalette->usingCount = 0; }

	list<PaletteInstance>::iterator plIt;
	bool getNextPalette = false;
	for ( plIt=objectManager->players[player]->palettes.begin(); plIt != objectManager->players[player]->palettes.end(); plIt++)
	{
		if(getNextPalette && plIt->hsPal->usingCount <= 0)
		{
			objectManager->players[player]->palette = plIt->hsPal;
			plIt->hsPal->usingCount++;
			break;
		}
		else if(plIt->hsPal == curPalette)
		{
			getNextPalette = true;
		}
	}
	
	if(objectManager->players[player]->palette == NULL && !objectManager->players[player]->palettes.empty())
	{
		objectManager->players[player]->palette = objectManager->players[player]->palettes.front().hsPal;
		objectManager->players[player]->palette->usingCount++;
	}
}

int Main::HandleEvent(SDL_Event* Event) 
{
    switch(Event->type) 
	{
        case SDL_KEYDOWN: 
		{
			if(Event->key.repeat != 0) { break; }
			KeyDown(Event->key.keysym.sym);
            break;
        }

        case SDL_KEYUP: 
		{
			if(Event->key.repeat != 0) { break; }
			KeyUp(Event->key.keysym.sym);
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

void Main::KeyDown(SDL_Keycode sym) 
{
	if(bindingKey != BINDING_NONE)
	{
		if(sym == SDLK_ESCAPE)
		{
			HSMenuFunction func = NO_MENU_FUNCTION;
			switch(bindingKey)
			{
			case BINDING_UP:
				mappings[playerToSetUp].up.type = KEY_SETTING_NONE;
				func = KEY_CONFIG_UP;
				break;
			case BINDING_DOWN:
				mappings[playerToSetUp].down.type = KEY_SETTING_NONE;
				func = KEY_CONFIG_DOWN;
				break;
			case BINDING_LEFT:
				mappings[playerToSetUp].left.type = KEY_SETTING_NONE;
				func = KEY_CONFIG_LEFT;
				break;
			case BINDING_RIGHT:
				mappings[playerToSetUp].right.type = KEY_SETTING_NONE;
				func = KEY_CONFIG_RIGHT;
				break;
			case BINDING_LIGHT:
				mappings[playerToSetUp].light.type = KEY_SETTING_NONE;
				func = KEY_CONFIG_LIGHT_ATTACK;
				break;
			case BINDING_HEAVY:
				mappings[playerToSetUp].heavy.type = KEY_SETTING_NONE;
				func = KEY_CONFIG_HEAVY_ATTACK;
				break;
			case BINDING_JUMP:
				mappings[playerToSetUp].jump.type = KEY_SETTING_NONE;
				func = KEY_CONFIG_JUMP;
				break;
			case BINDING_BLOCK:
				mappings[playerToSetUp].block.type = KEY_SETTING_NONE;
				func = KEY_CONFIG_BLOCK;
				break;
			case BINDING_PAUSE:
				mappings[playerToSetUp].start.type = KEY_SETTING_NONE;
				func = KEY_CONFIG_PAUSE;
				break;
			case BINDING_CONFIRM:
				mappings[playerToSetUp].menuConfirm.type = KEY_SETTING_NONE;
				func = KEY_CONFIG_MENU_CONFIRM;
				break;
			case BINDING_BACK:
				mappings[playerToSetUp].menuBack.type = KEY_SETTING_NONE;
				func = KEY_CONFIG_MENU_BACK;
				break;
			}
			objectManager->menuManager->SetKeyConfigNoSettingForItem(func);
			bindingKey = BINDING_NONE;
			return;
		}

		if(sym == SDLK_RETURN
			|| sym == SDLK_KP_ENTER
			|| sym == SDLK_UP
			|| sym == SDLK_DOWN
			|| sym == SDLK_LEFT
			|| sym == SDLK_RIGHT)
		{
			return;
		}

		ClearKeyForAllPlayers(sym, playerToSetUp);
		switch(bindingKey)
		{
		case BINDING_UP: //up
			mappings[playerToSetUp].up.type = KEY_SETTING_KEY;
			mappings[playerToSetUp].up.keycode = sym;
			//objectManager->menuManager->SetKeyConfigByKeyForItem(KEY_CONFIG_UP, sym);
			SetMenuKeyConfig();
			objectManager->menuManager->CursorNext();
			bindingKey = BINDING_NONE;
			SaveKeyConfig();
			break;
		case BINDING_DOWN: //down
			mappings[playerToSetUp].down.type = KEY_SETTING_KEY;
			mappings[playerToSetUp].down.keycode = sym;
			//objectManager->menuManager->SetKeyConfigByKeyForItem(KEY_CONFIG_DOWN, sym);
			SetMenuKeyConfig();
			objectManager->menuManager->CursorNext();
			bindingKey = BINDING_NONE;
			SaveKeyConfig();
			break;
		case BINDING_LEFT: //left
			mappings[playerToSetUp].left.type = KEY_SETTING_KEY;
			mappings[playerToSetUp].left.keycode = sym;
			//objectManager->menuManager->SetKeyConfigByKeyForItem(KEY_CONFIG_LEFT, sym);
			SetMenuKeyConfig();
			objectManager->menuManager->CursorNext();
			bindingKey = BINDING_NONE;
			SaveKeyConfig();
			break;
		case BINDING_RIGHT: //right
			mappings[playerToSetUp].right.type = KEY_SETTING_KEY;
			mappings[playerToSetUp].right.keycode = sym;
			//objectManager->menuManager->SetKeyConfigByKeyForItem(KEY_CONFIG_RIGHT, sym);
			SetMenuKeyConfig();
			objectManager->menuManager->CursorNext();
			bindingKey = BINDING_NONE;
			SaveKeyConfig();
			break;
		case BINDING_LIGHT: //light attack
			mappings[playerToSetUp].light.type = KEY_SETTING_KEY;
			mappings[playerToSetUp].light.keycode = sym;
			//objectManager->menuManager->SetKeyConfigByKeyForItem(KEY_CONFIG_LIGHT_ATTACK, sym);
			SetMenuKeyConfig();
			objectManager->menuManager->CursorNext();
			bindingKey = BINDING_NONE;
			SaveKeyConfig();
			break;
		case BINDING_HEAVY: //heavy attack
			mappings[playerToSetUp].heavy.type = KEY_SETTING_KEY;
			mappings[playerToSetUp].heavy.keycode = sym;
			//objectManager->menuManager->SetKeyConfigByKeyForItem(KEY_CONFIG_HEAVY_ATTACK, sym);
			SetMenuKeyConfig();
			objectManager->menuManager->CursorNext();
			bindingKey = BINDING_NONE;
			SaveKeyConfig();
			break;
		case BINDING_JUMP: //jump
			mappings[playerToSetUp].jump.type = KEY_SETTING_KEY;
			mappings[playerToSetUp].jump.keycode = sym;
			//objectManager->menuManager->SetKeyConfigByKeyForItem(KEY_CONFIG_JUMP, sym);
			SetMenuKeyConfig();
			objectManager->menuManager->CursorNext();
			bindingKey = BINDING_NONE;
			SaveKeyConfig();
			break;
		case BINDING_BLOCK: //block
			mappings[playerToSetUp].block.type = KEY_SETTING_KEY;
			mappings[playerToSetUp].block.keycode = sym;
			//objectManager->menuManager->SetKeyConfigByKeyForItem(KEY_CONFIG_BLOCK, sym);
			SetMenuKeyConfig();
			objectManager->menuManager->CursorNext();
			bindingKey = BINDING_NONE;
			SaveKeyConfig();
			break;
		case BINDING_PAUSE: //pause
			mappings[playerToSetUp].start.type = KEY_SETTING_KEY;
			mappings[playerToSetUp].start.keycode = sym;
			//objectManager->menuManager->SetKeyConfigByKeyForItem(KEY_CONFIG_PAUSE, sym);
			SetMenuKeyConfig();
			objectManager->menuManager->CursorNext();
			bindingKey = BINDING_NONE;
			SaveKeyConfig();
			break;
		case BINDING_CONFIRM: //menu confirm
			mappings[playerToSetUp].menuConfirm.type = KEY_SETTING_KEY;
			mappings[playerToSetUp].menuConfirm.keycode = sym;
			//objectManager->menuManager->SetKeyConfigByKeyForItem(KEY_CONFIG_MENU_CONFIRM, sym);
			SetMenuKeyConfig();
			objectManager->menuManager->CursorNext();
			bindingKey = BINDING_NONE;
			SaveKeyConfig();
			break;
		case BINDING_BACK: //menu back
			mappings[playerToSetUp].menuBack.type = KEY_SETTING_KEY;
			mappings[playerToSetUp].menuBack.keycode = sym;
			//objectManager->menuManager->SetKeyConfigByKeyForItem(KEY_CONFIG_MENU_BACK, sym);
			SetMenuKeyConfig();
			objectManager->menuManager->CursorNext();
			bindingKey = BINDING_NONE;
			SaveKeyConfig();
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
		if(i == 0 && SDLK_UP == sym)
		{
			curInputs[i]->bKeyUp.pressed = true;
			inputStateChange[i] = true;
		}
		if(i == 0 && SDLK_DOWN == sym)
		{
			curInputs[i]->bKeyDown.pressed = true;
			inputStateChange[i] = true;
		}
		if(i == 0 && SDLK_LEFT == sym)
		{
			curInputs[i]->bKeyLeft.pressed = true;
			inputStateChange[i] = true;
		}
		if(i == 0 && SDLK_RIGHT == sym)
		{
			curInputs[i]->bKeyRight.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].up.type == KEY_SETTING_KEY && mappings[i].up.keycode == sym)
		{
			curInputs[i]->bKeyUp.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].down.type == KEY_SETTING_KEY && mappings[i].down.keycode == sym)
		{
			curInputs[i]->bKeyDown.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].left.type == KEY_SETTING_KEY && mappings[i].left.keycode == sym)
		{
			curInputs[i]->bKeyLeft.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].right.type == KEY_SETTING_KEY && mappings[i].right.keycode == sym)
		{
			curInputs[i]->bKeyRight.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].jump.type == KEY_SETTING_KEY && mappings[i].jump.keycode == sym)
		{
			curInputs[i]->bKeyJump.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].block.type == KEY_SETTING_KEY && mappings[i].block.keycode == sym)
		{
			curInputs[i]->bKeyBlock.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].light.type == KEY_SETTING_KEY && mappings[i].light.keycode == sym)
		{
			curInputs[i]->bKeyLight.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].heavy.type == KEY_SETTING_KEY && mappings[i].heavy.keycode == sym)
		{
			curInputs[i]->bKeyHeavy.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].start.type == KEY_SETTING_KEY && mappings[i].start.keycode == sym)
		{
			curInputs[i]->bKeyStart.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].select.type == KEY_SETTING_KEY && mappings[i].select.keycode == sym)
		{
			curInputs[i]->bKeySelect.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].menuConfirm.type == KEY_SETTING_KEY && mappings[i].menuConfirm.keycode == sym)
		{
			curInputs[i]->keyMenuConfirm.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].menuBack.type == KEY_SETTING_KEY && mappings[i].menuBack.keycode == sym)
		{
			curInputs[i]->keyMenuBack.pressed = true;
			inputStateChange[i] = true;
		}
	}
}

void Main::KeyUp(SDL_Keycode sym) 
{
	if(bindingKey != BINDING_NONE)
	{
		return;
	}

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(i == 0 && (SDLK_RETURN == sym || SDLK_KP_ENTER == sym))
		{
			curInputs[i]->keyMenuConfirm.released = true;
			inputStateChange[i] = true;
		}
		if(i == 0 && SDLK_ESCAPE == sym)
		{
			curInputs[i]->keyMenuBack.released = true;
			inputStateChange[i] = true;
		}
		if(i == 0 && SDLK_UP == sym)
		{
			curInputs[i]->bKeyUp.released = true;
			inputStateChange[i] = true;
		}
		if(i == 0 && SDLK_DOWN == sym)
		{
			curInputs[i]->bKeyDown.released = true;
			inputStateChange[i] = true;
		}
		if(i == 0 && SDLK_LEFT == sym)
		{
			curInputs[i]->bKeyLeft.released = true;
			inputStateChange[i] = true;
		}
		if(i == 0 && SDLK_RIGHT == sym)
		{
			curInputs[i]->bKeyRight.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].up.type == KEY_SETTING_KEY && mappings[i].up.keycode == sym)
		{
			curInputs[i]->bKeyUp.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].down.type == KEY_SETTING_KEY && mappings[i].down.keycode == sym)
		{
			curInputs[i]->bKeyDown.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].left.type == KEY_SETTING_KEY && mappings[i].left.keycode == sym)
		{
			curInputs[i]->bKeyLeft.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].right.type == KEY_SETTING_KEY && mappings[i].right.keycode == sym)
		{
			curInputs[i]->bKeyRight.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].jump.type == KEY_SETTING_KEY && mappings[i].jump.keycode == sym)
		{
			curInputs[i]->bKeyJump.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].block.type == KEY_SETTING_KEY && mappings[i].block.keycode == sym)
		{
			curInputs[i]->bKeyBlock.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].light.type == KEY_SETTING_KEY && mappings[i].light.keycode == sym)
		{
			curInputs[i]->bKeyLight.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].heavy.type == KEY_SETTING_KEY && mappings[i].heavy.keycode == sym)
		{
			curInputs[i]->bKeyHeavy.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].start.type == KEY_SETTING_KEY && mappings[i].start.keycode == sym)
		{
			curInputs[i]->bKeyStart.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].select.type == KEY_SETTING_KEY && mappings[i].select.keycode == sym)
		{
			curInputs[i]->bKeySelect.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].menuConfirm.type == KEY_SETTING_KEY && mappings[i].menuConfirm.keycode == sym)
		{
			curInputs[i]->keyMenuConfirm.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].menuBack.type == KEY_SETTING_KEY && mappings[i].menuBack.keycode == sym)
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
	if(bindingKey != BINDING_NONE)
	{
		if(abs(value) <= STICK_THRESHOLD) { return; }

		int cursorMoves = 1;

		switch(bindingKey)
		{
		case BINDING_UP: //up
			cursorMoves++;
		case BINDING_DOWN: //down
			cursorMoves++;
		case BINDING_LEFT: //left
			cursorMoves++;
		case BINDING_RIGHT: //right
			ClearStickForAllPlayers(which);
			mappings[playerToSetUp].up.type = KEY_SETTING_STICK;
			mappings[playerToSetUp].up.stick = which;
			mappings[playerToSetUp].down.type = KEY_SETTING_STICK;
			mappings[playerToSetUp].down.stick = which;
			mappings[playerToSetUp].left.type = KEY_SETTING_STICK;
			mappings[playerToSetUp].left.stick = which;
			mappings[playerToSetUp].right.type = KEY_SETTING_STICK;
			mappings[playerToSetUp].right.stick = which;
			objectManager->menuManager->SetKeyConfigByStickForItem(KEY_CONFIG_UP, which);
			objectManager->menuManager->SetKeyConfigByStickForItem(KEY_CONFIG_DOWN, which);
			objectManager->menuManager->SetKeyConfigByStickForItem(KEY_CONFIG_LEFT, which);
			objectManager->menuManager->SetKeyConfigByStickForItem(KEY_CONFIG_RIGHT, which);
			for(int i = 0; i < cursorMoves; i++)
			{
				objectManager->menuManager->CursorNext();
			}
			bindingKey = BINDING_NONE;
			SaveKeyConfig();
			break;
		}

		return;
	}

    InputStates * tempInputs = NULL;
	PreviousJoystickStates * prevStickState = NULL;
	int player = -1;
	
	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(mappings[i].up.type == KEY_SETTING_STICK && mappings[i].up.stick == which)
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
			if(prevStickState->stickHeldRight) { tempInputs->bStickRight.released = true; inputStateChange[player] = true; }
			prevStickState->stickHeldLeft = true;
			prevStickState->stickHeldRight = false;
		}
		else if(value > STICK_THRESHOLD)
		{
			if(!prevStickState->stickHeldRight) { tempInputs->bStickRight.pressed = true; inputStateChange[player] = true; }
			if(prevStickState->stickHeldLeft) { tempInputs->bStickLeft.released = true; inputStateChange[player] = true; }
			prevStickState->stickHeldLeft = false;
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
			if(prevStickState->stickHeldHardRight) { tempInputs->bStickHardRight.released = true; inputStateChange[player] = true; }
			prevStickState->stickHeldHardLeft = true;
			prevStickState->stickHeldHardRight = false;
		}
		else if(value > STICK_HARD_THRESHOLD)
		{
			if(!prevStickState->stickHeldHardRight) { tempInputs->bStickHardRight.pressed = true; inputStateChange[player] = true; }
			if(prevStickState->stickHeldHardLeft) { tempInputs->bStickHardLeft.released = true; inputStateChange[player] = true; }
			prevStickState->stickHeldHardLeft = false;
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
			if(prevStickState->stickHeldDown) { tempInputs->bStickDown.released = true; inputStateChange[player] = true; }
			prevStickState->stickHeldUp = true;
			prevStickState->stickHeldDown = false;
		}
		else if(value > STICK_THRESHOLD)
		{
			if(!prevStickState->stickHeldDown) { tempInputs->bStickDown.pressed = true; inputStateChange[player] = true; }
			if(prevStickState->stickHeldUp) { tempInputs->bStickUp.released = true; inputStateChange[player] = true; }
			prevStickState->stickHeldUp = false;
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
			if(prevStickState->stickHeldHardDown) { tempInputs->bStickHardDown.released = true; inputStateChange[player] = true; }
			prevStickState->stickHeldHardUp = true;
			prevStickState->stickHeldHardDown = false;
		}
		else if(value > STICK_HARD_THRESHOLD)
		{
			if(!prevStickState->stickHeldHardDown) { tempInputs->bStickHardDown.pressed = true; inputStateChange[player] = true; }
			if(prevStickState->stickHeldHardUp) { tempInputs->bStickHardUp.released = true; inputStateChange[player] = true; }
			prevStickState->stickHeldHardUp = false;
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
	if(bindingKey != BINDING_NONE)
	{
		int cursorMoves = 1;

		switch(bindingKey)
		{
		case BINDING_UP: //up
			cursorMoves++;
		case BINDING_DOWN: //down
			cursorMoves++;
		case BINDING_LEFT: //left
			cursorMoves++;
		case BINDING_RIGHT: //right
			ClearHatForAllPlayers(which);
			mappings[playerToSetUp].up.type = KEY_SETTING_HAT;
			mappings[playerToSetUp].up.hat = which;
			mappings[playerToSetUp].down.type = KEY_SETTING_HAT;
			mappings[playerToSetUp].down.hat = which;
			mappings[playerToSetUp].left.type = KEY_SETTING_HAT;
			mappings[playerToSetUp].left.hat = which;
			mappings[playerToSetUp].right.type = KEY_SETTING_HAT;
			mappings[playerToSetUp].right.hat = which;
			objectManager->menuManager->SetKeyConfigByHatForItem(KEY_CONFIG_UP, which);
			objectManager->menuManager->SetKeyConfigByHatForItem(KEY_CONFIG_DOWN, which);
			objectManager->menuManager->SetKeyConfigByHatForItem(KEY_CONFIG_LEFT, which);
			objectManager->menuManager->SetKeyConfigByHatForItem(KEY_CONFIG_RIGHT, which);
			for(int i = 0; i < cursorMoves; i++)
			{
				objectManager->menuManager->CursorNext();
			}
			bindingKey = BINDING_NONE;
			SaveKeyConfig();
			break;
		}

		return;
	}

    InputStates * tempInputs = NULL;
	PreviousJoystickStates * prevStickState;
	int player = -1;
	
	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(mappings[i].up.type == KEY_SETTING_HAT && mappings[i].up.hat == which)
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
	if(bindingKey != BINDING_NONE)
	{
		ClearButtonForAllPlayers(which, button, playerToSetUp);
		switch(bindingKey)
		{
		case BINDING_UP: //up
			mappings[playerToSetUp].up.type = KEY_SETTING_BUTTON;
			mappings[playerToSetUp].up.joystickMapping.joystick = which; mappings[playerToSetUp].up.joystickMapping.button = button;
			//objectManager->menuManager->SetKeyConfigByButtonForItem(KEY_CONFIG_UP, which, button);
			SetMenuKeyConfig();
			objectManager->menuManager->CursorNext();
			bindingKey = BINDING_NONE;
			SaveKeyConfig();
			break;
		case BINDING_DOWN: //down
			mappings[playerToSetUp].down.type = KEY_SETTING_BUTTON;
			mappings[playerToSetUp].down.joystickMapping.joystick = which; mappings[playerToSetUp].down.joystickMapping.button = button;
			//objectManager->menuManager->SetKeyConfigByButtonForItem(KEY_CONFIG_DOWN, which, button);
			SetMenuKeyConfig();
			objectManager->menuManager->CursorNext();
			bindingKey = BINDING_NONE;
			SaveKeyConfig();
			break;
		case BINDING_LEFT: //left
			mappings[playerToSetUp].left.type = KEY_SETTING_BUTTON;
			mappings[playerToSetUp].left.joystickMapping.joystick = which; mappings[playerToSetUp].left.joystickMapping.button = button;
			//objectManager->menuManager->SetKeyConfigByButtonForItem(KEY_CONFIG_LEFT, which, button);
			SetMenuKeyConfig();
			objectManager->menuManager->CursorNext();
			bindingKey = BINDING_NONE;
			SaveKeyConfig();
			break;
		case BINDING_RIGHT: //right
			mappings[playerToSetUp].right.type = KEY_SETTING_BUTTON;
			mappings[playerToSetUp].right.joystickMapping.joystick = which; mappings[playerToSetUp].right.joystickMapping.button = button;
			//objectManager->menuManager->SetKeyConfigByButtonForItem(KEY_CONFIG_RIGHT, which, button);
			SetMenuKeyConfig();
			objectManager->menuManager->CursorNext();
			bindingKey = BINDING_NONE;
			SaveKeyConfig();
			break;
		case BINDING_LIGHT: //light attack
			mappings[playerToSetUp].light.type = KEY_SETTING_BUTTON;
			mappings[playerToSetUp].light.joystickMapping.joystick = which; mappings[playerToSetUp].light.joystickMapping.button = button;
			//objectManager->menuManager->SetKeyConfigByButtonForItem(KEY_CONFIG_LIGHT_ATTACK, which, button);
			SetMenuKeyConfig();
			objectManager->menuManager->CursorNext();
			bindingKey = BINDING_NONE;
			SaveKeyConfig();
			break;
		case BINDING_HEAVY: //heavy attack
			mappings[playerToSetUp].heavy.type = KEY_SETTING_BUTTON;
			mappings[playerToSetUp].heavy.joystickMapping.joystick = which; mappings[playerToSetUp].heavy.joystickMapping.button = button;
			//objectManager->menuManager->SetKeyConfigByButtonForItem(KEY_CONFIG_HEAVY_ATTACK, which, button);
			SetMenuKeyConfig();
			objectManager->menuManager->CursorNext();
			bindingKey = BINDING_NONE;
			SaveKeyConfig();
			break;
		case BINDING_JUMP: //jump
			mappings[playerToSetUp].jump.type = KEY_SETTING_BUTTON;
			mappings[playerToSetUp].jump.joystickMapping.joystick = which; mappings[playerToSetUp].jump.joystickMapping.button = button;
			//objectManager->menuManager->SetKeyConfigByButtonForItem(KEY_CONFIG_JUMP, which, button);
			SetMenuKeyConfig();
			objectManager->menuManager->CursorNext();
			bindingKey = BINDING_NONE;
			SaveKeyConfig();
			break;
		case BINDING_BLOCK: //block
			mappings[playerToSetUp].block.type = KEY_SETTING_BUTTON;
			mappings[playerToSetUp].block.joystickMapping.joystick = which; mappings[playerToSetUp].block.joystickMapping.button = button;
			//objectManager->menuManager->SetKeyConfigByButtonForItem(KEY_CONFIG_BLOCK, which, button);
			SetMenuKeyConfig();
			objectManager->menuManager->CursorNext();
			bindingKey = BINDING_NONE;
			SaveKeyConfig();
			break;
		case BINDING_PAUSE: //pause
			mappings[playerToSetUp].start.type = KEY_SETTING_BUTTON;
			mappings[playerToSetUp].start.joystickMapping.joystick = which; mappings[playerToSetUp].start.joystickMapping.button = button;
			//objectManager->menuManager->SetKeyConfigByButtonForItem(KEY_CONFIG_PAUSE, which, button);
			SetMenuKeyConfig();
			objectManager->menuManager->CursorNext();
			bindingKey = BINDING_NONE;
			SaveKeyConfig();
			break;
		case BINDING_CONFIRM: //menu confirm
			mappings[playerToSetUp].menuConfirm.type = KEY_SETTING_BUTTON;
			mappings[playerToSetUp].menuConfirm.joystickMapping.joystick = which; mappings[playerToSetUp].menuConfirm.joystickMapping.button = button;
			//objectManager->menuManager->SetKeyConfigByButtonForItem(KEY_CONFIG_MENU_CONFIRM, which, button);
			SetMenuKeyConfig();
			objectManager->menuManager->CursorNext();
			bindingKey = BINDING_NONE;
			SaveKeyConfig();
			break;
		case BINDING_BACK: //menu back
			mappings[playerToSetUp].menuBack.type = KEY_SETTING_BUTTON;
			mappings[playerToSetUp].menuBack.joystickMapping.joystick = which; mappings[playerToSetUp].menuBack.joystickMapping.button = button;
			//objectManager->menuManager->SetKeyConfigByButtonForItem(KEY_CONFIG_MENU_BACK, which, button);
			SetMenuKeyConfig();
			objectManager->menuManager->CursorNext();
			bindingKey = BINDING_NONE;
			SaveKeyConfig();
			break;
		}

		return;
	}

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(mappings[i].up.type == KEY_SETTING_BUTTON && mappings[i].up.joystickMapping.joystick == which && mappings[i].up.joystickMapping.button == button)
		{
			curInputs[i]->bButtonUp.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].down.type == KEY_SETTING_BUTTON && mappings[i].down.joystickMapping.joystick == which && mappings[i].down.joystickMapping.button == button)
		{
			curInputs[i]->bButtonDown.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].left.type == KEY_SETTING_BUTTON && mappings[i].left.joystickMapping.joystick == which && mappings[i].left.joystickMapping.button == button)
		{
			curInputs[i]->bButtonLeft.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].right.type == KEY_SETTING_BUTTON && mappings[i].right.joystickMapping.joystick == which && mappings[i].right.joystickMapping.button == button)
		{
			curInputs[i]->bButtonRight.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].jump.type == KEY_SETTING_BUTTON && mappings[i].jump.joystickMapping.joystick == which && mappings[i].jump.joystickMapping.button == button)
		{
			curInputs[i]->bButtonJump.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].block.type == KEY_SETTING_BUTTON && mappings[i].block.joystickMapping.joystick == which && mappings[i].block.joystickMapping.button == button)
		{
			curInputs[i]->bButtonBlock.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].light.type == KEY_SETTING_BUTTON && mappings[i].light.joystickMapping.joystick == which && mappings[i].light.joystickMapping.button == button)
		{
			curInputs[i]->bButtonLight.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].heavy.type == KEY_SETTING_BUTTON && mappings[i].heavy.joystickMapping.joystick == which && mappings[i].heavy.joystickMapping.button == button)
		{
			curInputs[i]->bButtonHeavy.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].start.type == KEY_SETTING_BUTTON && mappings[i].start.joystickMapping.joystick == which && mappings[i].start.joystickMapping.button == button)
		{
			curInputs[i]->bButtonStart.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].select.type == KEY_SETTING_BUTTON && mappings[i].select.joystickMapping.joystick == which && mappings[i].select.joystickMapping.button == button)
		{
			curInputs[i]->bButtonSelect.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].menuConfirm.type == KEY_SETTING_BUTTON && mappings[i].menuConfirm.joystickMapping.joystick == which && mappings[i].menuConfirm.joystickMapping.button == button)
		{
			curInputs[i]->buttonMenuConfirm.pressed = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].menuBack.type == KEY_SETTING_BUTTON && mappings[i].menuBack.joystickMapping.joystick == which && mappings[i].menuBack.joystickMapping.button == button)
		{
			curInputs[i]->buttonMenuBack.pressed = true;
			inputStateChange[i] = true;
		}
	}
}

void Main::JoyButtonUp(Uint8 which,Uint8 button) 
{
	if(bindingKey != BINDING_NONE)
	{
		return;
	}

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(mappings[i].up.type == KEY_SETTING_BUTTON && mappings[i].up.joystickMapping.joystick == which && mappings[i].up.joystickMapping.button == button)
		{
			curInputs[i]->bButtonUp.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].down.type == KEY_SETTING_BUTTON && mappings[i].down.joystickMapping.joystick == which && mappings[i].down.joystickMapping.button == button)
		{
			curInputs[i]->bButtonDown.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].left.type == KEY_SETTING_BUTTON && mappings[i].left.joystickMapping.joystick == which && mappings[i].left.joystickMapping.button == button)
		{
			curInputs[i]->bButtonLeft.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].right.type == KEY_SETTING_BUTTON && mappings[i].right.joystickMapping.joystick == which && mappings[i].right.joystickMapping.button == button)
		{
			curInputs[i]->bButtonRight.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].jump.type == KEY_SETTING_BUTTON && mappings[i].jump.joystickMapping.joystick == which && mappings[i].jump.joystickMapping.button == button)
		{
			curInputs[i]->bButtonJump.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].block.type == KEY_SETTING_BUTTON && mappings[i].block.joystickMapping.joystick == which && mappings[i].block.joystickMapping.button == button)
		{
			curInputs[i]->bButtonBlock.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].light.type == KEY_SETTING_BUTTON && mappings[i].light.joystickMapping.joystick == which && mappings[i].light.joystickMapping.button == button)
		{
			curInputs[i]->bButtonLight.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].heavy.type == KEY_SETTING_BUTTON && mappings[i].heavy.joystickMapping.joystick == which && mappings[i].heavy.joystickMapping.button == button)
		{
			curInputs[i]->bButtonHeavy.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].start.type == KEY_SETTING_BUTTON && mappings[i].start.joystickMapping.joystick == which && mappings[i].start.joystickMapping.button == button)
		{
			curInputs[i]->bButtonStart.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].select.type == KEY_SETTING_BUTTON && mappings[i].select.joystickMapping.joystick == which && mappings[i].select.joystickMapping.button == button)
		{
			curInputs[i]->bButtonSelect.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].menuConfirm.type == KEY_SETTING_BUTTON && mappings[i].menuConfirm.joystickMapping.joystick == which && mappings[i].menuConfirm.joystickMapping.button == button)
		{
			curInputs[i]->buttonMenuConfirm.released = true;
			inputStateChange[i] = true;
		}
		if(mappings[i].menuBack.type == KEY_SETTING_BUTTON && mappings[i].menuBack.joystickMapping.joystick == which && mappings[i].menuBack.joystickMapping.button == button)
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
    objectManager->notDone = false;
}

void Main::User(Uint8 type, int code, void* data1, void* data2) 
{
    string blah = "";
}

int RenderThreadControl(void *data)
{
	if(data == NULL) { return -1; }

	RenderingManager * rm = new RenderingManager((ObjectManager*)data);

	return rm->Execute();
}

//main
int main( int argc, char* argv[] )
{
	HSLog = "";

	Main * theGame = new Main();
	int error = theGame->Execute();

	return error;
}
