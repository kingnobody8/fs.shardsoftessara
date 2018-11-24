/*
	Author: Jorge Juaneda
	File: Wight.cpp
*/

#include "Wight.h"
#include "AnimationManager.h"
#include "Player.h"
#include "AudioManager.h"
#include "Entity.h"
#include "PlayState.h"
#include "../SGD Wrappers/CSGD_EventSystem.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/SGD_Math.h"
#include "Camera.h"
#include "AnimationManager.h"
#include "MessageSystem.h"
#include "AddCurrencyMessage.h"
#include "LevelManager.h"
#include "ItemManager.h"

#include "Player.h"

CWight::CWight(void)
{
	BloodTimer = 0.0f;
	BloodEffect = nullptr;

	SetType( ENT_WIGHT );
	m_pTarget = nullptr;
	StunEffect = nullptr;
	m_nHealth = 100;
	m_nMaxHealth = 100;
	m_bAttacking = false;
	m_bInRange = false;
	m_nExperience = 120;
	m_bCanAttack = true;
	m_nDamage = -10;
	m_fPathTimer = 3.0f;
	Aggroed = false;
	m_fStunTimer = 0.0f;
	m_nMeleeRange = 75;
	m_fAttackTimer = 0.0f;
	m_nSpeed = 150;
	m_bHasAlreadyHit = false;
	m_fTakeFireDamage = 0.0f;

	m_bDead = false;

	CSGD_EventSystem::GetInstance()->RegisterClient( "TAKE_DAMAGE", this );
	CSGD_EventSystem::GetInstance()->RegisterClient( "TAKE_FIRE_DAMAGE", this );
	CSGD_EventSystem::GetInstance()->RegisterClient( "TAKE_ICE_DAMAGE", this );
	CSGD_EventSystem::GetInstance()->RegisterClient( "TAKE_ELECTRIC_DAMAGE", this );
	CSGD_EventSystem::GetInstance()->RegisterClient( "TAKE_EARTH_DAMAGE", this );
	CSGD_EventSystem::GetInstance()->RegisterClient( "MODIFY_HEALTH", this );
	CSGD_EventSystem::GetInstance()->RegisterClient( "STUN", this );

	SetFacing(1);

	m_nCurrency = 10;

	m_nHealthBarBackgroundImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/HUD/enemy_health_bar_background.png"));
	m_nHealthBarRedImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/HUD/enemy_health_bar_red.png"));
	m_nHealthBarGreenImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/HUD/enemy_health_bar_green.png"));
	m_nHealthBarYellowImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/HUD/enemy_health_bar_yellow.png"));
}

void CWight::SetArea()
{	
	if(m_nLevel == 1)
	{
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightForestWalkUp"));	//Walk Up		0
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightForestWalkDown"));	//Walk Down		1
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightForestWalkLeft"));	//Walk Left		2
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightForestWalkRight"));	//Walk Right	3
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightForestStandUp"));	//Stand Up		4
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightForestStandDown"));	//Stand Down	5
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightForestStandLeft"));	//Stand Left	6
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightForestStandRight"));	//Stand Right	7
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightForestAttackUp"));	//Shoot Up		8
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightForestAttackDown"));	//Shoot Down	9
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightForestAttackLeft"));	//Shoot Left	10
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightForestAttackRight"));	//Shoot Right	11
		SetCurrAnim(5);
	}
	else if(m_nLevel == 2)
	{
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightSnowyWalkUp"));	//Walk Up		0
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightSnowyWalkDown"));	//Walk Down		1
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightSnowyWalkLeft"));	//Walk Left		2
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightSnowyWalkRight"));	//Walk Right	3
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightSnowyStandUp"));	//Stand Up		4
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightSnowyStandDown"));	//Stand Down	5
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightSnowyStandLeft"));	//Stand Left	6
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightSnowyStandRight"));	//Stand Right	7
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightSnowyAttackUp"));	//Shoot Up		8
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightSnowyAttackDown"));	//Shoot Down	9
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightSnowyAttackLeft"));	//Shoot Left	10
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightSnowyAttackRight"));	//Shoot Right	11
		SetCurrAnim(5);
	}
	else if(m_nLevel == 3)
	{
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightMountainWalkUp"));	//Walk Up		0
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightMountainWalkDown"));	//Walk Down		1
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightMountainWalkLeft"));	//Walk Left		2
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightMountainWalkRight"));	//Walk Right	3
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightMountainStandUp"));	//Stand Up		4
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightMountainStandDown"));	//Stand Down	5
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightMountainStandLeft"));	//Stand Left	6
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightMountainStandRight"));	//Stand Right	7
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightMountainAttackUp"));	//Shoot Up		8
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightMountainAttackDown"));	//Shoot Down	9
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightMountainAttackLeft"));	//Shoot Left	10
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightMountainAttackRight"));	//Shoot Right	11
		SetCurrAnim(5);
	}
	else
	{
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightDesertWalkUp"));	//Walk Up		0
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightDesertWalkDown"));	//Walk Down		1
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightDesertWalkLeft"));	//Walk Left		2
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightDesertWalkRight"));	//Walk Right	3
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightDesertStandUp"));	//Stand Up		4
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightDesertStandDown"));	//Stand Down	5
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightDesertStandLeft"));	//Stand Left	6
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightDesertStandRight"));	//Stand Right	7
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightDesertAttackUp"));	//Shoot Up		8
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightDesertAttackDown"));	//Shoot Down	9
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightDesertAttackLeft"));	//Shoot Left	10
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("meleeWightDesertAttackRight"));	//Shoot Right	11
		SetCurrAnim(5);
	}
}


