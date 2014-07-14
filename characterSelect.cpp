#include "characterSelect.h"

////////////////////
//StageChooserItem//
////////////////////

StageChooserItem::StageChooserItem() : ImageChooserItem()
{
	stageName = "";
	stageDefFilePath = "";
}

bool StageChooserItem::IsStageChooserItem()
{
	return true;
}

////////////////
//StageChooser//
////////////////

StageChooser::StageChooser() : ImageChooser()
{
	background = NULL;
	previewBorder = NULL;
	stageSelectTitle = "";
	title = NULL;
	stageName = NULL;
}

StageChooser::~StageChooser()
{
	if(title != NULL) { delete title; }
	if(stageName != NULL) { delete stageName; }
}

void StageChooser::PreviousStage()
{
	ChoicePrev();
}

void StageChooser::NextStage()
{
	ChoiceNext();
}

string StageChooser::GetCurrentDefFilePath()
{
	int cur = 0;

	list<ImageChooserItem*>::iterator ciIt;
	for(ciIt = items.begin(); ciIt != items.end(); ciIt++)
	{
		if(cur == _choice && (*ciIt)->IsStageChooserItem())
		{
			return ((StageChooserItem*)(*ciIt))->stageDefFilePath;
		}

		cur++;
	}

	return "";
}

string StageChooser::GetCurrentStageName()
{
	int cur = 0;

	list<ImageChooserItem*>::iterator ciIt;
	for(ciIt = items.begin(); ciIt != items.end(); ciIt++)
	{
		if(cur == _choice && (*ciIt)->IsStageChooserItem())
		{
			return ((StageChooserItem*)(*ciIt))->stageName;
		}

		cur++;
	}

	return "";
}

void StageChooser::RefreshChooser()
{
	ImageChooser::RefreshChooser();

	background->visible = _visible;
	previewBorder->visible = _visible;

	if(_visible)
	{
		title->SetText(stageSelectTitle);

		int cur = 0;

		list<ImageChooserItem*>::iterator ciIt;
		for(ciIt = items.begin(); ciIt != items.end(); ciIt++)
		{
			if(cur == _choice && (*ciIt)->IsStageChooserItem())
			{
				stageName->SetText(((StageChooserItem*)(*ciIt))->stageName);

				break;
			}

			cur++;
		}
	}
	else
	{
		title->DeleteText();
		stageName->DeleteText();
	}
}

////////////////////
//MusicChooserItem//
////////////////////

MusicChooserItem::MusicChooserItem() : TextChooserItem()
{
	musicFilePath = "";
}

bool MusicChooserItem::IsMusicChooserItem()
{
	return true;
}

////////////////
//MusicChooser//
////////////////

MusicChooser::MusicChooser(HSFont * font) : TextChooser(font)
{
	background = NULL;
	musicSelectTitle = "";
	title = NULL;
}

MusicChooser::~MusicChooser()
{
	if(title != NULL) { delete title; }
}

void MusicChooser::PreviousMusic()
{
	ChoicePrev();
}

void MusicChooser::NextMusic()
{
	ChoiceNext();
}

string MusicChooser::GetCurrentMusicFilePath()
{
	int cur = 0;

	list<TextChooserItem*>::iterator ciIt;
	for(ciIt = items.begin(); ciIt != items.end(); ciIt++)
	{
		if(cur == _choice && (*ciIt)->IsMusicChooserItem())
		{
			return ((MusicChooserItem*)(*ciIt))->musicFilePath;
		}

		cur++;
	}

	return "";
}

void MusicChooser::RefreshChooser()
{
	TextChooser::RefreshChooser();

	background->visible = _visible;

	if(_visible)
	{
		title->SetText(musicSelectTitle);
	}
	else
	{
		title->DeleteText();
	}
}

///////////////////////////
//CharacterSelectPortrait//
///////////////////////////

CharacterSelectPortrait::CharacterSelectPortrait()
{
	portraitImage = NULL;
	characterName = "";
	characterDefFilePath = "";
}

////////////////////////
//CharacterSelectPanel//
////////////////////////

CharacterSelectPanel::CharacterSelectPanel()
{
	panelBorderImage = NULL;
	panelCharacterImage = NULL;

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		portraitReference[i] = NULL;
	}
}

/////////////////////////
//CharacterSelectCursor//
/////////////////////////

CharacterSelectCursor::CharacterSelectCursor()
{
	cursorImage = NULL;
	currentPanel = NULL;
	position = 0;
}

///////////////////
//CharacterSelect//
///////////////////

