//---------------------------
// Includes
//---------------------------
#include "MassProjectile.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

Bitmap* MassProjectile::m_BmpMassProjectilePtr=nullptr;

//---------------------------
// Constructor & Destructor
//---------------------------
MassProjectile::MassProjectile(int x, int y, double angle, DOUBLE2 inheritance): m_Pos(x,y), m_Angle(angle), 
	m_Velocity(0,0), m_Speed(2000)
	, m_TickCount(0),m_TickCountNormalState(0), m_SlowDownAnimations(0), m_CurrentState(STATE_NORMAL)
	, m_VelocityProjected(0,0), m_VelocityOrthogonal(0,0), m_Mass(0.5), m_Radius(32), m_Acceleration(0,0),
	m_ExplosionTimer(0)
{

	if (MassProjectile::m_BmpMassProjectilePtr==nullptr)
	{
		MassProjectile::m_BmpMassProjectilePtr= new Bitmap("./resources/MassProjectile.png");
	}	

	//Key input setting the speed.	

	m_Velocity.x = m_Speed * cos(m_Angle);
	m_Velocity.y = m_Speed * sin(m_Angle);	
	if (m_Velocity.DotProduct(inheritance)>0)
		m_Velocity+=inheritance;

}


MassProjectile::~MassProjectile()
{

}

//---------------------------
// Own methods
//---------------------------

void MassProjectile::Tick(double deltaTime)
{	
	//Friction
	int k=m_Velocity.Length()/2;

	DOUBLE2 frictionForce = k * (-m_Velocity.Normalized());
	m_Acceleration += frictionForce;

	m_Acceleration/=m_Mass;

	// Key input setting the speed. Gravity applied in main game class
	m_Velocity += m_Acceleration * deltaTime;

	//Projectile stops moving when the inner ball explodes
	if (m_CurrentState==STATE_EXPLODING && m_TickCount>3) m_Velocity=DOUBLE2(0,0);



	//Resetting acceleration every tick
	m_Acceleration= DOUBLE2(0,0);

	switch (m_CurrentState) 
	{

	case STATE_NORMAL : 

		if (m_SlowDownAnimations>0)
		{
			m_SlowDownAnimations=RESET_SLOW_DOWN;			

			if (m_TickCountNormalState<3)
			{
				m_TickCountNormalState+=1;

			}
			else
			{
				m_CurrentState=STATE_EXPANDING;
			}

		}
		m_SlowDownAnimations+=EXPANDMASS_ANIMATION_SPEED*deltaTime;

		break;


	case STATE_EXPANDING : 

		if (m_SlowDownAnimations>0)
		{
			m_SlowDownAnimations=RESET_SLOW_DOWN;			

			m_ExplosionTimer+=1;

			if (m_TickCount<EXPANDMASS_FRAMES-1)
			{
				m_TickCount+=1;
				m_Mass+=2;
			}
			else
			{
				m_Mass=20;
			}


		}
		m_SlowDownAnimations+=EXPANDMASS_ANIMATION_SPEED*deltaTime;

		break;

	case STATE_EXPLODING : 

		if (m_SlowDownAnimations>0)
		{
			m_SlowDownAnimations=RESET_SLOW_DOWN;			

			if (m_TickCount<EXPLODING_FRAMES-1)
			{
				m_TickCount+=1;
			}
			else
			{
				m_CurrentState=STATE_DESTROYED;
			}


		}
		m_SlowDownAnimations+=EXPLODING_ANIMATION_SPEED*deltaTime;
		break;

	}

	//If the explosion timer becomes 100 -> set the projectile ready for explosion and destruction
	if (m_ExplosionTimer==100)
	{
		m_TickCount=0;
		m_CurrentState=STATE_EXPLODING;

		m_ExplosionTimer+=1;
	}




}

