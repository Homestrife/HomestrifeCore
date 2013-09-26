#ifndef __HUD_H_
#define __HUD_H_

#include "hsobject.h"

#define UNDER_METER_SPEED 0.003f

enum ComboCounterXPosition
{
	COUNTER_LEFT,
	COUNTER_RIGHT
};

enum ComboCounterYPosition
{
	COUNTER_ABOVE,
	COUNTER_BELOW
};

class HUDHold : public HSObjectHold
{
public:
	HUDHold();
	~HUDHold();

protected:


};

class HUD : public HSObject
{
public:
	HUD();
	~HUD();

	float hudWidth;
	float hudHeight;
	
	ComboCounterXPosition comboCounterXPosition;
	ComboCounterYPosition comboCounterYPosition;
	
	string healthMeterFilePath;
	string healthUnderMeterFilePath;
	string livesCounterFilePath;
	string hitsCounterFilePath;

	HSObject * healthMeter;
	HSObject * healthUnderMeter;
	HSVect2D healthMeterOffset;

	HSObject * livesOnesCounter;
	HSObject * livesTensCounter;
	HSVect2D livesCounterOffset;
	HSVectComp livesCounterDigitWidth;
	HSVectComp livesCounterDigitSeparation;

	HSObject * hitsOnesCounter;
	HSObject * hitsTensCounter;
	HSObject * hitsHundredsCounter;
	float hitsCounterXDistanceFromSide;
	float hitsCounterYDistanceFromHUD;
	HSVectComp hitsCounterDigitWidth;
	HSVectComp hitsCounterDigitHeight;
	HSVectComp hitsCounterDigitSeparation;

	int SetHealthMeterValue(float value);

	int SetLivesCounterValue(int value);
	int SetHitsCounterValue(int value);

	virtual int AdvanceHolds();
	virtual int Update(); //handle updates

	virtual bool IsHUD();

protected:
	float _healthMeterValue;
	float _healthUnderMeterValue;

	int _livesCounterValue;
	int _hitsCounterValue;
};

#endif