#include "physicsobject.h"

float gravity = 4;

/////////////////////
//PhysicsObjectHold//
/////////////////////

PhysicsObjectHold::PhysicsObjectHold() : TerrainObjectHold()
{
	changePhysicsAttributes = false;

	ignoreGravity = false;
}

PhysicsObjectHold::~PhysicsObjectHold()
{
}

bool PhysicsObjectHold::IsPhysicsObjectHold()
{
	return true;
}

/////////////////
//PhysicsObject//
/////////////////

PhysicsObject::PhysicsObject() : TerrainObject()
{
	health = 0;
	curHealth = 0;
	mass = 0;
	falls = true;
	ignoreGravity = false;
	maxFallSpeed = 100;
	ignoreJumpThroughTerrain = false;
	takesTerrainDamage = false;

	blockability = MID;
	horizontalDirectionBasedBlock = false;
	reversedHorizontalBlock = false;
	damage = 0;
	hitstun = 0;
	blockstun = 0;
	force.x = 0;		force.y = 0;
	trips = false;
}

PhysicsObject::~PhysicsObject()
{
}

int PhysicsObject::Event(InputStates * inputHistory, int frame)
{
	return 0;
}

int PhysicsObject::Update()
{
	if(int error = TerrainObject::Update() != 0) { return error; } //there was an error in the base update

	//apply gravity
	if(falls && !ignoreGravity)
	{
		vel.y += gravity;
		if(vel.y > maxFallSpeed)
		{
			vel.y = maxFallSpeed;
		}
	}

	return 0;
}

bool PhysicsObject::AdvanceHold(HSObjectHold * hold)
{
	if(TerrainObject::AdvanceHold(hold))
	{
		PhysicsObjectHold * phHold = (PhysicsObjectHold*)curHold;

		if(phHold->changePhysicsAttributes)
		{
			ignoreGravity = phHold->ignoreGravity;
		}

		return true;
	}

	return false;
}

bool PhysicsObject::ChangeHold(HSObjectHold * hold)
{
	ignoreGravity = false;

	return TerrainObject::ChangeHold(hold);
}

HSObjectHold * PhysicsObject::GetDefaultHold()
{
	ignoreGravity = false;

	return TerrainObject::GetDefaultHold();
}

HSVect2D PhysicsObject::GetLeftHypotenusePoint(HSVect2D * boxPos, HSBox * box)
{
	HSVect2D point;
	point.x = 0;	point.y = 0;

	if(boxPos == NULL || box == NULL) { return point; } //no position or box given

	point.x = boxPos->x + box->offset.x;
	point.y = boxPos->y + box->offset.y;
	if((!box->rightAlign && !box->bottomAlign) || (box->rightAlign && box->bottomAlign))
	{
		point.y += box->height;
	}

	return point;
}

HSVect2D PhysicsObject::GetRightHypotenusePoint(HSVect2D * boxPos, HSBox * box)
{
	HSVect2D point;
	point.x = 0;	point.y = 0;

	if(boxPos == NULL || box == NULL) { return point; } //no position or box given

	point.x = boxPos->x + box->offset.x + box->width;
	point.y = boxPos->y + box->offset.y;
	if((!box->rightAlign && box->bottomAlign) || (box->rightAlign && !box->bottomAlign))
	{
		point.y += box->height;
	}

	return point;
}

HSVect2D PhysicsObject::GetRightAnglePoint(HSVect2D * boxPos, HSBox * box)
{
	HSVect2D point;
	point.x = 0;	point.y = 0;

	if(boxPos == NULL || box == NULL) { return point; } //no position or box given

	point.x = boxPos->x + box->offset.x;
	if(box->rightAlign) { point.x += box->width; }
	point.y = boxPos->y + box->offset.y;
	if(box->bottomAlign) { point.y += box->height; }

	return point;
}

HSVect2D PhysicsObject::GetIntersectionPoint(HSVect2D * lineOnePointA, HSVect2D * lineOnePointB, HSVect2D * lineTwoPointA, HSVect2D * lineTwoPointB)
{
	HSVect2D intersectionPoint = *lineOnePointA;

	if(lineOnePointA == NULL || lineOnePointB == NULL || lineTwoPointA == NULL || lineTwoPointB == NULL) { return intersectionPoint; }

	float uDenom = ((lineTwoPointB->y - lineTwoPointA->y) * (lineOnePointB->x - lineOnePointA->x)) - ((lineTwoPointB->x - lineTwoPointA->x) * (lineOnePointB->y - lineOnePointA->y));

	if(uDenom == 0) { return intersectionPoint; } //lines are parallel

	float uA = (((lineTwoPointB->x - lineTwoPointA->x) * (lineOnePointA->y - lineTwoPointA->y)) - ((lineTwoPointB->y - lineTwoPointA->y) * (lineOnePointA->x - lineTwoPointA->x))) / uDenom;
	//float uB = (((lineOnePointB->x - lineOnePointA->x) * (lineOnePointA->y - lineTwoPointA->y)) - ((lineOnePointB->y - lineOnePointA->y) * (lineOnePointA->x - lineTwoPointA->x))) / uDenom;

	intersectionPoint.x = lineOnePointA->x + uA * (lineOnePointB->x - lineOnePointA->x);
	intersectionPoint.y = lineOnePointA->y + uA * (lineOnePointB->y - lineOnePointA->y);

	return intersectionPoint;
}

float PhysicsObject::IntervalDifference(float * minA, float * maxA, float * minB, float * maxB)
{
	if(minA == NULL || minB == NULL || maxA == NULL || maxB == NULL) { return 0; }

	if (*minA < *minB) { return *minB - *maxA; }
	else { return *minA - *maxB; }
}

