#include "HUDManager.h"

//////////////
//HUDManager//
//////////////

HUDManager::HUDManager()
{
	prompt = NULL;
	promptBackground = NULL;
	readyText = "";
	fightText = "";
	winText = "";
	_visible = false;
	_promptState = PROMPT_READY;

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		HUDs[i] = NULL;
	}
}

HUDManager::~HUDManager()
{
	delete prompt;

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		delete HUDs[i];
	}
}

void HUDManager::ChangePromptState(PromptState state)
{
	_promptState = state;

	Refresh();
}

void HUDManager::Refresh()
{
	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(HUDs[i] != NULL)
		{
			HUDs[i]->SetVisible(_visible);
		}
	}

	if(_visible)
	{
		switch (_promptState)
		{
		case PROMPT_READY:
			prompt->SetText(readyText);
			promptBackground->visible = true;
			break;
		case PROMPT_FIGHT:
			prompt->SetText(fightText);
			promptBackground->visible = true;
			break;
		case PROMPT_WIN:
			for(int i = 0; i < MAX_PLAYERS; i++)
			{
				if(HUDs[i] == NULL || HUDs[i]->objectToTrack == NULL || HUDs[i]->objectToTrack->lives == 0)
				{
					continue;
				}
				
				std::stringstream sstm;
				sstm << "Player " << (i + 1) << " " << winText;
				prompt->SetText(sstm.str());

				break;
			}
			promptBackground->visible = true;
			break;
		case PROMPT_CLEAR:
			prompt->DeleteText();
			promptBackground->visible = false;
			break;
		}
	}
	else
	{
		prompt->DeleteText();
		promptBackground->visible = false;
	}
}

void HUDManager::SetVisible(bool visible)
{
	_visible = visible;
	Refresh();
}