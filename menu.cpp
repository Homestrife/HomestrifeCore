#include "menu.h"

/////////////////
//MenuChooserItem//
/////////////////

MenuChooserItem::MenuChooserItem()
{
	itemText = "";
	parentChooser = NULL;
	function = NO_CHOOSER_FUNCTION;
	order = 0;
}

/////////////
//HSChooser//
/////////////

MenuChooser::MenuChooser(HSFont * font) : HSText(font)
{
	parentMenuItem = NULL;
	items.clear();
	_choice = 0;
	_visible = false;
}

MenuChooser::~MenuChooser()
{
	list<MenuChooserItem*>::iterator ciIt;
	for(ciIt = items.begin(); ciIt != items.end(); ciIt++)
	{
		delete *ciIt;
	}
}

void MenuChooser::ChoiceNext()
{
	_choice++;

	if(_choice >= items.size())
	{
		_choice = 0;
	}
	RefreshChooser();
}

void MenuChooser::ChoicePrev()
{
	_choice--;

	if(_choice < 0)
	{
		_choice = items.size() - 1;

		if(_choice < 0)
		{
			_choice = 0;
		}
	}
	RefreshChooser();
}

MenuChooserFunction MenuChooser::GetCurrentChooserFunction()
{
	int cur = 0;

	list<MenuChooserItem*>::iterator ciIt;
	for(ciIt = items.begin(); ciIt != items.end(); ciIt++)
	{
		if(cur == _choice)
		{
			return (*ciIt)->function;
		}

		cur++;
	}

	return NO_CHOOSER_FUNCTION;
}

void MenuChooser::SetByChoiceFunction(MenuChooserFunction function)
{
	int cur = 0;

	list<MenuChooserItem*>::iterator ciIt;
	for(ciIt = items.begin(); ciIt != items.end(); ciIt++)
	{
		if(function == (*ciIt)->function)
		{
			_choice = cur;
			break;
		}

		cur++;
	}
	RefreshChooser();
}

void MenuChooser::RefreshChooser()
{
	if(_visible)
	{
		int cur = 0;

		list<MenuChooserItem*>::iterator ciIt;
		for(ciIt = items.begin(); ciIt != items.end(); ciIt++)
		{
			if(cur == _choice)
			{
				SetText((*ciIt)->itemText);
				break;
			}

			cur++;
		}
	}
	else
	{
		DeleteText();
	}

	parentMenuItem->ChooserOrKeySettingChanged();
}

void MenuChooser::SetVisible(bool visible)
{
	_visible = visible;
	RefreshChooser();
}

////////////////////
//MenuKeySettingItem//
////////////////////

MenuKeySettingItem::MenuKeySettingItem()
{	
	itemText = "";
	parentKeySetting = NULL;
	keySetting.type = KEY_SETTING_NONE;
	keySetting.keycode = NO_CHOOSER_FUNCTION;
	keySetting.joystickMapping.button = 0;
	keySetting.joystickMapping.joystick = 0;
	keySetting.hat = 0;
	keySetting.stick = 0;
	order = 0;
}

////////////////
//MenuKeySetting//
////////////////

MenuKeySetting::MenuKeySetting(HSFont * font) : HSText(font)
{
	parentMenuItem = NULL;
	items.clear();
	_choice = 0;
	_visible = false;
}

MenuKeySetting::~MenuKeySetting()
{
	list<MenuKeySettingItem*>::iterator ciIt;
	for(ciIt = items.begin(); ciIt != items.end(); ciIt++)
	{
		delete *ciIt;
	}
}

KeySetting MenuKeySetting::GetCurretKeySetting()
{
	int cur = 0;

	list<MenuKeySettingItem*>::iterator ciIt;
	for(ciIt = items.begin(); ciIt != items.end(); ciIt++)
	{
		if(cur == _choice)
		{
			return (*ciIt)->keySetting;
		}

		cur++;
	}

	KeySetting setting;
	setting.type = KEY_SETTING_NONE;

	return setting;
}

void MenuKeySetting::SetByKey(SDL_Keycode keycode)
{
	int cur = 0;

	list<MenuKeySettingItem*>::iterator ciIt;
	for(ciIt = items.begin(); ciIt != items.end(); ciIt++)
	{
		if((*ciIt)->keySetting.type == KEY_SETTING_KEY && keycode == (*ciIt)->keySetting.keycode)
		{
			_choice = cur;
			break;
		}

		cur++;
	}
	RefreshKeySetting();
}