CharacterSelect::CharacterSelect()
{
	characterSelectBackground = NULL;
	characterSelectTitle = "";
	title = NULL;
	selectCharacterText = "";
	selectPaletteText = "";
	readyText = "";
	gridPos.x = 0;
	gridPos.y = 0;
	rows = 0;
	columns = 0;
	panelWidth = 0;
	panelHeight = 0;
	panelBorderOffset.x = 0;
	panelBorderOffset.y = 0;
	panelImageOffset.x = 0;
	panelImageOffset.y = 0;
	panelBorderDepth = 0;
	panelImageDepth = 0;
	panels.clear();
	portraitDepth = 0;
	_visible = false;

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		cursors[i] = NULL;
		portraits[i].clear();
		portraitBorders[i] = NULL;
		portraitPos[i].x = 0;
		portraitPos[i].y = 0;
		characterName[i] = NULL;
		portraitInstructions[i] = NULL;
		_prevPalette[i] = false;
		_nextPalette[i] = false;
		
		if(i == 0 || i == 1)
		{
			characterSelectPlayerState[i] = SELECTING_CHARACTER;
		}
		else
		{
			characterSelectPlayerState[i] = NOT_PARTICIPATING;
		}
	}
}

CharacterSelect::~CharacterSelect()
{
	if(title != NULL) { delete title; }

	list<CharacterSelectPanel*>::iterator cspItr;
	for(cspItr = panels.begin(); cspItr != panels.end(); cspItr++)
	{
		if(*cspItr != NULL) { delete *cspItr; }
	}

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(cursors[i] != NULL) { delete cursors[i]; }
		if(characterName[i] != NULL) { delete characterName[i]; }
		if(portraitInstructions[i] != NULL) { delete portraitInstructions[i]; }

		list<CharacterSelectPortrait*>::iterator csptItr;
		for(csptItr = portraits[i].begin(); csptItr != portraits[i].end(); csptItr++)
		{
			if(*csptItr != NULL) { delete *csptItr; }
		}
	}
}

void CharacterSelect::PlayerUp(int player)
{
	if(player < 0) { player = 0; }
	else if(player >= MAX_PLAYERS) { player = MAX_PLAYERS - 1; }

	switch(characterSelectPlayerState[player])
	{
	case NOT_PARTICIPATING:
		break;
	case SELECTING_CHARACTER:
		CursorUp(player);
		break;
	case SELECTING_PALETTE:
		break;
	case READY:
		break;
	}
}

void CharacterSelect::PlayerDown(int player)
{
	if(player < 0) { player = 0; }
	else if(player >= MAX_PLAYERS) { player = MAX_PLAYERS - 1; }

	switch(characterSelectPlayerState[player])
	{
	case NOT_PARTICIPATING:
		break;
	case SELECTING_CHARACTER:
		CursorDown(player);
		break;
	case SELECTING_PALETTE:
		break;
	case READY:
		break;
	}
}

void CharacterSelect::PlayerLeft(int player)
{
	if(player < 0) { player = 0; }
	else if(player >= MAX_PLAYERS) { player = MAX_PLAYERS - 1; }

	switch(characterSelectPlayerState[player])
	{
	case NOT_PARTICIPATING:
		break;
	case SELECTING_CHARACTER:
		CursorLeft(player);
		break;
	case SELECTING_PALETTE:
		PreviousPalette(player);
		break;
	case READY:
		break;
	}
}

void CharacterSelect::PlayerRight(int player)
{
	if(player < 0) { player = 0; }
	else if(player >= MAX_PLAYERS) { player = MAX_PLAYERS - 1; }

	switch(characterSelectPlayerState[player])
	{
	case NOT_PARTICIPATING:
		break;
	case SELECTING_CHARACTER:
		CursorRight(player);
		break;
	case SELECTING_PALETTE:
		NextPalette(player);
		break;
	case READY:
		break;
	}
}

bool CharacterSelect::PlayerConfirm(int player)
{
	if(player < 0) { player = 0; }
	else if(player >= MAX_PLAYERS) { player = MAX_PLAYERS - 1; }

	switch(characterSelectPlayerState[player])
	{
	case NOT_PARTICIPATING:
		ChangeCharacterSelectPlayerState(SELECTING_CHARACTER, player);
		break;
	case SELECTING_CHARACTER:
		if(cursors[player]->currentPanel != NULL && !cursors[player]->currentPanel->portraitReference[player]->characterDefFilePath.empty())
		{
			ChangeCharacterSelectPlayerState(SELECTING_PALETTE, player);
		}
		break;
	case SELECTING_PALETTE:
		ChangeCharacterSelectPlayerState(READY, player);
		break;
	case READY:
		break;
	}

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(characterSelectPlayerState[i] == SELECTING_CHARACTER || characterSelectPlayerState[i] == SELECTING_PALETTE)
		{
			return false;
		}
	}

	return true;
}

