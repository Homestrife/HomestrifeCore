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
	string counterFilePath;

	HSObject * healthMeter;
	HSObject * healthUnderMeter;
	HSVect2D healthMeterOffset;

	HSObject * onesCounter;
	HSObject * tensCounter;
	HSVect2D counterOffset;
	HSVectComp counterDigitWidth;
	HSVectComp counterDigitSeparation;

	int SetHealthMeterValue(float value);

	int SetCounterValue(int value);

	virtual int AdvanceHolds();
	virtual int Update(); //handle updates

	virtual bool IsHUD();

protected:
	float _healthMeterValue;
	float _healthUnderMeterValue;

	int _counterValue;

};

#endif