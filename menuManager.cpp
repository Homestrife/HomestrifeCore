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