#ifndef __MENUMANAGER_H_
#define __MENUMANAGER_H_

#include "menu.h"

class MenuManager
{
public:
	MenuManager(Menu * root);
	~MenuManager();

	int GetCursorIndex();
	HSVect2D GetCursorPos();

	void SetHeader(HSObject * header);
	void CursorNext();
	void CursorPrev();
	void ToParent();
	void ToChild();
	void RefreshVisibility();
	void Hide(bool hide);
	MenuItem MakeMenuItem(HSObject * graphic);
	MenuItem MakeMenuItem(HSObject * graphic, Menu * child);

protected:
	Menu * _root;
	Menu * _current;
	bool _hidden;

	void SetInvisible(Menu * menu);
	void DeleteMenu(Menu * menu);
};

#endif