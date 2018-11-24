#include "SnowArtifact.h"
#include "Projectile.h"
#include "AudioManager.h"
#include "EntityManager.h"
#include "CreateProjectileMsg.h"
#include "AddEntityToManager.h"
#include "MessageSystem.h"


CSnowArtifact::CSnowArtifact(void)
{
	RECT rRec;
	rRec.top = 0;
	rRec.left = 3;
	rRec.bottom = 33;
	rRec.right = 32;
	SetAbility1Rect(rRec);
	
	rRec.top = 0;
	rRec.left = 3;
	rRec.bottom = 33;
	rRec.right = 32;
	SetAbility2Rect(rRec);
	
	rRec.top = 0;
	rRec.left = 3;
	rRec.bottom = 33;
	rRec.right = 32;
	SetAbility3Rect(rRec);
}


CSnowArtifact::~CSnowArtifact(void)
{

}


void CSnowArtifact::Ability1 (int nX, int nY, int m_nDirec)
{
	switch( GetLevel() )
	{
	case 0:
		{
			CProjectile* pIce = new CProjectile();
			pIce->SetProjectile(300, m_nDirec, nX, nY);
			pIce->SetDamage(10);
			pIce->SetDuration(0.7f);
			pIce->SetElement(1);
			pIce->SetOwnerp(true);

			AddEntityToManager* Msg = new AddEntityToManager(pIce);
			CMessageSystem::GetInstace()->SendMsg(Msg);
			CAudioManager::GetInstance()->PlaySFX(SFX_ICE);
		}
		break;

	case 1:
		{
			CProjectile* pIce = new CProjectile();
			pIce->SetProjectile(300, m_nDirec, nX, nY);
			pIce->SetDamage(15);
			pIce->SetDuration(0.8f);
			pIce->SetElement(1);
			pIce->SetOwnerp(true);

			AddEntityToManager* Msg = new AddEntityToManager(pIce);
			CMessageSystem::GetInstace()->SendMsg(Msg);
			CAudioManager::GetInstance()->PlaySFX(SFX_ICE);
		}
		break;

	case 2:
		{
			CProjectile* pIce = new CProjectile();
			pIce->SetProjectile(300, m_nDirec, nX, nY);
			pIce->SetDamage(15);
			pIce->SetDuration(1);
			pIce->SetElement(1);
			pIce->SetOwnerp(true);

			AddEntityToManager* Msg = new AddEntityToManager(pIce);
			CMessageSystem::GetInstace()->SendMsg(Msg);
			CAudioManager::GetInstance()->PlaySFX(SFX_ICE);
		}
		break;

	case 3:
		{
			CProjectile* pIce = new CProjectile();
			pIce->SetProjectile(900, m_nDirec, nX, nY);
			pIce->SetDamage(15);
			pIce->SetDuration(0.5);
			pIce->SetElement(1);
			pIce->SetOwnerp(true);

			AddEntityToManager* Msg = new AddEntityToManager(pIce);
			CMessageSystem::GetInstace()->SendMsg(Msg);
			CAudioManager::GetInstance()->PlaySFX(SFX_ICE);
		}
		break;

	}

}

void CSnowArtifact::Ability2 (int nX, int nY, int m_nDirec)
{
	switch( GetLevel() )
	{
	case 0:

		break;

	case 1:

		break;

	case 2:

		break;

	}

}

void CSnowArtifact::Ability3 (int nX, int nY, int m_nDirec)
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