void MenuKeySetting::SetByButton(Uint8 joystick, Uint8 button)
{
	int cur = 0;

	list<MenuKeySettingItem*>::iterator ciIt;
	for(ciIt = items.begin(); ciIt != items.end(); ciIt++)
	{
		if((*ciIt)->keySetting.type == KEY_SETTING_BUTTON && joystick == (*ciIt)->keySetting.joystickMapping.joystick && button == (*ciIt)->keySetting.joystickMapping.button)
		{
			_choice = cur;
			break;
		}

		cur++;
	}
	RefreshKeySetting();
}

void MenuKeySetting::SetByHat(Uint8 joystick)
{
	int cur = 0;

	list<MenuKeySettingItem*>::iterator ciIt;
	for(ciIt = items.begin(); ciIt != items.end(); ciIt++)
	{
		if((*ciIt)->keySetting.type == KEY_SETTING_HAT && joystick == (*ciIt)->keySetting.hat)
		{
			_choice = cur;
			break;
		}

		cur++;
	}
	RefreshKeySetting();
}

void MenuKeySetting::SetByStick(Uint8 joystick)
{
	int cur = 0;

	list<MenuKeySettingItem*>::iterator ciIt;
	for(ciIt = items.begin(); ciIt != items.end(); ciIt++)
	{
		if((*ciIt)->keySetting.type == KEY_SETTING_STICK && joystick == (*ciIt)->keySetting.stick)
		{
			_choice = cur;
			break;
		}

		cur++;
	}
	RefreshKeySetting();
}

void MenuKeySetting::SetEnter()
{
	int cur = 0;

	list<MenuKeySettingItem*>::iterator ciIt;
	for(ciIt = items.begin(); ciIt != items.end(); ciIt++)
	{
		if((*ciIt)->keySetting.type == KEY_SETTING_ENTER)
		{
			_choice = cur;
			break;
		}

		cur++;
	}
	RefreshKeySetting();
}

void MenuKeySetting::SetNoSetting()
{
	int cur = 0;

	list<MenuKeySettingItem*>::iterator ciIt;
	for(ciIt = items.begin(); ciIt != items.end(); ciIt++)
	{
		if((*ciIt)->keySetting.type == KEY_SETTING_NONE)
		{
			_choice = cur;
			break;
		}

		cur++;
	}
	RefreshKeySetting();
}

void MenuKeySetting::RefreshKeySetting()
{
	if(_visible)
	{
		int cur = 0;

		list<MenuKeySettingItem*>::iterator ciIt;
		for(ciIt = items.begin(); ciIt != items.end(); ciIt++)
		{
			if(cur == _choice)
			{
				SetText((*ciIt)->itemText);
				break;
			}

			cur++;
		}
	}
	else
	{
		DeleteText();
	}

	parentMenuItem->ChooserOrKeySettingChanged();
}

void MenuKeySetting::SetVisible(bool visible)
{
	_visible = visible;
	RefreshKeySetting();
}

//////////////
//HSMenuItem//
//////////////

HSMenuItem::HSMenuItem(HSFont * font) : HSText(font)
{
	itemText = "";
	childNeedToSpawnText = false;
	chooserOrKeySettingNeedToSpawnText = false;
	parentMenu = NULL;
	child = NULL;
	chooser = NULL;
	order = 0;
	function = NO_MENU_FUNCTION;
	currentKeySetting = NULL;
	itemHeight = font->charHeight;
	_visible = false;
}

HSMenuItem::~HSMenuItem()
{
	if(child != NULL)
	{
		delete child;
	}

	if(chooser != NULL)
	{
		delete chooser;
	}

	if(currentKeySetting != NULL)
	{
		delete currentKeySetting;
	}
}

void HSMenuItem::SetByChoiceFunction(MenuChooserFunction function)
{
	if(chooser == NULL) { return; }

	chooser->SetByChoiceFunction(function);
}

void HSMenuItem::SetByKey(SDL_Keycode keycode)
{
	if(currentKeySetting == NULL) { return; }

	currentKeySetting->SetByKey(keycode);
}

void HSMenuItem::SetByButton(Uint8 joystick, Uint8 button)
{
	if(currentKeySetting == NULL) { return; }

	currentKeySetting->SetByButton(joystick, button);
}

void HSMenuItem::SetByHat(Uint8 joystick)
{
	if(currentKeySetting == NULL) { return; }

	currentKeySetting->SetByHat(joystick);
}

void HSMenuItem::SetByStick(Uint8 joystick)
{
	if(currentKeySetting == NULL) { return; }

	currentKeySetting->SetByStick(joystick);
}

