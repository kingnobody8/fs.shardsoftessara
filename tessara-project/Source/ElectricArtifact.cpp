#include "ElectricArtifact.h"
#include "Projectile.h"
#include "AudioManager.h"
#include "EntityManager.h"
#include "CreateProjectileMsg.h"
#include "CreateTripleShootMsg.h"
#include "AddEntityToManager.h"
#include "MessageSystem.h"

#include "Lightning.h"


CElectricArtifact::CElectricArtifact(void)
{
	RECT rRec;
	rRec.top = 0;
	rRec.left = 35;
	rRec.bottom = 33;
	rRec.right = 67;
	SetAbility1Rect(rRec);
	
	rRec.top = 0;
	rRec.left = 35;
	rRec.bottom = 33;
	rRec.right = 67;
	SetAbility2Rect(rRec);
	
	rRec.top = 0;
	rRec.left = 35;
	rRec.bottom = 33;
	rRec.right = 67;
	SetAbility3Rect(rRec);
}


CElectricArtifact::~CElectricArtifact(void)
{
}

void CElectricArtifact::Ability1 (int nX, int nY, int m_nDirec)
{
	switch( GetLevel() )
	{
	case 0:
		{

			CLightning* pLightning = new CLightning();
			pLightning->SetProjectile(500, m_nDirec, nX, nY);
			pLightning->SetElement(2);
			pLightning->SetOwnerp(false);
			pLightning->SetStunChance(0);
			pLightning->SetDamage(10);
			AddEntityToManager* Msg = new AddEntityToManager(pLightning);
			CMessageSystem::GetInstace()->SendMsg(Msg);
			pLightning = nullptr;

			CAudioManager::GetInstance()->PlaySFX( SFX_BOLT );	
		}
		break;

	case 1:
		{
			CLightning* pLightning = new CLightning();
			pLightning->SetProjectile(500, m_nDirec, nX, nY);
			pLightning->SetElement(2);
			pLightning->SetOwnerp(false);
			pLightning->SetStunChance(20);
			pLightning->SetDamage(16);
			AddEntityToManager* Msg = new AddEntityToManager(pLightning);
			CMessageSystem::GetInstace()->SendMsg(Msg);
			pLightning = nullptr;

			CAudioManager::GetInstance()->PlaySFX( SFX_BOLT );	
		}
		break;

	case 2:
		{
			CreateTripleShootMsg * Msg = new CreateTripleShootMsg(500, m_nDirec, 2, nX, nY, false);
			CMessageSystem::GetInstace()->SendMsg(Msg);
			CAudioManager::GetInstance()->PlaySFX( SFX_BOLT );	
			CAudioManager::GetInstance()->PlaySFX( SFX_BOLT );	
			CAudioManager::GetInstance()->PlaySFX( SFX_BOLT );	
		}
		break;

	case 3:
		{
			CreateTripleShootMsg * Msg = new CreateTripleShootMsg(500, m_nDirec, 2, nX, nY, false);
			CMessageSystem::GetInstace()->SendMsg(Msg);
			CAudioManager::GetInstance()->PlaySFX( SFX_BOLT );	
			CAudioManager::GetInstance()->PlaySFX( SFX_BOLT );	
			CAudioManager::GetInstance()->PlaySFX( SFX_BOLT );	
		}
		break;

	}

}

void CElectricArtifact::Ability2 (int nX, int nY, int m_nDirec)
{
	switch( GetLevel() )
	{
	case 1:
		{

		}
		break;

	case 2:

		break;

	case 3:

		break;

	}

}