bool CharacterSelect::PlayerCancel(int player)
{
	if(player < 0) { player = 0; }
	else if(player >= MAX_PLAYERS) { player = MAX_PLAYERS - 1; }

	switch(characterSelectPlayerState[player])
	{
	case NOT_PARTICIPATING:
		break;
	case SELECTING_CHARACTER:
		if(player == 0 || player == 1)
		{
			return true;
		}
		ChangeCharacterSelectPlayerState(NOT_PARTICIPATING, player);
		break;
	case SELECTING_PALETTE:
		ChangeCharacterSelectPlayerState(SELECTING_CHARACTER, player);
		break;
	case READY:
		ChangeCharacterSelectPlayerState(SELECTING_PALETTE, player);
		break;
	}

	return false;
}

CharacterSelectChoices CharacterSelect::GetChoices()
{
	CharacterSelectChoices choices;

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(characterSelectPlayerState[i] == NOT_PARTICIPATING || cursors[i]->currentPanel == NULL || cursors[i]->currentPanel->portraitReference[i]->characterDefFilePath.empty())
		{
			choices.participating[i] = false;
			choices.characterDefFilePaths[i] = "";
			choices.characterIconDefFilePaths[i] = "";
			choices.characterPalettes[i] = 0;
		}
		else
		{
			choices.participating[i] = true;
			choices.characterDefFilePaths[i] = cursors[i]->currentPanel->portraitReference[i]->characterDefFilePath;
			choices.characterIconDefFilePaths[i] = cursors[i]->currentPanel->portraitReference[i]->characterIconDefFilePath;
			choices.characterPalettes[i] = cursors[i]->currentPanel->portraitReference[i]->portraitImage->GetPalette();
		}
	}

	return choices;
}

void CharacterSelect::ChangeCharacterSelectPlayerState(CharacterSelectPlayerState state, int player)
{
	if(player < 0) { player = 0; }
	else if(player >= MAX_PLAYERS) { player = MAX_PLAYERS - 1; }

	characterSelectPlayerState[player] = state;

	if(state == READY)
	{
		//make sure two of the same character don't have the same palette
		string curDefFilePath = cursors[player]->currentPanel->portraitReference[player]->characterDefFilePath;
		HSObject * curImage = cursors[player]->currentPanel->portraitReference[player]->portraitImage;
		bool adjusting = true;
		int startPalette = curImage->GetPalette();

		while(adjusting)
		{
			adjusting = false;
			for(int i = 0; i < MAX_PLAYERS; i++)
			{
				if(i != player
					&& characterSelectPlayerState[i] == READY
					&& cursors[i]->currentPanel->portraitReference[i]->characterDefFilePath.compare(curDefFilePath) == 0
					&& cursors[i]->currentPanel->portraitReference[i]->portraitImage->GetPalette() == curImage->GetPalette())
				{
					curImage->NextPalette();
					if(curImage->GetPalette() != startPalette)
					{
						//only keep going if we haven't looped back to the palette we started at
						adjusting = true;
					}
					break;
				}
			}
		}
	}
	else if(state == SELECTING_CHARACTER)
	{
		//return all player portraits back to the default palette
		list<CharacterSelectPortrait*>::iterator pItr;
		for(pItr = portraits[player].begin(); pItr != portraits[player].end(); pItr++)
		{
			if((*pItr)->portraitImage != NULL)
			{
				(*pItr)->portraitImage->SetPalette(0);
			}
		}
	}

	Refresh();
}

void CharacterSelect::CursorUp(int player)
{
	if(player < 0) { player = 0; }
	else if(player >= MAX_PLAYERS) { player = MAX_PLAYERS - 1; }

	int columnPos = cursors[player]->position / columns;

	if(columnPos == 0)
	{
		cursors[player]->position += columns * (rows - 1);
	}
	else
	{
		cursors[player]->position -= columns;
	}

	Refresh();
}

void CharacterSelect::CursorDown(int player)
{
	if(player < 0) { player = 0; }
	else if(player >= MAX_PLAYERS) { player = MAX_PLAYERS - 1; }

	int columnPos = cursors[player]->position / columns;

	if(columnPos == (rows - 1))
	{
		cursors[player]->position -= columns * (rows - 1);
	}
	else
	{
		cursors[player]->position += columns;
	}

	Refresh();
}

