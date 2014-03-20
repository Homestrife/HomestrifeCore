#include "HSText.h"

HSText::HSText(HSFont * font)
{
	pos.x = 0;
	pos.y = 0;
	_font = font;
	characterList.clear();
	charListToClone.clear();
}

HSText::~HSText()
{
	/*list<HSObject*>::iterator obIt;
	for(obIt = characterList.begin(); obIt != characterList.end(); obIt++)
	{
		delete *obIt;
	}*/

	if(_font != NULL)
	{
		_font->usingCount--;
		if(_font->usingCount < 0)
		{
			_font->usingCount = 0;
		}
	}
}

void HSText::SetText(string text)
{
	DeleteText();
	charListToClone = _font->GenerateText(text, pos);
}

void HSText::DeleteText()
{
	list<HSObject*>::iterator obIt;
	for(obIt = characterList.begin(); obIt != characterList.end(); obIt++)
	{
		if(*obIt != NULL)
		{
			(*obIt)->toDelete = true;
		}
	}

	characterList.clear();
}