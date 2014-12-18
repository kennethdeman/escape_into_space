#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GameEngine.h"
#include "MassProjectile.h"

//-----------------------------------------------------
// MassProjList Class									
//-----------------------------------------------------
class MassProjList
{
public:
	MassProjList();				// Constructor
	virtual ~MassProjList();		// Destructor

	//-------------------------------------------------
	// Methods							
	//-------------------------------------------------
	bool Add(MassProjectile* massProjectilePtr);
	bool Remove(MassProjectile* massProjectilePtr);
	void RemoveAll();
	void Tick(double deltaTime);
	void Paint(MATRIX3X2 matTransform);
	void HandleCollision(HitRegion *hitLevelPtr, double deltaTime);
	DOUBLE2 HandleCircularCollision(DOUBLE2 otherCirclePos,DOUBLE2 otherCircleVelocity, double radius, double mass);

private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	static const int MAX_MASSPROJECTILES =100;
	MassProjectile* m_MassProjectilePtrArr[MAX_MASSPROJECTILES];
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	MassProjList(const MassProjList& yRef);									
	MassProjList& operator=(const MassProjList& yRef);	
};


 
