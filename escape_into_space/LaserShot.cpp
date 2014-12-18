//---------------------------
// Includes
//---------------------------
#include "LaserShot.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

Bitmap* LaserShot::m_BmpLaserShotPtr=nullptr;

//---------------------------
// Constructor & Destructor
//---------------------------
LaserShot::LaserShot(int x, int y, double angle, DOUBLE2 inheritance): m_Pos(x,y), m_Angle(angle), 
	m_Velocity(0,0), m_Speed(1200 + inheritance.Length()),m_Vector(0,0),m_Inheritance(inheritance), m_IsBeingReflected(false),
	m_NewPos(0,0), m_ReflectionPoint(0,0), m_ReflectionVector(0,0), m_NewAngle(0), m_AmountOfHits(0)
	, m_Color(COLOR_RED), m_ReflectionCount(0)
{

	if (LaserShot::m_BmpLaserShotPtr==nullptr)
	{
		LaserShot::m_BmpLaserShotPtr= new Bitmap("./resources/LaserShot.png");
	}

	for (int count=0; count < MAX_REFLECTIONS; ++count)
	{
		m_ReflectionAngleArr[count]=0;
		m_ReflectionPtr[count]=DOUBLE2(0,0);
		m_ReflectionPtr[count+5]=DOUBLE2(0,0);
	}



}

LaserShot::LaserShot(int x, int y, double angle, DOUBLE2 inheritance, int color): m_Pos(x,y), m_Angle(angle), 
	m_Velocity(0,0), m_Speed(1200 + inheritance.Length()),m_Vector(0,0),m_Inheritance(inheritance), m_IsBeingReflected(false),
	m_NewPos(0,0), m_ReflectionPoint(0,0), m_ReflectionVector(0,0), m_NewAngle(0), m_AmountOfHits(0)
	, m_Color(color), m_ReflectionCount(0)
{

	if (LaserShot::m_BmpLaserShotPtr==nullptr)
	{
		LaserShot::m_BmpLaserShotPtr= new Bitmap("./resources/LaserShot.png");
	}

	for (int count=0; count < MAX_REFLECTIONS; ++count)
	{
		m_ReflectionAngleArr[count]=0;
		m_ReflectionPtr[count]=DOUBLE2(0,0);
		m_ReflectionPtr[count+5]=DOUBLE2(0,0);
	}


}

LaserShot::~LaserShot()
{

}

//---------------------------
// Own methods
//---------------------------

void LaserShot::Tick(double deltaTime)
{
	//Key input setting the speed.	

	m_Velocity.x = m_Speed * cos(m_Angle);
	m_Velocity.y = m_Speed * sin(m_Angle);

	//m_Velocity+=m_Inheritance;

	m_Vector.x = 50 * cos(m_Angle);
	m_Vector.y = 50 * sin(m_Angle);


	m_Pos.x += m_Velocity.x*deltaTime;

	m_Pos.y += m_Velocity.y*deltaTime;

	//Positions the hitboxes


}

void LaserShot::Paint(MATRIX3X2 matTransform)
{


	MATRIX3X2 matCenter,matRotate,matScale,matTranslate,matTransformLaser;

	matCenter.SetAsTranslate(-CENTER_X,-CENTER_Y);

	matRotate.SetAsRotate(m_Angle);

	matScale.SetAsScale(1,1);

	matTranslate.SetAsTranslate(m_Pos);

	GAME_ENGINE->SetTransformMatrix(matTransform);

	//Draw Laser

	GAME_ENGINE->SetColor(COLOR(255,0,0));

	//if (m_ReflectionPoint==DOUBLE2(0,0) && m_ReflectionVector==DOUBLE2(0,0))
	//m_IsBeingReflected=false;

	RECT frame;
	frame.top=m_BmpLaserShotPtr->GetHeight()/2 * m_Color;
	frame.bottom=m_BmpLaserShotPtr->GetHeight()/2 + m_BmpLaserShotPtr->GetHeight()/2 * m_Color;
	frame.left= 0;
	frame.right= m_BmpLaserShotPtr->GetWidth();

	matCenter.SetAsTranslate(-m_BmpLaserShotPtr->GetWidth()/2, -m_BmpLaserShotPtr->GetHeight()/4);

	matRotate.SetAsRotate(m_Angle);

	matTranslate.SetAsTranslate(m_Pos+m_Vector/2);

	matTransformLaser= matCenter*matRotate*matTranslate;

	GAME_ENGINE->SetTransformMatrix(matTransformLaser*matTransform);

	GAME_ENGINE->DrawBitmap(m_BmpLaserShotPtr,0,0,frame);

	if (m_IsBeingReflected==false)
	{		
		if (m_NewPos!=DOUBLE2(0,0) && m_NewAngle!=0)
		{


			m_Pos=m_NewPos;

			m_Angle=m_NewAngle;		

			m_NewPos=DOUBLE2(0,0);
			m_NewAngle=0;
		}


	}
	else
	{		



		//GAME_ENGINE->DrawLine(m_Pos, m_ReflectionPtr[0]);
		for (int count=0; count < m_ReflectionCount; ++count)
		{
			matRotate.SetAsRotate(m_ReflectionAngleArr[0]);

			matTranslate.SetAsTranslate(m_ReflectionPtr[0]+m_ReflectionPtr[0+10]);

			matTransformLaser= matCenter*matRotate*matTranslate;

			GAME_ENGINE->SetTransformMatrix(matTransformLaser*matTransform);

			GAME_ENGINE->DrawBitmap(m_BmpLaserShotPtr,0,0,frame);

			GAME_ENGINE->SetTransformMatrix(matTransform);

			DOUBLE2 reflectionPoint=m_ReflectionPtr[count];
		}


	}
}


