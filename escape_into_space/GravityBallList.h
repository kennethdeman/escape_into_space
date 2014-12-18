#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GameEngine.h"
#include "GravityBall.h"

//-----------------------------------------------------
// GravityBallList Class									
//-----------------------------------------------------
class GravityBallList
{
public:
	GravityBallList();				// Constructor
	virtual ~GravityBallList();		// Destructor

	//-------------------------------------------------
	// Methods							
	//-------------------------------------------------
	bool Add(GravityBall* gravityBallPtr);
	bool Remove(GravityBall* gravityBallPtr);
	void RemoveAll();
	void Tick(double deltaTime);
	void Paint(MATRIX3X2 matTransform);
	void HandleCollision(HitRegion *hitLevelPtr, double deltaTime);
	DOUBLE2 HandleCircularCollision(DOUBLE2 otherCirclePos,DOUBLE2 otherCircleVelocity, double radius, double mass);
	DOUBLE2 ExerciseForces(DOUBLE2 objectPos, double objectMass);
	DOUBLE2 CastOutExplosionForces(DOUBLE2 targetPos);
	void AddForce(DOUBLE2 force);

private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	static const int MAX_GRAVITYBALLS =100;
	GravityBall* m_GravityBallPtrArr[MAX_GRAVITYBALLS];
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	GravityBallList(const GravityBallList& yRef);									
	GravityBallList& operator=(const GravityBallList& yRef);	
};


 