void CharacterSelect::CursorLeft(int player)
{
	if(player < 0) { player = 0; }
	else if(player >= MAX_PLAYERS) { player = MAX_PLAYERS - 1; }

	int rowPos = cursors[player]->position % columns;

	if(rowPos == 0)
	{
		cursors[player]->position += (columns - 1);
	}
	else
	{
		cursors[player]->position--;
	}

	Refresh();
}

void CharacterSelect::CursorRight(int player)
{
	if(player < 0) { player = 0; }
	else if(player >= MAX_PLAYERS) { player = MAX_PLAYERS - 1; }

	int rowPos = cursors[player]->position % columns;

	if(rowPos == (columns - 1))
	{
		cursors[player]->position -= (columns - 1);
	}
	else
	{
		cursors[player]->position++;
	}

	Refresh();
}

void CharacterSelect::PreviousPalette(int player)
{
	_prevPalette[player] = true;

	Refresh();
}

void CharacterSelect::NextPalette(int player)
{
	_nextPalette[player] = true;
	
	Refresh();
}

void CharacterSelect::Refresh()
{
	characterSelectBackground->visible = _visible;

	list<CharacterSelectPanel*>::iterator panIt;
	for(panIt = panels.begin(); panIt != panels.end(); panIt++)
	{
		(*panIt)->panelBorderImage->visible = _visible;
		if((*panIt)->panelCharacterImage != NULL)
		{
			(*panIt)->panelCharacterImage->visible = _visible;
		}
	}

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		bool shouldShowPlayer = _visible && characterSelectPlayerState[i] != NOT_PARTICIPATING;

		cursors[i]->cursorImage->visible = shouldShowPlayer;
		portraitBorders[i]->visible = shouldShowPlayer;

		cursors[i]->currentPanel = NULL;

		//get the panel for the current cursor position
		int index = 0;
		for(panIt = panels.begin(); panIt != panels.end(); panIt++)
		{
			if(index == cursors[i]->position)
			{
				cursors[i]->currentPanel = (*panIt);
				break;
			}

			index++;
		}

		cursors[i]->cursorImage->pos.x = gridPos.x + panelWidth * (cursors[i]->position % columns);
		cursors[i]->cursorImage->pos.y = gridPos.y + panelHeight * (cursors[i]->position / columns);

		list<CharacterSelectPortrait*>::iterator csIt;
		for(csIt = portraits[i].begin(); csIt != portraits[i].end(); csIt++)
		{
			if((*csIt)->portraitImage != NULL)
			{
				(*csIt)->portraitImage->visible = false;
			}
		}

		characterName[i]->DeleteText();
		portraitInstructions[i]->DeleteText();

		if(shouldShowPlayer)
		{
			if(cursors[i]->currentPanel != NULL)
			{
				characterName[i]->SetText(cursors[i]->currentPanel->portraitReference[i]->characterName);

				if(cursors[i]->currentPanel->portraitReference[i]->portraitImage != NULL)
				{
					cursors[i]->currentPanel->portraitReference[i]->portraitImage->visible = true;
					if(_prevPalette[i])
					{
						cursors[i]->currentPanel->portraitReference[i]->portraitImage->PrevPalette();
					}
					else if(_nextPalette[i])
					{
						cursors[i]->currentPanel->portraitReference[i]->portraitImage->NextPalette();
					}
				}
			}

			_prevPalette[i] = false;
			_nextPalette[i] = false;

			if(characterSelectPlayerState[i] == SELECTING_CHARACTER)
			{
				portraitInstructions[i]->SetText(selectCharacterText);
			}
			else if(characterSelectPlayerState[i] == SELECTING_PALETTE)
			{
				portraitInstructions[i]->SetText(selectPaletteText);
			}
			else if(characterSelectPlayerState[i] == READY)
			{
				portraitInstructions[i]->SetText(readyText);
			}
		}
	}

	if(_visible)
	{
		title->SetText(characterSelectTitle);
	}
	else
	{
		title->DeleteText();
	}
}

void CharacterSelect::SetVisible(bool visible)
{
	_visible = visible;
	Refresh();
}

//////////////////////////
//CharacterSelectManager//
//////////////////////////

CharacterSelectManager::CharacterSelectManager()
{
	characterSelectState = CHARACTERS_SELECT;
	characterSelect = NULL;
	stageChooser = NULL;
	musicChooser = NULL;
}

