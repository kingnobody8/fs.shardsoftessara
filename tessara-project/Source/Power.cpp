/*
File:		Power.cpp
Author:		Daniel Habig
Course:		SGP
Purpose:	The power class acts as an entity that can be charged with lightning to power doors
*/

#include "Power.h"
#include "AnimationManager.h"
#include "ParticleManager.h"
#include "ParticleEffect.h"

Power::Power(void)
{
	AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("lightningSwitchOn"));
	AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("lightningSwitchOff"));
	SetCurrAnim(1);
	SetType( ENT_POW );

	SetHeight(64);
	SetWidth(64);

	this->m_pDoor = nullptr;
	this->time = 0.0f;
	this->active = false;
	ActiveEffect = nullptr;
}


Power::~Power(void)
{
	if( this->m_pDoor != nullptr )
		this->m_pDoor->Release();
	this->m_pDoor = nullptr;
}

void Power::Update( float deltaTime )
{
	CEntity::Update(deltaTime);

	if (ActiveEffect != nullptr)
		ActiveEffect->Update( deltaTime );

	if( active )
	{
		time -= deltaTime;
		if( time < 0.0f)
			Deactivate();
	}

}
void Power::Render( void )
{
	CEntity::Render();
	if (ActiveEffect != nullptr)
		ActiveEffect->Render();
}

bool Power::CheckCollision( IEntity* pOther )
{
	if(pOther->GetType() == ENT_PROJECTILE)
	{
		CProjectile* proj = dynamic_cast<CProjectile*>(pOther);
		if( proj->GetElement() == 2 )
		{
			RECT ent = proj->GetRect();
			RECT sect, other;
			other = this->GetRect();
			if( IntersectRect( &sect, &ent, &other ) == TRUE )
				Activate();
			return true;
		}
	}
	return false;
}

/*virtual*/ RECT Power::GetRect ()
{
	Frame tempFrame = AnimationManager::GetInstance()->GetAnimation(this->m_vAnimInfos[this->m_nCurrAnim]->GetAnimID()).GetFrame(this->m_vAnimInfos[this->m_nCurrAnim]->GetCurFrame());
	POINT pAnchor = tempFrame.GetAnchorPoint();
	RECT rPassive = *tempFrame.GetPassiveRect();
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

void Power::Activate(void)
{
	this->active = true;
	this->m_pDoor->Activate();
	SetCurrAnim(0);
	this->time = APT;
	if (ActiveEffect == nullptr)
	{
		ActiveEffect = new CParticleEffect;
		CParticleManager::GetInstance()->GetEffect( *ActiveEffect, FX_LIGHTNINGDASH );
		ActiveEffect->ToggleCamera();
		ActiveEffect->SetXPosition( GetPosX() - 20 );
		ActiveEffect->SetYPosition( GetPosY() - 40 );
		ActiveEffect->Initialize();
	}

}

void Power::Deactivate(void)
{
	active = false;
	this->m_pDoor->Deactivate();
	SetCurrAnim(1);
	if (ActiveEffect != nullptr)
	{
		delete ActiveEffect;
		ActiveEffect = nullptr;
	}

}

