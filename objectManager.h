#ifndef __OBJECTMANAGER_H_
#define __OBJECTMANAGER_H_

#include <iostream>
#include <fstream>
#include "fighter.h"
#include "HUD.h"
#include "menu.h"
#include "menuManager.h"

#define MAX_PLAYERS 4
#define RINGOUT_BUFFER 500 //how far below the bottom of the stage something must fall before its considered "ringed out"

struct CurrentAudioEntry
{
	HSAudio* aud; //audio object
	Uint32 curPosition; //current position in the audio playthrough
	HSObject * sourceObj; //object that triggered the audio
	bool exclusive; //whether or not the object can be cut off by other exclusive sounds from the same object
};

struct PlayableCharacter
{
	string defFilePath;
	HSObject * demoObject;
};

struct PlayableStage
{
	string defFilePath;
	HSObject * demoObject;
};

extern list<CurrentAudioEntry*> currentAudio;

class ObjectManager
{
public:
	ObjectManager();

	unsigned int newObjectId;

	bool stageObjectsNeedSort;
	bool BGSpawnedObjectsNeedSort;
	bool fighterObjectsNeedSort;
	bool FGSpawnedObjectsNeedSort;
	bool HUDObjectsNeedSort;
	list<HSObject*> stageObjects; //for the background
	list<HSObject*> BGSpawnedObjects; //for things spawned by the player characters, which go behind them
	list<HSObject*> fighterObjects; //for the acutal player characters
	list<HSObject*> FGSpawnedObjects; //for things spawned by the player characters, which go in front of them
	list<HSObject*> HUDObjects; //for the HUD
	list<HSTexture*> textureRegistry;
	list<HSPalette*> paletteRegistry;
	list<HSAudio*> audioRegistry;
	bool openGL3;
	SDL_AudioSpec * obtainedAudioSpec;
	
	HSVect2D stageSize;
	
	HSVect2D spawnPoints[MAX_PLAYERS];
	HUD * playerHUDs[MAX_PLAYERS];
	HSObject * players[MAX_PLAYERS];
	HSObject * focusObject[MAX_PLAYERS];

	PlayableCharacter selectedCharacters[MAX_PLAYERS];
	int selectedPalettes[MAX_PLAYERS];
	PlayableStage selectedStage;

	MenuManager * menuManager;

	list<PlayableCharacter> characterList[MAX_PLAYERS];
	list<PlayableStage> stageList;
	HSObject * loading;
	HSObject * characterSelect;
	HSObject * playerOne;
	HSObject * playerTwo;
	HSObject * playerThree;
	HSObject * playerFour;
	HSObject * wins;
	HSObject * pressDesiredButton;
	HSObject * readyOne;
	HSObject * readyTwo;
	HSObject * readyThree;
	HSObject * readyFour;
	HSObject * selectCharacterOne;
	HSObject * selectCharacterTwo;
	HSObject * selectCharacterThree;
	HSObject * selectCharacterFour;
	HSObject * selectPaletteOne;
	HSObject * selectPaletteTwo;
	HSObject * selectPaletteThree;
	HSObject * selectPaletteFour;
	HSObject * selectPaletteLeftOne;
	HSObject * selectPaletteLeftTwo;
	HSObject * selectPaletteLeftThree;
	HSObject * selectPaletteLeftFour;
	HSObject * selectPaletteRightOne;
	HSObject * selectPaletteRightTwo;
	HSObject * selectPaletteRightThree;
	HSObject * selectPaletteRightFour;
	HSObject * stageSelect;
	HSObject * selectStage;
	HSObject * selectStageLeft;
	HSObject * selectStageRight;

	void SortAllObjects();

	int LoadDefinition(string defFilePath, list<HSObject*> * objects, HSObject ** returnValue = NULL);
	int LoadStage(string defFilePath);
	int LoadPlayableCharacters(bool loadPlayer[MAX_PLAYERS]);
	void PreviousCharacter(int player);
	void NextCharacter(int player);
	int LoadPlayableStages();
	void PreviousStage();
	void NextStage();

	int CloneObject(SpawnObject * objectToClone, list<HSObject*> * objects, HSObject ** returnValue = NULL);
	int CloneObject(HSObject * objectToClone, list<HSObject*> * objects, HSObject ** returnValue = NULL);

	int ClearAllObjects();
	int ClearSpecificObject(HSObject* object);

protected:
};

#endif