/*
	Author: Jorge Juaneda
	File: Projectile.cpp
*/

#include "Projectile.h"

#include "..\\SGD Wrappers\CSGD_Direct3D.h"
#include "EntityManager.h"
#include "Entity.h"
#include "../SGD Wrappers/CSGD_EventSystem.h"
#include "DeleteProjectileMessage.h"
#include "MessageSystem.h"
#include "ParticleEffect.h"
#include "ParticleManager.h"
#include "AnimationManager.h"
#include "LevelManager.h"

CProjectile::CProjectile(void)
{
	m_nLevel = 0;
	m_bExplosion = false;
	m_fExplosionTimer = 0.0f;
	m_nDirecction = 0;
	m_fVelocity = 0.0;
	m_fAccX = 0.0;
	m_fAccY = 0.0;
	m_nDamage = 20;
	this->m_eType = ENT_PROJECTILE;
	PEffect = nullptr;
	m_bOwnerP = true;
	m_bPierce = false;
	m_fTimer = 0.0f;
}

void CProjectile::SetProjectile(float fVel, int nDirection, int nX, int nY)
{

	m_nDirecction = nDirection;
	m_fVelocity = fVel;
	SetPosX(nX);
	SetPosY(nY);


	m_fAccX = 0.0;
	m_fAccY = 0.0;
	m_fTimer = 0;
	m_fDuration = 6;
	m_nDamage = 20;
	m_fTimer = 0.0f;
}


CProjectile::~CProjectile(void)
{
	if (PEffect != nullptr)
	{
		delete PEffect;
		PEffect = nullptr;
	}
}


void CProjectile::Render()
{
	if (PEffect != nullptr)
		PEffect->Render();
	CEntity::Render();
}


void CProjectile::Update( float deltaTime )
{
	if (m_bExplosion)
	{
		if (GetAnimationInformation(GetCurrAnim())->IsOver() )
		{
			CDeleteProjectileMessage* pMsg = new CDeleteProjectileMessage( this );
			CMessageSystem::GetInstace()->SendMsg( pMsg );
			pMsg = nullptr;
			return;
		}
	}

	if (m_fAccY == 0 && m_fAccX == 0 )
	{
		switch( m_nDirecction )
		{

		case 0:
				m_fAccY = - m_fVelocity;
			break;

		case 1:
				m_fAccY = + m_fVelocity;

			break;

		case 2:
				m_fAccX = - m_fVelocity;
			break;

		case 3:
				m_fAccX = + m_fVelocity;
			break;
		}
	}

	if (PEffect != nullptr)
	{
		PEffect->SetXPosition(GetPosX());
		PEffect->SetYPosition(GetPosY());

		/////////////////////////////////////////////////////////////////
		// BUG FIX
		// Reference Bug # BB-006
		// BUG FIX START
		/////////////////////////////////////////////////////////////////
		
		// Commented out this code, aligned particles with fireball when shooting left

		/*if (m_nDirecction == 2)
			PEffect->SetXPosition(GetPosX() - 50);

		if (m_nDirecction == 4)
			PEffect->SetYPosition(GetPosY() + 50);*/
		
		///////////////////////////////////////////////////////////////////////////
		// BUG FIX END  Reference # BB-006
		//////////////////////////////////////////////////////////////////////////
		

		PEffect->Update( deltaTime );
	}

	RECT rec;
	rec = LevelManager::GetInstance()->Collision(this);

	if (!m_bExplosion && rec.left != 0 && rec.right != 0)
	{
		if (m_eElement == 0 && !m_bExplosion && m_nLevel == 3)
		{
			m_bExplosion = true;
			SetCurrAnim( AnimationManager::GetInstance()->GetAnimID("explosion"));
			CEntity::AddAnimInfo(GetCurrAnim());
			SetCurrAnim(1);
			GetAnimationInformation( GetCurrAnim() )->Reset();
			return;
		}

		CDeleteProjectileMessage* pMsg = new CDeleteProjectileMessage( this );
		CMessageSystem::GetInstace()->SendMsg( pMsg );
		pMsg = nullptr;
		return;
	}

	m_fTimer += deltaTime;

	//Update Position and animation
	if (!m_bExplosion)
	{
		SetPosX( GetPosX() + (int)(m_fAccX * deltaTime));
		SetPosY( GetPosY() + (int)(m_fAccY * deltaTime));
	}


	this->m_vAnimInfos[m_nCurrAnim]->Update(deltaTime);


	if (m_fTimer >= m_fDuration)
	{
		CDeleteProjectileMessage* pMsg = new CDeleteProjectileMessage( this );
		CMessageSystem::GetInstace()->SendMsg( pMsg );
		pMsg = nullptr;
	}
}


