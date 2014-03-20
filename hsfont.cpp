#include "hsfont.h"

HSFont::HSFont()
{
	fontFilePath = "";
	usingCount = 0;

	characters.ampersand.charWidth = 0;
	characters.ampersand.character = NULL;
	characters.backslash.charWidth = 0;
	characters.backslash.character = NULL;
	characters.colon.charWidth = 0;
	characters.colon.character = NULL;
	characters.comma.charWidth = 0;
	characters.comma.character = NULL;
	characters.exclamation.charWidth = 0;
	characters.exclamation.character = NULL;
	characters.forwardslash.charWidth = 0;
	characters.forwardslash.character = NULL;
	characters.greaterThan.charWidth = 0;
	characters.greaterThan.character = NULL;
	characters.lessThan.charWidth = 0;
	characters.lessThan.character = NULL;
	characters.parenLeft.charWidth = 0;
	characters.parenLeft.character = NULL;
	characters.parenRight.charWidth = 0;
	characters.parenRight.character = NULL;
	characters.period.charWidth = 0;
	characters.period.character = NULL;
	characters.question.charWidth = 0;
	characters.question.character = NULL;
	characters.quoteDouble.charWidth = 0;
	characters.quoteDouble.character = NULL;
	characters.quoteSingle.charWidth = 0;
	characters.quoteSingle.character = NULL;

	characters.num0.charWidth = 0;
	characters.num0.character = NULL;
	characters.num1.charWidth = 0;
	characters.num1.character = NULL;
	characters.num2.charWidth = 0;
	characters.num2.character = NULL;
	characters.num3.charWidth = 0;
	characters.num3.character = NULL;
	characters.num4.charWidth = 0;
	characters.num4.character = NULL;
	characters.num5.charWidth = 0;
	characters.num5.character = NULL;
	characters.num6.charWidth = 0;
	characters.num6.character = NULL;
	characters.num7.charWidth = 0;
	characters.num7.character = NULL;
	characters.num8.charWidth = 0;
	characters.num8.character = NULL;
	characters.num9.charWidth = 0;
	characters.num9.character = NULL;
	
	characters.lcA.charWidth = 0;
	characters.lcA.character = NULL;
	characters.lcB.charWidth = 0;
	characters.lcB.character = NULL;
	characters.lcC.charWidth = 0;
	characters.lcC.character = NULL;
	characters.lcD.charWidth = 0;
	characters.lcD.character = NULL;
	characters.lcE.charWidth = 0;
	characters.lcE.character = NULL;
	characters.lcF.charWidth = 0;
	characters.lcF.character = NULL;
	characters.lcG.charWidth = 0;
	characters.lcG.character = NULL;
	characters.lcH.charWidth = 0;
	characters.lcH.character = NULL;
	characters.lcI.charWidth = 0;
	characters.lcI.character = NULL;
	characters.lcJ.charWidth = 0;
	characters.lcJ.character = NULL;
	characters.lcK.charWidth = 0;
	characters.lcK.character = NULL;
	characters.lcL.charWidth = 0;
	characters.lcL.character = NULL;
	characters.lcM.charWidth = 0;
	characters.lcM.character = NULL;
	characters.lcN.charWidth = 0;
	characters.lcN.character = NULL;
	characters.lcO.charWidth = 0;
	characters.lcO.character = NULL;
	characters.lcP.charWidth = 0;
	characters.lcP.character = NULL;
	characters.lcQ.charWidth = 0;
	characters.lcQ.character = NULL;
	characters.lcR.charWidth = 0;
	characters.lcR.character = NULL;
	characters.lcS.charWidth = 0;
	characters.lcS.character = NULL;
	characters.lcT.charWidth = 0;
	characters.lcT.character = NULL;
	characters.lcU.charWidth = 0;
	characters.lcU.character = NULL;
	characters.lcV.charWidth = 0;
	characters.lcV.character = NULL;
	characters.lcW.charWidth = 0;
	characters.lcW.character = NULL;
	characters.lcX.charWidth = 0;
	characters.lcX.character = NULL;
	characters.lcY.charWidth = 0;
	characters.lcY.character = NULL;
	characters.lcZ.charWidth = 0;
	characters.lcZ.character = NULL;
	
	characters.ucA.charWidth = 0;
	characters.ucA.character = NULL;
	characters.ucB.charWidth = 0;
	characters.ucB.character = NULL;
	characters.ucC.charWidth = 0;
	characters.ucC.character = NULL;
	characters.ucD.charWidth = 0;
	characters.ucD.character = NULL;
	characters.ucE.charWidth = 0;
	characters.ucE.character = NULL;
	characters.ucF.charWidth = 0;
	characters.ucF.character = NULL;
	characters.ucG.charWidth = 0;
	characters.ucG.character = NULL;
	characters.ucH.charWidth = 0;
	characters.ucH.character = NULL;
	characters.ucI.charWidth = 0;
	characters.ucI.character = NULL;
	characters.ucJ.charWidth = 0;
	characters.ucJ.character = NULL;
	characters.ucK.charWidth = 0;
	characters.ucK.character = NULL;
	characters.ucL.charWidth = 0;
	characters.ucL.character = NULL;
	characters.ucM.charWidth = 0;
	characters.ucM.character = NULL;
	characters.ucN.charWidth = 0;
	characters.ucN.character = NULL;
	characters.ucO.charWidth = 0;
	characters.ucO.character = NULL;
	characters.ucP.charWidth = 0;
	characters.ucP.character = NULL;
	characters.ucQ.charWidth = 0;
	characters.ucQ.character = NULL;
	characters.ucR.charWidth = 0;
	characters.ucR.character = NULL;
	characters.ucS.charWidth = 0;
	characters.ucS.character = NULL;
	characters.ucT.charWidth = 0;
	characters.ucT.character = NULL;
	characters.ucU.charWidth = 0;
	characters.ucU.character = NULL;
	characters.ucV.charWidth = 0;
	characters.ucV.character = NULL;
	characters.ucW.charWidth = 0;
	characters.ucW.character = NULL;
	characters.ucX.charWidth = 0;
	characters.ucX.character = NULL;
	characters.ucY.charWidth = 0;
	characters.ucY.character = NULL;
	characters.ucZ.charWidth = 0;
	characters.ucZ.character = NULL;

	charHeight = 0;
	charSeparation = 0;
	spaceWidth = 0;
}

