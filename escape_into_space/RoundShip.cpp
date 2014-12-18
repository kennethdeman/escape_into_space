//---------------------------
// Includes
//---------------------------
#include "RoundShip.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

Bitmap* RoundShip::m_BmpRoundShipPtr=nullptr;
Bitmap* RoundShip::m_BmpLeftThrusterPtr=nullptr;
Bitmap* RoundShip::m_BmpBottomThrusterPtr=nullptr;
Bitmap* RoundShip::m_BmpTopThrusterPtr=nullptr;
Bitmap* RoundShip::m_BmpRightThrusterPtr=nullptr;
Bitmap* RoundShip::m_BmpCannonPtr=nullptr;
Bitmap* RoundShip::m_BmpExplosionPtr=nullptr;

//---------------------------
// Constructor & Destructor
//---------------------------  
RoundShip::RoundShip(int x, int y): m_Pos(x,y), m_Angle(0), m_Velocity(0,0),
	m_IsLeftThrusterOn(false), 
	m_IsBottomThrusterOn(false), m_IsTopThrusterOn(false), m_IsRightThrusterOn(false),
	m_HitRegionPtr(false),

	m_LeftThrusterForce(0,0), m_LeftThrusterPower(500), m_BottomThrusterPower(1000),
	m_BottomThrusterForce(0,0),  m_RightThrusterForce(0,0),  m_RightThrusterPower(500), 
	m_TopThrusterForce(0,0), m_TopThrusterPower(500),

	m_FocusPoint(x,y), m_CannonAngle(0), m_HasTarget(false),m_HasCastedOutExplosionForce(false),

	m_LeftThrusterAngle(0), m_BottomThrusterAngle(0), 
	m_CurrentDirection(DIRECTION_LEFT), m_CurrentState(STATE_WORKING), 
	m_Acceleration(0,0), m_SumOfAccelerations(0,0), m_TimeWhenLastShot(0), 
	m_Health(150), m_TicksUntilExplosion(50), m_TickCount(0), m_SlowDownAnimations(0)
	, m_VelocityProjected(0,0), m_VelocityOrthogonal(0,0)


{

	if (RoundShip::m_BmpRoundShipPtr==nullptr)
	{
		RoundShip::m_BmpRoundShipPtr= new Bitmap("./resources/RoundShip.png");
	}

	if (RoundShip::m_BmpLeftThrusterPtr==nullptr)
	{
		RoundShip::m_BmpLeftThrusterPtr= new Bitmap("./resources/RoundShipLeftThruster.png");
	}

	if (RoundShip::m_BmpBottomThrusterPtr==nullptr)
	{
		RoundShip::m_BmpBottomThrusterPtr= new Bitmap("./resources/RoundShipBottomThruster.png");
	}

	if (RoundShip::m_BmpTopThrusterPtr==nullptr)
	{
		RoundShip::m_BmpTopThrusterPtr= new Bitmap("./resources/RoundShipTopThruster.png");
	}

	if (RoundShip::m_BmpRightThrusterPtr==nullptr)
	{
		RoundShip::m_BmpRightThrusterPtr= new Bitmap("./resources/RoundShipRightThruster.png");
	}
	if (RoundShip::m_BmpCannonPtr==nullptr)
	{
		RoundShip::m_BmpCannonPtr= new Bitmap("./resources/RoundShipCannon.png");
	}

	if (RoundShip::m_BmpExplosionPtr==nullptr)
	{
		RoundShip::m_BmpExplosionPtr= new Bitmap("./resources/GreenExplosion.png");
		RoundShip::m_BmpExplosionPtr->SetTransparencyColor(0,0,0);
	}


	m_HitRegionPtr= new HitRegion();
	m_HitRegionPtr->CreateFromEllipse(0,0,ROUNDSHIP_WIDTH,ROUNDSHIP_HEIGHT);
}

RoundShip::~RoundShip()
{

}

//---------------------------
// Own methods
//---------------------------

