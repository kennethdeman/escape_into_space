//-----------------------------------------------------------------
// Game File
// C++ Source - GameAMP2.cpp - version v2_12 jan 2013
// Copyright Kevin Hoefman - kevin.hoefman@howest.be
// http://www.digitalartsandentertainment.be/
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "GameAMP2.h"																				

//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

//-----------------------------------------------------------------
// GameAMP2 methods																				
//-----------------------------------------------------------------

GameAMP2::GameAMP2()
	:m_HitLevelPtr(nullptr),m_HitLevelCircularPtr(nullptr),m_HitIndoorPtr(nullptr),

	m_BmpBackgroundPtr(nullptr), m_BmpBackground2Ptr(nullptr), m_BmpLevelForegroundPtr(nullptr),
	m_BmpLevel2ForegroundPtr(nullptr), m_Gravity(0,981/4),

	m_Fps(0),

	m_PlayerShipPtr(nullptr), m_MatCamera(0,0,0,0,0,0), 
	m_LaserListPtr(nullptr),m_MassProjListPtr(nullptr), m_GravityBallListPtr(nullptr), 
	m_PlanetPos(5000+3270,2330), m_PlanetMass(5.97219 * pow(10,11)*5),
	m_HitLevel2Ptr(nullptr), m_BmpMainUIPtr(nullptr), m_BmpExtraUIPtr(nullptr), m_BmpHealthbarPtr(nullptr),
	m_ShowExtraUI(false)

{
	for (int count=0; count < MAX_ROUNDSHIPS; ++count)
	{
		m_RoundShipPtrArr[count]=nullptr;
	}

}

GameAMP2::~GameAMP2()																						
{
	// nothing to destroy
}

void GameAMP2::GameInitialize(HINSTANCE hInstance)			
{
	// Set the required values
	AbstractGame::GameInitialize(hInstance);
	GAME_ENGINE->SetTitle("GameAMP2 - De Man Kenneth - 1DAE2");					
	GAME_ENGINE->RunGameLoop(true);

	// Set the optional values
	//GAME_ENGINE->SetWidth(1280);
	//GAME_ENGINE->SetHeight(720);
	GAME_ENGINE->SetWidth(1800);
	GAME_ENGINE->SetHeight(900);
	GAME_ENGINE->SetKeyList(String("QSDZG12") + (TCHAR) VK_F1 + (TCHAR) VK_SHIFT +  (TCHAR) VK_LEFT +  (TCHAR) VK_RIGHT
		+ (TCHAR) VK_NUMPAD0 + (TCHAR) VK_SPACE);
}

void GameAMP2::GameStart()
{
	m_PlayerShipPtr= new PlayerShip(100,200);

	m_RoundShipPtrArr[0]= new RoundShip(2100,750);
	m_RoundShipPtrArr[1]= new RoundShip(1600,1000);
	m_RoundShipPtrArr[2]= new RoundShip(1500,1700);
	m_RoundShipPtrArr[3]= new RoundShip(2700,300);
	m_RoundShipPtrArr[4]= new RoundShip(3700,700);
	m_RoundShipPtrArr[5]= new RoundShip(3000,4200);
	m_RoundShipPtrArr[5]= new RoundShip(2500,4200);
	//Hanger
	for (int count=10; count < MAX_ROUNDSHIPS-5; ++count)
	{
		m_RoundShipPtrArr[count]= new RoundShip(1800+300*count,3000+rand()%200);
	}

	m_LaserListPtr= new LaserList();

	m_MassProjListPtr= new MassProjList();

	m_GravityBallListPtr= new GravityBallList();

	m_HitLevelPtr = new HitRegion();
	m_HitLevelPtr->CreateFromSVG("./resources/Level.svg");


	m_HitLevelCircularPtr= new HitRegion();
	m_HitLevelCircularPtr->CreateFromSVG("./resources/LevelCirc.svg");

	m_HitIndoorPtr= new HitRegion();
	m_HitIndoorPtr->CreateFromSVG("./resources/Indoor.svg");

	m_HitLevel2Ptr= new HitRegion();
	m_HitLevel2Ptr->CreateFromSVG("./resources/Level2.svg");

	m_BmpBackgroundPtr = new Bitmap("./resources/LevelBackground.png");
	m_BmpLevelForegroundPtr = new Bitmap("./resources/Level.png");
	m_BmpBackground2Ptr= new Bitmap("./resources/LevelBackground2.png");
	m_BmpLevel2ForegroundPtr = new Bitmap("./resources/Level2.png");

	m_BmpMainUIPtr=new Bitmap("./resources/MainUI.png");

	m_BmpExtraUIPtr=new Bitmap("./resources/ExtraUI.png");

	m_BmpHealthbarPtr=new Bitmap("./resources/Healthbar.png");
}