void HSMenuItem::SetEnter()
{
	if(currentKeySetting == NULL) { return; }

	currentKeySetting->SetEnter();
}

void HSMenuItem::SetNoSetting()
{
	if(currentKeySetting == NULL) { return; }

	currentKeySetting->SetNoSetting();
}

void HSMenuItem::RefreshMenuItem()
{
	if(_visible)
	{
		SetText(itemText);
	}
	else
	{
		DeleteText();
		childNeedToSpawnText = false;
		chooserOrKeySettingNeedToSpawnText = false;
	}

	if(chooser != NULL)
	{
		chooser->SetVisible(_visible);
	}
	if(currentKeySetting != NULL)
	{
		currentKeySetting->SetVisible(_visible);
	}
}

void HSMenuItem::SetVisible(bool visible)
{
	_visible = visible;
	RefreshMenuItem();
}

void HSMenuItem::ChildChanged()
{
	childNeedToSpawnText = true;
	parentMenu->ItemChanged();
}

void HSMenuItem::ChooserOrKeySettingChanged()
{
	chooserOrKeySettingNeedToSpawnText = true;
	parentMenu->ItemChanged();
}

//////////
//HSMenu//
//////////

HSMenu::HSMenu(HSFont * font) : HSText(font)
{
	titleText = "";
	itemNeedToSpawnText = false;
	parentMenuItem = NULL;
	cursorToClone = NULL;
	activeCursor = NULL;
	cursorIndex = 0;
	items.clear();
	titleHeight = font->charHeight;
	titleSeparation = 0;
	cursorWidth = 0;
	cursorSeparation = 0;
	itemHeight = 0;
	itemSeparation = 0;
	_visible = false;
}

HSMenu::~HSMenu()
{
	if(cursorToClone != NULL)
	{
		delete cursorToClone;
	}

	list<HSMenuItem*>::iterator itemItr;
	for(itemItr = items.begin(); itemItr != items.end(); itemItr++)
	{
		delete *itemItr;
	}
}

HSMenuItem * HSMenu::GetCurrentItem()
{
	int count = 0;
	list<HSMenuItem*>::iterator itemItr;
	for(itemItr = items.begin(); itemItr != items.end(); itemItr++)
	{
		if(count == cursorIndex)
		{
			return (*itemItr);
		}
		count++;
	}

	return NULL;
}

HSMenu * HSMenu::GetChild()
{
	int count = 0;
	list<HSMenuItem*>::iterator itemItr;
	for(itemItr = items.begin(); itemItr != items.end(); itemItr++)
	{
		if(count == cursorIndex)
		{
			return (*itemItr)->child;
		}
		count++;
	}

	return NULL;
}

HSMenuFunction HSMenu::GetCurrentFunction()
{
	HSMenuItem * item = GetCurrentItem();

	if(item == NULL) { return NO_MENU_FUNCTION; }

	return item->function;
}

MenuChooserFunction HSMenu::GetChooserFuncByMenuFunc(HSMenuFunction menuFunc)
{
	list<HSMenuItem*>::iterator itemItr;
	for(itemItr = items.begin(); itemItr != items.end(); itemItr++)
	{
		if(menuFunc == (*itemItr)->function && (*itemItr)->chooser != NULL)
		{
			return (*itemItr)->chooser->GetCurrentChooserFunction();
		}
	}

	return NO_CHOOSER_FUNCTION;
}

KeySetting HSMenu::GetKeyConfigByMenuFunc(HSMenuFunction menuFunc)
{
	list<HSMenuItem*>::iterator itemItr;
	for(itemItr = items.begin(); itemItr != items.end(); itemItr++)
	{
		if(menuFunc == (*itemItr)->function && (*itemItr)->currentKeySetting != NULL)
		{
			return (*itemItr)->currentKeySetting->GetCurretKeySetting();
		}
	}

	KeySetting setting;
	setting.type = KEY_SETTING_NONE;

	return setting;
}

void HSMenu::SetChoiceForItem(HSMenuFunction menuFunction, MenuChooserFunction chooserFunction)
{
	list<HSMenuItem*>::iterator itemItr;
	for(itemItr = items.begin(); itemItr != items.end(); itemItr++)
	{
		if(menuFunction == (*itemItr)->function)
		{
			(*itemItr)->SetByChoiceFunction(chooserFunction);
			break;
		}
	}
}

