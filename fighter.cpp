#include "fighter.h"

///////////////
//FighterHold//
///////////////

FighterHold::FighterHold() : PhysicsObjectHold()
{
}

FighterHold::~FighterHold()
{
}

int FighterHold::Define(XMLElement * definition, string defFileDirectory, list<HSTexture*> * textureRegistry, list<HSAudio*> * audioRegistry, SDL_AudioSpec * obtainedAudioSpec, bool openGL3, bool useTGAPalettes)
{
	if(int error = PhysicsObjectHold::Define(definition, defFileDirectory, textureRegistry, audioRegistry, obtainedAudioSpec, openGL3, useTGAPalettes) != 0)
	{
		return error;
	}

	//get changeAttackBoxAttributes
	const char * cc = definition->Attribute("changeCancels");
    string bString;
	if(cc != NULL)
	{
		bString.assign(cc);
		if(bString.compare("true") == 0) { changeCancels = true; }
		else if(bString.compare("false") == 0) { changeCancels = false; }
	}

	//get cancels
	const char * dc = definition->Attribute("dashCancel");
	if(dc != NULL)
	{
		bString.assign(dc);
		if(bString.compare("ANY_TIME") == 0) { cancels.dash = ANY_TIME; }
		else if(bString.compare("AFTER_HIT_OR_BLOCK") == 0) { cancels.dash = AFTER_HIT_OR_BLOCK; }
		else if(bString.compare("AFTER_HIT") == 0) { cancels.dash = AFTER_HIT; }
		else if(bString.compare("AFTER_BLOCK") == 0) { cancels.dash = AFTER_BLOCK; }
		else if(bString.compare("NEVER") == 0) { cancels.dash = NEVER; }
	}
	const char * jc = definition->Attribute("jumpCancel");
	if(jc != NULL)
	{
		bString.assign(jc);
		if(bString.compare("ANY_TIME") == 0) { cancels.jump = ANY_TIME; }
		else if(bString.compare("AFTER_HIT_OR_BLOCK") == 0) { cancels.jump = AFTER_HIT_OR_BLOCK; }
		else if(bString.compare("AFTER_HIT") == 0) { cancels.jump = AFTER_HIT; }
		else if(bString.compare("AFTER_BLOCK") == 0) { cancels.jump = AFTER_BLOCK; }
		else if(bString.compare("NEVER") == 0) { cancels.jump = NEVER; }
	}
	const char * lnc = definition->Attribute("lightNeutralCancel");
	if(lnc != NULL)
	{
		bString.assign(lnc);
		if(bString.compare("ANY_TIME") == 0) { cancels.lightNeutral = ANY_TIME; }
		else if(bString.compare("AFTER_HIT_OR_BLOCK") == 0) { cancels.lightNeutral = AFTER_HIT_OR_BLOCK; }
		else if(bString.compare("AFTER_HIT") == 0) { cancels.lightNeutral = AFTER_HIT; }
		else if(bString.compare("AFTER_BLOCK") == 0) { cancels.lightNeutral = AFTER_BLOCK; }
		else if(bString.compare("NEVER") == 0) { cancels.lightNeutral = NEVER; }
	}
	const char * lfc = definition->Attribute("lightForwardCancel");
	if(lfc != NULL)
	{
		bString.assign(lfc);
		if(bString.compare("ANY_TIME") == 0) { cancels.lightForward = ANY_TIME; }
		else if(bString.compare("AFTER_HIT_OR_BLOCK") == 0) { cancels.lightForward = AFTER_HIT_OR_BLOCK; }
		else if(bString.compare("AFTER_HIT") == 0) { cancels.lightForward = AFTER_HIT; }
		else if(bString.compare("AFTER_BLOCK") == 0) { cancels.lightForward = AFTER_BLOCK; }
		else if(bString.compare("NEVER") == 0) { cancels.lightForward = NEVER; }
	}
	const char * luc = definition->Attribute("lightUpCancel");
	if(luc != NULL)
	{
		bString.assign(luc);
		if(bString.compare("ANY_TIME") == 0) { cancels.lightUp = ANY_TIME; }
		else if(bString.compare("AFTER_HIT_OR_BLOCK") == 0) { cancels.lightUp = AFTER_HIT_OR_BLOCK; }
		else if(bString.compare("AFTER_HIT") == 0) { cancels.lightUp = AFTER_HIT; }
		else if(bString.compare("AFTER_BLOCK") == 0) { cancels.lightUp = AFTER_BLOCK; }
		else if(bString.compare("NEVER") == 0) { cancels.lightUp = NEVER; }
	}
	const char * ldc = definition->Attribute("lightDownCancel");
	if(ldc != NULL)
	{
		bString.assign(ldc);
		if(bString.compare("ANY_TIME") == 0) { cancels.lightDown = ANY_TIME; }
		else if(bString.compare("AFTER_HIT_OR_BLOCK") == 0) { cancels.lightDown = AFTER_HIT_OR_BLOCK; }
		else if(bString.compare("AFTER_HIT") == 0) { cancels.lightDown = AFTER_HIT; }
		else if(bString.compare("AFTER_BLOCK") == 0) { cancels.lightDown = AFTER_BLOCK; }
		else if(bString.compare("NEVER") == 0) { cancels.lightDown = NEVER; }
	}
	const char * lbc = definition->Attribute("lightBackwardCancel");
	if(lbc != NULL)
	{
		bString.assign(lbc);
		if(bString.compare("ANY_TIME") == 0) { cancels.lightBackward = ANY_TIME; }
		else if(bString.compare("AFTER_HIT_OR_BLOCK") == 0) { cancels.lightBackward = AFTER_HIT_OR_BLOCK; }
		else if(bString.compare("AFTER_HIT") == 0) { cancels.lightBackward = AFTER_HIT; }
		else if(bString.compare("AFTER_BLOCK") == 0) { cancels.lightBackward = AFTER_BLOCK; }
		else if(bString.compare("NEVER") == 0) { cancels.lightBackward = NEVER; }
	}
	const char * lqc = definition->Attribute("lightQCFCancel");
	if(lqc != NULL)
	{
		bString.assign(lqc);
		if(bString.compare("ANY_TIME") == 0) { cancels.lightQCF = ANY_TIME; }
		else if(bString.compare("AFTER_HIT_OR_BLOCK") == 0) { cancels.lightQCF = AFTER_HIT_OR_BLOCK; }
		else if(bString.compare("AFTER_HIT") == 0) { cancels.lightQCF = AFTER_HIT; }
		else if(bString.compare("AFTER_BLOCK") == 0) { cancels.lightQCF = AFTER_BLOCK; }
		else if(bString.compare("NEVER") == 0) { cancels.lightQCF = NEVER; }
	}
	const char * hnc = definition->Attribute("heavyNeutralCancel");
	if(hnc != NULL)
	{
		bString.assign(hnc);
		if(bString.compare("ANY_TIME") == 0) { cancels.heavyNeutral = ANY_TIME; }
		else if(bString.compare("AFTER_HIT_OR_BLOCK") == 0) { cancels.heavyNeutral = AFTER_HIT_OR_BLOCK; }
		else if(bString.compare("AFTER_HIT") == 0) { cancels.heavyNeutral = AFTER_HIT; }
		else if(bString.compare("AFTER_BLOCK") == 0) { cancels.heavyNeutral = AFTER_BLOCK; }
		else if(bString.compare("NEVER") == 0) { cancels.heavyNeutral = NEVER; }
	}
	const char * hfc = definition->Attribute("heavyForwardCancel");
	if(hfc != NULL)
	{
		bString.assign(hfc);
		if(bString.compare("ANY_TIME") == 0) { cancels.heavyForward = ANY_TIME; }
		else if(bString.compare("AFTER_HIT_OR_BLOCK") == 0) { cancels.heavyForward = AFTER_HIT_OR_BLOCK; }
		else if(bString.compare("AFTER_HIT") == 0) { cancels.heavyForward = AFTER_HIT; }
		else if(bString.compare("AFTER_BLOCK") == 0) { cancels.heavyForward = AFTER_BLOCK; }
		else if(bString.compare("NEVER") == 0) { cancels.heavyForward = NEVER; }
	}
	const char * huc = definition->Attribute("heavyUpCancel");
	if(huc != NULL)
	{
		bString.assign(huc);
		if(bString.compare("ANY_TIME") == 0) { cancels.heavyUp = ANY_TIME; }
		else if(bString.compare("AFTER_HIT_OR_BLOCK") == 0) { cancels.heavyUp = AFTER_HIT_OR_BLOCK; }
		else if(bString.compare("AFTER_HIT") == 0) { cancels.heavyUp = AFTER_HIT; }
		else if(bString.compare("AFTER_BLOCK") == 0) { cancels.heavyUp = AFTER_BLOCK; }
		else if(bString.compare("NEVER") == 0) { cancels.heavyUp = NEVER; }
	}
	const char * hdc = definition->Attribute("heavyDownCancel");
	if(hdc != NULL)
	{
		bString.assign(hdc);
		if(bString.compare("ANY_TIME") == 0) { cancels.heavyDown = ANY_TIME; }
		else if(bString.compare("AFTER_HIT_OR_BLOCK") == 0) { cancels.heavyDown = AFTER_HIT_OR_BLOCK; }
		else if(bString.compare("AFTER_HIT") == 0) { cancels.heavyDown = AFTER_HIT; }
		else if(bString.compare("AFTER_BLOCK") == 0) { cancels.heavyDown = AFTER_BLOCK; }
		else if(bString.compare("NEVER") == 0) { cancels.heavyDown = NEVER; }
	}
	const char * hbc = definition->Attribute("heavyBackwardCancel");
	if(hbc != NULL)
	{
		bString.assign(hbc);
		if(bString.compare("ANY_TIME") == 0) { cancels.heavyBackward = ANY_TIME; }
		else if(bString.compare("AFTER_HIT_OR_BLOCK") == 0) { cancels.heavyBackward = AFTER_HIT_OR_BLOCK; }
		else if(bString.compare("AFTER_HIT") == 0) { cancels.heavyBackward = AFTER_HIT; }
		else if(bString.compare("AFTER_BLOCK") == 0) { cancels.heavyBackward = AFTER_BLOCK; }
		else if(bString.compare("NEVER") == 0) { cancels.heavyBackward = NEVER; }
	}
	const char * hqc = definition->Attribute("heavyQCFCancel");
	if(hqc != NULL)
	{
		bString.assign(hqc);
		if(bString.compare("ANY_TIME") == 0) { cancels.heavyQCF = ANY_TIME; }
		else if(bString.compare("AFTER_HIT_OR_BLOCK") == 0) { cancels.heavyQCF = AFTER_HIT_OR_BLOCK; }
		else if(bString.compare("AFTER_HIT") == 0) { cancels.heavyQCF = AFTER_HIT; }
		else if(bString.compare("AFTER_BLOCK") == 0) { cancels.heavyQCF = AFTER_BLOCK; }
		else if(bString.compare("NEVER") == 0) { cancels.heavyQCF = NEVER; }
	}

	return 0;
}

bool FighterHold::IsFighterHold()
{
	return true;
}

FighterHold * FighterHold::CreateHoldOfSameType()
{
	return new FighterHold();
}

void FighterHold::CopyAttributes(HSObjectHold * target)
{
	PhysicsObjectHold::CopyAttributes(target);

	if(!target->IsFighterHold()) { return; }

	FighterHold * fo = (FighterHold*)target;
}

///////////
//Fighter//
///////////
Fighter::Fighter() : PhysicsObject()
{
	firstPreviousTerrainBox = NULL;
	lastPreviousTerrainBox = NULL;

	terrainBoxesChanged = false;

	firstUprightTerrainBox = NULL;
	lastUprightTerrainBox = NULL;
	firstCrouchingTerrainBox = NULL;
	lastCrouchingTerrainBox = NULL;
	firstProneTerrainBox = NULL;
	lastProneTerrainBox = NULL;
	firstCompactTerrainBox = NULL;
	lastCompactTerrainBox = NULL;

	falls = false;
	state = JUMPING;
	facing = RIGHT;
	blocking = false;
	hitstunBlockability = MID;
	jump = NO_JUMP;
	airDash = NO_AIR_DASH;
	currentSurfaceFriction = 0;
	jumpStartup = false;
	jumpStartupJustEnded = false;
	shortHop = false;
	turning = false;
	timeSinceWalkStop = 0;
	timeSinceRunStop = 0;
	runStopping = false;
	turning = false;
	sliding = false;
	attacking = false;
	walkAfterTurning = false;
	runAfterTurning = false;
	airVelocityCategory = LOW_VELOCITY;
	fastFall = false;
	airControl = NO_AIR_CONTROL;
	landingAction = NO_ACTION;
	bufferedAction = NO_ACTION;
	bufferedActionAge = 0;
	turnUponLanding = false;
	hitSomething = false;
	wasBlocked = false;
	fighterPushXAccel = 0;

	defaultCancels.jump = NEVER;
	defaultCancels.dash = NEVER;
	defaultCancels.lightNeutral = NEVER;
	defaultCancels.lightForward = NEVER;
	defaultCancels.lightUp = NEVER;
	defaultCancels.lightDown = NEVER;
	defaultCancels.lightBackward = NEVER;
	defaultCancels.lightQCF = NEVER;
	defaultCancels.heavyNeutral = NEVER;
	defaultCancels.heavyForward = NEVER;
	defaultCancels.heavyUp = NEVER;
	defaultCancels.heavyDown = NEVER;
    defaultCancels.heavyBackward = NEVER;
	defaultCancels.heavyQCF = NEVER;

	cancels = defaultCancels;

	curHitstop = 0;
	curHitstun = 0;
	curBlockstun = 0;

	framesSinceKnockout = 0;

	fighterEventHolds.standing = NULL;
	fighterEventHolds.turn = NULL;
	fighterEventHolds.walk = NULL;
	fighterEventHolds.walking = NULL;
	fighterEventHolds.walkingTurn = NULL;
	fighterEventHolds.run = NULL;
	fighterEventHolds.running = NULL;
	fighterEventHolds.runningTurn = NULL;
	fighterEventHolds.runningStop = NULL;
	fighterEventHolds.crouch = NULL;
	fighterEventHolds.crouching = NULL;
	fighterEventHolds.crouchingTurn = NULL;
	fighterEventHolds.stand = NULL;

	fighterEventHolds.jumpNeutralStart = NULL;
	fighterEventHolds.jumpNeutralStartAir = NULL;
	fighterEventHolds.jumpNeutralRising = NULL;
	fighterEventHolds.jumpNeutralFall = NULL;
	fighterEventHolds.jumpNeutralFalling = NULL;
	fighterEventHolds.jumpNeutralLand = NULL;
	fighterEventHolds.jumpBackwardRising = NULL;
	fighterEventHolds.jumpBackwardFall = NULL;

	fighterEventHolds.airDashForward = NULL;
	fighterEventHolds.airDashBackward = NULL;
        
	fighterEventHolds.blockHigh = NULL;
	fighterEventHolds.blockLow = NULL;
	fighterEventHolds.blockAir = NULL;

	fighterEventHolds.hitstunLightHighStanding = NULL;
	fighterEventHolds.hitstunLightMidStanding = NULL;
	fighterEventHolds.hitstunLightLowStanding = NULL;
	fighterEventHolds.hitstunLightMidCrouching = NULL;
	fighterEventHolds.hitstunLightAir = NULL;
    
	fighterEventHolds.attackLightNeutralGround = NULL;
	fighterEventHolds.attackLightDownGround = NULL;
	fighterEventHolds.attackLightUpGround = NULL;
	fighterEventHolds.attackLightForwardGround = NULL;
	fighterEventHolds.attackLightQCFGround = NULL;
	fighterEventHolds.attackLightNeutralAir = NULL;
	fighterEventHolds.attackLightDownAir = NULL;
	fighterEventHolds.attackLightUpAir = NULL;
	fighterEventHolds.attackLightForwardAir = NULL;
	fighterEventHolds.attackLightBackwardAir = NULL;
	fighterEventHolds.attackLightQCFAir = NULL;
    
	fighterEventHolds.attackHeavyNeutralGround = NULL;
	fighterEventHolds.attackHeavyDownGround = NULL;
	fighterEventHolds.attackHeavyUpGround = NULL;
	fighterEventHolds.attackHeavyForwardGround = NULL;
	fighterEventHolds.attackHeavyQCFGround = NULL;
	fighterEventHolds.attackHeavyNeutralAir = NULL;
	fighterEventHolds.attackHeavyDownAir = NULL;
	fighterEventHolds.attackHeavyUpAir = NULL;
	fighterEventHolds.attackHeavyForwardAir = NULL;
	fighterEventHolds.attackHeavyBackwardAir = NULL;
	fighterEventHolds.attackHeavyQCFAir = NULL;

	walkSpeed = 0;
	runSpeed = 0;
	jumpSpeed = 0;
	forwardAirDashSpeed = 0;
	forwardAirDashDuration = 0;
	backwardAirDashSpeed = 0;
	backwardAirDashDuration = 0;
	curAirDashDuration = 0;
	stepHeight = 0;
	airActions = 1;
	curAirActions = airActions;
	airControlAccel = 0;
	maxAirControlSpeed = 0;
}

