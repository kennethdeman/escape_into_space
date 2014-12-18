#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GameEngine.h"

#include "LaserList.h"
#include "LaserShot.h"

//-----------------------------------------------------
// RoundShip Class									
//-----------------------------------------------------
class RoundShip
{
public:
	RoundShip(int x, int y);				// Constructor
	virtual ~RoundShip();		// Destructor

	//-------------------------------------------------
	// Own methods								
	//-------------------------------------------------
	void Tick(double deltaTime);
	void Paint(MATRIX3X2 matTransform);
	void HandleCollision(HitRegion *hitLevelPtr, double deltaTime);
	DOUBLE2 HandleCircularCollision(DOUBLE2 otherCirclePos,DOUBLE2 otherCircleVelocity, double radius);

	void AddForce(DOUBLE2 force);	
	void AddGravity(DOUBLE2 gravity);

	void TurnLeftThrusterOn (bool isOn);
	void TurnBottomThrusterOn (bool isOn);
	void TurnRightThrusterOn (bool isOn);
	void TurnTopThrusterOn (bool isOn);

	void SetDirectionLeft();
	void SetDirectionRight();

	void SetTimeWhenLastShot(double time);
	double GetTimeWhenLastShot();

	DOUBLE2 GetPos();

	void SetVelocity(DOUBLE2 velocity);
	DOUBLE2 GetVelocity();



	DOUBLE2 GetAcceleration();

	void Shoot(LaserList* laserListPtr);

	double GetShootingAngle();
	double GetAngle();
	double GetSpeed();

	HitRegion* GetHitRegion();

	void SetFocusPoint(DOUBLE2 focus);

	void LineOfSight(DOUBLE2 focus, HitRegion* hitRegionPtr);

	bool Hit(HitRegion* hitTargetPtr);

	bool IsExploding();

	DOUBLE2 CastOutExplosionForce(DOUBLE2 targetPos);

	void HasExploded();

	bool HasTarget();
	DOUBLE2 GetTarget();

	void SubtractHealth(double amount);

	double GetRadius();
	double GetMass();

	bool IsReadyToBeDeleted();

	static const int ROUNDSHIP_WIDTH=31, ROUNDSHIP_HEIGHT=31;
private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------

	HitRegion *m_HitRegionPtr;

	static Bitmap* m_BmpRoundShipPtr, *m_BmpLeftThrusterPtr, *m_BmpBottomThrusterPtr, *m_BmpTopThrusterPtr, 
		*m_BmpRightThrusterPtr, *m_BmpCannonPtr, *m_BmpExplosionPtr;

	static const int DIRECTION_LEFT=0;
	static const int DIRECTION_RIGHT=1;

	static const int STATE_WORKING=0;
	static const int STATE_FALLING=1;
	static const int STATE_EXPLODING=2;
	static const int STATE_DESTROYED=3;

	static const int MASS=3;

	static const int RADIUS=31;

	bool m_IsLeftThrusterOn;
	bool m_IsBottomThrusterOn;
	bool m_IsTopThrusterOn;
	bool m_IsRightThrusterOn;

	bool m_HasCastedOutExplosionForce;

	bool m_HasTarget;

	double m_Health, m_TicksUntilExplosion;

	DOUBLE2 m_Pos, m_Velocity, m_LeftThrusterForce, m_BottomThrusterForce, m_RightThrusterForce
		, m_TopThrusterForce, m_Acceleration, m_SumOfAccelerations;
	DOUBLE2 m_FocusPoint, m_FocusVector;
	DOUBLE2 m_VelocityProjected, m_VelocityOrthogonal;

	double m_Angle, m_LeftThrusterPower, m_BottomThrusterPower, m_RightThrusterPower, 
		m_TopThrusterPower,
		m_LeftThrusterAngle, m_BottomThrusterAngle, m_CannonAngle;
	double m_TimeWhenLastShot;
	int m_CurrentDirection, m_CurrentState;

	int m_TickCount;
	double m_SlowDownAnimations;

	static const int RESET_SLOW_DOWN=-10;
	static const int EXPLOSION_FRAMES=16;
	static const int EXPLOSION_ANIMATION_SPEED=300;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	RoundShip(const RoundShip& yRef);									
	RoundShip& operator=(const RoundShip& yRef);	
};


