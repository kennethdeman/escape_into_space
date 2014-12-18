//---------------------------
// Includes
//---------------------------
#include "MassProjList.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

//---------------------------
// Constructor & Destructor
//---------------------------
MassProjList::MassProjList()
{
	for (int count=0; count < MAX_MASSPROJECTILES; ++count)
	{
		m_MassProjectilePtrArr[count]=nullptr;
	}
}

MassProjList::~MassProjList()
{
	RemoveAll();
}

//---------------------------
// Eigen methoden
//---------------------------

bool MassProjList::Add(MassProjectile* massProjectilePtr)
{
	for (int count=0; count < MAX_MASSPROJECTILES; ++count)
	{
		if (m_MassProjectilePtrArr[count] == nullptr)
		{
			m_MassProjectilePtrArr[count]=massProjectilePtr;
			return true;
		}
	}
	return false;
}

bool MassProjList::Remove(MassProjectile* massProjectilePtr)
{
	for (int count=0; count < MAX_MASSPROJECTILES; ++count)
	{
		if (m_MassProjectilePtrArr[count] == massProjectilePtr)
		{
			delete m_MassProjectilePtrArr[count];
			m_MassProjectilePtrArr[count] = nullptr; 
			return true;
		}
	}
	return false;
}

void MassProjList::RemoveAll()
{
	for (int count=0; count < MAX_MASSPROJECTILES; ++count)
	{
		delete m_MassProjectilePtrArr[count];
		m_MassProjectilePtrArr[count] = nullptr; 
	}
}


void MassProjList::Tick(double deltaTime)
{
	for (int count=0; count < MAX_MASSPROJECTILES; ++count)
	{
		if (m_MassProjectilePtrArr[count] != nullptr)
		{
			m_MassProjectilePtrArr[count]->Tick(deltaTime);

			//Circular collision between projectiles
			for(int count2 = count + 1; count2<MAX_MASSPROJECTILES; ++count2)
			{
				if (m_MassProjectilePtrArr[count2]!=nullptr)
				{
					DOUBLE2 newVelocity=m_MassProjectilePtrArr[count2]->HandleCircularCollision(
						m_MassProjectilePtrArr[count]->GetPos(),m_MassProjectilePtrArr[count]->GetVelocity() ,
						m_MassProjectilePtrArr[count]->GetRadius(), m_MassProjectilePtrArr[count]->GetMass() );
					if (newVelocity!=DOUBLE2(0,0))
						m_MassProjectilePtrArr[count]->SetVelocity(newVelocity);
				}
			}

			if (m_MassProjectilePtrArr[count]->GetPos().x<-10000 || m_MassProjectilePtrArr[count]->GetPos().x > 20000 ||
				m_MassProjectilePtrArr[count]->GetPos().y<-10000 || m_MassProjectilePtrArr[count]->GetPos().y > 20000 ||
				m_MassProjectilePtrArr[count]->ReadyToBeDestroyed()==true)
			{
				Remove(m_MassProjectilePtrArr[count]);
			}

		}

	}
}

void MassProjList::Paint(MATRIX3X2 matTransform)
{
	for (int count=0; count < MAX_MASSPROJECTILES; ++count)
	{
		if (m_MassProjectilePtrArr[count] != nullptr)
		{
			m_MassProjectilePtrArr[count]->Paint(matTransform);
		}


	}
}

void MassProjList::HandleCollision(HitRegion *hitLevelPtr, double deltaTime)
{
	for (int count=0; count < MAX_MASSPROJECTILES; ++count)
	{
		if (m_MassProjectilePtrArr[count] != nullptr)
		{
			m_MassProjectilePtrArr[count]->HandleCollision(hitLevelPtr, deltaTime);
		}


	}
}

DOUBLE2 MassProjList::HandleCircularCollision(DOUBLE2 otherCirclePos,DOUBLE2 otherCircleVelocity, double radius, double mass)
{
	DOUBLE2 velocity(0,0), newVelocity(0,0);
	velocity=otherCircleVelocity;
	for (int count=0; count < MAX_MASSPROJECTILES; ++count)
	{
		if (m_MassProjectilePtrArr[count] != nullptr)
		{
			newVelocity=m_MassProjectilePtrArr[count]->HandleCircularCollision(otherCirclePos,velocity ,radius, mass );
			if (newVelocity!=DOUBLE2(0,0))
				velocity= newVelocity;
		}

	}

	return velocity;

}

