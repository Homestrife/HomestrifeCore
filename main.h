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

#define JOYSTICK_UNKNOWN 1000
#define JOYBUTTON_UNKNOWN 1000

#define PROMPT_READY_DURATION 120
#define PROMPT_FIGHT_DURATION 120
#define PROMPT_WIN_DURATION 240

#define STARTING_LIVES 2

int RenderThreadControl(void *data);

enum GameState
{
	MAIN_MENU,
	CHARACTER_SELECT,
	MATCH
};

enum GameType
{
	GAME_TYPE_FREE_FOR_ALL
};

enum MatchState
{
	INTRO,
	IN_PROGRESS,
	PAUSED,
	RESULTS
};

enum MatchPlayerState
{
	ACTIVE,
	LOST
};

enum BindingKey
{
	BINDING_NONE,
	BINDING_UP,
	BINDING_DOWN,
	BINDING_LEFT,
	BINDING_RIGHT,
	BINDING_LIGHT,
	BINDING_HEAVY,
	BINDING_JUMP,
	BINDING_BLOCK,
	BINDING_PAUSE,
	BINDING_CONFIRM,
	BINDING_BACK
};

struct InputMappings
{
	KeySetting up;
	KeySetting down;
	KeySetting left;
	KeySetting right;
	KeySetting jump;
	KeySetting block;
	KeySetting light;
	KeySetting heavy;
	KeySetting start;
	KeySetting select;
	KeySetting menuConfirm;
	KeySetting menuBack;

	//keyboard key mappings
	//SDL_Keycode keyUp;
	//SDL_Keycode keyDown;
	//SDL_Keycode keyLeft;
	//SDL_Keycode keyRight;
	//SDL_Keycode keyJump;
	//SDL_Keycode keyBlock;
	//SDL_Keycode keyLight;
	//SDL_Keycode keyHeavy;
	//SDL_Keycode keyStart;
	//SDL_Keycode keySelect;
	//SDL_Keycode keyMenuConfirm;
	//SDL_Keycode keyMenuBack;

	//joystick button mappings
	//JoystickMapping buttonUp;
	//JoystickMapping buttonDown;
	//JoystickMapping buttonLeft;
	//JoystickMapping buttonRight;
	//JoystickMapping buttonJump;
	//JoystickMapping buttonBlock;
	//JoystickMapping buttonLight;
	//JoystickMapping buttonHeavy;
	//JoystickMapping buttonStart;
	//JoystickMapping buttonSelect;
	//JoystickMapping buttonMenuConfirm;
	//JoystickMapping buttonMenuBack;
	
	//joystick stick and hat mapping
	//Uint8 stick;
	//Uint8 hat;
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
	GameType gameType;
	MatchState matchState;
	MatchPlayerState matchPlayerState[MAX_PLAYERS];
	BindingKey bindingKey;

	list<SDL_Joystick*> sticks;

	unsigned int lastFrameTicks;
	unsigned int frame;

	bool needToPause;
	int matchPromptTimer;

	int Initialize();
	void DebugOutput();
	int AdvanceHolds();
	int Event();
	int Update();
	int Collide();
	int SpawnObjects();
	int SpawnMenus(HSMenu * menu);
	int SpawnCharacterSelect(CharacterSelectManager * manager);
	int SpawnHUD(HUDManager * manager);
	int SpawnText(HSText * text);
	int PlayAudio();
	int PlayAudio(list<HSObject*> * objects);
	int DeleteObjects();
	int DeleteObjects(list<HSObject*> * objects);
	int Cleanup();

	//state handling
	int ChangeGameState(GameState newState);
	int StartLoading();
	int EndLoading();

	int EventMenu(InputStates * inputHistory, int frame);
	int UpdateMenu();

	int InitializeMainMenu();
	int EventMainMenu(InputStates * inputHistory, int frame);
	int UpdateMainMenu();

	int InitializeCharacterSelect();
	int EventCharacterSelect(InputStates * inputHistory, int frame, int player);
	int UpdateCharacterSelect();

	int InitializeMatch();
	int ChangeMatchState(MatchState newState);
	int ChangeMatchPlayerState(MatchPlayerState newState, int player);
	int EventMatch(InputStates * inputHistory, int frame, int player);
	int UpdateMatch();
	int CollideMatch();

	//event/input crap
	InputMappings mappings[MAX_PLAYERS];
	InputStates * inputHistory[MAX_PLAYERS];
	InputStates * curInputs[MAX_PLAYERS];
	bool inputStateChange[MAX_PLAYERS];
	PreviousJoystickStates prevJoystickStates[MAX_PLAYERS];
	InputStates defaultInputs;

	int playerToSetUp;

	void DefaultKeyConfig();
	int LoadKeyConfig();
	void LoadPlayerConfig(XMLElement * config, int player);
	bool LoadToKeyConfig(string config, SDL_Keycode * key);
	bool LoadToJoyButtonConfig(string config, JoystickMapping * joyButton);
	bool LoadToHatConfig(string config, Uint8 * hat);
	bool LoadToStickConfig(string config, Uint8 * stick);

	void SetMenuVideoSettings();
	void SetMenuKeyConfig();
	void SetMenuKeyConfig(HSMenuFunction function, KeySetting setting);

	void ApplyAndSaveVideoSettings();
	int SaveKeyConfig();
	void SavePlayerKeyConfig(XMLElement * config, int player);
	string GetKeyConfigString(KeySetting setting);
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