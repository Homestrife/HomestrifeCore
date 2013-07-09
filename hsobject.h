#ifndef __HSOBJECT_H_
#define __HSOBJECT_H_

//this only works for windows!
#include <direct.h>
#define GetCurrentWorkingDirectory _getcwd

#include <list>
#include "texture.h"
#include "audio.h"
#include "tinyxml2.h"
using namespace tinyxml2;
using std::list;

list<string> GetStringParts(list<string> parts, string toSplit, string splitToken);
list<string> SplitString(string toSplit, string splitToken);
string CreateAbsolutePath(string baseDirectory, string relPath);

struct TextureInstance
{
	HSTexture * hsTex;
	HSVect2D offset; //offset from the position of the object that owns this texture
	int depth; //how far in front or back it should be drawn
	float hScale; //horizontal scaling
	float vScale; //vertical scaling
};

struct AudioInstance
{
	HSAudio * hsAud;
	int delay; //number of frames to wait before actually playing it
	bool exclusive; //whether or not it should cut off or be cut off by other exclusive sounds triggered by this object
	bool usePercentage; //whether or not the sound will be involved in chance-to-play calculations
	int percentage; //chance sound will play, if usePercentage is true
};

struct InputState
{
	bool pressed;
	bool held;
	bool released;
};

struct InputStates
{
	//every bool array signifies if an input is pressed, held, or released in the current frame, in that order
	//keyboard input states
	InputState bKeyUp;
	InputState bKeyDown;
	InputState bKeyLeft;
	InputState bKeyRight;
	InputState bKeyJump;
	InputState bKeyBlock;
	InputState bKeyLight;
	InputState bKeyHeavy;
	InputState bKeyStart;
	InputState bKeySelect;
	InputState keyMenuConfirm;
	InputState keyMenuBack;

	//joystic button input states
	InputState bButtonUp;
	InputState bButtonDown;
	InputState bButtonLeft;
	InputState bButtonRight;
	InputState bButtonJump;
	InputState bButtonBlock;
	InputState bButtonLight;
	InputState bButtonHeavy;
	InputState bButtonStart;
	InputState bButtonSelect;
	InputState buttonMenuConfirm;
	InputState buttonMenuBack;

	//stick input states
	InputState bStickUp;
	InputState bStickDown;
	InputState bStickLeft;
	InputState bStickRight;
	InputState bStickHardUp;
	InputState bStickHardDown;
	InputState bStickHardLeft;
	InputState bStickHardRight;

	//hat input states
	InputState bHatUp;
	InputState bHatDown;
	InputState bHatLeft;
	InputState bHatRight;

	//previous input states
	InputStates * prevInputState;

	//frame in which this input state was created
	int frame;
};

struct HSBox //a rectangle hitbox
{
	HSBox * nextBox; //next box in this list of boxes
	float height;
	float width;
	HSVect2D offset; //offset from the position of the object that owns this box
	bool isTriangle; //whether or not this box is a triangle
	bool rightAlign; //whether or not the triangle's right angle is to the right
	bool bottomAlign; //wheather or not the traingle's right angle is at the top
};

class HSObject;

struct SpawnObject
{
	HSObject * object; //object that will be spawned
	bool followParent; //whether or not it should follow the parent's movements
	HSVect2D parentOffset; //initial location in reference to the parent
	HSVect2D vel; //initial velocity of the object
	string defFilePath; //path to the file that defines this
	int delay; //frames to wait before actually spawning the object
	bool collideParent; //whether or not it should collide with its parent
	int number; //how many to spawn
};

int DefineBox(XMLElement * definition, HSBox * newBox);

class HSObjectHold //a single segment of an animation. can actually last for multiple frames
{
public:
	unsigned int id; //the id number of the hold
	unsigned int nextHoldId; //the id number of the intended nextHold
	list<TextureInstance> textures; //textures in this hold
	list<AudioInstance> audioList; //sounds in this hold 
	HSObjectHold * nextHold; //the hold to change to when this one's duration is reached
	HSObjectHold * nextListHold; //the next hold in the list of holds
	list<SpawnObject> spawnObjects; //objects that will be spawned on this frame

	unsigned int duration; //how many frames this hold lasts (based on 60fps)

	HSObjectHold();
	~HSObjectHold();

	virtual int Define(XMLElement * definition, string defFileDirectory, list<HSTexture*> * textureRegistry, list<HSAudio*> * audioRegistry, SDL_AudioSpec * obtainedAudioSpec, bool openGL3, bool useTGAPalettes); //initialize this hold based on definition file info
	HSObjectHold * Clone(); //return a copy of this hold