Fighter::~Fighter()
{
	HSBox * boxToDelete = firstUprightTerrainBox;
	HSBox * nextBoxToDelete;

	while(boxToDelete != NULL)
	{
		nextBoxToDelete = boxToDelete->nextBox;

		delete boxToDelete;

		boxToDelete = nextBoxToDelete;
	}

	boxToDelete = firstCrouchingTerrainBox;

	while(boxToDelete != NULL)
	{
		nextBoxToDelete = boxToDelete->nextBox;

		delete boxToDelete;

		boxToDelete = nextBoxToDelete;
	}

	boxToDelete = firstProneTerrainBox;

	while(boxToDelete != NULL)
	{
		nextBoxToDelete = boxToDelete->nextBox;

		delete boxToDelete;

		boxToDelete = nextBoxToDelete;
	}

	boxToDelete = firstCompactTerrainBox;

	while(boxToDelete != NULL)
	{
		nextBoxToDelete = boxToDelete->nextBox;

		delete boxToDelete;

		boxToDelete = nextBoxToDelete;
	}
}

int Fighter::Define(XMLElement * definition, string defFileDirectory, list<HSTexture*> * textureRegistry, list<HSPalette*> * paletteRegistry, list<HSAudio*> * audioRegistry, SDL_AudioSpec * obtainedAudioSpec, bool openGL3)
{
	if(int error = PhysicsObject::Define(definition, defFileDirectory, textureRegistry, paletteRegistry, audioRegistry, obtainedAudioSpec, openGL3) != 0)
	{
		return error;
	}

	//get the upright terrain boxes
	XMLElement * uprightTerrainBox;;
	if((uprightTerrainBox = definition->FirstChildElement("UprightTerrainBox")) != NULL)
	{
		HSBox * newUprightTerrainBox = new HSBox();
		newUprightTerrainBox->nextBox = NULL;
					
		//add the terrain box to the object
		if(int error = DefineUprightTerrainBox(newUprightTerrainBox, uprightTerrainBox) != 0)
		{
			return error; //there was an error adding the terrain box
		}
	}

	//get the crouching terrain boxes
	XMLElement * crouchingTerrainBox;
	if((crouchingTerrainBox = definition->FirstChildElement("CrouchingTerrainBox")) != NULL)
	{
		HSBox * newCrouchingTerrainBox = new HSBox();
		newCrouchingTerrainBox->nextBox = NULL;
					
		//add the terrain box to the object
		if(int error = DefineCrouchingTerrainBox(newCrouchingTerrainBox, crouchingTerrainBox) != 0)
		{
			return error; //there was an error adding the terrain box
		}
	}

	//get the prone terrain boxes
	XMLElement * proneTerrainBox;
	if((proneTerrainBox = definition->FirstChildElement("ProneTerrainBox")) != NULL)
	{
		HSBox * newProneTerrainBox = new HSBox();
		newProneTerrainBox->nextBox = NULL;
					
		//add the terrain box to the object
		if(int error = DefineProneTerrainBox(newProneTerrainBox, proneTerrainBox) != 0)
		{
			return error; //there was an error adding the terrain box
		}
	}

	//get the compact terrain boxes
	XMLElement * compactTerrainBox;
	if((compactTerrainBox = definition->FirstChildElement("CompactTerrainBox")) != NULL)
	{
		HSBox * newCompactTerrainBox = new HSBox();
		newCompactTerrainBox->nextBox = NULL;
					
		//add the terrain box to the object
		if(int error = DefineCompactTerrainBox(newCompactTerrainBox, compactTerrainBox) != 0)
		{
			return error; //there was an error adding the terrain box
		}
	}

	ChangeTerrainBoxes(firstUprightTerrainBox);

	//get the walkSpeed
	definition->QueryFloatAttribute("walkSpeed", &walkSpeed);

	//get the runSpeed
	definition->QueryFloatAttribute("runSpeed", &runSpeed);

	//get the jumpSpeed
	definition->QueryFloatAttribute("jumpSpeed", &jumpSpeed);

	//get the stepHeight
	definition->QueryFloatAttribute("stepHeight", &stepHeight);

	//get air dash statistics
	definition->QueryFloatAttribute("forwardAirDashSpeed", &forwardAirDashSpeed);
	definition->QueryFloatAttribute("backwardAirDashSpeed", &backwardAirDashSpeed);
	definition->QueryIntAttribute("forwardAirDashDuration", &forwardAirDashDuration);
	definition->QueryIntAttribute("backwardAirDashDuration", &backwardAirDashDuration);

	//get air control statistics
	definition->QueryFloatAttribute("airControlAccel", &airControlAccel);
	definition->QueryFloatAttribute("maxAirControlSpeed", &maxAirControlSpeed);

	return 0;
}

void Fighter::ChangeTerrainBoxes(HSBox * newFirstTerrainBox)
{
	//don't bother if the new box is same as the current one
	if(newFirstTerrainBox == firstTerrainBox) { return; }

	//change the box, saving the current one
	firstPreviousTerrainBox = firstTerrainBox;
	firstTerrainBox = newFirstTerrainBox;

	terrainBoxesChanged = true;
}

int Fighter::AdvanceHolds()
{
	//update hitstop
	if(curHitstop > 0)
	{
		curHitstop--;
		if(curHitstop > 0)
		{
			return 0;
		}
	}

	objectsSpawned = false;
	audioPlayed = false;

	//update turn timers
	if(timeSinceWalkStop < MOVING_TURN_THRESHOLD) { timeSinceWalkStop++; }
	if(timeSinceRunStop < MOVING_TURN_THRESHOLD) { timeSinceRunStop++; }

	//update air dash timer
	if(state == AIR_DASHING)
	{
		falls = false;
		curAirDashDuration++;
		if(airDash == FORWARD_AIR_DASH && curAirDashDuration >= forwardAirDashDuration)
		{
			falls = true;
			state = JUMPING;
			if(!attacking && !blocking && curBlockstun <= 0)
			{
				ChangeHold(fighterEventHolds.jumpNeutralFall);
			}
			curAirDashDuration = 0;
			airDash = NO_AIR_DASH;
		}
		else if(airDash == BACKWARD_AIR_DASH && curAirDashDuration >= backwardAirDashDuration)
		{
			falls = true;
			state = JUMPING;
			if(!attacking && !blocking && curBlockstun <= 0)
			{
				ChangeHold(fighterEventHolds.jumpBackwardFall);
			}
			curAirDashDuration = 0;
			airDash = NO_AIR_DASH;
		}
	}

	//update stun
	bool hitstunEnd = false;
	bool blockstunEnd = false;
	if(curHitstun > 0)
	{
		curHitstun--;
		blocking = false;
		if(curHitstun <= 0)
		{
			hitstunEnd = true;
		}
	}
	if(curBlockstun > 0)
	{
		curBlockstun--;
		blocking = false;
		if(curBlockstun > 0)
		{
			blocking = true;
		}
		else
		{
			blockstunEnd = true;
		}
	}

	//if knockout has ended, stop here
	if(state == KNOCKOUT && curHold->nextHold == NULL)
	{
		framesSinceKnockout++;

		if(framesSinceKnockout > WAIT_AFTER_KNOCKOUT)
		{
			framesSinceKnockout = 0;
			ChangeHold(fighterEventHolds.standing);
			state = STANDING;
			vel.x = 0;
			vel.y = 0;
			falls = false;
		}

		return 0;
	}

	//do the base hold advancement
	if(int error = HSObject::AdvanceHolds() != 0) { return error; }

	//if stun ended, apply the new appropriate hold here
	if(hitstunEnd || blockstunEnd)
	{
		ChangeHold(GetDefaultHold());
	}

	return 0;
}

int Fighter::Event(InputStates * inputHistory, int frame)
{
	//dash
	if(inputHistory->frame == frame && (ForwardPressed(inputHistory, frame, frame) && ForwardWasTapped(inputHistory, frame, frame - RUN_INPUT_FRAMES)) || ForwardHardPressed(inputHistory, frame, frame))
	{
		bufferedAction = DASH;
		bufferedActionAge = 0;
	}
	else if(inputHistory->frame == frame && (BackwardPressed(inputHistory, frame, frame) && BackwardWasTapped(inputHistory, frame, frame - RUN_INPUT_FRAMES)) || BackwardHardPressed(inputHistory, frame, frame))
	{
		bufferedAction = BACKWARD_DASH;
		bufferedActionAge = 0;
	}

	//jump
	if(inputHistory->frame == frame && (inputHistory->bKeyJump.pressed || inputHistory->bButtonJump.pressed))
	{
		bufferedAction = JUMP;
		bufferedActionAge = 0;
	}

	//light attacks
	if(inputHistory->frame == frame && (inputHistory->bKeyLight.pressed || inputHistory->bButtonLight.pressed))
	{
		if(QCFInput(inputHistory, frame, frame - COMMAND_INPUT_THRESHOLD) &&
			((fighterEventHolds.attackLightQCFGround != NULL && (state == STANDING || state == CROUCHING || state == WALKING || state == RUNNING || (state == JUMPING && jumpStartup))) ||
			(fighterEventHolds.attackLightQCFAir != NULL && state == JUMPING)))
		{
			bufferedAction = QCF_LIGHT;
			bufferedActionAge = 0;
		}
		else if(inputHistory->bKeyUp.held || inputHistory->bButtonUp.held || inputHistory->bHatUp.held || inputHistory->bStickUp.held)
		{
			bufferedAction = UP_LIGHT;
			bufferedActionAge = 0;
		}
		else if(inputHistory->bKeyDown.held || inputHistory->bButtonDown.held || inputHistory->bHatDown.held || inputHistory->bStickDown.held)
		{
			bufferedAction = DOWN_LIGHT;
			bufferedActionAge = 0;
		}
		else if(ForwardHeld(inputHistory, frame))
		{
			bufferedAction = FORWARD_LIGHT;
			bufferedActionAge = 0;
		}
		else if(BackwardHeld(inputHistory, frame))
		{
			bufferedAction = BACKWARD_LIGHT;
			bufferedActionAge = 0;
		}
		else
		{
			bufferedAction = NEUTRAL_LIGHT;
			bufferedActionAge = 0;
		}
	}

	//heavy attacks
	if(inputHistory->frame == frame && (inputHistory->bKeyHeavy.pressed || inputHistory->bButtonHeavy.pressed))
	{
		if(QCFInput(inputHistory, frame, frame - COMMAND_INPUT_THRESHOLD) &&
			((fighterEventHolds.attackHeavyQCFGround != NULL && (state == STANDING || state == CROUCHING || state == WALKING || state == RUNNING || (state == JUMPING && jumpStartup))) ||
			(fighterEventHolds.attackHeavyQCFAir != NULL && state == JUMPING)))
		{
			bufferedAction = QCF_HEAVY;
			bufferedActionAge = 0;
		}
		else if(inputHistory->bKeyUp.held || inputHistory->bButtonUp.held || inputHistory->bHatUp.held || inputHistory->bStickUp.held)
		{
			bufferedAction = UP_HEAVY;
			bufferedActionAge = 0;
		}
		else if(inputHistory->bKeyDown.held || inputHistory->bButtonDown.held || inputHistory->bHatDown.held || inputHistory->bStickDown.held)
		{
			bufferedAction = DOWN_HEAVY;
			bufferedActionAge = 0;
		}
		else if(ForwardHeld(inputHistory, frame))
		{
			bufferedAction = FORWARD_HEAVY;
			bufferedActionAge = 0;
		}
		else if(BackwardHeld(inputHistory, frame))
		{
			bufferedAction = BACKWARD_HEAVY;
			bufferedActionAge = 0;
		}
		else
		{
			bufferedAction = NEUTRAL_HEAVY;
			bufferedActionAge = 0;
		}
	}

	return ExecuteAction(inputHistory, frame);
}