void PhysicsObject::ProjectLineSegmentAgainstLine(HSVect2D * pointA, HSVect2D * pointB, HSVect2D * targetLine, float * min, float * max)
{
	if(pointA == NULL || pointB == NULL || targetLine == NULL || min == NULL || max == NULL) { return; }
	
	//get the dot product of the line and the first point
	float dotProd = (targetLine->x * pointA->x) + (targetLine->y * pointA->y);
	*min = dotProd;
	*max = dotProd;

	//do the same with the second point
	dotProd = (targetLine->x * pointB->x) + (targetLine->y * pointB->y);
	if(dotProd < *min) { *min = dotProd; }
	if(dotProd > *max) { *max = dotProd; }
}

void PhysicsObject::ProjectBoxAgainstLine(HSVect2D * boxPos, HSBox * box, HSVect2D * targetLine, float * min, float * max)
{
	if(boxPos == NULL || box == NULL || targetLine == NULL || min == NULL || max == NULL) { return; }

	if(box->isTriangle)
	{
		//get the three points
		HSVect2D rightAng, leftHyp, rightHyp;
		rightAng = GetRightAnglePoint(boxPos, box);
		leftHyp = GetLeftHypotenusePoint(boxPos, box);
		rightHyp = GetRightHypotenusePoint(boxPos, box);
		
		//get the dot product of the line and the first point
		float dotProd = (targetLine->x * rightAng.x) + (targetLine->y * rightAng.y);
		*min = dotProd;
		*max = dotProd;

		//do the same with the second point
		dotProd = (targetLine->x * leftHyp.x) + (targetLine->y * leftHyp.y);
		if(dotProd < *min) { *min = dotProd; }
		if(dotProd > *max) { *max = dotProd; }

		//do the same with the third point
		dotProd = (targetLine->x * rightHyp.x) + (targetLine->y * rightHyp.y);
		if(dotProd < *min) { *min = dotProd; }
		if(dotProd > *max) { *max = dotProd; }
	}
	else
	{
		//get the four points
		HSVect2D pointA, pointB, pointC, pointD;
		pointA.x = pos.x + box->offset.x;
		pointA.y = pos.y + box->offset.y;
		pointB.x = pointA.x + box->width;
		pointB.y = pointA.y;
		pointC.x = pointA.x;
		pointC.y = pointA.y + box->height;
		pointD.x = pointB.x;
		pointD.y = pointC.y;

		//get the dot product of the line and the first point
		float dotProd = (targetLine->x * pointA.x) + (targetLine->y * pointA.y);
		*min = dotProd;
		*max = dotProd;

		//do the same with the second point
		dotProd = (targetLine->x * pointB.x) + (targetLine->y * pointB.y);
		if(dotProd < *min) { *min = dotProd; }
		if(dotProd > *max) { *max = dotProd; }

		//do the same with the third point
		dotProd = (targetLine->x * pointC.x) + (targetLine->y * pointB.y);
		if(dotProd < *min) { *min = dotProd; }
		if(dotProd > *max) { *max = dotProd; }

		//do the same with the fourth point
		dotProd = (targetLine->x * pointD.x) + (targetLine->y * pointB.y);
		if(dotProd < *min) { *min = dotProd; }
		if(dotProd > *max) { *max = dotProd; }
	}
}

