/*
Author: Jorge Juaneda
File: MonaterManager.cpp
*/

#include "MonsterManager.h"
#include <cassert>


/*static*/CMonsterManager* CMonsterManager::s_pInstance = nullptr;
/*static*/CMonsterManager* CMonsterManager::GetInstance( void )
{
	if ( s_pInstance == nullptr )
		s_pInstance = new CMonsterManager();

	return s_pInstance;
}

/* This function needs to be called in the destructor or shutdown
function of the GamePlayState. It deletes the instance of
CEntityManager */
/*static*/void CMonsterManager::DeleteInstance( void )
{
	delete s_pInstance;
	s_pInstance = nullptr;
}


CMonsterManager::CMonsterManager(void)
{
}


CMonsterManager::~CMonsterManager(void)
{
	for (unsigned int i = 0; i < m_vMonsters.size(); i++)
		m_vMonsters[i]->Release();

	m_vMonsters.clear();
}


void  CMonsterManager::FindLowesHealthEnemy( )
{
	int nHealth = 0;
	CMonster* LowHMonster = nullptr;

	std::vector< CMonster* >::iterator iter = m_vMonsters.begin();
	for ( ; iter != m_vMonsters.end(); ++iter )
	{
		if ( (*iter)->GetHealth() < nHealth)
		{
			LowHMonster = (*iter);
			nHealth = LowHMonster->GetHealth();
		}
	}

}


void  CMonsterManager::FindClosestEnemy(CMonster * pMonster)
{
	std::vector< CMonster* >::iterator iter = m_vMonsters.begin();
	for ( ; iter != m_vMonsters.end(); ++iter )
	{
		if ( *iter == pMonster )
		{
			// Found the entity
			// Release the reference to the Entity!
			pMonster->Release();

			// Remove from list
			m_vMonsters.erase(iter);
			break;
		}
	}

}

void CMonsterManager::AddMonster( CMonster * pMonster)
{
	// Add the entity
	m_vMonsters.push_back( pMonster );

	//Add a reference to the entity!
	pMonster->AddRef();

}


void CMonsterManager::RemoveMonster( CMonster * pMonster)
{
	std::vector< CMonster* >::iterator iter = m_vMonsters.begin();
	for ( ; iter != m_vMonsters.end(); ++iter )
	{
		if ( *iter == pMonster )
		{
			// Found the entity
			// Release the reference to the Entity!
			pMonster->Release();

			// Remove from list
			m_vMonsters.erase(iter);
			break;
		}
	}
}