int Fighter::ExecuteAction(InputStates * inputHistory, int frame)
{
	landingAction = NO_ACTION;
	turnUponLanding = false;

	if(bufferedAction != NO_ACTION)
	{
		if(bufferedActionAge > BUFFERED_ACTION_LIFETIME)
		{
			bufferedAction = NO_ACTION;
		}
		else
		{
			bufferedActionAge++;
		}
	}

	//stun
	if(curHitstun > 0 || curHitstop > 0 || state == KNOCKOUT || state == KNOCKOUT_AIR)
	{
		return 0;
	}

	//left/right movement
	if(inputHistory->bKeyLeft.held || inputHistory->bButtonLeft.held || inputHistory->bHatLeft.held || inputHistory->bStickLeft.held)
	{
		//air control
		if(state == JUMPING && !jumpStartup && !jumpStartupJustEnded)
		{
			airControl = CONTROL_LEFT;
			landingAction = MOVE;
			if(facing == RIGHT)
			{
				turnUponLanding = true;
			}
		}

		if(facing == LEFT)
		{
			/*if(state == STANDING && (turning || runStopping) && (ForwardWasTapped(inputHistory, frame, frame - RUN_INPUT_FRAMES) || ForwardHardPressed(inputHistory, frame, frame)))
			{
				runAfterTurning = true;
			}
			else */if(!blocking && state == STANDING && !turning && !runStopping)
			{
				if(runAfterTurning)
				{
					ChangeHold(fighterEventHolds.run);
					if(runAfterTurning)
					{
						walkAfterTurning = false;
						runAfterTurning = false;
					}
					state = RUNNING;
				}
				else if(!attacking)
				{
					if(walkAfterTurning)
					{
						ChangeHold(fighterEventHolds.walking);
						walkAfterTurning = false;
						runAfterTurning = false;
					}
					else
					{
						ChangeHold(fighterEventHolds.walk);
					}
					state = WALKING;
				}
			}
			/*else if(!blocking && state == JUMPING && !jumpStartup && !jumpStartupJustEnded)
			{
				if(CanDashCancel() && curAirActions > 0 && (ForwardWasTapped(inputHistory, frame, frame - AIR_DASH_INPUT_FRAMES) || ForwardHardPressed(inputHistory, frame, frame)))
				{
					ChangeHold(fighterEventHolds.airDashForward);
					state = AIR_DASHING;
					airDash = FORWARD_AIR_DASH;
					airControl = NO_AIR_CONTROL;
					curAirActions--;
					landingAction = DASH;
					turnUponLanding = false;
				}
			}*/
		}
		else
		{
			if(blocking && state != JUMPING)
			{
				facing = LEFT;
			}
			else if(!attacking && !blocking && ((state == STANDING && timeSinceWalkStop < MOVING_TURN_THRESHOLD) || state == WALKING))
			{
				ChangeHold(fighterEventHolds.walkingTurn);
				facing = LEFT;
				turning = true;
				walkAfterTurning = false;
				runAfterTurning = false;
				/*if(ForwardHardPressed(inputHistory, frame, frame)) { runAfterTurning = true; }
				else { */walkAfterTurning = true; //}
				timeSinceWalkStop = MOVING_TURN_THRESHOLD + 1;
				state = STANDING;
			}
			else if(!attacking && !blocking && state == STANDING && !runStopping)
			{
				ChangeHold(fighterEventHolds.turn);
				facing = LEFT;
				turning = true;
				walkAfterTurning = false;
				runAfterTurning = false;
				//if(ForwardHardPressed(inputHistory, frame, frame)) { runAfterTurning = true; }
				state = STANDING;
			}
			else if(!attacking && !blocking && state == CROUCHING)
			{
				ChangeHold(fighterEventHolds.crouchingTurn);
				facing = LEFT;
				turning = true;
			}
			else if(!attacking && !blocking && ((state == STANDING && timeSinceRunStop < MOVING_TURN_THRESHOLD && runStopping) || state == RUNNING))
			{
				ChangeHold(fighterEventHolds.runningTurn);
				facing = LEFT;
				turning = true;
				runStopping = false;
				walkAfterTurning = false;
				runAfterTurning = true;
				timeSinceRunStop = MOVING_TURN_THRESHOLD + 1;
				sliding = true;
				state = STANDING;
			}
			/*else if(!blocking && state == JUMPING && !jumpStartup && !jumpStartupJustEnded)
			{
				if(CanDashCancel() && curAirActions > 0 && (BackwardWasTapped(inputHistory, frame, frame - AIR_DASH_INPUT_FRAMES) || BackwardHardPressed(inputHistory, frame, frame)))
				{
					ChangeHold(fighterEventHolds.airDashBackward);
					state = AIR_DASHING;
					airDash = BACKWARD_AIR_DASH;
					curAirActions--;
					landingAction = DASH;
					turnUponLanding = true;
				}
			}*/
		}
	}
	else if(inputHistory->bKeyRight.held || inputHistory->bButtonRight.held || inputHistory->bHatRight.held || inputHistory->bStickRight.held)
	{
		//air control
		if(state == JUMPING && !jumpStartup && !jumpStartupJustEnded)
		{
			airControl = CONTROL_RIGHT;
			landingAction = MOVE;
			if(facing == LEFT)
			{
				turnUponLanding = true;
			}
		}

		if(facing == LEFT)
		{
			if(blocking && state != JUMPING)
			{
				facing = RIGHT;
			}
			else if(!attacking && !blocking && ((state == STANDING && timeSinceWalkStop < MOVING_TURN_THRESHOLD) || state == WALKING))
			{
				ChangeHold(fighterEventHolds.walkingTurn);
				facing = RIGHT;
				turning = true;
				walkAfterTurning = false;
				runAfterTurning = false;
				/*if(ForwardHardPressed(inputHistory, frame, frame)) { runAfterTurning = true; }
				else { */walkAfterTurning = true; //}
				timeSinceWalkStop = MOVING_TURN_THRESHOLD + 1;
				state = STANDING;
			}
			else if(!attacking && !blocking && state == STANDING && !runStopping)
			{
				ChangeHold(fighterEventHolds.turn);
				facing = RIGHT;
				turning = true;
				walkAfterTurning = false;
				runAfterTurning = false;
				//if(ForwardHardPressed(inputHistory, frame, frame)) { runAfterTurning = true; }
				state = STANDING;
			}
			else if(!attacking && !blocking && state == CROUCHING)
			{
				ChangeHold(fighterEventHolds.crouchingTurn);
				facing = RIGHT;
				turning = true;
			}
			else if(!attacking && !blocking && ((state == STANDING && timeSinceRunStop < MOVING_TURN_THRESHOLD && runStopping) || state == RUNNING))
			{
				ChangeHold(fighterEventHolds.runningTurn);
				facing = RIGHT;
				turning = true;
				runStopping = false;
				walkAfterTurning = false;
				runAfterTurning = true;
				sliding = true;
				timeSinceRunStop = MOVING_TURN_THRESHOLD + 1;
				state = STANDING;
			}
			/*else if(!blocking && state == JUMPING && !jumpStartup && !jumpStartupJustEnded)
			{
				if(CanDashCancel() && curAirActions > 0 && (BackwardWasTapped(inputHistory, frame, frame - AIR_DASH_INPUT_FRAMES) || BackwardHardPressed(inputHistory, frame, frame)))
				{
					ChangeHold(fighterEventHolds.airDashBackward);
					state = AIR_DASHING;
					airDash = BACKWARD_AIR_DASH;
					curAirActions--;
					landingAction = DASH;
					turnUponLanding = true;
				}
			}*/
		}
		else
		{
			/*if(state == STANDING && (turning || runStopping) && (ForwardWasTapped(inputHistory, frame, frame - RUN_INPUT_FRAMES) || ForwardHardPressed(inputHistory, frame, frame)))
			{
				runAfterTurning = true;
			}
			else */if(!blocking && state == STANDING && !turning && !runStopping)
			{
				if(runAfterTurning)
				{ 
					ChangeHold(fighterEventHolds.run);
					if(runAfterTurning)
					{
						walkAfterTurning = false;
						runAfterTurning = false;
					}
					state = RUNNING;
				}
				else if(!attacking)
				{
					if(walkAfterTurning)
					{
						ChangeHold(fighterEventHolds.walking);
						walkAfterTurning = false;
						runAfterTurning = false;
					}
					else
					{
						ChangeHold(fighterEventHolds.walk);
					}
					state = WALKING;
				}
			}
			else if(!blocking && state == JUMPING && !jumpStartup && !jumpStartupJustEnded)
			{
				/*if(CanDashCancel() && curAirActions > 0 && (ForwardWasTapped(inputHistory, frame, frame - AIR_DASH_INPUT_FRAMES) || ForwardHardPressed(inputHistory, frame, frame)))
				{
					ChangeHold(fighterEventHolds.airDashForward);
					state = AIR_DASHING;
					airDash = FORWARD_AIR_DASH;
					airControl = NO_AIR_CONTROL;
					curAirActions--;
					landingAction = DASH;
					turnUponLanding = false;
				}*/
			}
		}
	}
	else
	{
		walkAfterTurning = false;
		runAfterTurning = false;
		if(state == WALKING)
		{
			ChangeHold(fighterEventHolds.standing);
			timeSinceWalkStop = 0;
			state = STANDING;
		}
		else if(state == RUNNING)
		{
			ChangeHold(fighterEventHolds.runningStop);
			timeSinceRunStop = 0;
			runStopping = true;
			sliding = true;
			state = STANDING;
		}
	}

	//dash
	if(CanDashCancel() && !turning && !runStopping && !blocking)
	{
		if(bufferedAction == DASH)
		{
			if(state == STANDING || state == WALKING || state == CROUCHING)
			{
				bufferedAction = NO_ACTION;
				ChangeHold(fighterEventHolds.run);
				walkAfterTurning = false;
				runAfterTurning = false;
				attacking = false;
				state = RUNNING;
			}
			else if(state == JUMPING && curAirActions > 0 && !jumpStartup && !jumpStartupJustEnded)
			{
				bufferedAction = NO_ACTION;
				ChangeHold(fighterEventHolds.airDashForward);
				state = AIR_DASHING;
				airDash = FORWARD_AIR_DASH;
				airControl = NO_AIR_CONTROL;
				curAirActions--;
				landingAction = DASH;
				turnUponLanding = false;
				attacking = false;
			}
		}
		else if(bufferedAction == BACKWARD_DASH && state == JUMPING && curAirActions > 0 && !jumpStartup && !jumpStartupJustEnded)
		{
			bufferedAction = NO_ACTION;
			ChangeHold(fighterEventHolds.airDashBackward);
			state = AIR_DASHING;
			airDash = BACKWARD_AIR_DASH;
			curAirActions--;
			landingAction = DASH;
			turnUponLanding = true;
			attacking = false;
		}
	}

	//crouch
	if(inputHistory->bKeyDown.held || inputHistory->bButtonDown.held || inputHistory->bHatDown.held || inputHistory->bStickDown.held)
	{
		if(!attacking && (state == STANDING || state == WALKING || state == RUNNING))
		{
			if(blocking || curBlockstun > 0)
			{
				ChangeHold(fighterEventHolds.blockLow);
			}
			else
			{
				ChangeHold(fighterEventHolds.crouch);
			}
			walkAfterTurning = false;
			runAfterTurning = false;
			turning = false;
			runStopping = false;
			if(state == RUNNING) { sliding = true; }
			state = CROUCHING;
		}
		else if(state == JUMPING)
		{
			landingAction = CROUCH;
			//if(inputHistory->bKeyJump.held || inputHistory->bButtonJump.held)
			//{
				ignoreJumpThroughTerrain = true;
			//}
		}
	}
	else if(!attacking && state == CROUCHING)
	{
		if(blocking || curBlockstun > 0)
		{
			ChangeHold(fighterEventHolds.blockHigh);
		}
		else
		{
			ChangeHold(fighterEventHolds.stand);
		}
		turning = false;
		state = STANDING;
	}

	//jump
	if(CanJumpCancel() && !jumpStartup && !blocking && curBlockstun <= 0 && bufferedAction == JUMP)
	{
		if(state != JUMPING && state != AIRBORN && state != AIR_DASHING)
		{
			bufferedAction = NO_ACTION;
			//ground jump
			if(state == CROUCHING)
			{
				ignoreJumpThroughTerrain = true;
			}
			else
			{
				ChangeHold(fighterEventHolds.jumpNeutralStart);
				if(state == STANDING)
				{
					if(turning || runStopping || attacking)
					{
						turning = false;
						runStopping = false;
						attacking = false;
						//base it on which direction is being held
						if(inputHistory->bKeyLeft.held || inputHistory->bButtonLeft.held || inputHistory->bHatLeft.held || inputHistory->bStickLeft.held)
						{
							if(facing == LEFT)
							{
								jump = WALKING_JUMP;
							}
							else
							{
								jump = BACKWARD_JUMP;
							}
						}
						else if(inputHistory->bKeyRight.held || inputHistory->bButtonRight.held || inputHistory->bHatRight.held || inputHistory->bStickRight.held)
						{
							if(facing == LEFT)
							{
								jump = BACKWARD_JUMP;
							}
							else
							{
								jump = WALKING_JUMP;
							}
						}
						else
						{
							jump = STANDING_JUMP;
						}
					}
					else
					{
						jump = STANDING_JUMP;
					}
				}
				if(state == WALKING)
				{
					jump = WALKING_JUMP;
				}
				if(state == RUNNING)
				{
					jump = RUNNING_JUMP;
				}
				walkAfterTurning = false;
				runAfterTurning = false;
				jumpStartup = true;
				shortHop = false;
				state = JUMPING;
			}
		}
		else if(state == JUMPING)// && !(inputHistory->bKeyDown.held || inputHistory->bButtonDown.held || inputHistory->bHatDown.held || inputHistory->bStickDown.held))
		{
			bufferedAction = NO_ACTION;
			//air jump
			if(curAirActions > 0)
			{
				ChangeHold(fighterEventHolds.jumpNeutralStartAir);
				if(inputHistory->bKeyLeft.held || inputHistory->bButtonLeft.held || inputHistory->bHatLeft.held || inputHistory->bStickLeft.held)
				{
					if(facing == LEFT)
					{
						if(airVelocityCategory == HIGH_VELOCITY_FORWARD)
						{
							jump = RUNNING_JUMP;
						}
						else
						{
							jump = WALKING_JUMP;
						}
					}
					else
					{
						if(airVelocityCategory == HIGH_VELOCITY_FORWARD)
						{
							jump = STANDING_JUMP;
						}
						else
						{
							jump = BACKWARD_JUMP;
						}
					}
				}
				else if(inputHistory->bKeyRight.held || inputHistory->bButtonRight.held || inputHistory->bHatRight.held || inputHistory->bStickRight.held)
				{
					if(facing == LEFT)
					{
						if(airVelocityCategory == HIGH_VELOCITY_FORWARD)
						{
							jump = STANDING_JUMP;
						}
						else
						{
							jump = BACKWARD_JUMP;
						}
					}
					else
					{
						if(airVelocityCategory == HIGH_VELOCITY_FORWARD)
						{
							jump = RUNNING_JUMP;
						}
						else
						{
							jump = WALKING_JUMP;
						}
					}
				}
				else
				{
					if(airVelocityCategory == HIGH_VELOCITY_FORWARD)
					{
						jump = WALKING_JUMP;
					}
					else
					{
						jump = STANDING_JUMP;
					}
				}
				attacking = false;
				curAirActions--;
			}
		}
	}

	//short hop
	if(jumpStartup && !inputHistory->bKeyJump.held && !inputHistory->bButtonJump.held)
	{
		shortHop = true;
	}

	//blocking
	if(!attacking && !blocking && curBlockstun <= 0 && (inputHistory->bKeyBlock.held || inputHistory->bButtonBlock.held))
	{
		if(state == STANDING || state == WALKING || state == RUNNING || state == CROUCHING || (state == JUMPING && jumpStartup))
		{
			if(state == RUNNING)
			{
				sliding = true;
			}
			if(inputHistory->bKeyDown.held || inputHistory->bButtonDown.held || inputHistory->bStickDown.held || inputHistory->bHatDown.held)
			{
				ChangeHold(fighterEventHolds.blockLow);
				state = CROUCHING;
			}
			else
			{
				ChangeHold(fighterEventHolds.blockHigh);
				state = STANDING;
			}
			walkAfterTurning = false;
			runAfterTurning = false;
			turning = false;
			runStopping = false;
			jump = NO_JUMP;
			blocking = true;
			jumpStartup = false;
			shortHop = false;
		}
		else if(state == JUMPING || state == AIR_DASHING)
		{
			ChangeHold(fighterEventHolds.blockAir);
			blocking = true;
		}
	}
	else if(blocking && curBlockstun <= 0 && !inputHistory->bKeyBlock.held && !inputHistory->bButtonBlock.held)
	{
		if(state == STANDING)
		{
			ChangeHold(fighterEventHolds.standing);
		}
		else if(state == CROUCHING)
		{
			ChangeHold(fighterEventHolds.crouching);
		}
		else if(state == JUMPING)
		{
			if(vel.y < 0)
			{
				if((facing == LEFT && vel.x > 0) || (facing == RIGHT && vel.x < 0))
				{
					ChangeHold(fighterEventHolds.jumpBackwardRising);
				}
				else
				{
					ChangeHold(fighterEventHolds.jumpNeutralRising);
				}
			}
			else
			{
				ChangeHold(fighterEventHolds.jumpNeutralFalling);
			}
		}
		else if(state == AIR_DASHING)
		{
			if(airDash == FORWARD_AIR_DASH)
			{
				ChangeHold(fighterEventHolds.airDashForward);
			}
			else if(airDash == BACKWARD_AIR_DASH)
			{
				ChangeHold(fighterEventHolds.airDashBackward);
			}
		}
		turning = false;
		runStopping = false;
		blocking = false;
	}

	if(curBlockstun > 0) { return 0; }

	//light attacks
	if(bufferedAction >= NEUTRAL_LIGHT && bufferedAction <= QCF_LIGHT)
	{
		if(state == STANDING || state == CROUCHING || state == WALKING || state == RUNNING || (state == JUMPING && jumpStartup))
		{
			if(CanLightQCFCancel() && fighterEventHolds.attackLightQCFGround != NULL && bufferedAction == QCF_LIGHT)
			{
				ChangeHold(fighterEventHolds.attackLightQCFGround);
				GroundAttackExecuted();
			}
			else if(CanLightUpCancel() && fighterEventHolds.attackLightUpGround != NULL && bufferedAction == UP_LIGHT)
			{
				ChangeHold(fighterEventHolds.attackLightUpGround);
				GroundAttackExecuted();
			}
			else if(CanLightDownCancel() && fighterEventHolds.attackLightDownGround != NULL && bufferedAction == DOWN_LIGHT)
			{
				ChangeHold(fighterEventHolds.attackLightDownGround);
				GroundAttackExecuted();
				state = CROUCHING;
			}
			else if(CanLightForwardCancel() && fighterEventHolds.attackLightForwardGround != NULL && (bufferedAction == FORWARD_LIGHT))
			{
				ChangeHold(fighterEventHolds.attackLightForwardGround);
				GroundAttackExecuted();
			}
			else if(CanLightNeutralCancel() && fighterEventHolds.attackLightNeutralGround != NULL && (bufferedAction == NEUTRAL_LIGHT || bufferedAction == BACKWARD_LIGHT))
			{
				ChangeHold(fighterEventHolds.attackLightNeutralGround);
				GroundAttackExecuted();
			}
			bufferedAction = NO_ACTION;
		}
		else if(state == JUMPING || state == AIR_DASHING)
		{
			if(CanLightQCFCancel() && fighterEventHolds.attackLightQCFAir != NULL && bufferedAction == QCF_LIGHT)
			{
				ChangeHold(fighterEventHolds.attackLightQCFAir);
				AirAttackExecuted();
				landingAction = QCF_LIGHT;
			}
			else if(CanLightUpCancel() && fighterEventHolds.attackLightUpAir != NULL && bufferedAction == UP_LIGHT)
			{
				ChangeHold(fighterEventHolds.attackLightUpAir);
				AirAttackExecuted();
				landingAction = UP_LIGHT;
			}
			else if(CanLightDownCancel() && fighterEventHolds.attackLightDownAir != NULL && bufferedAction == DOWN_LIGHT)
			{
				ChangeHold(fighterEventHolds.attackLightDownAir);
				AirAttackExecuted();
				landingAction = DOWN_LIGHT;
			}
			else if(CanLightForwardCancel() && fighterEventHolds.attackLightForwardAir != NULL && bufferedAction == FORWARD_LIGHT)
			{
				ChangeHold(fighterEventHolds.attackLightForwardAir);
				AirAttackExecuted();
				landingAction = FORWARD_LIGHT;
				turnUponLanding = false;
			}
			else if(CanLightBackwardCancel() && fighterEventHolds.attackLightBackwardAir != NULL && bufferedAction == BACKWARD_LIGHT)
			{
				ChangeHold(fighterEventHolds.attackLightBackwardAir);
				AirAttackExecuted();
				landingAction = FORWARD_LIGHT;
				turnUponLanding = true;
			}
			else if(CanLightNeutralCancel() && fighterEventHolds.attackLightNeutralAir != NULL && bufferedAction == NEUTRAL_LIGHT)
			{
				ChangeHold(fighterEventHolds.attackLightNeutralAir);
				AirAttackExecuted();
				landingAction = NEUTRAL_LIGHT;
			}
			bufferedAction = NO_ACTION;
		}
	}

	//heavy attacks
	if(bufferedAction >= NEUTRAL_HEAVY && bufferedAction <= QCF_HEAVY)
	{
		if(state == STANDING || state == CROUCHING || state == WALKING || state == RUNNING || (state == JUMPING && jumpStartup))
		{
			if(CanHeavyQCFCancel() && fighterEventHolds.attackHeavyQCFGround != NULL && bufferedAction == QCF_HEAVY)
			{
				ChangeHold(fighterEventHolds.attackHeavyQCFGround);
				GroundAttackExecuted();
			}
			else if(CanHeavyUpCancel() && fighterEventHolds.attackHeavyUpGround != NULL && bufferedAction == UP_HEAVY)
			{
				ChangeHold(fighterEventHolds.attackHeavyUpGround);
				GroundAttackExecuted();
			}
			else if(CanHeavyDownCancel() && fighterEventHolds.attackHeavyDownGround != NULL && bufferedAction == DOWN_HEAVY)
			{
				ChangeHold(fighterEventHolds.attackHeavyDownGround);
				GroundAttackExecuted();
				state = CROUCHING;
			}
			else if(CanHeavyForwardCancel() && fighterEventHolds.attackHeavyForwardGround != NULL && (bufferedAction == FORWARD_HEAVY))
			{
				ChangeHold(fighterEventHolds.attackHeavyForwardGround);
				GroundAttackExecuted();
			}
			else if(CanHeavyNeutralCancel() && fighterEventHolds.attackHeavyNeutralGround != NULL && (bufferedAction == NEUTRAL_HEAVY || bufferedAction == BACKWARD_HEAVY))
			{
				ChangeHold(fighterEventHolds.attackHeavyNeutralGround);
				GroundAttackExecuted();
			}
			bufferedAction = NO_ACTION;
		}
		else if(state == JUMPING || state == AIR_DASHING)
		{
			if(CanHeavyQCFCancel() && fighterEventHolds.attackHeavyQCFAir != NULL && bufferedAction == QCF_HEAVY)
			{
				ChangeHold(fighterEventHolds.attackHeavyQCFAir);
				AirAttackExecuted();
				landingAction = QCF_HEAVY;
			}
			else if(CanHeavyUpCancel() && fighterEventHolds.attackHeavyUpAir != NULL && bufferedAction == UP_HEAVY)
			{
				ChangeHold(fighterEventHolds.attackHeavyUpAir);
				AirAttackExecuted();
				landingAction = UP_HEAVY;
			}
			else if(CanHeavyDownCancel() && fighterEventHolds.attackHeavyDownAir != NULL && bufferedAction == DOWN_HEAVY)
			{
				ChangeHold(fighterEventHolds.attackHeavyDownAir);
				AirAttackExecuted();
				landingAction = DOWN_HEAVY;
			}
			else if(CanHeavyForwardCancel() && fighterEventHolds.attackHeavyForwardAir != NULL && bufferedAction == FORWARD_HEAVY)
			{
				ChangeHold(fighterEventHolds.attackHeavyForwardAir);
				AirAttackExecuted();
				landingAction = FORWARD_HEAVY;
			}
			else if(CanHeavyBackwardCancel() && fighterEventHolds.attackHeavyBackwardAir != NULL && bufferedAction == BACKWARD_HEAVY)
			{
				ChangeHold(fighterEventHolds.attackHeavyBackwardAir);
				AirAttackExecuted();
				landingAction = FORWARD_HEAVY;
			}
			else if(CanHeavyNeutralCancel() && fighterEventHolds.attackHeavyNeutralAir != NULL && bufferedAction == NEUTRAL_HEAVY)
			{
				ChangeHold(fighterEventHolds.attackHeavyNeutralAir);
				AirAttackExecuted();
				landingAction = NEUTRAL_HEAVY;
			}
			bufferedAction = NO_ACTION;
		}
	}

	jumpStartupJustEnded = false;

	return 0;
}

