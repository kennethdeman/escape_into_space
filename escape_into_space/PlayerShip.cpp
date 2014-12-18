//---------------------------
// Includes
//---------------------------
#include "PlayerShip.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

Bitmap* PlayerShip::m_BmpPlayerShipPtr=nullptr;
Bitmap* PlayerShip::m_BmpBackThrusterPtr=nullptr;
Bitmap* PlayerShip::m_BmpBottomThrusterPtr=nullptr;
Bitmap* PlayerShip::m_BmpNoseThrusterDownPtr=nullptr;
Bitmap* PlayerShip::m_BmpNoseThrusterUpPtr=nullptr;
Bitmap* PlayerShip::m_BmpExplosionPtr=nullptr;

//---------------------------
// Constructor & Destructor
//---------------------------
PlayerShip::PlayerShip(int x, int y): m_Pos(x,y), m_Angle(0), m_Velocity(0,0),
	m_IsBackThrusterOn(false), 
	m_IsBottomThrusterOn(false), m_IsNoseThrusterDownOn(false), m_IsNoseThrusterUpOn(false),
	m_BackThrusterForce(0,0), m_BackThrusterPower(1200), m_BottomThrusterPower(1200),
	m_BottomThrusterForce(0,0), m_BackThrusterAngle(0), m_BottomThrusterAngle(0), 
	m_CurrentDirection(DIRECTION_RIGHT), m_Acceleration(0,0), m_SumOfAccelerations(0,0), m_TimeWhenLastShot(0),
	m_Point1(0,0),m_Point2(0,0),m_Point3(0,0),m_Point4(0,0),m_Point5(0,0),
	m_Vector1(0,0),m_Vector2(0,0), m_Vector3(0,0), m_Vector4(0,0), m_Vector5(0,0),m_CollisionForce(0,0),
	m_CollisionFriction(0,0),m_FrontPos(0,0),m_BackPos(0,0), m_TopPos(0,0), m_BottomPos(0,0)


	,m_HitRegionPtr(nullptr), m_HasCastedOutExplosionForce(false), m_Godmode(false),

	m_CurrentState(STATE_WORKING),m_MaxHealth(350),m_Health(350), m_TicksUntilExplosion(50)
	, m_MassProjAmmo(7), m_GravBallAmmo(2),


	m_TickCount(0) ,m_SlowDownAnimations(0)


{

	if (PlayerShip::m_BmpPlayerShipPtr==nullptr)
	{
		PlayerShip::m_BmpPlayerShipPtr= new Bitmap("./resources/PlayerShip.png");
	}

	if (PlayerShip::m_BmpBackThrusterPtr==nullptr)
	{
		PlayerShip::m_BmpBackThrusterPtr= new Bitmap("./resources/BackThruster.png");
	}

	if (PlayerShip::m_BmpBottomThrusterPtr==nullptr)
	{
		PlayerShip::m_BmpBottomThrusterPtr= new Bitmap("./resources/BottomThruster.png");
	}

	if (PlayerShip::m_BmpNoseThrusterDownPtr==nullptr)
	{
		PlayerShip::m_BmpNoseThrusterDownPtr= new Bitmap("./resources/NoseThrusterDown.png");
	}

	if (PlayerShip::m_BmpNoseThrusterUpPtr==nullptr)
	{
		PlayerShip::m_BmpNoseThrusterUpPtr= new Bitmap("./resources/NoseThrusterUp.png");
	}

	if (PlayerShip::m_BmpExplosionPtr==nullptr)
	{
		PlayerShip::m_BmpExplosionPtr= new Bitmap("./resources/Explosion.png");
		PlayerShip::m_BmpExplosionPtr->SetTransparencyColor(0,0,0);
	}

	m_HitRegionPoints[0]=DOUBLE2(-33,7);
	m_HitRegionPoints[1]=DOUBLE2(-3,-12);
	m_HitRegionPoints[2]=DOUBLE2(53,14);

	m_HitRegionPtr= new HitRegion();
	m_HitRegionPtr->CreateFromPolygon(m_HitRegionPoints,MAX_POINTS,true);

}

PlayerShip::~PlayerShip()
{

}

//---------------------------
// Own methods
//---------------------------