void GameAMP2::GameEnd()
{
	delete m_PlayerShipPtr;
	m_PlayerShipPtr=nullptr;

	for (int count=0; count < MAX_ROUNDSHIPS; ++count)
	{
		delete m_RoundShipPtrArr[count];
		m_RoundShipPtrArr[count]=nullptr;
	}


	delete m_HitLevelPtr;
	m_HitLevelPtr=nullptr;

	delete m_HitLevelCircularPtr;
	m_HitLevelCircularPtr=nullptr;

	delete m_HitIndoorPtr;
	m_HitIndoorPtr=nullptr;

	delete m_HitLevel2Ptr;
	m_HitLevel2Ptr=nullptr;

	delete m_BmpBackgroundPtr;
	m_BmpBackgroundPtr=nullptr;

	delete m_BmpBackground2Ptr;
	m_BmpBackground2Ptr=nullptr;

	delete m_BmpLevelForegroundPtr;
	m_BmpLevelForegroundPtr=nullptr;

	delete m_BmpLevel2ForegroundPtr;
	m_BmpLevel2ForegroundPtr=nullptr;

	delete m_LaserListPtr;
	m_LaserListPtr=nullptr;

	delete m_MassProjListPtr;
	m_MassProjListPtr=nullptr;

	delete m_GravityBallListPtr;
	m_GravityBallListPtr=nullptr;

	delete m_BmpMainUIPtr;
	m_BmpMainUIPtr=nullptr;

	delete m_BmpExtraUIPtr;
	m_BmpExtraUIPtr=nullptr;

	delete m_BmpHealthbarPtr;
	m_BmpHealthbarPtr=nullptr;
}

void GameAMP2::MouseButtonAction(bool isLeft, bool isDown, int x, int y, WPARAM wParam)
{		


}

void GameAMP2::MouseMove(int x, int y, WPARAM wParam)
{	

}