int Fighter::Update()
{
	//apply push
	HSVectComp prevVelX = vel.x;
	vel.x += fighterPushXAccel;
	fighterPushXAccel = 0;

	//save velocity before ignoring it during hitstop
	bool didFall = falls;
	HSVect2D hadVel = vel;
	if(curHitstop > 0)
	{
		vel.x = 0;
		vel.y = 0;
		falls = false;
	}

	if(int error = PhysicsObject::Update() != 0) { return error; } //there was an error in the base update

	//reapply velocity after ignoring it during hitstop
	if(curHitstop > 0)
	{
		vel = hadVel;
		falls = didFall;
	}

	//return to velocity before push
	prevVel.x = prevVelX;
	vel.x = prevVelX;

	//handle facing direction
	if(facing == LEFT)
	{
		hFlip = true;
	}
	else
	{
		hFlip = false;
	}

	//handle terrain boxes
	if(state == STANDING || state == WALKING || state == RUNNING || state == JUMPING)
	{
		ChangeTerrainBoxes(firstUprightTerrainBox);
	}
	else if(state == CROUCHING || state == KNOCKOUT)
	{
		ChangeTerrainBoxes(firstUprightTerrainBox);
	}
	else if(state == AIR_DASHING || state == AIRBORN || state == KNOCKOUT_AIR)
	{
		ChangeTerrainBoxes(firstUprightTerrainBox);
	}

	//handle motion
	if((state != JUMPING && state != AIRBORN && state != KNOCKOUT_AIR) || (state == JUMPING && jumpStartup))
	{
		vel.y = 0;
	}

	if(state == STANDING || state == CROUCHING || (state == JUMPING && jumpStartup) || state == KNOCKOUT)
	{
		if(sliding)
		{
			float totalFriction = (1 - friction) * (1 - currentSurfaceFriction);
			vel.x *= totalFriction;
			if(abs(vel.x) < MIN_VEL)
			{
				vel.x = 0;
				sliding = false;
			}
		}
		else
		{
			vel.x = 0;
		}
	}
	else
	{
		sliding = false;
		walkAfterTurning = false;
		runAfterTurning = false;
	}
	if(jump == STANDING_JUMP && !jumpStartup)
	{
		vel.x = 0;
	}
	if(state == WALKING)
	{
		vel.x = walkSpeed * facing;
	}
	if(state == RUNNING)
	{
		vel.x = runSpeed * facing;
	}
	if(state == AIR_DASHING)
	{
		if(airDash == FORWARD_AIR_DASH)
		{
			falls = false;
			vel.x = forwardAirDashSpeed * facing;
		}
		else if(airDash == BACKWARD_AIR_DASH)
		{
			falls = false;
			vel.x = backwardAirDashSpeed * -facing;
		}
		if((facing == LEFT && vel.x < -maxAirControlSpeed) || (facing == RIGHT && vel.x > maxAirControlSpeed))
		{
			airVelocityCategory = HIGH_VELOCITY_FORWARD;
		}
		else if((facing == RIGHT && vel.x < -maxAirControlSpeed) || (facing == LEFT && vel.x > maxAirControlSpeed))
		{
			airVelocityCategory = HIGH_VELOCITY_BACKWARD;
		}
		else
		{
			airVelocityCategory = LOW_VELOCITY;
		}
	}
	if(state == JUMPING && !jumpStartup)
	{
		//air control
		if(airControl != NO_AIR_CONTROL)
		{
			if(airControl == CONTROL_LEFT &&  vel.x > -maxAirControlSpeed)
			{
				vel.x -= airControlAccel;
				if(vel.x < -maxAirControlSpeed)
				{
					vel.x = -maxAirControlSpeed;
				}
			}
			else if(airControl == CONTROL_RIGHT &&  vel.x < maxAirControlSpeed)
			{
				vel.x += airControlAccel;
				if(vel.x > maxAirControlSpeed)
				{
					vel.x = maxAirControlSpeed;
				}
			}
			airControl = NO_AIR_CONTROL;
		}

		//make jump happen
		if(jump != NO_JUMP)
		{
			if(curAirActions == airActions)
			{
				ChangeHold(fighterEventHolds.jumpNeutralRising);
			}

			if(jump == BACKWARD_JUMP)
			{
				vel.x = walkSpeed * -facing;
				if(curAirActions == airActions)
				{
					ChangeHold(fighterEventHolds.jumpBackwardRising);
				}
			}
			else if(jump == STANDING_JUMP)
			{
				vel.x = 0;
			}
			else if(jump == WALKING_JUMP)
			{
				vel.x = walkSpeed * facing;
			}
			else if(jump == RUNNING_JUMP)
			{
				vel.x = runSpeed * facing;
			}

			if(shortHop)
			{
				vel.y = -jumpSpeed/2;
			}
			else
			{
				vel.y = -jumpSpeed;
			}
			shortHop = false;
			jump = NO_JUMP;
			falls = true;
		}

		//figure out the air velocity category
		if((facing == LEFT && vel.x < -maxAirControlSpeed) || (facing == RIGHT && vel.x > maxAirControlSpeed))
		{
			airVelocityCategory = HIGH_VELOCITY_FORWARD;
		}
		else if((facing == RIGHT && vel.x < -maxAirControlSpeed) || (facing == LEFT && vel.x > maxAirControlSpeed))
		{
			airVelocityCategory = HIGH_VELOCITY_BACKWARD;
		}
		else
		{
			airVelocityCategory = LOW_VELOCITY;
		}

		//jump animation changes based on change in velocity
		if(!attacking && !blocking && curHitstun <= 0 && curBlockstun <= 0 && state != KNOCKOUT_AIR)
		{
			if(vel.y < 0)
			{
				//jump rising
				if((facing == LEFT && prevVel.x <= 0 && vel.x > 0) || (facing == RIGHT && prevVel.x >= 0 && vel.x < 0))
				{
					ChangeHold(fighterEventHolds.jumpBackwardRising);
				}
				else if((facing == LEFT && prevVel.x > 0 && vel.x <= 0) || (facing == RIGHT && prevVel.x < 0 && vel.x >= 0))
				{
					ChangeHold(fighterEventHolds.jumpNeutralRising);
				}
			}
			if(prevVel.y < 0 && vel.y >= 0)
			{
				//jump peak
				if((facing == LEFT && vel.x > 0) || (facing == RIGHT && vel.x < 0))
				{
					ChangeHold(fighterEventHolds.jumpBackwardFall);
				}
				else
				{
					ChangeHold(fighterEventHolds.jumpNeutralFall);
				}
			}
		}
	}

	return 0;
}

void Fighter::HandleChangeInTerrainBoxes(list<HSObject*> * gameObjects)
{
	if(!terrainBoxesChanged) { return; }

	terrainBoxesChanged = false;

	return;

	//check if the new box's lower boundary is lower than the old one
	HSBox * oldBox = firstPreviousTerrainBox;
	float oldLowerBound = oldBox->offset.y + oldBox->height;
	oldBox = oldBox->nextBox;
	while(oldBox != NULL)
	{
		float lowerBound = oldBox->offset.y + oldBox->height;

		if(lowerBound > oldLowerBound) { oldLowerBound = lowerBound; }

		oldBox = oldBox->nextBox;
	}

	HSBox * newBox = firstTerrainBox;
	bool newIsLower = false;
	while(newBox != NULL)
	{
		float lowerBound = newBox->offset.y + newBox->height;

		if(lowerBound > oldLowerBound)
		{
			newIsLower = true;
			break;
		}
	}

	//new box's lower bound isn't lower than the old one, so change boxes and return
	if(!newIsLower) { return; }

	bool adjustmentRequired = true;
	HSBox * selfTerrainBox;
	TerrainObject * targetTerrainObject;
	HSBox * targetTerrainBox;

	while(adjustmentRequired)
	{
		adjustmentRequired = false;

		//check against jump-through platforms
		list<HSObject*>::iterator objIt;
		for ( objIt=gameObjects->begin(); objIt != gameObjects->end(); objIt++)
		{
			bool collisionWithOldTerrainBoxes = false;

			//loop through this object's previous terrain boxes
			selfTerrainBox = firstPreviousTerrainBox;
			while(selfTerrainBox != NULL)
			{
				//loop through the target's terrain boxes
				if(!(*objIt)->IsTerrain()) { continue; }
				targetTerrainObject = (TerrainObject*)(*objIt);

				targetTerrainBox = targetTerrainObject->firstTerrainBox;
				while(targetTerrainBox != NULL)
				{
					//check for collisions
					if(AreRectanglesColliding(&pos, selfTerrainBox, &(*objIt)->pos, targetTerrainBox))
					{
						collisionWithOldTerrainBoxes = true;
						break;
					}

					targetTerrainBox = targetTerrainBox->nextBox;
				}
				if(collisionWithOldTerrainBoxes)
				{
					break;
				}
				selfTerrainBox = selfTerrainBox->nextBox;
			}

			if(collisionWithOldTerrainBoxes)
			{
				continue;
			}

			//loop through this object's current terrain boxes
			selfTerrainBox = firstTerrainBox;
			while(selfTerrainBox != NULL)
			{
				//loop through the target's terrain boxes
				if(!(*objIt)->IsTerrain()) { continue; }
				targetTerrainObject = (TerrainObject*)(*objIt);

				targetTerrainBox = targetTerrainObject->firstTerrainBox;
				while(targetTerrainBox != NULL)
				{
					//check for collisions
					if(AreRectanglesColliding(&pos, selfTerrainBox, &(*objIt)->pos, targetTerrainBox))
					{
						adjustmentRequired = true;
						break;
					}

					targetTerrainBox = targetTerrainBox->nextBox;
				}
				if(adjustmentRequired)
				{
					break;
				}
				selfTerrainBox = selfTerrainBox->nextBox;
			}

			if(adjustmentRequired) { break; }
		}

		if(!adjustmentRequired) { break; }

		//move this object upward to put the selfTerrainBox just above the targetTerrainBox
		pos.y = targetTerrainObject->pos.y + targetTerrainBox->offset.y + selfTerrainBox->height - pos.y;
	}
}