CWight::~CWight(void)
{
	delete BloodEffect;
	BloodEffect = nullptr;

	delete StunEffect;
	StunEffect = nullptr;

	if (m_pTarget != nullptr)
		m_pTarget->Release();

	CSGD_EventSystem::GetInstance()->UnregisterClientAll(this);

	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nHealthBarBackgroundImgID);
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nHealthBarYellowImgID);
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nHealthBarRedImgID);
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nHealthBarGreenImgID);
}


void CWight::Summon()
{

}

void CWight::IdleUpdate()
{

}

void CWight::OffensiveUpdate()
{

}

void CWight::RetreatingUpdate()
{

}

void CWight::SetTarget( CEntity* t )
{
	if (m_pTarget != nullptr)
		m_pTarget->Release();

	m_pTarget = t;
	m_pTarget->AddRef();
}

void CWight::Update( float deltaTime )
{
	UpdateEffects( deltaTime );

	tVector2D vToTarget = 
		{
			(float)(m_pTarget->GetPosX() - this->GetPosX()),
			(float)(m_pTarget->GetPosY() - this->GetPosY())
		};



	if (!Aggroed)
	{
		if (Vector2DLength(vToTarget) < 300.0f)
		{
			CAudioManager::GetInstance()->PlaySFX( SFX_WIGHTAGGRO );
			Aggroed = true;
		}
		else 
			return;
	}
	else if  (Vector2DLength(vToTarget) > 900.0f)
			Aggroed = false;


	if (LevelManager::GetInstance()->OnFire(this) && m_fTakeFireDamage <= 0)
	{
		ModifyHealth(-5);
		m_fTakeFireDamage = 1.0f;
	}

	m_fTakeFireDamage -= deltaTime;
	if (m_fTakeFireDamage <= 0.01)
		m_fTakeFireDamage = 0;

	m_fPathTimer += deltaTime;
	if (m_fPathTimer > 2.0f || Waypoints.empty())
	{
		POINT MyPosition;
		POINT TargetPosition;

		MyPosition.x = GetPosX();
		MyPosition.y = GetPosY();
		TargetPosition.x = m_pTarget->GetPosX() + 15;
		TargetPosition.y = m_pTarget->GetPosY() - 32;

		if (FindPath(MyPosition, TargetPosition))
		{
			Goal = Waypoints.top();
			Waypoints.pop();
			if (Waypoints.size() == 1)
			{
				Goal.x = m_pTarget->GetPosX()+ 15;;
				Goal.y = m_pTarget->GetPosY()- 32;
			}
			m_fPathTimer = 0.0f;
		}
		else 
			return;
	}

	if (m_fAttackCooldown > 0.0f)
	{
		m_fAttackCooldown -= deltaTime;

		if (m_fAttackCooldown <= 0.0f)
		{
			m_fAttackCooldown = 0.0f;
			m_bCanAttack = true;
		}
	}


	if (m_bCanAttack)
	{
		if (Vector2DLength(vToTarget) < m_nMeleeRange)
			m_bInRange = true;
		else
			m_bInRange = false;
	}

	if (m_bInRange && m_bCanAttack && !m_bAttacking && m_fAttackCooldown <= 0)
	{
		Attack();
	}
		
	if (m_bAttacking )
	{
		Attack();
		this->m_vAnimInfos[this->m_nCurrAnim]->Update(deltaTime);
		return;
	}

	if (!m_bInRange)
	{
		tVector2D vToWaypoint = 
		{
			(float)(Goal.x - GetPosX()),
			(float)(Goal.y - GetPosY())
		};

		if (vToWaypoint.fY > 0)
		{
			SetVelY((float)m_nSpeed);
			SetCurrAnim(1);
			SetFacing(1);
		}
		else if (vToWaypoint.fY < 0)
		{
			SetVelY((float)-m_nSpeed);
			SetCurrAnim(0);
			SetFacing(0);
		}

		if (abs(vToWaypoint.fY) < 10)
			SetVelY(0);

		if (vToWaypoint.fX > 0)
		{
			SetVelX((float)m_nSpeed);
			SetCurrAnim(3);
			SetFacing(3);
		}
		else if (vToWaypoint.fX < 0)
		{
			SetVelX((float)-m_nSpeed);
			SetCurrAnim(2);
			SetFacing(2);
		}

		if (abs(vToWaypoint.fX) < 10)
			SetVelX(0);

		CEntity::Update(deltaTime);

		vToWaypoint.fX  = (float)(Goal.x - GetPosX());
		vToWaypoint.fY	= (float)(Goal.y - GetPosY());

		if ( Vector2DLength( vToWaypoint ) < 15.0f )
		{
			if (!Waypoints.empty())
			{
				Goal = Waypoints.top();
				Waypoints.pop();
			}
		}
	}

}

