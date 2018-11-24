#include "FireArtifact.h"
#include "Projectile.h"
#include "EntityManager.h"
#include "chakram.h"
#include "AudioManager.h"
#include "CreateProjectileMsg.h"
#include "MessageSystem.h"


FireArtifact::FireArtifact(void) 
{
	RECT rRec;
	rRec.top = 0;
	rRec.left = 3;
	rRec.bottom = 33;
	rRec.right = 32;
	SetAbility1Rect(rRec);
	
	rRec.top = 0;
	rRec.left = 35;
	rRec.bottom = 33;
	rRec.right = 67;
	SetAbility2Rect(rRec);
	
	rRec.top = 1;
	rRec.left = 74;
	rRec.bottom = 33;
	rRec.right = 97;
	SetAbility3Rect(rRec);

}


FireArtifact::~FireArtifact(void)
{
}


void FireArtifact::Ability1 (int nX, int nY, int m_nDirec)
{

	CAudioManager::GetInstance()->PlaySFX( SFX_FIREBALL );
	CreateProjectileMsg* Msg = new CreateProjectileMsg(300, m_nDirec, 0, nX, nY, false);
	Msg->SetLevel( GetLevel() );
	CMessageSystem::GetInstace()->SendMsg(Msg);

}

void FireArtifact::Ability2 (int nX, int nY, int m_nDirec)
{
	CreateProjectileMsg* Msg = new CreateProjectileMsg(300, m_nDirec, 0, nX, nY, true);
	Msg->SetLevel( GetLevel() );
	CMessageSystem::GetInstace()->SendMsg(Msg);
}

void FireArtifact::Ability3 (int nX, int nY, int m_nDirec)
{
	switch( GetLevel() )
	{
	case 1:

		break;

	case 2:

		break;

	case 3:

		break;

	}

}