DOUBLE2 LaserShot::GetPos()
{
	return m_Pos;
}


void LaserShot::HandleCollision(HitRegion *hitLevelPtr, double deltaTime)
{
	HIT hitArr[1];
	DOUBLE2 projected, reflected, newPoint, remainingVector;


	bool wasTrue=false;
	if (m_IsBeingReflected==true) wasTrue=true;

	m_IsBeingReflected=false;
	m_ReflectionCount=0;

	double remainingTime=deltaTime;

	newPoint=m_Pos;
	remainingVector=m_Vector;


	while (remainingTime>0)
	{
		if (hitLevelPtr->Raycast(newPoint, remainingVector, hitArr, 1, -1)>=1)
		{		

			//When it starts reflecting
			if (wasTrue==false) ++m_AmountOfHits;

			m_IsBeingReflected=true;

			DOUBLE2 normal = DOUBLE2(hitArr[0].normal);

			projected=normal*normal.DotProduct(remainingVector);  
			reflected=remainingVector-2*projected;
			reflected=reflected.Normalized();

			// Reflectionvector - velocity.length/2
			m_BitMapReflectionPoint= reflected * (remainingVector.Length()* (1 - hitArr[0].lambda) - remainingVector.Length()/2);

			reflected *= remainingVector.Length()* (1 - hitArr[0].lambda);

			m_IsBeingReflected=true;			

			newPoint=hitArr[0].point;
			remainingVector=reflected;

			remainingTime= deltaTime * (1 - hitArr[0].lambda);

			m_NewPos=hitArr[0].point;	

			m_NewAngle= -reflected.AngleWith( DOUBLE2(1,0) );



			m_ReflectionPoint=hitArr[0].point;

			m_ReflectionVector=reflected;

			m_ReflectionAngleArr[m_ReflectionCount]=m_NewAngle;
			//Points
			m_ReflectionPtr[m_ReflectionCount]=hitArr[0].point;
			//Vectors
			m_ReflectionPtr[m_ReflectionCount+5]=reflected;

			//Bitmap middelepoint
			m_ReflectionPtr[m_ReflectionCount+10]=reflected-reflected.Normalized()*m_Vector.Length()/2;



			if (m_ReflectionCount>0)
			{
				m_ReflectionPtr[m_ReflectionCount+5-1]=m_ReflectionPtr[m_ReflectionCount+5-1].Normalized() * 
					(m_ReflectionPtr[m_ReflectionCount] - m_ReflectionPtr[m_ReflectionCount-1]).Length();
			}

			//Makes sure only max. 5 reflections are drawn / ReflictionCount = Max. 4
			if (m_ReflectionCount<MAX_REFLECTIONS-1) ++m_ReflectionCount;

		}
		else
		{
			remainingTime=0;
		}
	}


}

bool LaserShot::CheckHitCollision(HitRegion *hitRegionPtr, DOUBLE2 targetPos)
{
	HIT hitArr[1];





	if (m_IsBeingReflected==false)
	{
		if (hitRegionPtr->Raycast(m_Pos, m_Vector, hitArr, 1, -1)>0)
		{

			return true;		

		}
	}
	else
	{
		//When being reflected

		//Raycast with main laser part
		if (hitRegionPtr->Raycast(m_Pos, m_ReflectionPtr[m_ReflectionCount], hitArr, 1, -1)>0)
		{

			return true;		

		}

		//Raycast with reflection parts
		for (int count=0; count < m_ReflectionCount; ++count)
		{			
			if (hitRegionPtr->Raycast(m_ReflectionPtr[m_ReflectionCount], m_ReflectionPtr[m_ReflectionCount+5], hitArr, 1, -1)>0)
			{

				return true;		

			}
		}
	}

	return false;
}

int LaserShot::GetAmountOfHits()
{
	return m_AmountOfHits;
}