void PlayerShip::Tick(double deltaTime)
{
	//Makes sure the angle never becomes higher than 360 degrees or lower than -360
	if (m_Angle>M_PI) m_Angle=-M_PI+(m_Angle-M_PI);
	else if (m_Angle<-M_PI) m_Angle=M_PI-(m_Angle+M_PI);


	//Position Hitbox	
	PositionHitRegion();


	if (m_CurrentState==STATE_WORKING)
	{



		//Thruster for the angle
		if (m_IsNoseThrusterDownOn==true)
		{
			m_Angle+=3*deltaTime;
		}

		if (m_IsNoseThrusterUpOn==true)
		{
			m_Angle-=3*deltaTime;
		}

		if (m_TimeWhenLastShot>0)
			m_TimeWhenLastShot-=1*deltaTime;
		else
			m_TimeWhenLastShot=0;

		if (m_CurrentDirection==DIRECTION_RIGHT)
		{
			m_BackThrusterAngle=m_Angle;

		}
		else
		{
			m_BackThrusterAngle=m_Angle+M_PI;

		}

		m_BottomThrusterAngle=m_Angle-M_PI/2;

		PositionsRaycast();		

		m_BackThrusterForce.x = m_BackThrusterPower * cos(m_BackThrusterAngle);
		m_BackThrusterForce.y = m_BackThrusterPower * sin(m_BackThrusterAngle);

		m_BottomThrusterForce.x = m_BottomThrusterPower * cos(m_BottomThrusterAngle);
		m_BottomThrusterForce.y = m_BottomThrusterPower * sin(m_BottomThrusterAngle);


		if (m_IsBackThrusterOn==true)
		{
			m_Acceleration+= m_BackThrusterForce;

		}

		if (m_IsBottomThrusterOn==true)
		{
			m_Acceleration+= m_BottomThrusterForce;
		}


	}



	if (m_CurrentState!=STATE_EXPLODING)
	{
		//Friction
		int k=m_Velocity.Length()/2;

		DOUBLE2 frictionForce = k * (-m_Velocity.Normalized());
		m_Acceleration += frictionForce;

		m_Acceleration/=MASS;

		//Gravity is in m_SumOfAccelerations
		m_SumOfAccelerations+=m_Acceleration;

		// Key input setting the speed. 
		m_Velocity += m_SumOfAccelerations * deltaTime;			

		//Resetting acceleration every tick
		m_Acceleration= DOUBLE2(0,0);
		m_SumOfAccelerations= DOUBLE2(0,0);

		if (m_Velocity.Length()>1200)
			m_Velocity=m_Velocity.Normalized()*1200;
		m_Pos += m_Velocity * deltaTime;
	}	


	if (m_CurrentState==STATE_FALLING)
	{

		if (m_SlowDownAnimations>0)
		{
			m_SlowDownAnimations=RESET_SLOW_DOWN;

			if (m_TickCount<m_TicksUntilExplosion)
			{
				m_TickCount+=1;

			}
			else
			{
				m_TickCount=0;
				m_CurrentState=STATE_EXPLODING;
			}



		}
		m_SlowDownAnimations+=1500*deltaTime;

	}
	if (m_CurrentState==STATE_EXPLODING)
	{

		if (m_SlowDownAnimations>0)
		{
			m_SlowDownAnimations=RESET_SLOW_DOWN;

			if (m_TickCount<EXPLOSION_FRAMES)
			{
				m_TickCount+=1;

			}



		}
		m_SlowDownAnimations+=EXPLOSION_ANIMATION_SPEED*deltaTime;

	}


}

