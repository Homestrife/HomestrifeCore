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
	mainMenuState = TOP;
	gameType = FREE_FOR_ALL_2;

	lastFrameTicks = 0;
	frame = 0;

	playerToSetUp = -1;
	keyToSetUp = -1;

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

	list<HSObject*>::iterator objIt;
	for ( objIt=objectManager->HUDObjects.begin(); objIt != objectManager->HUDObjects.end(); objIt++)
	{
		if(int error = (*objIt)->Update() != 0)
		{
			return error; //there was an error updating the object
		}
	}

	if(gameState == MATCH && matchState != IN_PROGRESS) { return 0; }

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
	if(gameState == MATCH && matchState != IN_PROGRESS) { return 0; }

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

	if(gameState == MATCH)
	{
		if(int error = CollideMatch()!= 0) { return error; }
	}

	return 0;
}

int Main::SpawnObjects()
{
	list<HSObject*>::iterator objIt;
	for ( objIt=objectManager->stageObjects.begin(); objIt != objectManager->stageObjects.end(); objIt++)
	{
		list<SpawnObject*> spawnObjects = (*objIt)->GetSpawnObjects();

		list<SpawnObject*>::iterator spawnIt;
		for(spawnIt = spawnObjects.begin(); spawnIt != spawnObjects.end(); spawnIt++)
		{
			objectManager->CloneObject((*spawnIt), &objectManager->FGSpawnedObjects);
		}
	}
	for ( objIt=objectManager->BGSpawnedObjects.begin(); objIt != objectManager->BGSpawnedObjects.end(); objIt++)
	{
		list<SpawnObject*> spawnObjects = (*objIt)->GetSpawnObjects();

		list<SpawnObject*>::iterator spawnIt;
		for(spawnIt = spawnObjects.begin(); spawnIt != spawnObjects.end(); spawnIt++)
		{
			objectManager->CloneObject((*spawnIt), &objectManager->FGSpawnedObjects);
		}
	}
	for ( objIt=objectManager->fighterObjects.begin(); objIt != objectManager->fighterObjects.end(); objIt++)
	{
		list<SpawnObject*> spawnObjects = (*objIt)->GetSpawnObjects();

		list<SpawnObject*>::iterator spawnIt;
		for(spawnIt = spawnObjects.begin(); spawnIt != spawnObjects.end(); spawnIt++)
		{
			objectManager->CloneObject((*spawnIt), &objectManager->FGSpawnedObjects);
		}
	}
	for ( objIt=objectManager->FGSpawnedObjects.begin(); objIt != objectManager->FGSpawnedObjects.end(); objIt++)
	{
		list<SpawnObject*> spawnObjects = (*objIt)->GetSpawnObjects();

		list<SpawnObject*>::iterator spawnIt;
		for(spawnIt = spawnObjects.begin(); spawnIt != spawnObjects.end(); spawnIt++)
		{
			objectManager->CloneObject((*spawnIt), &objectManager->FGSpawnedObjects);
		}
	}

	objectManager->SortAllObjects();

	return 0;
}

int Main::PlayAudio()
{
	if(gameState == MATCH && matchState != IN_PROGRESS) { return 0; }

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
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\loading\\loading.xml", &objectManager->HUDObjects, &objectManager->loading) != 0) { return error; }
	objectManager->loading->pos.x = -90;
	objectManager->loading->pos.y = -18;

	objectManager->loadTexturesAndPalettes = true;
	objectManager->doRender = true;
	renderingManager->Execute();

	return 0;
}

int Main::EndLoading()
{
	objectManager->loading->visible = false;

	return 0;
}