void CWight::UpdateEffects( float deltaTime )
{
	if (StunEffect)
	{
		StunEffect->Update( deltaTime );
		if (StunEffect->IsDead())
		{
			delete StunEffect;
			StunEffect = nullptr;
		}
	}

	if (m_fStunTimer > 0.0f)
	{
		m_fStunTimer -= deltaTime;
		if (m_fStunTimer <= 0.0f)
		{
			StunEffect->StopEmitting();
			m_fStunTimer = 0.0f;
		}
		else
			return;
	}
	
	if (BloodEffect)
	{
		BloodEffect->SetXPosition( GetPosX()- rand() % 20 );
		BloodEffect->SetYPosition( GetPosY()- rand() % 20 );
		BloodEffect->Update(deltaTime);
		if (BloodEffect->IsDead())
		{
			delete BloodEffect;
			BloodEffect = nullptr;
		}
	}

	if (BloodTimer > 0.0f)
	{
		BloodTimer -= deltaTime;
		if (BloodTimer <= 0.0f)
		{
			BloodTimer = 0.0f;
			BloodEffect->StopEmitting();
		}
	}
}

void CWight::CalculateAttack( void )
{
		RECT rRect = CEntity::GetRect();
		RECT Cam;
		Camera::GetInstance()->CamRect(&Cam);

		m_rAttack.left = rRect.left - 50 - Cam.left;
		m_rAttack.right = rRect.right + 50 - Cam.left;
		m_rAttack.top = rRect.top - 50 - Cam.top;
		m_rAttack.bottom = rRect.bottom + 50 -Cam.top;
}

/*virtual*/ void CWight::HandleEvent( CEvent* pEvent )
{
	// What is the event id?
	if ( pEvent->GetEventID() == "TAKE_DAMAGE" )
	{
		// Is the event for us?
		if( pEvent->GetDestination() == this			// intended for us
			|| pEvent->GetDestination() == nullptr )	// broadcast for everyone
		{
			int* pDamage = reinterpret_cast< int* >( pEvent->GetParam() );
			int damage = *pDamage;
			ElementalDamage( 4, damage);
		}

	}

	// What is the event id?
	if ( pEvent->GetEventID() == "TAKE_ELECTRIC_DAMAGE" )
	{
		// Is the event for us?
		if( pEvent->GetDestination() == this			// intended for us
			|| pEvent->GetDestination() == nullptr )	// broadcast for everyone
		{
			int* pDamage = reinterpret_cast< int* >( pEvent->GetParam() );
			int damage = *pDamage;
			ElementalDamage( 3, damage);
		}

	}

	// What is the event id?
	if ( pEvent->GetEventID() == "TAKE_ICE_DAMAGE" )
	{
		// Is the event for us?
		if( pEvent->GetDestination() == this			// intended for us
			|| pEvent->GetDestination() == nullptr )	// broadcast for everyone
		{
			int* pDamage = reinterpret_cast< int* >( pEvent->GetParam() );
			int damage = *pDamage;
			ElementalDamage( 1, damage);
		}

	}

	// What is the event id?
	if ( pEvent->GetEventID() == "TAKE_FIRE_DAMAGE" )
	{
		// Is the event for us?
		if( pEvent->GetDestination() == this			// intended for us
			|| pEvent->GetDestination() == nullptr )	// broadcast for everyone
		{
			int* pDamage = reinterpret_cast< int* >( pEvent->GetParam() );
			int damage = *pDamage;
			ElementalDamage( 0, damage);
		}

	}

	// What is the event id?
	if ( pEvent->GetEventID() == "TAKE_EARTH_DAMAGE" )
	{
		// Is the event for us?
		if( pEvent->GetDestination() == this			// intended for us
			|| pEvent->GetDestination() == nullptr )	// broadcast for everyone
		{
			int* pDamage = reinterpret_cast< int* >( pEvent->GetParam() );
			int damage = *pDamage;
			ElementalDamage( 2, damage);
		}

	}

	if ( pEvent->GetEventID() == "MODIFY_HEALTH" )
	{
		if( pEvent->GetDestination() == this			// intended for us
			|| pEvent->GetDestination() == nullptr )	// broadcast for everyone
		{
			int* pHeal = reinterpret_cast< int* >( pEvent->GetParam() );
			ModifyHealth(*pHeal);
		}
	}

	if ( pEvent->GetEventID() == "STUN" )
	{
		if( pEvent->GetDestination() == this			// intended for us
			|| pEvent->GetDestination() == nullptr )	// broadcast for everyone
			Stun();
	}

}