void Fighter::HandleFighterCollison(TerrainObject * target)
{
	if(pos.x <= target->pos.x)
	{
		fighterPushXAccel -= FIGHTER_OVERLAP_PUSH_ACCEL;
	}
	else
	{
		fighterPushXAccel += FIGHTER_OVERLAP_PUSH_ACCEL;
	}
}

int Fighter::HandleTerrainCollision(list<HSObject*> * gameObjects, TerrainCollisionResult * result, HSVect2D * ownPos, HSVect2D * ownPrevPos)
{
	HSVect2D handlePos;
	handlePos.x = pos.x;
	handlePos.y = pos.y;
	HSVect2D handlePrevPos;
	handlePrevPos.x = prevPos.x;
	handlePrevPos.y = prevPos.y;

	if(ownPos != NULL)
	{
		handlePos.x = ownPos->x;
		handlePos.y = ownPos->y;
	}

	if(ownPrevPos != NULL)
	{
		handlePrevPos.x = ownPrevPos->x;
		handlePrevPos.y = ownPrevPos->y;
	}

	//get the new position
	HSVect2D newPos = CollisionReposition(gameObjects, result, &handlePos, &handlePrevPos);

	//handle fighter-specific impact physics
	if(state != JUMPING && state != AIRBORN && state != KNOCKOUT_AIR)
	{
		pos.x = newPos.x;
		pos.y = newPos.y;
	}
	else if(state == JUMPING || state == KNOCKOUT_AIR)
	{
		//set the new velocity, states, and position
		if(result->horizontalImpact)
		{
			vel.x = 0;
			airVelocityCategory = LOW_VELOCITY;
		}
		if(result->verticalImpact)
		{
			if(vel.y > 0)
			{
				HandleJumpLanding();
				currentSurfaceFriction = result->lastTargetObject->friction;
			}
			vel.y = 0;
		}
		pos.x = newPos.x;
		pos.y = newPos.y;

		if(vel.x != 0 || vel.y != 0)
		{
			//get the remaining movement distance not covered in this frame
			HSVect2D checkDist;
			checkDist.x = 0;
			checkDist.y = 0;

			if(vel.x != 0)
			{
				checkDist.x = (handlePos.x - handlePrevPos.x) - (pos.x - handlePrevPos.x);
			}
			if(vel.y != 0)
			{
				checkDist.y = (handlePos.y - handlePrevPos.y) - (pos.y - handlePrevPos.y);
			}

			if(checkDist.x == 0 && checkDist.y == 0)
			{
				return 0;
			}

			//do a collision check all over again, using the new position as the previous position, and that position plus the remaining movement distance as the current position
			HSVect2D checkPos;
			checkPos.x = pos.x + checkDist.x;
			checkPos.y = pos.y + checkDist.y;

			TerrainCollisionResult result = IsCollidingWithAnyTerrain(gameObjects, NULL, &checkPos, &pos);
			if(result.lastSelfBox != NULL)
			{
				HandleTerrainCollision(gameObjects, &result, &checkPos, &pos);
			}
			else
			{
				pos.x = checkPos.x;
				pos.y = checkPos.y;
			}
		}
	}
	else if(state == AIRBORN)
	{
		//handle terrain impact physics like this is a basic physics object
		CollisionPhysics(&newPos, result, &handlePos, &handlePrevPos);
	}

	return 0;
}

int Fighter::StepCheck(list<HSObject*> * gameObjects)
{
	HSVect2D checkPos;
	checkPos.x = pos.x;
	checkPos.y = pos.y + stepHeight;

	TerrainCollisionResult result = IsCollidingWithAnyTerrain(gameObjects, NULL, &checkPos, &pos);

	if(!result.verticalImpact)
	{
		if(state == RUNNING)
		{
			airVelocityCategory = HIGH_VELOCITY_FORWARD;
		}
		ChangeHold(fighterEventHolds.jumpNeutralFall);
		state = JUMPING;
		falls = true;
	}
	else
	{
		//save the friction of the surface currently stood upon
		currentSurfaceFriction = result.lastTargetObject->friction;
	}

	return 0;
}

int Fighter::HandleJumpLanding()
{
	curAirActions = airActions;
	falls = false;
	attacking = false;

	if(airVelocityCategory != LOW_VELOCITY)
	{
		sliding = true;
	}
	else
	{
		vel.x = 0;
	}

	if(state == KNOCKOUT_AIR)
	{
		ChangeHold(fighterEventHolds.knockout);
		state = KNOCKOUT;
		return 0;
	}

	if(turnUponLanding)
	{
		if(facing == LEFT)
		{
			facing = RIGHT;
			hFlip = false;
		}
		else
		{
			facing = LEFT;
			hFlip = true;
		}
	}

	switch(landingAction)
	{
		case NO_ACTION:
			if(blocking || curBlockstun > 0)
			{
				ChangeHold(fighterEventHolds.blockHigh);
			}
			else if(airVelocityCategory == HIGH_VELOCITY_FORWARD)
			{
				ChangeHold(fighterEventHolds.runningStop);
				runStopping = true;
				timeSinceRunStop = 0;
			}
			else if(airVelocityCategory == HIGH_VELOCITY_BACKWARD)
			{
				ChangeHold(fighterEventHolds.jumpNeutralLand);
				runStopping = true;
			}
			else
			{
				ChangeHold(fighterEventHolds.jumpNeutralLand);
			}
			state = STANDING;
			break;
		case MOVE:
			if(blocking || curBlockstun > 0)
			{
				ChangeHold(fighterEventHolds.blockHigh);
				state = STANDING;
			}
			else if(airVelocityCategory == HIGH_VELOCITY_FORWARD)
			{
				if(turnUponLanding)
				{
					ChangeHold(fighterEventHolds.runningTurn);
					state = STANDING;
					turning = true;
					runAfterTurning = true;
				}
				else
				{
					ChangeHold(fighterEventHolds.running);
					vel.x = runSpeed * facing;
					state = RUNNING;
				}
			}
			else if(airVelocityCategory == HIGH_VELOCITY_BACKWARD)
			{
				if(turnUponLanding)
				{
					ChangeHold(fighterEventHolds.turn);
					state = STANDING;
					turning = true;
				}
				else
				{
					ChangeHold(fighterEventHolds.jumpNeutralLand);
					runStopping = true;
					state = STANDING;
				}
			}
			else
			{
				if(turnUponLanding)
				{
					ChangeHold(fighterEventHolds.turn);
					state = STANDING;
					turning = true;
				}
				else
				{
					ChangeHold(fighterEventHolds.walking);
					vel.x = walkSpeed * facing;
					state = WALKING;
				}
			}
			break;
		case DASH:
			if(airVelocityCategory == HIGH_VELOCITY_BACKWARD)
			{
				if(turnUponLanding)
				{
					ChangeHold(fighterEventHolds.turn);
					state = STANDING;
					turning = true;
					runAfterTurning = true;
				}
				else
				{
					ChangeHold(fighterEventHolds.jumpNeutralLand);
					runStopping = true;
					runAfterTurning = true;
					state = STANDING;
				}
			}
			else
			{
				if(turnUponLanding)
				{
					ChangeHold(fighterEventHolds.runningTurn);
					state = STANDING;
					turning = true;
					runAfterTurning = true;
				}
				else
				{
					ChangeHold(fighterEventHolds.running);
					vel.x = runSpeed * facing;
					state = RUNNING;
				}
			}
			break;
		case CROUCH:
			if(blocking || curBlockstun > 0)
			{
				ChangeHold(fighterEventHolds.blockLow);
			}
			else
			{
				ChangeHold(fighterEventHolds.crouch);
			}
			state = CROUCHING;
			break;
		case JUMP:
			//ChangeHold(fighterEventHolds.jumpNeutralStart);
			break;
		case NEUTRAL_LIGHT:
			ChangeHold(fighterEventHolds.attackLightNeutralGround);
			GroundAttackExecuted();
			break;
		case UP_LIGHT:
			ChangeHold(fighterEventHolds.attackLightUpGround);
			GroundAttackExecuted();
			break;
		case DOWN_LIGHT:
			ChangeHold(fighterEventHolds.attackLightDownGround);
			GroundAttackExecuted();
			state = CROUCHING;
			break;
		case FORWARD_LIGHT:
			ChangeHold(fighterEventHolds.attackLightForwardGround);
			GroundAttackExecuted();
			break;
		case QCF_LIGHT:
			ChangeHold(fighterEventHolds.attackLightQCFGround);
			GroundAttackExecuted();
			break;
		case NEUTRAL_HEAVY:
			ChangeHold(fighterEventHolds.attackHeavyNeutralGround);
			GroundAttackExecuted();
			break;
		case UP_HEAVY:
			ChangeHold(fighterEventHolds.attackHeavyUpGround);
			GroundAttackExecuted();
			break;
		case DOWN_HEAVY:
			ChangeHold(fighterEventHolds.attackHeavyDownGround);
			GroundAttackExecuted();
			state = CROUCHING;
			break;
		case FORWARD_HEAVY:
			ChangeHold(fighterEventHolds.attackHeavyForwardGround);
			GroundAttackExecuted();
			break;
		case QCF_HEAVY:
			ChangeHold(fighterEventHolds.attackHeavyQCFGround);
			GroundAttackExecuted();
			break;
	}

	return 0;
}

bool Fighter::CanDashCancel()
{
	if(!attacking) { return true; }

	if((cancels.dash == ANY_TIME) ||
		(cancels.dash == AFTER_HIT_OR_BLOCK && (hitSomething || wasBlocked)) ||
		(cancels.dash == AFTER_HIT && hitSomething) ||
		(cancels.dash == AFTER_BLOCK && wasBlocked))
	{ return true; }

	return false;
}

bool Fighter::CanJumpCancel()
{
	if(!attacking) { return true; }

	if((cancels.jump == ANY_TIME) ||
		(cancels.jump == AFTER_HIT_OR_BLOCK && (hitSomething || wasBlocked)) ||
		(cancels.jump == AFTER_HIT && hitSomething) ||
		(cancels.jump == AFTER_BLOCK && wasBlocked))
	{ return true; }

	return false;
}

bool Fighter::CanLightNeutralCancel()
{
	if(!attacking) { return true; }

	if((cancels.lightNeutral == ANY_TIME) ||
		(cancels.lightNeutral == AFTER_HIT_OR_BLOCK && (hitSomething || wasBlocked)) ||
		(cancels.lightNeutral == AFTER_HIT && hitSomething) ||
		(cancels.lightNeutral == AFTER_BLOCK && wasBlocked))
	{ return true; }

	return false;
}

bool Fighter::CanLightForwardCancel()
{
	if(!attacking) { return true; }

	if((cancels.lightForward == ANY_TIME) ||
		(cancels.lightForward == AFTER_HIT_OR_BLOCK && (hitSomething || wasBlocked)) ||
		(cancels.lightForward == AFTER_HIT && hitSomething) ||
		(cancels.lightForward == AFTER_BLOCK && wasBlocked))
	{ return true; }

	return false;
}

bool Fighter::CanLightUpCancel()
{
	if(!attacking) { return true; }

	if((cancels.lightUp == ANY_TIME) ||
		(cancels.lightUp == AFTER_HIT_OR_BLOCK && (hitSomething || wasBlocked)) ||
		(cancels.lightUp == AFTER_HIT && hitSomething) ||
		(cancels.lightUp == AFTER_BLOCK && wasBlocked))
	{ return true; }

	return false;
}

bool Fighter::CanLightDownCancel()
{
	if(!attacking) { return true; }

	if((cancels.lightDown == ANY_TIME) ||
		(cancels.lightDown == AFTER_HIT_OR_BLOCK && (hitSomething || wasBlocked)) ||
		(cancels.lightDown == AFTER_HIT && hitSomething) ||
		(cancels.lightDown == AFTER_BLOCK && wasBlocked))
	{ return true; }

	return false;
}

bool Fighter::CanLightBackwardCancel()
{
	if(!attacking) { return true; }

	if((cancels.lightBackward == ANY_TIME) ||
		(cancels.lightBackward == AFTER_HIT_OR_BLOCK && (hitSomething || wasBlocked)) ||
		(cancels.lightBackward == AFTER_HIT && hitSomething) ||
		(cancels.lightBackward == AFTER_BLOCK && wasBlocked))
	{ return true; }

	return false;
}

bool Fighter::CanLightQCFCancel()
{
	if(!attacking) { return true; }

	if((cancels.lightQCF == ANY_TIME) ||
		(cancels.lightQCF == AFTER_HIT_OR_BLOCK && (hitSomething || wasBlocked)) ||
		(cancels.lightQCF == AFTER_HIT && hitSomething) ||
		(cancels.lightQCF == AFTER_BLOCK && wasBlocked))
	{ return true; }

	return false;
}

bool Fighter::CanHeavyNeutralCancel()
{
	if(!attacking) { return true; }

	if((cancels.heavyNeutral == ANY_TIME) ||
		(cancels.heavyNeutral == AFTER_HIT_OR_BLOCK && (hitSomething || wasBlocked)) ||
		(cancels.heavyNeutral == AFTER_HIT && hitSomething) ||
		(cancels.heavyNeutral == AFTER_BLOCK && wasBlocked))
	{ return true; }

	return false;
}

bool Fighter::CanHeavyForwardCancel()
{
	if(!attacking) { return true; }

	if((cancels.heavyForward == ANY_TIME) ||
		(cancels.heavyForward == AFTER_HIT_OR_BLOCK && (hitSomething || wasBlocked)) ||
		(cancels.heavyForward == AFTER_HIT && hitSomething) ||
		(cancels.heavyForward == AFTER_BLOCK && wasBlocked))
	{ return true; }

	return false;
}

bool Fighter::CanHeavyUpCancel()
{
	if(!attacking) { return true; }

	if((cancels.heavyUp == ANY_TIME) ||
		(cancels.heavyUp == AFTER_HIT_OR_BLOCK && (hitSomething || wasBlocked)) ||
		(cancels.heavyUp == AFTER_HIT && hitSomething) ||
		(cancels.heavyUp == AFTER_BLOCK && wasBlocked))
	{ return true; }

	return false;
}

