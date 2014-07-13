#ifndef __HSFONT_H_
#define __HSFONT_H_

#include "hsobject.h"

enum TextJustification
{
	JUSTIFICATION_LEFT,
	JUSTIFICATION_CENTER,
	JUSTIFICATION_RIGHT
};

struct HSCharToClone
{
	HSObject * character;
	HSVect2D pos;
	float width;
	int depth;
};

struct HSCharacter
{
	HSObject * character;
	float charWidth;
};

struct Characters
{
	HSCharacter lcA;
	HSCharacter lcB;
	HSCharacter lcC;
	HSCharacter lcD;
	HSCharacter lcE;
	HSCharacter lcF;
	HSCharacter lcG;
	HSCharacter lcH;
	HSCharacter lcI;
	HSCharacter lcJ;
	HSCharacter lcK;
	HSCharacter lcL;
	HSCharacter lcM;
	HSCharacter lcN;
	HSCharacter lcO;
	HSCharacter lcP;
	HSCharacter lcQ;
	HSCharacter lcR;
	HSCharacter lcS;
	HSCharacter lcT;
	HSCharacter lcU;
	HSCharacter lcV;
	HSCharacter lcW;
	HSCharacter lcX;
	HSCharacter lcY;
	HSCharacter lcZ;
	
	HSCharacter ucA;
	HSCharacter ucB;
	HSCharacter ucC;
	HSCharacter ucD;
	HSCharacter ucE;
	HSCharacter ucF;
	HSCharacter ucG;
	HSCharacter ucH;
	HSCharacter ucI;
	HSCharacter ucJ;
	HSCharacter ucK;
	HSCharacter ucL;
	HSCharacter ucM;
	HSCharacter ucN;
	HSCharacter ucO;
	HSCharacter ucP;
	HSCharacter ucQ;
	HSCharacter ucR;
	HSCharacter ucS;
	HSCharacter ucT;
	HSCharacter ucU;
	HSCharacter ucV;
	HSCharacter ucW;
	HSCharacter ucX;
	HSCharacter ucY;
	HSCharacter ucZ;

	HSCharacter num0;
	HSCharacter num1;
	HSCharacter num2;
	HSCharacter num3;
	HSCharacter num4;
	HSCharacter num5;
	HSCharacter num6;
	HSCharacter num7;
	HSCharacter num8;
	HSCharacter num9;

	HSCharacter exclamation;
	HSCharacter ampersand;
	HSCharacter parenLeft;
	HSCharacter parenRight;
	HSCharacter backslash;
	HSCharacter forwardslash;
	HSCharacter colon;
	HSCharacter quoteSingle;
	HSCharacter quoteDouble;
	HSCharacter comma;
	HSCharacter period;
	HSCharacter question;
	HSCharacter lessThan;
	HSCharacter greaterThan;
	HSCharacter at;
	HSCharacter hash;
	HSCharacter dollar;
	HSCharacter percent;
	HSCharacter carat;
	HSCharacter asterisk;
	HSCharacter minus;
	HSCharacter underscore;
	HSCharacter plus;
	HSCharacter equals;
	HSCharacter bracketLeft;
	HSCharacter bracketRight;
	HSCharacter semicolon;
	HSCharacter quoteBack;

	HSCharacter space;
};

class HSFont
{
public:
	string fontFilePath;
	Characters characters;
	float charHeight;
	float charSeparation;

	HSFont();
	~HSFont();

	list<HSCharToClone> GenerateText(string text, HSVect2D pos, int depth, TextJustification justification);
	void GenerateCharacter(HSCharacter character, float * textWidth, int depth, list<HSCharToClone> * charList);

protected:
};

#endif