TerrainCollisionResult PhysicsObject::IsBoxCollidingWithHypotenuse(HSVect2D * ownPos, HSBox * ownBox, TerrainObject * targetObject, HSBox * targetBox, bool movingAway)
{
	TerrainCollisionResult result;
	result.lastSelfBox = NULL;
	result.lastTargetBox = NULL;
	result.lastTargetObject = NULL;
	result.horizontalImpact = false;
	result.verticalImpact = false;

	if(ownPos == NULL || ownBox == NULL || targetObject == NULL || targetBox == NULL) { return result; }
	
	//get own box's point that is threatening to collide with the hypotenuse, at both current and previous position
	HSVect2D ownBoxPoint;
	HSVect2D ownBoxPrevPoint;
	HSVect2D ownBoxFurthestPoint;
	ownBoxPoint.x = ownPos->x + ownBox->offset.x;
	ownBoxPrevPoint.x = prevPos.x + ownBox->offset.x;
	ownBoxFurthestPoint.x = pos.x + ownBox->offset.x;
	if(targetBox->rightAlign)
	{
		ownBoxPoint.x += ownBox->width;
		ownBoxPrevPoint.x += ownBox->width;
		ownBoxFurthestPoint.x += ownBox->width;
	}
	ownBoxPoint.y = ownPos->y + ownBox->offset.y;
	ownBoxPrevPoint.y = prevPos.y + ownBox->offset.y;
	ownBoxFurthestPoint.y = pos.y + ownBox->offset.y;
	if(targetBox->bottomAlign)
	{
		ownBoxPoint.y += ownBox->height;
		ownBoxPrevPoint.y += ownBox->height;
		ownBoxFurthestPoint.y += ownBox->height;
	}
	
	//get the target box's hypotenuse points
	HSVect2D leftHyp = GetLeftHypotenusePoint(&(targetObject->pos), targetBox);
	HSVect2D rightHyp = GetRightHypotenusePoint(&(targetObject->pos), targetBox);

	////compare ownBoxPoint to the collision point
	//float distSqrd = pow(ownBoxPoint.x - collisionPoint.x , 2) + pow(ownBoxPoint.y - collisionPoint.y, 2);
	//if(distSqrd < pow(MIN_VEL, 2)) { ownBoxPoint = collisionPoint; } //they're practically the same, so make them the same

	////compare ownBoxPrevPoint to the collision point
	//distSqrd = pow(ownBoxPrevPoint.x - collisionPoint.x , 2) + pow(ownBoxPrevPoint.y - collisionPoint.y, 2);
	//if(distSqrd < pow(MIN_VEL, 2)) { ownBoxPrevPoint = collisionPoint; } //they're practically the same, so make them the same

	if(movingAway)
	{
		float curPointDist;
		float prevPointDist;
		curPointDist = ((rightHyp.x - leftHyp.x) * (ownBoxPoint.y - leftHyp.y)) - ((rightHyp.y - leftHyp.y) * (ownBoxPoint.x - leftHyp.x));
		prevPointDist = ((rightHyp.x - leftHyp.x) * (ownBoxPrevPoint.y - leftHyp.y)) - ((rightHyp.y - leftHyp.y) * (ownBoxPrevPoint.x - leftHyp.x));

		//if a point is very close to the line, just treat it like it's on top of it
		if(curPointDist < MIN_VEL) { curPointDist = 0; }
		if(prevPointDist < MIN_VEL) { prevPointDist = 0; }

		if((targetBox->bottomAlign && (curPointDist > 0 || prevPointDist > 0)) || (!targetBox->bottomAlign && (curPointDist < 0 || prevPointDist < 0)))
		{
			//this counts as a collision
			result.lastSelfBox = ownBox;
			result.lastTargetBox = targetBox;
			result.lastTargetObject = targetObject;
			result.horizontalImpact = true;
			result.verticalImpact = true;
			return result;
		}
	}
	else
	{
		//get the collision point
		HSVect2D collisionPoint = GetIntersectionPoint(&ownBoxFurthestPoint, &ownBoxPrevPoint, &leftHyp, &rightHyp);

		if(collisionPoint.x == ownBoxPoint.x && collisionPoint.y == ownBoxPoint.y)
		{
			//collision happened right at an own box point
			//this doesn't count as a collision
			return result;
		}

		//float curPointDist = ((rightHyp.x - leftHyp.x) * (ownBoxPoint.y - leftHyp.y)) - ((rightHyp.y - leftHyp.y) * (ownBoxPoint.x - leftHyp.x));

		if(//((targetBox->bottomAlign && curPointDist > 0) || (!targetBox->bottomAlign && curPointDist < 0)) &&
			(((ownBoxPoint.x <= ownBoxPrevPoint.x && collisionPoint.x >= ownBoxPoint.x && collisionPoint.x <= ownBoxPrevPoint.x) ||
			(ownBoxPoint.x >= ownBoxPrevPoint.x && collisionPoint.x <= ownBoxPoint.x && collisionPoint.x >= ownBoxPrevPoint.x)) &&
			((ownBoxPoint.y <= ownBoxPrevPoint.y && collisionPoint.y >= ownBoxPoint.y && collisionPoint.y <= ownBoxPrevPoint.y) ||
			(ownBoxPoint.y >= ownBoxPrevPoint.y && collisionPoint.y <= ownBoxPoint.y && collisionPoint.y >= ownBoxPrevPoint.y))))
		{
			//collision point lies somewhere on the line described by own box's two points
			result.lastSelfBox = ownBox;
			result.lastTargetBox = targetBox;
			result.lastTargetObject = targetObject;
			result.horizontalImpact = true;
			result.verticalImpact = true;
			result.hypCollisionPoint = collisionPoint;
			return result;
		}
	}

	//no valid intersection
	return result;
}