bool Fighter::CanHeavyDownCancel()
{
	if(!attacking) { return true; }

	if((cancels.heavyDown == ANY_TIME) ||
		(cancels.heavyDown == AFTER_HIT_OR_BLOCK && (hitSomething || wasBlocked)) ||
		(cancels.heavyDown == AFTER_HIT && hitSomething) ||
		(cancels.heavyDown == AFTER_BLOCK && wasBlocked))
	{ return true; }

	return false;
}

bool Fighter::CanHeavyBackwardCancel()
{
	if(!attacking) { return true; }

	if((cancels.heavyBackward == ANY_TIME) ||
		(cancels.heavyBackward == AFTER_HIT_OR_BLOCK && (hitSomething || wasBlocked)) ||
		(cancels.heavyBackward == AFTER_HIT && hitSomething) ||
		(cancels.heavyBackward == AFTER_BLOCK && wasBlocked))
	{ return true; }

	return false;
}

bool Fighter::CanHeavyQCFCancel()
{
	if(!attacking) { return true; }

	if((cancels.heavyQCF == ANY_TIME) ||
		(cancels.heavyQCF == AFTER_HIT_OR_BLOCK && (hitSomething || wasBlocked)) ||
		(cancels.heavyQCF == AFTER_HIT && hitSomething) ||
		(cancels.heavyQCF == AFTER_BLOCK && wasBlocked))
	{ return true; }

	return false;
}

int Fighter::CollideTerrain(list<HSObject*> * gameObjects)
{
	//check for collisions with terrain, if this object has any terrain boxes
	if(firstTerrainBox != NULL)
	{
		//handle a change in terrain box
		HandleChangeInTerrainBoxes(gameObjects);

		//check for collisions, if this object moved
		if(pos.x != prevPos.x || pos.y != prevPos.y)
		{
			TerrainCollisionResult result = IsCollidingWithAnyTerrain(gameObjects);
			if(result.lastSelfBox != NULL) { HandleTerrainCollision(gameObjects, &result); }
		}

		//check directly below this object if it is grounded. If there is no terrain there, either step down or start falling
		if(state != JUMPING && state != AIRBORN && state != AIR_DASHING && state != KNOCKOUT_AIR)
		{
			StepCheck(gameObjects);
		}
		else
		{
			//set this to false since this object is now airborn, and guaranteed to have fallen inside of any jump-through terrain
			//that it was previously resting on
			ignoreJumpThroughTerrain = false;
		}
	}
	
	return 0;
}

TerrainCollisionResult Fighter::IsCollidingWithAnyFighter(list<HSObject*> * gameObjects)
{
	list<HSObject*>::iterator objIt;
	for ( objIt=gameObjects->begin(); objIt != gameObjects->end(); objIt++)
	{
		if((*objIt)->id != id && (*objIt)->IsFighter() && ((TerrainObject*)(*objIt))->firstTerrainBox != NULL)
		{
			TerrainCollisionResult result = IsCollidingWithObjectTerrain((TerrainObject*)(*objIt), NULL, NULL, NULL, false);
			if(result.lastSelfBox != NULL)
			{
				HandleFighterCollison(result.lastTargetObject);
			}
		}
	}

	//no collisions
	TerrainCollisionResult result;
	result.lastSelfBox = NULL;
	result.lastTargetBox = NULL;
	result.lastTargetObject = NULL;
	result.horizontalImpact = false;
	result.verticalImpact = false;
	return result;
}

int Fighter::CollideFighters(list<HSObject*> * gameObjects)
{
	if(firstTerrainBox != NULL)
	{
		TerrainCollisionResult result = IsCollidingWithAnyFighter(gameObjects);
	}

	return 0;
}

void Fighter::ApplyAttackResults()
{
	if(!attackResults.struck)
	{
		if(attackResults.didStrike)
		{
			curHitstop = attackResults.hitstop;
		}

		ResetAttackResults();
		return;
	}

	if(attackResults.hFlip) { facing = LEFT; hFlip = true; }
	else { facing = RIGHT; hFlip = false; }

	if(health > 0)
	{
		curHealth -= attackResults.damage;
	
		if(curHealth <= 0)
		{
			curHealth = 0;

			if(state == JUMPING || state == AIR_DASHING)
			{
				ChangeHold(fighterEventHolds.hitstunLightAir);
				state = KNOCKOUT_AIR;
			}
			else if(state == STANDING || state == CROUCHING)
			{
				ChangeHold(fighterEventHolds.knockout);
				state = KNOCKOUT;
			}

			blocking = false;
			jump = NO_JUMP;
			airDash = NO_AIR_DASH;
			jumpStartup = false;
			shortHop = false;
			runStopping = false;
			turning = false;
			attacking = false;
			walkAfterTurning = false;
			runAfterTurning = false;
			airControl = NO_AIR_CONTROL;
			landingAction = NO_ACTION;
			turnUponLanding = false;
			hitSomething = false;
			wasBlocked = false;
			curHitstun = 0;
			curBlockstun = 0;
			
			vel.x = attackResults.force.x * -facing;
			vel.y = attackResults.force.y;

			if(vel.y < 0)
			{
				ChangeHold(fighterEventHolds.hitstunLightAir);
				state = KNOCKOUT_AIR;
				falls = true;
			}

			if(state == KNOCKOUT && vel.x != 0)
			{
				sliding = true;
			}
			else
			{
				sliding = false;
			}
			
			ResetAttackResults();

			return;
		}
		else if(curHealth > health) { curHealth = health; }
	}

	hitstunBlockability = attackResults.blockability;
	curHitstop = attackResults.hitstop;
	curHitstun = attackResults.hitstun;
	curBlockstun = attackResults.blockstun;
	vel.x = attackResults.force.x * -facing;
	vel.y = attackResults.force.y;

	if(vel.y < 0)
	{
		state = JUMPING;
		falls = true;
	}

	if((state == STANDING || state == CROUCHING) && vel.x != 0)
	{
		sliding = true;
	}
	else
	{
		sliding = false;
	}

	if(curHitstun > 0)
	{
		if(state == STANDING || state == WALKING || state == RUNNING)
		{
			if(hitstunBlockability == MID || hitstunBlockability == UNBLOCKABLE)
			{
				ChangeHold(fighterEventHolds.hitstunLightMidStanding);
			}
			else if(hitstunBlockability == HIGH)
			{
				ChangeHold(fighterEventHolds.hitstunLightHighStanding);
			}
			else if(hitstunBlockability == LOW)
			{
				ChangeHold(fighterEventHolds.hitstunLightLowStanding);
			}
			state = STANDING;
		}
		else if(state == CROUCHING)
		{
			if(hitstunBlockability == MID || hitstunBlockability == UNBLOCKABLE || hitstunBlockability == HIGH)
			{
				ChangeHold(fighterEventHolds.hitstunLightMidCrouching);
			}
			else
			{
				ChangeHold(fighterEventHolds.hitstunLightLowCrouching);
			}
		}
		else if(state == JUMPING)
		{
			ChangeHold(fighterEventHolds.hitstunLightAir);
		}

		blocking = false;
	}
	else if(curBlockstun > 0)
	{
		if(state == STANDING || state == WALKING || state == RUNNING)
		{
			ChangeHold(fighterEventHolds.blockHigh);
			state = STANDING;
		}
		else if(state == CROUCHING)
		{
			ChangeHold(fighterEventHolds.blockLow);
		}
		else if(state == JUMPING)
		{
			ChangeHold(fighterEventHolds.blockAir);
		}

		blocking = true;
	}

	ResetAttackResults();
}

void Fighter::HandleHurtCollision(TerrainObject * attacker)
{
	attackResults.struck = true;
	attackResults.blockability = attacker->blockability;

	FighterFacing reqBlockDirection = LEFT;
	attackResults.hFlip = true;
	if(pos.x < attacker->pos.x)
	{
		reqBlockDirection = RIGHT;
		attackResults.hFlip = false;
	}

	if(facing == reqBlockDirection && attacker->blockability != UNBLOCKABLE &&
		((attacker->blockability == MID && blocking) ||
		(state == JUMPING && blocking) ||
		(attacker->blockability == HIGH && state == STANDING && blocking) ||
		(attacker->blockability == LOW && state == CROUCHING && blocking)))
	{
		attackResults.hitstop = attacker->victimHitstop;
		attackResults.blockstun = attacker->blockstun;
		attackResults.force.x = attacker->force.x;
		attackResults.force.y = 0;
		attacker->numBlockedByThisFrame++;
		
		if(attacker->IsFighter())
		{
			((Fighter*)attacker)->wasBlocked = true;
		}

		return;
	}

	attackResults.damage += attacker->damage;
	attackResults.hitstop = attacker->victimHitstop;
	attackResults.hitstun = attacker->hitstun;
	attackResults.force.x = attacker->force.x;
	attackResults.force.y = attacker->force.y;
	attacker->numHitThisFrame++;
	
	if(attacker->IsFighter())
	{
		((Fighter*)attacker)->hitSomething = true;
	}
}

list<SpawnObject*> Fighter::GetSpawnObjects()
{
	return HSObject::GetSpawnObjects();
}

list<AudioInstance*> Fighter::GetAudio()
{
	return TerrainObject::GetAudio();
}

bool Fighter::IsFighter()
{
	return true;
}

FighterHold * Fighter::CreateNewHold()
{
	FighterHold * newHold = new FighterHold();

	return newHold;
}

Fighter * Fighter::CreateObjectOfSameType()
{
	return new Fighter();
}

void Fighter::CopyAttributes(HSObject * target)
{
	PhysicsObject::CopyAttributes(target);

	if(!target->IsFighter()) { return; }

	Fighter * f = (Fighter*)target;

	HSBox * curBox = f->firstUprightTerrainBox;
	while(curBox != NULL)
	{
		HSBox * newBox = new HSBox();
		newBox->bottomAlign = curBox->bottomAlign;
		newBox->height = curBox->height;
		newBox->isTriangle = curBox->isTriangle;
		newBox->offset.x = curBox->offset.x;
		newBox->offset.y = curBox->offset.y;
		newBox->rightAlign = curBox->rightAlign;
		newBox->width = curBox->width;

		f->AddUprightTerrainBox(newBox);

		curBox = curBox->nextBox;
	}

	curBox = f->firstCrouchingTerrainBox;
	while(curBox != NULL)
	{
		HSBox * newBox = new HSBox();
		newBox->bottomAlign = curBox->bottomAlign;
		newBox->height = curBox->height;
		newBox->isTriangle = curBox->isTriangle;
		newBox->offset.x = curBox->offset.x;
		newBox->offset.y = curBox->offset.y;
		newBox->rightAlign = curBox->rightAlign;
		newBox->width = curBox->width;

		f->AddCrouchingTerrainBox(newBox);

		curBox = curBox->nextBox;
	}

	curBox = f->firstProneTerrainBox;
	while(curBox != NULL)
	{
		HSBox * newBox = new HSBox();
		newBox->bottomAlign = curBox->bottomAlign;
		newBox->height = curBox->height;
		newBox->isTriangle = curBox->isTriangle;
		newBox->offset.x = curBox->offset.x;
		newBox->offset.y = curBox->offset.y;
		newBox->rightAlign = curBox->rightAlign;
		newBox->width = curBox->width;

		f->AddProneTerrainBox(newBox);

		curBox = curBox->nextBox;
	}

	curBox = f->firstCompactTerrainBox;
	while(curBox != NULL)
	{
		HSBox * newBox = new HSBox();
		newBox->bottomAlign = curBox->bottomAlign;
		newBox->height = curBox->height;
		newBox->isTriangle = curBox->isTriangle;
		newBox->offset.x = curBox->offset.x;
		newBox->offset.y = curBox->offset.y;
		newBox->rightAlign = curBox->rightAlign;
		newBox->width = curBox->width;

		f->AddCompactTerrainBox(newBox);

		curBox = curBox->nextBox;
	}
}