void RoundShip::Tick(double deltaTime)
{


	//Position Hitbox

	MATRIX3X2 matRotate;
	matRotate.SetAsRotate(m_Angle);

	m_HitRegionPtr->SetPos(m_Pos);
	//m_HitRegionPtr->SetTransformMatrix(matRotate);	



	//Reset target if ship is destroyed
	if (m_CurrentState!=STATE_WORKING) m_HasTarget=false;

	if (m_CurrentState==STATE_WORKING)
	{
		//Set the focus vector
		m_FocusVector=m_FocusPoint-m_Pos; 	 


		//Make the ship look to the focus point
		double offset=100 - 0.1*m_Velocity.Length();

		if (m_FocusPoint.x - m_Pos.x -10 <0)
		{
			m_CurrentDirection=DIRECTION_LEFT;
		}
		else if (m_FocusPoint.x - m_Pos.x +10 >0)
		{
			m_CurrentDirection=DIRECTION_RIGHT;
		}


		// "AI"
		if (m_FocusPoint.x+offset < m_Pos.x)
		{
			if (m_CurrentDirection==DIRECTION_LEFT)	m_IsRightThrusterOn=true;
			else m_IsLeftThrusterOn=true;
		}
		if (m_FocusPoint.x-offset > m_Pos.x)
		{
			if (m_CurrentDirection==DIRECTION_LEFT)	m_IsLeftThrusterOn=true;
			else m_IsRightThrusterOn=true;		
		}

		if (m_FocusPoint.y+offset < m_Pos.y)
		{
			m_IsBottomThrusterOn=true;
		}
		else m_IsBottomThrusterOn=false;

		if (m_FocusPoint.y-offset > m_Pos.y)
		{
			m_IsTopThrusterOn=true;
		}
		else m_IsTopThrusterOn=false;

		//Determines when the ship can shoot again
		if (m_TimeWhenLastShot>0)
			m_TimeWhenLastShot-=1*deltaTime;
		else
			m_TimeWhenLastShot=0;

		if (m_CurrentDirection==DIRECTION_RIGHT)
		{
			m_LeftThrusterAngle=m_Angle+M_PI;

		}
		else
		{
			m_LeftThrusterAngle=m_Angle;

		}

		m_BottomThrusterAngle=m_Angle-M_PI/2;



		m_LeftThrusterForce.x = m_LeftThrusterPower * cos(m_LeftThrusterAngle);
		m_LeftThrusterForce.y = m_LeftThrusterPower * sin(m_LeftThrusterAngle);

		m_RightThrusterForce.x = -m_RightThrusterPower * cos(m_LeftThrusterAngle);
		m_RightThrusterForce.y = -m_RightThrusterPower * sin(m_LeftThrusterAngle);

		m_BottomThrusterForce.x = m_BottomThrusterPower * cos(m_BottomThrusterAngle);
		m_BottomThrusterForce.y = m_BottomThrusterPower * sin(m_BottomThrusterAngle);

		m_TopThrusterForce.x = m_TopThrusterPower * cos(-m_BottomThrusterAngle);
		m_TopThrusterForce.y = m_TopThrusterPower * sin(-m_BottomThrusterAngle);


		if (m_IsLeftThrusterOn==true)
		{

			m_Acceleration+= m_LeftThrusterForce;


		}

		if (m_IsBottomThrusterOn==true)
		{
			m_Acceleration+= m_BottomThrusterForce;
		}

		if (m_IsTopThrusterOn==true)
		{
			m_Acceleration+= m_TopThrusterForce;
		}

		if (m_IsRightThrusterOn==true)
		{
			m_Acceleration+= m_RightThrusterForce;
		}

		//Cannon angle is determined and used to rotate the cannon bitmap
		if (m_CurrentDirection==DIRECTION_RIGHT)
			m_CannonAngle=-m_FocusVector.AngleWith(DOUBLE2(1,0));
		else
			m_CannonAngle=-m_FocusVector.AngleWith(DOUBLE2(-1,0));



	}



	if (m_CurrentState!=STATE_EXPLODING)
	{
		//Friction
		int k=m_Velocity.Length()/2;

		DOUBLE2 frictionForce = k * (-m_Velocity.Normalized());
		m_Acceleration += frictionForce;

		m_Acceleration/=MASS;

		m_SumOfAccelerations+=m_Acceleration;


		// Key input setting the speed. Gravity applied in main game class
		m_Velocity += m_SumOfAccelerations  * deltaTime;

		//Resetting acceleration every tick
		m_Acceleration= DOUBLE2(0,0);
		m_SumOfAccelerations= DOUBLE2(0,0);



	}	


	if (m_CurrentState==STATE_FALLING)
	{

		if (m_SlowDownAnimations>0)
		{
			m_SlowDownAnimations=RESET_SLOW_DOWN;

			if (m_TickCount<50)
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
			else
			{
				m_CurrentState=STATE_DESTROYED;
			}



		}
		m_SlowDownAnimations+=EXPLOSION_ANIMATION_SPEED*deltaTime;

	}

}

