
/*
	Author: Jorge Juaneda
	File: Artifact.cpp
*/

#include "Artifact.h"
#include "Projectile.h"
#include "EntityManager.h"

#include "chakram.h"

CArtifact::CArtifact(void)
{
	m_nLevel = 0;
	
	m_rAbility1.top = 0;
	m_rAbility1.left = 3;
	m_rAbility1.bottom = 33;
	m_rAbility1.right = 32;
	
	m_rAbility2.top = 0;
	m_rAbility2.left = 35;
	m_rAbility2.bottom = 33;
	m_rAbility2.right = 67;
	
	m_rAbility3.top = 1;
	m_rAbility3.left = 74;
	m_rAbility3.bottom = 33;
	m_rAbility3.right = 97;
}


CArtifact::~CArtifact(void)
{
}



void CArtifact::LevelUp()
{
	m_nLevel++;
	if ( m_nLevel > 3)
		m_nLevel = 3;
}



/*virtual*/ void CArtifact::Ability1 (int nX, int nY, int m_nDirec)
{
}



/*virtual*/ void CArtifact::Ability2 (int nX, int nY, int m_nDirec)
{

}


/*virtual*/ void CArtifact::Ability3 (int nX, int nY, int m_nDirec)	
{

}