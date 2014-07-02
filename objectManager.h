#ifndef __OBJECTMANAGER_H_
#define __OBJECTMANAGER_H_

#include <iostream>
#include <fstream>
#include "fighter.h"
#include "HUD.h"
#include "menuManager.h"
#include "characterSelect.h"

#define RINGOUT_BUFFER 500 //how far below the bottom of the stage something must fall before its considered "ringed out"

#define CHARACTER_SELECT_CURSOR_DEPTH 0
#define CHARACTER_SELECT_TITLE_DEPTH 1
#define CHARACTER_SELECT_NAME_DEPTH 2
#define CHARACTER_SELECT_INSTRUCTIONS_DEPTH 3
#define CHARACTER_SELECT_PANEL_BORDER_DEPTH 4
#define CHARACTER_SELECT_PANEL_DEPTH 5
#define CHARACTER_SELECT_PORTRAIT_BORDER_DEPTH 6
#define CHARACTER_SELECT_PORTRAIT_DEPTH 7
#define CHARACTER_SELECT_BACKGROUND_DEPTH 8

#define STAGE_SELECT_TITLE_DEPTH 0
#define STAGE_SELECT_NAME_DEPTH 1
#define STAGE_SELECT_PREVIEW_BORDER_DEPTH 2
#define STAGE_SELECT_PREVIEW_DEPTH 3
#define STAGE_SELECT_BACKGROUND_DEPTH 4

#define MUSIC_SELECT_TITLE_DEPTH 0
#define MUSIC_SELECT_NAME_DEPTH 1
#define MUSIC_SELECT_BACKGROUND_DEPTH 2

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

	MenuManager * menuManager;
	CharacterSelectManager * characterSelectManager;
	CharacterSelectChoices characterSelectChoices;

	HSObject * loading;
	HSObject * playerOne;
	HSObject * playerTwo;
	HSObject * playerThree;
	HSObject * playerFour;
	HSObject * wins;

	void SortAllObjects();

	int LoadDefinition(string defFilePath, list<HSObject*> * objects, HSObject ** returnValue = NULL);
	int LoadStage(string defFilePath);
	int LoadHSMenu(string defFilePath, HSVect2D menuPos, HSMenu ** returnValue = NULL);
	
	int LoadMenuChooser(string defFilePath, HSFont * font, MenuChooser ** returnValue = NULL);
	int CreateMenuKeySetting(HSFont * font, MenuKeySetting ** returnValue = NULL);
	int LoadHSFont(string defFilePath, HSFont ** returnValue = NULL);
	int LoadHSCharacter(XMLElement * xml, HSCharacter * hsChar);

	int LoadCharacterSelect(string defFilePath, string pcFilePath, string psFilePath, string pmFilePath);
	int LoadPlayableCharacters(string pcFilePath, string panelBorderFilePath);
	int LoadPlayableStages(string psFilePath);
	int LoadPlayableMusic(string pmFilePath);

	void UpdateMenu();
	int ApplyVideoSettings();

	int CloneObject(SpawnObject * objectToClone, list<HSObject*> * objects, HSObject ** returnValue = NULL);
	int CloneObject(HSObject * objectToClone, list<HSObject*> * objects, HSObject ** returnValue = NULL);

	int ClearAllObjects();
	int ClearSpecificObject(HSObject* object);

protected:
};

#endif