void RoundShip::Paint(MATRIX3X2 matTransform)
{
	MATRIX3X2 matShipCenter, matShipRotate, matShipScale, matShipTranslate,matShipTransform,
		matCanonTransform, matCanonRotate;

	matShipCenter.SetAsTranslate(-m_BmpRoundShipPtr->GetWidth()/2,-m_BmpRoundShipPtr->GetHeight()/2);
	matShipRotate.SetAsRotate(m_Angle);

	if (m_CurrentDirection==DIRECTION_LEFT)
	{
		matShipScale.SetAsScale(1,1);
	}
	else
	{
		matShipScale.SetAsScale(-1,1);
	}	

	matShipTranslate.SetAsTranslate(m_Pos);	

	matShipTransform=matShipCenter *  matShipScale * matShipRotate * matShipTranslate * matTransform;

	GAME_ENGINE->SetTransformMatrix(matShipTransform);

	if (m_CurrentState==STATE_WORKING)
	{
		if (m_IsLeftThrusterOn==true)
		{
			GAME_ENGINE->DrawBitmap(m_BmpLeftThrusterPtr,0,0);
		}

		if (m_IsBottomThrusterOn==true)
		{
			GAME_ENGINE->DrawBitmap(m_BmpBottomThrusterPtr,0,0);
		}

		if (m_IsTopThrusterOn==true)
		{
			GAME_ENGINE->DrawBitmap(m_BmpTopThrusterPtr,0,0);
		}

		if (m_IsRightThrusterOn==true)
		{
			GAME_ENGINE->DrawBitmap(m_BmpRightThrusterPtr,0,0);
		}
	}

	if (m_CurrentState==STATE_WORKING || m_CurrentState==STATE_FALLING)
	{
		GAME_ENGINE->DrawBitmap(m_BmpRoundShipPtr,0,0);

		matCanonRotate.SetAsRotate(m_CannonAngle);

		matCanonTransform= matShipCenter *  matShipScale * matCanonRotate * matShipTranslate * matTransform;

		GAME_ENGINE->SetTransformMatrix(matCanonTransform);

		GAME_ENGINE->DrawBitmap(m_BmpCannonPtr,0,0);
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

		matExplosionTransform=matExplosionCenter *  matShipScale * matShipRotate * matShipTranslate * matTransform;

		GAME_ENGINE->SetTransformMatrix(matExplosionTransform);
		GAME_ENGINE->DrawBitmap(m_BmpExplosionPtr,0,0,frame);
	}

}


DOUBLE2 RoundShip::GetPos()
{
	return m_Pos;
}

bool RoundShip::Hit(HitRegion* hitTargetPtr)
{
	return hitTargetPtr->HitTest(m_Pos);
}