void Fighter::CopyEventHold(HSObject * target, HSObjectHold * targetHold)
{
	PhysicsObject::CopyEventHold(target, targetHold);

	if(!target->IsFighter() || !targetHold->IsFighterHold()) { return; }

	Fighter * f = (Fighter*)target;
	FighterHold * fh = (FighterHold*)targetHold;
	
	if(fighterEventHolds.standing != NULL && fh->id == fighterEventHolds.standing->id) { f->fighterEventHolds.standing = fh; }
	if(fighterEventHolds.turn != NULL && fh->id == fighterEventHolds.turn->id) { f->fighterEventHolds.turn = fh; }
	if(fighterEventHolds.walk != NULL && fh->id == fighterEventHolds.walk->id) { f->fighterEventHolds.walk = fh; }
	if(fighterEventHolds.walking != NULL && fh->id == fighterEventHolds.walking->id) { f->fighterEventHolds.walking = fh; }
	if(fighterEventHolds.walkingTurn != NULL && fh->id == fighterEventHolds.walkingTurn->id) { f->fighterEventHolds.walkingTurn = fh; }
	if(fighterEventHolds.run != NULL && fh->id == fighterEventHolds.run->id) { f->fighterEventHolds.run = fh; }
	if(fighterEventHolds.running != NULL && fh->id == fighterEventHolds.running->id) { f->fighterEventHolds.running = fh; }
	if(fighterEventHolds.runningStop != NULL && fh->id == fighterEventHolds.runningStop->id) { f->fighterEventHolds.runningStop = fh; }
	if(fighterEventHolds.runningTurn != NULL && fh->id == fighterEventHolds.runningTurn->id) { f->fighterEventHolds.runningTurn = fh; }
	if(fighterEventHolds.crouch != NULL && fh->id == fighterEventHolds.crouch->id) { f->fighterEventHolds.crouch = fh; }
	if(fighterEventHolds.crouching != NULL && fh->id == fighterEventHolds.crouching->id) { f->fighterEventHolds.crouching = fh; }
	if(fighterEventHolds.crouchingTurn != NULL && fh->id == fighterEventHolds.crouchingTurn->id) { f->fighterEventHolds.crouchingTurn = fh; }
	if(fighterEventHolds.stand != NULL && fh->id == fighterEventHolds.stand->id) { f->fighterEventHolds.stand = fh; }
	
	if(fighterEventHolds.jumpNeutralStart != NULL && fh->id == fighterEventHolds.jumpNeutralStart->id) { f->fighterEventHolds.jumpNeutralStart = fh; }
	if(fighterEventHolds.jumpNeutralStartAir != NULL && fh->id == fighterEventHolds.jumpNeutralStartAir->id) { f->fighterEventHolds.jumpNeutralStartAir = fh; }
	if(fighterEventHolds.jumpNeutralRising != NULL && fh->id == fighterEventHolds.jumpNeutralRising->id) { f->fighterEventHolds.jumpNeutralRising = fh; }
	if(fighterEventHolds.jumpNeutralFall != NULL && fh->id == fighterEventHolds.jumpNeutralFall->id) { f->fighterEventHolds.jumpNeutralFall = fh; }
	if(fighterEventHolds.jumpNeutralFalling != NULL && fh->id == fighterEventHolds.jumpNeutralFalling->id) { f->fighterEventHolds.jumpNeutralFalling = fh; }
	if(fighterEventHolds.jumpNeutralLand != NULL && fh->id == fighterEventHolds.jumpNeutralLand->id) { f->fighterEventHolds.jumpNeutralLand = fh; }
	if(fighterEventHolds.jumpBackwardRising != NULL && fh->id == fighterEventHolds.jumpBackwardRising->id) { f->fighterEventHolds.jumpBackwardRising = fh; }
	if(fighterEventHolds.jumpBackwardFall != NULL && fh->id == fighterEventHolds.jumpBackwardFall->id) { f->fighterEventHolds.jumpBackwardFall = fh; }
	
	if(fighterEventHolds.airDashForward != NULL && fh->id == fighterEventHolds.airDashForward->id) { f->fighterEventHolds.airDashForward = fh; }
	if(fighterEventHolds.airDashBackward != NULL && fh->id == fighterEventHolds.airDashBackward->id) { f->fighterEventHolds.airDashBackward = fh; }
	
	if(fighterEventHolds.blockHigh != NULL && fh->id == fighterEventHolds.blockHigh->id) { f->fighterEventHolds.blockHigh = fh; }
	if(fighterEventHolds.blockLow != NULL && fh->id == fighterEventHolds.blockLow->id) { f->fighterEventHolds.blockLow = fh; }
	if(fighterEventHolds.blockAir != NULL && fh->id == fighterEventHolds.blockAir->id) { f->fighterEventHolds.blockAir = fh; }
	
	if(fighterEventHolds.hitstunLightHighStanding != NULL && fh->id == fighterEventHolds.hitstunLightHighStanding->id) { f->fighterEventHolds.hitstunLightHighStanding = fh; }
	if(fighterEventHolds.hitstunLightMidStanding != NULL && fh->id == fighterEventHolds.hitstunLightMidStanding->id) { f->fighterEventHolds.hitstunLightMidStanding = fh; }
	if(fighterEventHolds.hitstunLightLowStanding != NULL && fh->id == fighterEventHolds.hitstunLightLowStanding->id) { f->fighterEventHolds.hitstunLightLowStanding = fh; }
	if(fighterEventHolds.hitstunLightMidCrouching != NULL && fh->id == fighterEventHolds.hitstunLightMidCrouching->id) { f->fighterEventHolds.hitstunLightMidCrouching = fh; }
	if(fighterEventHolds.hitstunLightLowCrouching != NULL && fh->id == fighterEventHolds.hitstunLightLowCrouching->id) { f->fighterEventHolds.hitstunLightLowCrouching = fh; }
	if(fighterEventHolds.hitstunLightAir != NULL && fh->id == fighterEventHolds.hitstunLightAir->id) { f->fighterEventHolds.hitstunLightAir = fh; }
	
	if(fighterEventHolds.attackLightNeutralGround != NULL && fh->id == fighterEventHolds.attackLightNeutralGround->id) { f->fighterEventHolds.attackLightNeutralGround = fh; }
	if(fighterEventHolds.attackLightDownGround != NULL && fh->id == fighterEventHolds.attackLightDownGround->id) { f->fighterEventHolds.attackLightDownGround = fh; }
	if(fighterEventHolds.attackLightUpGround != NULL && fh->id == fighterEventHolds.attackLightUpGround->id) { f->fighterEventHolds.attackLightUpGround = fh; }
	if(fighterEventHolds.attackLightForwardGround != NULL && fh->id == fighterEventHolds.attackLightForwardGround->id) { f->fighterEventHolds.attackLightForwardGround = fh; }
	if(fighterEventHolds.attackLightQCFGround != NULL && fh->id == fighterEventHolds.attackLightQCFGround->id) { f->fighterEventHolds.attackLightQCFGround = fh; }
	if(fighterEventHolds.attackLightNeutralAir != NULL && fh->id == fighterEventHolds.attackLightNeutralAir->id) { f->fighterEventHolds.attackLightNeutralAir = fh; }
	if(fighterEventHolds.attackLightDownAir != NULL && fh->id == fighterEventHolds.attackLightDownAir->id) { f->fighterEventHolds.attackLightDownAir = fh; }
	if(fighterEventHolds.attackLightUpAir != NULL && fh->id == fighterEventHolds.attackLightUpAir->id) { f->fighterEventHolds.attackLightUpAir = fh; }
	if(fighterEventHolds.attackLightForwardAir != NULL && fh->id == fighterEventHolds.attackLightForwardAir->id) { f->fighterEventHolds.attackLightForwardAir = fh; }
	if(fighterEventHolds.attackLightBackwardAir != NULL && fh->id == fighterEventHolds.attackLightBackwardAir->id) { f->fighterEventHolds.attackLightBackwardAir = fh; }
	if(fighterEventHolds.attackLightQCFAir != NULL && fh->id == fighterEventHolds.attackLightQCFAir->id) { f->fighterEventHolds.attackLightQCFAir = fh; }
	
	if(fighterEventHolds.attackHeavyNeutralGround != NULL && fh->id == fighterEventHolds.attackHeavyNeutralGround->id) { f->fighterEventHolds.attackHeavyNeutralGround = fh; }
	if(fighterEventHolds.attackHeavyDownGround != NULL && fh->id == fighterEventHolds.attackHeavyDownGround->id) { f->fighterEventHolds.attackHeavyDownGround = fh; }
	if(fighterEventHolds.attackHeavyUpGround != NULL && fh->id == fighterEventHolds.attackHeavyUpGround->id) { f->fighterEventHolds.attackHeavyUpGround = fh; }
	if(fighterEventHolds.attackHeavyForwardGround != NULL && fh->id == fighterEventHolds.attackHeavyForwardGround->id) { f->fighterEventHolds.attackHeavyForwardGround = fh; }
	if(fighterEventHolds.attackHeavyQCFGround != NULL && fh->id == fighterEventHolds.attackHeavyQCFGround->id) { f->fighterEventHolds.attackHeavyQCFGround = fh; }
	if(fighterEventHolds.attackHeavyNeutralAir != NULL && fh->id == fighterEventHolds.attackHeavyNeutralAir->id) { f->fighterEventHolds.attackHeavyNeutralAir = fh; }
	if(fighterEventHolds.attackHeavyDownAir != NULL && fh->id == fighterEventHolds.attackHeavyDownAir->id) { f->fighterEventHolds.attackHeavyDownAir = fh; }
	if(fighterEventHolds.attackHeavyUpAir != NULL && fh->id == fighterEventHolds.attackHeavyUpAir->id) { f->fighterEventHolds.attackHeavyUpAir = fh; }
	if(fighterEventHolds.attackHeavyForwardAir != NULL && fh->id == fighterEventHolds.attackHeavyForwardAir->id) { f->fighterEventHolds.attackHeavyForwardAir = fh; }
	if(fighterEventHolds.attackHeavyBackwardAir != NULL && fh->id == fighterEventHolds.attackHeavyBackwardAir->id) { f->fighterEventHolds.attackHeavyBackwardAir = fh; }
	if(fighterEventHolds.attackHeavyQCFAir != NULL && fh->id == fighterEventHolds.attackHeavyQCFAir->id) { f->fighterEventHolds.attackHeavyQCFAir = fh; }
}

