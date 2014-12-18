//---------------------------
// Includes
//---------------------------
#include "GravityBall.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

Bitmap* GravityBall::m_BmpGravityBallPtr=nullptr;
Bitmap* GravityBall::m_BmpExplosionPtr=nullptr;

//---------------------------
// Constructor & Destructor
//---------------------------
GravityBall::GravityBall(int x, int y, double angle, DOUBLE2 inheritance): m_Pos(x,y), m_Angle(angle), 
	m_Velocity(0,0), m_Speed(1500)
	, m_TickCount(0), m_SlowDownAnimations(0), m_CurrentState(STATE_NORMAL)
	, m_VelocityProjected(0,0), m_VelocityOrthogonal(0,0), m_Mass(5.97219 * pow(10,13)/2), m_Acceleration(0,0),
	m_ExplosionTimer(0),
	m_HasExplodedAtleastOnce(false),m_HasCastedOutExplosionForce(false),m_NormalAnimationSpeed(50)
{

	if (GravityBall::m_BmpGravityBallPtr==nullptr)
	{
		GravityBall::m_BmpGravityBallPtr= new Bitmap("./resources/GravityBall.png");
	}	

	if (GravityBall::m_BmpExplosionPtr==nullptr)
	{
		GravityBall::m_BmpExplosionPtr= new Bitmap("./resources/Explosion.png");
		GravityBall::m_BmpExplosionPtr->SetTransparencyColor(0,0,0);
	}

	//Key input setting the speed.	

	m_Velocity.x = m_Speed * cos(m_Angle);
	m_Velocity.y = m_Speed * sin(m_Angle);	
	if (m_Velocity.DotProduct(inheritance)>0)
		m_Velocity+=inheritance;

}


GravityBall::~GravityBall()
{

}

//---------------------------
// Own methods
//---------------------------

void GravityBall::Tick(double deltaTime)
{	
	if (m_HasExplodedAtleastOnce==true) m_HasCastedOutExplosionForce=true;

	//Friction
	int k=m_Velocity.Length()/2;

	DOUBLE2 frictionForce = k * (-m_Velocity.Normalized());
	m_Acceleration/=m_Mass;
	m_Acceleration += frictionForce;

	// Key input setting the speed. Gravity applied in main game class
	m_Velocity += m_Acceleration * deltaTime;


	//Resetting acceleration every tick
	m_Acceleration= DOUBLE2(0,0);

	switch (m_CurrentState) 
	{

	case STATE_NORMAL : 



		if (m_SlowDownAnimations>0)
		{
			if (m_NormalAnimationSpeed<500) m_NormalAnimationSpeed+=m_ExplosionTimer;

			m_SlowDownAnimations=RESET_SLOW_DOWN;	
			m_ExplosionTimer+=1;

			m_TickCount+=1;

			if (m_TickCount>=NORMAL_FRAMES)
			{
				m_TickCount=0;
			}

		}
		m_SlowDownAnimations+=m_NormalAnimationSpeed*deltaTime;


		break;


	case STATE_EXPLODING : 

		if (m_SlowDownAnimations>0)
		{
			m_SlowDownAnimations=RESET_SLOW_DOWN;			

			if (m_TickCount<EXPLOSION_FRAMES-1)
			{
				m_TickCount+=1;
			}
			else
			{
				m_CurrentState=STATE_DESTROYED;
			}


		}
		m_SlowDownAnimations+=EXPLOSION_ANIMATION_SPEED*deltaTime;
		break;

	}

	//If the explosion timer becomes 100 -> set the projectile ready for explosion and destruction
	if (m_ExplosionTimer==200)
	{
		m_TickCount=0;
		m_CurrentState=STATE_EXPLODING;

		m_ExplosionTimer+=1;
	}




}

