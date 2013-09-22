#ifndef __FIGHTER_H_
#define __FIGHTER_H_

#include "physicsobject.h"

#define RUN_INPUT_FRAMES 10 //max number of frames between the two left/right taps that activate a ground run/dash
#define AIR_DASH_INPUT_FRAMES RUN_INPUT_FRAMES //max number of frames between the two left/right taps that activate an air run/dash
#define MOVING_TURN_THRESHOLD 10 //number of frames after walk/run stop that turning will trigger the walking/running turn animation
#define COMMAND_INPUT_THRESHOLD 10 //number of frames allowed between individual direction inputs for a command
#define BUFFERED_ACTION_LIFETIME 10 //number of frames to hold on to buffered inputs
#define TAP_THRESHOLD 10 //number of frames between a press and release of a button to count as a "tap"
#define HARD_PRESS_THRESHOLD 4 //number of frames the stick has to go from neutral to hard pressed to be equivalent to a double-tap
#define WAIT_AFTER_KNOCKOUT 120 //how many frames to wait after a knockout to respawn
#define FIGHTER_OVERLAP_PUSH_ACCEL 5 //how much to alter a fighter's velocity when overlapping another fighter

enum FighterState
{
	STANDING,
	WALKING,
	RUNNING,
	CROUCHING,
	JUMPING,
	AIR_DASHING,
	AIRBORN,
	KNOCKOUT,
	KNOCKOUT_AIR
};

enum FighterFacing
{
	LEFT = -1,
	RIGHT = 1
};

enum FighterJump
{
	NO_JUMP,
	STANDING_JUMP,
	WALKING_JUMP,
	RUNNING_JUMP,
	BACKWARD_JUMP
};

enum FighterAirDash
{
	NO_AIR_DASH,
	FORWARD_AIR_DASH,
	BACKWARD_AIR_DASH
};

enum FighterAirControl
{
	NO_AIR_CONTROL,
	CONTROL_LEFT,
	CONTROL_RIGHT
};

enum FighterAirVelocityCategory
{
	LOW_VELOCITY,
	HIGH_VELOCITY_FORWARD,
	HIGH_VELOCITY_BACKWARD
};

enum FighterAction
{
	NO_ACTION,
	MOVE,
	DASH,
	BACKWARD_DASH,
	CROUCH,
	JUMP,
	NEUTRAL_LIGHT,
	UP_LIGHT,
	DOWN_LIGHT,
	FORWARD_LIGHT,
	BACKWARD_LIGHT,
	QCF_LIGHT,
	NEUTRAL_HEAVY,
	UP_HEAVY,
	DOWN_HEAVY,
	FORWARD_HEAVY,
	BACKWARD_HEAVY,
	QCF_HEAVY
};

enum FighterAttackAction
{
	NO_ATTACK_ACTION,
	GROUND_NEUTRAL_LIGHT,
	GROUND_UP_LIGHT,
	GROUND_DOWN_LIGHT,
	GROUND_FORWARD_LIGHT,
	GROUND_QCF_LIGHT,
	GROUND_NEUTRAL_HEAVY,
	GROUND_UP_HEAVY,
	GROUND_DOWN_HEAVY,
	GROUND_FORWARD_HEAVY,
	GROUND_QCF_HEAVY,
	AIR_NEUTRAL_LIGHT,
	AIR_UP_LIGHT,
	AIR_DOWN_LIGHT,
	AIR_FORWARD_LIGHT,
	AIR_BACKWARD_LIGHT,
	AIR_QCF_LIGHT,
	AIR_NEUTRAL_HEAVY,
	AIR_UP_HEAVY,
	AIR_DOWN_HEAVY,
	AIR_FORWARD_HEAVY,
	AIR_BACKWARD_HEAVY,
	AIR_QCF_HEAVY
};

enum Cancel
{
	ANY_TIME,
	AFTER_HIT_OR_BLOCK,
	AFTER_HIT,
	AFTER_BLOCK,
	NEVER
};

struct Cancels
{
	Cancel dash;
	Cancel jump;

	Cancel lightNeutral;
	Cancel lightForward;
	Cancel lightUp;
	Cancel lightDown;
	Cancel lightBackward;
	Cancel lightQCF;

	Cancel heavyNeutral;
	Cancel heavyForward;
	Cancel heavyUp;
	Cancel heavyDown;
	Cancel heavyBackward;
	Cancel heavyQCF;
};

struct ComboTrack
{
	unsigned int stage;
	unsigned int hits;
	TerrainObject * attacker;
	list<FighterAttackAction> usedAttacks;
};

class FighterHold : public PhysicsObjectHold
{
public:
	//if currently attacking, this determines what other actions can be triggered
	Cancels cancels;

	//whether or not previously set cancels should be overwritten
	bool changeCancels;

	FighterHold();
	~FighterHold();

	virtual bool IsFighterHold();

protected:
};