void GameAMP2::CheckKeyboard()
{	
	if (GAME_ENGINE->IsKeyDown(VK_UP))
	{

		m_PlayerShipPtr->Shoot(m_LaserListPtr);

	}



	if (GAME_ENGINE->IsKeyDown('1'))
	{

		if (m_HitLevelCircularPtr->HitTest(m_PlayerShipPtr->GetShootingPos())==false && m_PlayerShipPtr->GetTimeWhenLastShot() == 0
			&& m_PlayerShipPtr->GetMassProjAmmo()>0)
		{
			MassProjectile *tempPtr;
			tempPtr= new MassProjectile(m_PlayerShipPtr->GetShootingPos().x, m_PlayerShipPtr->GetShootingPos().y,
				m_PlayerShipPtr->GetShootingAngle(), m_PlayerShipPtr->GetVelocity());
			m_MassProjListPtr->Add(tempPtr);

			m_PlayerShipPtr->SetTimeWhenLastShot(0.5);
			m_PlayerShipPtr->SubtractMassProjAmmo(1);
		}



	}



	if (GAME_ENGINE->IsKeyDown('2'))
	{



		if (m_PlayerShipPtr->GetTimeWhenLastShot() == 0 && m_PlayerShipPtr->GetGravBallAmmo()>0)
		{
			GravityBall *tempPtr;
			tempPtr= new GravityBall(m_PlayerShipPtr->GetShootingPos().x, m_PlayerShipPtr->GetShootingPos().y,
				m_PlayerShipPtr->GetShootingAngle(), m_PlayerShipPtr->GetVelocity());
			m_GravityBallListPtr->Add(tempPtr);

			m_PlayerShipPtr->SetTimeWhenLastShot(0.5);
			m_PlayerShipPtr->SubtractGravBallAmmo(1);
		}


	}

	if (GAME_ENGINE->IsKeyDown(VK_SHIFT))
	{
		m_PlayerShipPtr->TurnBackThrusterOn(true);
	}
	else
	{
		m_PlayerShipPtr->TurnBackThrusterOn(false);
	}

	if (GAME_ENGINE->IsKeyDown(VK_SPACE))
	{
		m_PlayerShipPtr->TurnBottomThrusterOn(true);
	}
	else
	{
		m_PlayerShipPtr->TurnBottomThrusterOn(false);
	}

	if (GAME_ENGINE->IsKeyDown(VK_LEFT)) 
	{
		m_PlayerShipPtr->TurnNoseThrusterUpOn(true);
	}
	else
	{
		m_PlayerShipPtr->TurnNoseThrusterUpOn(false);
	}

	if (GAME_ENGINE->IsKeyDown(VK_RIGHT))
	{
		m_PlayerShipPtr->TurnNoseThrusterDownOn(true);
	}
	else
	{
		m_PlayerShipPtr->TurnNoseThrusterDownOn(false);
	}

	if (GAME_ENGINE->IsKeyDown('Q') || GAME_ENGINE->IsKeyDown('A'))
	{
		m_PlayerShipPtr->SetDirectionLeft();
	}
	else if (GAME_ENGINE->IsKeyDown('D'))
	{
		m_PlayerShipPtr->SetDirectionRight();
	}

	if (GAME_ENGINE->IsKeyDown(VK_F1))
	{
		m_ShowExtraUI=!m_ShowExtraUI;
	}


	if (GAME_ENGINE->IsKeyDown(VK_NUMPAD8))
	{
		m_PlayerShipPtr->GodModeMoveUp();
	}
	
	if (GAME_ENGINE->IsKeyDown(VK_NUMPAD5))
	{
		m_PlayerShipPtr->GodModeMoveDown();
	}
	
	if (GAME_ENGINE->IsKeyDown(VK_NUMPAD4))
	{
		m_PlayerShipPtr->GodModeMoveLeft();
	}
	
	
	if (GAME_ENGINE->IsKeyDown(VK_NUMPAD6))
	{
		m_PlayerShipPtr->GodModeMoveRight();
	}
	

}

void GameAMP2::KeyPressed(TCHAR cKey)
{	


	switch (cKey)
	{
	case 'G':
		m_PlayerShipPtr->ToggleGodmode();
		break;

	}

}

