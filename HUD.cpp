#include "HUD.h"

///////////
//HUDHold//
///////////

HUDHold::HUDHold() : HSObjectHold()
{
	
}

HUDHold::~HUDHold()
{
	
}

///////
//HUD//
///////

HUD::HUD() : HSObject()
{
	healthMeterFilePath = "";
	healthUnderMeterFilePath = "";
	livesCounterFilePath = "";
	hitsCounterFilePath = "";

	healthMeter = NULL;
	healthUnderMeter = NULL;
	healthMeterOffset.x = 0;
	healthMeterOffset.y = 0;

	_healthMeterValue = 1.0f;
	_healthUnderMeterValue = 1.0f;

	livesOnesCounter = NULL;
	livesTensCounter = NULL;
	livesCounterOffset.x = 0;
	livesCounterOffset.y = 0;
	livesCounterDigitWidth = 0;
	livesCounterDigitSeparation = 0;

	hitsOnesCounter = NULL;
	hitsTensCounter = NULL;
	hitsCounterOffset.x = 0;
	hitsCounterOffset.y = 0;
	hitsCounterDigitWidth = 0;
	hitsCounterDigitSeparation = 0;

	_livesCounterValue = 0;
	_hitsCounterValue = 0;
}

HUD::~HUD()
{

}

int HUD::SetHealthMeterValue(float value)
{
	if(value < 0.0f) { value = 0.0f; }
	if(value > 1.0f) { value = 1.0f; }

	_healthMeterValue = value;

	if(_healthMeterValue > _healthUnderMeterValue)
	{
		_healthUnderMeterValue = _healthMeterValue;
	}

	return 0;
}

int HUD::SetLivesCounterValue(int value)
{
	if(value < 0) { value = 0; }
	if(value > 99) { value = 99; }

	_livesCounterValue = value;

	return 0;
}

int HUD::SetHitsCounterValue(int value)
{
	if(value < 0) { value = 0; }
	if(value > 999) { value = 999; }

	_hitsCounterValue = value;

	return 0;
}

int HUD::AdvanceHolds()
{
	if(curHold == NULL)
	{
		curHold = firstHold;
	}

	return 0;
}

int HUD::Update()
{
	if(_healthUnderMeterValue > _healthMeterValue)
	{
		_healthUnderMeterValue -= UNDER_METER_SPEED;

		if(_healthUnderMeterValue < _healthMeterValue)
		{
			_healthUnderMeterValue = _healthMeterValue;
		}
	}

	if(healthMeter != NULL)
	{
		healthMeter->pos.x = pos.x + healthMeterOffset.x;
		healthMeter->pos.y = pos.y + healthMeterOffset.y;

		HSObjectHold * curHold = healthMeter->firstHold;
		while(curHold != NULL)
		{
			list<TextureInstance>::iterator texItr;
			for(texItr = curHold->textures.begin(); texItr != curHold->textures.end(); texItr++)
			{
				texItr->hScale = _healthMeterValue;
			}

			curHold = curHold->nextListHold;
		}
	}

	if(healthUnderMeter != NULL)
	{
		healthUnderMeter->pos.x = pos.x + healthMeterOffset.x;
		healthUnderMeter->pos.y = pos.y + healthMeterOffset.y;

		HSObjectHold * curHold = healthUnderMeter->firstHold;
		while(curHold != NULL)
		{
			list<TextureInstance>::iterator texItr;
			for(texItr = curHold->textures.begin(); texItr != curHold->textures.end(); texItr++)
			{
				texItr->hScale = _healthUnderMeterValue;
			}

			curHold = curHold->nextListHold;
		}
	}

	if(livesOnesCounter != NULL)
	{
		livesOnesCounter->pos.x = pos.x + livesCounterOffset.x + livesCounterDigitWidth + livesCounterDigitSeparation;
		livesOnesCounter->pos.y = pos.y + livesCounterOffset.y;

		int curValue = _livesCounterValue % 10;

		HSObjectHold* curHold = livesOnesCounter->firstHold;
		while(curValue > 0)
		{
			curHold = curHold->nextListHold;
			curValue--;
		}

		livesOnesCounter->ChangeHold(curHold);
	}

	if(livesTensCounter != NULL)
	{
		livesTensCounter->pos.x = pos.x + livesCounterOffset.x;
		livesTensCounter->pos.y = pos.y + livesCounterOffset.y;

		int curValue = _livesCounterValue / 10;

		HSObjectHold* curHold = livesTensCounter->firstHold;
		while(curValue > 0)
		{
			curHold = curHold->nextListHold;
			curValue--;
		}

		livesTensCounter->ChangeHold(curHold);
	}

	if(hitsOnesCounter != NULL)
	{
		hitsOnesCounter->pos.x = pos.x + hitsCounterOffset.x + hitsCounterDigitWidth*2 + hitsCounterDigitSeparation*2;
		hitsOnesCounter->pos.y = pos.y + hitsCounterOffset.y;

		int curValue = _hitsCounterValue % 10;

		HSObjectHold* curHold = hitsOnesCounter->firstHold;
		while(curValue > 0)
		{
			curHold = curHold->nextListHold;
			curValue--;
		}

		hitsOnesCounter->ChangeHold(curHold);
	}

	if(hitsTensCounter != NULL)
	{
		hitsTensCounter->pos.x = pos.x + hitsCounterOffset.x + hitsCounterDigitWidth + hitsCounterDigitSeparation;
		hitsTensCounter->pos.y = pos.y + hitsCounterOffset.y;

		int curValue = _hitsCounterValue / 10;

		HSObjectHold* curHold = hitsTensCounter->firstHold;
		while(curValue > 0)
		{
			curHold = curHold->nextListHold;
			curValue--;
		}

		hitsTensCounter->ChangeHold(curHold);
	}

	if(hitsHundredsCounter != NULL)
	{
		hitsHundredsCounter->pos.x = pos.x + hitsCounterOffset.x;
		hitsHundredsCounter->pos.y = pos.y + hitsCounterOffset.y;

		int curValue = _hitsCounterValue / 100;

		HSObjectHold* curHold = hitsHundredsCounter->firstHold;
		while(curValue > 0)
		{
			curHold = curHold->nextListHold;
			curValue--;
		}

		hitsHundredsCounter->ChangeHold(curHold);
	}

	return 0;
}

bool HUD::IsHUD()
{
	return true;
}