bool CProjectile::CheckCollision( IEntity* pOther)
{
	if (CEntity::CheckCollision(pOther))
	{
		if (pOther->GetType() == ENT_PLAYER || pOther->GetType() == ENT_ITEM)
			return true;
		int d = -m_nDamage;

		if (m_bExplosion)
		{
			
			for (unsigned int i = 0; i < ExplosionVec.size(); ++i)
				if (pOther == ExplosionVec[i])
					return false;

			CSGD_EventSystem::GetInstance()->SendEventNow(
					"TAKE_FIRE_DAMAGE", &d, pOther, this);

			ExplosionVec.push_back(pOther);
		}

		switch(GetElement())
		{
		case 0:
			{
				CSGD_EventSystem::GetInstance()->SendEventNow(
					"TAKE_FIRE_DAMAGE", &d, pOther, this);

				if (!m_bExplosion && m_nLevel == 3)
				{
					m_bExplosion = true;
					SetCurrAnim( AnimationManager::GetInstance()->GetAnimID("explosion"));
					CEntity::AddAnimInfo(GetCurrAnim());
					SetCurrAnim(1);
					GetAnimationInformation( GetCurrAnim() )->Reset();
				}
			}
			break;
		case 1:
			{
				CSGD_EventSystem::GetInstance()->SendEventNow(
					"TAKE_ICE_DAMAGE", &d, pOther, this);
			}
			break;

		case 2:
			{
				CSGD_EventSystem::GetInstance()->SendEventNow(
					"TAKE_ELECTRIC_DAMAGE", &d, pOther, this);
			}
			break;
		case 3:
			{
				CSGD_EventSystem::GetInstance()->SendEventNow(
					"TAKE_EARTH_DAMAGE", &d, pOther, this);
			}
			break;

		case 4:
			{
				CSGD_EventSystem::GetInstance()->SendEventNow(
					"TAKE_DAMAGE", &d, pOther, this );
			}
			break;
		}

		if (!m_bPierce && !m_bExplosion)
		{
			CDeleteProjectileMessage* pMsg = new CDeleteProjectileMessage( this );
			CMessageSystem::GetInstace()->SendMsg( pMsg );
			pMsg = nullptr;
		}

		return true;
	}

	return false;	
}

