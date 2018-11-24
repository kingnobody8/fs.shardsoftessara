#include "Lightning.h"
#include "AnimationManager.h"

#include "..\\SGD Wrappers\CSGD_TextureManager.h"
#include "..\\SGD Wrappers\CSGD_EventSystem.h"
#include "DeleteProjectileMessage.h"
#include "MessageSystem.h"
#include "LevelManager.h"
#include "Camera.h"
#include "Monster.h"

CLightning::CLightning(void)
{

	SetWidth( 20 );
	SetHeight( 20 );
	m_nStunChance = 30;

	m_bCollided = false;

	this->m_eType = ENT_PROJECTILE;

	SetCurrAnim(AnimationManager::GetInstance()->GetAnimID("lightning"));
	CEntity::AddAnimInfo(GetCurrAnim());

	SetDamage(10);
	SetDuration(2);
}


CLightning::~CLightning(void)
{
}

 void CLightning::Render()
{
	CProjectile::Render();
 }


void CLightning::Update( float deltaTime )
{
	CProjectile::Update(deltaTime);
}


bool CLightning::CheckCollision( IEntity* pOther)
{
	RECT rOther = pOther->GetRect();
	RECT rOver;
	RECT rRect = GetRect();


	if (IntersectRect( &rOver, &rOther, &rRect ) == TRUE )
	{
		if (pOther->GetType() == ENT_ARCHER || pOther->GetType() == ENT_WIGHT || pOther->GetType() == ENT_WRAITH || pOther->GetType() == ENT_BOSS || pOther->GetType() == ENT_ZOMBIE)
		{
			CMonster * pMonster = dynamic_cast< CMonster* >( pOther);

			//physical damage
			int d = -GetDamage();
			CSGD_EventSystem::GetInstance()->SendEventNow(
				"TAKE_DAMAGE", &d, pOther, this );

			CSGD_EventSystem::GetInstance()->SendEventNow(
				"TAKE_LIGHTNING_DAMAGE", &d, pOther, this);


			if (m_nStunChance > rand() % 100)
				CSGD_EventSystem::GetInstance()->SendEventNow(
				"STUN", &d, pOther, this );

			CDeleteProjectileMessage* pMsg = new CDeleteProjectileMessage( this );
			CMessageSystem::GetInstace()->SendMsg( pMsg );
			pMsg = nullptr;
		}

		return true;
	}
	return false;
}


RECT CLightning::GetRect ()
{
	return CProjectile::GetRect();
}

void CLightning::SetProjectile(float fVel, int nDirection, int nX, int nY)
{

	SetDirection(nDirection);

	switch(nDirection)
	{
	case 0:
		SetAccX(0);
		SetAccY(-fVel);
		break;

	case 1:
		SetAccX(0);
		SetAccY(fVel);
		break;

	case 2:
		SetAccY(0);
		SetAccX(-fVel);
		break;

	case 3:
		SetAccY(0);
		SetAccX(fVel);
		break;
	}	

	SetPosX(nX);
	SetPosY(nY);

}

