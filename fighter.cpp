#include "fighter.h"

///////////////
//FighterHold//
///////////////

FighterHold::FighterHold() : PhysicsObjectHold()
{
	disableAirControl = false;
	endAirDash = false;
	changeCancels = false;
	cancels.dash = NEVER;
	cancels.heavyBackward = NEVER;
	cancels.heavyDown = NEVER;
	cancels.heavyForward = NEVER;
	cancels.heavyNeutral = NEVER;
	cancels.heavyQCF = NEVER;
	cancels.heavyUp = NEVER;
	cancels.jump = NEVER;
	cancels.lightBackward = NEVER;
	cancels.lightDown = NEVER;
	cancels.lightForward = NEVER;
	cancels.lightNeutral = NEVER;
	cancels.lightQCF = NEVER;
	cancels.lightUp = NEVER;
	changeFighterAttributes = false;
	superArmorDamage = 0;
	superArmorHits = 0;
	superArmorDamageScaling = 0;
}

FighterHold::~FighterHold()
{

}

bool FighterHold::IsFighterHold()
{

	return true;
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
	hitstunHitLevel = HIT_MID;
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
	disableAirControl = false;
	endAirDash = false;
	superArmorDamage = 0;
	superArmorHits = 0;
	superArmorDamageScaling = 1;
	hasHitSuperArmor = false;
	hasDamageSuperArmor = false;

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

	curAttackAction = NO_ATTACK_ACTION;
	inComboString = false;
	comboTrack.clear();

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
			airDash = NO_AIR_DASH;
			curAirDashDuration = 0;
			if(!attacking && !blocking && curBlockstun <= 0)
			{
				ChangeHold(fighterEventHolds.jumpNeutralFall);
			}
		}
		else if(airDash == BACKWARD_AIR_DASH && curAirDashDuration >= backwardAirDashDuration)
		{
			falls = true;
			state = JUMPING;
			airDash = NO_AIR_DASH;
			curAirDashDuration = 0;
			if(!attacking && !blocking && curBlockstun <= 0)
			{
				ChangeHold(fighterEventHolds.jumpBackwardFall);
			}
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
			comboTrack.clear();
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
			state = STANDING;
			vel.x = 0;
			vel.y = 0;
			falls = false;
			ChangeHold(fighterEventHolds.standing);
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
			(fighterEventHolds.attackLightQCFAir != NULL && (state == JUMPING || state == AIR_DASHING))))
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
			(fighterEventHolds.attackHeavyQCFAir != NULL && (state == JUMPING || state == AIR_DASHING))))
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
					state = RUNNING;
					if(runAfterTurning)
					{
						walkAfterTurning = false;
						runAfterTurning = false;
					}
					ChangeHold(fighterEventHolds.run);
				}
				else if(!attacking)
				{
					state = WALKING;
					if(walkAfterTurning)
					{
						walkAfterTurning = false;
						runAfterTurning = false;
						ChangeHold(fighterEventHolds.walking);
					}
					else
					{
						ChangeHold(fighterEventHolds.walk);
					}
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
				facing = LEFT;
				state = STANDING;
				turning = true;
				walkAfterTurning = false;
				runAfterTurning = false;
				/*if(ForwardHardPressed(inputHistory, frame, frame)) { runAfterTurning = true; }
				else { */walkAfterTurning = true; //}
				timeSinceWalkStop = MOVING_TURN_THRESHOLD + 1;
				ChangeHold(fighterEventHolds.walkingTurn);
			}
			else if(!attacking && !blocking && state == STANDING && !runStopping)
			{
				facing = LEFT;
				turning = true;
				walkAfterTurning = false;
				runAfterTurning = false;
				//if(ForwardHardPressed(inputHistory, frame, frame)) { runAfterTurning = true; }
				state = STANDING;
				ChangeHold(fighterEventHolds.turn);
			}
			else if(!attacking && !blocking && state == CROUCHING)
			{
				facing = LEFT;
				turning = true;
				ChangeHold(fighterEventHolds.crouchingTurn);
			}
			else if(!attacking && !blocking && ((state == STANDING && timeSinceRunStop < MOVING_TURN_THRESHOLD && runStopping) || state == RUNNING))
			{
				facing = LEFT;
				turning = true;
				runStopping = false;
				walkAfterTurning = false;
				runAfterTurning = true;
				timeSinceRunStop = MOVING_TURN_THRESHOLD + 1;
				sliding = true;
				state = STANDING;
				ChangeHold(fighterEventHolds.runningTurn);
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
				facing = RIGHT;
				turning = true;
				walkAfterTurning = false;
				runAfterTurning = false;
				/*if(ForwardHardPressed(inputHistory, frame, frame)) { runAfterTurning = true; }
				else { */walkAfterTurning = true; //}
				timeSinceWalkStop = MOVING_TURN_THRESHOLD + 1;
				state = STANDING;
				ChangeHold(fighterEventHolds.walkingTurn);
			}
			else if(!attacking && !blocking && state == STANDING && !runStopping)
			{
				facing = RIGHT;
				turning = true;
				walkAfterTurning = false;
				runAfterTurning = false;
				//if(ForwardHardPressed(inputHistory, frame, frame)) { runAfterTurning = true; }
				state = STANDING;
				ChangeHold(fighterEventHolds.turn);
			}
			else if(!attacking && !blocking && state == CROUCHING)
			{
				facing = RIGHT;
				turning = true;
				ChangeHold(fighterEventHolds.crouchingTurn);
			}
			else if(!attacking && !blocking && ((state == STANDING && timeSinceRunStop < MOVING_TURN_THRESHOLD && runStopping) || state == RUNNING))
			{
				facing = RIGHT;
				turning = true;
				runStopping = false;
				walkAfterTurning = false;
				runAfterTurning = true;
				sliding = true;
				timeSinceRunStop = MOVING_TURN_THRESHOLD + 1;
				state = STANDING;
				ChangeHold(fighterEventHolds.runningTurn);
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
					if(runAfterTurning)
					{
						walkAfterTurning = false;
						runAfterTurning = false;
					}
					state = RUNNING;
					ChangeHold(fighterEventHolds.run);
				}
				else if(!attacking)
				{
					state = WALKING;
					if(walkAfterTurning)
					{
						walkAfterTurning = false;
						runAfterTurning = false;
						ChangeHold(fighterEventHolds.walking);
					}
					else
					{
						ChangeHold(fighterEventHolds.walk);
					}
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
			timeSinceWalkStop = 0;
			state = STANDING;
			ChangeHold(fighterEventHolds.standing);
		}
		else if(state == RUNNING)
		{
			timeSinceRunStop = 0;
			runStopping = true;
			sliding = true;
			state = STANDING;
			ChangeHold(fighterEventHolds.runningStop);
		}
	}

	//dash
	if(CanDashCancel() && !runStopping && !blocking)
	{
		if(!turning && bufferedAction == DASH)
		{
			inComboString = false;
			curAttackAction = NO_ATTACK_ACTION;
			if(state == STANDING || state == WALKING || state == CROUCHING)
			{
				bufferedAction = NO_ACTION;
				walkAfterTurning = false;
				runAfterTurning = false;
				attacking = false;
				state = RUNNING;
				ChangeHold(fighterEventHolds.run);
			}
			else if(state == JUMPING && curAirActions > 0 && !jumpStartup && !jumpStartupJustEnded)
			{
				bufferedAction = NO_ACTION;
				state = AIR_DASHING;
				airDash = FORWARD_AIR_DASH;
				airControl = NO_AIR_CONTROL;
				curAirActions--;
				landingAction = DASH;
				turnUponLanding = false;
				attacking = false;
				ChangeHold(fighterEventHolds.airDashForward);
			}
		}
		else if(bufferedAction == BACKWARD_DASH)
		{
			if(!attacking && (state == STANDING || state == WALKING || state == CROUCHING))
			{
				bufferedAction = NO_ACTION;
				runAfterTurning = true;
			}
			else if(!turning && state == JUMPING && curAirActions > 0 && !jumpStartup && !jumpStartupJustEnded)
			{
				inComboString = false;
				curAttackAction = NO_ATTACK_ACTION;
				bufferedAction = NO_ACTION;
				state = AIR_DASHING;
				airDash = BACKWARD_AIR_DASH;
				curAirActions--;
				landingAction = DASH;
				turnUponLanding = true;
				attacking = false;
				ChangeHold(fighterEventHolds.airDashBackward);
			}
		}
	}

	//crouch
	if(inputHistory->bKeyDown.held || inputHistory->bButtonDown.held || inputHistory->bHatDown.held || inputHistory->bStickDown.held)
	{
		if(!attacking && (state == STANDING || state == WALKING || state == RUNNING))
		{
			walkAfterTurning = false;
			runAfterTurning = false;
			turning = false;
			runStopping = false;
			if(state == RUNNING) { sliding = true; }
			state = CROUCHING;
			if(blocking || curBlockstun > 0)
			{
				ChangeHold(fighterEventHolds.blockLow);
			}
			else
			{
				ChangeHold(fighterEventHolds.crouch);
			}
		}
		else if(state == JUMPING)
		{
			landingAction = CROUCH;
			if(inputHistory->bKeyJump.held || inputHistory->bButtonJump.held)
			{
				ignoreJumpThroughTerrain = true;
			}
		}
	}
	else if(!attacking && state == CROUCHING)
	{
		turning = false;
		state = STANDING;
		if(blocking || curBlockstun > 0)
		{
			ChangeHold(fighterEventHolds.blockHigh);
		}
		else
		{
			ChangeHold(fighterEventHolds.stand);
		}
	}

	//jump
	if(CanJumpCancel() && !jumpStartup && !jumpStartupJustEnded && !blocking && curBlockstun <= 0 && bufferedAction == JUMP)
	{
		if(state != JUMPING && state != AIRBORN && state != AIR_DASHING)
		{
			inComboString = false;
			curAttackAction = NO_ATTACK_ACTION;
			bufferedAction = NO_ACTION;
			//ground jump
			if(state == CROUCHING)
			{
				ignoreJumpThroughTerrain = true;
			}
			else
			{
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
				ChangeHold(fighterEventHolds.jumpNeutralStart);
			}
		}
		else if(state == JUMPING && !(inputHistory->bKeyDown.held || inputHistory->bButtonDown.held || inputHistory->bHatDown.held || inputHistory->bStickDown.held))
		{
			bufferedAction = NO_ACTION;
			//air jump
			if(curAirActions > 0)
			{
				inComboString = false;
				curAttackAction = NO_ATTACK_ACTION;
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
				ChangeHold(fighterEventHolds.jumpNeutralStartAir);
			}
		}
	}

	//short hop
	if((jumpStartup || jumpStartupJustEnded) && !inputHistory->bKeyJump.held && !inputHistory->bButtonJump.held)
	{
		shortHop = true;
	}

	//blocking
	if(!attacking && !blocking && !jumpStartup && curBlockstun <= 0 && (inputHistory->bKeyBlock.held || inputHistory->bButtonBlock.held))
	{
		if(state == STANDING || state == WALKING || state == RUNNING || state == CROUCHING)
		{
			if(state == RUNNING)
			{
				sliding = true;
			}
			walkAfterTurning = false;
			runAfterTurning = false;
			turning = false;
			runStopping = false;
			jump = NO_JUMP;
			blocking = true;
			jumpStartup = false;
			shortHop = false;
			if(inputHistory->bKeyDown.held || inputHistory->bButtonDown.held || inputHistory->bStickDown.held || inputHistory->bHatDown.held)
			{
				state = CROUCHING;
				ChangeHold(fighterEventHolds.blockLow);
			}
			else
			{
				state = STANDING;
				ChangeHold(fighterEventHolds.blockHigh);
			}
		}
		else if(state == JUMPING || state == AIR_DASHING)
		{
			blocking = true;
			ChangeHold(fighterEventHolds.blockAir);
		}
	}
	else if(blocking && curBlockstun <= 0 && !inputHistory->bKeyBlock.held && !inputHistory->bButtonBlock.held)
	{
		turning = false;
		runStopping = false;
		blocking = false;
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
	}

	if(curBlockstun > 0 || jumpStartup || jumpStartupJustEnded)
	{
		jumpStartupJustEnded = false;
		return 0;
	}

	//light attacks
	if(bufferedAction >= NEUTRAL_LIGHT && bufferedAction <= QCF_LIGHT)
	{
		if(state == STANDING || state == CROUCHING || state == WALKING || state == RUNNING)
		{
			if(CanLightQCFCancel() && fighterEventHolds.attackLightQCFGround != NULL && bufferedAction == QCF_LIGHT)
			{
				curAttackAction = GROUND_QCF_LIGHT;
				GroundAttackExecuted();
				ChangeHold(fighterEventHolds.attackLightQCFGround);
			}
			else if(CanLightUpCancel() && fighterEventHolds.attackLightUpGround != NULL && bufferedAction == UP_LIGHT)
			{
				curAttackAction = GROUND_UP_LIGHT;
				GroundAttackExecuted();
				ChangeHold(fighterEventHolds.attackLightUpGround);
			}
			else if(CanLightDownCancel() && fighterEventHolds.attackLightDownGround != NULL && bufferedAction == DOWN_LIGHT)
			{
				curAttackAction = GROUND_DOWN_LIGHT;
				GroundAttackExecuted();
				state = CROUCHING;
				ChangeHold(fighterEventHolds.attackLightDownGround);
			}
			else if(CanLightForwardCancel() && fighterEventHolds.attackLightForwardGround != NULL && (bufferedAction == FORWARD_LIGHT || ((bufferedAction == NEUTRAL_LIGHT || bufferedAction == BACKWARD_LIGHT) && (curAttackAction == GROUND_NEUTRAL_LIGHT || curAttackAction == GROUND_DOWN_LIGHT))))
			{
				curAttackAction = GROUND_FORWARD_LIGHT;
				GroundAttackExecuted();
				ChangeHold(fighterEventHolds.attackLightForwardGround);
			}
			else if(CanLightNeutralCancel() && fighterEventHolds.attackLightNeutralGround != NULL && (bufferedAction == NEUTRAL_LIGHT || bufferedAction == BACKWARD_LIGHT))
			{
				curAttackAction = GROUND_NEUTRAL_LIGHT;
				GroundAttackExecuted();
				ChangeHold(fighterEventHolds.attackLightNeutralGround);
			}
			bufferedAction = NO_ACTION;
		}
		else if(state == JUMPING || state == AIR_DASHING)
		{
			if(CanLightQCFCancel() && fighterEventHolds.attackLightQCFAir != NULL && bufferedAction == QCF_LIGHT)
			{
				curAttackAction = AIR_QCF_LIGHT;
				AirAttackExecuted();
				landingAction = QCF_LIGHT;
				ChangeHold(fighterEventHolds.attackLightQCFAir);
			}
			else if(CanLightUpCancel() && fighterEventHolds.attackLightUpAir != NULL && bufferedAction == UP_LIGHT)
			{
				curAttackAction = AIR_UP_LIGHT;
				AirAttackExecuted();
				landingAction = UP_LIGHT;
				ChangeHold(fighterEventHolds.attackLightUpAir);
			}
			else if(CanLightDownCancel() && fighterEventHolds.attackLightDownAir != NULL && bufferedAction == DOWN_LIGHT)
			{
				curAttackAction = AIR_DOWN_LIGHT;
				AirAttackExecuted();
				landingAction = DOWN_LIGHT;
				ChangeHold(fighterEventHolds.attackLightDownAir);
			}
			else if(CanLightForwardCancel() && fighterEventHolds.attackLightForwardAir != NULL && (bufferedAction == FORWARD_LIGHT || (bufferedAction == NEUTRAL_LIGHT && curAttackAction == AIR_NEUTRAL_LIGHT)))
			{
				curAttackAction = AIR_FORWARD_LIGHT;
				AirAttackExecuted();
				landingAction = FORWARD_LIGHT;
				turnUponLanding = false;
				ChangeHold(fighterEventHolds.attackLightForwardAir);
			}
			else if(CanLightBackwardCancel() && fighterEventHolds.attackLightBackwardAir != NULL && bufferedAction == BACKWARD_LIGHT)
			{
				curAttackAction = AIR_BACKWARD_LIGHT;
				AirAttackExecuted();
				landingAction = FORWARD_LIGHT;
				turnUponLanding = true;
				ChangeHold(fighterEventHolds.attackLightBackwardAir);
			}
			else if(CanLightNeutralCancel() && fighterEventHolds.attackLightNeutralAir != NULL && bufferedAction == NEUTRAL_LIGHT)
			{
				curAttackAction = AIR_NEUTRAL_LIGHT;
				AirAttackExecuted();
				landingAction = NEUTRAL_LIGHT;
				ChangeHold(fighterEventHolds.attackLightNeutralAir);
			}
			bufferedAction = NO_ACTION;
		}
	}

	//heavy attacks
	if(bufferedAction >= NEUTRAL_HEAVY && bufferedAction <= QCF_HEAVY)
	{
		if(state == STANDING || state == CROUCHING || state == WALKING || state == RUNNING)
		{
			if(CanHeavyQCFCancel() && fighterEventHolds.attackHeavyQCFGround != NULL && bufferedAction == QCF_HEAVY)
			{
				curAttackAction = GROUND_QCF_HEAVY;
				GroundAttackExecuted();
				ChangeHold(fighterEventHolds.attackHeavyQCFGround);
			}
			else if(CanHeavyUpCancel() && fighterEventHolds.attackHeavyUpGround != NULL && bufferedAction == UP_HEAVY)
			{
				curAttackAction = GROUND_UP_HEAVY;
				GroundAttackExecuted();
				ChangeHold(fighterEventHolds.attackHeavyUpGround);
			}
			else if(CanHeavyDownCancel() && fighterEventHolds.attackHeavyDownGround != NULL && bufferedAction == DOWN_HEAVY)
			{
				curAttackAction = GROUND_DOWN_HEAVY;
				GroundAttackExecuted();
				state = CROUCHING;
				ChangeHold(fighterEventHolds.attackHeavyDownGround);
			}
			else if(CanHeavyForwardCancel() && fighterEventHolds.attackHeavyForwardGround != NULL && (bufferedAction == FORWARD_HEAVY || ((bufferedAction == NEUTRAL_HEAVY || bufferedAction == BACKWARD_HEAVY) && curAttackAction == GROUND_NEUTRAL_HEAVY)))
			{
				curAttackAction = GROUND_FORWARD_HEAVY;
				GroundAttackExecuted();
				ChangeHold(fighterEventHolds.attackHeavyForwardGround);
			}
			else if(CanHeavyNeutralCancel() && fighterEventHolds.attackHeavyNeutralGround != NULL && (bufferedAction == NEUTRAL_HEAVY || bufferedAction == BACKWARD_HEAVY))
			{
				curAttackAction = GROUND_NEUTRAL_HEAVY;
				GroundAttackExecuted();
				ChangeHold(fighterEventHolds.attackHeavyNeutralGround);
			}
			bufferedAction = NO_ACTION;
		}
		else if(state == JUMPING || state == AIR_DASHING)
		{
			if(CanHeavyQCFCancel() && fighterEventHolds.attackHeavyQCFAir != NULL && bufferedAction == QCF_HEAVY)
			{
				curAttackAction = AIR_QCF_HEAVY;
				AirAttackExecuted();
				landingAction = QCF_HEAVY;
				ChangeHold(fighterEventHolds.attackHeavyQCFAir);
			}
			else if(CanHeavyUpCancel() && fighterEventHolds.attackHeavyUpAir != NULL && bufferedAction == UP_HEAVY)
			{
				curAttackAction = AIR_UP_HEAVY;
				AirAttackExecuted();
				landingAction = UP_HEAVY;
				ChangeHold(fighterEventHolds.attackHeavyUpAir);
			}
			else if(CanHeavyDownCancel() && fighterEventHolds.attackHeavyDownAir != NULL && bufferedAction == DOWN_HEAVY)
			{
				curAttackAction = AIR_DOWN_HEAVY;
				AirAttackExecuted();
				landingAction = DOWN_HEAVY;
				ChangeHold(fighterEventHolds.attackHeavyDownAir);
			}
			else if(CanHeavyForwardCancel() && fighterEventHolds.attackHeavyForwardAir != NULL && (bufferedAction == FORWARD_HEAVY || (bufferedAction == NEUTRAL_HEAVY && curAttackAction == AIR_NEUTRAL_HEAVY)))
			{
				curAttackAction = AIR_FORWARD_HEAVY;
				AirAttackExecuted();
				landingAction = FORWARD_HEAVY;
				ChangeHold(fighterEventHolds.attackHeavyForwardAir);
			}
			else if(CanHeavyBackwardCancel() && fighterEventHolds.attackHeavyBackwardAir != NULL && bufferedAction == BACKWARD_HEAVY)
			{
				curAttackAction = AIR_BACKWARD_HEAVY;
				AirAttackExecuted();
				landingAction = FORWARD_HEAVY;
				ChangeHold(fighterEventHolds.attackHeavyBackwardAir);
			}
			else if(CanHeavyNeutralCancel() && fighterEventHolds.attackHeavyNeutralAir != NULL && bufferedAction == NEUTRAL_HEAVY)
			{
				curAttackAction = AIR_NEUTRAL_HEAVY;
				AirAttackExecuted();
				landingAction = NEUTRAL_HEAVY;
				ChangeHold(fighterEventHolds.attackHeavyNeutralAir);
			}
			bufferedAction = NO_ACTION;
		}
	}

	return 0;
}