struct FighterEventHolds// : public TerrainEventHolds
{
	FighterHold * standing;
	FighterHold * turn;
	FighterHold * walk;
	FighterHold * walking;
	FighterHold * walkingTurn;
	FighterHold * run;
	FighterHold * running;
	FighterHold * runningTurn;
	FighterHold * runningStop;
	FighterHold * crouch;
	FighterHold * crouching;
	FighterHold * crouchingTurn;
	FighterHold * stand;
	
	FighterHold * jumpNeutralStart;
	FighterHold * jumpNeutralStartAir;
	FighterHold * jumpNeutralRising;
	FighterHold * jumpNeutralFall;
	FighterHold * jumpNeutralFalling;
	FighterHold * jumpNeutralLand;
	//FighterHold * jumpNeutralLandHard;
	/*FighterHold * jumpForwardStart;
	FighterHold * jumpForwardRising;
	FighterHold * jumpForwardFall;
	FighterHold * jumpForwardFalling;
	FighterHold * jumpForwardLand;
	FighterHold * jumpForwardLandHard;
	FighterHold * jumpBackwardStart;*/
	FighterHold * jumpBackwardRising;
	FighterHold * jumpBackwardFall;
	/*FighterHold * jumpBackwardFalling;
	FighterHold * jumpBackwardLand;
	FighterHold * jumpBackwardLandHard;*/

	FighterHold * airDashForward;
	FighterHold * airDashBackward;

	/*FighterHold * ledgeGrab;
	FighterHold * ledgeClimb;*/

	FighterHold * blockHigh;
	FighterHold * blockLow;
	FighterHold * blockAir;

	//FighterHold * hitstunHighStandingStart;
	FighterHold * hitstunLightHighStanding;
	//FighterHold * hitstunHighStandingEnd;
	FighterHold * hitstunLightMidStanding;
	//FighterHold * hitstunLowStandingStart;
	FighterHold * hitstunLightLowStanding;
	//FighterHold * hitstunLowStandingEnd;
	//FighterHold * hitstunCrouchingStart;
	FighterHold * hitstunLightMidCrouching;
	//FighterHold * hitstunCrouchingEnd;
	FighterHold * hitstunLightLowCrouching;
	FighterHold * hitstunLightAir;
	/*FighterHold * tripForward;
	FighterHold * tripBackward;
	FighterHold * proneFaceUp;
	FighterHold * proneFaceUpStand;
	FighterHold * proneFaceDown;
	FighterHold * proneFaceDownStand;
	FighterHold * crumpleStart;
	FighterHold * crumple;*/

	/*FighterHold * airbornFaceUpRising;
	FighterHold * airbornFaceUpFall;
	FighterHold * airbornFaceUpFalling;
	FighterHold * airbornFaceUpRise;
	FighterHold * airbornFaceDownRising;
	FighterHold * airbornFaceDownFall;
	FighterHold * airbornFaceDownFalling;
	FighterHold * airbornFaceDownRise;
	FighterHold * airbornFaceSideMovingForward;
	FighterHold * airbornFaceSideMoveBackward;
	FighterHold * airbornFaceSideMovingBackward;
	FighterHold * airbornFaceSideMoveForward;*/
    
    FighterHold * attackLightNeutralGround;
    FighterHold * attackLightDownGround;
    FighterHold * attackLightUpGround;
    FighterHold * attackLightForwardGround;
	FighterHold * attackLightQCFGround;
    FighterHold * attackLightNeutralAir;
    FighterHold * attackLightDownAir;
    FighterHold * attackLightUpAir;
    FighterHold * attackLightForwardAir;
    FighterHold * attackLightBackwardAir;
	FighterHold * attackLightQCFAir;
    
    FighterHold * attackHeavyNeutralGround;
    FighterHold * attackHeavyDownGround;
    FighterHold * attackHeavyUpGround;
    FighterHold * attackHeavyForwardGround;
	FighterHold * attackHeavyQCFGround;
    FighterHold * attackHeavyNeutralAir;
    FighterHold * attackHeavyDownAir;
    FighterHold * attackHeavyUpAir;
    FighterHold * attackHeavyForwardAir;
    FighterHold * attackHeavyBackwardAir;
	FighterHold * attackHeavyQCFAir;

	FighterHold * knockout;
};

class Fighter : public PhysicsObject
{
public:
	//the previous terrain hitbox
	HSBox * firstPreviousTerrainBox;
	HSBox * lastPreviousTerrainBox;

	//whether or not the terrain hitbox was just changed
	bool terrainBoxesChanged;

	//special terrain boxes that represent a fighter in an upright state (standing, jumping, etc)
	HSBox * firstUprightTerrainBox;
	HSBox * lastUprightTerrainBox;

	//special terrain boxes that represent a fighter in a crouching state
	HSBox * firstCrouchingTerrainBox;
	HSBox * lastCrouchingTerrainBox;

	//special terrain boxes that represent a fighter in a prone state
	HSBox * firstProneTerrainBox;
	HSBox * lastProneTerrainBox;

	//special terrain boxes that represent a fighter in small compact centralized state
	HSBox * firstCompactTerrainBox;
	HSBox * lastCompactTerrainBox;

