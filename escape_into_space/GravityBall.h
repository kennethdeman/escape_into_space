#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GameEngine.h"

//-----------------------------------------------------
// GravityBall Class									
//-----------------------------------------------------
class GravityBall
{
public:
	GravityBall(int x, int y, double angle, DOUBLE2 inheritance);				// Constructor
	virtual ~GravityBall();		// Destructor

	//-------------------------------------------------
	// Own methods								
	//-------------------------------------------------
	void Tick(double deltaTime);
	void Paint(MATRIX3X2 matTransform);
	void HandleCollision(HitRegion *hitLevelPtr, double deltaTime);
	DOUBLE2 HandleCircularCollision(DOUBLE2 otherCirclePos, DOUBLE2 otherCircleVelocity, double radius, double mass);

	bool ReadyToBeDestroyed();

	DOUBLE2 ExerciseForce(DOUBLE2 objectPos, double objectMass);

	bool IsExploding();

	DOUBLE2 CastOutExplosionForce(DOUBLE2 targetPos);

	void AddForce(DOUBLE2 force);

	DOUBLE2 GetPos();

	double GetRadius();

	static const int LASERSHOT_WIDTH=26, LASERSHOT_HEIGHT=26;
	static const int CENTER_X = LASERSHOT_WIDTH/2;
	static const int CENTER_Y = LASERSHOT_HEIGHT/2;

	static const int COLOR_RED=0;
	static const int COLOR_GREEN=1;
private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	static Bitmap* m_BmpGravityBallPtr,  *m_BmpExplosionPtr;
	DOUBLE2 m_Pos, m_Velocity, m_VelocityProjected, m_VelocityOrthogonal, m_Acceleration;
	double m_Angle, m_Speed, m_Mass;

	int m_TickCount, m_ExplosionTimer;
	double m_SlowDownAnimations;

	int m_CurrentState;	

	bool m_HasExplodedAtleastOnce,m_HasCastedOutExplosionForce;

	static const int RADIUS=10;

	static const int STATE_NORMAL=0;
	static const int STATE_EXPLODING=1;
	static const int STATE_DESTROYED=2;

	static const int RESET_SLOW_DOWN=-10;
	static const int NORMAL_FRAMES=3;
	double m_NormalAnimationSpeed;

	static const int EXPLOSION_FRAMES=16;
	static const int EXPLOSION_ANIMATION_SPEED=300;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	GravityBall(const GravityBall& yRef);									
	GravityBall& operator=(const GravityBall& yRef);	
};

 
