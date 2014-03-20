#ifndef __HSFONT_H_
#define __HSFONT_H_

#include "hsobject.h"

struct HSCharToClone
{
	HSObject * character;
	HSVect2D pos;
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
};

class HSFont
{
public:
	string fontFilePath;
	int usingCount;
	Characters characters;
	float charHeight;
	float charSeparation;
	float spaceWidth;

	HSFont();
	~HSFont();

	list<HSCharToClone> GenerateText(string text, HSVect2D pos);
	void GenerateCharacter(HSCharacter character, HSVect2D * pos, list<HSCharToClone> * charList);

protected:
};

#endif