void GameAMP2::GameTick(double deltaTime)
{
	//Slow motion effects
	deltaTime*=0.4;

	//Calculate fps
	m_Fps=1/deltaTime;


	//Player
	m_PlayerShipPtr->Tick(deltaTime);

	//If the player is in the ship apply gravity of the ship
	if (m_HitIndoorPtr->HitTest(m_PlayerShipPtr->GetPos())==true) m_PlayerShipPtr->AddGravity(m_Gravity);

	m_PlayerShipPtr->AddGravity(CalculatePlanetGravity(m_PlayerShipPtr->GetPos(),m_PlayerShipPtr->GetMass()));

	//Gravity ball forces
	m_PlayerShipPtr->AddForce(
		m_GravityBallListPtr->ExerciseForces(m_PlayerShipPtr->GetPos(),m_PlayerShipPtr->GetMass()));
	DOUBLE2 explosionForce=m_GravityBallListPtr->CastOutExplosionForces(m_PlayerShipPtr->GetPos());
	m_PlayerShipPtr->AddForce(explosionForce);
	m_PlayerShipPtr->SubtractHealth(ExplosionDamage(explosionForce));

	m_PlayerShipPtr->HandleCollision(m_HitLevelPtr, deltaTime);
	m_PlayerShipPtr->HandleCollision(m_HitLevel2Ptr, deltaTime);


	m_PlayerShipPtr->SubtractHealth(m_LaserListPtr->CheckHitCollision(m_PlayerShipPtr->GetHitRegion(),
		m_PlayerShipPtr->GetPos()));

	for (int count=0; count < MAX_ROUNDSHIPS; ++count)
	{
		if (m_RoundShipPtrArr[count]!=nullptr)
		{
			m_RoundShipPtrArr[count]->LineOfSight(m_PlayerShipPtr->GetPos(), m_HitLevelPtr);
			m_RoundShipPtrArr[count]->Tick(deltaTime);
			if (m_HitIndoorPtr->HitTest(m_RoundShipPtrArr[count]->GetPos())==true)  m_RoundShipPtrArr[count]->AddGravity(m_Gravity);	

			//Gravity ball forces
			m_RoundShipPtrArr[count]->AddForce(
				m_GravityBallListPtr->ExerciseForces(m_RoundShipPtrArr[count]->GetPos(),m_RoundShipPtrArr[count]->GetMass()));
			DOUBLE2 explosionForce=m_GravityBallListPtr->CastOutExplosionForces(m_RoundShipPtrArr[count]->GetPos());
			m_RoundShipPtrArr[count]->AddForce(explosionForce);
			m_RoundShipPtrArr[count]->SubtractHealth(ExplosionDamage(explosionForce));




			//Mass Proj / Round Ship Collision
			DOUBLE2 newVelocity=m_MassProjListPtr->HandleCircularCollision(
				m_RoundShipPtrArr[count]->GetPos(),m_RoundShipPtrArr[count]->GetVelocity() ,
				m_RoundShipPtrArr[count]->GetRadius(), m_RoundShipPtrArr[count]->GetMass() );
			if (newVelocity!=DOUBLE2(0,0))
				m_RoundShipPtrArr[count]->SetVelocity(newVelocity);


			//Grav Ball / Round Ship Collision
			newVelocity=m_GravityBallListPtr->HandleCircularCollision(
				m_RoundShipPtrArr[count]->GetPos(),m_RoundShipPtrArr[count]->GetVelocity() ,
				m_RoundShipPtrArr[count]->GetRadius(), m_RoundShipPtrArr[count]->GetMass() );
			if (newVelocity!=DOUBLE2(0,0))
				m_RoundShipPtrArr[count]->SetVelocity(newVelocity);

			//Circular collision with other round ships
			for(int count2 = count + 1; count2<MAX_ROUNDSHIPS; ++count2)
			{		
				if (m_RoundShipPtrArr[count2]!=nullptr)
				{
					DOUBLE2 newVelocity=m_RoundShipPtrArr[count]->HandleCircularCollision(m_RoundShipPtrArr[count2]->GetPos(),
						m_RoundShipPtrArr[count2]->GetVelocity(), m_RoundShipPtrArr[count2]->GetRadius());
					if (newVelocity!=DOUBLE2(0,0))
						m_RoundShipPtrArr[count2]->SetVelocity(newVelocity);
				}
			}

			m_RoundShipPtrArr[count]->HandleCollision(m_HitLevelCircularPtr, deltaTime);

			//Check damage taken from laser shots
			m_RoundShipPtrArr[count]->SubtractHealth(m_LaserListPtr->CheckHitCollision(m_RoundShipPtrArr[count]->GetHitRegion(),
				m_RoundShipPtrArr[count]->GetPos()));


			//Makes the ship shoot at the player
			m_RoundShipPtrArr[count]->Shoot(m_LaserListPtr);

			//Checks if the ship is ready for deletion
			if (m_RoundShipPtrArr[count]->IsReadyToBeDeleted()==true)
			{
				delete m_RoundShipPtrArr[count];
				m_RoundShipPtrArr[count]=nullptr;
			}
		}

	}


	//Explosions

	bool didPlayerExplode=false;
	for (int count=0; count < MAX_ROUNDSHIPS; ++count)
	{		


		if ( m_RoundShipPtrArr[count]!=nullptr)
		{
			bool didRoundShipExplode=false;
			DOUBLE2 explosionForce;

			if ( m_RoundShipPtrArr[count]->IsExploding()==true)
			{


				//Explosion force on player
				explosionForce=m_RoundShipPtrArr[count]->CastOutExplosionForce(m_PlayerShipPtr->GetPos());
				m_PlayerShipPtr->AddForce(explosionForce);
				m_PlayerShipPtr->SubtractHealth(ExplosionDamage(explosionForce));


				//Explosion force on other round ships
				for (int count2=0; count2 < MAX_ROUNDSHIPS; ++count2)
				{
					if (m_RoundShipPtrArr[count2]!=nullptr)
					{
						explosionForce=m_RoundShipPtrArr[count]->CastOutExplosionForce(
							m_RoundShipPtrArr[count2]->GetPos());
						m_RoundShipPtrArr[count2]->AddForce(explosionForce);
						m_RoundShipPtrArr[count2]->SubtractHealth(ExplosionDamage(explosionForce));
					}

				}

				didRoundShipExplode=true;
			}

			//Makes sure the ships don't keep sending out explosion forces the next ticks while he is exploding
			if (didRoundShipExplode==true) m_RoundShipPtrArr[count]->HasExploded();


			//Explosion force from playership
			if ( m_PlayerShipPtr->IsExploding()==true)
			{
				explosionForce=m_PlayerShipPtr->CastOutExplosionForce(m_RoundShipPtrArr[count]->GetPos() );
				m_RoundShipPtrArr[count]->AddForce(explosionForce);
				m_RoundShipPtrArr[count]->SubtractHealth(ExplosionDamage(explosionForce));

				didPlayerExplode=true;
			}

		}

	}

	//Makes sure the playership doesn't keep sending out explosion forces the next ticks while he is exploding
	if (didPlayerExplode==true) m_PlayerShipPtr->HasExploded();


	//Handles the laser shots already fired	

	m_LaserListPtr->Tick(deltaTime);
	m_LaserListPtr->HandleCollision(m_HitLevelPtr, deltaTime);
	m_LaserListPtr->HandleCollision(m_HitLevel2Ptr, deltaTime);

	m_MassProjListPtr->Tick(deltaTime);
	m_MassProjListPtr->HandleCollision(m_HitLevelCircularPtr,deltaTime);	

	m_GravityBallListPtr->Tick(deltaTime);
	m_GravityBallListPtr->HandleCollision(m_HitLevelPtr,deltaTime);



}

