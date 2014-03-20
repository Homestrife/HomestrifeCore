#ifndef __HSTEXT_H_
#define __HSTEXT_H_

#include "hsfont.h"

class HSText
{
public:
	HSVect2D pos;
	list<HSCharToClone> charListToClone;
	list<HSObject*> characterList;

	HSText(HSFont * font);
	~HSText();

	void SetText(string text);
	void DeleteText();
	void SetVisible(bool visible);

protected:
	HSFont * _font;
};

#endif