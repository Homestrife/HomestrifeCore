#ifndef __MENU_H_
#define __MENU_H_

#include "hsobject.h"

class Menu;

struct MenuItem
{
	HSObject * graphic;
	Menu * child;
};

class MenuHold : public HSObjectHold
{
public:
	MenuHold();
	~MenuHold();

protected:

};

class Menu : public HSObject
{
public:
	Menu(int headerHeight, int cursorWidth, int itemHeight, int itemSpacing);
	~Menu();

	int GetCursorIndex();
	HSVect2D GetCursorPos();
	Menu * GetParent();
	Menu * GetChild();
	list<MenuItem> GetItems();

	void SetParent(Menu * parent);
	void SetDimensions(int headerHeight, int cursorWidth, int itemHeight, int itemSpacing);
	void SetHeader(HSObject * header);
	void SetCursor(HSObject * cursor);
	void AddItem(MenuItem item);
	void CursorFirst();
	void CursorNext();
	void CursorPrev();
	void SetVisible(bool visible);

protected:
	Menu * _parent;
	HSObject * _header;
	HSObject * _cursor;
	int _cursorIndex;
	list<MenuItem> _items;

	int _headerHeight;
	int _cursorWidth;
	int _itemHeight;
	int _itemSpacing;

	void RepositionCursor();
};

#endif