void PlayerShip::Paint(MATRIX3X2 matTransform)
{

	MATRIX3X2 matPlayerCenter, matPlayerRotate, matPlayerScale, matPlayerTranslate,matPlayerTransform;

	matPlayerCenter.SetAsTranslate(-m_BmpPlayerShipPtr->GetWidth()/2,-m_BmpPlayerShipPtr->GetHeight()/2);
	matPlayerRotate.SetAsRotate(m_Angle);

	if (m_CurrentDirection==DIRECTION_RIGHT)
	{
		matPlayerScale.SetAsScale(1,1);
	}
	else
	{
		matPlayerScale.SetAsScale(-1,1);
	}	

	matPlayerTranslate.SetAsTranslate(m_Pos);	

	matPlayerTransform=matPlayerCenter *  matPlayerScale * matPlayerRotate * matPlayerTranslate * matTransform;

	GAME_ENGINE->SetTransformMatrix(matPlayerTransform);

	if (m_CurrentState==STATE_WORKING)
	{

		if (m_IsBackThrusterOn==true)
		{
			GAME_ENGINE->DrawBitmap(m_BmpBackThrusterPtr,0,0);
		}

		if (m_IsBottomThrusterOn==true)
		{			
			GAME_ENGINE->DrawBitmap(m_BmpBottomThrusterPtr,0,0);
		}

		if (m_IsNoseThrusterDownOn==true)
		{
			if (m_CurrentDirection==DIRECTION_RIGHT)
				GAME_ENGINE->DrawBitmap(m_BmpNoseThrusterDownPtr,0,0);
			else
				GAME_ENGINE->DrawBitmap(m_BmpNoseThrusterUpPtr,0,0);
		}

		if (m_IsNoseThrusterUpOn==true)
		{
			if (m_CurrentDirection==DIRECTION_RIGHT)
				GAME_ENGINE->DrawBitmap(m_BmpNoseThrusterUpPtr,0,0);
			else
				GAME_ENGINE->DrawBitmap(m_BmpNoseThrusterDownPtr,0,0);
		}

	}

	if (m_CurrentState==STATE_WORKING || m_CurrentState==STATE_FALLING)
	{
		GAME_ENGINE->DrawBitmap(m_BmpPlayerShipPtr,0,0);
	}

	if (m_CurrentState==STATE_EXPLODING)
	{
		RECT frame;

		int row= m_TickCount%4;
		int column= m_TickCount/4;

		frame.top=m_BmpExplosionPtr->GetHeight()/4*column;
		frame.bottom=m_BmpExplosionPtr->GetHeight()/4+m_BmpExplosionPtr->GetHeight()/4*column;
		frame.left=m_BmpExplosionPtr->GetWidth()/4*row;
		frame.right=m_BmpExplosionPtr->GetWidth()/4+m_BmpExplosionPtr->GetWidth()/4*row;

		MATRIX3X2 matExplosionCenter, matExplosionTransform;

		matExplosionCenter.SetAsTranslate(-m_BmpExplosionPtr->GetWidth()/4/2,
			-m_BmpExplosionPtr->GetHeight()/4/2);

		matExplosionTransform=matExplosionCenter *  matPlayerScale * matPlayerRotate * matPlayerTranslate * matTransform;

		GAME_ENGINE->SetTransformMatrix(matExplosionTransform);
		GAME_ENGINE->DrawBitmap(m_BmpExplosionPtr,0,0,frame);
	}

	//GAME_ENGINE->SetTransformMatrix(matTransform);
	//
	//GAME_ENGINE->SetColor(COLOR(0,255,0));
	//
	//GAME_ENGINE->DrawLine(m_Pos, m_Pos + m_Velocity);


	////Raycast vectors
	//GAME_ENGINE->SetColor(COLOR(255,0,255));
	//GAME_ENGINE->DrawLine(m_Point1,m_Point1+m_Vector1);
	//
	//GAME_ENGINE->SetColor(COLOR(0,255,255));
	//GAME_ENGINE->DrawLine(m_Point2,m_Point2+m_Vector2);
	//
	//GAME_ENGINE->SetColor(COLOR(180,180,0));
	//GAME_ENGINE->DrawLine(m_Point3,m_Point3+m_Vector3);
	//					  
	//GAME_ENGINE->SetColor(COLOR(180,255,180));
	//GAME_ENGINE->DrawLine(m_Point4,m_Point4+m_Vector4);
	//
	//GAME_ENGINE->SetColor(COLOR(100,255,255));
	//GAME_ENGINE->DrawLine(m_Point5,m_Point5+m_Vector5);

	//
	//GAME_ENGINE->DrawLine(m_Pos,m_Pos+m_CollisionFriction);
	//GAME_ENGINE->SetColor(COLOR(255,0,0));
	//GAME_ENGINE->DrawLine(m_Pos,m_Pos+m_CollisionForce);

	//GAME_ENGINE->FillEllipse(m_FrontPos,3,3);

	//GAME_ENGINE->FillEllipse(m_BackPos,3,3);

	//GAME_ENGINE->FillEllipse(m_TopPos,3,3);

	//GAME_ENGINE->FillEllipse(m_BottomPos,3,3);

	////Collision lines

	////Hitregion
	////GAME_ENGINE->SetColor(COLOR(255,0,0,127));
	////GAME_ENGINE->FillHitRegion(m_HitRegionPtr);

	//GAME_ENGINE->SetTransformMatrix(MATRIX3X2::CreateIdentityMatrix());
	//GAME_ENGINE->DrawString(String("") + m_Velocity.ToString(),200,200);



}


