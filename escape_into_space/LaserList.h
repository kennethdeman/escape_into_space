#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GameEngine.h"
#include "LaserShot.h"

//-----------------------------------------------------
// LaserList Class									
//-----------------------------------------------------
class LaserList
{
public:
	LaserList();				// Constructor
	virtual ~LaserList();		// Destructor

	//-------------------------------------------------
	// Methods							
	//-------------------------------------------------
	bool Add(LaserShot* laserPtr);
	bool Remove(LaserShot* laserPtr);
	void RemoveAll();
	void Tick(double deltaTime);
	void Paint(MATRIX3X2 matTransform);
	void HandleCollision(HitRegion *hitLevelPtr, double deltaTime);
	double CheckHitCollision(HitRegion *hitRegionPtr, DOUBLE2 targetPos);

private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	static const int MAX_LASERSHOTS =100;
	LaserShot* m_LaserShotPtrArr[MAX_LASERSHOTS];
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	LaserList(const LaserList& yRef);									
	LaserList& operator=(const LaserList& yRef);	
};


 
