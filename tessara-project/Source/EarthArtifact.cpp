#include "EarthArtifact.h"
#include "Projectile.h"
#include "EntityManager.h"
#include "AudioManager.h"
#include "CreateProjectileMsg.h"
#include "AddEntityToManager.h"
#include "MessageSystem.h"

CEarthArtifact::CEarthArtifact(void)
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


CEarthArtifact::~CEarthArtifact(void)
{
}

void CEarthArtifact::Ability1 (int nX, int nY, int m_nDirec)
{
	switch( GetLevel() )
	{
	case 0:
		{

			CProjectile* pEarth = new CProjectile();
			pEarth->SetProjectile(200, m_nDirec, nX, nY);
			pEarth->SetElement(3);
			pEarth->SetDuration(1.5f);
			pEarth->SetDamage(10);
			pEarth->SetOwnerp(false);

			AddEntityToManager* Msg = new AddEntityToManager(pEarth);
			CMessageSystem::GetInstace()->SendMsg(Msg);
			CAudioManager::GetInstance()->PlaySFX( SFX_EARTHSHARD );
		}
		break;

	case 1:
		{

			CProjectile* pEarth = new CProjectile();
			pEarth->SetProjectile(300, m_nDirec, nX, nY);
			pEarth->SetElement(3);
			pEarth->SetDuration(2.0f);
			pEarth->SetDamage(15);
			pEarth->SetOwnerp(false);

			AddEntityToManager* Msg = new AddEntityToManager(pEarth);
			CMessageSystem::GetInstace()->SendMsg(Msg);
			CAudioManager::GetInstance()->PlaySFX( SFX_EARTHSHARD );
		}
		break;

	case 2:
		{

			CProjectile* pEarth = new CProjectile();
			pEarth->SetProjectile(300, m_nDirec, nX, nY);
			pEarth->SetElement(3);
			pEarth->SetDuration(3.0f);
			pEarth->SetDamage(20);
			pEarth->SetPierce(true);
			pEarth->SetOwnerp(false);

			AddEntityToManager* Msg = new AddEntityToManager(pEarth);
			CMessageSystem::GetInstace()->SendMsg(Msg);
			CAudioManager::GetInstance()->PlaySFX( SFX_EARTHSHARD );
		}
		break;

	case 3:
		{
			CProjectile* pEarth = new CProjectile();
			pEarth->SetProjectile(300, m_nDirec, nX, nY);
			pEarth->SetElement(3);
			pEarth->SetDuration(4.0f);
			pEarth->SetDamage(25);
			pEarth->SetPierce(true);
			pEarth->SetOwnerp(false);

			AddEntityToManager* Msg = new AddEntityToManager(pEarth);
			CMessageSystem::GetInstace()->SendMsg(Msg);
			CAudioManager::GetInstance()->PlaySFX( SFX_EARTHSHARD );
		}
		break;
	}

}

void CEarthArtifact::Ability2 (int nX, int nY, int m_nDirec)
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

void CEarthArtifact::Ability3 (int nX, int nY, int m_nDirec)
{
	switch( GetLevel()  )
	{
	case 1:

		break;

	case 2:

		break;

	case 3:

		break;

	}

}