CharacterSelectManager::~CharacterSelectManager()
{
	if(characterSelect != NULL) { delete characterSelect; }
	if(stageChooser != NULL) { delete stageChooser; }
	if(musicChooser != NULL) { delete musicChooser; }
}

void CharacterSelectManager::PlayerUp(int player)
{
	if(player < 0) { player = 0; }
	else if(player >= MAX_PLAYERS) { player = MAX_PLAYERS - 1; }

	switch(characterSelectState)
	{
	case CHARACTERS_SELECT:
		characterSelect->PlayerUp(player);
		break;
	case STAGE_SELECT:
		break;
	case MUSIC_SELECT:
		break;
	}
}

void CharacterSelectManager::PlayerDown(int player)
{
	if(player < 0) { player = 0; }
	else if(player >= MAX_PLAYERS) { player = MAX_PLAYERS - 1; }

	switch(characterSelectState)
	{
	case CHARACTERS_SELECT:
		characterSelect->PlayerDown(player);
		break;
	case STAGE_SELECT:
		break;
	case MUSIC_SELECT:
		break;
	}
}

void CharacterSelectManager::PlayerLeft(int player)
{
	if(player < 0) { player = 0; }
	else if(player >= MAX_PLAYERS) { player = MAX_PLAYERS - 1; }

	switch(characterSelectState)
	{
	case CHARACTERS_SELECT:
		characterSelect->PlayerLeft(player);
		break;
	case STAGE_SELECT:
		stageChooser->PreviousStage();
		break;
	case MUSIC_SELECT:
		musicChooser->PreviousMusic();
		break;
	}
}

void CharacterSelectManager::PlayerRight(int player)
{
	if(player < 0) { player = 0; }
	else if(player >= MAX_PLAYERS) { player = MAX_PLAYERS - 1; }

	switch(characterSelectState)
	{
	case CHARACTERS_SELECT:
		characterSelect->PlayerRight(player);
		break;
	case STAGE_SELECT:
		stageChooser->NextStage();
		break;
	case MUSIC_SELECT:
		musicChooser->NextMusic();
		break;
	}
}

bool CharacterSelectManager::PlayerConfirm(int player)
{
	if(player < 0) { player = 0; }
	else if(player >= MAX_PLAYERS) { player = MAX_PLAYERS - 1; }

	switch(characterSelectState)
	{
	case CHARACTERS_SELECT:
		if(characterSelect->PlayerConfirm(player))
		{
			ChangeCharacterSelectState(STAGE_SELECT);
		}
		break;
	case STAGE_SELECT:
		//ChangeCharacterSelectState(MUSIC_SELECT);
		return true; //ignore music select for now
		break;
	case MUSIC_SELECT:
		return true;
		break;
	}

	return false;
}

bool CharacterSelectManager::PlayerCancel(int player)
{
	if(player < 0) { player = 0; }
	else if(player >= MAX_PLAYERS) { player = MAX_PLAYERS - 1; }
	
	switch(characterSelectState)
	{
	case CHARACTERS_SELECT:
		return characterSelect->PlayerCancel(player);
		break;
	case STAGE_SELECT:
		ChangeCharacterSelectState(CHARACTERS_SELECT);
		break;
	case MUSIC_SELECT:
		ChangeCharacterSelectState(STAGE_SELECT);
		break;
	}

	return false;
}

CharacterSelectChoices CharacterSelectManager::GetChoices()
{
	CharacterSelectChoices choices = characterSelect->GetChoices();

	choices.stageDefFilePath = stageChooser->GetCurrentDefFilePath();
	choices.musicFilePath = musicChooser->GetCurrentMusicFilePath();

	return choices;
}

void CharacterSelectManager::ChangeCharacterSelectState(CharacterSelectState state)
{
	characterSelectState = state;

	Refresh();
}

void CharacterSelectManager::Refresh()
{
	if(characterSelectState == CHARACTERS_SELECT)
	{
		characterSelect->SetVisible(true);
		stageChooser->SetVisible(false);
		musicChooser->SetVisible(false);
	}
	else if(characterSelectState == STAGE_SELECT)
	{
		characterSelect->SetVisible(false);
		stageChooser->SetVisible(true);
		musicChooser->SetVisible(false);
	}
	else if(characterSelectState == MUSIC_SELECT)
	{
		characterSelect->SetVisible(false);
		stageChooser->SetVisible(false);
		musicChooser->SetVisible(true);
	}
}