HSFont::~HSFont()
{
	delete characters.ampersand.character;
	delete characters.backslash.character;
	delete characters.colon.character;
	delete characters.comma.character;
	delete characters.exclamation.character;
	delete characters.forwardslash.character;
	delete characters.greaterThan.character;
	delete characters.lessThan.character;
	delete characters.parenLeft.character;
	delete characters.parenRight.character;
	delete characters.period.character;
	delete characters.question.character;
	delete characters.quoteDouble.character;
	delete characters.quoteSingle.character;

	delete characters.num0.character;
	delete characters.num1.character;
	delete characters.num2.character;
	delete characters.num3.character;
	delete characters.num4.character;
	delete characters.num5.character;
	delete characters.num6.character;
	delete characters.num7.character;
	delete characters.num8.character;
	delete characters.num9.character;
	
	delete characters.lcA.character;
	delete characters.lcB.character;
	delete characters.lcC.character;
	delete characters.lcD.character;
	delete characters.lcE.character;
	delete characters.lcF.character;
	delete characters.lcG.character;
	delete characters.lcH.character;
	delete characters.lcI.character;
	delete characters.lcJ.character;
	delete characters.lcK.character;
	delete characters.lcL.character;
	delete characters.lcM.character;
	delete characters.lcN.character;
	delete characters.lcO.character;
	delete characters.lcP.character;
	delete characters.lcQ.character;
	delete characters.lcR.character;
	delete characters.lcS.character;
	delete characters.lcT.character;
	delete characters.lcU.character;
	delete characters.lcV.character;
	delete characters.lcW.character;
	delete characters.lcX.character;
	delete characters.lcY.character;
	delete characters.lcZ.character;
	
	delete characters.ucA.character;
	delete characters.ucB.character;
	delete characters.ucC.character;
	delete characters.ucD.character;
	delete characters.ucE.character;
	delete characters.ucF.character;
	delete characters.ucG.character;
	delete characters.ucH.character;
	delete characters.ucI.character;
	delete characters.ucJ.character;
	delete characters.ucK.character;
	delete characters.ucL.character;
	delete characters.ucM.character;
	delete characters.ucN.character;
	delete characters.ucO.character;
	delete characters.ucP.character;
	delete characters.ucQ.character;
	delete characters.ucR.character;
	delete characters.ucS.character;
	delete characters.ucT.character;
	delete characters.ucU.character;
	delete characters.ucV.character;
	delete characters.ucW.character;
	delete characters.ucX.character;
	delete characters.ucY.character;
	delete characters.ucZ.character;
}