RECT CProjectile::GetRect ()
{
	Frame tempFrame = AnimationManager::GetInstance()->GetAnimation(this->m_vAnimInfos[this->m_nCurrAnim]->GetAnimID()).GetFrame(this->m_vAnimInfos[this->m_nCurrAnim]->GetCurFrame());
	POINT pAnchor = tempFrame.GetAnchorPoint();
	RECT rPassive = *tempFrame.GetActiveRect();
	RECT rRect;

	int nHeight = rPassive.bottom - rPassive.top;
	int nWidth = rPassive.right - rPassive.left;
	int nX = (int)GetPosX();
	int nY = (int)GetPosY();

	rRect.bottom = nY + (rPassive.bottom - pAnchor.y);
	rRect.top = rRect.bottom - nHeight;
	rRect.right = nX + (rPassive.right - pAnchor.x);
	rRect.left = rRect.right - nWidth;

	return rRect;
}

 void CProjectile::SetElement(int eElm)
 {
	 this->m_eElement = (eElement)eElm;

	 switch(eElm)
	 {
	 case 0:
		 {
			switch(m_nDirecction)
			{
			case 0:
			SetCurrAnim(AnimationManager::GetInstance()->GetAnimID("fireballUp"));
				break;
			case 1:
			SetCurrAnim(AnimationManager::GetInstance()->GetAnimID("fireballDown"));
				break;
			case 2:
			SetCurrAnim(AnimationManager::GetInstance()->GetAnimID("fireballLeft"));
				break;
			case 3:
			SetCurrAnim(AnimationManager::GetInstance()->GetAnimID("fireballRight"));
				break;
			}

			CEntity::AddAnimInfo(GetCurrAnim());
			SetCurrAnim(0);
			PEffect = new CParticleEffect;
			CParticleManager::GetInstance()->GetEffect(*PEffect, FX_FIREBALL);
			PEffect->SetXPosition(GetPosX());
			PEffect->SetYPosition(GetPosY());

			PEffect->ToggleCamera();
			PEffect->Initialize();
		 }
		 break;
	 case 1:
		 {
			switch(m_nDirecction)
			{
			case 0:
			SetCurrAnim(AnimationManager::GetInstance()->GetAnimID("iceboltUp"));
				break;
			case 1:
			SetCurrAnim(AnimationManager::GetInstance()->GetAnimID("iceboltDown"));
				break;
			case 2:
			SetCurrAnim(AnimationManager::GetInstance()->GetAnimID("iceboltLeft"));
				break;
			case 3:
			SetCurrAnim(AnimationManager::GetInstance()->GetAnimID("iceboltRight"));
				break;
			}

			CEntity::AddAnimInfo(GetCurrAnim());
			SetCurrAnim(0);
			PEffect = new CParticleEffect;
			CParticleManager::GetInstance()->GetEffect(*PEffect, FX_ICEBALL);
			PEffect->SetXPosition(GetPosX());
			PEffect->SetYPosition(GetPosY());
			PEffect->ToggleCamera();
			PEffect->Initialize();
		 }
		 break;
	 case 2:
		 {
			SetCurrAnim(AnimationManager::GetInstance()->GetAnimID("lightning"));
			CEntity::AddAnimInfo(GetCurrAnim());
			SetCurrAnim(0);
			PEffect = new CParticleEffect;
			CParticleManager::GetInstance()->GetEffect(*PEffect, FX_LIGHTNINGBOLT);
			PEffect->SetXPosition(GetPosX());
			PEffect->SetYPosition(GetPosY());
			PEffect->ToggleCamera();
			PEffect->Initialize();
			
		 }
		 break;
	 case 3:
		 {
			switch(m_nDirecction)
			{
			case 0:
			SetCurrAnim(AnimationManager::GetInstance()->GetAnimID("earthShardUp"));
				break;
			case 1:
			SetCurrAnim(AnimationManager::GetInstance()->GetAnimID("earthShardDown"));
				break;
			case 2:
			SetCurrAnim(AnimationManager::GetInstance()->GetAnimID("earthShardLeft"));
				break;
			case 3:
			SetCurrAnim(AnimationManager::GetInstance()->GetAnimID("earthShardRight"));
				break;
			}

			PEffect = new CParticleEffect;
			CParticleManager::GetInstance()->GetEffect(*PEffect, FX_EARTHSHARD);
			PEffect->SetXPosition(GetPosX());
			PEffect->SetYPosition(GetPosY());
			PEffect->ToggleCamera();
			PEffect->Initialize();
			CEntity::AddAnimInfo(GetCurrAnim());
			SetCurrAnim(0);
		 }
	 case 4:
		 {
			switch(m_nDirecction)
			{
			case 0:
			SetCurrAnim(AnimationManager::GetInstance()->GetAnimID("arrowUp"));
				break;
			case 1:
			SetCurrAnim(AnimationManager::GetInstance()->GetAnimID("arrowDown"));
				break;
			case 2:
			SetCurrAnim(AnimationManager::GetInstance()->GetAnimID("arrowLeft"));
				break;
			case 3:
			SetCurrAnim(AnimationManager::GetInstance()->GetAnimID("arrowRight"));
				break;
			}

			CEntity::AddAnimInfo(GetCurrAnim());
			SetCurrAnim(0);
		 }
		 break;
	 }
 }