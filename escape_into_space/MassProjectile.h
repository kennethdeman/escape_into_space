#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GameEngine.h"

//-----------------------------------------------------
// MassProjectile Class									
//-----------------------------------------------------
class MassProjectile
{
public:
	MassProjectile(int x, int y, double angle, DOUBLE2 inheritance);				// Constructor
	virtual ~MassProjectile();		// Destructor

	//-------------------------------------------------
	// Own methods								
	//-------------------------------------------------
	void Tick(double deltaTime);
	void Paint(MATRIX3X2 matTransform);
	void HandleCollision(HitRegion *hitLevelPtr, double deltaTime);
	DOUBLE2 HandleCircularCollision(DOUBLE2 otherCirclePos,DOUBLE2 otherCircleVelocity, double radius, double mass);
	bool ReadyToBeDestroyed();

	DOUBLE2 GetPos();
	DOUBLE2 GetVelocity();

	void SetVelocity(DOUBLE2 velocity);

	void AddForce(DOUBLE2 force);

	double GetRadius();
	double GetMass();


	static const int LASERSHOT_WIDTH=26, LASERSHOT_HEIGHT=26;
	static const int CENTER_X = LASERSHOT_WIDTH/2;
	static const int CENTER_Y = LASERSHOT_HEIGHT/2;

	static const int COLOR_RED=0;
	static const int COLOR_GREEN=1;
private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	static Bitmap* m_BmpMassProjectilePtr;
	DOUBLE2 m_Pos, m_Velocity, m_VelocityProjected, m_VelocityOrthogonal, m_Acceleration;
	double m_Angle, m_Speed, m_Mass, m_Radius;

	int m_TickCountNormalState,m_TickCount, m_ExplosionTimer;
	double m_SlowDownAnimations;

	int m_CurrentState;	

	static const int STATE_NORMAL=0;
	static const int STATE_EXPANDING=1;
	static const int STATE_EXPLODING=2;
	static const int STATE_DESTROYED=3;

	static const int RESET_SLOW_DOWN=-10;
	static const int EXPANDMASS_FRAMES=7;
	static const int EXPANDMASS_ANIMATION_SPEED=400;

	static const int EXPLODING_FRAMES=7;
	static const int EXPLODING_ANIMATION_SPEED=100;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	MassProjectile(const MassProjectile& yRef);									
	MassProjectile& operator=(const MassProjectile& yRef);	
};