void RoundShip::HandleCollision(HitRegion *hitLevelPtr, double deltaTime)
{


	//Velocity reflection raycast

	HIT hitArr[1];
	DOUBLE2 projected, reflected, newPoint, remainingVector;


	double remainingTime=deltaTime;

	newPoint=m_Pos;
	remainingVector=m_Velocity*deltaTime;

	while (remainingTime>0)
	{
		if (hitLevelPtr->Raycast(newPoint, remainingVector, hitArr, 1, -1)>=1)
		{		


			DOUBLE2 normal = DOUBLE2(hitArr[0].normal);

			projected=normal*normal.DotProduct(remainingVector);  
			reflected=remainingVector-2*projected;
			reflected=reflected.Normalized();

			reflected *= remainingVector.Length()* (1 - hitArr[0].lambda);

			remainingVector=reflected;

			remainingTime= deltaTime * (1 - hitArr[0].lambda);

			newPoint=hitArr[0].point+reflected;
			m_Pos=hitArr[0].point+reflected;	

			//Take damage when the velocity is higher than 600
			if (m_Velocity.Length()>600)
			{
				int amount=m_Velocity.Length()/5;
				SubtractHealth(m_Velocity.Length()/5);
			}


			//Restitution
			double cr=0.65;
			m_Velocity=reflected.Normalized()*m_Velocity.Length()*cr;


		}
		else
		{

			if (m_CurrentState!=STATE_EXPLODING) m_Pos += m_Velocity * remainingTime;	
			remainingTime=0;
		}
	}


	//Position Hitbox
	m_HitRegionPtr->SetPos(m_Pos);
}


DOUBLE2 RoundShip::HandleCircularCollision(DOUBLE2 otherCirclePos, DOUBLE2 otherCircleVelocity, double radius)
{
	//calculate the normalized vector connecting two positions
	DOUBLE2 normal= (m_Pos-otherCirclePos).Normalized();

	double scalproj= normal.DotProduct(m_Velocity);
	m_VelocityProjected=normal*scalproj;
	m_VelocityOrthogonal= m_Velocity-m_VelocityProjected;	

	double scalproj2= normal.DotProduct(otherCircleVelocity);

	DOUBLE2 otherCircleVeloProj=normal*scalproj2;
	DOUBLE2 otherCircleVeloOrth=otherCircleVelocity-otherCircleVeloProj;

	if ((otherCirclePos-m_Pos).Length() < RADIUS + radius)
	{	
		m_Velocity=m_VelocityOrthogonal;
		m_Velocity+=otherCircleVeloProj;		


		otherCircleVelocity=otherCircleVeloOrth;
		otherCircleVelocity+=m_VelocityProjected;

		double cr=0.8;

		m_Velocity *= cr;
		otherCircleVelocity *= cr;

		//project the velocity vectors onto the vNormal.
		DOUBLE2 projVelocity1 = normal.DotProduct(m_Velocity)*normal;
		DOUBLE2 projVelocity2 = normal.DotProduct(otherCircleVelocity)*normal;

		//if the projected velocities are pointing in the same direction:
		if(projVelocity1.DotProduct(projVelocity2)>0)
		{
			//if the first one is moving faster than the second, don't interfere
			//first one is identified by dot with vNormal
			if(normal.DotProduct(projVelocity1)>0)
			{

				if(projVelocity1.Length()>projVelocity2.Length())return otherCircleVelocity;

			}
			else
			{
				if(projVelocity1.Length()<projVelocity2.Length())return otherCircleVelocity;
			}
		}
		//they are not moving in the same direction
		else
		{
			if(normal.DotProduct(projVelocity1)>0)
			{
				return otherCircleVelocity;
			}
		}
	}

	return DOUBLE2(0,0);


}


void RoundShip::TurnLeftThrusterOn (bool isOn)
{
	if (m_CurrentState==STATE_WORKING)	m_IsLeftThrusterOn=isOn;
}

void RoundShip::TurnBottomThrusterOn (bool isOn)
{
	if (m_CurrentState==STATE_WORKING)	m_IsBottomThrusterOn=isOn;
}

void RoundShip::TurnRightThrusterOn (bool isOn)
{
	if (m_CurrentState==STATE_WORKING)	m_IsRightThrusterOn=isOn;
}

