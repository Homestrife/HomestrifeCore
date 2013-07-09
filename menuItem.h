#ifndef __MENUITEM_H_
#define __MENUITEM_H_

#include "hsobject.h"
#include "menu.h"

class MenuItem
{
public:
	MenuItem(HSObject * grahpic);
	~MenuItem();

	Menu * GetChild();
	HSObject * GetGraphic();

	void SetChild(Menu * child);
	void SetGraphic(HSObject * graphic);

protected:
	Menu * _child;
	HSObject * _graphic;
};

#endif