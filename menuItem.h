#ifndef __MENUITEM_H_
#define __MENUITEM_H_

#include "hsobject.h"
#include "menu.h"

class MenuItem
{
public:
	MenuItem(HSObject * grahpic);
	~MenuItem();

	HSMenu * GetChild();
	HSObject * GetGraphic();

	void SetChild(HSMenu * child);
	void SetGraphic(HSObject * graphic);

protected:
	HSMenu * _child;
	HSObject * _graphic;
};

#endif