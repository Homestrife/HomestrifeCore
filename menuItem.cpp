#include "menuItem.h"

////////////
//MenuItem//
////////////

MenuItem::MenuItem(HSObject * graphic)
{
	_child = NULL;
	_graphic = graphic;
}

MenuItem::~MenuItem()
{

}

Menu * MenuItem::GetChild()
{
	return _child;
}

HSObject * MenuItem::GetGraphic()
{
	return _graphic;
}

void MenuItem::SetChild(Menu * child)
{
	_child = child;
}

void MenuItem::SetGraphic(HSObject * graphic)
{
	_graphic = graphic;
}