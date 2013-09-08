#ifndef __MAIN_H_
#define __MAIN_H_

#include <iostream>
#include <fstream>
#include "fighter.h"
#include "HUD.h"
#include "menu.h"
#include "menuManager.h"
#include "objectManager.h"

#define TARGET_FPS 60
#define MAX_GAME_RESOLUTION_X 1920
#define MAX_GAME_RESOLUTION_Y 1080
#define MIN_GAME_RESOLUTION_X 640
#define MIN_GAME_RESOLUTION_Y 360
#define GAME_ASPECT_RATIO_X 16
#define GAME_ASPECT_RATIO_Y 9
#define MAX_INPUT_HISTORY 60
#define STICK_THRESHOLD 18000 //how far a stick must be tilted before it actually registers as a direction being "pressed"
#define STICK_HARD_THRESHOLD 32000 //how far a stick must be tilted before it actually registers as a direction being pressed hard
#define PAN_SPEED 200
#define ZOOM_BOUNDARY_X_THRESHOLD 800
#define ZOOM_BOUNDARY_Y_THRESHOLD 450

#define MAIN_MENU_HEADER_HEIGHT 43
#define MAIN_MENU_CURSOR_WIDTH 35
#define MAIN_MENU_ITEM_HEIGHT 35
#define MAIN_MENU_ITEM_SPACING 20
#define PRESS_DESIRED_BUTTON_OFFSET_X 400

#define CHAR_SELECT_PLAYER_ONE_POS_X -600
#define CHAR_SELECT_PLAYER_TWO_POS_X 600
#define CHAR_SELECT_PLAYER_POS_Y 0
#define CHARACTER_SELECT_POS_X (-174)
#define CHARACTER_SELECT_POS_Y ((MAX_GAME_RESOLUTION_Y / -2) + 20)
#define PLAYER_ONE_POS_X -500
#define PLAYER_TWO_POS_X ((PLAYER_ONE_POS_X * -1) - 159)
#define PLAYER_POS_Y 200
#define READY_ONE_POS_X (PLAYER_ONE_POS_X + 19)
#define READY_TWO_POS_X (PLAYER_TWO_POS_X + 19)
#define SELECT_PALETTE_ONE_POS_X (READY_ONE_POS_X - 80)
#define SELECT_PALETTE_TWO_POS_X (READY_TWO_POS_X - 80)
#define SELECT_PALETTE_POS_Y PLAYER_POS_Y + 55

#define JOYSTICK_UNKNOWN 1000
#define JOYBUTTON_UNKNOWN 1000

enum GameState
{
	MAIN_MENU,
	CHARACTER_SELECT,
	MATCH
};

enum MainMenuState
{
	TOP,
	VERSUS,
	OPTIONS,
	VIDEO,
	KEY_CONFIG,
	PLAYER_KEY_CONFIG,
	INPUT_KEY
};

enum GameType
{
	FREE_FOR_ALL
};

enum CharacterSelectState
{
	PLAYERS_SELECTING,
	STAGE_SELECT,
	MUSIC_SELECT
};

enum CharacterSelectPlayerState
{
	SELECTING_CHARACTER,
	SELECTING_PALETTE,
	READY
};

enum MatchState
{
	IN_PROGRESS,
	PAUSED,
	RESULTS
};

enum MatchPlayerState
{
	ACTIVE,
	LOST
};

enum PauseMenuState
{
	PAUSE_TOP,
	PAUSE_KEY_CONFIG,
	PAUSE_PLAYER_KEY_CONFIG,
	PAUSE_INPUT_KEY
};

struct JoystickMapping
{
	Uint8 joystick;
	Uint8 button;
};

struct InputMappings
{
	//keyboard key mappings
	SDLKey keyUp;
	SDLKey keyDown;
	SDLKey keyLeft;
	SDLKey keyRight;
	SDLKey keyJump;
	SDLKey keyBlock;
	SDLKey keyLight;
	SDLKey keyHeavy;
	SDLKey keyStart;
	SDLKey keySelect;
	SDLKey keyMenuConfirm;
	SDLKey keyMenuBack;

	//joystick button mappings
	JoystickMapping buttonUp;
	JoystickMapping buttonDown;
	JoystickMapping buttonLeft;
	JoystickMapping buttonRight;
	JoystickMapping buttonJump;
	JoystickMapping buttonBlock;
	JoystickMapping buttonLight;
	JoystickMapping buttonHeavy;
	JoystickMapping buttonStart;
	JoystickMapping buttonSelect;
	JoystickMapping buttonMenuConfirm;
	JoystickMapping buttonMenuBack;
	
	//joystick stick and hat mapping
	Uint8 stick;
	Uint8 hat;
};

struct PreviousJoystickStates
{
	Uint8 hat;
	bool stickHeldUp;
	bool stickHeldDown;
	bool stickHeldLeft;
	bool stickHeldRight;
	bool stickHeldHardUp;
	bool stickHeldHardDown;
	bool stickHeldHardLeft;
	bool stickHeldHardRight;
};

enum CurrentSettingMapping
{
	SETTING_UP,
	SETTING_DOWN,
	SETTING_LEFT,
	SETTING_RIGHT,
	SETTING_JUMP,
	SETTING_LIGHT,
	SETTING_BLOCK
};

class Main
{
public:
	Main();
	int Execute();

protected:
	string currentWorkingDirectory;

	ObjectManager * objectManager;

