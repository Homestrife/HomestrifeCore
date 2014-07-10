#ifndef __CHARACTERSELECT_H_
#define __CHARACTERSELECT_H_

#include "menu.h"

enum CharacterSelectState
{
	CHARACTERS_SELECT,
	STAGE_SELECT,
	MUSIC_SELECT
};

enum CharacterSelectPlayerState
{
	NOT_PARTICIPATING,
	SELECTING_CHARACTER,
	SELECTING_PALETTE,
	READY
};

struct CharacterSelectChoices
{
	bool participating[4];
	string characterDefFilePaths[4];
	int characterPalettes[4];
	string characterIconDefFilePaths[4];
	string stageDefFilePath;
	string musicFilePath;
};

class StageChooser;

class StageChooserItem : public ImageChooserItem
{
public:
	string stageName;
	string stageDefFilePath;

	StageChooserItem();

	virtual bool IsStageChooserItem();

protected:

};

class StageChooser : public ImageChooser
{
public:
	HSObject * background;
	HSObject * previewBorder;
	string stageSelectTitle;
	HSText * title;
	HSText * stageName;

	StageChooser();
	~StageChooser();

	void PreviousStage();
	void NextStage();

	string GetCurrentDefFilePath();
	string GetCurrentStageName();

	virtual void RefreshChooser();

protected:
};

class MusicChooser;

class MusicChooserItem : public TextChooserItem
{
public:
	string musicFilePath;

	MusicChooserItem();
	
	virtual bool IsMusicChooserItem();

protected:
};

class MusicChooser : public TextChooser
{
public:
	HSObject * background;
	string musicSelectTitle;
	HSText * title;

	MusicChooser(HSFont * font);
	~MusicChooser();

	void PreviousMusic();
	void NextMusic();

	string GetCurrentMusicFilePath();

	virtual void RefreshChooser();

protected:
};

class CharacterSelectPortrait
{
public:
	HSObject * portraitImage;

	string characterName;
	string characterDefFilePath;
	string characterIconDefFilePath;

	CharacterSelectPortrait();

protected:
};

class CharacterSelectPanel : public HSOrderable
{
public:
	HSObject * panelBorderImage;
	HSObject * panelCharacterImage;

	CharacterSelectPortrait * portraitReference[MAX_PLAYERS];

	CharacterSelectPanel();

protected:
};

class CharacterSelectCursor
{
public:
	HSObject * cursorImage;
	CharacterSelectPanel * currentPanel;
	int position;

	CharacterSelectCursor();

protected:
};

class CharacterSelect
{
public:
	CharacterSelectPlayerState characterSelectPlayerState[MAX_PLAYERS];

	HSObject * characterSelectBackground;
	string characterSelectTitle;
	HSText * title;
	
	HSVect2D gridPos;
	int rows;
	int columns;
	float panelWidth;
	float panelHeight;
	HSVect2D panelBorderOffset;
	HSVect2D panelImageOffset;
	float panelBorderDepth;
	float panelImageDepth;
	list<CharacterSelectPanel*> panels;
	CharacterSelectCursor * cursors[MAX_PLAYERS];
	
	float portraitDepth;
	list<CharacterSelectPortrait*> portraits[MAX_PLAYERS];
	HSObject * portraitBorders[MAX_PLAYERS];
	HSVect2D portraitPos[MAX_PLAYERS];
	HSText * characterName[MAX_PLAYERS];
	HSText * portraitInstructions[MAX_PLAYERS];

	CharacterSelect();
	~CharacterSelect();
	
	void PlayerUp(int player);
	void PlayerDown(int player);
	void PlayerLeft(int player);
	void PlayerRight(int player);
	bool PlayerConfirm(int player); //returns true if control needs to move past the character selection step
	bool PlayerCancel(int player); //returns true if control needs to go back before the character selection step

	CharacterSelectChoices GetChoices();

	void ChangeCharacterSelectPlayerState(CharacterSelectPlayerState state, int player);

	void CursorUp(int player);
	void CursorDown(int player);
	void CursorLeft(int player);
	void CursorRight(int player);

	void PreviousPalette(int player);
	void NextPalette(int player);

	void Refresh();
	void SetVisible(bool visible);

protected:
	bool _visible;
	bool _prevPalette[MAX_PLAYERS];
	bool _nextPalette[MAX_PLAYERS];
};

class CharacterSelectManager
{
public:
	CharacterSelectState characterSelectState;

	CharacterSelect * characterSelect;

	StageChooser * stageChooser;

	MusicChooser * musicChooser;

	CharacterSelectManager();
	~CharacterSelectManager();
	
	void PlayerUp(int player);
	void PlayerDown(int player);
	void PlayerLeft(int player);
	void PlayerRight(int player);
	bool PlayerConfirm(int player); //returns true if control needs to move past the select menus
	bool PlayerCancel(int player); //returns true if control needs to go back before the select menus

	CharacterSelectChoices GetChoices();

	void ChangeCharacterSelectState(CharacterSelectState state);

	void Refresh();

protected:
};

#endif