#include "HSText.h"

HSText::HSText(HSFont * font)
{
	palette = 0;
	pos.x = 0;
	pos.y = 0;
	_font = font;
	characterList.clear();
	charListToClone.clear();
	justification = JUSTIFICATION_LEFT;
	depth = 0;
	_curText = "";
}

HSText::~HSText()
{

}

void HSText::SetText(string text)
{
	//don't bother deleting an recreating objects if the text isn't actually different
	if(_curText.compare(text) == 0) { return; }

	DeleteText();
	if(_font != NULL)
	{
		charListToClone = _font->GenerateText(text, pos, depth, justification, palette);
	}
	_curText = text;
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
	charListToClone.clear();
	_curText = "";
}