#include "HUD.h"

///////
//HUD//
///////

HUD::HUD()
{
	characterIcon = NULL;
	characterIconPos.x = 0;
	characterIconPos.y = 0;
	background = NULL;
	healthMeter = NULL;
	healthUnderMeter = NULL;
	healthMeterCover = NULL;
	underMeterAdjustRate = 0;
	flipMeterHorizontally = false;
	healthMeterPosX = 0;
	livesCounter = NULL;
	comboCounter = NULL;
	comboCounterBackground = NULL;
	objectToTrack = NULL;
}

HUD::~HUD()
{
	delete livesCounter;
	delete comboCounter;
}

void HUD::Refresh()
{
	float health = 0;
	float curHealth = 0;
	int lives = 0;
	int hits = 0;
	if(objectToTrack != NULL)
	{
		health = objectToTrack->health;
		curHealth = objectToTrack->curHealth;
		lives = objectToTrack->lives;

		if(objectToTrack->IsFighter())
		{
			Fighter * ott = (Fighter*)objectToTrack;

			list<ComboTrack>::iterator ctItr;
			for(ctItr = ott->comboTrack.begin(); ctItr != ott->comboTrack.end(); ctItr++)
			{
				hits += ctItr->hits;
			}
		}
	}

	characterIcon->visible = _visible;
	background->visible = _visible;
	healthMeter->visible = _visible;
	healthUnderMeter->visible = _visible;
	healthMeterCover->visible = _visible;

	float healthMeterScale = 0;
	if(health != 0)
	{
		healthMeterScale = curHealth / health;

		if(healthMeterScale < 0) { healthMeterScale = 0; }
		else if(healthMeterScale > 1) { healthMeterScale = 1; }
	}

	HSObjectHold * curHold = healthMeter->firstHold;
	while(curHold != NULL)
	{
		list<TextureInstance>::iterator tiItr;
		for(tiItr = curHold->textures.begin(); tiItr != curHold->textures.end(); tiItr++)
		{
			tiItr->hScale = healthMeterScale;
		}

		curHold = curHold->nextHold;
	}

	float healthUnderMeterScale = 0;
	curHold = healthUnderMeter->firstHold;
	while(curHold != NULL)
	{
		list<TextureInstance>::iterator tiItr;
		for(tiItr = curHold->textures.begin(); tiItr != curHold->textures.end(); tiItr++)
		{
			if(tiItr->hScale > healthMeterScale) { tiItr->hScale -= underMeterAdjustRate; }
			if(tiItr->hScale < healthMeterScale) { tiItr->hScale = healthMeterScale; }

			if(tiItr->hScale > healthUnderMeterScale) { healthUnderMeterScale = tiItr->hScale; }
		}

		curHold = curHold->nextHold;
	}

	if(flipMeterHorizontally)
	{
		float healthMeterXOffset = healthMeterWidth - (healthMeterWidth * healthMeterScale);
		healthMeter->pos.x = healthMeterPosX + healthMeterXOffset;

		float healthUnderMeterXOffset = healthMeterWidth - (healthMeterWidth * healthUnderMeterScale);
		healthUnderMeter->pos.x = healthMeterPosX + healthUnderMeterXOffset;
	}

	if(_visible)
	{
		std::stringstream sstm;
		sstm << lives;

		livesCounter->SetText(sstm.str());

		if(hits > 1)
		{
			sstm.str("");
			sstm << hits << " Hits";
			comboCounter->SetText(sstm.str());
			comboCounterBackground->visible = true;
		}
		else
		{
			comboCounter->DeleteText();
			comboCounterBackground->visible = false;
		}
	}
	else
	{
		livesCounter->DeleteText();
		comboCounter->DeleteText();
		comboCounterBackground->visible = false;
	}
}

void HUD::SetVisible(bool visible)
{
	_visible = visible;
	Refresh();
}