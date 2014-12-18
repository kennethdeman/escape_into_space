//---------------------------
// Includes
//---------------------------
#include "LaserList.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

//---------------------------
// Constructor & Destructor
//---------------------------
LaserList::LaserList()
{
	for (int count=0; count < MAX_LASERSHOTS; ++count)
	{
		m_LaserShotPtrArr[count]=nullptr;
	}
}

LaserList::~LaserList()
{
	RemoveAll();
}

//---------------------------
// Eigen methoden
//---------------------------

bool LaserList::Add(LaserShot* laserPtr)
{
	for (int count=0; count < MAX_LASERSHOTS; ++count)
	{
		if (m_LaserShotPtrArr[count] == nullptr)
		{
			m_LaserShotPtrArr[count]=laserPtr;
			return true;
		}
	}
	return false;
}

bool LaserList::Remove(LaserShot* laserPtr)
{
	for (int count=0; count < MAX_LASERSHOTS; ++count)
	{
		if (m_LaserShotPtrArr[count] == laserPtr)
		{
			delete m_LaserShotPtrArr[count];
			m_LaserShotPtrArr[count] = nullptr; 
			return true;
		}
	}
	return false;
}

void LaserList::RemoveAll()
{
	for (int count=0; count < MAX_LASERSHOTS; ++count)
	{
		delete m_LaserShotPtrArr[count];
		m_LaserShotPtrArr[count] = nullptr; 
	}
}


void LaserList::Tick(double deltaTime)
{
	for (int count=0; count < MAX_LASERSHOTS; ++count)
	{
		if (m_LaserShotPtrArr[count] != nullptr)
		{
			m_LaserShotPtrArr[count]->Tick(deltaTime);

			if (m_LaserShotPtrArr[count]->GetPos().x<-10000 || m_LaserShotPtrArr[count]->GetPos().x > 20000 ||
				m_LaserShotPtrArr[count]->GetPos().y<-10000 || m_LaserShotPtrArr[count]->GetPos().y > 20000
				|| m_LaserShotPtrArr[count]->GetAmountOfHits()>=10)
			{
				Remove(m_LaserShotPtrArr[count]);
			}

		}
			
	}
}

void LaserList::Paint(MATRIX3X2 matTransform)
{
	for (int count=0; count < MAX_LASERSHOTS; ++count)
	{
		if (m_LaserShotPtrArr[count] != nullptr)
		{
			m_LaserShotPtrArr[count]->Paint(matTransform);
		}

			
	}
}

void LaserList::HandleCollision(HitRegion *hitLevelPtr, double deltaTime)
{
	for (int count=0; count < MAX_LASERSHOTS; ++count)
	{
		if (m_LaserShotPtrArr[count] != nullptr)
		{
			m_LaserShotPtrArr[count]->HandleCollision(hitLevelPtr, deltaTime);
		}

			
	}
}


double LaserList::CheckHitCollision(HitRegion *hitRegionPtr, DOUBLE2 targetPos)
{
	double totalDamage=0;
	for (int count=0; count < MAX_LASERSHOTS; ++count)
	{
		if (m_LaserShotPtrArr[count] != nullptr)
		{
			if (m_LaserShotPtrArr[count]->CheckHitCollision(hitRegionPtr,targetPos)==true)
			{
				totalDamage+=20;
				Remove(m_LaserShotPtrArr[count]);
			}

		}

			
	}

	return totalDamage;
}

