#ifndef __PHYSICSOBJECT_H_
#define __PHYSICSOBJECT_H_

#include <cmath>
#include "terrainobject.h"

#define GRAVITY_ACC 4
#define MIN_VEL 0.5

struct TerrainCollisionResult
{
	HSBox * lastSelfBox;
	HSBox * lastTargetBox;
	TerrainObject * lastTargetObject;
	bool horizontalImpact; //signifies an impact during horizontal motion
	bool verticalImpact; //signifies an impact during vertical motion
	HSVect2D hypCollisionPoint;
};

class PhysicsObjectHold : public TerrainObjectHold
{
public:
	PhysicsObjectHold();
	~PhysicsObjectHold();

	virtual bool IsPhysicsObjectHold();

protected:
};

class PhysicsObject : public TerrainObject
{
public:
	//the complete mass of this object
	HSVectComp mass;

	//whether or not this object is affected by gravity
	bool falls;
	float maxFallSpeed;

	//whether or not this object falls through jump-through terrain when its falling downward
	bool ignoreJumpThroughTerrain;

	PhysicsObject();
	~PhysicsObject();
	
	virtual int Event(InputStates * inputHistory, int frame); //handle events
	virtual int Update();
	virtual int CollideTerrain(list<HSObject*> * gameObjects);
	
	virtual bool IsTerrain();
	virtual bool IsTerrainObject();
	virtual bool IsPhysicsObject();

protected:
	//Gets a corner point from a triangle
	HSVect2D GetLeftHypotenusePoint(HSVect2D * boxPos, HSBox * box);
	HSVect2D GetRightHypotenusePoint(HSVect2D * boxPos, HSBox * box);
	HSVect2D GetRightAnglePoint(HSVect2D * boxPos, HSBox * box);

	//finds the intersection point of two lines
	HSVect2D GetIntersectionPoint(HSVect2D * lineOnePointA, HSVect2D * lineOnePointB, HSVect2D * lineTwoPointA, HSVect2D * lineTwoPointB);

	//gets the difference of two intervals found by ProjectBoxAgainstLine()
	float IntervalDifference(float * minA, float * maxA, float * minB, float * maxB);

	//gets the projection interval of a line segment against a line
	void ProjectLineSegmentAgainstLine(HSVect2D * pointA, HSVect2D * pointB, HSVect2D * targetLine, float * min, float * max);

	//gets the projection interval of a box against a line
	void ProjectBoxAgainstLine(HSVect2D * boxPos, HSBox * box, HSVect2D * targetLine, float * min, float * max);

	//checks if one box is colliding with a triangular box's hypotenuse, assuming we know it might
	TerrainCollisionResult IsBoxCollidingWithHypotenuse(HSVect2D * ownPos, HSBox * ownBox, TerrainObject * targetObject, HSBox * targetBox, bool movingAway = false);

	//checkes whether the two given terrain boxes are overlapping or not. assumes the first box is a rectangle
	TerrainCollisionResult AreTerrainBoxesColliding(HSBox * ownBox, TerrainObject * targetObject, HSBox * targetBox, TerrainCollisionResult * prevResult = NULL, HSVect2D * ownPos = NULL, HSVect2D * ownPrevPos = NULL, bool ignoreCurrentlyOverlapping = true);

	//check this object against a particular object's terrain boxes.
	virtual TerrainCollisionResult IsCollidingWithObjectTerrain(TerrainObject * targetObject, TerrainCollisionResult * prevResult = NULL, HSVect2D * ownPos = NULL, HSVect2D * ownPrevPos = NULL, bool ignoreCurrentlyOverlapping = true);

	//check this object against all terrain.
	virtual TerrainCollisionResult IsCollidingWithAnyTerrain(list<HSObject*> * gameObjects, TerrainCollisionResult * prevResult = NULL, HSVect2D * ownPos = NULL, HSVect2D * ownPrevPos = NULL);

	//adjust the position of this object if it is stuck in terrain
	void MoveOutOfTerrain(list<HSObject*> * gameObjects, HSVect2D * ownPrevPos = NULL);

	//adjust the position of this object if it collides with something
	HSVect2D CollisionReposition(list<HSObject*> * gameObjects, TerrainCollisionResult * result, HSVect2D * ownPos = NULL, HSVect2D * ownPrevPos = NULL);

	//adjust velocity and apply collision damage based on the properties of collided objects
	void CollisionPhysics(HSVect2D * newPos, TerrainCollisionResult * result, HSVect2D * ownPos = NULL, HSVect2D * ownPrevPos = NULL);

	//represensts a more thorough collision detection method for placing this object's terrain boxes
	//just outside of other terrain boxes
	virtual int HandleTerrainCollision(list<HSObject*> * gameObjects, TerrainCollisionResult * result, HSVect2D * ownPos = NULL, HSVect2D * ownPrevPos = NULL);
};

#endif