DOUBLE2 PlayerShip::GetPos()
{
	return m_Pos;
}

bool PlayerShip::Hit(HitRegion* hitTargetPtr)
{
	return hitTargetPtr->HitTest(m_Pos);
}

void PlayerShip::HandleCollision(HitRegion *hitLevelPtr, double deltaTime)
{
	RaycastCollision(m_Point1, m_Vector1, m_BottomPos, hitLevelPtr, deltaTime);
	RaycastCollision(m_Point1+m_Vector1, -m_Vector1, m_BottomPos, hitLevelPtr, deltaTime);

	RaycastCollision(m_Point2, m_Vector2, m_TopPos, hitLevelPtr, deltaTime);
	RaycastCollision(m_Point2+m_Vector2, -m_Vector2, m_TopPos, hitLevelPtr, deltaTime);

	RaycastCollision(m_Point3, m_Vector3, m_FrontPos, hitLevelPtr, deltaTime);
	RaycastCollision(m_Point3+m_Vector3, -m_Vector3, m_FrontPos, hitLevelPtr, deltaTime);

	RaycastCollision(m_Point4, m_Vector4, m_BackPos, hitLevelPtr, deltaTime);
	RaycastCollision(m_Point4+m_Vector4, -m_Vector4, m_BackPos, hitLevelPtr, deltaTime);



	RaycastCollision(m_Point5, m_Vector5, m_MidPos, hitLevelPtr, deltaTime);
	RaycastCollision(m_Point5+m_Vector5, -m_Vector5, m_MidPos, hitLevelPtr, deltaTime);

	PositionsRaycast();
	PositionHitRegion();
}

void PlayerShip::ToggleGodmode()
{
	m_Godmode=!m_Godmode;
	if (m_Godmode==true)
	{
		m_CurrentState=STATE_WORKING;
		m_Health=m_MaxHealth;
	}

}


void PlayerShip::PositionsRaycast()
{
	m_FrontPos.x = m_Pos.x + 40 * cos(m_BackThrusterAngle);
	m_FrontPos.y = m_Pos.y + 40 * sin(m_BackThrusterAngle);

	m_BackPos.x = m_Pos.x - 25 * cos(m_BackThrusterAngle);
	m_BackPos.y = m_Pos.y - 25 * sin(m_BackThrusterAngle);

	m_TopPos.x = m_Pos.x + 5 * cos(m_BottomThrusterAngle);
	m_TopPos.y = m_Pos.y + 5 * sin(m_BottomThrusterAngle);

	m_BottomPos.x = m_Pos.x - 5 * cos(m_BottomThrusterAngle);
	m_BottomPos.y = m_Pos.y - 5 * sin(m_BottomThrusterAngle);

	m_MidPos.x = m_Pos.x + 8 * cos(m_BackThrusterAngle);
	m_MidPos.y = m_Pos.y + 8 * sin(m_BackThrusterAngle);

	m_Point1.x = m_BottomPos.x - 35 * cos(m_BackThrusterAngle);
	m_Point1.y = m_BottomPos.y - 35 * sin(m_BackThrusterAngle);

	m_Vector1.x = 90 * cos(m_BackThrusterAngle);
	m_Vector1.y = 90 * sin(m_BackThrusterAngle);

	m_Point2.x = m_TopPos.x - 30 * cos(m_BackThrusterAngle);
	m_Point2.y = m_TopPos.y - 30 * sin(m_BackThrusterAngle);

	m_Vector2.x = 45 * cos(m_BackThrusterAngle);
	m_Vector2.y = 45 * sin(m_BackThrusterAngle);


	m_Point3.x = m_FrontPos.x;
	m_Point3.y = m_FrontPos.y;

	m_Vector3.x = 20 * cos(m_Angle+M_PI/2);
	m_Vector3.y = 20 * sin(m_Angle+M_PI/2);


	m_Point4.x = m_BackPos.x - 10 * cos(m_Angle+M_PI/2);
	m_Point4.y = m_BackPos.y - 10 * sin(m_Angle+M_PI/2);

	m_Vector4.x = 23 * cos(m_Angle+M_PI/2);
	m_Vector4.y = 23 * sin(m_Angle+M_PI/2);

	m_Point5.x = m_MidPos.x - 12 * cos(m_Angle+M_PI/2);
	m_Point5.y = m_MidPos.y - 12 * sin(m_Angle+M_PI/2);

	m_Vector5.x = 28 * cos(m_Angle+M_PI/2);
	m_Vector5.y = 28 * sin(m_Angle+M_PI/2);
}