int Main::InitializeMainMenu()
{
	//load all main menu elements and place them in HUDObjects
	HSObject * newObject;
	
	Menu * playerKeyConfigMenu = new Menu(MAIN_MENU_HEADER_HEIGHT, MAIN_MENU_CURSOR_WIDTH, MAIN_MENU_ITEM_HEIGHT, MAIN_MENU_ITEM_SPACING);
	playerKeyConfigMenu->pos.x = (MAX_GAME_RESOLUTION_X / -2) + MAIN_MENU_ITEM_SPACING;
	playerKeyConfigMenu->pos.y = (MAX_GAME_RESOLUTION_Y / -2) + MAIN_MENU_ITEM_SPACING;

	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\cursor\\cursor.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	playerKeyConfigMenu->SetCursor(newObject);
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\player\\player1.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	playerKeyConfigMenu->SetHeader(newObject);
	objectManager->playerOne = newObject;
	newObject->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\player\\player2.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	objectManager->playerTwo = newObject;
	newObject->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\player\\player3.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	objectManager->playerThree = newObject;
	newObject->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\player\\player4.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	objectManager->playerFour = newObject;
	newObject->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\up\\up.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	playerKeyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\down\\down.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	playerKeyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\left\\left.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	playerKeyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\right\\right.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	playerKeyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\lightAttack\\lightAttack.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	playerKeyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\heavyAttack\\heavyAttack.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	playerKeyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\jump\\jump.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	playerKeyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\block\\block.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	playerKeyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\pause\\pause.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	playerKeyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\menuConfirm\\menuConfirm.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	playerKeyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\menuBack\\menuBack.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	playerKeyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\back\\back.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	playerKeyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	Menu * videoMenu = new Menu(MAIN_MENU_HEADER_HEIGHT, MAIN_MENU_CURSOR_WIDTH, MAIN_MENU_ITEM_HEIGHT, MAIN_MENU_ITEM_SPACING);
	videoMenu->pos.x = (MAX_GAME_RESOLUTION_X / -2) + MAIN_MENU_ITEM_SPACING;
	videoMenu->pos.y = (MAX_GAME_RESOLUTION_Y / -2) + MAIN_MENU_ITEM_SPACING;

	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\cursor\\cursor.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	videoMenu->SetCursor(newObject);
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\video\\video.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	videoMenu->SetHeader(newObject);
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\fullscreen\\fullscreen.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	videoMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\stretchScreen\\stretchScreen.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	videoMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\resolution\\fullscreenResolution.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	videoMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\resolution\\windowedResolution.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	videoMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\apply\\apply.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	videoMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\back\\back.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	videoMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\yes\\yes.xml", &objectManager->HUDObjects, &objectManager->fullscreenYes) != 0) { return error; }
	objectManager->fullscreenYes->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->fullscreenYes->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING;
	objectManager->fullscreenYes->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\no\\no.xml", &objectManager->HUDObjects, &objectManager->fullscreenNo) != 0) { return error; }
	objectManager->fullscreenNo->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->fullscreenNo->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING;
	objectManager->fullscreenNo->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\yes\\yes.xml", &objectManager->HUDObjects, &objectManager->stretchYes) != 0) { return error; }
	objectManager->stretchYes->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->stretchYes->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 2 + MAIN_MENU_ITEM_HEIGHT;
	objectManager->stretchYes->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\no\\no.xml", &objectManager->HUDObjects, &objectManager->stretchNo) != 0) { return error; }
	objectManager->stretchNo->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->stretchNo->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 2 + MAIN_MENU_ITEM_HEIGHT;
	objectManager->stretchNo->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\640x360\\640x360.xml", &objectManager->HUDObjects, &objectManager->fullscreen640x360) != 0) { return error; }
	objectManager->fullscreen640x360->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->fullscreen640x360->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 3 + MAIN_MENU_ITEM_HEIGHT * 2;
	objectManager->fullscreen640x360->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\800x450\\800x450.xml", &objectManager->HUDObjects, &objectManager->fullscreen800x450) != 0) { return error; }
	objectManager->fullscreen800x450->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->fullscreen800x450->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 3 + MAIN_MENU_ITEM_HEIGHT * 2;
	objectManager->fullscreen800x450->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1024x576\\1024x576.xml", &objectManager->HUDObjects, &objectManager->fullscreen1024x576) != 0) { return error; }
	objectManager->fullscreen1024x576->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->fullscreen1024x576->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 3 + MAIN_MENU_ITEM_HEIGHT * 2;
	objectManager->fullscreen1024x576->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1152x648\\1152x648.xml", &objectManager->HUDObjects, &objectManager->fullscreen1152x648) != 0) { return error; }
	objectManager->fullscreen1152x648->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->fullscreen1152x648->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 3 + MAIN_MENU_ITEM_HEIGHT * 2;
	objectManager->fullscreen1152x648->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1280x720\\1280x720.xml", &objectManager->HUDObjects, &objectManager->fullscreen1280x720) != 0) { return error; }
	objectManager->fullscreen1280x720->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->fullscreen1280x720->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 3 + MAIN_MENU_ITEM_HEIGHT * 2;
	objectManager->fullscreen1280x720->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1360x765\\1360x765.xml", &objectManager->HUDObjects, &objectManager->fullscreen1360x765) != 0) { return error; }
	objectManager->fullscreen1360x765->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->fullscreen1360x765->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 3 + MAIN_MENU_ITEM_HEIGHT * 2;
	objectManager->fullscreen1360x765->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1366x768\\1366x768.xml", &objectManager->HUDObjects, &objectManager->fullscreen1366x768) != 0) { return error; }
	objectManager->fullscreen1366x768->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->fullscreen1366x768->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 3 + MAIN_MENU_ITEM_HEIGHT * 2;
	objectManager->fullscreen1366x768->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1400x787\\1400x787.xml", &objectManager->HUDObjects, &objectManager->fullscreen1400x787) != 0) { return error; }
	objectManager->fullscreen1400x787->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->fullscreen1400x787->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 3 + MAIN_MENU_ITEM_HEIGHT * 2;
	objectManager->fullscreen1400x787->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1440x810\\1440x810.xml", &objectManager->HUDObjects, &objectManager->fullscreen1440x810) != 0) { return error; }
	objectManager->fullscreen1440x810->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->fullscreen1440x810->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 3 + MAIN_MENU_ITEM_HEIGHT * 2;
	objectManager->fullscreen1440x810->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1600x900\\1600x900.xml", &objectManager->HUDObjects, &objectManager->fullscreen1600x900) != 0) { return error; }
	objectManager->fullscreen1600x900->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->fullscreen1600x900->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 3 + MAIN_MENU_ITEM_HEIGHT * 2;
	objectManager->fullscreen1600x900->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1680x945\\1680x945.xml", &objectManager->HUDObjects, &objectManager->fullscreen1680x945) != 0) { return error; }
	objectManager->fullscreen1680x945->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->fullscreen1680x945->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 3 + MAIN_MENU_ITEM_HEIGHT * 2;
	objectManager->fullscreen1680x945->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1920x1080\\1920x1080.xml", &objectManager->HUDObjects, &objectManager->fullscreen1920x1080) != 0) { return error; }
	objectManager->fullscreen1920x1080->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->fullscreen1920x1080->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 3 + MAIN_MENU_ITEM_HEIGHT * 2;
	objectManager->fullscreen1920x1080->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\640x360\\640x360.xml", &objectManager->HUDObjects, &objectManager->windowed640x360) != 0) { return error; }
	objectManager->windowed640x360->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->windowed640x360->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 4 + MAIN_MENU_ITEM_HEIGHT * 3;
	objectManager->windowed640x360->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\800x450\\800x450.xml", &objectManager->HUDObjects, &objectManager->windowed800x450) != 0) { return error; }
	objectManager->windowed800x450->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->windowed800x450->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 4 + MAIN_MENU_ITEM_HEIGHT * 3;
	objectManager->windowed800x450->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1024x576\\1024x576.xml", &objectManager->HUDObjects, &objectManager->windowed1024x576) != 0) { return error; }
	objectManager->windowed1024x576->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->windowed1024x576->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 4 + MAIN_MENU_ITEM_HEIGHT * 3;
	objectManager->windowed1024x576->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1152x648\\1152x648.xml", &objectManager->HUDObjects, &objectManager->windowed1152x648) != 0) { return error; }
	objectManager->windowed1152x648->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->windowed1152x648->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 4 + MAIN_MENU_ITEM_HEIGHT * 3;
	objectManager->windowed1152x648->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1280x720\\1280x720.xml", &objectManager->HUDObjects, &objectManager->windowed1280x720) != 0) { return error; }
	objectManager->windowed1280x720->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->windowed1280x720->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 4 + MAIN_MENU_ITEM_HEIGHT * 3;
	objectManager->windowed1280x720->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1360x765\\1360x765.xml", &objectManager->HUDObjects, &objectManager->windowed1360x765) != 0) { return error; }
	objectManager->windowed1360x765->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->windowed1360x765->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 4 + MAIN_MENU_ITEM_HEIGHT * 3;
	objectManager->windowed1360x765->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1366x768\\1366x768.xml", &objectManager->HUDObjects, &objectManager->windowed1366x768) != 0) { return error; }
	objectManager->windowed1366x768->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->windowed1366x768->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 4 + MAIN_MENU_ITEM_HEIGHT * 3;
	objectManager->windowed1366x768->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1400x787\\1400x787.xml", &objectManager->HUDObjects, &objectManager->windowed1400x787) != 0) { return error; }
	objectManager->windowed1400x787->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->windowed1400x787->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 4 + MAIN_MENU_ITEM_HEIGHT * 3;
	objectManager->windowed1400x787->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1440x810\\1440x810.xml", &objectManager->HUDObjects, &objectManager->windowed1440x810) != 0) { return error; }
	objectManager->windowed1440x810->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->windowed1440x810->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 4 + MAIN_MENU_ITEM_HEIGHT * 3;
	objectManager->windowed1440x810->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1600x900\\1600x900.xml", &objectManager->HUDObjects, &objectManager->windowed1600x900) != 0) { return error; }
	objectManager->windowed1600x900->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->windowed1600x900->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 4 + MAIN_MENU_ITEM_HEIGHT * 3;
	objectManager->windowed1600x900->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1680x945\\1680x945.xml", &objectManager->HUDObjects, &objectManager->windowed1680x945) != 0) { return error; }
	objectManager->windowed1680x945->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->windowed1680x945->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 4 + MAIN_MENU_ITEM_HEIGHT * 3;
	objectManager->windowed1680x945->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1920x1080\\1920x1080.xml", &objectManager->HUDObjects, &objectManager->windowed1920x1080) != 0) { return error; }
	objectManager->windowed1920x1080->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->windowed1920x1080->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 4 + MAIN_MENU_ITEM_HEIGHT * 3;
	objectManager->windowed1920x1080->visible = false;
	
	Menu * keyConfigMenu = new Menu(MAIN_MENU_HEADER_HEIGHT, MAIN_MENU_CURSOR_WIDTH, MAIN_MENU_ITEM_HEIGHT, MAIN_MENU_ITEM_SPACING);
	keyConfigMenu->pos.x = (MAX_GAME_RESOLUTION_X / -2) + MAIN_MENU_ITEM_SPACING;
	keyConfigMenu->pos.y = (MAX_GAME_RESOLUTION_Y / -2) + MAIN_MENU_ITEM_SPACING;

	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\cursor\\cursor.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	keyConfigMenu->SetCursor(newObject);
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\keyConfig\\keyConfig.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	keyConfigMenu->SetHeader(newObject);
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\player\\player1.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	keyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject, playerKeyConfigMenu));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\player\\player2.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	keyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject, playerKeyConfigMenu));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\player\\player3.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	keyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject, playerKeyConfigMenu));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\player\\player4.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	keyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject, playerKeyConfigMenu));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\back\\back.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	keyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));

	Menu * freeForAllMenu = new Menu(MAIN_MENU_HEADER_HEIGHT, MAIN_MENU_CURSOR_WIDTH, MAIN_MENU_ITEM_HEIGHT, MAIN_MENU_ITEM_SPACING);
	freeForAllMenu->pos.x = (MAX_GAME_RESOLUTION_X / -2) + MAIN_MENU_ITEM_SPACING;
	freeForAllMenu->pos.y = (MAX_GAME_RESOLUTION_Y / -2) + MAIN_MENU_ITEM_SPACING;

	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\cursor\\cursor.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	freeForAllMenu->SetCursor(newObject);
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\freeForAll\\freeForAll.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	freeForAllMenu->SetHeader(newObject);

	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\players\\2players.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	freeForAllMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));

	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\players\\3players.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	freeForAllMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));

	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\players\\4players.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	freeForAllMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\back\\back.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	freeForAllMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));

	Menu * versusMenu = new Menu(MAIN_MENU_HEADER_HEIGHT, MAIN_MENU_CURSOR_WIDTH, MAIN_MENU_ITEM_HEIGHT, MAIN_MENU_ITEM_SPACING);
	versusMenu->pos.x = (MAX_GAME_RESOLUTION_X / -2) + MAIN_MENU_ITEM_SPACING;
	versusMenu->pos.y = (MAX_GAME_RESOLUTION_Y / -2) + MAIN_MENU_ITEM_SPACING;

	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\cursor\\cursor.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	versusMenu->SetCursor(newObject);
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\versus\\versus.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	versusMenu->SetHeader(newObject);

	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\freeForAll\\freeForAll.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	versusMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject, freeForAllMenu));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\back\\back.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	versusMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	Menu * optionsMenu = new Menu(MAIN_MENU_HEADER_HEIGHT, MAIN_MENU_CURSOR_WIDTH, MAIN_MENU_ITEM_HEIGHT, MAIN_MENU_ITEM_SPACING);
	optionsMenu->pos.x = (MAX_GAME_RESOLUTION_X / -2) + MAIN_MENU_ITEM_SPACING;
	optionsMenu->pos.y = (MAX_GAME_RESOLUTION_Y / -2) + MAIN_MENU_ITEM_SPACING;

	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\cursor\\cursor.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	optionsMenu->SetCursor(newObject);
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\options\\options.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	optionsMenu->SetHeader(newObject);
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\video\\video.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	optionsMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject, videoMenu));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\keyConfig\\keyConfig.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	optionsMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject, keyConfigMenu));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\back\\back.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	optionsMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));

	Menu * mainMenu = new Menu(MAIN_MENU_HEADER_HEIGHT, MAIN_MENU_CURSOR_WIDTH, MAIN_MENU_ITEM_HEIGHT, MAIN_MENU_ITEM_SPACING);
	mainMenu->pos.x = (MAX_GAME_RESOLUTION_X / -2) + MAIN_MENU_ITEM_SPACING;
	mainMenu->pos.y = (MAX_GAME_RESOLUTION_Y / -2) + MAIN_MENU_ITEM_SPACING;

	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\cursor\\cursor.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	mainMenu->SetCursor(newObject);
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\mainMenu\\mainMenu.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	mainMenu->SetHeader(newObject);
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\versus\\versus.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	mainMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject, versusMenu));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\options\\options.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	mainMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject, optionsMenu));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\quit\\quitGame.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	mainMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject, optionsMenu));

	objectManager->menuManager = new MenuManager(mainMenu);
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\pressDesiredButton\\pressDesiredButton.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	objectManager->pressDesiredButton = newObject;
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
	case FREE_FOR_ALL:
		break;
	case OPTIONS:
		objectManager->MakeVideoSettingsInvisible();
		break;
	case VIDEO:
		objectManager->fullScreenToApply = objectManager->fullScreen;
		objectManager->stretchScreenToApply = objectManager->stretchScreen;
		objectManager->fullscreenResolutionXToApply = objectManager->fullscreenResolutionX;
		objectManager->fullscreenResolutionYToApply = objectManager->fullscreenResolutionY;
		objectManager->windowedResolutionXToApply = objectManager->windowedResolutionX;
		objectManager->windowedResolutionYToApply = objectManager->windowedResolutionY;

		objectManager->SetVideoSettingVisibility();
		break;
	case KEY_CONFIG:
		break;
	case PLAYER_KEY_CONFIG:
		objectManager->pressDesiredButton->visible = false;
		switch(playerToSetUp)
		{
		case 0:
			objectManager->menuManager->SetHeader(objectManager->playerOne);
			break;
		case 1:
			objectManager->menuManager->SetHeader(objectManager->playerTwo);
			break;
		case 2:
			objectManager->menuManager->SetHeader(objectManager->playerThree);
			break;
		case 3:
			objectManager->menuManager->SetHeader(objectManager->playerFour);
			break;
		}
		break;
	case INPUT_KEY:
		objectManager->pressDesiredButton->pos.x = objectManager->menuManager->GetCursorPos().x + PRESS_DESIRED_BUTTON_OFFSET_X;
		objectManager->pressDesiredButton->pos.y = objectManager->menuManager->GetCursorPos().y;
		objectManager->pressDesiredButton->visible = true;
		break;
	}

	return 0;
}

