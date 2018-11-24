/*
	Author: Jorge Juaneda
	File: MonsterManager.h
*/

#pragma once
#include <vector>

#include "Monster.h"

class CMonsterManager
{
public:

	CMonsterManager* GetInstance();
	void DeleteInstance();

	void  FindLowesHealthEnemy();
	void FindClosestEnemy(CMonster * pMonster);

	void AddMonster( CMonster * pMonster);
	void RemoveMonster( CMonster * pMonster);



private:
	static CMonsterManager* s_pInstance;
	std::vector<CMonster *> m_vMonsters;

	CMonsterManager(void);
	~CMonsterManager(void);
	CMonsterManager( const CMonsterManager& );
	CMonsterManager* operator= ( const CMonsterManager& );
};