void PlayerShip::RaycastCollision(DOUBLE2 point, DOUBLE2 vector, DOUBLE2 refPos, HitRegion* hitRegion, double deltaTime)
{
	HIT hitArr[1];
	DOUBLE2 projected;

	if (hitRegion->Raycast(point, vector, hitArr, 1, -1)>=1 && m_Godmode==false)	
	{	

		DOUBLE2 normal = DOUBLE2(hitArr[0].normal);

		double dotProduct=normal.DotProduct(m_Velocity);
		projected=-normal*normal.DotProduct(m_Velocity);

		m_CollisionForce=projected;

		m_Velocity+=projected;


		DOUBLE2 ortho = normal.Orthogonal();
		ortho=ortho.Normalized();

		//Friction
		DOUBLE2 friction=-ortho.Normalized()*ortho.DotProduct(m_Velocity*7);

		m_CollisionFriction=friction;
		m_Acceleration+=friction;

		DOUBLE2 reduceLength=refPos-point;

		m_Pos=hitArr[0].point - vector.Normalized()*(vector.Length()-reduceLength.Length())
			-(refPos-m_Pos);


	}
	else 
	{

	}
}


void PlayerShip::AddForce(DOUBLE2 force)
{
	m_Acceleration+=force;
}

void PlayerShip::AddGravity(DOUBLE2 gravity)
{
	m_SumOfAccelerations+=gravity;
}


void PlayerShip::TurnBackThrusterOn (bool isOn)
{
	if (m_CurrentState==STATE_WORKING)	m_IsBackThrusterOn=isOn;
}

void PlayerShip::TurnBottomThrusterOn (bool isOn)
{
	if (m_CurrentState==STATE_WORKING)	m_IsBottomThrusterOn=isOn;
}

void PlayerShip::TurnNoseThrusterUpOn (bool isOn)
{
	if (m_CurrentState==STATE_WORKING)	m_IsNoseThrusterUpOn=isOn;
}

void PlayerShip::TurnNoseThrusterDownOn (bool isOn)
{
	if (m_CurrentState==STATE_WORKING)	m_IsNoseThrusterDownOn=isOn;
}


void PlayerShip::SetDirectionLeft()
{
	if (m_CurrentState==STATE_WORKING)	m_CurrentDirection=DIRECTION_LEFT;
}

void PlayerShip::SetDirectionRight()
{
	if (m_CurrentState==STATE_WORKING)	m_CurrentDirection=DIRECTION_RIGHT;
}


void PlayerShip::SetTimeWhenLastShot(double time)
{
	m_TimeWhenLastShot=time;
}

double PlayerShip::GetTimeWhenLastShot()
{
	return m_TimeWhenLastShot;
}


void PlayerShip::SetVelocity(DOUBLE2 velocity)
{
	m_Velocity=velocity;
}

DOUBLE2 PlayerShip::GetVelocity()
{
	return m_Velocity;
}

double PlayerShip::GetMass()
{
	return MASS;
}



DOUBLE2 PlayerShip::GetAcceleration()
{
	return m_Acceleration;
}