	GameState gameState;
	MainMenuState mainMenuState;
	GameType gameType;
	CharacterSelectState characterSelectState;
	CharacterSelectPlayerState characterSelectPlayerState[MAX_PLAYERS];
	MatchState matchState;
	MatchPlayerState matchPlayerState[MAX_PLAYERS];
	PauseMenuState pauseMenuState;

	GLuint currentShaderProgramID;
	int nonIndexedPosOffsetLoc;
	int indexedPosOffsetLoc;
	int nonIndexedScaleLoc;
	int indexedScaleLoc;
	int nonIndexedResolutionLoc;
	int indexedResolutionLoc;
	int nonIndexedResScaleLoc;
	int indexedResScaleLoc;
	int nonIndexedFocusPosLoc;
	int indexedFocusPosLoc;
	int nonIndexedZoomOutLoc;
	int indexedZoomOutLoc;
	int nonIndexedDepthLoc;
	int indexedDepthLoc;
	int nonIndexedTexLoc;
	int indexedTexLoc;
	int paletteLoc;
	bool notDone;
	SDL_Surface* surf_display;
	int screenResolutionX;
	int screenResolutionY;
	int gameResolutionX;
	int gameResolutionY;
	float resolutionScale;
	bool fullScreen;

	list<SDL_Joystick*> sticks;

	float zoomOut;
	HSVect2D focusPos;

	unsigned int lastFrameTicks;
	unsigned int frame;

	int SetBestGameResolution();
	void ChangeShaderProgram(GLuint programID);

	int Initialize();
	int InitializeGraphics();
	int AdvanceHolds();
	int Event(SDL_Event* events);
	int Update();
	int Collide();
	int SpawnObjects();
	int Render();
	int RenderTexture(HSObject * obj, TextureInstance tex); 
	int PlayAudio();
	int DeleteObjects();
	int Cleanup();

	//state handling
	int ChangeGameState(GameState newState);
	int StartLoading();
	int EndLoading();

	int InitializeMainMenu();
	int ChangeMainMenuState(MainMenuState newState);
	int EventMainMenu(InputStates * inputHistory, int frame);
	int UpdateMainMenu();

	int InitializeCharacterSelect();
	int ChangeCharacterSelectState(CharacterSelectState newState);
	int ChangeCharacterSelectPlayerState(CharacterSelectPlayerState newState, int player);
	int EventCharacterSelect(InputStates * inputHistory, int frame, int player);
	int UpdateCharacterSelect();

	string selectedCharacters[MAX_PLAYERS];
	int selectedPalettes[MAX_PLAYERS];

	int InitializeMatch();
	int ChangeMatchState(MatchState newState);
	int ChangeMatchPlayerState(MatchPlayerState newState, int player);
	int ChangePauseMenuState(PauseMenuState newState);
	int EventMatch(InputStates * inputHistory, int frame, int player);
	int UpdateMatch();
	int CollideMatch();

	int playerLives[MAX_PLAYERS];

	//event/input crap
	InputMappings mappings[MAX_PLAYERS];
	InputStates * inputHistory[MAX_PLAYERS];
	InputStates * curInputs[MAX_PLAYERS];
	bool inputStateChange[MAX_PLAYERS];
	PreviousJoystickStates prevJoystickStates[MAX_PLAYERS];
	InputStates defaultInputs;

	int playerToSetUp;
	int keyToSetUp;
	CurrentSettingMapping currentSettingMapping;

	void DefaultConfig();
	int LoadConfig();
	void LoadPlayerConfig(XMLElement * config, int player);
	bool LoadToKeyConfig(string config, SDLKey * key);
	bool LoadToJoyButtonConfig(string config, JoystickMapping * joyButton);
	bool LoadToHatConfig(string config, Uint8 * hat);
	bool LoadToStickConfig(string config, Uint8 * stick);

	int SaveConfig();
	void SetPlayerConfig(XMLElement * config, int player);
	string GetKeyConfigString(SDLKey key);
	string GetJoyButtonConfigString(JoystickMapping joyButton);
	string GetHatConfigString(Uint8 hat);
	string GetStickConfigString(Uint8 stick);

	void ClearControls(int player);

	void ChangePlayerPalette(int player);
	
	int HandleEvent(SDL_Event * Event);
	bool IsHeld(InputState * cur, InputState * prev);
	bool IsPressed(InputState * cur, InputState * prev);
	bool IsReleased(InputState * cur, InputState * prev);
    void InputFocus();
    void InputBlur();
    void KeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
    void KeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);
    void MouseFocus();
    void MouseBlur();
    void MouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle);
    void MouseWheel(bool Up, bool Down);
    void LButtonDown(int mX, int mY);
    void LButtonUp(int mX, int mY);
    void RButtonDown(int mX, int mY);
    void RButtonUp(int mX, int mY);
    void MButtonDown(int mX, int mY);
    void MButtonUp(int mX, int mY);
    void JoyAxis(Uint8 which, Uint8 axis, Sint16 value);
    void JoyButtonDown(Uint8 which, Uint8 button);
    void JoyButtonUp(Uint8 which, Uint8 button);
    void JoyHat(Uint8 which, Uint8 hat, Uint8 value);
    void JoyBall(Uint8 which, Uint8 ball, Sint16 xrel, Sint16 yrel);
    void Minimize();
    void Restore();
    void Resize(int w,int h);
    void Expose();
    void Exit();
	int ToggleFullScreen();
	int SetFullScreen(bool newFullScreen);
    void User(Uint8 type, int code, void* data1, void* data2);
};

#endif