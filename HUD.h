#ifndef __HUD_H_
#define __HUD_H_

#include "hsobject.h"

#define UNDER_METER_SPEED 0.003f

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
	HSVect2D hitsCounterOffset;
	HSVectComp hitsCounterDigitWidth;
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