void GravityBall::Paint(MATRIX3X2 matTransform)
{


	MATRIX3X2 matCenter,matRotate,matScale,matTranslate,matTransformGravBall;
	RECT frame;
	MATRIX3X2 matExplosionCenter, matExplosionTransform;

	matCenter.SetAsTranslate(-m_BmpGravityBallPtr->GetWidth()/7/2, -m_BmpGravityBallPtr->GetHeight()/4);

	matRotate.SetAsRotate(m_Angle);

	matScale.SetAsScale(1,1);

	matTranslate.SetAsTranslate(m_Pos);


	frame.top=0;
	frame.bottom=0;
	frame.left= 0;
	frame.right= 0;

	int row= m_TickCount%4;
	int column= m_TickCount/4;

	switch (m_CurrentState) 
	{

	case STATE_NORMAL : 

		frame.top=0;
		frame.bottom=m_BmpGravityBallPtr->GetHeight();
		frame.left= m_BmpGravityBallPtr->GetWidth()/3*m_TickCount;
		frame.right= m_BmpGravityBallPtr->GetWidth()/3 + m_BmpGravityBallPtr->GetWidth()/3*m_TickCount;


		matTransformGravBall= matCenter*matRotate*matTranslate;

		GAME_ENGINE->SetTransformMatrix(matTransformGravBall*matTransform);

		if (m_CurrentState!=STATE_DESTROYED)
			GAME_ENGINE->DrawBitmap(m_BmpGravityBallPtr,0,0,frame);

		break;

	case STATE_EXPLODING : 




		frame.top=m_BmpExplosionPtr->GetHeight()/4*column;
		frame.bottom=m_BmpExplosionPtr->GetHeight()/4+m_BmpExplosionPtr->GetHeight()/4*column;
		frame.left=m_BmpExplosionPtr->GetWidth()/4*row;
		frame.right=m_BmpExplosionPtr->GetWidth()/4+m_BmpExplosionPtr->GetWidth()/4*row;

		matScale.SetAsScale(2,2);

		matExplosionCenter.SetAsTranslate(-m_BmpExplosionPtr->GetWidth()/4/2,
			-m_BmpExplosionPtr->GetHeight()/4/2);

		matExplosionTransform=matExplosionCenter *  matScale * matRotate * matTranslate * matTransform;

		GAME_ENGINE->SetTransformMatrix(matExplosionTransform);
		GAME_ENGINE->DrawBitmap(m_BmpExplosionPtr,0,0,frame);

		break;

	case STATE_DESTROYED : 

		frame.top=0;
		frame.bottom=0;
		frame.left= 0;
		frame.right= 0;
		break;


	}

}

DOUBLE2 GravityBall::ExerciseForce(DOUBLE2 objectPos, double objectMass)
{
	if ( (m_Pos-objectPos).Length() < 700)
	{
		//G * m1*m2*rn12/|r12|2
		double g=0.000000000066742;

		double distance = (objectPos-m_Pos).Length();

		DOUBLE2 force= g *(m_Mass * objectMass   / distance*distance) * (m_Pos-objectPos).Normalized() ;

		return force;

	}


	return DOUBLE2(0,0);
}

bool GravityBall::IsExploding()
{
	if (m_CurrentState==STATE_EXPLODING) return true;
	return false;
}

DOUBLE2 GravityBall::CastOutExplosionForce(DOUBLE2 targetPos)
{
	DOUBLE2 explosionVector= targetPos-m_Pos;

	DOUBLE2 explosionForce= DOUBLE2(0,0);

	int maxDistance=500;
	if (maxDistance- explosionVector.Length() > 0 && m_HasCastedOutExplosionForce==false && m_CurrentState==STATE_EXPLODING)
	{
		explosionForce= explosionVector.Normalized() * 1000 * (50 + maxDistance- explosionVector.Length());
		m_HasExplodedAtleastOnce=true;
	}	

	//When the force is already cast out, return zero force
	return explosionForce;
}


void GravityBall::AddForce(DOUBLE2 force)
{
	m_Acceleration+=force/m_Mass;
}


DOUBLE2 GravityBall::GetPos()
{
	return m_Pos;
}

double GravityBall::GetRadius()
{
	return RADIUS;
}



void GravityBall::HandleCollision(HitRegion *hitLevelPtr, double deltaTime)
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

DOUBLE2 GravityBall::HandleCircularCollision(DOUBLE2 otherCirclePos, DOUBLE2 otherCircleVelocity, double radius, double mass)
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

	if ((otherCirclePos-m_Pos).Length() < RADIUS + radius)
	{	
		m_Velocity=m_VelocityOrthogonal;
		m_Velocity+=otherCircleVeloProj;		


		otherCircleVelocity=otherCircleVeloOrth;
		otherCircleVelocity+=m_VelocityProjected;

		double cr=0.5;


		m_Velocity *= cr;
		otherCircleVelocity *= cr;

		//return otherCircleVelocity;


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

bool GravityBall::ReadyToBeDestroyed()
{
	if (m_CurrentState==STATE_DESTROYED)
		return true;

	return false;
}