TerrainCollisionResult PhysicsObject::AreTerrainBoxesColliding(HSBox * ownBox, TerrainObject * targetObject, HSBox * targetBox, TerrainCollisionResult * prevResult, HSVect2D * ownPos, HSVect2D * ownPrevPos, bool ignoreCurrentlyOverlapping)
{
	TerrainCollisionResult result;
	result.lastSelfBox = NULL;
	result.lastTargetBox = NULL;
	result.lastTargetObject = NULL;
	result.horizontalImpact = false;
	result.verticalImpact = false;

	if(ownBox == NULL || targetBox == NULL || targetBox == NULL) { return result; } //a position or box is not given

	if(targetObject->canBeJumpedThrough && ignoreJumpThroughTerrain)
	{
		//this is jump-through terrain, and this object is currently ignoring it. ignore it
		return result;
	}

	if(prevResult != NULL && ownBox == prevResult->lastSelfBox && targetBox == prevResult->lastTargetBox)
	{
		//this collision has already been handled. Skip it
	}

	if(ownPos == NULL) { ownPos = &pos; }
	if(ownPrevPos == NULL) { ownPrevPos = &prevPos; }

	//get own box's change in position
	HSVect2D posChange;
	posChange.x = ownPos->x - ownPrevPos->x;
	posChange.y = ownPos->y - ownPrevPos->y;

	//no change in position, so don't even bother
	/*if(posChange.x == 0 && posChange.y == 0)
	{
		return result;
	}*/

	//check if own box at the starting position is overlapping the target box. if so, ignore this collision.
	//this is mostly for jump-through platforms, but also makes for an "easy out" in the event that another object
	//gets stuck in terrain somehow.
	if(ignoreCurrentlyOverlapping && AreRectanglesColliding(&prevPos, ownBox, &(targetObject->pos), targetBox))
	{
		return result;
	}

	//get the total bounding box of both positions
	HSBox totalBox;
	//get its width and height
	totalBox.width = ownBox->width + abs(posChange.x);
	totalBox.height = ownBox->height + abs(posChange.y);
	//get its offset in relation to the current position
	if(posChange.x < 0) { totalBox.offset.x = ownBox->offset.x; }
	else { totalBox.offset.x = ownBox->offset.x - abs(posChange.x); }
	if(posChange.y < 0) { totalBox.offset.y = ownBox->offset.y; }
	else { totalBox.offset.y = ownBox->offset.y - abs(posChange.y); }

	//see if the target box's rectangle falls anywhere in the total bounding box
	if(!AreRectanglesColliding(ownPos, &totalBox, &(targetObject->pos), targetBox))
	{
		//the target box definitely isn't colliding with the total box at any point
		return result;
	}

	//target box falls within the total bounding box
	if(posChange.x == 0 || posChange.y == 0)
	{
		//ownBox only moved on one axis, so there was a collision between rectangles at some point.
		if(posChange.x != 0)
		{
			//if the box can be jumped through then horizontal collisions don't count
			if(targetObject->canBeJumpedThrough)
			{
				return result;
			}

			if(!targetBox->isTriangle || (posChange.x < 0 && targetBox->rightAlign) || (posChange.x > 0 && !targetBox->rightAlign) ||
				(targetBox->bottomAlign && ownPos->y + ownBox->offset.y + ownBox->height > targetObject->pos.y + targetBox->offset.y + targetBox->height) ||
				(!targetBox->bottomAlign && ownPos->y + ownBox->offset.y < targetObject->pos.y + targetBox->offset.y))
			{
				if(targetBox->isTriangle && ((posChange.x < 0 && targetBox->rightAlign) || (posChange.x > 0 && !targetBox->rightAlign)))
				{
					//own box could be moving directly away from the hypotenuse. check for this.
					TerrainCollisionResult testResult = IsBoxCollidingWithHypotenuse(ownPos, ownBox, targetObject, targetBox, true);
					if(!testResult.horizontalImpact)
					{
						//box is moving away from the hypotenuse, so ignore it
						return result;
					}
				}

				//own box is impacting a vertical edge or a hypotenuse point on the target box
				result.horizontalImpact = true;
				result.lastSelfBox = ownBox;
				result.lastTargetBox = targetBox;
				result.lastTargetObject = targetObject;
				return result;
			}
			else
			{
				//own box is potentially impacting the hypotenuse of the target box.
				return IsBoxCollidingWithHypotenuse(ownPos, ownBox, targetObject, targetBox);
			}
		}
		else if(posChange.y != 0)
		{
			//if the target box is jump-through, then upward moving collisions don't count
			if(targetObject->canBeJumpedThrough && posChange.y <= 0)
			{
				return result;
			}

			if(!targetBox->isTriangle || (posChange.y < 0 && targetBox->bottomAlign) || (posChange.y > 0 && !targetBox->bottomAlign) ||
				(targetBox->rightAlign && ownPos->x + ownBox->offset.x + ownBox->width > targetObject->pos.x + targetBox->offset.x + targetBox->width) ||
				(!targetBox->rightAlign && ownPos->x + ownBox->offset.x < targetObject->pos.x + targetBox->offset.x))
			{
				if(targetBox->isTriangle && ((posChange.y < 0 && targetBox->bottomAlign) || (posChange.y > 0 && !targetBox->bottomAlign)))
				{
					//own box could be moving directly away from the hypotenuse. check for this.
					TerrainCollisionResult testResult = IsBoxCollidingWithHypotenuse(ownPos, ownBox, targetObject, targetBox, true);
					if(!testResult.horizontalImpact)
					{
						//box is moving away from the hypotenuse, so ignore it
						return result;
					}
				}

				//own box is impacting a horizontal edge or a hypotenuse point on the target box
				result.verticalImpact = true;
				result.lastSelfBox = ownBox;
				result.lastTargetBox = targetBox;
				result.lastTargetObject = targetObject;
				return result;
			}
			else
			{
				//own box is potentially impacting the hypotenuse of the target box.
				return IsBoxCollidingWithHypotenuse(ownPos, ownBox, targetObject, targetBox);
			}
		}
		else if(posChange.x == 0 && posChange.y == 0)
		{
			if(AreRectanglesColliding(&prevPos, ownBox, &(targetObject->pos), targetBox))
			{
				result.lastSelfBox = ownBox;
				result.lastTargetBox = targetBox;
				result.lastTargetObject = targetObject;
				return result;
			}
		}
	}

	//own box moves on both axes.
	//get the potential collision timeframe
	HSVect2D tMin, tMax;
	if(posChange.x < 0)
	{
		tMin.x = ((targetObject->pos.x + targetBox->offset.x + targetBox->width) - (ownPos->x + ownBox->offset.x)) / posChange.x;
		tMax.x = ((targetObject->pos.x + targetBox->offset.x) - (ownPos->x + ownBox->offset.x + ownBox->width)) / posChange.x;
	}
	else if(posChange.x > 0)
	{
		tMin.x = ((targetObject->pos.x + targetBox->offset.x) - (ownPos->x + ownBox->offset.x + ownBox->width)) / posChange.x;
		tMax.x = ((targetObject->pos.x + targetBox->offset.x + targetBox->width) - (ownPos->x + ownBox->offset.x)) / posChange.x;
	}
	if(posChange.y < 0)
	{
		tMin.y = ((targetObject->pos.y + targetBox->offset.y + targetBox->height) - (ownPos->y + ownBox->offset.y)) / posChange.y;
		tMax.y = ((targetObject->pos.y + targetBox->offset.y) - (ownPos->y + ownBox->offset.y + ownBox->height)) / posChange.y;
	}
	else if(posChange.y > 0)
	{
		tMin.y = ((targetObject->pos.y + targetBox->offset.y) - (ownPos->y + ownBox->offset.y + ownBox->height)) / posChange.y;
		tMax.y = ((targetObject->pos.y + targetBox->offset.y + targetBox->height) - (ownPos->y + ownBox->offset.y)) / posChange.y;
	}

	//compare the timeframes
	if(tMax.y < tMin.x || tMax.x < tMin.y)
	{
		//no collision
		return result;
	}

	//assume our own box is a rectangle
	if(targetBox->isTriangle &&
		((posChange.x < 0 && !targetBox->rightAlign) ||
		(posChange.x > 0 && targetBox->rightAlign) ||
		(posChange.y < 0 && !targetBox->bottomAlign) ||
		(posChange.y > 0 && targetBox->bottomAlign)))
	{
		//target box is a triangle and the hypotenuse isn't oriented away from the direction of movement,
		//so we still need to check against it's hypotenuse.
		//projections against the axis perpendicular to the movement vector are necessary, so get that axis
		HSVect2D movePerp;
		movePerp.x = posChange.y * -1;
		movePerp.y = posChange.x;
		float movePerpMag = sqrt(pow(movePerp.x, 2) + pow(movePerp.y, 2));
		movePerp.x = movePerp.x / movePerpMag;
		movePerp.y = movePerp.y / movePerpMag;
		
		//get all the necessary points of own box
		HSVect2D centerPoint, verticalPoint, horizontalPoint, leftHyp, rightHyp;
		if(posChange.x < 0)
		{
			centerPoint.x = ownPos->x + ownBox->offset.x;
			horizontalPoint.x = centerPoint.x + ownBox->width;
			if(posChange.y < 0)
			{
				centerPoint.y = ownPos->y + ownBox->offset.y;
				verticalPoint.y = centerPoint.y + ownBox->height;
			}
			else if(posChange.y > 0)
			{
				centerPoint.y = ownPos->y + ownBox->offset.y + ownBox->height;
				verticalPoint.y = centerPoint.y - ownBox->height;
			}
		}
		else if(posChange.x > 0)
		{
			centerPoint.x = ownPos->x + ownBox->offset.x + ownBox->width;
			horizontalPoint.x = centerPoint.x - ownBox->width;
			if(posChange.y < 0)
			{
				centerPoint.y = ownPos->y + ownBox->offset.y;
				verticalPoint.y = centerPoint.y + ownBox->height;
			}
			else if(posChange.y > 0)
			{
				centerPoint.y = ownPos->y + ownBox->offset.y + ownBox->height;
				verticalPoint.y = centerPoint.y - ownBox->height;
			}
		}
		verticalPoint.x = centerPoint.x;
		horizontalPoint.y = centerPoint.y;

		if(((posChange.x < 0 && !targetBox->rightAlign) || (posChange.x > 0 && targetBox->rightAlign)) &&
			((posChange.y < 0 && !targetBox->bottomAlign) || (posChange.y > 0 && targetBox->bottomAlign)))
		{
			//hypotenuse is oriented directly towards the direction of movement.
			//project own box's forward edges and the target box's hypotenuse aganst the axis perpendicular to the movement vector.
			leftHyp = GetLeftHypotenusePoint(&(targetObject->pos), targetBox);
			rightHyp = GetRightHypotenusePoint(&(targetObject->pos), targetBox);

			//project the hypotenuse against the axis
			float hypMin, hypMax;
			ProjectLineSegmentAgainstLine(&leftHyp, &rightHyp, &movePerp, &hypMin, &hypMax);

			//project the horizontal line against the axis
			float ownMin, ownMax;
			ProjectLineSegmentAgainstLine(&centerPoint, &horizontalPoint, &movePerp, &ownMin, &ownMax);

			//compare the results
			if(IntervalDifference(&hypMin, &hypMax, &ownMin, &ownMax) > 0)
			{
				//if the box can be jumped through then horizontal collisions don't count
				if(targetObject->canBeJumpedThrough)
				{
					return result;
				}

				//horizontal line never hits the hypotenuse, so the vertical line must hit a hypotenuse point
				result.horizontalImpact = true;
				result.lastSelfBox = ownBox;
				result.lastTargetBox = targetBox;
				result.lastTargetObject = targetObject;
				return result;
			}

			//project the vertical line against the axis
			ProjectLineSegmentAgainstLine(&centerPoint, &verticalPoint, &movePerp, &ownMin, &ownMax);
			
			//compare the results
			if(IntervalDifference(&hypMin, &hypMax, &ownMin, &ownMax) >= 0)
			{
				//check if the box is jump through
				if(targetObject->canBeJumpedThrough && posChange.y <= 0)
				{
					//the box is jump-through and own box isn't moving downward, so ignore the box
					return result;
				}

				//vertical line never hits the hypotenuse, so the horizontal line hits a hypotenuse point
				result.verticalImpact = true;
				result.lastSelfBox = ownBox;
				result.lastTargetBox = targetBox;
				result.lastTargetObject = targetObject;
				return result;
			}

			//there is a potential collision with the hypotenuse. Exit this if/else statment
		}
		else
		{
			//hypotenuse is facing in a direction perpendicular-ish to the direction of movement.
			//project own box's forward edges and the target box's hypotenuse and opposing edge aganst the axis perpendicular to the movement vector.
			//get the points on the triangle
			HSVect2D rightAng, closeHyp, farHyp;
			rightAng = GetRightAnglePoint(&(targetObject->pos), targetBox);
			if(posChange.x < 0)
			{
				closeHyp = GetRightHypotenusePoint(&(targetObject->pos), targetBox);
				farHyp = GetLeftHypotenusePoint(&(targetObject->pos), targetBox);
			}
			else if(posChange.x > 0)
			{
				closeHyp = GetLeftHypotenusePoint(&(targetObject->pos), targetBox);
				farHyp = GetRightHypotenusePoint(&(targetObject->pos), targetBox);
			}

			//project close edge against the axis
			float targetMin, targetMax;
			ProjectLineSegmentAgainstLine(&closeHyp, &rightAng, &movePerp, &targetMin, &targetMax);

			if(closeHyp.x == rightAng.x)
			{
				//target box's straight edge facing own box is vertical.
				if(tMin.x > tMin.y)
				{
					//we already know that the vertical edges might strike.
					//own box could be moving directly away from the hypotenuse. check for this.
					TerrainCollisionResult testResult = IsBoxCollidingWithHypotenuse(ownPos, ownBox, targetObject, targetBox, true);
					if(!testResult.horizontalImpact)
					{
						//box is moving away from the hypotenuse, so ignore it
						return result;
					}

					//if the box can be jumped through then horizontal collisions don't count
					if(targetObject->canBeJumpedThrough)
					{
						return result;
					}

					//vertical edges strike
					result.horizontalImpact = true;
					result.lastSelfBox = ownBox;
					result.lastTargetBox = targetBox;
					result.lastTargetObject = targetObject;
					return result;
				}

				//project own box's horizontal edge against the axis
				float ownMin, ownMax;
				ProjectLineSegmentAgainstLine(&horizontalPoint, &centerPoint, &movePerp, &ownMin, &ownMax);
				if(IntervalDifference(&targetMin, &targetMax, &ownMin, &ownMax) <= 0)
				{
					//own horizontal edge might strike a hypotenuse point.
					//own box could be moving directly away from the hypotenuse. check for this.
					TerrainCollisionResult testResult = IsBoxCollidingWithHypotenuse(ownPos, ownBox, targetObject, targetBox, true);
					if(!testResult.horizontalImpact)
					{
						//box is moving away from the hypotenuse, so ignore it
						return result;
					}
					
					//check if the box is jump through
					if(targetObject->canBeJumpedThrough && posChange.y <= 0)
					{
						//the box is jump-through and own box isn't moving downward, so ignore the box
						return result;
					}

					//hypotenuse point struck
					result.verticalImpact = true;
					result.lastSelfBox = ownBox;
					result.lastTargetBox = targetBox;
					result.lastTargetObject = targetObject;
					return result;
				}

				//project the hypotenuse against the axis
				ProjectLineSegmentAgainstLine(&closeHyp, &farHyp, &movePerp, &targetMin, &targetMax);
				if(IntervalDifference(&targetMin, &targetMax, &ownMin, &ownMax) >= 0)
				{
					//own horizontal edge is not approaching the hypotenuse, so no collision
					return result;
				}
				
				//there is a potential collision with the hypotenuse. Exit this if/else statment
			}
			else if(closeHyp.y == rightAng.y)
			{
				//target box's straight edge facing own box is horizontal.
				if(tMin.x <= tMin.y)
				{
					//we already know that the horizontal edge might strike.
					//own box could be moving directly away from the hypotenuse. check for this.
					TerrainCollisionResult testResult = IsBoxCollidingWithHypotenuse(ownPos, ownBox, targetObject, targetBox, true);
					if(!testResult.horizontalImpact)
					{
						//box is moving away from the hypotenuse, so ignore it
						return result;
					}

					//check if the box is jump through
					if(targetObject->canBeJumpedThrough && posChange.y <= 0)
					{
						//the box is jump-through and own box isn't moving downward, so ignore the box
						return result;
					}

					//horizontal edges strike
					result.verticalImpact = true;
					result.lastSelfBox = ownBox;
					result.lastTargetBox = targetBox;
					result.lastTargetObject = targetObject;
					return result;
				}

				//project own box's vertical edge against the axis
				float ownMin, ownMax;
				ProjectLineSegmentAgainstLine(&verticalPoint, &centerPoint, &movePerp, &ownMin, &ownMax);
				if(IntervalDifference(&targetMin, &targetMax, &ownMin, &ownMax) < 0)
				{
					//own vertical edge might strike a hypotenuse point.
					//own box could be moving directly away from the hypotenuse. check for this.
					TerrainCollisionResult testResult = IsBoxCollidingWithHypotenuse(ownPos, ownBox, targetObject, targetBox, true);
					if(!testResult.horizontalImpact)
					{
						//box is moving away from the hypotenuse, so ignore it
						return result;
					}

					//if the box can be jumped through then horizontal collisions don't count
					if(targetObject->canBeJumpedThrough)
					{
						return result;
					}

					//hypotenuse point struck
					result.horizontalImpact = true;
					result.lastSelfBox = ownBox;
					result.lastTargetBox = targetBox;
					result.lastTargetObject = targetObject;
					return result;
				}

				//project the hypotenuse against the axis
				ProjectLineSegmentAgainstLine(&closeHyp, &farHyp, &movePerp, &targetMin, &targetMax);
				if(IntervalDifference(&targetMin, &targetMax, &ownMin, &ownMax) >= 0)
				{
					//own vertical edge is not approaching the hypotenuse, so no collision
					return result;
				}
				
				//there is a potential collision with the hypotenuse. Exit this if/else statment
			}
				
			//there is a potential collision with the hypotenuse. Exit this if/else statment
		}

		//potential impact against the hypotenuse.
		return IsBoxCollidingWithHypotenuse(ownPos, ownBox, targetObject, targetBox);
	}
	else
	{
		//we know both rectangles collide
		if(targetBox->isTriangle)
		{
			//own box could be moving directly away from the hypotenuse. check for this.
			TerrainCollisionResult testResult = IsBoxCollidingWithHypotenuse(ownPos, ownBox, targetObject, targetBox, true);
			if(!testResult.horizontalImpact)
			{
				//box is moving away from the hypotenuse, so ignore it
				return result;
			}
		}

		//figure out the earliest collision point
		if(tMin.x > tMin.y)
		{
			//if the box can be jumped through then horizontal collisions don't count
			if(targetObject->canBeJumpedThrough)
			{
				return result;
			}

			//x hit first
			result.horizontalImpact = true;
		}
		else
		{
			//check if the box is jump through
			if(targetObject->canBeJumpedThrough && posChange.y <= 0)
			{
				//the box is jump-through and own box isn't moving downward, so ignore the box
				return result;
			}

			//y hit first
			result.verticalImpact = true;
		}

		result.lastSelfBox = ownBox;
		result.lastTargetBox = targetBox;
		result.lastTargetObject = targetObject;
		return result;
	}
}

