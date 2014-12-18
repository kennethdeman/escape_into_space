//---------------------------
// Includes
//---------------------------
#include "GravityBallList.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

//---------------------------
// Constructor & Destructor
//---------------------------
GravityBallList::GravityBallList()
{
	for (int count=0; count < MAX_GRAVITYBALLS; ++count)
	{
		m_GravityBallPtrArr[count]=nullptr;
	}
}

GravityBallList::~GravityBallList()
{
	RemoveAll();
}

//---------------------------
// Eigen methoden
//---------------------------

bool GravityBallList::Add(GravityBall* gravityBallPtr)
{
	for (int count=0; count < MAX_GRAVITYBALLS; ++count)
	{
		if (m_GravityBallPtrArr[count] == nullptr)
		{
			m_GravityBallPtrArr[count]=gravityBallPtr;
			return true;
		}
	}
	return false;
}

bool GravityBallList::Remove(GravityBall* gravityBallPtr)
{
	for (int count=0; count < MAX_GRAVITYBALLS; ++count)
	{
		if (m_GravityBallPtrArr[count] == gravityBallPtr)
		{
			delete m_GravityBallPtrArr[count];
			m_GravityBallPtrArr[count] = nullptr; 
			return true;
		}
	}
	return false;
}

void GravityBallList::RemoveAll()
{
	for (int count=0; count < MAX_GRAVITYBALLS; ++count)
	{
		delete m_GravityBallPtrArr[count];
		m_GravityBallPtrArr[count] = nullptr; 
	}
}


void GravityBallList::Tick(double deltaTime)
{
	for (int count=0; count < MAX_GRAVITYBALLS; ++count)
	{
		if (m_GravityBallPtrArr[count] != nullptr)
		{
			m_GravityBallPtrArr[count]->Tick(deltaTime);


			if (m_GravityBallPtrArr[count]->GetPos().x<-10000 || m_GravityBallPtrArr[count]->GetPos().x > 20000 ||
				m_GravityBallPtrArr[count]->GetPos().y<-10000 || m_GravityBallPtrArr[count]->GetPos().y > 20000 ||
				m_GravityBallPtrArr[count]->ReadyToBeDestroyed()==true)
			{
				Remove(m_GravityBallPtrArr[count]);
			}

		}

	}
}

void GravityBallList::Paint(MATRIX3X2 matTransform)
{
	for (int count=0; count < MAX_GRAVITYBALLS; ++count)
	{
		if (m_GravityBallPtrArr[count] != nullptr)
		{
			m_GravityBallPtrArr[count]->Paint(matTransform);
		}


	}
}

void GravityBallList::HandleCollision(HitRegion *hitLevelPtr, double deltaTime)
{
	for (int count=0; count < MAX_GRAVITYBALLS; ++count)
	{
		if (m_GravityBallPtrArr[count] != nullptr)
		{
			m_GravityBallPtrArr[count]->HandleCollision(hitLevelPtr, deltaTime);
		}			
	}
}

DOUBLE2 GravityBallList::HandleCircularCollision(DOUBLE2 otherCirclePos,DOUBLE2 otherCircleVelocity, double radius, double mass)
{
	DOUBLE2 velocity(0,0), newVelocity(0,0);
	velocity=otherCircleVelocity;
	for (int count=0; count < MAX_GRAVITYBALLS; ++count)
	{
		if (m_GravityBallPtrArr[count] != nullptr)
		{
			newVelocity=m_GravityBallPtrArr[count]->HandleCircularCollision(otherCirclePos,velocity ,radius, mass );
			if (newVelocity!=DOUBLE2(0,0))
				velocity= newVelocity;
		}

	}

	return velocity;					
}



DOUBLE2 GravityBallList::ExerciseForces(DOUBLE2 objectPos, double objectMass)
{
	DOUBLE2 sumOfForces(0,0);
	for (int count=0; count < MAX_GRAVITYBALLS; ++count)
	{
		if (m_GravityBallPtrArr[count] != nullptr)
		{
			sumOfForces+=m_GravityBallPtrArr[count]->ExerciseForce(objectPos,objectMass);
		}			
	}

	return sumOfForces;
}

DOUBLE2 GravityBallList::CastOutExplosionForces(DOUBLE2 targetPos)
{
	DOUBLE2 sumOfForces(0,0);
	for (int count=0; count < MAX_GRAVITYBALLS; ++count)
	{
		if (m_GravityBallPtrArr[count] != nullptr)
		{
			sumOfForces+=m_GravityBallPtrArr[count]->CastOutExplosionForce(targetPos);
		}			
	}

	return sumOfForces;
}


void GravityBallList::AddForce(DOUBLE2 force)
{
	for (int count=0; count < MAX_GRAVITYBALLS; ++count)
	{
		if (m_GravityBallPtrArr[count] != nullptr)
		{
			m_GravityBallPtrArr[count]->AddForce(force);
		}			
	}
}
