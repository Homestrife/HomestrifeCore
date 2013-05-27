#include "menuManager.h"

MenuManager::MenuManager(Menu * root)
{
	_root = root;
	_current = root;
	_hidden = false;
	RefreshVisibility();
}

MenuManager::~MenuManager()
{
	delete _root;
}

int MenuManager::GetCursorIndex()
{
	if(_current == NULL) { return 0; }

	return _current->GetCursorIndex();
}

HSVect2D MenuManager::GetCursorPos()
{
	if(_current == NULL) { HSVect2D returnVal; returnVal.x = 0; returnVal.y = 0; return returnVal; }

	return _current->GetCursorPos();
}

void MenuManager::SetHeader(HSObject * header)
{
	if(_current == NULL) { return; }

	_current->SetHeader(header);
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
	if(_current == NULL || _current->GetParent() == NULL) { return; }

	_current->CursorFirst();
	_current = _current->GetParent();
	RefreshVisibility();
}


void MenuManager::ToChild()
{
	if(_current == NULL || _current->GetChild() == NULL) { return; }

	_current = _current->GetChild();
	RefreshVisibility();
}

void MenuManager::RefreshVisibility()
{
	if(_root == NULL) { return; }
	SetInvisible(_root);

	if(_current == NULL) { _current = _root; }

	if(_hidden) { return; }

	_current->SetVisible(true);
}

void MenuManager::Hide(bool hide)
{
	_hidden = hide;
	RefreshVisibility();
}

MenuItem MenuManager::MakeMenuItem(HSObject * graphic)
{
	return MakeMenuItem(graphic, NULL);
}

MenuItem MenuManager::MakeMenuItem(HSObject * graphic, Menu * child)
{
	MenuItem item;
	item.graphic = graphic;
	item.child = child;

	return item;
}

void MenuManager::SetInvisible(Menu * menu)
{
	if(menu == NULL) { return; }

	menu->SetVisible(false);

	list<MenuItem> items = menu->GetItems();
	list<MenuItem>::iterator itemItr;
	for(itemItr = items.begin(); itemItr != items.end(); itemItr++)
	{
		SetInvisible((*itemItr).child);
	}
}