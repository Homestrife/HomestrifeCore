#ifndef __MENUMANAGER_H_
#define __MENUMANAGER_H_

#include "menu.h"

class MenuManager
{
public:
	MenuManager(HSMenu * root);
	~MenuManager();

	HSMenu * GetRoot();
	HSMenuFunction GetCurrentFunction();

	void ChoiceNext();
	void ChoicePrev();

	void CursorNext();
	void CursorPrev();
	void ToParent();
	void ToChild();

	MenuChooserFunction GetChooserFuncByMenuFunc(HSMenuFunction menuFunc);
	KeySetting GetKeyConfigByMenuFunc(HSMenuFunction menuFunc);

	void SetHidden(bool hidden);

	void SetCurrentTitleText(string text);
	void SetChoiceForItem(HSMenuFunction menuFunction, MenuChooserFunction chooserFunction);
	void SetKeyConfigByKeyForItem(HSMenuFunction menuFunction, SDL_Keycode keycode);
	void SetKeyConfigByButtonForItem(HSMenuFunction menuFunction, Uint8 joystick, Uint8 button);
	void SetKeyConfigByHatForItem(HSMenuFunction menuFunction, Uint8 joystick);
	void SetKeyConfigByStickForItem(HSMenuFunction menuFunction, Uint8 joystick);
	void SetKeyConfigEnterForItem(HSMenuFunction menuFunction);
	void SetKeyConfigNoSettingForItem(HSMenuFunction menuFunction);
	
	bool GetYesNoBoolean(MenuChooserFunction function);
	int GetResolutionXInt(MenuChooserFunction function);
	int GetResolutionYInt(MenuChooserFunction function);
	string GetYesNoConfigString(MenuChooserFunction function);
	string GetResolutionXConfigString(MenuChooserFunction function);
	string GetResolutionYConfigString(MenuChooserFunction function);

	MenuChooserFunction GetYesNoChooserFunction(bool isYes);
	MenuChooserFunction GetResolutionChooserFunction(int resolutionX, int resolutionY);

protected:
	HSMenu * _root;
	HSMenu * _current;
	bool _hidden;

	void RefreshVisibility();
	void RefreshVisibility(HSMenu * menu);

	void SetSpawnNotifications(HSMenu * menu);
};

#endif