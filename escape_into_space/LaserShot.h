#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GameEngine.h"

//-----------------------------------------------------
// LaserShot Class									
//-----------------------------------------------------
class LaserShot
{
public:
	LaserShot(int x, int y, double angle, DOUBLE2 inheritance);				// Constructor
	LaserShot(int x, int y, double angle, DOUBLE2 inheritance, int color);
	virtual ~LaserShot();		// Destructor

	//-------------------------------------------------
	// Own methods								
	//-------------------------------------------------
	void Tick(double deltaTime);
	void Paint(MATRIX3X2 matTransform);
	void HandleCollision(HitRegion *hitLevelPtr, double deltaTime);
	bool CheckHitCollision(HitRegion *hitRegionPtr, DOUBLE2 targetPos);

	DOUBLE2 GetPos();

	int GetAmountOfHits();

	static const int LASERSHOT_WIDTH=26, LASERSHOT_HEIGHT=26;
	static const int CENTER_X = LASERSHOT_WIDTH/2;
	static const int CENTER_Y = LASERSHOT_HEIGHT/2;

	static const int COLOR_RED=0;
	static const int COLOR_GREEN=1;
private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	static Bitmap* m_BmpLaserShotPtr;
	DOUBLE2 m_Pos, m_Vector, m_Velocity, m_Inheritance;
	double m_Angle, m_Speed;

	int m_AmountOfHits, m_Color;

	static const int MAX_REFLECTIONS=5;
	double m_ReflectionAngleArr[MAX_REFLECTIONS];
	DOUBLE2 m_ReflectionPtr[MAX_REFLECTIONS*3];
	int m_ReflectionCount;

	bool m_IsBeingReflected;
	DOUBLE2 m_NewPos, m_ReflectionPoint, m_ReflectionVector;
	DOUBLE2 m_BitMapReflectionPoint;
	double m_NewAngle;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	LaserShot(const LaserShot& yRef);									
	LaserShot& operator=(const LaserShot& yRef);	
};

 
