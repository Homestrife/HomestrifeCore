#include "menuManager.h"

MenuManager::MenuManager(HSMenu * root)
{
	_root = root;
	_current = root;
	_hidden = true;
	RefreshVisibility();
}

MenuManager::~MenuManager()
{
	delete _root;
}

HSMenu * MenuManager::GetRoot()
{
	return _root;
}

HSMenuFunction MenuManager::GetCurrentFunction()
{
	if(_current == NULL) { return NO_MENU_FUNCTION; }

	return _current->GetCurrentFunction();
}

void MenuManager::ChoiceNext()
{
	if(_current == NULL) { return; }

	_current->ChoiceNext();
}

void MenuManager::ChoicePrev()
{
	if(_current == NULL) { return; }

	_current->ChoicePrev();
}

void MenuManager::CursorNext()
{
	if(_current == NULL) { return; }

	_current->CursorNext();
}

void MenuManager::CursorPrev()
{
	if(_current == NULL) { return; }

	_current->CursorPrev();
}

void MenuManager::ToParent()
{
	if(_current == NULL || _current->parentMenuItem == NULL || _current->parentMenuItem->parentMenu == NULL) { return; }

	_current->CursorFirst();
	_current = _current->parentMenuItem->parentMenu;
	RefreshVisibility();
}

void MenuManager::ToChild()
{
	if(_current == NULL || _current->GetChild() == NULL) { return; }

	_current = _current->GetChild();
	RefreshVisibility();
}

MenuChooserFunction MenuManager::GetChooserFuncByMenuFunc(HSMenuFunction menuFunc)
{
	if(_current == NULL) { return NO_CHOOSER_FUNCTION; }

	return _current->GetChooserFuncByMenuFunc(menuFunc);
}

KeySetting MenuManager::GetKeyConfigByMenuFunc(HSMenuFunction menuFunc)
{
	KeySetting setting;
	setting.type = KEY_SETTING_NONE;

	if(_current == NULL) { return setting; }

	return _current->GetKeyConfigByMenuFunc(menuFunc);
}

void MenuManager::SetHidden(bool hidden)
{
	_hidden = hidden;
	RefreshVisibility();
}

void MenuManager::SetCurrentTitleText(string text)
{
	if(_current == NULL) { return; }

	_current->titleText = text;
	RefreshVisibility();
}

void MenuManager::SetChoiceForItem(HSMenuFunction menuFunction, MenuChooserFunction chooserFunction)
{
	if(_current == NULL) { return; }

	_current->SetChoiceForItem(menuFunction, chooserFunction);
}

void MenuManager::SetKeyConfigByKeyForItem(HSMenuFunction menuFunction, SDL_Keycode keycode)
{
	if(_current == NULL) { return; }

	_current->SetKeyConfigByKeyForItem(menuFunction, keycode);
}

void MenuManager::SetKeyConfigByButtonForItem(HSMenuFunction menuFunction, Uint8 joystick, Uint8 button)
{
	if(_current == NULL) { return; }

	_current->SetKeyConfigByButtonForItem(menuFunction, joystick, button);
}

void MenuManager::SetKeyConfigByHatForItem(HSMenuFunction menuFunction, Uint8 joystick)
{
	if(_current == NULL) { return; }

	_current->SetKeyConfigByHatForItem(menuFunction, joystick);
}

void MenuManager::SetKeyConfigByStickForItem(HSMenuFunction menuFunction, Uint8 joystick)
{
	if(_current == NULL) { return; }

	_current->SetKeyConfigByStickForItem(menuFunction, joystick);
}

void MenuManager::SetKeyConfigEnterForItem(HSMenuFunction menuFunction)
{
	if(_current == NULL) { return; }

	_current->SetKeyConfigEnterForItem(menuFunction);
}

void MenuManager::SetKeyConfigNoSettingForItem(HSMenuFunction menuFunction)
{
	if(_current == NULL) { return; }

	_current->SetKeyConfigNoSettingForItem(menuFunction);
}

bool MenuManager::GetYesNoBoolean(MenuChooserFunction function)
{
	if(function == YES) { return true; }
	return false;
}

int MenuManager::GetResolutionXInt(MenuChooserFunction function)
{
	switch(function)
	{
	case RES800X450: return 800; break;
	case RES1024X576: return 1024; break;
	case RES1152X648: return 1152; break;
	case RES1280X720: return 1280; break;
	case RES1360X765: return 1360; break;
	case RES1366X768: return 1366; break;
	case RES1400X787: return 1400; break;
	case RES1440X810: return 1440; break;
	case RES1600X900: return 1600; break;
	case RES1680X945: return 1680; break;
	case RES1920X1080: return 1920; break;
	}

	return 640;
}

int MenuManager::GetResolutionYInt(MenuChooserFunction function)
{
	switch(function)
	{
	case RES800X450: return 450; break;
	case RES1024X576: return 576; break;
	case RES1152X648: return 648; break;
	case RES1280X720: return 720; break;
	case RES1360X765: return 765; break;
	case RES1366X768: return 768; break;
	case RES1400X787: return 787; break;
	case RES1440X810: return 810; break;
	case RES1600X900: return 900; break;
	case RES1680X945: return 945; break;
	case RES1920X1080: return 1080; break;
	}

	return 360;
}