int Main::EventMainMenu(InputStates * inputHistory, int frame)
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
		switch(mainMenuState)
		{
		case VIDEO:
			switch(objectManager->menuManager->GetCursorIndex())
			{
			case 0:
				if(objectManager->fullScreenToApply) { objectManager->fullScreenToApply = false; }
				else { objectManager->fullScreenToApply = true; }
				objectManager->SetVideoSettingVisibility();
				break;
			case 1:
				if(objectManager->stretchScreenToApply) { objectManager->stretchScreenToApply = false; }
				else { objectManager->stretchScreenToApply = true; }
				objectManager->SetVideoSettingVisibility();
				break;
			case 2:
				objectManager->PrevFullscreenResolution();
				objectManager->SetVideoSettingVisibility();
				break;
			case 3:
				objectManager->PrevWindowedResolution();
				objectManager->SetVideoSettingVisibility();
				break;
			}
			break;
		}
	}
	else if(inputHistory->frame == frame && (inputHistory->bButtonRight.pressed || inputHistory->bHatRight.pressed || inputHistory->bKeyRight.pressed || inputHistory->bStickRight.pressed))
	{
		switch(mainMenuState)
		{
		case VIDEO:
			switch(objectManager->menuManager->GetCursorIndex())
			{
			case 0:
				if(objectManager->fullScreenToApply) { objectManager->fullScreenToApply = false; }
				else { objectManager->fullScreenToApply = true; }
				objectManager->SetVideoSettingVisibility();
				break;
			case 1:
				if(objectManager->stretchScreenToApply) { objectManager->stretchScreenToApply = false; }
				else { objectManager->stretchScreenToApply = true; }
				objectManager->SetVideoSettingVisibility();
				break;
			case 2:
				objectManager->NextFullscreenResolution();
				objectManager->SetVideoSettingVisibility();
				break;
			case 3:
				objectManager->NextWindowedResolution();
				objectManager->SetVideoSettingVisibility();
				break;
			}
			break;
		}
	}

	if(inputHistory->frame == frame && (inputHistory->buttonMenuConfirm.pressed || inputHistory->keyMenuConfirm.pressed))
	{
		switch(mainMenuState)
		{
		case TOP:
			switch(objectManager->menuManager->GetCursorIndex())
			{
			case 0:
				objectManager->menuManager->ToChild();
				ChangeMainMenuState(VERSUS);
				break;
			case 1:
				objectManager->menuManager->ToChild();
				ChangeMainMenuState(OPTIONS);
				break;
			case 2:
				Exit();
				break;
			}
			break;
		case VERSUS:
			switch(objectManager->menuManager->GetCursorIndex())
			{
			case 0:
				objectManager->menuManager->ToChild();
				ChangeMainMenuState(FREE_FOR_ALL);
				break;
			case 1:
				objectManager->menuManager->ToParent();
				ChangeMainMenuState(TOP);
				break;
			}
			break;
		case FREE_FOR_ALL:
			switch(objectManager->menuManager->GetCursorIndex())
			{
			case 0:
				gameType = FREE_FOR_ALL_2;
				if(int i = ChangeGameState(CHARACTER_SELECT) != 0) { return i; }
				break;
			case 1:
				gameType = FREE_FOR_ALL_3;
				if(int i = ChangeGameState(CHARACTER_SELECT) != 0) { return i; }
				break;
			case 2:
				gameType = FREE_FOR_ALL_4;
				if(int i = ChangeGameState(CHARACTER_SELECT) != 0) { return i; }
				break;
			case 3:
				objectManager->menuManager->ToParent();
				ChangeMainMenuState(VERSUS);
				break;
			}
			break;
		case OPTIONS:
			switch(objectManager->menuManager->GetCursorIndex())
			{
			case 0:
				objectManager->menuManager->ToChild();
				ChangeMainMenuState(VIDEO);
				break;
			case 1:
				objectManager->menuManager->ToChild();
				ChangeMainMenuState(KEY_CONFIG);
				break;
			case 2:
				objectManager->menuManager->ToParent();
				ChangeMainMenuState(TOP);
				break;
			}
			break;
		case VIDEO:
			switch(objectManager->menuManager->GetCursorIndex())
			{
			case 0:
				if(objectManager->fullScreenToApply) { objectManager->fullScreenToApply = false; }
				else { objectManager->fullScreenToApply = true; }
				objectManager->SetVideoSettingVisibility();
				break;
			case 1:
				if(objectManager->stretchScreenToApply) { objectManager->stretchScreenToApply = false; }
				else { objectManager->stretchScreenToApply = true; }
				objectManager->SetVideoSettingVisibility();
				break;
			case 2:
				objectManager->NextFullscreenResolution();
				objectManager->SetVideoSettingVisibility();
				break;
			case 3:
				objectManager->NextWindowedResolution();
				objectManager->SetVideoSettingVisibility();
				break;
			case 4:
				if(int error = objectManager->ApplyVideoSettings() != 0) { return error; }
				objectManager->SetVideoSettingVisibility();
				break;
			case 5:
				objectManager->menuManager->ToParent();
				ChangeMainMenuState(OPTIONS);
				break;
			}
			break;
		case KEY_CONFIG:
			switch(objectManager->menuManager->GetCursorIndex())
			{
			case 0:
			case 1:
			case 2:
			case 3:
				playerToSetUp = objectManager->menuManager->GetCursorIndex();
				objectManager->menuManager->ToChild();
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
				break;
			case 4:
				objectManager->menuManager->ToParent();
				ChangeMainMenuState(OPTIONS);
				break;
			}
			break;
		case PLAYER_KEY_CONFIG:
			switch(objectManager->menuManager->GetCursorIndex())
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
				keyToSetUp = objectManager->menuManager->GetCursorIndex();
				ChangeMainMenuState(INPUT_KEY);
				break;
			case 11:
				playerToSetUp = -1;
				objectManager->menuManager->ToParent();
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
			objectManager->menuManager->ToParent();
			ChangeMainMenuState(TOP);
			break;
		case FREE_FOR_ALL:
			objectManager->menuManager->ToParent();
			ChangeMainMenuState(VERSUS);
			break;
		case OPTIONS:
			objectManager->menuManager->ToParent();
			ChangeMainMenuState(TOP);
			break;
		case VIDEO:
			objectManager->menuManager->ToParent();
			ChangeMainMenuState(OPTIONS);
			break;
		case KEY_CONFIG:
			objectManager->menuManager->ToParent();
			ChangeMainMenuState(OPTIONS);
			break;
		case PLAYER_KEY_CONFIG:
			playerToSetUp = -1;
			objectManager->menuManager->ToParent();
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
	bool toPlay[MAX_PLAYERS];

	toPlay[0] = true;
	toPlay[1] = true;
	toPlay[2] = false;
	toPlay[3] = false;

	if(gameType == FREE_FOR_ALL_3 || gameType == FREE_FOR_ALL_4)
	{
		toPlay[2] = true;
	}

	if(gameType == FREE_FOR_ALL_4)
	{
		toPlay[3] = true;
	}
	
	if(int error = objectManager->LoadPlayableCharacters(toPlay) != 0) { return error; }
	if(int error = objectManager->LoadPlayableStages() != 0) { return error; }

	float hudYOffset = 0;
	if(gameType == FREE_FOR_ALL_2)
	{
		hudYOffset = 350;
	}

	//hud
	HSObject * newObject;
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\characterSelect\\characterSelect.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	newObject->pos.x = CHARACTER_SELECT_POS_X;
	newObject->pos.y = CHARACTER_SELECT_POS_Y;
	objectManager->characterSelect = newObject;
	
	//player 1 hud
	list<PlayableCharacter>::iterator pcIt;
	for(pcIt = objectManager->characterList[0].begin(); pcIt != objectManager->characterList[0].end(); pcIt++)
	{
		pcIt->demoObject->pos.x = CHAR_SELECT_PLAYER_LEFT_POS_X;
		pcIt->demoObject->pos.y = CHAR_SELECT_PLAYER_TOP_POS_Y + hudYOffset;
	}

	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\player\\player1.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	newObject->pos.x = PLAYER_LEFT_POS_X;
	newObject->pos.y = PLAYER_TOP_POS_Y + hudYOffset;
	objectManager->playerOne = newObject;

	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\selectCharacter\\selectCharacter.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	newObject->pos.x = SELECT_CHARACTER_LEFT_POS_X;
	newObject->pos.y = SELECT_PALETTE_TOP_POS_Y + hudYOffset;
	objectManager->selectCharacterOne = newObject;

	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\selectPalette\\selectPalette.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	newObject->pos.x = SELECT_PALETTE_LEFT_POS_X;
	newObject->pos.y = SELECT_PALETTE_TOP_POS_Y + hudYOffset;
	objectManager->selectPaletteOne = newObject;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\leftArrow\\leftArrow.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	newObject->pos.x = SELECT_CHARACTER_LEFT_POS_X - 55;
	newObject->pos.y = SELECT_PALETTE_TOP_POS_Y + hudYOffset;
	objectManager->selectPaletteLeftOne = newObject;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\rightArrow\\rightArrow.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	newObject->pos.x = SELECT_CHARACTER_LEFT_POS_X + 363;
	newObject->pos.y = SELECT_PALETTE_TOP_POS_Y + hudYOffset;
	objectManager->selectPaletteRightOne = newObject;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\ready\\ready.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	newObject->pos.x = READY_LEFT_POS_X;
	newObject->pos.y = SELECT_PALETTE_TOP_POS_Y + hudYOffset;
	objectManager->readyOne = newObject;
	
	//player 2 hud
	for(pcIt = objectManager->characterList[1].begin(); pcIt != objectManager->characterList[1].end(); pcIt++)
	{
		pcIt->demoObject->pos.x = CHAR_SELECT_PLAYER_RIGHT_POS_X;
		pcIt->demoObject->pos.y = CHAR_SELECT_PLAYER_TOP_POS_Y + hudYOffset;
		pcIt->demoObject->hFlip = true;
	}

	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\player\\player2.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	newObject->pos.x = PLAYER_RIGHT_POS_X;
	newObject->pos.y = PLAYER_TOP_POS_Y + hudYOffset;
	objectManager->playerTwo = newObject;

	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\selectCharacter\\selectCharacter.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	newObject->pos.x = SELECT_CHARACTER_RIGHT_POS_X;
	newObject->pos.y = SELECT_PALETTE_TOP_POS_Y + hudYOffset;
	objectManager->selectCharacterTwo = newObject;

	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\selectPalette\\selectPalette.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	newObject->pos.x = SELECT_PALETTE_RIGHT_POS_X;
	newObject->pos.y = SELECT_PALETTE_TOP_POS_Y + hudYOffset;
	objectManager->selectPaletteTwo = newObject;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\leftArrow\\leftArrow.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	newObject->pos.x = SELECT_CHARACTER_RIGHT_POS_X - 55;
	newObject->pos.y = SELECT_PALETTE_TOP_POS_Y + hudYOffset;
	objectManager->selectPaletteLeftTwo = newObject;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\rightArrow\\rightArrow.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	newObject->pos.x = SELECT_CHARACTER_RIGHT_POS_X + 363;
	newObject->pos.y = SELECT_PALETTE_TOP_POS_Y + hudYOffset;
	objectManager->selectPaletteRightTwo = newObject;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\ready\\ready.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	newObject->pos.x = READY_RIGHT_POS_X;
	newObject->pos.y = SELECT_PALETTE_TOP_POS_Y + hudYOffset;
	objectManager->readyTwo = newObject;
	
	//player 3 hud
	if(gameType == FREE_FOR_ALL_3 || gameType == FREE_FOR_ALL_4)
	{
		for(pcIt = objectManager->characterList[2].begin(); pcIt != objectManager->characterList[2].end(); pcIt++)
		{
			pcIt->demoObject->pos.x = CHAR_SELECT_PLAYER_LEFT_POS_X;
			pcIt->demoObject->pos.y = CHAR_SELECT_PLAYER_BOTTOM_POS_Y;
		}

		if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\player\\player3.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
		newObject->pos.x = PLAYER_LEFT_POS_X;
		newObject->pos.y = PLAYER_BOTTOM_POS_Y;
		objectManager->playerThree = newObject;

		if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\selectCharacter\\selectCharacter.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
		newObject->pos.x = SELECT_CHARACTER_LEFT_POS_X;
		newObject->pos.y = SELECT_PALETTE_BOTTOM_POS_Y;
		objectManager->selectCharacterThree = newObject;

		if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\selectPalette\\selectPalette.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
		newObject->pos.x = SELECT_PALETTE_LEFT_POS_X;
		newObject->pos.y = SELECT_PALETTE_BOTTOM_POS_Y;
		objectManager->selectPaletteThree = newObject;
	
		if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\leftArrow\\leftArrow.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
		newObject->pos.x = SELECT_CHARACTER_LEFT_POS_X - 55;
		newObject->pos.y = SELECT_PALETTE_BOTTOM_POS_Y;
		objectManager->selectPaletteLeftThree = newObject;
	
		if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\rightArrow\\rightArrow.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
		newObject->pos.x = SELECT_CHARACTER_LEFT_POS_X + 363;
		newObject->pos.y = SELECT_PALETTE_BOTTOM_POS_Y;
		objectManager->selectPaletteRightThree = newObject;
	
		if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\ready\\ready.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
		newObject->pos.x = READY_LEFT_POS_X;
		newObject->pos.y = SELECT_PALETTE_BOTTOM_POS_Y;
		objectManager->readyThree = newObject;
	}
	
	//player 4 hud
	if(gameType == FREE_FOR_ALL_4)
	{
		for(pcIt = objectManager->characterList[3].begin(); pcIt != objectManager->characterList[3].end(); pcIt++)
		{
			pcIt->demoObject->pos.x = CHAR_SELECT_PLAYER_RIGHT_POS_X;
			pcIt->demoObject->pos.y = CHAR_SELECT_PLAYER_BOTTOM_POS_Y;
			pcIt->demoObject->hFlip = true;
		}

		if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\player\\player4.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
		newObject->pos.x = PLAYER_RIGHT_POS_X;
		newObject->pos.y = PLAYER_BOTTOM_POS_Y;
		objectManager->playerFour = newObject;

		if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\selectCharacter\\selectCharacter.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
		newObject->pos.x = SELECT_CHARACTER_RIGHT_POS_X;
		newObject->pos.y = SELECT_PALETTE_BOTTOM_POS_Y;
		objectManager->selectCharacterFour = newObject;

		if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\selectPalette\\selectPalette.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
		newObject->pos.x = SELECT_PALETTE_RIGHT_POS_X;
		newObject->pos.y = SELECT_PALETTE_BOTTOM_POS_Y;
		objectManager->selectPaletteFour = newObject;
	
		if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\leftArrow\\leftArrow.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
		newObject->pos.x = SELECT_CHARACTER_RIGHT_POS_X - 55;
		newObject->pos.y = SELECT_PALETTE_BOTTOM_POS_Y;
		objectManager->selectPaletteLeftFour = newObject;
	
		if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\rightArrow\\rightArrow.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
		newObject->pos.x = SELECT_CHARACTER_RIGHT_POS_X + 363;
		newObject->pos.y = SELECT_PALETTE_BOTTOM_POS_Y;
		objectManager->selectPaletteRightFour = newObject;
	
		if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\ready\\ready.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
		newObject->pos.x = READY_RIGHT_POS_X;
		newObject->pos.y = SELECT_PALETTE_BOTTOM_POS_Y;
		objectManager->readyFour = newObject;
	}

	//stage select
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\stageSelect\\stageSelect.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	newObject->pos.x = STAGE_SELECT_POS_X;
	newObject->pos.y = CHARACTER_SELECT_POS_Y;
	objectManager->stageSelect = newObject;

	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\selectStage\\selectStage.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	newObject->pos.x = SELECT_STAGE_POS_X;
	newObject->pos.y = SELECT_STAGE_POS_Y;
	objectManager->selectStage = newObject;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\leftArrow\\leftArrow.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	newObject->pos.x = SELECT_STAGE_POS_X - 55;
	newObject->pos.y = SELECT_STAGE_POS_Y;
	objectManager->selectStageLeft = newObject;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\rightArrow\\rightArrow.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	newObject->pos.x = SELECT_STAGE_POS_X + 280;
	newObject->pos.y = SELECT_STAGE_POS_Y;
	objectManager->selectStageRight = newObject;

	ChangeCharacterSelectState(PLAYERS_SELECTING);

	if(toPlay[0]) { ChangeCharacterSelectPlayerState(SELECTING_CHARACTER, 0); }
	if(toPlay[1]) { ChangeCharacterSelectPlayerState(SELECTING_CHARACTER, 1); }
	if(toPlay[2]) { ChangeCharacterSelectPlayerState(SELECTING_CHARACTER, 2); }
	if(toPlay[3]) { ChangeCharacterSelectPlayerState(SELECTING_CHARACTER, 3); }

	objectManager->centerCameraInstantly = true;

	return 0;
}

int Main::ChangeCharacterSelectState(CharacterSelectState newState)
{
	characterSelectState = newState;

	switch(characterSelectState)
	{
	case PLAYERS_SELECTING:
		//make all stage select assets invisible
		objectManager->stageSelect->visible = false;
		objectManager->selectStage->visible = false;
		objectManager->selectStageLeft->visible = false;
		objectManager->selectStageRight->visible = false;
		objectManager->selectedStage.demoObject->visible = false;

		//make all character selct assets visible
		objectManager->characterSelect->visible = true;
		if(objectManager->selectedCharacters[0].demoObject != NULL)
		{
			objectManager->selectedCharacters[0].demoObject->visible = true;
			objectManager->playerOne->visible = true;

			if(characterSelectPlayerState[0] == READY)
			{
				objectManager->readyOne->visible = true;
			}
			else if(characterSelectPlayerState[0] == SELECTING_PALETTE)
			{
				objectManager->selectPaletteOne->visible = true;
				objectManager->selectPaletteLeftOne->visible = true;
				objectManager->selectPaletteRightOne->visible = true;
			}
		}
		if(objectManager->selectedCharacters[1].demoObject != NULL)
		{
			objectManager->selectedCharacters[1].demoObject->visible = true;
			objectManager->playerTwo->visible = true;

			if(characterSelectPlayerState[1] == READY)
			{
				objectManager->readyTwo->visible = true;
			}
			else if(characterSelectPlayerState[1] == SELECTING_PALETTE)
			{
				objectManager->selectPaletteTwo->visible = true;
				objectManager->selectPaletteLeftTwo->visible = true;
				objectManager->selectPaletteRightTwo->visible = true;
			}
		}
		if(objectManager->selectedCharacters[2].demoObject != NULL)
		{
			objectManager->selectedCharacters[2].demoObject->visible = true;
			objectManager->playerThree->visible = true;

			if(characterSelectPlayerState[2] == READY)
			{
				objectManager->readyThree->visible = true;
			}
			else if(characterSelectPlayerState[2] == SELECTING_PALETTE)
			{
				objectManager->selectPaletteThree->visible = true;
				objectManager->selectPaletteLeftThree->visible = true;
				objectManager->selectPaletteRightThree->visible = true;
			}
		}
		if(objectManager->selectedCharacters[3].demoObject != NULL)
		{
			objectManager->selectedCharacters[3].demoObject->visible = true;
			objectManager->playerFour->visible = true;

			if(characterSelectPlayerState[3] == READY)
			{
				objectManager->readyFour->visible = true;
			}
			else if(characterSelectPlayerState[3] == SELECTING_PALETTE)
			{
				objectManager->selectPaletteFour->visible = true;
				objectManager->selectPaletteLeftFour->visible = true;
				objectManager->selectPaletteRightFour->visible = true;
			}
		}
		break;
	case STAGE_SELECT:
		//make all character select assets invisible
		objectManager->characterSelect->visible = false;
		if(objectManager->selectedCharacters[0].demoObject != NULL)
		{
			objectManager->selectedCharacters[0].demoObject->visible = false;
			objectManager->playerOne->visible = false;
			objectManager->readyOne->visible = false;
		}
		if(objectManager->selectedCharacters[1].demoObject != NULL)
		{
			objectManager->selectedCharacters[1].demoObject->visible = false;
			objectManager->playerTwo->visible = false;
			objectManager->readyTwo->visible = false;
		}
		if(objectManager->selectedCharacters[2].demoObject != NULL)
		{
			objectManager->selectedCharacters[2].demoObject->visible = false;
			objectManager->playerThree->visible = false;
			objectManager->readyThree->visible = false;
		}
		if(objectManager->selectedCharacters[3].demoObject != NULL)
		{
			objectManager->selectedCharacters[3].demoObject->visible = false;
			objectManager->playerFour->visible = false;
			objectManager->readyFour->visible = false;
		}

		//make all stage select assets visible
		objectManager->stageSelect->visible = true;
		objectManager->selectStage->visible = true;
		objectManager->selectStageLeft->visible = true;
		objectManager->selectStageRight->visible = true;
		objectManager->selectedStage.demoObject->visible = true;
		break;
	}

	return 0;
}

int Main::ChangeCharacterSelectPlayerState(CharacterSelectPlayerState newState, int player)
{
	characterSelectPlayerState[player] = newState;

	switch(characterSelectPlayerState[player])
	{
	case SELECTING_CHARACTER:
		objectManager->selectedPalettes[player] = 0;
		objectManager->selectedCharacters[player].demoObject->SetPalette(0);
		switch(player)
		{
		case 0:
			objectManager->readyOne->visible = false;
			objectManager->selectPaletteLeftOne->visible = true;
			objectManager->selectPaletteRightOne->visible = true;
			objectManager->selectCharacterOne->visible = true;
			objectManager->selectPaletteOne->visible = false;
			break;
		case 1:
			objectManager->readyTwo->visible = false;
			objectManager->selectPaletteLeftTwo->visible = true;
			objectManager->selectPaletteRightTwo->visible = true;
			objectManager->selectCharacterTwo->visible = true;
			objectManager->selectPaletteTwo->visible = false;
			break;
		case 2:
			if(objectManager->selectedCharacters[2].demoObject != NULL)
			{
				objectManager->readyThree->visible = false;
				objectManager->selectPaletteLeftThree->visible = true;
				objectManager->selectPaletteRightThree->visible = true;
				objectManager->selectCharacterThree->visible = true;
				objectManager->selectPaletteThree->visible = false;
			}
			break;
		case 3:
			if(objectManager->selectedCharacters[3].demoObject != NULL)
			{
				objectManager->readyFour->visible = false;
				objectManager->selectPaletteLeftFour->visible = true;
				objectManager->selectPaletteRightFour->visible = true;
				objectManager->selectCharacterFour->visible = true;
				objectManager->selectPaletteFour->visible = false;
			}
			break;
		}
		break;
	case SELECTING_PALETTE:
		switch(player)
		{
		case 0:
			objectManager->readyOne->visible = false;
			objectManager->selectPaletteLeftOne->visible = true;
			objectManager->selectPaletteRightOne->visible = true;
			objectManager->selectCharacterOne->visible = false;
			objectManager->selectPaletteOne->visible = true;
			break;
		case 1:
			objectManager->readyTwo->visible = false;
			objectManager->selectPaletteLeftTwo->visible = true;
			objectManager->selectPaletteRightTwo->visible = true;
			objectManager->selectCharacterTwo->visible = false;
			objectManager->selectPaletteTwo->visible = true;
			break;
		case 2:
			if(objectManager->selectedCharacters[2].demoObject != NULL)
			{
				objectManager->readyThree->visible = false;
				objectManager->selectPaletteLeftThree->visible = true;
				objectManager->selectPaletteRightThree->visible = true;
				objectManager->selectCharacterThree->visible = false;
				objectManager->selectPaletteThree->visible = true;
			}
			break;
		case 3:
			if(objectManager->selectedCharacters[3].demoObject != NULL)
			{
				objectManager->readyFour->visible = false;
				objectManager->selectPaletteLeftFour->visible = true;
				objectManager->selectPaletteRightFour->visible = true;
				objectManager->selectCharacterFour->visible = false;
				objectManager->selectPaletteFour->visible = true;
			}
			break;
		}
		break;
	case READY:
		switch(player)
		{
		case 0:
			objectManager->readyOne->visible = true;
			objectManager->selectPaletteLeftOne->visible = false;
			objectManager->selectPaletteRightOne->visible = false;
			objectManager->selectCharacterOne->visible = false;
			objectManager->selectPaletteOne->visible = false;
			break;
		case 1:
			objectManager->readyTwo->visible = true;
			objectManager->selectPaletteRightTwo->visible = false;
			objectManager->selectPaletteLeftTwo->visible = false;
			objectManager->selectCharacterTwo->visible = false;
			objectManager->selectPaletteTwo->visible = false;
			break;
		case 2:
			if(objectManager->selectedCharacters[2].demoObject != NULL)
			{
				objectManager->readyThree->visible = true;
				objectManager->selectPaletteLeftThree->visible = false;
				objectManager->selectPaletteRightThree->visible = false;
				objectManager->selectCharacterThree->visible = false;
				objectManager->selectPaletteThree->visible = false;
			}
			break;
		case 3:
			if(objectManager->selectedCharacters[3].demoObject != NULL)
			{
				objectManager->readyFour->visible = true;
				objectManager->selectPaletteLeftFour->visible = false;
				objectManager->selectPaletteRightFour->visible = false;
				objectManager->selectCharacterFour->visible = false;
				objectManager->selectPaletteFour->visible = false;
			}
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
			case SELECTING_CHARACTER:
				objectManager->PreviousCharacter(player);
				break;
			case SELECTING_PALETTE:
				if(objectManager->selectedCharacters[player].demoObject != NULL)
				{
					objectManager->selectedCharacters[player].demoObject->PrevPalette();
				}
				break;
			case READY:
				break;
			}
			break;
		case STAGE_SELECT:
			objectManager->PreviousStage();
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
			case SELECTING_CHARACTER:
				objectManager->NextCharacter(player);
				break;
			case SELECTING_PALETTE:
				if(objectManager->selectedCharacters[player].demoObject != NULL)
				{
					objectManager->selectedCharacters[player].demoObject->NextPalette();
				}
				break;
			case READY:
				break;
			}
			break;
		case STAGE_SELECT:
			objectManager->NextStage();
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
			case SELECTING_CHARACTER:
				ChangeCharacterSelectPlayerState(SELECTING_PALETTE, player);
				break;
			case SELECTING_PALETTE:
				{
					objectManager->selectedPalettes[player] = objectManager->selectedCharacters[player].demoObject->GetPalette();
					bool searchFailed = true;
					while(searchFailed)
					{
						searchFailed = false;
						for(int i = 0; i < MAX_PLAYERS; i++)
						{
							if(i != player && characterSelectPlayerState[i] == READY && objectManager->selectedCharacters[player].defFilePath == objectManager->selectedCharacters[i].defFilePath && objectManager->selectedPalettes[player] == objectManager->selectedPalettes[i])
							{
								objectManager->selectedCharacters[player].demoObject->NextPalette();
								objectManager->selectedPalettes[player] = objectManager->selectedCharacters[player].demoObject->GetPalette();
								searchFailed = true;
								break;
							}
						}
					}
					ChangeCharacterSelectPlayerState(READY, player);
					
					bool ready = true;

					if(characterSelectPlayerState[0] != READY || characterSelectPlayerState[1] != READY)
					{
						ready = false;
					}

					if((gameType == FREE_FOR_ALL_3 || gameType == FREE_FOR_ALL_4) && characterSelectPlayerState[2] != READY)
					{
						ready = false;
					}

					if(gameType == FREE_FOR_ALL_4 && characterSelectPlayerState[3] != READY)
					{
						ready = false;
					}

					if(ready)
					{
						if(int i = ChangeCharacterSelectState(STAGE_SELECT) != 0) { return i; }
					}
					break;
				}
			case READY:
				break;
			}
			break;
		case STAGE_SELECT:
			if(int i = ChangeGameState(MATCH) != 0) { return i; }
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
			case SELECTING_CHARACTER:
				if(int i = ChangeGameState(MAIN_MENU) != 0) { return i; }
				break;
			case SELECTING_PALETTE:
				ChangeCharacterSelectPlayerState(SELECTING_CHARACTER, player);
				break;
			case READY:
				ChangeCharacterSelectPlayerState(SELECTING_PALETTE, player);
				break;
			}
			break;
		case STAGE_SELECT:
			ChangeCharacterSelectPlayerState(SELECTING_PALETTE, player);
			ChangeCharacterSelectState(PLAYERS_SELECTING);
			break;
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
	objectManager->LoadStage(objectManager->selectedStage.defFilePath);

	//load characters
	HSObject * fighterOne;
	if(int error = objectManager->LoadDefinition(objectManager->selectedCharacters[0].defFilePath, &objectManager->fighterObjects, &fighterOne) != 0) { return error; }
	fighterOne->pos.x = objectManager->spawnPoints[0].x;
	fighterOne->pos.y = objectManager->spawnPoints[0].y;
	objectManager->players[0] = fighterOne;
	fighterOne->SetPalette(objectManager->selectedPalettes[0]);
	((Fighter*)fighterOne)->state = STANDING;
	((Fighter*)fighterOne)->curHealth = ((Fighter*)fighterOne)->health;
	fighterOne->ChangeHold(((Fighter*)fighterOne)->fighterEventHolds.standing);
	objectManager->focusObject[0] = fighterOne;
	
	HSObject * fighterTwo;
	if(int error = objectManager->LoadDefinition(objectManager->selectedCharacters[1].defFilePath, &objectManager->fighterObjects, &fighterTwo) != 0) { return error; }
	fighterTwo->pos.x = objectManager->spawnPoints[1].x;
	fighterTwo->pos.y = objectManager->spawnPoints[1].y;
	objectManager->players[1] = fighterTwo;
	fighterTwo->SetPalette(objectManager->selectedPalettes[1]);
	((Fighter*)fighterTwo)->state = STANDING;
	((Fighter*)fighterTwo)->facing = LEFT;
	((Fighter*)fighterTwo)->curHealth = ((Fighter*)fighterTwo)->health;
	fighterTwo->ChangeHold(((Fighter*)fighterTwo)->fighterEventHolds.standing);
	objectManager->focusObject[1] = fighterTwo;

	if(gameType == FREE_FOR_ALL_3 || gameType == FREE_FOR_ALL_4)
	{
		HSObject * fighterThree;
		if(int error = objectManager->LoadDefinition(objectManager->selectedCharacters[2].defFilePath, &objectManager->fighterObjects, &fighterThree) != 0) { return error; }
		fighterThree->pos.x = objectManager->spawnPoints[2].x;
		fighterThree->pos.y = objectManager->spawnPoints[2].y;
		objectManager->players[2] = fighterThree;
		fighterThree->SetPalette(objectManager->selectedPalettes[2]);
		((Fighter*)fighterThree)->state = STANDING;
		((Fighter*)fighterThree)->curHealth = ((Fighter*)fighterThree)->health;
		fighterThree->ChangeHold(((Fighter*)fighterThree)->fighterEventHolds.standing);
		objectManager->focusObject[2] = fighterThree;
	}

	if(gameType == FREE_FOR_ALL_4)
	{
		HSObject * fighterFour;
		if(int error = objectManager->LoadDefinition(objectManager->selectedCharacters[3].defFilePath, &objectManager->fighterObjects, &fighterFour) != 0) { return error; }
		fighterFour->pos.x = objectManager->spawnPoints[3].x;
		fighterFour->pos.y = objectManager->spawnPoints[3].y;
		objectManager->players[3] = fighterFour;
		fighterFour->SetPalette(objectManager->selectedPalettes[3]);
		((Fighter*)fighterFour)->state = STANDING;
		((Fighter*)fighterFour)->facing = LEFT;
		((Fighter*)fighterFour)->curHealth = ((Fighter*)fighterFour)->health;
		fighterFour->ChangeHold(((Fighter*)fighterFour)->fighterEventHolds.standing);
		objectManager->focusObject[3] = fighterFour;
	}

	//load HUD
	HSObject * newHUD;
	if(int error = objectManager->LoadDefinition("data\\hud\\TestHUD\\john Hud.xml", &objectManager->HUDObjects, &newHUD) != 0) { return error; }
	objectManager->playerHUDs[0] = (HUD*)newHUD;
	((HUD*)newHUD)->pos.x = (MAX_GAME_RESOLUTION_X / -2) + 20;
	((HUD*)newHUD)->pos.y = (MAX_GAME_RESOLUTION_Y / -2) + 20;

	if(int error = objectManager->LoadDefinition("data\\hud\\TestHUD\\john Hud.xml", &objectManager->HUDObjects, &newHUD) != 0) { return error; }
	objectManager->playerHUDs[1] = (HUD*)newHUD;
	((HUD*)newHUD)->pos.x = (MAX_GAME_RESOLUTION_X / 2) - 560;
	((HUD*)newHUD)->pos.y = (MAX_GAME_RESOLUTION_Y / -2) + 20;
	((HUD*)newHUD)->comboCounterXPosition = COUNTER_RIGHT;

	if(gameType == FREE_FOR_ALL_3 || gameType == FREE_FOR_ALL_4)
	{
		if(int error = objectManager->LoadDefinition("data\\hud\\TestHUD\\john Hud.xml", &objectManager->HUDObjects, &newHUD) != 0) { return error; }
		objectManager->playerHUDs[2] = (HUD*)newHUD;
		((HUD*)newHUD)->pos.x = (MAX_GAME_RESOLUTION_X / -2) + 20;
		((HUD*)newHUD)->pos.y = (MAX_GAME_RESOLUTION_Y / -2) + 900;
		((HUD*)newHUD)->comboCounterYPosition = COUNTER_ABOVE;
	}

	if(gameType == FREE_FOR_ALL_4)
	{
		if(int error = objectManager->LoadDefinition("data\\hud\\TestHUD\\john Hud.xml", &objectManager->HUDObjects, &newHUD) != 0) { return error; }
		objectManager->playerHUDs[3] = (HUD*)newHUD;
		((HUD*)newHUD)->pos.x = (MAX_GAME_RESOLUTION_X / 2) - 560;
		((HUD*)newHUD)->pos.y = (MAX_GAME_RESOLUTION_Y / -2) + 900;
		((HUD*)newHUD)->comboCounterXPosition = COUNTER_RIGHT;
		((HUD*)newHUD)->comboCounterYPosition = COUNTER_ABOVE;
	}

	//load pause menu
	Menu * playerKeyConfigMenu = new Menu(MAIN_MENU_HEADER_HEIGHT, MAIN_MENU_CURSOR_WIDTH, MAIN_MENU_ITEM_HEIGHT, MAIN_MENU_ITEM_SPACING);
	playerKeyConfigMenu->pos.x = -382;
	playerKeyConfigMenu->pos.y = -352;

	HSObject * newObject;
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\cursor\\cursor.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	playerKeyConfigMenu->SetCursor(newObject);
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\player\\player1.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	playerKeyConfigMenu->SetHeader(newObject);
	objectManager->playerOne = newObject;
	newObject->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\player\\player2.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	objectManager->playerTwo = newObject;
	newObject->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\player\\player3.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	objectManager->playerThree = newObject;
	newObject->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\player\\player4.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	objectManager->playerFour = newObject;
	newObject->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\up\\up.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	playerKeyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\down\\down.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	playerKeyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\left\\left.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	playerKeyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\right\\right.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	playerKeyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\lightAttack\\lightAttack.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	playerKeyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\heavyAttack\\heavyAttack.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	playerKeyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\jump\\jump.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	playerKeyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\block\\block.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	playerKeyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\pause\\pause.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	playerKeyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\menuConfirm\\menuConfirm.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	playerKeyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\menuBack\\menuBack.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	playerKeyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\back\\back.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	playerKeyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));

	Menu * videoMenu = new Menu(MAIN_MENU_HEADER_HEIGHT, MAIN_MENU_CURSOR_WIDTH, MAIN_MENU_ITEM_HEIGHT, MAIN_MENU_ITEM_SPACING);
	videoMenu->pos.x = -382;
	videoMenu->pos.y = -352;

	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\cursor\\cursor.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	videoMenu->SetCursor(newObject);
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\video\\video.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	videoMenu->SetHeader(newObject);
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\fullscreen\\fullscreen.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	videoMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\stretchScreen\\stretchScreen.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	videoMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\resolution\\fullscreenResolution.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	videoMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\resolution\\windowedResolution.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	videoMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\apply\\apply.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	videoMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\back\\back.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	videoMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\yes\\yes.xml", &objectManager->HUDObjects, &objectManager->fullscreenYes) != 0) { return error; }
	objectManager->fullscreenYes->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->fullscreenYes->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING;
	objectManager->fullscreenYes->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\no\\no.xml", &objectManager->HUDObjects, &objectManager->fullscreenNo) != 0) { return error; }
	objectManager->fullscreenNo->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->fullscreenNo->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING;
	objectManager->fullscreenNo->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\yes\\yes.xml", &objectManager->HUDObjects, &objectManager->stretchYes) != 0) { return error; }
	objectManager->stretchYes->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->stretchYes->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 2 + MAIN_MENU_ITEM_HEIGHT;
	objectManager->stretchYes->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\no\\no.xml", &objectManager->HUDObjects, &objectManager->stretchNo) != 0) { return error; }
	objectManager->stretchNo->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->stretchNo->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 2 + MAIN_MENU_ITEM_HEIGHT;
	objectManager->stretchNo->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\640x360\\640x360.xml", &objectManager->HUDObjects, &objectManager->fullscreen640x360) != 0) { return error; }
	objectManager->fullscreen640x360->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->fullscreen640x360->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 3 + MAIN_MENU_ITEM_HEIGHT * 2;
	objectManager->fullscreen640x360->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\800x450\\800x450.xml", &objectManager->HUDObjects, &objectManager->fullscreen800x450) != 0) { return error; }
	objectManager->fullscreen800x450->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->fullscreen800x450->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 3 + MAIN_MENU_ITEM_HEIGHT * 2;
	objectManager->fullscreen800x450->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1024x576\\1024x576.xml", &objectManager->HUDObjects, &objectManager->fullscreen1024x576) != 0) { return error; }
	objectManager->fullscreen1024x576->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->fullscreen1024x576->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 3 + MAIN_MENU_ITEM_HEIGHT * 2;
	objectManager->fullscreen1024x576->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1152x648\\1152x648.xml", &objectManager->HUDObjects, &objectManager->fullscreen1152x648) != 0) { return error; }
	objectManager->fullscreen1152x648->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->fullscreen1152x648->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 3 + MAIN_MENU_ITEM_HEIGHT * 2;
	objectManager->fullscreen1152x648->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1280x720\\1280x720.xml", &objectManager->HUDObjects, &objectManager->fullscreen1280x720) != 0) { return error; }
	objectManager->fullscreen1280x720->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->fullscreen1280x720->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 3 + MAIN_MENU_ITEM_HEIGHT * 2;
	objectManager->fullscreen1280x720->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1360x765\\1360x765.xml", &objectManager->HUDObjects, &objectManager->fullscreen1360x765) != 0) { return error; }
	objectManager->fullscreen1360x765->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->fullscreen1360x765->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 3 + MAIN_MENU_ITEM_HEIGHT * 2;
	objectManager->fullscreen1360x765->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1366x768\\1366x768.xml", &objectManager->HUDObjects, &objectManager->fullscreen1366x768) != 0) { return error; }
	objectManager->fullscreen1366x768->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->fullscreen1366x768->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 3 + MAIN_MENU_ITEM_HEIGHT * 2;
	objectManager->fullscreen1366x768->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1400x787\\1400x787.xml", &objectManager->HUDObjects, &objectManager->fullscreen1400x787) != 0) { return error; }
	objectManager->fullscreen1400x787->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->fullscreen1400x787->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 3 + MAIN_MENU_ITEM_HEIGHT * 2;
	objectManager->fullscreen1400x787->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1440x810\\1440x810.xml", &objectManager->HUDObjects, &objectManager->fullscreen1440x810) != 0) { return error; }
	objectManager->fullscreen1440x810->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->fullscreen1440x810->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 3 + MAIN_MENU_ITEM_HEIGHT * 2;
	objectManager->fullscreen1440x810->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1600x900\\1600x900.xml", &objectManager->HUDObjects, &objectManager->fullscreen1600x900) != 0) { return error; }
	objectManager->fullscreen1600x900->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->fullscreen1600x900->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 3 + MAIN_MENU_ITEM_HEIGHT * 2;
	objectManager->fullscreen1600x900->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1680x945\\1680x945.xml", &objectManager->HUDObjects, &objectManager->fullscreen1680x945) != 0) { return error; }
	objectManager->fullscreen1680x945->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->fullscreen1680x945->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 3 + MAIN_MENU_ITEM_HEIGHT * 2;
	objectManager->fullscreen1680x945->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1920x1080\\1920x1080.xml", &objectManager->HUDObjects, &objectManager->fullscreen1920x1080) != 0) { return error; }
	objectManager->fullscreen1920x1080->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->fullscreen1920x1080->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 3 + MAIN_MENU_ITEM_HEIGHT * 2;
	objectManager->fullscreen1920x1080->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\640x360\\640x360.xml", &objectManager->HUDObjects, &objectManager->windowed640x360) != 0) { return error; }
	objectManager->windowed640x360->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->windowed640x360->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 4 + MAIN_MENU_ITEM_HEIGHT * 3;
	objectManager->windowed640x360->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\800x450\\800x450.xml", &objectManager->HUDObjects, &objectManager->windowed800x450) != 0) { return error; }
	objectManager->windowed800x450->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->windowed800x450->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 4 + MAIN_MENU_ITEM_HEIGHT * 3;
	objectManager->windowed800x450->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1024x576\\1024x576.xml", &objectManager->HUDObjects, &objectManager->windowed1024x576) != 0) { return error; }
	objectManager->windowed1024x576->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->windowed1024x576->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 4 + MAIN_MENU_ITEM_HEIGHT * 3;
	objectManager->windowed1024x576->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1152x648\\1152x648.xml", &objectManager->HUDObjects, &objectManager->windowed1152x648) != 0) { return error; }
	objectManager->windowed1152x648->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->windowed1152x648->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 4 + MAIN_MENU_ITEM_HEIGHT * 3;
	objectManager->windowed1152x648->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1280x720\\1280x720.xml", &objectManager->HUDObjects, &objectManager->windowed1280x720) != 0) { return error; }
	objectManager->windowed1280x720->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->windowed1280x720->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 4 + MAIN_MENU_ITEM_HEIGHT * 3;
	objectManager->windowed1280x720->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1360x765\\1360x765.xml", &objectManager->HUDObjects, &objectManager->windowed1360x765) != 0) { return error; }
	objectManager->windowed1360x765->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->windowed1360x765->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 4 + MAIN_MENU_ITEM_HEIGHT * 3;
	objectManager->windowed1360x765->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1366x768\\1366x768.xml", &objectManager->HUDObjects, &objectManager->windowed1366x768) != 0) { return error; }
	objectManager->windowed1366x768->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->windowed1366x768->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 4 + MAIN_MENU_ITEM_HEIGHT * 3;
	objectManager->windowed1366x768->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1400x787\\1400x787.xml", &objectManager->HUDObjects, &objectManager->windowed1400x787) != 0) { return error; }
	objectManager->windowed1400x787->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->windowed1400x787->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 4 + MAIN_MENU_ITEM_HEIGHT * 3;
	objectManager->windowed1400x787->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1440x810\\1440x810.xml", &objectManager->HUDObjects, &objectManager->windowed1440x810) != 0) { return error; }
	objectManager->windowed1440x810->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->windowed1440x810->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 4 + MAIN_MENU_ITEM_HEIGHT * 3;
	objectManager->windowed1440x810->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1600x900\\1600x900.xml", &objectManager->HUDObjects, &objectManager->windowed1600x900) != 0) { return error; }
	objectManager->windowed1600x900->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->windowed1600x900->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 4 + MAIN_MENU_ITEM_HEIGHT * 3;
	objectManager->windowed1600x900->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1680x945\\1680x945.xml", &objectManager->HUDObjects, &objectManager->windowed1680x945) != 0) { return error; }
	objectManager->windowed1680x945->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->windowed1680x945->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 4 + MAIN_MENU_ITEM_HEIGHT * 3;
	objectManager->windowed1680x945->visible = false;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\1920x1080\\1920x1080.xml", &objectManager->HUDObjects, &objectManager->windowed1920x1080) != 0) { return error; }
	objectManager->windowed1920x1080->pos.x = videoMenu->pos.x + VIDEO_SETTING_OFFSET_X;
	objectManager->windowed1920x1080->pos.y = videoMenu->pos.y + MAIN_MENU_HEADER_HEIGHT + MAIN_MENU_ITEM_SPACING * 4 + MAIN_MENU_ITEM_HEIGHT * 3;
	objectManager->windowed1920x1080->visible = false;
	
	Menu * keyConfigMenu = new Menu(MAIN_MENU_HEADER_HEIGHT, MAIN_MENU_CURSOR_WIDTH, MAIN_MENU_ITEM_HEIGHT, MAIN_MENU_ITEM_SPACING);
	keyConfigMenu->pos.x = -382;
	keyConfigMenu->pos.y = -352;

	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\cursor\\cursor.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	keyConfigMenu->SetCursor(newObject);
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\keyConfig\\keyConfig.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	keyConfigMenu->SetHeader(newObject);
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\player\\player1.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	keyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject, playerKeyConfigMenu));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\player\\player2.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	keyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject, playerKeyConfigMenu));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\player\\player3.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	keyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject, playerKeyConfigMenu));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\player\\player4.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	keyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject, playerKeyConfigMenu));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\back\\back.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	keyConfigMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	Menu * pauseMenu = new Menu(MAIN_MENU_HEADER_HEIGHT, MAIN_MENU_CURSOR_WIDTH, MAIN_MENU_ITEM_HEIGHT, MAIN_MENU_ITEM_SPACING);
	pauseMenu->pos.x = -382;
	pauseMenu->pos.y = -352;

	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\cursor\\cursor.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	pauseMenu->SetCursor(newObject);
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\pause\\pause.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	pauseMenu->SetHeader(newObject);
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\resume\\resumeMatch.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	pauseMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\video\\video.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	pauseMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject, videoMenu));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\keyConfig\\keyConfig.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	pauseMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject, keyConfigMenu));
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\quit\\quitMatch.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	pauseMenu->AddItem(objectManager->menuManager->MakeMenuItem(newObject));

	objectManager->menuManager = new MenuManager(pauseMenu);
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\pressDesiredButton\\pressDesiredButton.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	objectManager->pressDesiredButton = newObject;
	newObject->visible = false;

	//load win text
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\player\\player1.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	newObject->visible = false;
	newObject->pos.x = -82;
	newObject->pos.y = -45;
	objectManager->playerOne = newObject;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\player\\player2.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	newObject->visible = false;
	newObject->pos.x = -82;
	newObject->pos.y = -45;
	objectManager->playerTwo = newObject;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\player\\player3.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	newObject->visible = false;
	newObject->pos.x = -82;
	newObject->pos.y = -45;
	objectManager->playerThree = newObject;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\player\\player4.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	newObject->visible = false;
	newObject->pos.x = -82;
	newObject->pos.y = -45;
	objectManager->playerFour = newObject;
	
	if(int error = objectManager->LoadDefinition("data\\hud\\MainMenuGUI\\wins\\wins.xml", &objectManager->HUDObjects, &newObject) != 0) { return error; }
	newObject->visible = false;
	newObject->pos.x = -53;
	newObject->pos.y = 10;
	objectManager->wins = newObject;

	ChangeMatchState(IN_PROGRESS);

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		playerLives[i] = 3;
		ChangeMatchPlayerState(ACTIVE, i);
	}

	ChangePauseMenuState(PAUSE_TOP);

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
			objectManager->menuManager->Hide(true);
			break;
		case PAUSED:
			objectManager->menuManager->Hide(false);
			break;
		case RESULTS:
			if(matchPlayerState[0] == ACTIVE)
			{
				objectManager->playerOne->visible = true;
			}
			else if(matchPlayerState[1] == ACTIVE)
			{
				objectManager->playerTwo->visible = true;
			}
			objectManager->wins->visible = true;
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
			if(objectManager->playerHUDs[player] != NULL)
			{
				objectManager->playerHUDs[player]->SetHealthMeterValue(0);
				objectManager->playerHUDs[player]->SetLivesCounterValue(0);
				objectManager->playerHUDs[player]->SetHitsCounterValue(0);
			}
			if(objectManager->players[player] == NULL) { break; }
			objectManager->players[player]->visible = false;
			if(objectManager->players[player]->IsTerrainObject())
			{
				((TerrainObject*)objectManager->players[player])->curHealth = 0;
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
		objectManager->MakeVideoSettingsInvisible();
		break;
	case PAUSE_VIDEO:
		objectManager->fullScreenToApply = objectManager->fullScreen;
		objectManager->stretchScreenToApply = objectManager->stretchScreen;
		objectManager->fullscreenResolutionXToApply = objectManager->fullscreenResolutionX;
		objectManager->fullscreenResolutionYToApply = objectManager->fullscreenResolutionY;
		objectManager->windowedResolutionXToApply = objectManager->windowedResolutionX;
		objectManager->windowedResolutionYToApply = objectManager->windowedResolutionY;

		objectManager->SetVideoSettingVisibility();
		break;
	case PAUSE_KEY_CONFIG:
		break;
	case PAUSE_PLAYER_KEY_CONFIG:
		objectManager->pressDesiredButton->visible = false;
		switch(playerToSetUp)
		{
		case 0:
			objectManager->menuManager->SetHeader(objectManager->playerOne);
			break;
		case 1:
			objectManager->menuManager->SetHeader(objectManager->playerTwo);
			break;
		case 2:
			objectManager->menuManager->SetHeader(objectManager->playerThree);
			break;
		case 3:
			objectManager->menuManager->SetHeader(objectManager->playerFour);
			break;
		}
		break;
	case PAUSE_INPUT_KEY:
		objectManager->pressDesiredButton->pos.x = objectManager->menuManager->GetCursorPos().x + PRESS_DESIRED_BUTTON_OFFSET_X;
		objectManager->pressDesiredButton->pos.y = objectManager->menuManager->GetCursorPos().y;
		objectManager->pressDesiredButton->visible = true;
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
		else if(objectManager->players[player] != 0)
		{
			objectManager->players[player]->Event(inputHistory, frame);
		}
		break;
	case PAUSED:
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
			switch(pauseMenuState)
			{
			case PAUSE_VIDEO:
				switch(objectManager->menuManager->GetCursorIndex())
				{
				case 0:
					if(objectManager->fullScreenToApply) { objectManager->fullScreenToApply = false; }
					else { objectManager->fullScreenToApply = true; }
					objectManager->SetVideoSettingVisibility();
					break;
				case 1:
					if(objectManager->stretchScreenToApply) { objectManager->stretchScreenToApply = false; }
					else { objectManager->stretchScreenToApply = true; }
					objectManager->SetVideoSettingVisibility();
					break;
				case 2:
					objectManager->PrevFullscreenResolution();
					objectManager->SetVideoSettingVisibility();
					break;
				case 3:
					objectManager->PrevWindowedResolution();
					objectManager->SetVideoSettingVisibility();
					break;
				}
				break;
			}
		}
		else if(inputHistory->frame == frame && (inputHistory->bButtonRight.pressed || inputHistory->bHatRight.pressed || inputHistory->bKeyRight.pressed || inputHistory->bStickRight.pressed))
		{
			switch(pauseMenuState)
			{
			case PAUSE_VIDEO:
				switch(objectManager->menuManager->GetCursorIndex())
				{
				case 0:
					if(objectManager->fullScreenToApply) { objectManager->fullScreenToApply = false; }
					else { objectManager->fullScreenToApply = true; }
					objectManager->SetVideoSettingVisibility();
					break;
				case 1:
					if(objectManager->stretchScreenToApply) { objectManager->stretchScreenToApply = false; }
					else { objectManager->stretchScreenToApply = true; }
					objectManager->SetVideoSettingVisibility();
					break;
				case 2:
					objectManager->NextFullscreenResolution();
					objectManager->SetVideoSettingVisibility();
					break;
				case 3:
					objectManager->NextWindowedResolution();
					objectManager->SetVideoSettingVisibility();
					break;
				}
				break;
			}
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
				switch(objectManager->menuManager->GetCursorIndex())
				{
				case 0:
					ChangeMatchState(IN_PROGRESS);
					break;
				case 1:
					objectManager->menuManager->ToChild();
					ChangePauseMenuState(PAUSE_VIDEO);
					break;
				case 2:
					objectManager->menuManager->ToChild();
					ChangePauseMenuState(PAUSE_KEY_CONFIG);
					break;
				case 3:
					if(int i = ChangeGameState(MAIN_MENU) != 0) { return i; }
					break;
				}
				break;
			case PAUSE_VIDEO:
				switch(objectManager->menuManager->GetCursorIndex())
				{
				case 0:
					if(objectManager->fullScreenToApply) { objectManager->fullScreenToApply = false; }
					else { objectManager->fullScreenToApply = true; }
					objectManager->SetVideoSettingVisibility();
					break;
				case 1:
					if(objectManager->stretchScreenToApply) { objectManager->stretchScreenToApply = false; }
					else { objectManager->stretchScreenToApply = true; }
					objectManager->SetVideoSettingVisibility();
					break;
				case 2:
					objectManager->NextFullscreenResolution();
					objectManager->SetVideoSettingVisibility();
					break;
				case 3:
					objectManager->NextWindowedResolution();
					objectManager->SetVideoSettingVisibility();
					break;
				case 4:
					if(int error = objectManager->ApplyVideoSettings() != 0) { return error; }
					objectManager->SetVideoSettingVisibility();
					break;
				case 5:
					objectManager->menuManager->ToParent();
					ChangePauseMenuState(PAUSE_TOP);
					break;
				}
				break;
			case PAUSE_KEY_CONFIG:
				switch(objectManager->menuManager->GetCursorIndex())
				{
				case 0:
				case 1:
				case 2:
				case 3:
					playerToSetUp = objectManager->menuManager->GetCursorIndex();
					objectManager->menuManager->ToChild();
					ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
					break;
				case 4:
					objectManager->menuManager->ToParent();
					ChangePauseMenuState(PAUSE_TOP);
					break;
				}
				break;
			case PAUSE_PLAYER_KEY_CONFIG:
				switch(objectManager->menuManager->GetCursorIndex())
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
					keyToSetUp = objectManager->menuManager->GetCursorIndex();
					ChangePauseMenuState(PAUSE_INPUT_KEY);
					break;
				case 11:
					playerToSetUp = -1;
					objectManager->menuManager->ToParent();
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
			case PAUSE_VIDEO:
				objectManager->menuManager->ToParent();
				ChangePauseMenuState(PAUSE_TOP);
			case PAUSE_KEY_CONFIG:
				objectManager->menuManager->ToParent();
				ChangePauseMenuState(PAUSE_TOP);
				break;
			case PAUSE_PLAYER_KEY_CONFIG:
				playerToSetUp = -1;
				objectManager->menuManager->ToParent();
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
		if(objectManager->players[i] == NULL) { ChangeMatchPlayerState(LOST, i); continue; }

		if(matchPlayerState[i] == LOST) { continue; }

		if((!objectManager->players[i]->IsTerrain() && objectManager->players[i]->pos.y > (objectManager->stageSize.y / 2) + RINGOUT_BUFFER) || (objectManager->players[i]->IsTerrainObject() && ((TerrainObject*)objectManager->players[i])->curHealth <= 0 &&
			(!objectManager->players[i]->IsFighter() || ((Fighter*)objectManager->players[i])->state != KNOCKOUT && ((Fighter*)objectManager->players[i])->state != KNOCKOUT_AIR)))
		{
			playerLives[i]--;

			if(playerLives[i] <= 0)
			{
				if(int error = ChangeMatchPlayerState(LOST, i) != 0) { return error; }
				objectManager->players[i]->toDelete = true;
				continue;
			}

			objectManager->players[i]->pos.x = objectManager->spawnPoints[i].x;
			objectManager->players[i]->pos.y = objectManager->spawnPoints[i].y;
			objectManager->players[i]->vel.x = 0;
			objectManager->players[i]->vel.y = 0;

			if(objectManager->players[i]->IsTerrainObject())
			{
				((TerrainObject*)objectManager->players[i])->curHealth = ((TerrainObject*)objectManager->players[i])->health;
			}

			if(objectManager->players[i]->IsFighter())
			{
				((Fighter*)objectManager->players[i])->ChangeHold(((Fighter*)objectManager->players[i])->fighterEventHolds.jumpNeutralFall);
				((Fighter*)objectManager->players[i])->state = JUMPING;
				((Fighter*)objectManager->players[i])->falls = true;
			}
		}

		if(objectManager->playerHUDs[i] != NULL)
		{
			float curHealth = ((TerrainObject*)objectManager->players[i])->curHealth;
			float health = ((TerrainObject*)objectManager->players[i])->health;
			objectManager->playerHUDs[i]->SetHealthMeterValue(curHealth / health);
			objectManager->playerHUDs[i]->SetLivesCounterValue(playerLives[i]);

			if(objectManager->players[i]->IsFighter())
			{
				Fighter * fighter = (Fighter*) objectManager->players[i];

				int hits = 0;
				list<ComboTrack>::iterator comboItr;
				for(comboItr = fighter->comboTrack.begin(); comboItr != fighter->comboTrack.end(); comboItr++)
				{
					hits += comboItr->hits;
				}

				objectManager->playerHUDs[i]->SetHitsCounterValue(hits);
			}
		}
		
		survivingPlayers++;
	}

	if(survivingPlayers <= 1)
	{
		if(int i = ChangeMatchState(RESULTS) != 0) { return i; }
	}

	return 0;
}