int Fighter::SaveEventHolds(HSObjectHold * hold, XMLElement * eventHolds)
{
	if(int error = TerrainObject::SaveEventHolds(hold, eventHolds) != 0)
	{
		return error;
	}

	//check the attribute names, and put the hold pointers into the proper event hold slots.
	unsigned int eventHoldId;
	eventHolds->QueryUnsignedAttribute("standing", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.standing = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("turn", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.turn = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("walk", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.walk = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("walking", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.walking = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("walkingTurn", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.walkingTurn = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("run", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.run = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("running", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.running = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("runningTurn", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.runningTurn = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("runningStop", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.runningStop = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("crouch", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.crouch = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("crouching", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.crouching = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("crouchingTurn", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.crouchingTurn = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("stand", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.stand = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("jumpNeutralStart", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.jumpNeutralStart = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("jumpNeutralStartAir", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.jumpNeutralStartAir = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("jumpNeutralRising", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.jumpNeutralRising = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("jumpNeutralFall", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.jumpNeutralFall = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("jumpNeutralFalling", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.jumpNeutralFalling = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("jumpNeutralLand", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.jumpNeutralLand = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("jumpBackwardRising", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.jumpBackwardRising = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("jumpBackwardFall", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.jumpBackwardFall = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("airDashForward", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.airDashForward = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("airDashBackward", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.airDashBackward = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("blockHigh", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.blockHigh = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("blockLow", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.blockLow = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("blockAir", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.blockAir = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("hitstunLightHighStanding", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.hitstunLightHighStanding = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("hitstunLightMidStanding", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.hitstunLightMidStanding = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("hitstunLightLowStanding", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.hitstunLightLowStanding = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("hitstunLightMidCrouching", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.hitstunLightMidCrouching = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("hitstunLightLowCrouching", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.hitstunLightLowCrouching = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("hitstunLightAir", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.hitstunLightAir = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("attackLightNeutralGround", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.attackLightNeutralGround = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("attackLightDownGround", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.attackLightDownGround = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("attackLightUpGround", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.attackLightUpGround = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("attackLightForwardGround", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.attackLightForwardGround = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("attackLightQCFGround", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.attackLightQCFGround = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("attackLightNeutralAir", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.attackLightNeutralAir = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("attackLightDownAir", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.attackLightDownAir = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("attackLightUpAir", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.attackLightUpAir = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("attackLightForwardAir", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.attackLightForwardAir = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("attackLightBackwardAir", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.attackLightBackwardAir = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("attackLightQCFAir", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.attackLightQCFAir = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("attackHeavyNeutralGround", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.attackHeavyNeutralGround = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("attackHeavyDownGround", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.attackHeavyDownGround = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("attackHeavyUpGround", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.attackHeavyUpGround = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("attackHeavyForwardGround", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.attackHeavyForwardGround = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("attackHeavyQCFGround", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.attackHeavyQCFGround = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("attackHeavyNeutralAir", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.attackHeavyNeutralAir = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("attackHeavyDownAir", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.attackHeavyDownAir = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("attackHeavyUpAir", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.attackHeavyUpAir = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("attackHeavyForwardAir", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.attackHeavyForwardAir = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("attackHeavyBackwardAir", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.attackHeavyBackwardAir = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("attackHeavyQCFAir", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.attackHeavyQCFAir = (FighterHold*)hold; }
	
	eventHoldId = 0;
	eventHolds->QueryUnsignedAttribute("knockout", &eventHoldId);
	if(hold->id == eventHoldId) { fighterEventHolds.knockout = (FighterHold*)hold; }

	return 0;
}

int Fighter::DefineUprightTerrainBox(HSBox * newUprightTerrainBox, XMLElement * definition)
{
	if(int error = DefineBox(definition, newUprightTerrainBox) != 0)
	{
		return error; //there was an error defining the box
	}

	AddUprightTerrainBox(newUprightTerrainBox);

	return 0;
}

void Fighter::AddUprightTerrainBox(HSBox * newUprightTerrainBox)
{
	if(firstUprightTerrainBox == NULL)
	{
		//just make the new terrain box the first terrain box
		firstUprightTerrainBox = newUprightTerrainBox;
		lastUprightTerrainBox = newUprightTerrainBox;
	}
	else
	{
		//put the new terrain box at the end of the list
		lastUprightTerrainBox->nextBox = newUprightTerrainBox;
		lastUprightTerrainBox = newUprightTerrainBox;
	}
}

int Fighter::DefineCrouchingTerrainBox(HSBox * newCrouchingTerrainBox, XMLElement * definition)
{
	if(int error = DefineBox(definition, newCrouchingTerrainBox) != 0)
	{
		return error; //there was an error defining the box
	}

	AddCrouchingTerrainBox(newCrouchingTerrainBox);

	return 0;
}

void Fighter::AddCrouchingTerrainBox(HSBox * newCrouchingTerrainBox)
{
	if(firstCrouchingTerrainBox == NULL)
	{
		//just make the new terrain box the first terrain box
		firstCrouchingTerrainBox = newCrouchingTerrainBox;
		lastCrouchingTerrainBox = newCrouchingTerrainBox;
	}
	else
	{
		//put the new terrain box at the end of the list
		lastCrouchingTerrainBox->nextBox = newCrouchingTerrainBox;
		lastCrouchingTerrainBox = newCrouchingTerrainBox;
	}
}

int Fighter::DefineProneTerrainBox(HSBox * newProneTerrainBox, XMLElement * definition)
{
	if(int error = DefineBox(definition, newProneTerrainBox) != 0)
	{
		return error; //there was an error defining the box
	}

	AddProneTerrainBox(newProneTerrainBox);

	return 0;
}

void Fighter::AddProneTerrainBox(HSBox * newProneTerrainBox)
{
	if(firstProneTerrainBox == NULL)
	{
		//just make the new terrain box the first terrain box
		firstProneTerrainBox = newProneTerrainBox;
		lastProneTerrainBox = newProneTerrainBox;
	}
	else
	{
		//put the new terrain box at the end of the list
		lastProneTerrainBox->nextBox = newProneTerrainBox;
		lastProneTerrainBox = newProneTerrainBox;
	}
}

int Fighter::DefineCompactTerrainBox(HSBox * newCompactTerrainBox, XMLElement * definition)
{
	if(int error = DefineBox(definition, newCompactTerrainBox) != 0)
	{
		return error; //there was an error defining the box
	}

	AddCompactTerrainBox(newCompactTerrainBox);

	return 0;
}

void Fighter::AddCompactTerrainBox(HSBox * newCompactTerrainBox)
{
	if(firstCompactTerrainBox == NULL)
	{
		//just make the new terrain box the first terrain box
		firstCompactTerrainBox = newCompactTerrainBox;
		lastCompactTerrainBox = newCompactTerrainBox;
	}
	else
	{
		//put the new terrain box at the end of the list
		lastCompactTerrainBox->nextBox = newCompactTerrainBox;
		lastCompactTerrainBox = newCompactTerrainBox;
	}
}

bool Fighter::ChangeHold(HSObjectHold* hold)
{
	if(TerrainObject::ChangeHold(hold))
	{
		FighterHold * fHold = (FighterHold*)curHold;

		if(fHold->changeCancels)
		{
			cancels = fHold->cancels;
		}

		return true;
	}

	return false;
}

HSObjectHold * Fighter::GetDefaultHold()
{
	turning = false;
	runStopping = false;
	jumpStartupJustEnded = jumpStartup;
	jumpStartup = false;
	attacking = false;
	victims.clear();
	hitAudioList.clear();
	blockability = MID;
	horizontalDirectionBasedBlock = false;
	reversedHorizontalBlock = false;
	damage = 0;
	hitstun = 0;
	blockstun = 0;
	force.x = 0;
	force.y = 0;
	trips = false;
	cancels = defaultCancels;
	hitSomething = false;
	wasBlocked = false;

	if(state == KNOCKOUT_AIR)
	{
		return fighterEventHolds.hitstunLightAir;
	}
	else if(curHitstun > 0)
	{
		if(state == STANDING || state == WALKING || state == RUNNING)
		{
			if(hitstunBlockability == MID || hitstunBlockability == UNBLOCKABLE)
			{
				return fighterEventHolds.hitstunLightMidStanding;
			}
			else if(hitstunBlockability == HIGH)
			{
				return fighterEventHolds.hitstunLightHighStanding;
			}
			else if(hitstunBlockability == LOW)
			{
				return fighterEventHolds.hitstunLightLowStanding;
			}
			state = STANDING;
		}
		else if(state == CROUCHING)
		{
			if(hitstunBlockability == MID || hitstunBlockability == UNBLOCKABLE || hitstunBlockability == HIGH)
			{
				return fighterEventHolds.hitstunLightMidCrouching;
			}
			else
			{
				return fighterEventHolds.hitstunLightLowCrouching;
			}
		}
		else if(state == JUMPING)
		{
			return fighterEventHolds.hitstunLightAir;
		}
	}
	else if(curBlockstun > 0 || blocking)
	{
		if(state == STANDING || state == WALKING || state == RUNNING)
		{
			state = STANDING;
			return fighterEventHolds.blockHigh;
		}
		else if(state == CROUCHING)
		{
			return fighterEventHolds.blockLow;
		}
		else if(state == JUMPING || state == AIR_DASHING)
		{
			return fighterEventHolds.blockAir;
		}
	}

	if(state == STANDING)
	{
		return fighterEventHolds.standing;
	}
	else if(state == WALKING)
	{
		return fighterEventHolds.walking;
	}
	else if(state == RUNNING)
	{
		return fighterEventHolds.running;
	}
	else if(state == CROUCHING)
	{
		return fighterEventHolds.crouching;
	}
	else if(state == JUMPING)
	{
		if(vel.y < 0)
		{
			if((facing == LEFT && vel.x > 0) || (facing == RIGHT && vel.x < 0))
			{
				return fighterEventHolds.jumpBackwardRising;
			}
			else
			{
				return fighterEventHolds.jumpNeutralRising;
			}
		}
		else
		{
			return fighterEventHolds.jumpNeutralFalling;
		}
	}
	else if(state == AIR_DASHING)
	{
		if(airDash == FORWARD_AIR_DASH)
		{
			return fighterEventHolds.airDashForward;
		}
		else if(airDash == BACKWARD_AIR_DASH)
		{
			return fighterEventHolds.airDashBackward;
		}
	}

	//default to whatever the base class does
	return TerrainObject::GetDefaultHold();
}

void Fighter::GroundAttackExecuted()
{
	walkAfterTurning = false;
	runAfterTurning = false;
	turning = false;
	runStopping = false;
	jump = NO_JUMP;
	blocking = false;
	attacking = true;
	victims.clear();
	hitSomething = false;
	wasBlocked = false;
	if(state == RUNNING)
	{
		sliding = true;
	}
	state = STANDING;
}

void Fighter::AirAttackExecuted()
{
	blocking = false;
	attacking = true;
	victims.clear();
	hitSomething = false;
	wasBlocked = false;
}

bool Fighter::ForwardWasTapped(InputStates * inputHistory, int untilFrame, int sinceFrame)
{
	if(inputHistory == NULL)
	{
		return false;
	}

	InputStates * curHistory = inputHistory;

	//trace back to the point in history just before the until frame
	while(curHistory->frame > untilFrame)
	{
		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}

	int releasedFrame;

	//find the latest point in history that forward was released
	while(curHistory->frame >= sinceFrame)
	{
		if((facing == LEFT && (curHistory->bKeyLeft.released || curHistory->bButtonLeft.released || curHistory->bHatLeft.released || curHistory->bStickLeft.released)) ||
		(facing == RIGHT && (curHistory->bKeyRight.released || curHistory->bButtonRight.released || curHistory->bHatRight.released || curHistory->bStickRight.released)))
		{
			releasedFrame = curHistory->frame;
			break;
		}

		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}

	if(curHistory->frame < sinceFrame) { return false; }

	//find the latest point in history that forward was pressed
	while(curHistory->frame >= releasedFrame - TAP_THRESHOLD)
	{
		if((facing == LEFT && (curHistory->bKeyLeft.pressed || curHistory->bButtonLeft.pressed || curHistory->bHatLeft.pressed || curHistory->bStickLeft.pressed)) ||
		(facing == RIGHT && (curHistory->bKeyRight.pressed || curHistory->bButtonRight.pressed || curHistory->bHatRight.pressed || curHistory->bStickRight.pressed)))
		{
			return true;
		}

		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}

	return false;
}

bool Fighter::BackwardWasTapped(InputStates * inputHistory, int untilFrame, int sinceFrame)
{
	if(inputHistory == NULL)
	{
		return false;
	}

	InputStates * curHistory = inputHistory;

	//trace back to the point in history just before the until frame
	while(curHistory->frame > untilFrame)
	{
		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}

	int releasedFrame;

	//find the latest point in history that backward was released
	while(curHistory->frame >= sinceFrame)
	{
		if((facing == RIGHT && (curHistory->bKeyLeft.released || curHistory->bButtonLeft.released || curHistory->bHatLeft.released || curHistory->bStickLeft.released)) ||
		(facing == LEFT && (curHistory->bKeyRight.released || curHistory->bButtonRight.released || curHistory->bHatRight.released || curHistory->bStickRight.released)))
		{
			releasedFrame = curHistory->frame;
			break;
		}

		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}

	if(curHistory->frame < sinceFrame) { return false; }

	//find the latest point in history that backward was pressed
	while(curHistory->frame >= releasedFrame - TAP_THRESHOLD)
	{
		if((facing == RIGHT && (curHistory->bKeyLeft.pressed || curHistory->bButtonLeft.pressed || curHistory->bHatLeft.pressed || curHistory->bStickLeft.pressed)) ||
		(facing == LEFT && (curHistory->bKeyRight.pressed || curHistory->bButtonRight.pressed || curHistory->bHatRight.pressed || curHistory->bStickRight.pressed)))
		{
			return true;
		}

		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}

	return false;
}

bool Fighter::ForwardPressed(InputStates * inputHistory, int untilFrame, int sinceFrame)
{
	if(inputHistory == NULL)
	{
		return false;
	}

	InputStates * curHistory = inputHistory;

	//trace back to the point in history just before the until frame
	while(curHistory->frame > untilFrame)
	{
		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}

	//find the latest point in history that forward was pressed
	while(curHistory->frame >= sinceFrame)
	{
		if((facing == LEFT && (curHistory->bKeyLeft.pressed || curHistory->bButtonLeft.pressed || curHistory->bHatLeft.pressed || curHistory->bStickLeft.pressed)) ||
		(facing == RIGHT && (curHistory->bKeyRight.pressed || curHistory->bButtonRight.pressed || curHistory->bHatRight.pressed || curHistory->bStickRight.pressed)))
		{
			return true;
		}

		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}

	return false;
}

bool Fighter::BackwardPressed(InputStates * inputHistory, int untilFrame, int sinceFrame)
{
	if(inputHistory == NULL)
	{
		return false;
	}

	InputStates * curHistory = inputHistory;

	//trace back to the point in history just before the until frame
	while(curHistory->frame > untilFrame)
	{
		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}

	//find the latest point in history that backward was pressed
	while(curHistory->frame >= sinceFrame)
	{
		if((facing == RIGHT && (curHistory->bKeyLeft.pressed || curHistory->bButtonLeft.pressed || curHistory->bHatLeft.pressed || curHistory->bStickLeft.pressed)) ||
		(facing == LEFT && (curHistory->bKeyRight.pressed || curHistory->bButtonRight.pressed || curHistory->bHatRight.pressed || curHistory->bStickRight.pressed)))
		{
			return true;
		}

		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}

	return false;
}

bool Fighter::DownPressed(InputStates * inputHistory, int untilFrame, int sinceFrame)
{
	if(inputHistory == NULL)
	{
		return false;
	}

	InputStates * curHistory = inputHistory;

	//trace back to the point in history just before the until frame
	while(curHistory->frame > untilFrame)
	{
		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}

	//find the latest point in history that down was pressed
	while(curHistory->frame >= sinceFrame)
	{
		if(curHistory->bKeyDown.pressed || curHistory->bButtonDown.pressed || curHistory->bHatDown.pressed || curHistory->bStickDown.pressed)
		{
			return true;
		}

		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}

	return false;
}

bool Fighter::ForwardHeld(InputStates * inputHistory, int targetFrame)
{
	if(inputHistory == NULL) { return false; }

	InputStates * curHistory = inputHistory;

	//trace back to the point in history just before the target frame
	while(curHistory->frame > targetFrame)
	{
		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}

	//check if that point in history is held
	if((facing == LEFT && (curHistory->bKeyLeft.held || curHistory->bButtonLeft.held || curHistory->bHatLeft.held || curHistory->bStickLeft.held)) ||
		(facing == RIGHT && (curHistory->bKeyRight.held || curHistory->bButtonRight.held || curHistory->bHatRight.held || curHistory->bStickRight.held)))
	{
		return true;
	}

	return false;
}

bool Fighter::BackwardHeld(InputStates * inputHistory, int targetFrame)
{
	if(inputHistory == NULL) { return false; }

	InputStates * curHistory = inputHistory;
	
	//trace back to the history just before the target frame
	while(curHistory->frame > targetFrame)
	{
		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}
	
	//check if that point in history is held
	if((facing == RIGHT && (curHistory->bKeyLeft.held || curHistory->bButtonLeft.held || curHistory->bHatLeft.held || curHistory->bStickLeft.held)) ||
		(facing == LEFT && (curHistory->bKeyRight.held || curHistory->bButtonRight.held || curHistory->bHatRight.held || curHistory->bStickRight.held)))
	{
		return true;
	}

	return false;
}

bool Fighter::DownHeld(InputStates * inputHistory, int targetFrame)
{
	if(inputHistory == NULL) { return false; }

	InputStates * curHistory = inputHistory;
	
	//trace back to the history just before the target frame
	while(curHistory->frame > targetFrame)
	{
		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}
	
	//check if that point in history is held
	if(curHistory->bKeyDown.held || curHistory->bButtonDown.held || curHistory->bHatDown.held || curHistory->bStickDown.held)
	{
		return true;
	}

	return false;
}

bool Fighter::ForwardReleased(InputStates * inputHistory, int untilFrame, int sinceFrame)
{
	if(inputHistory == NULL)
	{
		return false;
	}

	InputStates * curHistory = inputHistory;

	//trace back to the point in history just before the until frame
	while(curHistory->frame > untilFrame)
	{
		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}

	//find the latest point in history that forward was released
	while(curHistory->frame >= sinceFrame)
	{
		if((facing == LEFT && (curHistory->bKeyLeft.released || curHistory->bButtonLeft.released || curHistory->bHatLeft.released || curHistory->bStickLeft.released)) ||
		(facing == RIGHT && (curHistory->bKeyRight.released || curHistory->bButtonRight.released || curHistory->bHatRight.released || curHistory->bStickRight.released)))
		{
			return true;
		}

		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}

	return false;
}

bool Fighter::BackwardReleased(InputStates * inputHistory, int untilFrame, int sinceFrame)
{
	if(inputHistory == NULL)
	{
		return false;
	}

	InputStates * curHistory = inputHistory;

	//trace back to the point in history just before the until frame
	while(curHistory->frame > untilFrame)
	{
		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}

	//find the latest point in history that backward was released
	while(curHistory->frame >= sinceFrame)
	{
		if((facing == RIGHT && (curHistory->bKeyLeft.released || curHistory->bButtonLeft.released || curHistory->bHatLeft.released || curHistory->bStickLeft.released)) ||
		(facing == LEFT && (curHistory->bKeyRight.released || curHistory->bButtonRight.released || curHistory->bHatRight.released || curHistory->bStickRight.released)))
		{
			return true;
		}

		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}

	return false;
}

bool Fighter::DownReleased(InputStates * inputHistory, int untilFrame, int sinceFrame)
{
	if(inputHistory == NULL)
	{
		return false;
	}

	InputStates * curHistory = inputHistory;

	//trace back to the point in history just before the until frame
	while(curHistory->frame > untilFrame)
	{
		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}

	//find the latest point in history that down was released
	while(curHistory->frame >= sinceFrame)
	{
		if(curHistory->bKeyDown.released || curHistory->bButtonDown.released || curHistory->bHatDown.released || curHistory->bStickDown.released)
		{
			return true;
		}

		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}

	return false;
}

bool Fighter::ForwardHardPressed(InputStates * inputHistory, int untilFrame, int sinceFrame)
{
	if(inputHistory == NULL)
	{
		return false;
	}

	InputStates * curHistory = inputHistory;

	//trace back to the point in history just before the until frame
	while(curHistory->frame > untilFrame)
	{
		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}

	int hardPressFrame;

	//find the latest point in history the stick was pressed hard
	while(curHistory->frame >= sinceFrame)
	{
		if((facing == LEFT && curHistory->bStickHardLeft.pressed) ||
		(facing == RIGHT && curHistory->bStickHardRight.pressed))
		{
			hardPressFrame = curHistory->frame;
			break;
		}

		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}

	if(curHistory->frame < sinceFrame) { return false; }
	
	//find the latest point in history the stick was neutral
	while(curHistory->frame >= hardPressFrame - HARD_PRESS_THRESHOLD)
	{
		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return true; }

		if(!curHistory->bStickLeft.held && !curHistory->bStickRight.held && !curHistory->bStickUp.held && !curHistory->bStickDown.held)
		{
			return true;
		}
	}

	return false;
}

bool Fighter::BackwardHardPressed(InputStates * inputHistory, int untilFrame, int sinceFrame)
{
	if(inputHistory == NULL)
	{
		return false;
	}

	InputStates * curHistory = inputHistory;

	//trace back to the point in history just before the until frame
	while(curHistory->frame > untilFrame)
	{
		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}

	int hardPressFrame;

	//find the latest point in history the stick was pressed hard
	while(curHistory->frame >= sinceFrame)
	{
		if((facing == RIGHT && curHistory->bStickHardLeft.pressed) ||
		(facing == LEFT && curHistory->bStickHardRight.pressed))
		{
			hardPressFrame = curHistory->frame;
			break;
		}

		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}

	if(curHistory->frame < sinceFrame) { return false; }
	
	//find the latest point in history the stick was neutral
	while(curHistory->frame >= hardPressFrame - HARD_PRESS_THRESHOLD)
	{
		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return true; }

		if(!curHistory->bStickLeft.held && !curHistory->bStickRight.held && !curHistory->bStickUp.held && !curHistory->bStickDown.held)
		{
			return true;
		}
	}

	return false;
}

bool Fighter::QCFInput(InputStates * inputHistory, int untilFrame, int sinceFrame)
{
	if(inputHistory == NULL)
	{
		return false;
	}

	InputStates * curHistory = inputHistory;

	//trace back to the point in history just before the until frame
	while(curHistory->frame > untilFrame)
	{
		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}

	int forwardFrame;

	//find the latest point in history that down was released while forward is held
	while(curHistory->frame >= sinceFrame)
	{
		if(((facing == LEFT && (curHistory->bKeyLeft.held || curHistory->bButtonLeft.held || curHistory->bHatLeft.held || curHistory->bStickLeft.held)) ||
		(facing == RIGHT && (curHistory->bKeyRight.held || curHistory->bButtonRight.held || curHistory->bHatRight.held || curHistory->bStickRight.held)))
		&& (curHistory->bKeyDown.released || curHistory->bButtonDown.released || curHistory->bHatDown.released || curHistory->bStickDown.released))
		{
			forwardFrame = curHistory->frame;
			break;
		}

		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}

	if(curHistory->frame < sinceFrame) { return false; }

	int diagonalFrame;

	//find the latest point in history that forward was pressed while down is held
	while(curHistory->frame >= forwardFrame - COMMAND_INPUT_THRESHOLD)
	{
		if(((facing == LEFT && (curHistory->bKeyLeft.pressed || curHistory->bButtonLeft.pressed || curHistory->bHatLeft.pressed || curHistory->bStickLeft.pressed)) ||
		(facing == RIGHT && (curHistory->bKeyRight.pressed || curHistory->bButtonRight.pressed || curHistory->bHatRight.pressed || curHistory->bStickRight.pressed)))
		&& (curHistory->bKeyDown.held || curHistory->bButtonDown.held || curHistory->bHatDown.held || curHistory->bStickDown.held))
		{
			diagonalFrame = curHistory->frame;
			break;
		}

		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}

	if(curHistory->frame < forwardFrame - COMMAND_INPUT_THRESHOLD) { return false; }

	//find the latest point in history that down was pressed
	while(curHistory->frame >= diagonalFrame - COMMAND_INPUT_THRESHOLD)
	{
		if(curHistory->bKeyDown.pressed || curHistory->bButtonDown.pressed || curHistory->bHatDown.pressed || curHistory->bStickDown.pressed)
		{
			return true;
		}

		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}

	return false;
}