list<HSCharToClone> HSFont::GenerateText(string text, HSVect2D pos)
{
	list<HSCharToClone> result;
	string::iterator stIt;

	for(stIt = text.begin(); stIt != text.end(); stIt++)
	{
		if(*stIt == 'a') { GenerateCharacter(characters.lcA, &pos, &result); }
		else if(*stIt == 'b') { GenerateCharacter(characters.lcB, &pos, &result); }
		else if(*stIt == 'c') { GenerateCharacter(characters.lcC, &pos, &result); }
		else if(*stIt == 'd') { GenerateCharacter(characters.lcD, &pos, &result); }
		else if(*stIt == 'e') { GenerateCharacter(characters.lcE, &pos, &result); }
		else if(*stIt == 'f') { GenerateCharacter(characters.lcF, &pos, &result); }
		else if(*stIt == 'g') { GenerateCharacter(characters.lcG, &pos, &result); }
		else if(*stIt == 'h') { GenerateCharacter(characters.lcH, &pos, &result); }
		else if(*stIt == 'i') { GenerateCharacter(characters.lcI, &pos, &result); }
		else if(*stIt == 'j') { GenerateCharacter(characters.lcJ, &pos, &result); }
		else if(*stIt == 'k') { GenerateCharacter(characters.lcK, &pos, &result); }
		else if(*stIt == 'l') { GenerateCharacter(characters.lcL, &pos, &result); }
		else if(*stIt == 'm') { GenerateCharacter(characters.lcM, &pos, &result); }
		else if(*stIt == 'n') { GenerateCharacter(characters.lcN, &pos, &result); }
		else if(*stIt == 'o') { GenerateCharacter(characters.lcO, &pos, &result); }
		else if(*stIt == 'p') { GenerateCharacter(characters.lcP, &pos, &result); }
		else if(*stIt == 'q') { GenerateCharacter(characters.lcQ, &pos, &result); }
		else if(*stIt == 'r') { GenerateCharacter(characters.lcR, &pos, &result); }
		else if(*stIt == 's') { GenerateCharacter(characters.lcS, &pos, &result); }
		else if(*stIt == 't') { GenerateCharacter(characters.lcT, &pos, &result); }
		else if(*stIt == 'u') { GenerateCharacter(characters.lcU, &pos, &result); }
		else if(*stIt == 'v') { GenerateCharacter(characters.lcV, &pos, &result); }
		else if(*stIt == 'w') { GenerateCharacter(characters.lcW, &pos, &result); }
		else if(*stIt == 'x') { GenerateCharacter(characters.lcX, &pos, &result); }
		else if(*stIt == 'y') { GenerateCharacter(characters.lcY, &pos, &result); }
		else if(*stIt == 'z') { GenerateCharacter(characters.lcZ, &pos, &result); }
		else if(*stIt == 'A') { GenerateCharacter(characters.ucA, &pos, &result); }
		else if(*stIt == 'B') { GenerateCharacter(characters.ucB, &pos, &result); }
		else if(*stIt == 'C') { GenerateCharacter(characters.ucC, &pos, &result); }
		else if(*stIt == 'D') { GenerateCharacter(characters.ucD, &pos, &result); }
		else if(*stIt == 'E') { GenerateCharacter(characters.ucE, &pos, &result); }
		else if(*stIt == 'F') { GenerateCharacter(characters.ucF, &pos, &result); }
		else if(*stIt == 'G') { GenerateCharacter(characters.ucG, &pos, &result); }
		else if(*stIt == 'H') { GenerateCharacter(characters.ucH, &pos, &result); }
		else if(*stIt == 'I') { GenerateCharacter(characters.ucI, &pos, &result); }
		else if(*stIt == 'J') { GenerateCharacter(characters.ucJ, &pos, &result); }
		else if(*stIt == 'K') { GenerateCharacter(characters.ucK, &pos, &result); }
		else if(*stIt == 'L') { GenerateCharacter(characters.ucL, &pos, &result); }
		else if(*stIt == 'M') { GenerateCharacter(characters.ucM, &pos, &result); }
		else if(*stIt == 'N') { GenerateCharacter(characters.ucN, &pos, &result); }
		else if(*stIt == 'O') { GenerateCharacter(characters.ucO, &pos, &result); }
		else if(*stIt == 'P') { GenerateCharacter(characters.ucP, &pos, &result); }
		else if(*stIt == 'Q') { GenerateCharacter(characters.ucQ, &pos, &result); }
		else if(*stIt == 'R') { GenerateCharacter(characters.ucR, &pos, &result); }
		else if(*stIt == 'S') { GenerateCharacter(characters.ucS, &pos, &result); }
		else if(*stIt == 'T') { GenerateCharacter(characters.ucT, &pos, &result); }
		else if(*stIt == 'U') { GenerateCharacter(characters.ucU, &pos, &result); }
		else if(*stIt == 'V') { GenerateCharacter(characters.ucV, &pos, &result); }
		else if(*stIt == 'W') { GenerateCharacter(characters.ucW, &pos, &result); }
		else if(*stIt == 'X') { GenerateCharacter(characters.ucX, &pos, &result); }
		else if(*stIt == 'Y') { GenerateCharacter(characters.ucY, &pos, &result); }
		else if(*stIt == 'Z') { GenerateCharacter(characters.ucZ, &pos, &result); }
		else if(*stIt == '0') { GenerateCharacter(characters.num0, &pos, &result); }
		else if(*stIt == '1') { GenerateCharacter(characters.num1, &pos, &result); }
		else if(*stIt == '2') { GenerateCharacter(characters.num2, &pos, &result); }
		else if(*stIt == '3') { GenerateCharacter(characters.num3, &pos, &result); }
		else if(*stIt == '4') { GenerateCharacter(characters.num4, &pos, &result); }
		else if(*stIt == '5') { GenerateCharacter(characters.num5, &pos, &result); }
		else if(*stIt == '6') { GenerateCharacter(characters.num6, &pos, &result); }
		else if(*stIt == '7') { GenerateCharacter(characters.num7, &pos, &result); }
		else if(*stIt == '8') { GenerateCharacter(characters.num8, &pos, &result); }
		else if(*stIt == '9') { GenerateCharacter(characters.num9, &pos, &result); }
		else if(*stIt == '!') { GenerateCharacter(characters.exclamation, &pos, &result); }
		else if(*stIt == '&') { GenerateCharacter(characters.ampersand, &pos, &result); }
		else if(*stIt == '(') { GenerateCharacter(characters.parenLeft, &pos, &result); }
		else if(*stIt == ')') { GenerateCharacter(characters.parenRight, &pos, &result); }
		else if(*stIt == '\\') { GenerateCharacter(characters.backslash, &pos, &result); }
		else if(*stIt == '/') { GenerateCharacter(characters.forwardslash, &pos, &result); }
		else if(*stIt == ':') { GenerateCharacter(characters.colon, &pos, &result); }
		else if(*stIt == '\'') { GenerateCharacter(characters.quoteSingle, &pos, &result); }
		else if(*stIt == '"') { GenerateCharacter(characters.quoteDouble, &pos, &result); }
		else if(*stIt == ',') { GenerateCharacter(characters.comma, &pos, &result); }
		else if(*stIt == '.') { GenerateCharacter(characters.period, &pos, &result); }
		else if(*stIt == '?') { GenerateCharacter(characters.question, &pos, &result); }
		else if(*stIt == '<') { GenerateCharacter(characters.lessThan, &pos, &result); }
		else if(*stIt == '>') { GenerateCharacter(characters.greaterThan, &pos, &result); }
		else if(*stIt == ' ') { pos.x += spaceWidth + charSeparation; }
	}

	return result;
}



void HSFont::GenerateCharacter(HSCharacter character, HSVect2D * pos, list<HSCharToClone> * charList)
{
	HSCharToClone result;

	result.character = character.character;
	result.pos.x = pos->x;
	result.pos.y = pos->y;

	pos->x += character.charWidth + charSeparation;
	
	charList->push_back(result);
}