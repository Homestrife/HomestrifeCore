#ifndef __MENU_H_
#define __MENU_H_

#include "hstext.h"

enum MenuChooserFunction
{
	NO_CHOOSER_FUNCTION,
	YES,
	NO,
	RES640X360,
	RES800X450,
	RES1024X576,
	RES1152X648,
	RES1280X720,
	RES1360X765,
	RES1366X768,
	RES1400X787,
	RES1440X810,
	RES1600X900,
	RES1680X945,
	RES1920X1080
};

enum HSMenuFunction
{
	NO_MENU_FUNCTION,
	BACK,
	QUIT_GAME,
	RESUME_MATCH,
	QUIT_MATCH,
	VERSUS,
	OPTIONS,
	FREE_FOR_ALL,
	FREE_FOR_ALL_2_PLAYERS,
	FREE_FOR_ALL_3_PLAYERS,
	FREE_FOR_ALL_4_PLAYERS,
	VIDEO,
	KEY_CONFIG,
	KEY_CONFIG_PLAYER_1,
	KEY_CONFIG_PLAYER_2,
	KEY_CONFIG_PLAYER_3,
	KEY_CONFIG_PLAYER_4,
	FULL_SCREEN,
	STRETCH_SCREEN,
	FULL_SCREEN_RESOLUTION,
	WINDOWED_RESOLUTION,
	APPLY_VIDEO_SETTINGS,
	KEY_CONFIG_UP,
	KEY_CONFIG_DOWN,
	KEY_CONFIG_LEFT,
	KEY_CONFIG_RIGHT,
	KEY_CONFIG_LIGHT_ATTACK,
	KEY_CONFIG_HEAVY_ATTACK,
	KEY_CONFIG_JUMP,
	KEY_CONFIG_BLOCK,
	KEY_CONFIG_PAUSE,
	KEY_CONFIG_MENU_CONFIRM,
	KEY_CONFIG_MENU_BACK
};

enum KeySettingType
{
	KEY_SETTING_NONE,
	KEY_SETTING_KEY,
	KEY_SETTING_BUTTON,
	KEY_SETTING_HAT,
	KEY_SETTING_STICK,
	KEY_SETTING_ENTER
};

struct JoystickMapping
{
	Uint8 joystick;
	Uint8 button;
};

struct KeySetting
{
	KeySettingType type;
	SDL_Keycode keycode;
	JoystickMapping joystickMapping;
	Uint8 hat;
	Uint8 stick;
};

class MenuChooser;

class MenuChooserItem
{
public:
	string itemText;
	MenuChooser * parentChooser;
	MenuChooserFunction function;
	int order;

	MenuChooserItem();

protected:
};

class HSMenuItem;

class MenuChooser : public HSText
{
public:
	HSMenuItem * parentMenuItem;
	list<MenuChooserItem*> items;

	MenuChooser(HSFont * font);
	~MenuChooser();

	void ChoiceNext();
	void ChoicePrev();

	MenuChooserFunction GetCurrentChooserFunction();

	void SetByChoiceFunction(MenuChooserFunction function);

	void RefreshChooser();
	void SetVisible(bool visible);

protected:
	bool _visible;
	int _choice;
};

class MenuKeySetting;

class MenuKeySettingItem
{
public:
	string itemText;
	MenuKeySetting * parentKeySetting;
	KeySetting keySetting;
	int order;

	MenuKeySettingItem();

protected:

};

class MenuKeySetting : public HSText
{
public:
	HSMenuItem * parentMenuItem;
	list<MenuKeySettingItem*> items;

	MenuKeySetting(HSFont * font);
	~MenuKeySetting();

	KeySetting GetCurretKeySetting();

	void SetByKey(SDL_Keycode keycode);
	void SetByButton(Uint8 joystick, Uint8 button);
	void SetByHat(Uint8 joystick);
	void SetByStick(Uint8 joystick);
	void SetEnter();
	void SetNoSetting();

	void RefreshKeySetting();
	void SetVisible(bool visible);

protected:
	bool _visible;
	int _choice;
};

class HSMenu;

class HSMenuItem : public HSText
{
public:
	string itemText;
	bool childNeedToSpawnText;
	bool chooserOrKeySettingNeedToSpawnText;
	HSMenu * parentMenu;
	HSMenu * child;
	HSMenuFunction function;
	MenuChooser * chooser;
	int order;
	MenuKeySetting * currentKeySetting;

	float itemHeight;

	HSMenuItem(HSFont * font);
	~HSMenuItem();

	void SetByChoiceFunction(MenuChooserFunction function);
	void SetByKey(SDL_Keycode keycode);
	void SetByButton(Uint8 joystick, Uint8 button);
	void SetByHat(Uint8 joystick);
	void SetByStick(Uint8 joystick);
	void SetEnter();
	void SetNoSetting();

	void RefreshMenuItem();
	void SetVisible(bool visible);

	void ChildChanged();
	void ChooserOrKeySettingChanged();

protected:
	bool _visible;
};

class HSMenu : public HSText
{
public:
	string titleText;
	bool itemNeedToSpawnText;
	HSMenuItem * parentMenuItem;
	HSObject * cursorToClone;
	HSObject * activeCursor;
	bool needToSpawnCursor;
	int cursorIndex;
	list<HSMenuItem*> items;

	float titleHeight;
	float titleSeparation;
	float cursorWidth;
	float cursorSeparation;
	float itemHeight;
	float itemSeparation;

	HSMenu(HSFont * font);
	~HSMenu();

	HSMenuItem * GetCurrentItem();
	HSMenu * GetChild();
	HSMenuFunction GetCurrentFunction();
	MenuChooserFunction GetChooserFuncByMenuFunc(HSMenuFunction menuFunc);
	KeySetting GetKeyConfigByMenuFunc(HSMenuFunction menuFunc);

	void SetChoiceForItem(HSMenuFunction menuFunction, MenuChooserFunction chooserFunction);
	void SetKeyConfigByKeyForItem(HSMenuFunction menuFunction, SDL_Keycode keycode);
	void SetKeyConfigByButtonForItem(HSMenuFunction menuFunction, Uint8 joystick, Uint8 button);
	void SetKeyConfigByHatForItem(HSMenuFunction menuFunction, Uint8 joystick);
	void SetKeyConfigByStickForItem(HSMenuFunction menuFunction, Uint8 joystick);
	void SetKeyConfigEnterForItem(HSMenuFunction menuFunction);
	void SetKeyConfigNoSettingForItem(HSMenuFunction menuFunction);
	
	void ChoiceNext();
	void ChoicePrev();

	void CursorFirst();
	void CursorNext();
	void CursorPrev();

	void RefreshMenu();
	void SetVisible(bool visible);
	
	void RepositionCursor();

	void ItemChanged();

protected:
	bool _visible;
};

#endif