void PlayerShip::Shoot(LaserList* laserListPtr)
{
	LaserShot *tempPtr;

	tempPtr= new LaserShot(GetShootingPos().x, GetShootingPos().y, GetShootingAngle(), GetVelocity());

	if (GetTimeWhenLastShot() == 0 )
	{	

		laserListPtr->Add(tempPtr);
		SetTimeWhenLastShot(0.5);

	}
}



DOUBLE2 PlayerShip::GetShootingPos()
{
	return m_FrontPos;
}



double PlayerShip::GetShootingAngle()
{
	return m_BackThrusterAngle;
}

void PlayerShip::SetAngleWithVector(DOUBLE2 vector)
{
	if (m_CurrentDirection==DIRECTION_RIGHT)
		m_Angle=-vector.AngleWith(DOUBLE2(1,0));
	else m_Angle=-vector.AngleWith(DOUBLE2(-1,0));
}



double PlayerShip::GetAngle()
{
	return m_Angle;
}


double PlayerShip::GetSpeed()
{
	return m_Velocity.Length();
}

HitRegion* PlayerShip::GetHitRegion()
{
	return m_HitRegionPtr;
}

void PlayerShip::PositionHitRegion()
{
	MATRIX3X2 matScale,matRotate, matTranslate;

	if (m_CurrentDirection==DIRECTION_RIGHT)
	{
		matScale.SetAsScale(1,1);
	}
	else
	{
		matScale.SetAsScale(-1,1);
	}	

	matRotate.SetAsRotate(m_Angle);
	matTranslate.SetAsTranslate(m_Pos);
	m_HitRegionPtr->SetTransformMatrix(matScale*matRotate*matTranslate);
}


void PlayerShip::SubtractHealth(double amount)
{

	double overkill=amount-m_Health;

	if (m_CurrentState==STATE_WORKING && m_Godmode==false)	
		m_Health-=amount;



	if (m_Health<=0)
	{
		if (m_CurrentState==STATE_WORKING) m_CurrentState=STATE_FALLING;

		//Ship will exlode faster according to overkill damage, at a certain point it will explode instantly
		m_TicksUntilExplosion= 50 - overkill*2;

		m_Health=0;


	}

}

void PlayerShip::SetHealth(double health)
{
	if (health >= 0) m_Health=health;
}


double PlayerShip::GetHealth()
{
	return m_Health;
}

double PlayerShip::GetMaxHealth()
{
	return m_MaxHealth;
}


bool PlayerShip::IsExploding()
{
	if (m_CurrentState==STATE_EXPLODING) return true;
	return false;
}

DOUBLE2 PlayerShip::CastOutExplosionForce(DOUBLE2 targetPos)
{
	DOUBLE2 explosionVector= targetPos-m_Pos;

	DOUBLE2 explosionForce= DOUBLE2(0,0);

	int maxDistance=500;
	if (maxDistance- explosionVector.Length() > 0 && m_HasCastedOutExplosionForce==false)
	{
		explosionForce= explosionVector.Normalized() * 100 * (50 + maxDistance- explosionVector.Length());

	}	

	//When the force is already cast out, return zero force
	return explosionForce;
}

void PlayerShip::HasExploded()
{
	m_HasCastedOutExplosionForce=true;
}

int PlayerShip::GetMassProjAmmo()
{
	return m_MassProjAmmo;
}

void PlayerShip::SubtractMassProjAmmo(int amount)
{
	if (m_MassProjAmmo-amount>=0) m_MassProjAmmo-=amount;
}


int PlayerShip::GetGravBallAmmo()
{
	return m_GravBallAmmo;
}

void PlayerShip::SubtractGravBallAmmo(int amount)
{
	if (m_GravBallAmmo-amount>=0) m_GravBallAmmo-=amount;
}

int PlayerShip::GetRadius()
{
	return RADIUS;
}

void PlayerShip::GodModeMoveLeft()
{
	if (m_Godmode==true) m_Velocity.x=-GODMODE_SPEED;
}

void PlayerShip::GodModeMoveRight()
{
	if (m_Godmode==true) m_Velocity.x=GODMODE_SPEED;
}

void PlayerShip::GodModeMoveUp()
{
	if (m_Godmode==true) m_Velocity.y=-GODMODE_SPEED;
}

void PlayerShip::GodModeMoveDown()
{
	if (m_Godmode==true) m_Velocity.y=GODMODE_SPEED;
}
