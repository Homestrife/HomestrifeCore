#ifndef __MAIN_H_
#define __MAIN_H_

#include "fighter.h"
#include "HUD.h"
#include "menu.h"
#include "menuManager.h"
#include "objectManager.h"
#include "renderingManager.h"

#define MAX_INPUT_HISTORY 60
#define STICK_THRESHOLD 18000 //how far a stick must be tilted before it actually registers as a direction being "pressed"
#define STICK_HARD_THRESHOLD 32700 //how far a stick must be tilted before it actually registers as a direction being pressed hard

#define MAIN_MENU_HEADER_HEIGHT 43
#define MAIN_MENU_CURSOR_WIDTH 35
#define MAIN_MENU_ITEM_HEIGHT 35
#define MAIN_MENU_ITEM_SPACING 20
#define PRESS_DESIRED_BUTTON_OFFSET_X 400
#define VIDEO_SETTING_OFFSET_X 600

#define CHAR_SELECT_PLAYER_LEFT_POS_X -400
#define CHAR_SELECT_PLAYER_RIGHT_POS_X CHAR_SELECT_PLAYER_LEFT_POS_X*-1
#define CHAR_SELECT_PLAYER_TOP_POS_Y -200
#define CHAR_SELECT_PLAYER_BOTTOM_POS_Y CHAR_SELECT_PLAYER_TOP_POS_Y*-1
#define CHARACTER_SELECT_POS_X (-174)
#define CHARACTER_SELECT_POS_Y ((MAX_GAME_RESOLUTION_Y / -2) + 20)
#define PLAYER_LEFT_POS_X -750
#define PLAYER_RIGHT_POS_X ((PLAYER_LEFT_POS_X * -1) - 159)
#define PLAYER_TOP_POS_Y -400
#define PLAYER_BOTTOM_POS_Y 50
#define READY_LEFT_POS_X (PLAYER_LEFT_POS_X + 19)
#define READY_RIGHT_POS_X (PLAYER_RIGHT_POS_X + 19)
#define SELECT_PALETTE_LEFT_POS_X (READY_LEFT_POS_X - 80)
#define SELECT_PALETTE_RIGHT_POS_X (READY_RIGHT_POS_X - 80)
#define SELECT_PALETTE_TOP_POS_Y PLAYER_TOP_POS_Y + 55
#define SELECT_PALETTE_BOTTOM_POS_Y PLAYER_BOTTOM_POS_Y + 55
#define SELECT_CHARACTER_LEFT_POS_X (READY_LEFT_POS_X - 109)
#define SELECT_CHARACTER_RIGHT_POS_X (READY_RIGHT_POS_X - 109)
#define STAGE_SELECT_POS_X (-132)
#define SELECT_STAGE_POS_X (-130)
#define SELECT_STAGE_POS_Y 300

#define JOYSTICK_UNKNOWN 1000
#define JOYBUTTON_UNKNOWN 1000

int RenderThreadControl(void *data);

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
	FREE_FOR_ALL,
	OPTIONS,
	VIDEO,
	KEY_CONFIG,
	PLAYER_KEY_CONFIG,
	INPUT_KEY
};

enum GameType
{
	FREE_FOR_ALL_2,
	FREE_FOR_ALL_3,
	FREE_FOR_ALL_4
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
	PAUSE_VIDEO,
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
	SDL_Keycode keyUp;
	SDL_Keycode keyDown;
	SDL_Keycode keyLeft;
	SDL_Keycode keyRight;
	SDL_Keycode keyJump;
	SDL_Keycode keyBlock;
	SDL_Keycode keyLight;
	SDL_Keycode keyHeavy;
	SDL_Keycode keyStart;
	SDL_Keycode keySelect;
	SDL_Keycode keyMenuConfirm;
	SDL_Keycode keyMenuBack;

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

class Main
{
public:
	Main();
	int Execute();

protected:
	ObjectManager * objectManager;
	RenderingManager * renderingManager;
	
	GameState gameState;
	MainMenuState mainMenuState;
	GameType gameType;
	CharacterSelectState characterSelectState;
	CharacterSelectPlayerState characterSelectPlayerState[MAX_PLAYERS];
	MatchState matchState;
	MatchPlayerState matchPlayerState[MAX_PLAYERS];
	PauseMenuState pauseMenuState;

	list<SDL_Joystick*> sticks;

	unsigned int lastFrameTicks;
	unsigned int frame;

	int Initialize();
	void DebugOutput();
	int AdvanceHolds();
	int Event();
	int Update();
	int Collide();
	int SpawnObjects();
	int PlayAudio();
	int PlayAudio(list<HSObject*> * objects);
	int DeleteObjects();
	int DeleteObjects(list<HSObject*> * objects);
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

	void DefaultKeyConfig();
	int LoadKeyConfig();
	void LoadPlayerConfig(XMLElement * config, int player);
	bool LoadToKeyConfig(string config, SDL_Keycode * key);
	bool LoadToJoyButtonConfig(string config, JoystickMapping * joyButton);
	bool LoadToHatConfig(string config, Uint8 * hat);
	bool LoadToStickConfig(string config, Uint8 * stick);

	int SaveKeyConfig();
	void SetPlayerConfig(XMLElement * config, int player);
	string GetKeyConfigString(SDL_Keycode key);
	string GetJoyButtonConfigString(JoystickMapping joyButton);
	string GetHatConfigString(Uint8 hat);
	string GetStickConfigString(Uint8 stick);

	void ClearStickForAllPlayers(Uint8 which);
	void ClearHatForAllPlayers(Uint8 which);
	void ClearButtonForAllPlayers(Uint8 which, Uint8 button, int playerToIgnore);
	void ClearKeyForAllPlayers(SDL_Keycode sym, int playerToIgnore);
	void ClearControls(int player);

	void ChangePlayerPalette(int player);
	
	int HandleEvent(SDL_Event * Event);
	bool IsHeld(InputState * cur, InputState * prev);
	bool IsPressed(InputState * cur, InputState * prev);
	bool IsReleased(InputState * cur, InputState * prev);
    void InputFocus();
    void InputBlur();
    void KeyDown(SDL_Keycode sym);
    void KeyUp(SDL_Keycode sym);
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
    void User(Uint8 type, int code, void* data1, void* data2);
};

#endif