void MassProjectile::Paint(MATRIX3X2 matTransform)
{


	MATRIX3X2 matCenter,matRotate,matScale,matTranslate,matTransformProjectile;

	GAME_ENGINE->SetTransformMatrix(matTransform);

	//Draw Projectile

	GAME_ENGINE->SetColor(COLOR(255,0,0));

	//if (m_ReflectionPoint==DOUBLE2(0,0) && m_ReflectionVector==DOUBLE2(0,0))
	//m_IsBeingReflected=false;

	RECT frame;
	frame.top=0;
	frame.bottom=m_BmpMassProjectilePtr->GetHeight()/2;
	frame.left= m_BmpMassProjectilePtr->GetWidth()/7*m_TickCount;
	frame.right= m_BmpMassProjectilePtr->GetWidth()/7 + m_BmpMassProjectilePtr->GetWidth()/7*m_TickCount;

	switch (m_CurrentState) 
	{

	case STATE_EXPLODING : 

		frame.top=m_BmpMassProjectilePtr->GetHeight()/2;
		frame.bottom=m_BmpMassProjectilePtr->GetHeight();
		//Left and right same as above
		break;

	case STATE_DESTROYED : 

		frame.top=0;
		frame.bottom=0;
		frame.left= 0;
		frame.right= 0;
		break;


	}


	matCenter.SetAsTranslate(-m_BmpMassProjectilePtr->GetWidth()/7/2, -m_BmpMassProjectilePtr->GetHeight()/4);

	matRotate.SetAsRotate(m_Angle);

	matTranslate.SetAsTranslate(m_Pos);

	matTransformProjectile= matCenter*matRotate*matTranslate;

	GAME_ENGINE->SetTransformMatrix(matTransformProjectile*matTransform);

	if (m_CurrentState!=STATE_DESTROYED)
		GAME_ENGINE->DrawBitmap(m_BmpMassProjectilePtr,0,0,frame);


}


DOUBLE2 MassProjectile::GetPos()
{
	return m_Pos;
}

DOUBLE2 MassProjectile::GetVelocity()
{
	return m_Velocity;
}

void MassProjectile::SetVelocity(DOUBLE2 velocity)
{
	m_Velocity=velocity;
}



void MassProjectile::AddForce(DOUBLE2 force)
{
	m_Acceleration+=force;
}

void MassProjectile::HandleCollision(HitRegion *hitLevelPtr, double deltaTime)
{
	HIT hitArr[1];

	DOUBLE2 projected, reflected, newPoint, remainingVector;


	double remainingTime=deltaTime, cr=0.8;
	int crMultiplier=0;

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

			//m_Angle= -reflected.AngleWith( DOUBLE2(1,0) );			

			crMultiplier+=1;
			m_Velocity=reflected.Normalized()*m_Velocity.Length()*cr*crMultiplier;


		}
		else
		{
			m_Pos += m_Velocity*remainingTime;	
			remainingTime=0;			
		}
	}
}
DOUBLE2 MassProjectile::HandleCircularCollision(DOUBLE2 otherCirclePos, DOUBLE2 otherCircleVelocity, double radius, double mass)
{
	//calculate the normalized vector connecting two positions
	//DOUBLE2 normal= (otherCirclePos-m_Pos).Normalized();
	DOUBLE2 normal= (m_Pos-otherCirclePos).Normalized();

	double scalproj= normal.DotProduct(m_Velocity);
	m_VelocityProjected=normal*scalproj;
	m_VelocityOrthogonal= m_Velocity-m_VelocityProjected;	

	double scalproj2= normal.DotProduct(otherCircleVelocity);

	DOUBLE2 otherCircleVeloProj=normal*scalproj2;
	DOUBLE2 otherCircleVeloOrth=otherCircleVelocity-otherCircleVeloProj;

	if ((otherCirclePos-m_Pos).Length() < m_Radius + radius)
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

bool MassProjectile::ReadyToBeDestroyed()
{
	if (m_CurrentState==STATE_DESTROYED)
		return true;

	return false;
}

double MassProjectile::GetRadius()
{
	return m_Radius;
}

double MassProjectile::GetMass()
{
	return m_Mass;
}
