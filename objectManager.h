#ifndef __OBJECTMANAGER_H_
#define __OBJECTMANAGER_H_

#include <iostream>
#include <fstream>
#include "fighter.h"
#include "HUD.h"
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
	
	bool notDone;
	SDL_Thread * renderingThread;
	bool doRender;
	bool applyVideoSettings;
	bool centerCameraInstantly;
	bool matchCamera;
	bool loadTexturesAndPalettes;
	bool clearTexturesAndPalettes;
	int renderingErrorCode;
	
	list<GLuint> texturesToDelete;
	list<GLuint> buffersToDelete;
	list<GLuint> vaosToDelete;
	list<GLuint> palettesToDelete;

	SDL_mutex * vidInitLock;
	SDL_cond * vidInitDone;

	SDL_mutex * renderLock;
	SDL_cond * renderDone;

	SDL_mutex * processLock;
	SDL_cond * processDone;
	
	bool fullScreen;
	bool fullScreenToApply;
	bool stretchScreen;
	bool stretchScreenToApply;
	int windowedResolutionX;
	int windowedResolutionY;
	int windowedResolutionXToApply;
	int windowedResolutionYToApply;
	int fullscreenResolutionX;
	int fullscreenResolutionY;
	int fullscreenResolutionXToApply;
	int fullscreenResolutionYToApply;
	bool needApplyWindowedResolutionForMenu;
	int windowedResolutionXForMenu;
	int windowedResolutionYForMenu;
	bool needApplyFullscreenResolutionForMenu;
	int fullscreenResolutionXForMenu;
	int fullscreenResolutionYForMenu;

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
	list<HSFont*> fontRegistry;
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
	HSObject * fullscreenYes;
	HSObject * fullscreenNo;
	HSObject * stretchYes;
	HSObject * stretchNo;
	HSObject * fullscreen640x360;
	HSObject * fullscreen800x450;
	HSObject * fullscreen1024x576;
	HSObject * fullscreen1152x648;
	HSObject * fullscreen1280x720;
	HSObject * fullscreen1360x765;
	HSObject * fullscreen1366x768;
	HSObject * fullscreen1400x787;
	HSObject * fullscreen1440x810;
	HSObject * fullscreen1600x900;
	HSObject * fullscreen1680x945;
	HSObject * fullscreen1920x1080;
	HSObject * windowed640x360;
	HSObject * windowed800x450;
	HSObject * windowed1024x576;
	HSObject * windowed1152x648;
	HSObject * windowed1280x720;
	HSObject * windowed1360x765;
	HSObject * windowed1366x768;
	HSObject * windowed1400x787;
	HSObject * windowed1440x810;
	HSObject * windowed1600x900;
	HSObject * windowed1680x945;
	HSObject * windowed1920x1080;

	void SortAllObjects();

	int LoadDefinition(string defFilePath, list<HSObject*> * objects, HSObject ** returnValue = NULL);
	int LoadStage(string defFilePath);
	int LoadHSMenu(string defFilePath, HSVect2D menuPos, HSMenu ** returnValue = NULL);
	int LoadMenuChooser(string defFilePath, HSFont * font, MenuChooser ** returnValue = NULL);
	int CreateMenuKeySetting(HSFont * font, MenuKeySetting ** returnValue = NULL);
	int LoadHSFont(string defFilePath, HSFont ** returnValue = NULL);
	int LoadHSCharacter(XMLElement * xml, HSCharacter * hsChar);

	int LoadPlayableCharacters(bool loadPlayer[MAX_PLAYERS]);
	void PreviousCharacter(int player);
	void NextCharacter(int player);
	int LoadPlayableStages();
	void PreviousStage();
	void NextStage();

	void UpdateMenu();
	int ApplyVideoSettings();

	int CloneObject(SpawnObject * objectToClone, list<HSObject*> * objects, HSObject ** returnValue = NULL);
	int CloneObject(HSObject * objectToClone, list<HSObject*> * objects, HSObject ** returnValue = NULL);

	int ClearAllObjects();
	int ClearSpecificObject(HSObject* object);

protected:
};

#endif