#ifndef __HSTEXT_H_
#define __HSTEXT_H_

#include "hsfont.h"

class HSText : public HSOrderable
{
public:
	int palette;
	HSVect2D pos;
	list<HSCharToClone> charListToClone;
	list<HSObject*> characterList;
	TextJustification justification;
	int depth;

	HSText(HSFont * font);
	~HSText();

	void SetText(string text);
	void DeleteText();

protected:
	HSFont * _font;
	string _curText;
};

#endif