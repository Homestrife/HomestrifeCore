#ifndef __HUDMANAGER_H_
#define __HUDMANAGER_H_

#include "HUD.h"

enum PromptState
{
	PROMPT_READY,
	PROMPT_FIGHT,
	PROMPT_WIN,
	PROMPT_CLEAR
};

class HUDManager
{
public:
	HUD * HUDs[MAX_PLAYERS];

	HSText * prompt;
	HSObject * promptBackground;

	string readyText;
	string fightText;
	string winText;

	HUDManager();
	~HUDManager();

	void ChangePromptState(PromptState state);

	void Refresh();
	void SetVisible(bool visible);

protected:
	bool _visible;
	PromptState _promptState;
};

#endif