void Main::DefaultKeyConfig()
{
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
	mappings[1].keyJump = SDLK_KP_5;
	mappings[1].keyLight = SDLK_KP_4;
	mappings[1].keyHeavy = SDLK_KP_1;
	mappings[1].keyBlock = SDLK_KP_6;
	mappings[1].keyStart = SDLK_KP_PLUS;
	mappings[1].keyMenuConfirm = mappings[1].keyLight;
	mappings[1].keyMenuBack = mappings[1].keyJump;
}

int Main::LoadKeyConfig()
{
	//get the XML structure from the file
	XMLDocument * file = new XMLDocument();
	if(int error = file->LoadFile("data\\config\\keyConfig.xml") != 0)
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

int Main::SaveKeyConfig()
{
	//create a new xml structure
	XMLDocument * file = new XMLDocument();

	XMLElement * playerOne = file->NewElement("PlayerOne");
	SetPlayerConfig(playerOne, 0);

	XMLElement * playerTwo = file->NewElement("PlayerTwo");
	SetPlayerConfig(playerTwo, 1);

	XMLElement * playerThree = file->NewElement("PlayerThree");
	SetPlayerConfig(playerThree, 2);

	XMLElement * playerFour = file->NewElement("PlayerFour");
	SetPlayerConfig(playerFour, 3);

	XMLElement * config = file->NewElement("KeyConfiguration");
	config->InsertEndChild(playerOne);
	config->InsertEndChild(playerTwo);
	config->InsertEndChild(playerThree);
	config->InsertEndChild(playerFour);

	file->InsertEndChild(config);

	file->SaveFile("data\\config\\keyConfig.xml");

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
	list<HSObject*>::iterator objIt;
	for ( objIt=objectManager->HUDObjects.begin(); objIt != objectManager->HUDObjects.end(); objIt++)
	{
		if(int error = (*objIt)->AdvanceHolds() != 0)
		{
			return error; //there was an error advancing holds
		}
	}

	if(gameState == MATCH && matchState != IN_PROGRESS) { return 0; }

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
		if(mappings[i].stick == which)
		{
			mappings[i].stick = JOYSTICK_UNKNOWN;
		}
	}
}