void RoundShip::TurnTopThrusterOn (bool isOn)
{
	if (m_CurrentState==STATE_WORKING)	m_IsTopThrusterOn=isOn;
}


void RoundShip::SetDirectionLeft()
{
	if (m_CurrentState==STATE_WORKING)	m_CurrentDirection=DIRECTION_LEFT;
}

void RoundShip::SetDirectionRight()
{
	if (m_CurrentState==STATE_WORKING)	m_CurrentDirection=DIRECTION_RIGHT;
}


void RoundShip::SetTimeWhenLastShot(double time)
{
	m_TimeWhenLastShot=time;
}

double RoundShip::GetTimeWhenLastShot()
{
	return m_TimeWhenLastShot;
}


void RoundShip::SetVelocity(DOUBLE2 velocity)
{
	m_Velocity=velocity;
}

DOUBLE2 RoundShip::GetVelocity()
{
	return m_Velocity;
}

void RoundShip::AddForce(DOUBLE2 force)
{
	m_Acceleration+=force;
}

void RoundShip::AddGravity(DOUBLE2 gravity)
{
	m_SumOfAccelerations+=gravity;
}


DOUBLE2 RoundShip::GetAcceleration()
{
	return m_Acceleration;
}

void RoundShip::Shoot(LaserList* laserListPtr)
{
	LaserShot *tempPtr;

	tempPtr= new LaserShot(GetPos().x, GetPos().y, GetShootingAngle(), GetVelocity(), LaserShot::COLOR_GREEN);

	if (GetTimeWhenLastShot() == 0 && m_HasTarget==true)
	{	

		laserListPtr->Add(tempPtr);
		SetTimeWhenLastShot(2.5);

	}
}


double RoundShip::GetShootingAngle()
{
	//m_CannonAngle is not used because it has to be adapted to the bitmap inverse scaling (left to right)
	return -m_FocusVector.AngleWith(DOUBLE2(1,0));;
}

double RoundShip::GetAngle()
{
	return m_Angle;
}


double RoundShip::GetSpeed()
{
	return m_Velocity.Length();
}

void RoundShip::SetFocusPoint(DOUBLE2 focus)
{
	m_FocusPoint=focus;
}

void RoundShip::LineOfSight(DOUBLE2 focus, HitRegion* hitRegionPtr)
{
	DOUBLE2 vector=focus-m_Pos;
	HIT hitArr[1];

	if (hitRegionPtr->Raycast(m_Pos, vector, hitArr, 1, 0)==0)
	{		
		m_FocusPoint=focus;
		m_HasTarget=true;
	}
	else
	{
		m_HasTarget=false;
	}


}

bool RoundShip::HasTarget()
{
	if (m_CurrentState!=STATE_WORKING) return false;

	return m_HasTarget;
}

DOUBLE2 RoundShip::GetTarget()
{
	return m_FocusPoint;
}



HitRegion* RoundShip::GetHitRegion()
{
	return m_HitRegionPtr;
}

void RoundShip::SubtractHealth(double amount)
{
	double overkill=amount-m_Health;

	if (m_CurrentState==STATE_WORKING)
		m_Health-=amount;

	if (m_Health<=0)
	{	

		if (m_CurrentState==STATE_WORKING) m_CurrentState=STATE_FALLING;

		//Ship will exlode faster according to overkill damage, at a certain point it will explode instantly
		m_TicksUntilExplosion= 50 - overkill*2;

		m_Health=0;
	}

}

bool RoundShip::IsExploding()
{
	if (m_CurrentState==STATE_EXPLODING) return true;
	return false;
}

DOUBLE2 RoundShip::CastOutExplosionForce(DOUBLE2 targetPos)
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

void RoundShip::HasExploded()
{
	m_HasCastedOutExplosionForce=true;
}

double RoundShip::GetRadius()
{
	return RADIUS;
}

double RoundShip::GetMass()
{
	return MASS;
}

bool RoundShip::IsReadyToBeDeleted()
{
	return m_CurrentState==STATE_DESTROYED;
}

