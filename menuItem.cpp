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

HSMenu * MenuItem::GetChild()
{
	return _child;
}

HSObject * MenuItem::GetGraphic()
{
	return _graphic;
}

void MenuItem::SetChild(HSMenu * child)
{
	_child = child;
}

void MenuItem::SetGraphic(HSObject * graphic)
{
	_graphic = graphic;
}