void CWight::Render( void )
{
	if (!Aggroed)
		return;

	//if (m_bAttacking)
	//{
	//	CalculateAttack();
	//	CSGD_Direct3D::GetInstance()->DrawRect(m_rAttack,255,0,0);
	//}

	CEntity::Render();
	RECT temp;
	Camera::GetInstance()->CamRect(&temp);

	RECT Me, Way;
	Me.top = GetPosY() - temp.top;
	Me.left = GetPosX() - temp.left;
	Me.right = Me.left + 5;
	Me.bottom = Me.top + 5;

	Way.top = Goal.y - temp.top;
	Way.left = Goal.x - temp.left;
	Way.right = Way.left + 5;
	Way.bottom = Way.top + 5;

	//CSGD_Direct3D::GetInstance()->DrawRect(Me, 255,0,0);
	//CSGD_Direct3D::GetInstance()->DrawRect(Way, 255,255,0);

	RECT rRect2;
	rRect2.top = 0;
	rRect2.left = 0;
	rRect2.bottom = 12;
	rRect2.right = (LONG)(70 * (this->m_nHealth / (float)this->m_nMaxHealth));

	CSGD_TextureManager::GetInstance()->Draw(this->m_nHealthBarBackgroundImgID, GetPosX() - temp.left - 32, GetPosY() - temp.top - 80);

	if((this->m_nHealth / (float)this->m_nMaxHealth) >= 0.66)
	{
		CSGD_TextureManager::GetInstance()->Draw(this->m_nHealthBarGreenImgID, GetPosX() - temp.left - 32, GetPosY() - temp.top - 80, 1.0f, 1.0f, &rRect2);
	}
	else if((this->m_nHealth / (float)this->m_nMaxHealth) >= 0.33)
	{
		CSGD_TextureManager::GetInstance()->Draw(this->m_nHealthBarYellowImgID, GetPosX() - temp.left - 32, GetPosY() - temp.top - 80, 1.0f, 1.0f, &rRect2);
	}
	else
	{
		CSGD_TextureManager::GetInstance()->Draw(this->m_nHealthBarRedImgID, GetPosX() - temp.left - 32, GetPosY() - temp.top - 80,1.0f, 1.0f, &rRect2);
	}

	if (BloodEffect)
		BloodEffect->Render();
	if (StunEffect)
		StunEffect->Render();
}

void CWight::ElementalDamage( int type, int amount )
{
	float modifier = 0.0f;
	switch (type)
	{
	case 0:
		modifier = m_fFireResist;
		break;
	case 1:
		modifier = m_fIceResist;
		break;
	case 2:
		modifier = m_fEarthResist;
		break;
	case 3:
		modifier = m_fLightResist;
		break;
	case 4:
		modifier = m_fPhysicalResist;
		break;
	}

	TakeDamage( (int)(amount * modifier) );
}


bool CWight::CheckCollision( IEntity* pOther )
{

	return false;

}