void HSMenu::SetKeyConfigByKeyForItem(HSMenuFunction menuFunction, SDL_Keycode keycode)
{
	list<HSMenuItem*>::iterator itemItr;
	for(itemItr = items.begin(); itemItr != items.end(); itemItr++)
	{
		if(menuFunction == (*itemItr)->function)
		{
			(*itemItr)->SetByKey(keycode);
			break;
		}
	}
}

void HSMenu::SetKeyConfigByButtonForItem(HSMenuFunction menuFunction, Uint8 joystick, Uint8 button)
{
	list<HSMenuItem*>::iterator itemItr;
	for(itemItr = items.begin(); itemItr != items.end(); itemItr++)
	{
		if(menuFunction == (*itemItr)->function)
		{
			(*itemItr)->SetByButton(joystick, button);
			break;
		}
	}
}

void HSMenu::SetKeyConfigByHatForItem(HSMenuFunction menuFunction, Uint8 joystick)
{
	list<HSMenuItem*>::iterator itemItr;
	for(itemItr = items.begin(); itemItr != items.end(); itemItr++)
	{
		if(menuFunction == (*itemItr)->function)
		{
			(*itemItr)->SetByHat(joystick);
			break;
		}
	}
}

void HSMenu::SetKeyConfigByStickForItem(HSMenuFunction menuFunction, Uint8 joystick)
{
	list<HSMenuItem*>::iterator itemItr;
	for(itemItr = items.begin(); itemItr != items.end(); itemItr++)
	{
		if(menuFunction == (*itemItr)->function)
		{
			(*itemItr)->SetByStick(joystick);
			break;
		}
	}
}

void HSMenu::SetKeyConfigEnterForItem(HSMenuFunction menuFunction)
{
	list<HSMenuItem*>::iterator itemItr;
	for(itemItr = items.begin(); itemItr != items.end(); itemItr++)
	{
		if(menuFunction == (*itemItr)->function)
		{
			(*itemItr)->SetEnter();
			break;
		}
	}
}

void HSMenu::SetKeyConfigNoSettingForItem(HSMenuFunction menuFunction)
{
	list<HSMenuItem*>::iterator itemItr;
	for(itemItr = items.begin(); itemItr != items.end(); itemItr++)
	{
		if(menuFunction == (*itemItr)->function)
		{
			(*itemItr)->SetNoSetting();
			break;
		}
	}
}

void HSMenu::ChoiceNext()
{
	HSMenuItem * curItem = GetCurrentItem();

	if(curItem == NULL || curItem->chooser == NULL) { return; }

	curItem->chooser->ChoiceNext();
}

void HSMenu::ChoicePrev()
{
	HSMenuItem * curItem = GetCurrentItem();

	if(curItem == NULL || curItem->chooser == NULL) { return; }

	curItem->chooser->ChoicePrev();
}

void HSMenu::CursorFirst()
{
	cursorIndex = 0;

	RepositionCursor();
}

void HSMenu::CursorNext()
{
	cursorIndex++;
	if(cursorIndex >= items.size())
	{
		cursorIndex = 0;
	}

	RepositionCursor();
}

void HSMenu::CursorPrev()
{
	cursorIndex--;
	if(cursorIndex < 0)
	{
		cursorIndex = items.size() - 1;
	}

	RepositionCursor();
}

void HSMenu::RefreshMenu()
{
	if(_visible)
	{
		SetText(titleText);
	}
	else
	{
		DeleteText();
		itemNeedToSpawnText = false;
		needToSpawnCursor = false;
	}

	if(activeCursor != NULL)
	{
		activeCursor->toDelete = true;
		activeCursor = NULL;
	}

	list<HSMenuItem*>::iterator itemItr;
	for(itemItr = items.begin(); itemItr != items.end(); itemItr++)
	{
		(*itemItr)->SetVisible(_visible);
	}
}

void HSMenu::SetVisible(bool visible)
{
	_visible = visible;
	RefreshMenu();
}

void HSMenu::RepositionCursor()
{
	if(cursorToClone != NULL)
	{
		cursorToClone->pos.x = pos.x;
		cursorToClone->pos.y = pos.y + titleSeparation + itemHeight + (itemHeight + itemSeparation) * cursorIndex;
	}
	
	if(activeCursor != NULL)
	{
		activeCursor->pos.x = pos.x;
		activeCursor->pos.y = pos.y + titleSeparation + itemHeight + (itemHeight + itemSeparation) * cursorIndex;
	}
}

void HSMenu::ItemChanged()
{
	itemNeedToSpawnText = true;

	if(parentMenuItem != NULL)
	{
		parentMenuItem->ChildChanged();
	}
}