void Main::ClearHatForAllPlayers(Uint8 which)
{
	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(mappings[i].hat == which)
		{
			mappings[i].hat = JOYSTICK_UNKNOWN;
		}
	}
}

void Main::ClearButtonForAllPlayers(Uint8 which, Uint8 button, int playerToIgnore)
{
	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(i == playerToIgnore) { continue; }

		if(mappings[i].buttonUp.joystick == which && mappings[i].buttonUp.button == button)
		{
			mappings[i].buttonUp.joystick = JOYSTICK_UNKNOWN; mappings[i].buttonUp.button = JOYBUTTON_UNKNOWN;
		}
		if(mappings[i].buttonDown.joystick == which && mappings[i].buttonDown.button == button)
		{
			mappings[i].buttonDown.joystick = JOYSTICK_UNKNOWN; mappings[i].buttonDown.button = JOYBUTTON_UNKNOWN;
		}
		if(mappings[i].buttonLeft.joystick == which && mappings[i].buttonLeft.button == button)
		{
			mappings[i].buttonLeft.joystick = JOYSTICK_UNKNOWN; mappings[i].buttonLeft.button = JOYBUTTON_UNKNOWN;
		}
		if(mappings[i].buttonRight.joystick == which && mappings[i].buttonRight.button == button)
		{
			mappings[i].buttonRight.joystick = JOYSTICK_UNKNOWN; mappings[i].buttonRight.button = JOYBUTTON_UNKNOWN;
		}
		if(mappings[i].buttonJump.joystick == which && mappings[i].buttonJump.button == button)
		{
			mappings[i].buttonJump.joystick = JOYSTICK_UNKNOWN; mappings[i].buttonJump.button = JOYBUTTON_UNKNOWN;
		}
		if(mappings[i].buttonLight.joystick == which && mappings[i].buttonLight.button == button)
		{
			mappings[i].buttonLight.joystick = JOYSTICK_UNKNOWN; mappings[i].buttonLight.button = JOYBUTTON_UNKNOWN;
		}
		if(mappings[i].buttonHeavy.joystick == which && mappings[i].buttonHeavy.button == button)
		{
			mappings[i].buttonHeavy.joystick = JOYSTICK_UNKNOWN; mappings[i].buttonHeavy.button = JOYBUTTON_UNKNOWN;
		}
		if(mappings[i].buttonBlock.joystick == which && mappings[i].buttonBlock.button == button)
		{
			mappings[i].buttonBlock.joystick = JOYSTICK_UNKNOWN; mappings[i].buttonBlock.button = JOYBUTTON_UNKNOWN;
		}
		if(mappings[i].buttonStart.joystick == which && mappings[i].buttonStart.button == button)
		{
			mappings[i].buttonStart.joystick = JOYSTICK_UNKNOWN; mappings[i].buttonStart.button = JOYBUTTON_UNKNOWN;
		}
		if(mappings[i].buttonMenuConfirm.joystick == which && mappings[i].buttonMenuConfirm.button == button)
		{
			mappings[i].buttonMenuConfirm.joystick = JOYSTICK_UNKNOWN; mappings[i].buttonMenuConfirm.button = JOYBUTTON_UNKNOWN;
		}
		if(mappings[i].buttonMenuBack.joystick == which && mappings[i].buttonMenuBack.button == button)
		{
			mappings[i].buttonMenuBack.joystick = JOYSTICK_UNKNOWN; mappings[i].buttonMenuBack.button = JOYBUTTON_UNKNOWN;
		}
	}
}

