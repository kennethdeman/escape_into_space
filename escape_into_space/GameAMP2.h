//-----------------------------------------------------------------
// Game File
// C++ Header - GameAMP2.h - version v2_12 jan 2013
// Copyright Kevin Hoefman - kevin.hoefman@howest.be
// http://www.digitalartsandentertainment.be/
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------

#include "GameEngine.h"
#include "AbstractGame.h"

#include "LaserList.h"
#include "MassProjList.h"
#include "GravityBallList.h"

#include "LaserShot.h"
#include "MassProjectile.h"
#include "GravityBall.h"

#include "PlayerShip.h"
#include "RoundShip.h"

//-----------------------------------------------------------------
// GameAMP2 Class																
//-----------------------------------------------------------------
class GameAMP2 : public AbstractGame, public Callable
{
public:				
	//---------------------------
	// Constructor(s)
	//---------------------------
	GameAMP2();

	//---------------------------
	// Destructor
	//---------------------------
	virtual ~GameAMP2();

	//---------------------------
	// General Methods
	//---------------------------

	void GameInitialize(HINSTANCE hInstance);
	void GameStart();				
	void GameEnd();
	void MouseButtonAction(bool isLeft, bool isDown, int x, int y, WPARAM wParam);
	void MouseMove(int x, int y, WPARAM wParam);
	void CheckKeyboard();
	void KeyPressed(TCHAR cKey);
	void GameTick(double deltaTime);
	void GamePaint(RECT rect);

	//void CallAction(Caller* callerPtr);

	// -------------------------
	// Member functions
	// -------------------------

	static const int MAX_ROUNDSHIPS=20;


private:
	// -------------------------
	// Member functions
	// -------------------------
	double ExplosionDamage(DOUBLE2 explosionForce);
	DOUBLE2 CalculatePlanetGravity(DOUBLE2 objectPos, double objectMass);
	// -------------------------
	// Datamembers
	// -------------------------
	double m_Fps;

	PlayerShip *m_PlayerShipPtr;

	RoundShip *m_RoundShipPtrArr[MAX_ROUNDSHIPS];

	LaserList *m_LaserListPtr;

	MassProjList *m_MassProjListPtr;

	GravityBallList *m_GravityBallListPtr;


	HitRegion *m_HitLevelPtr, *m_HitLevelCircularPtr, *m_HitIndoorPtr,*m_HitLevel2Ptr;

	Bitmap *m_BmpBackgroundPtr, *m_BmpBackground2Ptr, *m_BmpLevelForegroundPtr, 
		*m_BmpLevel2ForegroundPtr;

	Bitmap *m_BmpMainUIPtr, *m_BmpExtraUIPtr, *m_BmpHealthbarPtr;
	bool m_ShowExtraUI;

	DOUBLE2 m_Gravity;

	MATRIX3X2 m_MatCamera;

	double m_PlanetMass;
	DOUBLE2 m_PlanetPos;

	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	GameAMP2(const GameAMP2& tRef);
	GameAMP2& operator=(const GameAMP2& tRef);
};