TerrainCollisionResult PhysicsObject::IsCollidingWithObjectTerrain(TerrainObject * targetObject, TerrainCollisionResult * prevResult, HSVect2D * ownPos, HSVect2D * ownPrevPos, bool ignoreCurrentlOverlapping)
{
	//loop through this object's terrain boxes
	HSBox * selfTerrainBox = firstTerrainBox;
	HSBox * targetTerrainBox;
	while(selfTerrainBox != NULL)
	{
		//loop through the target's terrain boxes
		targetTerrainBox = targetObject->firstTerrainBox;
		while(targetTerrainBox != NULL)
		{
			//check for collision
			TerrainCollisionResult result = AreTerrainBoxesColliding(selfTerrainBox, targetObject, targetTerrainBox, prevResult, ownPos, ownPrevPos, ignoreCurrentlOverlapping);
			if(result.lastSelfBox != NULL) { return result; }

			targetTerrainBox = targetTerrainBox->nextBox;
		}
		selfTerrainBox = selfTerrainBox->nextBox;
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

TerrainCollisionResult PhysicsObject::IsCollidingWithAnyTerrain(list<HSObject*> * gameObjects, TerrainCollisionResult * prevResult, HSVect2D * ownPos, HSVect2D * ownPrevPos)
{
	list<HSObject*>::iterator objIt;
	for ( objIt=gameObjects->begin(); objIt != gameObjects->end(); objIt++)
	{
		if((*objIt)->id != id && (*objIt)->IsTerrain() && ((TerrainObject*)(*objIt))->firstTerrainBox != NULL)
		{
			TerrainCollisionResult result = IsCollidingWithObjectTerrain((TerrainObject*)(*objIt), prevResult, ownPos, ownPrevPos);
			if(result.lastSelfBox != NULL) { return result; }
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

void PhysicsObject::MoveOutOfTerrain(list<HSObject*> * gameObjects, HSVect2D * ownPrevPos)
{
	//see if this object is currently colliding with anything
	TerrainCollisionResult result = IsCollidingWithAnyTerrain(gameObjects, NULL, ownPrevPos, ownPrevPos);

	//not colliding with anything, so return
	if(result.lastSelfBox == NULL) { return; }

	//move own terrain box just outside of the target terrain box

}

HSVect2D PhysicsObject::CollisionReposition(list<HSObject*> * gameObjects, TerrainCollisionResult * result, HSVect2D * ownPos, HSVect2D * ownPrevPos)
{
	HSVect2D newPos;
	bool collision = true;

	if(ownPos == NULL)
	{
		ownPos = &pos;
	}

	if(ownPrevPos == NULL)
	{
		ownPrevPos = &prevPos;
	}

	//store the collision position
	newPos.x = ownPos->x;
	newPos.y = ownPos->y;

	//get own box's change in position
	HSVect2D posChange;
	posChange.x = ownPos->x - ownPrevPos->x;
	posChange.y = ownPos->y - ownPrevPos->y;

	//move this object out of terrain
	while(collision)
	{
		collision = false;

		if(result->horizontalImpact && result->verticalImpact)
		{
			//need to move to the edge of the hypotenuse.
			//we should already have the point of impact. we just need to use that to find the new position
			newPos.x = result->hypCollisionPoint.x - result->lastSelfBox->offset.x;
			if(result->lastTargetBox->rightAlign)
			{
				newPos.x -= result->lastSelfBox->width;
			}
			newPos.y = result->hypCollisionPoint.y - result->lastSelfBox->offset.y;
			if(result->lastTargetBox->bottomAlign)
			{
				newPos.y -= result->lastSelfBox->height;
			}
		}
		else if(result->horizontalImpact)
		{
			//need to move to the edge of a vertical surface.
			//adjust x based on the target box
			if(posChange.x < 0)
			{
				newPos.x = result->lastTargetObject->pos.x + result->lastTargetBox->offset.x + result->lastTargetBox->width - result->lastSelfBox->offset.x;
			}
			else if(posChange.x > 0)
			{
				newPos.x = result->lastTargetObject->pos.x + result->lastTargetBox->offset.x - result->lastSelfBox->width - result->lastSelfBox->offset.x;
			}
			//adjust y based on x adjustment
			float xRatio = (newPos.x - ownPrevPos->x) / posChange.x;
			newPos.y = ownPrevPos->y + (posChange.y * xRatio);
		}
		else if(result->verticalImpact)
		{
			//need to move to the edge of a horizontal surface.
			//adjust y based on the target box
			if(posChange.y < 0)
			{
				newPos.y = result->lastTargetObject->pos.y + result->lastTargetBox->offset.y + result->lastTargetBox->height - result->lastSelfBox->offset.y;
			}
			else if(posChange.y > 0)
			{
				newPos.y = result->lastTargetObject->pos.y + result->lastTargetBox->offset.y - result->lastSelfBox->height - result->lastSelfBox->offset.y;
			}
			//adjust x based on y adjustment
			float yRatio = (newPos.y - ownPrevPos->y) / posChange.y;
			newPos.x = ownPrevPos->x + (posChange.x * yRatio);
		}

		//don't let the object get moved too far back
		if((posChange.x > 0 && newPos.x < ownPrevPos->x) || (posChange.x < 0 && newPos.x > ownPrevPos->x))
		{
			newPos.x = ownPrevPos->x;
		}
		if((posChange.y > 0 && newPos.y < ownPrevPos->y) || (posChange.y < 0 && newPos.y > ownPrevPos->y))
		{
			newPos.y = ownPrevPos->y;
		}

		//now, try colliding with all objects again, using the new position
		TerrainCollisionResult newResult = IsCollidingWithAnyTerrain(gameObjects, result, &newPos);
		if(newResult.lastSelfBox != NULL)
		{
			if(ownPrevPos->x == newPos.x || ownPrevPos->y == newPos.y)
			{
				//this object can't be moved any farther and is still stuck in terrain. Move it out a different way
				MoveOutOfTerrain(gameObjects, ownPrevPos);
				break;
			}

			//there's still a collision, so try again
			collision = true;
			*result = newResult;
		}
	}

	return newPos;
}

void PhysicsObject::CollisionPhysics(HSVect2D * newPos, TerrainCollisionResult * result, HSVect2D * ownPos, HSVect2D * ownPrevPos)
{
	if(ownPos == NULL)
	{
		ownPos = &pos;
	}

	if(ownPrevPos == NULL)
	{
		ownPrevPos = &prevPos;
	}

	//handle terrain impact physics
	HSVect2D newVel;
	newVel.x = vel.x;
	newVel.y = vel.y;

	//scale back the y velocity due to the fact that it actually had less time to accelerate
	//due to gravity before hitting the terrain
	if(newPos->y != ownPos->y && ownPrevPos->y != ownPos->y)
	{
		newVel.y -= (abs(newPos->y - ownPos->y) / abs(ownPrevPos->y - ownPos->y)) * gravity;
	}

	//get the largest bounce factor between the two objects
	float largestBounce = bounce;
	if(result->lastTargetObject->bounce > largestBounce) { largestBounce = result->lastTargetObject->bounce; }

	//get the friction
	float totalFriction = (1 - friction) * (1 - result->lastTargetObject->friction);

	//apply bounce and friciton
	if(result->horizontalImpact && result->verticalImpact)
	{
		//a hit on the hypotenuse. gotta do something more complicated
		//figure out how the impact line is oriented
		HSVect2D lineA, lineB;
		if(result->lastTargetBox->bottomAlign)
		{
			lineA = GetLeftHypotenusePoint(&result->lastTargetObject->pos, result->lastTargetBox);
			lineB = GetRightHypotenusePoint(&result->lastTargetObject->pos, result->lastTargetBox);
		}
		else
		{
			lineA = GetRightHypotenusePoint(&result->lastTargetObject->pos, result->lastTargetBox);
			lineB = GetLeftHypotenusePoint(&result->lastTargetObject->pos, result->lastTargetBox);
		}

		//get the normal of the impact line, pointing away from the triangle
		HSVect2D normal;
		normal.x = lineB.y - lineA.y;
		normal.y = (lineB.x - lineA.x) * -1;
			
		//get components of the new velocity
		float uPart = ((newVel.x * normal.x) + (newVel.y * normal.y)) / (pow(normal.x, 2) + pow(normal.y, 2));
		HSVect2D u, w;
		u.x = normal.x * uPart;
		u.y = normal.y * uPart;
		w.x = newVel.x - u.x;
		w.y = newVel.y - u.y;

		//get the new velocity, applying bounce and friction
		newVel.x = (w.x * totalFriction) - (u.x * largestBounce);
		newVel.y = (w.y * totalFriction) - (u.y * largestBounce);
	}
	else if(result->horizontalImpact)
	{
		//a hit on the left/right
		newVel.x = newVel.x * largestBounce * -1;
		newVel.y = newVel.y * totalFriction;
	}
	else if(result->verticalImpact)
	{
		//a hit on the top/bottom
		newVel.x = newVel.x * totalFriction;
		newVel.y = newVel.y * largestBounce * -1;
	}

	//scale back the y velocity once again, in order to reflect the remainder of the frame in
	//which it is pulling downward on the object and counteracting the upward bounce
	if(newPos->y != ownPrevPos->y && ownPos->y != ownPrevPos->y)
	{
		newVel.y += (abs(newPos->y - ownPos->y) / abs(ownPrevPos->y - ownPos->y)) * gravity;
	}

	//now, get the magnitude of the velocity and if it's too small, just set it to zero
	float velMagSqrd = pow(newVel.x, 2) + pow(newVel.y, 2);
	if(velMagSqrd < pow(MIN_VEL, 2))
	{
		newVel.x = 0;
		newVel.y = 0;
	}

	//apply the new position and velocity
	pos.x = newPos->x;
	pos.y = newPos->y;
	vel.x = newVel.x;
	vel.y = newVel.y;
}

int PhysicsObject::HandleTerrainCollision(list<HSObject*> * gameObjects, TerrainCollisionResult * result, HSVect2D * ownPos, HSVect2D * ownPrevPos)
{
	if(ownPos == NULL)
	{
		ownPos = &pos;
	}

	if(ownPrevPos == NULL)
	{
		ownPrevPos = &prevPos;
	}

	//get the new position
	HSVect2D newPos = CollisionReposition(gameObjects, result, ownPos, ownPrevPos);
	
	////handle terrain impact physics
	CollisionPhysics(&newPos, result, ownPos, ownPrevPos);

	if(fragile) { toDelete = true; }

	return 0;
}

int PhysicsObject::CollideTerrain(list<HSObject*> * gameObjects)
{
	//check for collisions with terrain, if this object has any terrain boxes. if there is one, handle it
	if(firstTerrainBox != NULL && (pos.x != prevPos.x || pos.y != prevPos.y))
	{
		TerrainCollisionResult result = IsCollidingWithAnyTerrain(gameObjects);
		if(result.lastSelfBox != NULL) { HandleTerrainCollision(gameObjects, &result); }
	}

	return 0;
}

bool PhysicsObject::IsTerrain()
{
	return false;
}

bool PhysicsObject::IsTerrainObject()
{
	return true;
}

bool PhysicsObject::IsPhysicsObject()
{
	return true;
}