void Main::ClearKeyForAllPlayers(SDL_Keycode sym, int playerToIgnore)
{
	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(i == playerToIgnore) { continue; }

		if(mappings[i].keyUp == sym) { mappings[i].keyUp = SDLK_UNKNOWN; }
		if(mappings[i].keyDown == sym) { mappings[i].keyDown = SDLK_UNKNOWN; }
		if(mappings[i].keyLeft == sym) { mappings[i].keyLeft = SDLK_UNKNOWN; }
		if(mappings[i].keyRight == sym) { mappings[i].keyRight = SDLK_UNKNOWN; }
		if(mappings[i].keyJump == sym) { mappings[i].keyJump = SDLK_UNKNOWN; }
		if(mappings[i].keyLight == sym) { mappings[i].keyLight = SDLK_UNKNOWN; }
		if(mappings[i].keyHeavy == sym) { mappings[i].keyHeavy = SDLK_UNKNOWN; }
		if(mappings[i].keyBlock == sym) { mappings[i].keyBlock = SDLK_UNKNOWN; }
		if(mappings[i].keyStart == sym) { mappings[i].keyStart = SDLK_UNKNOWN; }
		if(mappings[i].keyMenuConfirm == sym) { mappings[i].keyMenuConfirm = SDLK_UNKNOWN; }
		if(mappings[i].keyMenuBack == sym) { mappings[i].keyMenuBack = SDLK_UNKNOWN; }
	}
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
	mappings[player].keyMenuBack = SDLK_UNKNOWN;

	mappings[player].hat = JOYSTICK_UNKNOWN;
	mappings[player].stick = JOYSTICK_UNKNOWN;
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
	if(mainMenuState == INPUT_KEY || pauseMenuState == PAUSE_INPUT_KEY)
	{
		ClearKeyForAllPlayers(sym, playerToSetUp);
		switch(keyToSetUp)
		{
		case 0: //up
			mappings[playerToSetUp].keyUp = sym;
			mappings[playerToSetUp].stick = -1;
			mappings[playerToSetUp].hat = -1;
			mappings[playerToSetUp].buttonUp.joystick = -1;
			mappings[playerToSetUp].buttonUp.button = -1;
			keyToSetUp = -1;
			objectManager->menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveKeyConfig();
			break;
		case 1: //down
			mappings[playerToSetUp].keyDown = sym;
			mappings[playerToSetUp].stick = -1;
			mappings[playerToSetUp].hat = -1;
			mappings[playerToSetUp].buttonDown.joystick = -1;
			mappings[playerToSetUp].buttonDown.button = -1;
			keyToSetUp = -1;
			objectManager->menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveKeyConfig();
			break;
		case 2: //left
			mappings[playerToSetUp].keyLeft = sym;
			mappings[playerToSetUp].stick = -1;
			mappings[playerToSetUp].hat = -1;
			mappings[playerToSetUp].buttonLeft.joystick = -1;
			mappings[playerToSetUp].buttonLeft.button = -1;
			keyToSetUp = -1;
			objectManager->menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveKeyConfig();
			break;
		case 3: //right
			mappings[playerToSetUp].keyRight = sym;
			mappings[playerToSetUp].stick = -1;
			mappings[playerToSetUp].hat = -1;
			mappings[playerToSetUp].buttonRight.joystick = -1;
			mappings[playerToSetUp].buttonRight.button = -1;
			keyToSetUp = -1;
			objectManager->menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveKeyConfig();
			break;
		case 4: //light attack
			mappings[playerToSetUp].keyLight = sym;
			mappings[playerToSetUp].buttonLight.joystick = -1;
			mappings[playerToSetUp].buttonLight.button = -1;
			keyToSetUp = -1;
			objectManager->menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveKeyConfig();
			break;
		case 5: //heavy attack
			mappings[playerToSetUp].keyHeavy = sym;
			mappings[playerToSetUp].buttonHeavy.joystick = -1;
			mappings[playerToSetUp].buttonHeavy.button = -1;
			keyToSetUp = -1;
			objectManager->menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveKeyConfig();
			break;
		case 6: //jump
			mappings[playerToSetUp].keyJump = sym;
			mappings[playerToSetUp].buttonJump.joystick = -1;
			mappings[playerToSetUp].buttonJump.button = -1;
			keyToSetUp = -1;
			objectManager->menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveKeyConfig();
			break;
		case 7: //block
			mappings[playerToSetUp].keyBlock = sym;
			mappings[playerToSetUp].buttonBlock.joystick = -1;
			mappings[playerToSetUp].buttonBlock.button = -1;
			keyToSetUp = -1;
			objectManager->menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveKeyConfig();
			break;
		case 8: //pause
			mappings[playerToSetUp].keyStart = sym;
			mappings[playerToSetUp].buttonStart.joystick = -1;
			mappings[playerToSetUp].buttonStart.button = -1;
			keyToSetUp = -1;
			objectManager->menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveKeyConfig();
			break;
		case 9: //menu confirm
			mappings[playerToSetUp].keyMenuConfirm = sym;
			mappings[playerToSetUp].buttonMenuConfirm.joystick = -1;
			mappings[playerToSetUp].buttonMenuConfirm.button = -1;
			keyToSetUp = -1;
			objectManager->menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveKeyConfig();
			break;
		case 10: //menu back
			mappings[playerToSetUp].keyMenuBack = sym;
			mappings[playerToSetUp].buttonMenuBack.joystick = -1;
			mappings[playerToSetUp].buttonMenuBack.button = -1;
			keyToSetUp = -1;
			objectManager->menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
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

void Main::KeyUp(SDL_Keycode sym) 
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
			objectManager->menuManager->CursorNext();
		case 1: //down
			objectManager->menuManager->CursorNext();
		case 2: //left
			objectManager->menuManager->CursorNext();
		case 3: //right
			ClearStickForAllPlayers(which);
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
			objectManager->menuManager->CursorNext();
			keyToSetUp = -1;
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
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
	if(mainMenuState == INPUT_KEY || pauseMenuState == PAUSE_INPUT_KEY)
	{
		switch(keyToSetUp)
		{
		case 0: //up
			objectManager->menuManager->CursorNext();
		case 1: //down
			objectManager->menuManager->CursorNext();
		case 2: //left
			objectManager->menuManager->CursorNext();
		case 3: //right
			ClearHatForAllPlayers(which);
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
			objectManager->menuManager->CursorNext();
			keyToSetUp = -1;
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
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
		ClearButtonForAllPlayers(which, button, playerToSetUp);
		switch(keyToSetUp)
		{
		case 0: //up
			mappings[playerToSetUp].buttonUp.joystick = which; mappings[playerToSetUp].buttonUp.button = button;
			mappings[playerToSetUp].stick = -1;
			mappings[playerToSetUp].hat = -1;
			mappings[playerToSetUp].keyUp = SDLK_UNKNOWN;
			keyToSetUp = -1;
			objectManager->menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveKeyConfig();
			break;
		case 1: //down
			mappings[playerToSetUp].buttonDown.joystick = which; mappings[playerToSetUp].buttonDown.button = button;
			mappings[playerToSetUp].stick = -1;
			mappings[playerToSetUp].hat = -1;
			mappings[playerToSetUp].keyDown = SDLK_UNKNOWN;
			keyToSetUp = -1;
			objectManager->menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveKeyConfig();
			break;
		case 2: //left
			mappings[playerToSetUp].buttonLeft.joystick = which; mappings[playerToSetUp].buttonLeft.button = button;
			mappings[playerToSetUp].stick = -1;
			mappings[playerToSetUp].hat = -1;
			mappings[playerToSetUp].keyLeft = SDLK_UNKNOWN;
			keyToSetUp = -1;
			objectManager->menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveKeyConfig();
			break;
		case 3: //right
			mappings[playerToSetUp].buttonRight.joystick = which; mappings[playerToSetUp].buttonRight.button = button;
			mappings[playerToSetUp].stick = -1;
			mappings[playerToSetUp].hat = -1;
			mappings[playerToSetUp].keyRight = SDLK_UNKNOWN;
			keyToSetUp = -1;
			objectManager->menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveKeyConfig();
			break;
		case 4: //light attack
			mappings[playerToSetUp].buttonLight.joystick = which; mappings[playerToSetUp].buttonLight.button = button;
			mappings[playerToSetUp].keyLight = SDLK_UNKNOWN;
			keyToSetUp = -1;
			objectManager->menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveKeyConfig();
			break;
		case 5: //heavy attack
			mappings[playerToSetUp].buttonHeavy.joystick = which; mappings[playerToSetUp].buttonHeavy.button = button;
			mappings[playerToSetUp].keyHeavy = SDLK_UNKNOWN;
			keyToSetUp = -1;
			objectManager->menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveKeyConfig();
			break;
		case 6: //jump
			mappings[playerToSetUp].buttonJump.joystick = which; mappings[playerToSetUp].buttonJump.button = button;
			mappings[playerToSetUp].keyJump = SDLK_UNKNOWN;
			keyToSetUp = -1;
			objectManager->menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveKeyConfig();
			break;
		case 7: //block
			mappings[playerToSetUp].buttonBlock.joystick = which; mappings[playerToSetUp].buttonBlock.button = button;
			mappings[playerToSetUp].keyBlock = SDLK_UNKNOWN;
			keyToSetUp = -1;
			objectManager->menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveKeyConfig();
			break;
		case 8: //pause
			mappings[playerToSetUp].buttonStart.joystick = which; mappings[playerToSetUp].buttonStart.button = button;
			mappings[playerToSetUp].keyStart = SDLK_UNKNOWN;
			keyToSetUp = -1;
			objectManager->menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveKeyConfig();
			break;
		case 9: //menu confirm
			mappings[playerToSetUp].buttonMenuConfirm.joystick = which; mappings[playerToSetUp].buttonMenuConfirm.button = button;
			mappings[playerToSetUp].keyMenuConfirm = SDLK_UNKNOWN;
			keyToSetUp = -1;
			objectManager->menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveKeyConfig();
			break;
		case 10: //menu back
			mappings[playerToSetUp].buttonMenuBack.joystick = which; mappings[playerToSetUp].buttonMenuBack.button = button;
			mappings[playerToSetUp].keyMenuBack = SDLK_UNKNOWN;
			keyToSetUp = -1;
			objectManager->menuManager->CursorNext();
			if(gameState == MAIN_MENU)
			{
				ChangeMainMenuState(PLAYER_KEY_CONFIG);
			}
			else if(gameState == MATCH)
			{
				ChangePauseMenuState(PAUSE_PLAYER_KEY_CONFIG);
			}
			SaveKeyConfig();
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