	virtual bool IsTerrainObjectHold();
	virtual bool IsPhysicsObjectHold();
	virtual bool IsFighterHold();

protected:
	int AddTexture(XMLElement * texture, string defFileDirectory, list<HSTexture*> * textureRegistry, bool openGL3, bool useTGAPalettes); //add a texture to the list of textures
	int AddAudio(XMLElement * audio, string defFileDirectory, list<HSAudio*> * audioRegistry, SDL_AudioSpec * obtainedAudioSpec); //add a piece of audio to be played with the hold
	int AddSpawnObject(XMLElement * spawnObject, string defFileDirectory, list<HSTexture*> * textureRegistry, list<HSAudio*> * audioRegistry, SDL_AudioSpec * obtainedAudioSpec); //add an object to the list of spawn objects
	virtual HSObjectHold * CreateHoldOfSameType();
	virtual void CopyAttributes(HSObjectHold * target);
};

struct HSObjectEventHolds
{
	HSObjectHold * lifetimeDeath;
};

class HSObject
{
public:
	unsigned int id; //the object's id. Every object should have a unique id
	string definitionFilePath; //path to the object's definition file
	string definitionFileDirectory; //path to the directory containing the definition file

	//int player; //player currently in control of this object. -1 means no player controls this
	
	list<HSPalette*> palettes; //all of the object's palettes
	HSPalette * palette; //current palette
	bool useTGAPalettes; //whether or not to use the palettes within individual tga files
	HSObjectHold * firstHold; //the first of a list of all of the object's animation holds
	HSObjectHold * lastHold; //the last of the list
	HSObjectHold * curHold; //the object's current animation hold

	HSObject * parent; //the object that created this, if any
	bool followParent; //whether or not this should follow its parent's movements

	bool hFlip; //whether or not this object's textures should be flipped horizontally about the current position
	bool visible; //whether or not the object is visible

	unsigned int lifetime; //how long the object exists. 0 means it exists forever
	unsigned int time; //how long the object has existed
	unsigned int holdTime; //how long since curHold has been changed
	bool toDelete; //whether or not the object needs to be removed from the game
	HSVect2D pos; //the object's current position
	HSVect2D prevPos; //the object's previous position
	HSVect2D vel; //the object's velocity
	HSVect2D prevVel; //the object's previous velocity

	//the holds this object moves to upon particular events
	HSObjectEventHolds hsObjectEventHolds;

	bool objectsSpawned;
	bool audioPlayed;

	HSObject();
	~HSObject();

	int LoadPalettes(list<HSPalette*> * paletteRegistry);
	virtual int Define(XMLElement * definition, string defFileDirectory, list<HSTexture*> * textureRegistry, list<HSPalette*> * paletteRegistry, list<HSAudio*> * audioRegistry, SDL_AudioSpec * obtainedAudioSpec, bool openGL3); //initialize this object based on definition file info
	HSObject * Clone(); //return a copy of this object

	void PrevPalette();
	void NextPalette();
	void SetPalette(int index);
	int GetPalette();

	virtual int AdvanceHolds();
	virtual int Event(InputStates * inputHistory, int frame); //handle events
	virtual int Update(); //handle updates
	virtual int CollideTerrain(list<HSObject*> * gameObjects); //handle collisions between terrain boxes
	virtual int CollideAttack(list<HSObject*> * gameObjects); //handle collisions between attack and hurt boxes
	virtual void ApplyAttackResults(); //apply the results of the attack collision step
	virtual void HandleHurtCollision(HSObject * attacker); //react to another object's attack
	virtual list<SpawnObject*> GetSpawnObjects(); //get any objects this object currently wishes to spawn
	virtual list<AudioInstance*> GetAudio(); //get any audio this object currently wishes to play
	
	virtual bool IsHUD();
	virtual bool IsTerrain();
	virtual bool IsTerrainObject();
	virtual bool IsPhysicsObject();
	virtual bool IsFighter();
	
	virtual bool ChangeHold(HSObjectHold * hold); //change to a new hold

protected:
	virtual HSObjectHold * CreateNewHold();
	virtual HSObject * CreateObjectOfSameType();
	virtual void CopyAttributes(HSObject * target);
	virtual void CopyEventHold(HSObject * target, HSObjectHold * targetHold);
	virtual int SaveEventHolds(HSObjectHold * hold, XMLElement * eventHolds);
	virtual int AddHold(HSObjectHold * newHold); //add a new hold to the list of holds
	int LinkHolds(); //sets the appropriate nextHold pointers for each hold based on its nextHoldId
	virtual HSObjectHold * GetDefaultHold(); //gets a default hold, based on current states
};

#endif