string MenuManager::GetYesNoConfigString(MenuChooserFunction function)
{
	switch(function)
	{
	case YES: return "true"; break;
	case NO: return "false"; break;
	}

	return "";
}
	
string MenuManager::GetResolutionXConfigString(MenuChooserFunction function)
{
	switch(function)
	{
	case RES640X360: return "640"; break;
	case RES800X450: return "800"; break;
	case RES1024X576: return "1024"; break;
	case RES1152X648: return "1152"; break;
	case RES1280X720: return "1280"; break;
	case RES1360X765: return "1360"; break;
	case RES1366X768: return "1366"; break;
	case RES1400X787: return "1400"; break;
	case RES1440X810: return "1440"; break;
	case RES1600X900: return "1600"; break;
	case RES1680X945: return "1680"; break;
	case RES1920X1080: return "1920"; break;
	}

	return "";
}
	
string MenuManager::GetResolutionYConfigString(MenuChooserFunction function)
{
	switch(function)
	{
	case RES640X360: return "360"; break;
	case RES800X450: return "450"; break;
	case RES1024X576: return "576"; break;
	case RES1152X648: return "648"; break;
	case RES1280X720: return "720"; break;
	case RES1360X765: return "765"; break;
	case RES1366X768: return "768"; break;
	case RES1400X787: return "787"; break;
	case RES1440X810: return "810"; break;
	case RES1600X900: return "900"; break;
	case RES1680X945: return "945"; break;
	case RES1920X1080: return "1080"; break;
	}

	return "";
}

string MenuManager::GetKeyConfigString(KeySetting setting)
{
	if(setting.type == KEY_SETTING_KEY)
	{
		return GetKeyConfigString(setting.keycode);
	}
	else if(setting.type == KEY_SETTING_BUTTON)
	{
		return GetJoyButtonConfigString(setting.joystickMapping);
	}
	else if(setting.type == KEY_SETTING_HAT)
	{
		return GetHatConfigString(setting.hat);
	}
	else if(setting.type == KEY_SETTING_STICK)
	{
		return GetStickConfigString(setting.stick);
	}

	return "";
}

string MenuManager::GetKeyConfigString(SDL_Keycode key)
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

string MenuManager::GetJoyButtonConfigString(JoystickMapping joyButton)
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

string MenuManager::GetHatConfigString(Uint8 hat)
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

string MenuManager::GetStickConfigString(Uint8 stick)
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

MenuChooserFunction MenuManager::GetYesNoChooserFunction(bool isYes)
{
	if(isYes)
	{
		return YES;
	}

	return NO;
}

MenuChooserFunction MenuManager::GetResolutionChooserFunction(int resolutionX, int resolutionY)
{
	if(resolutionX == 1920) { return RES1920X1080; }
	else if(resolutionX == 1680) { return RES1680X945; }
	else if(resolutionX == 1600) { return RES1600X900; }
	else if(resolutionX == 1440) { return RES1440X810; }
	else if(resolutionX == 1400) { return RES1400X787; }
	else if(resolutionX == 1366) { return RES1366X768; }
	else if(resolutionX == 1360) { return RES1360X765; }
	else if(resolutionX == 1280) { return RES1280X720; }
	else if(resolutionX == 1152) { return RES1152X648; }
	else if(resolutionX == 1024) { return RES1024X576; }
	else if(resolutionX == 800) { return RES800X450; }
	else if(resolutionX == 640) { return RES640X360; }

	return NO_CHOOSER_FUNCTION;
}

void MenuManager::RefreshVisibility()
{
	RefreshVisibility(_root);
}

void MenuManager::RefreshVisibility(HSMenu * menu)
{
	if(menu == NULL) { return; }

	if(!_hidden && menu == _current)
	{
		menu->SetVisible(true);
		SetSpawnNotifications(menu);
	}
	else
	{
		menu->SetVisible(false);
	}

	list<HSMenuItem*>::iterator miIt;
	for(miIt = menu->items.begin(); miIt != menu->items.end(); miIt++)
	{
		RefreshVisibility((*miIt)->child);
	}
}

void MenuManager::SetSpawnNotifications(HSMenu * menu)
{
	if(menu == NULL) { return; }

	menu->itemNeedToSpawnText = true;
	menu->needToSpawnCursor = true;
	
	list<HSMenuItem*>::iterator miIt;
	for(miIt = menu->items.begin(); miIt != menu->items.end(); miIt++)
	{
		(*miIt)->chooserOrKeySettingNeedToSpawnText = true;
	}

	HSMenuItem * curItem = menu->parentMenuItem;
	while(curItem != NULL)
	{
		curItem->childNeedToSpawnText = true;
		curItem->parentMenu->itemNeedToSpawnText = true;
		curItem = curItem->parentMenu->parentMenuItem;
	}
}