int Fighter::Update()
{
	//apply facing to reposition
	reposition.x = reposition.x * facing;

	//apply facing to overwrite velocity
	if(overwriteVelocity)
	{
		holdVelocity.x = holdVelocity.x * facing;

		if(vel.x != 0 && (state == STANDING || state == CROUCHING || (state == JUMPING && jumpStartup) || state == KNOCKOUT))
		{
			sliding = true;
		}
	}

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

	bool returnToPreviousVelocity = !overwriteVelocity;

	if(int error = PhysicsObject::Update() != 0) { return error; } //there was an error in the base update

	//reapply velocity after ignoring it during hitstop
	if(curHitstop > 0)
	{
		vel = hadVel;
		falls = didFall;
	}

	//return to velocity before push
	prevVel.x = prevVelX;
	if(returnToPreviousVelocity) { vel.x = prevVelX; }

	//handle facing direction
	if(facing == LEFT)
	{
		hFlip = true;
	}
	else
	{
		hFlip = false;
	}

	if(endAirDash && state == AIR_DASHING)
	{
		state = JUMPING;
		falls = true;
		airDash = NO_AIR_DASH;
		curAirDashDuration = 0;
	}
	endAirDash = false;

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
		if(curHitstop <= 0)
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
		if(disableAirControl)
		{
			airControl = NO_AIR_CONTROL;
		}
		else
		{
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
				vel.y = ceil(-jumpSpeed * 2 / 3);
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
	if(pos.x < target->pos.x)
	{
		fighterPushXAccel -= FIGHTER_OVERLAP_PUSH_ACCEL;
	}
	else if(pos.x > target->pos.x)
	{
		fighterPushXAccel += FIGHTER_OVERLAP_PUSH_ACCEL;
	}
	else
	{
		Fighter * ft;
		if(target->IsFighter())
		{
			ft = (Fighter*)target;
		}
		else
		{
			fighterPushXAccel -= FIGHTER_OVERLAP_PUSH_ACCEL;
			return;
		}

		if(ft->fighterPushXAccel <= 0)
		{
			fighterPushXAccel += FIGHTER_OVERLAP_PUSH_ACCEL;
		}
		else
		{
			fighterPushXAccel -= FIGHTER_OVERLAP_PUSH_ACCEL;
		}
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
		state = JUMPING;
		falls = true;
		ChangeHold(fighterEventHolds.jumpNeutralFall);
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
	inComboString = false;
	curAttackAction = NO_ATTACK_ACTION;
	curAirActions = airActions;
	falls = false;
	attacking = false;

	if(airVelocityCategory != LOW_VELOCITY || curHitstun > 0 || curBlockstun > 0)
	{
		sliding = true;
	}
	else
	{
		vel.x = 0;
	}

	if(state == KNOCKOUT_AIR)
	{
		state = KNOCKOUT;
		ChangeHold(fighterEventHolds.knockout);
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
			state = STANDING;
			if(curHitstun > 0)
			{
				if(hitstunHitLevel == HIT_MID)
				{
					ChangeHold(fighterEventHolds.hitstunLightMidStanding);
				}
				else if(hitstunHitLevel == HIT_HIGH)
				{
					ChangeHold(fighterEventHolds.hitstunLightHighStanding);
				}
				else if(hitstunHitLevel == HIT_LOW)
				{
					ChangeHold(fighterEventHolds.hitstunLightLowStanding);
				}
			}
			else if(blocking || curBlockstun > 0)
			{
				ChangeHold(fighterEventHolds.blockHigh);
			}
			else if(airVelocityCategory == HIGH_VELOCITY_FORWARD)
			{
				runStopping = true;
				timeSinceRunStop = 0;
				ChangeHold(fighterEventHolds.runningStop);
			}
			else if(airVelocityCategory == HIGH_VELOCITY_BACKWARD)
			{
				runStopping = true;
				ChangeHold(fighterEventHolds.jumpNeutralLand);
			}
			else
			{
				ChangeHold(fighterEventHolds.jumpNeutralLand);
			}
			break;
		case MOVE:
			if(curHitstun > 0)
			{
				state = STANDING;
				if(hitstunHitLevel == HIT_MID)
				{
					ChangeHold(fighterEventHolds.hitstunLightMidStanding);
				}
				else if(hitstunHitLevel == HIT_HIGH)
				{
					ChangeHold(fighterEventHolds.hitstunLightHighStanding);
				}
				else if(hitstunHitLevel == HIT_LOW)
				{
					ChangeHold(fighterEventHolds.hitstunLightLowStanding);
				}
			}
			else if(blocking || curBlockstun > 0)
			{
				state = STANDING;
				ChangeHold(fighterEventHolds.blockHigh);
			}
			else if(airVelocityCategory == HIGH_VELOCITY_FORWARD)
			{
				if(turnUponLanding)
				{
					state = STANDING;
					turning = true;
					runAfterTurning = true;
					ChangeHold(fighterEventHolds.runningTurn);
				}
				else
				{
					vel.x = runSpeed * facing;
					state = RUNNING;
					ChangeHold(fighterEventHolds.running);
				}
			}
			else if(airVelocityCategory == HIGH_VELOCITY_BACKWARD)
			{
				if(turnUponLanding)
				{
					state = STANDING;
					turning = true;
					ChangeHold(fighterEventHolds.turn);
				}
				else
				{
					runStopping = true;
					state = STANDING;
					ChangeHold(fighterEventHolds.jumpNeutralLand);
				}
			}
			else
			{
				if(turnUponLanding)
				{
					state = STANDING;
					turning = true;
					ChangeHold(fighterEventHolds.turn);
				}
				else
				{
					vel.x = walkSpeed * facing;
					state = WALKING;
					ChangeHold(fighterEventHolds.walking);
				}
			}
			break;
		case DASH:
			if(airVelocityCategory == HIGH_VELOCITY_BACKWARD)
			{
				if(turnUponLanding)
				{
					state = STANDING;
					turning = true;
					runAfterTurning = true;
					ChangeHold(fighterEventHolds.turn);
				}
				else
				{
					runStopping = true;
					runAfterTurning = true;
					state = STANDING;
					ChangeHold(fighterEventHolds.jumpNeutralLand);
				}
			}
			else
			{
				if(turnUponLanding)
				{
					state = STANDING;
					turning = true;
					runAfterTurning = true;
					ChangeHold(fighterEventHolds.runningTurn);
				}
				else
				{
					vel.x = runSpeed * facing;
					state = RUNNING;
					ChangeHold(fighterEventHolds.running);
				}
			}
			break;
		case CROUCH:
			state = CROUCHING;
			if(curHitstun > 0)
			{
				state = STANDING;
				if(hitstunHitLevel == HIT_MID)
				{
					ChangeHold(fighterEventHolds.hitstunLightMidStanding);
				}
				else if(hitstunHitLevel == HIT_HIGH)
				{
					ChangeHold(fighterEventHolds.hitstunLightHighStanding);
				}
				else if(hitstunHitLevel == HIT_LOW)
				{
					ChangeHold(fighterEventHolds.hitstunLightLowStanding);
				}
			}
			else if(blocking || curBlockstun > 0)
			{
				ChangeHold(fighterEventHolds.blockLow);
			}
			else
			{
				ChangeHold(fighterEventHolds.crouch);
			}
			break;
		case JUMP:
			//ChangeHold(fighterEventHolds.jumpNeutralStart);
			break;
		case NEUTRAL_LIGHT:
			GroundAttackExecuted();
			ChangeHold(fighterEventHolds.attackLightNeutralGround);
			break;
		case UP_LIGHT:
			GroundAttackExecuted();
			ChangeHold(fighterEventHolds.attackLightUpGround);
			break;
		case DOWN_LIGHT:
			GroundAttackExecuted();
			state = CROUCHING;
			ChangeHold(fighterEventHolds.attackLightDownGround);
			break;
		case FORWARD_LIGHT:
			GroundAttackExecuted();
			ChangeHold(fighterEventHolds.attackLightForwardGround);
			break;
		case QCF_LIGHT:
			GroundAttackExecuted();
			ChangeHold(fighterEventHolds.attackLightQCFGround);
			break;
		case NEUTRAL_HEAVY:
			GroundAttackExecuted();
			ChangeHold(fighterEventHolds.attackHeavyNeutralGround);
			break;
		case UP_HEAVY:
			GroundAttackExecuted();
			ChangeHold(fighterEventHolds.attackHeavyUpGround);
			break;
		case DOWN_HEAVY:
			GroundAttackExecuted();
			state = CROUCHING;
			ChangeHold(fighterEventHolds.attackHeavyDownGround);
			break;
		case FORWARD_HEAVY:
			GroundAttackExecuted();
			ChangeHold(fighterEventHolds.attackHeavyForwardGround);
			break;
		case QCF_HEAVY:
			GroundAttackExecuted();
			ChangeHold(fighterEventHolds.attackHeavyQCFGround);
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
	if(attackResults.timesStruck <= 0)
	{
		if(attackResults.didStrike)
		{
			curHitstop = attackResults.hitstop;
		}

		ResetAttackResults();
		return;
	}

	if(attackResults.blockstun <= 0 && (hasDamageSuperArmor || hasHitSuperArmor))
	{
		if(hasDamageSuperArmor)
		{
			superArmorDamage -= attackResults.damage;
		}
		if(hasHitSuperArmor)
		{
			superArmorHits -= attackResults.timesStruck;
		}

		int scaledDamage = (float)attackResults.damage * superArmorDamageScaling;

		if(((hasDamageSuperArmor && superArmorDamage >= 0) || (hasHitSuperArmor && superArmorHits >= 0)) && scaledDamage < curHealth)
		{
			curHealth -= scaledDamage;
			curHitstop = attackResults.hitstop;

			ResetAttackResults();
			return;
		}

		superArmorDamage = 0;
		superArmorHits = 0;
		superArmorDamageScaling = 1.0;
		hasDamageSuperArmor = false;
		hasHitSuperArmor = false;
	}

	if(state == JUMPING && jumpStartup)
	{
		state = STANDING;
	}

	jump = NO_JUMP;
	airDash = NO_AIR_DASH;
	jumpStartup = false;
	jumpStartupJustEnded = false;
	shortHop = false;
	runStopping = false;
	turning = false;
	attacking = false;
	walkAfterTurning = false;
	runAfterTurning = false;
	airControl = NO_AIR_CONTROL;
	landingAction = NO_ACTION;
	turnUponLanding = false;
	curAttackAction = NO_ATTACK_ACTION;

	if(attackResults.hFlip) { facing = LEFT; hFlip = true; }
	else { facing = RIGHT; hFlip = false; }

	if(health > 0)
	{
		curHealth -= attackResults.damage;

		if(attackResults.blockstun > 0 && curHealth < 1) { curHealth = 1; } //can't die from chip damage
	
		if(curHealth <= 0)
		{
			curHealth = 0;

			blocking = false;
			hitSomething = false;
			wasBlocked = false;
			curHitstun = 0;
			curBlockstun = 0;
			comboTrack.clear();
			
			vel.x = attackResults.force.x * -facing;
			vel.y = attackResults.force.y;

			if(state == KNOCKOUT && vel.x != 0)
			{
				sliding = true;
			}
			else
			{
				sliding = false;
			}

			if(state == JUMPING || state == AIR_DASHING)
			{
				state = KNOCKOUT_AIR;
				ChangeHold(fighterEventHolds.hitstunLightAir);
			}
			else if(state == STANDING || state == CROUCHING)
			{
				state = KNOCKOUT;
				ChangeHold(fighterEventHolds.knockout);
			}

			if(vel.y < 0)
			{
				state = KNOCKOUT_AIR;
				falls = true;
				ChangeHold(fighterEventHolds.hitstunLightAir);
			}
			
			ResetAttackResults();

			return;
		}
		else if(curHealth > health) { curHealth = health; }
	}

	hitstunHitLevel = attackResults.hitLevel;
	curHitstun = attackResults.hitstun;
	curBlockstun = attackResults.blockstun;
	curHitstop = attackResults.hitstop;
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
	
	if(attackResults.IPSTriggered)
	{
		curHitstun = 1;
	}

	if(curHitstun > 0)
	{
		blocking = false;
		if(state == STANDING || state == WALKING || state == RUNNING)
		{
			state = STANDING;
			if(hitstunHitLevel == HIT_MID)
			{
				ChangeHold(fighterEventHolds.hitstunLightMidStanding);
			}
			else if(hitstunHitLevel == HIT_HIGH)
			{
				ChangeHold(fighterEventHolds.hitstunLightHighStanding);
			}
			else if(hitstunHitLevel == HIT_LOW)
			{
				ChangeHold(fighterEventHolds.hitstunLightLowStanding);
			}
		}
		else if(state == CROUCHING)
		{
			if(hitstunHitLevel == HIT_MID || hitstunHitLevel == HIT_HIGH)
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
	}
	else if(curBlockstun > 0)
	{
		blocking = true;
		if(state == STANDING || state == WALKING || state == RUNNING)
		{
			state = STANDING;
			ChangeHold(fighterEventHolds.blockHigh);
		}
		else if(state == CROUCHING)
		{
			ChangeHold(fighterEventHolds.blockLow);
		}
		else if(state == JUMPING)
		{
			ChangeHold(fighterEventHolds.blockAir);
		}
	}

	ResetAttackResults();
}

void Fighter::HandleAttackCollision(TerrainObject * targetObject)
{
	//first, let the target know it's been hit
	if(!targetObject->HandleHurtCollision(this)) { return; }

	//save the target so this object doesn't strike it again until the attack is over
	victims.push_back(targetObject->id);

	attackResults.hitstop = ownHitstop;
	attackResults.didStrike = true;

	inComboString = true;
}

bool Fighter::HandleHurtCollision(TerrainObject * attacker)
{
	//get the effective hit level
	HitLevel effectiveHitLevel = attacker->hitLevel;
	if(state == JUMPING || attacker->hitLevel == HIT_STRICTLY_MID
		|| (attacker->hitLevel == HIT_HIGH && attacker->pos.y > pos.y)
		|| (attacker->hitLevel == HIT_LOW && attacker->pos.y < pos.y))
	{
		effectiveHitLevel = HIT_MID;
	}
	else if(effectiveHitLevel == HIT_MID)
	{
		if(attacker->IsFighter() && ((Fighter*)attacker)->state == JUMPING)
		{
			if(attacker->pos.y <= pos.y && attacker->vel.y > 0)
			{
				effectiveHitLevel = HIT_HIGH;
			}
			else if(attacker->pos.y >= pos.y && attacker->vel.y < 0)
			{
				effectiveHitLevel = HIT_LOW;
			}
		}
		else
		{
			if(attacker->pos.y < pos.y && attacker->vel.y >= 0)
			{
				effectiveHitLevel = HIT_HIGH;
			}
			else if(attacker->pos.y > pos.y && attacker->vel.y <= 0)
			{
				effectiveHitLevel = HIT_LOW;
			}
		}
	}

	//handle invuln
	if(invulnerability == INVULN_FULL
		|| (effectiveHitLevel == HIT_HIGH && invulnerability == INVULN_HIGH)
		|| (effectiveHitLevel == HIT_LOW && invulnerability == INVULN_LOW))
	{
		return false;
	}

	//handle blocking
	attackResults.timesStruck++;
	attackResults.hitLevel = effectiveHitLevel;

	FighterFacing reqBlockDirection = LEFT;
	attackResults.hFlip = true;
	if((attacker->horizontalDirectionBasedBlock && attacker->hFlip)
		|| (pos.x < attacker->pos.x))
	{
		if(attacker->hFlip)
		{
			reqBlockDirection = RIGHT;
			attackResults.hFlip = false;
		}
	}

	if(attacker->reversedHorizontalBlock)
	{
		if(reqBlockDirection == RIGHT)
		{
			reqBlockDirection = LEFT;
			attackResults.hFlip = true;
		}
		else
		{
			reqBlockDirection = RIGHT;
			attackResults.hFlip = false;
		}
	}

	if(blocking && facing == reqBlockDirection && attacker->blockability != UNBLOCKABLE
		&& (effectiveHitLevel == HIT_MID
		|| (effectiveHitLevel == HIT_HIGH && state == STANDING) 
		|| (effectiveHitLevel == HIT_LOW && state == CROUCHING)))
	{
		attackResults.damage += attacker->damage / 10;
		attackResults.hitstop = attacker->victimHitstop;
		attackResults.blockstun = attacker->blockstun;
		attackResults.force.x = attacker->force.x;
		attackResults.force.y = 0;
		attacker->numBlockedByThisFrame++;
		
		if(attacker->IsFighter())
		{
			((Fighter*)attacker)->wasBlocked = true;
		}

		return true;
	}

	//handle being hit
	attackResults.damage += attacker->damage;
	attackResults.hitstop = attacker->victimHitstop;
	attackResults.hitstun = attacker->hitstun;
	attackResults.force.x = attacker->force.x;
	attackResults.force.y = attacker->force.y;
	attacker->numHitThisFrame++;
	
	if(attacker->IsFighter() || attacker->parent->IsFighter())
	{
		Fighter * fAttacker;

		if(attacker->IsFighter())
		{
			fAttacker = (Fighter*)attacker;
		}
		else
		{
			fAttacker = (Fighter*)attacker->parent;
		}

		fAttacker->hitSomething = true;

		//handle ips
		bool attackerFound = false;
		list<ComboTrack>::iterator comboItr;
		for(comboItr = comboTrack.begin(); comboItr != comboTrack.end(); comboItr++)
		{
			if(comboItr->attacker == fAttacker)
			{
				attackerFound = true;
				comboItr->hits++;

				if(!fAttacker->inComboString)
				{
					//attacker started a new string, so increment the combo stage
					comboItr->stage++;
				}

				//track hits from stage 3 and onward
				if(comboItr->stage >= 3)
				{
					bool actionFound = false;
					list<FighterAttackAction>::iterator actionItr;
					for(actionItr = comboItr->usedAttacks.begin(); actionItr != comboItr->usedAttacks.end(); actionItr++)
					{
						if(*actionItr == fAttacker->curAttackAction)
						{
							actionFound = true;

							if(comboItr->stage >= 4 && !fAttacker->inComboString)
							{
								//ips triggered! do... something
								attackResults.IPSTriggered = true;
								comboTrack.clear();
							}

							break;
						}
					}

					if(!actionFound)
					{
						comboItr->usedAttacks.push_back(fAttacker->curAttackAction);
					}
				}

				break;
			}
		}

		if(!attackerFound)
		{
			ComboTrack newTrack;
			newTrack.attacker = fAttacker;
			newTrack.hits = 1;

			if(fAttacker->state == JUMPING)
			{
				newTrack.stage = 1;
			}
			else
			{
				newTrack.stage = 2;
			}

			comboTrack.push_back(newTrack);
		}
	}

	return true;
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

bool Fighter::AdvanceHold(HSObjectHold* hold)
{
	if(PhysicsObject::AdvanceHold(hold))
	{
		FighterHold * fHold = (FighterHold*)curHold;

		if(fHold->changeCancels)
		{
			cancels = fHold->cancels;
		}

		if(fHold->changeFighterAttributes)
		{
			disableAirControl = fHold->disableAirControl;
			endAirDash = fHold->endAirDash;
		}

		if(fHold->changeHurtBoxAttributes)
		{
			superArmorHits = fHold->superArmorHits;
			superArmorDamage = fHold->superArmorDamage;
			if(superArmorHits > 0) { hasHitSuperArmor = true; }
			if(superArmorDamage > 0) { hasDamageSuperArmor = true; }
		}

		return true;
	}

	return false;
}

bool Fighter::ChangeHold(HSObjectHold* hold)
{
	disableAirControl = false;
	endAirDash = false;

	return PhysicsObject::ChangeHold(hold);
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
	hitLevel = HIT_MID;
	blockability = BLOCKABLE;
	invulnerability = INVULN_NONE;
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
	inComboString = false;
	curAttackAction = NO_ATTACK_ACTION;
	ignoreGravity = false;
	disableAirControl = false;
	endAirDash = false;
	superArmorDamage = 0;
	superArmorHits = 0;
	hasHitSuperArmor = false;
	hasDamageSuperArmor = false;

	HSObjectHold * returnHold = NULL;

	if(state == STANDING)
	{
		returnHold =  fighterEventHolds.standing;
	}
	else if(state == WALKING)
	{
		returnHold =  fighterEventHolds.walking;
	}
	else if(state == RUNNING)
	{
		returnHold =  fighterEventHolds.running;
	}
	else if(state == CROUCHING)
	{
		returnHold =  fighterEventHolds.crouching;
	}
	else if(state == JUMPING)
	{
		if(vel.y < 0)
		{
			if((facing == LEFT && vel.x > 0) || (facing == RIGHT && vel.x < 0))
			{
				returnHold =  fighterEventHolds.jumpBackwardRising;
			}
			else
			{
				returnHold =  fighterEventHolds.jumpNeutralRising;
			}
		}
		else
		{
			returnHold =  fighterEventHolds.jumpNeutralFalling;
		}
	}
	else if(state == AIR_DASHING)
	{
		if(airDash == FORWARD_AIR_DASH)
		{
			returnHold =  fighterEventHolds.airDashForward;
		}
		else if(airDash == BACKWARD_AIR_DASH)
		{
			returnHold =  fighterEventHolds.airDashBackward;
		}
	}

	if(state == KNOCKOUT_AIR)
	{
		returnHold = fighterEventHolds.hitstunLightAir;
	}
	else if(curHitstun > 0)
	{
		if(state == STANDING || state == WALKING || state == RUNNING)
		{
			state = STANDING;
			if(hitstunHitLevel == HIT_MID)
			{
				returnHold =  fighterEventHolds.hitstunLightMidStanding;
			}
			else if(hitstunHitLevel == HIT_HIGH)
			{
				returnHold =  fighterEventHolds.hitstunLightHighStanding;
			}
			else if(hitstunHitLevel == HIT_LOW)
			{
				returnHold =  fighterEventHolds.hitstunLightLowStanding;
			}
		}
		else if(state == CROUCHING)
		{
			if(hitstunHitLevel == HIT_MID || hitstunHitLevel == HIT_HIGH)
			{
				returnHold =  fighterEventHolds.hitstunLightMidCrouching;
			}
			else
			{
				returnHold =  fighterEventHolds.hitstunLightLowCrouching;
			}
		}
		else if(state == JUMPING)
		{
			returnHold =  fighterEventHolds.hitstunLightAir;
		}
	}
	else if(curBlockstun > 0 || blocking)
	{
		if(state == STANDING || state == WALKING || state == RUNNING)
		{
			state = STANDING;
			returnHold =  fighterEventHolds.blockHigh;
		}
		else if(state == CROUCHING)
		{
			returnHold =  fighterEventHolds.blockLow;
		}
		else if(state == JUMPING || state == AIR_DASHING)
		{
			returnHold =  fighterEventHolds.blockAir;
		}
	}

	if(returnHold == NULL)
	{
		returnHold = fighterEventHolds.standing; //try to default to standing
	}

	if(returnHold == NULL)
	{
		//default to whatever the base class does
		return PhysicsObject::GetDefaultHold();
	}

	return returnHold;
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
	while(curHistory->frame >= releasedFrame - TAP_THRESHOLD && curHistory->frame >= sinceFrame)
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
	while(curHistory->frame >= releasedFrame - TAP_THRESHOLD && curHistory->frame >= sinceFrame)
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
		if(!curHistory->bStickLeft.held && !curHistory->bStickRight.held && !curHistory->bStickUp.held && !curHistory->bStickDown.held)
		{
			return true;
		}
		
		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}

	//check one more time
	if(!curHistory->bStickLeft.held && !curHistory->bStickRight.held && !curHistory->bStickUp.held && !curHistory->bStickDown.held)
	{
		return true;
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
		if(!curHistory->bStickLeft.held && !curHistory->bStickRight.held && !curHistory->bStickUp.held && !curHistory->bStickDown.held)
		{
			return true;
		}
		
		curHistory = curHistory->prevInputState;
		if(curHistory == NULL) { return false; }
	}

	//check one more time
	if(!curHistory->bStickLeft.held && !curHistory->bStickRight.held && !curHistory->bStickUp.held && !curHistory->bStickDown.held)
	{
		return true;
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