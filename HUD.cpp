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
	counterFilePath = "";

	healthMeter = NULL;
	healthUnderMeter = NULL;
	healthMeterOffset.x = 0;
	healthMeterOffset.y = 0;

	_healthMeterValue = 1.0f;
	_healthUnderMeterValue = 1.0f;

	onesCounter = NULL;
	tensCounter = NULL;
	counterOffset.x = 0;
	counterOffset.y = 0;
	counterDigitWidth = 0;
	counterDigitSeparation = 0;

	_counterValue = 0;
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

int HUD::SetCounterValue(int value)
{
	if(value < 0) { value = 0; }
	if(value > 99) { value = 99; }

	_counterValue = value;

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

	if(onesCounter != NULL)
	{
		onesCounter->pos.x = pos.x + counterOffset.x + counterDigitWidth + counterDigitSeparation;
		onesCounter->pos.y = pos.y + counterOffset.y;

		int curValue = _counterValue % 10;

		HSObjectHold* curHold = onesCounter->firstHold;
		while(curValue > 0)
		{
			curHold = curHold->nextListHold;
			curValue--;
		}

		onesCounter->ChangeHold(curHold);
	}

	if(tensCounter != NULL)
	{
		tensCounter->pos.x = pos.x + counterOffset.x;
		tensCounter->pos.y = pos.y + counterOffset.y;

		int curValue = _counterValue / 10;

		HSObjectHold* curHold = tensCounter->firstHold;
		while(curValue > 0)
		{
			curHold = curHold->nextListHold;
			curValue--;
		}

		tensCounter->ChangeHold(curHold);
	}

	return 0;
}

bool HUD::IsHUD()
{
	return true;
}