#include "menu.h"

////////////
//MenuHold//
////////////

MenuHold::MenuHold() : HSObjectHold()
{

}

MenuHold::~MenuHold()
{

}

////////
//Menu//
////////

Menu::Menu(int headerHeight, int cursorWidth, int itemHeight, int itemSpacing) : HSObject()
{
	_parent = NULL;
	_header = NULL;
	_cursor = NULL;
	_cursorIndex = 0;
	_items.clear();
	SetDimensions(headerHeight, cursorWidth, itemHeight, itemSpacing);
}

Menu::~Menu()
{
	//several of the child menus could easily be the same actual menu, so we need to make sure we don't try to delete it more than once
	list<Menu*> deleted;
	deleted.clear();

	list<MenuItem>::iterator itemItr;
	for(itemItr = _items.begin(); itemItr != _items.end(); itemItr++)
	{
		bool deleteIt = true;
		list<Menu*>::iterator delItr;
		for(delItr = deleted.begin(); delItr != deleted.end(); delItr++)
		{
			if(*delItr == (*itemItr).child)
			{
				deleteIt = false;
				break;
			}
		}

		if(deleteIt)
		{
			deleted.push_back((*itemItr).child);
			delete (*itemItr).child;
		}
	}
}

int Menu::GetCursorIndex()
{
	return _cursorIndex;
}

HSVect2D Menu::GetCursorPos()
{
	if(_cursor == NULL) { HSVect2D returnVal; returnVal.x = 0; returnVal.y = 0; return returnVal; }

	return _cursor->pos;
}

Menu * Menu::GetParent()
{
	return _parent;
}

Menu * Menu::GetChild()
{
	int count = 0;
	list<MenuItem>::iterator itemItr;
	for(itemItr = _items.begin(); itemItr != _items.end(); itemItr++)
	{
		if(count == _cursorIndex)
		{
			return (*itemItr).child;
		}
		count++;
	}

	return NULL;
}

list<MenuItem> Menu::GetItems()
{
	return _items;
}

void Menu::SetParent(Menu * parent)
{
	_parent = parent;
}

void Menu::SetDimensions(int headerHeight, int cursorWidth, int itemHeight, int itemSpacing)
{
	_headerHeight = headerHeight;
	_cursorWidth = cursorWidth;
	_itemHeight = itemHeight;
	_itemSpacing = itemSpacing;
}

void Menu::SetHeader(HSObject * header)
{
	if(_header != NULL) { _header->visible = false; }
	header->pos.x = pos.x;
	header->pos.y = pos.y;
	header->visible = visible;
	_header = header;
}

void Menu::SetCursor(HSObject * cursor)
{
	if(_cursor != NULL) { _cursor->visible = false; }
	cursor->visible = visible;
	_cursor = cursor;
	RepositionCursor();
}

void Menu::AddItem(MenuItem item)
{
	item.graphic->pos.x = pos.x + _cursorWidth + _itemSpacing;
	item.graphic->pos.y = pos.y + _headerHeight + _itemSpacing + (_itemHeight + _itemSpacing) * _items.size();

	if(item.child != NULL) { item.child->SetParent(this); }
	_items.push_back(item);
}

void Menu::CursorFirst()
{
	_cursorIndex = 0;

	RepositionCursor();
}

void Menu::CursorNext()
{
	_cursorIndex++;
	if(_cursorIndex >= _items.size())
	{
		_cursorIndex = 0;
	}

	RepositionCursor();
}

void Menu::CursorPrev()
{
	_cursorIndex--;
	if(_cursorIndex < 0)
	{
		_cursorIndex = _items.size() - 1;
	}

	RepositionCursor();
}

void Menu::SetVisible(bool visible)
{
	visible = visible;
	if(_header != NULL) { _header->visible = visible; }
	if(_cursor != NULL) { _cursor->visible = visible; }
	list<MenuItem>::iterator itemItr;
	for(itemItr = _items.begin(); itemItr != _items.end(); itemItr++)
	{
		(*itemItr).graphic->visible = visible;
	}
}

void Menu::RepositionCursor()
{
	_cursor->pos.x = pos.x;
	_cursor->pos.y = pos.y + _headerHeight + _itemSpacing + (_itemHeight + _itemSpacing) * _cursorIndex;
}