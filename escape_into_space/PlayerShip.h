#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GameEngine.h"

#include "LaserList.h"
#include "LaserShot.h"

//-----------------------------------------------------
// PlayerShip Class									
//-----------------------------------------------------
class PlayerShip
{
public:
	PlayerShip(int x, int y);				// Constructor
	virtual ~PlayerShip();		// Destructor

	//-------------------------------------------------
	// Own methods								
	//-------------------------------------------------
	void Tick(double deltaTime);
	void Paint(MATRIX3X2 matTransform);
	void HandleCollision(HitRegion *hitLevelPtr, double deltaTime);
	void ToggleGodmode();
	void AddForce(DOUBLE2 force);
	void AddGravity(DOUBLE2 gravity);

	void TurnBackThrusterOn (bool isOn);
	void TurnBottomThrusterOn (bool isOn);
	void TurnNoseThrusterUpOn (bool isOn);
	void TurnNoseThrusterDownOn (bool isOn);

	void SetDirectionLeft();
	void SetDirectionRight();

	void SetTimeWhenLastShot(double time);
	double GetTimeWhenLastShot();

	DOUBLE2 GetPos();

	void SetVelocity(DOUBLE2 velocity);
	DOUBLE2 GetVelocity();

	double GetMass();



	DOUBLE2 GetAcceleration();

	void Shoot(LaserList* laserListPtr);

	DOUBLE2 GetShootingPos();
	double GetShootingAngle();

	void SetAngleWithVector(DOUBLE2 vector);
	double GetAngle();
	double GetSpeed();

	bool Hit(HitRegion* hitTargetPtr);

	bool IsExploding();
	
	DOUBLE2 CastOutExplosionForce(DOUBLE2 targetPos);

	void HasExploded();

	HitRegion* GetHitRegion();

	void SubtractHealth(double amount);
	void SetHealth(double health);
	double GetHealth();
	double GetMaxHealth();

	int GetMassProjAmmo();
	void SubtractMassProjAmmo(int amount);

	int GetGravBallAmmo();
	void SubtractGravBallAmmo(int amount);

	int GetRadius();

	void GodModeMoveLeft();
	void GodModeMoveRight();
	void GodModeMoveUp();
	void GodModeMoveDown();

private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	void PositionsRaycast();
	void RaycastCollision(DOUBLE2 point, DOUBLE2 vector, DOUBLE2 refPos, HitRegion* hitRegion, double deltaTime);

	HitRegion *m_HitRegionPtr;
	void PositionHitRegion();

	static const int MAX_POINTS=3;
	DOUBLE2 m_HitRegionPoints[MAX_POINTS];

	static Bitmap* m_BmpPlayerShipPtr, *m_BmpBackThrusterPtr, *m_BmpBottomThrusterPtr, *m_BmpNoseThrusterDownPtr, 
		*m_BmpNoseThrusterUpPtr, *m_BmpExplosionPtr;

	static const int DIRECTION_LEFT=0;
	static const int DIRECTION_RIGHT=1;

	static const int STATE_WORKING=0;
	static const int STATE_FALLING=1;
	static const int STATE_EXPLODING=2;

	static const int MASS=2, RADIUS=15, GODMODE_SPEED=1500;;
	
	double m_Health,m_MaxHealth, m_TicksUntilExplosion;
	int m_MassProjAmmo, m_GravBallAmmo;
	
	bool m_IsBackThrusterOn;
	bool m_IsBottomThrusterOn;
	bool m_IsNoseThrusterDownOn;
	bool m_IsNoseThrusterUpOn;

	bool m_HasCastedOutExplosionForce, m_Godmode;

	DOUBLE2 m_Pos, m_Velocity, m_BackThrusterForce, m_BottomThrusterForce, m_Acceleration, 
		m_SumOfAccelerations,
		m_FrontPos,m_BackPos, m_TopPos, m_BottomPos, m_MidPos;
	DOUBLE2 m_Point1,m_Point2,m_Point3,m_Point4,m_Point5,
		m_Vector1, m_Vector2, m_Vector3, m_Vector4, m_Vector5, m_CollisionForce, m_CollisionFriction;
	double m_Angle, m_BackThrusterPower, m_BottomThrusterPower, m_BackThrusterAngle, m_BottomThrusterAngle;
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
	PlayerShip(const PlayerShip& yRef);									
	PlayerShip& operator=(const PlayerShip& yRef);	
};

 