void CWight::TakeDamage( int Damage )
{
	ModifyHealth( Damage );

	if (Damage < 0)
	{
		if (BloodEffect)
			delete BloodEffect;

		BloodEffect = new CParticleEffect;
		CParticleManager::GetInstance()->GetEffect(*BloodEffect,FX_ZOMBIEBLOOD);
		BloodEffect->ToggleCamera();
		BloodEffect->SetXPosition( GetPosX() - rand() % 20 );
		BloodEffect->SetYPosition( GetPosY() - rand() % 20);
		BloodEffect->Initialize();
		BloodTimer = 0.25f;
	}

	if(GetHealth() < 50)
		CSGD_EventSystem::GetInstance()->SendEventNow(
		"HEAL_MONSTER", NULL, NULL, this );

	if (GetHealth() <= 0)
	{
		if (m_bDead)
			return;

		CAudioManager::GetInstance()->PlaySFX( SFX_WIGHTDIE );
		CSGD_EventSystem::GetInstance()->SendEventNow(
					"GAIN_EXPERIENCE", &m_nExperience, m_pTarget, this );

		AddCurrencyMessage* Msg = new AddCurrencyMessage ( m_nCurrency );
		CMessageSystem::GetInstace()->SendMsg(Msg);
		Msg = nullptr;

		CItemManager::GetInstance()->DropItem(2, GetPosX(), GetPosY());
		m_bDead = true;
	}

}

void CWight::Stun( void )
{
	m_fStunTimer = 3.0f;
	if (StunEffect)
		delete StunEffect;

	StunEffect = new CParticleEffect;
	CParticleManager::GetInstance()->GetEffect(*StunEffect, FX_LIGHTNINGDASH);
	StunEffect->ToggleCamera();
	StunEffect->SetXPosition(GetPosX() - 10);
	StunEffect->SetYPosition(GetPosY() - 40);
	StunEffect->Initialize();
}

void CWight::Attack()
{
	if (!m_bAttacking)
	{
		m_bAttacking = true;	
		switch(this->m_nFacing)
		{
		case 0:
			{
				this->SetCurrAnim(8);
			}							
			break;						
		case 1:						
			{							
				this->SetCurrAnim(9);
			}							
			break;						
		case 2:						
			{							
				this->SetCurrAnim(10);
			}						
			break;					
		case 3:					
			{						
				this->SetCurrAnim(11);
			}
			break;
		}

		this->GetAnimationInformation(this->GetCurrAnim())->Reset();
		m_bHasAlreadyHit = false;

	}

	if (!m_bHasAlreadyHit)
	{
		tVector2D vToTarget = 
		{
			(float)(m_pTarget->GetPosX() - this->GetPosX()),
			(float)(m_pTarget->GetPosY() - this->GetPosY())
		};

		if (Vector2DLength(vToTarget) <= m_nMeleeRange+15)
		{
			CPlayer* Temp = dynamic_cast<CPlayer*> (m_pTarget);

			m_bHasAlreadyHit = true;
			if (!Temp->GetEarthWall())
			CSGD_EventSystem::GetInstance()->SendEventNow(
				"TAKE_DAMAGE", &m_nDamage, m_pTarget, this );
		}
	}

	if (this->GetAnimationInformation(GetCurrAnim())->IsOver())
	{
		switch(this->m_nFacing)
		{
		case 0:
			{
				this->SetCurrAnim(4);
			}							
			break;						
		case 1:						
			{							
				this->SetCurrAnim(5);
			}							
			break;						
		case 2:						
			{							
				this->SetCurrAnim(6);
			}						
			break;					
		case 3:					
			{						
				this->SetCurrAnim(7);
			}
			break;
		}

		m_bAttacking = false;
		m_bCanAttack = false;
		m_fAttackCooldown = 2.5f;
	}

}

void CWight::SetLevel( int L )
{
	m_nLevel = L;
	m_fFireResist	= 1.0f;
	m_fIceResist	= 1.0f;
	m_fLightResist	= 1.0f;
	m_fEarthResist	= 1.0f;
	m_fPhysicalResist = 1.0f;

	switch (m_nLevel)
	{
	case 1:
		m_nMaxHealth = m_nHealth = 35;
		m_nExperience = 35;
		m_nDamage = -6;
		m_nSpeed = 110;
		break;
	case 2:
		m_nMaxHealth = m_nHealth = 65;
		m_nExperience = 350;
		m_nDamage = -9;
		m_nSpeed = 130;
		m_fFireResist	= 1.5f;
		m_fPhysicalResist = 0.5f;
		break;
	case 3:
		m_nMaxHealth = m_nHealth = 95;
		m_nExperience = 3500;
		m_nDamage = -20;
		m_nSpeed = 140;
		m_fFireResist	= 0.3f;
		m_fEarthResist	= 1.5f;
		break;
	case 4:
		m_nMaxHealth = m_nHealth = 150;
		m_nExperience = 35000;
		m_nDamage = -32;
		m_nSpeed = 150;
		m_fIceResist	= 0.3f;
		m_fEarthResist	= 0.3f;
		m_fLightResist	= 1.5f;
		break;
	}
}
