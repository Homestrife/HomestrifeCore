#ifndef __HUD_H_
#define __HUD_H_

#include "hstext.h"
#include "fighter.h"

class HUD
{
public:
	HSObject * characterIcon;
	HSVect2D characterIconPos;

	HSObject * background;
	HSObject * healthMeter;
	HSObject * healthUnderMeter;
	HSObject * healthMeterCover;
	float underMeterAdjustRate;
	float healthMeterWidth;
	float healthMeterPosX;

	bool flipMeterHorizontally;

	HSText * livesCounter;
	HSText * comboCounter;
	HSObject * comboCounterBackground;

	TerrainObject * objectToTrack;

	HUD();
	~HUD();

	void Refresh();
	void SetVisible(bool visible);

protected:
	bool _visible;
};

#endif