	//The fighter's current state
	FighterState state;
	FighterFacing facing;
	bool blocking;
	Blockability hitstunBlockability;
	FighterJump jump;
	FighterAirDash airDash;
	float currentSurfaceFriction;
	bool jumpStartup;
	bool jumpStartupJustEnded;
	bool shortHop;
	int timeSinceWalkStop;
	int timeSinceRunStop;
	bool runStopping;
	bool turning;
	bool sliding;
	bool attacking;
	bool walkAfterTurning;
	bool runAfterTurning;
	FighterAirVelocityCategory airVelocityCategory;
	bool fastFall;
	FighterAirControl airControl;
	FighterAction landingAction;
	FighterAction bufferedAction;
	int bufferedActionAge;
	bool turnUponLanding;
	Cancels cancels;
	bool hitSomething;
	bool wasBlocked;
	HSVectComp fighterPushXAccel;

	Cancels defaultCancels;

	int curHitstun;
	int curBlockstun;

	int framesSinceKnockout;

	FighterAttackAction curAttackAction;
	bool inComboString;
	list<ComboTrack> comboTrack;

	//the holds this object moves to upon particular events
	FighterEventHolds fighterEventHolds;

	//fighter statistics
	float walkSpeed;
	float runSpeed;
	float jumpSpeed;
	float forwardAirDashSpeed;
	float backwardAirDashSpeed;
	int forwardAirDashDuration;
	int backwardAirDashDuration;
	int curAirDashDuration;
	float stepHeight;
	int airActions;
	int curAirActions;
	float airControlAccel;
	float maxAirControlSpeed;

	Fighter();
	~Fighter();
	
	virtual int AdvanceHolds();
	virtual int Event(InputStates * inputHistory, int frame); //handle events
	virtual int Update();
	virtual int CollideTerrain(list<HSObject*> * gameObjects);
	int CollideFighters(list<HSObject*> * gameObjects);
	virtual void ApplyAttackResults();
	virtual void HandleAttackCollision(TerrainObject * targetObject);
	virtual void HandleHurtCollision(TerrainObject * attacker);
	virtual list<SpawnObject*> GetSpawnObjects(); //get any objects this object currently wishes to spawn
	virtual list<AudioInstance*> GetAudio(); //get any audio this object currently wishes to play
	
	virtual bool IsFighter();

	//command inputs
	void GroundAttackExecuted();
	void AirAttackExecuted();
	bool ForwardWasTapped(InputStates * inputHistory, int untilFrame, int sinceFrame);
	bool BackwardWasTapped(InputStates * inputHistory, int untilFrame, int sinceFrame);
	bool ForwardPressed(InputStates * inputHistory, int untilFrame, int sinceFrame);
	bool BackwardPressed(InputStates * inputHistory, int untilFrame, int sinceFrame);
	bool DownPressed(InputStates * inputHistory, int untilFrame, int sinceFrame);
	bool ForwardHeld(InputStates * inputHistory, int targetFrame);
	bool BackwardHeld(InputStates * inputHistory, int targetFrame);
	bool DownHeld(InputStates * inputHistory, int targetFrame);
	bool ForwardReleased(InputStates * inputHistory, int untilFrame, int sinceFrame);
	bool BackwardReleased(InputStates * inputHistory, int untilFrame, int sinceFrame);
	bool DownReleased(InputStates * inputHistory, int untilFrame, int sinceFrame);
	bool ForwardHardPressed(InputStates * inputHistory, int untilFrame, int sinceFrame);
	bool BackwardHardPressed(InputStates * inputHistory, int untilFrame, int sinceFrame);
	bool QCFInput(InputStates * inputHistory, int untilFrame, int sinceFrame);
	
	virtual bool AdvanceHold(HSObjectHold * hold);
	virtual bool ChangeHold(HSObjectHold * hold);

protected:
	void ChangeTerrainBoxes(HSBox * newFirstTerrainBox);
	virtual HSObjectHold * GetDefaultHold();
	
	virtual void HandleChangeInTerrainBoxes(list<HSObject*> * gameObjects);
	TerrainCollisionResult IsCollidingWithAnyFighter(list<HSObject*> * gameObjects);
	void HandleFighterCollison(TerrainObject * targetObject);
	virtual int HandleTerrainCollision(list<HSObject*> * gameObjects, TerrainCollisionResult * result, HSVect2D * ownPos = NULL, HSVect2D * ownPrevPos = NULL);
	virtual int StepCheck(list<HSObject*> * gameObjects);
	virtual int HandleJumpLanding();

	int ExecuteAction(InputStates * inputHistory, int frame);

	bool CanDashCancel();
	bool CanJumpCancel();
	bool CanLightNeutralCancel();
	bool CanLightForwardCancel();
	bool CanLightUpCancel();
	bool CanLightDownCancel();
	bool CanLightBackwardCancel();
	bool CanLightQCFCancel();
	bool CanHeavyNeutralCancel();
	bool CanHeavyForwardCancel();
	bool CanHeavyUpCancel();
	bool CanHeavyDownCancel();
	bool CanHeavyBackwardCancel();
	bool CanHeavyQCFCancel();
};

#endif