#ifndef __OBJECTMANAGER_H_
#define __OBJECTMANAGER_H_

#include <iostream>
#include <fstream>
#include "fighter.h"
#include "HUD.h"
#include "menu.h"
#include "menuManager.h"

#define MAX_PLAYERS 4

struct CurrentAudioEntry
{
	HSAudio* aud; //audio object
	Uint32 curPosition; //current position in the audio playthrough
	HSObject * sourceObj; //object that triggered the audio
	bool exclusive; //whether or not the object can be cut off by other exclusive sounds from the same object
};

extern list<CurrentAudioEntry*> currentAudio;

class ObjectManager
{
public:
	ObjectManager();

	unsigned int newObjectId;

	bool gameObjectsNeedSort;
	bool HUDObjectsNeedSort;
	list<HSObject*> gameObjects;
	list<HSObject*> HUDObjects;
	list<HSTexture*> textureRegistry;
	list<HSPalette*> paletteRegistry;
	list<HSAudio*> audioRegistry;
	bool openGL3;
	SDL_AudioSpec * obtainedAudioSpec;
	
	HSVect2D stageSize;
	float gravity;
	
	HSVect2D spawnPoints[MAX_PLAYERS];
	HUD * playerHUDs[MAX_PLAYERS];
	HSObject * players[MAX_PLAYERS];
	HSObject * focusObjectOne;
	HSObject * focusObjectTwo;

	MenuManager * menuManager;

	HSObject * loading;
	HSObject * playerOne;
	HSObject * playerTwo;
	HSObject * playerThree;
	HSObject * playerFour;
	HSObject * wins;
	HSObject * pressDesiredButton;
	HSObject * readyOne;
	HSObject * readyTwo;
	HSObject * selectPaletteOne;
	HSObject * selectPaletteTwo;
	HSObject * selectPaletteLeftOne;
	HSObject * selectPaletteLeftTwo;
	HSObject * selectPaletteRightOne;
	HSObject * selectPaletteRightTwo;

	void SortAllObjects();
	
	int LoadDefinition(string defFilePath, list<HSObject*> * objects, HSObject ** returnValue = NULL);
	int LoadStage(string defFilePath);

	int CloneObject(SpawnObject * objectToClone, list<HSObject*> * objects, HSObject ** returnValue = NULL);
	int CloneObject(HSObject * objectToClone, list<HSObject*> * objects, HSObject ** returnValue = NULL);

	int ClearAllObjects();
	int ClearSpecificObject(HSObject* object);

protected:
};

#endif