void GameAMP2::GamePaint(RECT rect)
{
	GAME_ENGINE->DrawSolidBackground(15,15,15);

	GAME_ENGINE->SetTransformMatrix(MATRIX3X2::CreateIdentityMatrix());

	double scale=1;

	MATRIX3X2 matCamera, matCameraScale;

	DOUBLE2 cameraPos= DOUBLE2(m_PlayerShipPtr->GetPos().x - GAME_ENGINE->GetWidth()/2/scale, 
		m_PlayerShipPtr->GetPos().y - GAME_ENGINE->GetHeight()/2/scale);


	int width = m_BmpLevelForegroundPtr->GetWidth()*2-GAME_ENGINE->GetWidth();
	int height = m_BmpLevelForegroundPtr->GetHeight()-GAME_ENGINE->GetHeight();

	//Prevent camera from going outside the level
	if (cameraPos.x < 0) cameraPos.x=0;
	if (cameraPos.x > width) cameraPos.x=width;
	if (cameraPos.y < 0) cameraPos.y=0;
	if (cameraPos.y >height) cameraPos.y=height;

	matCamera.SetAsTranslate(cameraPos);
	matCameraScale.SetAsScale(scale,scale);
	MATRIX3X2 matView=matCamera.Inverse() * matCameraScale ;

	m_MatCamera=matView.Inverse();

	GAME_ENGINE->SetTransformMatrix(matView);
	GAME_ENGINE->DrawBitmap(m_BmpBackgroundPtr,0,0);

	MATRIX3X2 matBackground2;

	matBackground2.SetAsTranslate(m_BmpBackgroundPtr->GetWidth(),0);

	GAME_ENGINE->SetTransformMatrix(matBackground2*matView);
	GAME_ENGINE->DrawBitmap(m_BmpBackground2Ptr,0,0);


	m_PlayerShipPtr->Paint(matView);	

	for (int count=0; count < MAX_ROUNDSHIPS; ++count)
	{
		if (m_RoundShipPtrArr[count]!=nullptr)
			m_RoundShipPtrArr[count]->Paint(matView);
	}

	//LaserShots
	m_LaserListPtr->Paint(matView);


	//MassProjectiles
	m_MassProjListPtr->Paint(matView);	

	//GravityBalls
	m_GravityBallListPtr->Paint(matView);

	GAME_ENGINE->SetTransformMatrix(matView);

	//Foreground
	GAME_ENGINE->DrawBitmap(m_BmpLevelForegroundPtr,0,0);

	GAME_ENGINE->SetTransformMatrix(matBackground2*matView);
	GAME_ENGINE->DrawBitmap(m_BmpLevel2ForegroundPtr,0,0);

	//GAME_ENGINE->SetColor(COLOR(255,0,0,127));
	//GAME_ENGINE->FillHitRegion(m_HitLevelPtr);

	//GAME_ENGINE->SetColor(COLOR(0,0,255,127));
	//GAME_ENGINE->FillHitRegion(m_HitLevelCircularPtr);


	//Hud

	GAME_ENGINE->SetTransformMatrix(MATRIX3X2::CreateIdentityMatrix());
	GAME_ENGINE->DrawBitmap(m_BmpMainUIPtr,0,0);

	MATRIX3X2 matHealthbarTranslate;
	matHealthbarTranslate.SetAsTranslate(13,831);

	RECT frame;

	frame.top=0;
	frame.bottom=m_BmpHealthbarPtr->GetHeight();
	frame.left=0;
	frame.right=m_BmpHealthbarPtr->GetWidth()/m_PlayerShipPtr->GetMaxHealth()*m_PlayerShipPtr->GetHealth();

	GAME_ENGINE->SetTransformMatrix(matHealthbarTranslate);
	GAME_ENGINE->DrawBitmap(m_BmpHealthbarPtr,0,0,frame);

	double hudScale=2;
	MATRIX3X2 matHudScale;
	matHudScale.SetAsScale(2);


	GAME_ENGINE->SetTransformMatrix(matHudScale);

	GAME_ENGINE->SetColor(COLOR(255,255,255));
	GAME_ENGINE->DrawString(String("") + m_PlayerShipPtr->GetMassProjAmmo(),350,GAME_ENGINE->GetHeight()/hudScale-7*hudScale);

	GAME_ENGINE->DrawString(String("") + m_PlayerShipPtr->GetGravBallAmmo(),400,GAME_ENGINE->GetHeight()/hudScale-7*hudScale);


	if (m_ShowExtraUI==true)
	{
		GAME_ENGINE->SetTransformMatrix(MATRIX3X2::CreateIdentityMatrix());
		GAME_ENGINE->DrawBitmap(m_BmpExtraUIPtr,0,0);




		GAME_ENGINE->SetTransformMatrix(matHudScale);



		GAME_ENGINE->DrawString(String("Angle: ") +m_PlayerShipPtr->GetAngle(), 10,10);

		GAME_ENGINE->DrawString(String("Speed: ") + m_PlayerShipPtr->GetSpeed(), 10,30);

		GAME_ENGINE->DrawString(String("Position: ") + m_PlayerShipPtr->GetPos().ToString(),10,50);

		GAME_ENGINE->DrawString(String("Shift: BackThruster \nSpace: BottomThruster \nQ or A: Turn Left \nD: Turn Right \nLeft/Right: Rotate \nUp: Shoot \n1: Shoot Mass Projectile \n2: Shoot Gravity Ball"),10,80);

		GAME_ENGINE->DrawString(String("fps: ") + m_Fps,GAME_ENGINE->GetWidth()/hudScale-50*hudScale,0);

	}








}

//void GameAMP2::CallAction(Caller* callerPtr)
//{
//	// Insert the code that needs to be executed when a Caller has to perform an action
//}

double GameAMP2::ExplosionDamage(DOUBLE2 explosionForce)
{
	return explosionForce.Length()/600;
}

DOUBLE2 GameAMP2::CalculatePlanetGravity(DOUBLE2 objectPos, double objectMass)
{
	if ( (m_PlanetPos-objectPos).Length() < 3000)
	{
		//G * m1*m2*rn12/|r12|2
		double g=0.000000000066742;

		double distance = (objectPos-m_PlanetPos).Length();

		DOUBLE2 force= g *(m_PlanetMass * objectMass   / distance*distance) * (m_PlanetPos-objectPos).Normalized() ;

		return force;

	}


	return DOUBLE2(0,0);
}



