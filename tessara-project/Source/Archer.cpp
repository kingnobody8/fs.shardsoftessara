/*
	Author: Jorge Juaneda
	File: Archer.cpp
*/

#include "Archer.h"
#include "PlayState.h"
#include "AnimationManager.h"
#include "..\\SGD Wrappers\CSGD_EventSystem.h"
#include "AddCurrencyMessage.h"
#include "MessageSystem.h"
#include "ParticleEffect.h"
#include "ParticleManager.h"
#include "CreateProjectileMsg.h"
#include "MessageSystem.h"
#include "LevelManager.h"
#include "AudioManager.h"
#include "Camera.h"
#include "PathFinder.h"

CArcher::CArcher(void)
{
	Aggroed = false;
	m_bAttacking = false;
	m_bInRange = false;
	m_bCanAttack = false;
	m_nRetreat = false;
	m_bShootX = false;
	m_bShootY = false;
	m_bDead = false;

	StunEffect = nullptr;
	m_fStunTimer = 0.0f;
	m_fAttackTimer = 2.5f;
	m_fAttackCooldown = 0.0f;
	m_fRetreatTimer = 3.0f;
	m_fRetreatCooldown = 0.0f;
	m_fTakeFireDamage = 0.0f;
	m_fPathTimer = 0.0f;

	m_nExperience = 120;
	m_nCurrency  = 10;
	SetHealth(300);
	m_nRange = 200;
	m_nDistance = 0;
	m_nElement = 3;

	m_nSpeed = 100;

	m_pTarget = nullptr;
	m_bAttacking = false;

	SetType( ENT_ARCHER );
	
	CSGD_EventSystem::GetInstance()->RegisterClient( "TAKE_DAMAGE", this );
	CSGD_EventSystem::GetInstance()->RegisterClient( "TAKE_FIRE_DAMAGE", this );
	CSGD_EventSystem::GetInstance()->RegisterClient( "TAKE_ICE_DAMAGE", this );
	CSGD_EventSystem::GetInstance()->RegisterClient( "TAKE_ELECTRIC_DAMAGE", this );
	CSGD_EventSystem::GetInstance()->RegisterClient( "TAKE_EARTH_DAMAGE", this );
	CSGD_EventSystem::GetInstance()->RegisterClient( "MODIFY_HEALTH", this );
	CSGD_EventSystem::GetInstance()->RegisterClient( "STUN", this );

	m_nHealthBarBackgroundImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/HUD/enemy_health_bar_background.png"));
	m_nHealthBarRedImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/HUD/enemy_health_bar_red.png"));
	m_nHealthBarGreenImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/HUD/enemy_health_bar_green.png"));
	m_nHealthBarYellowImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/HUD/enemy_health_bar_yellow.png"));	
}

void CArcher::SetArea()
{
	if(m_nLevel == 1)
	{
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherForestWalkUp"));	//Walk Up		0
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherForestWalkDown"));	//Walk Down		1
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherForestWalkLeft"));	//Walk Left		2
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherForestWalkRight"));	//Walk Right	3
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherForestStandUp"));	//Stand Up		4
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherForestStandDown"));	//Stand Down	5
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherForestStandLeft"));	//Stand Left	6
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherForestStandRight"));	//Stand Right	7
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherForestShootUp"));	//Shoot Up		8
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherForestShootDown"));	//Shoot Down	9
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherForestShootLeft"));	//Shoot Left	10
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherForestShootRight"));	//Shoot Right	11
		SetCurrAnim(0);
	}
	else if(m_nLevel == 2)
	{
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherSnowWalkUp"));	//Walk Up		0
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherSnowWalkDown"));	//Walk Down		1
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherSnowWalkLeft"));	//Walk Left		2
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherSnowWalkRight"));	//Walk Right	3
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherSnowStandUp"));	//Stand Up		4
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherSnowStandDown"));	//Stand Down	5
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherSnowStandLeft"));	//Stand Left	6
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherSnowStandRight"));	//Stand Right	7
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherSnowShootUp"));	//Shoot Up		8
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherSnowShootDown"));	//Shoot Down	9
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherSnowShootLeft"));	//Shoot Left	10
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherSnowShootRight"));	//Shoot Right	11
		SetCurrAnim(0);
	}
	else if(m_nLevel == 3)
	{
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherMountainWalkUp"));	//Walk Up		0
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherMountainWalkDown"));	//Walk Down		1
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherMountainWalkLeft"));	//Walk Left		2
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherMountainWalkRight"));	//Walk Right	3
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherMountainStandUp"));	//Stand Up		4
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherMountainStandDown"));	//Stand Down	5
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherMountainStandLeft"));	//Stand Left	6
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherMountainStandRight"));	//Stand Right	7
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherMountainShootUp"));	//Shoot Up		8
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherMountainShootDown"));	//Shoot Down	9
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherMountainShootLeft"));	//Shoot Left	10
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherMountainShootRight"));	//Shoot Right	11
		SetCurrAnim(0);
	}
	else
	{
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherDesertWalkUp"));	//Walk Up		0
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherDesertWalkDown"));	//Walk Down		1
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherDesertWalkLeft"));	//Walk Left		2
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherDesertWalkRight"));	//Walk Right	3
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherDesertStandUp"));	//Stand Up		4
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherDesertStandDown"));	//Stand Down	5
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherDesertStandLeft"));	//Stand Left	6
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherDesertStandRight"));	//Stand Right	7
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherDesertShootUp"));	//Shoot Up		8
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherDesertShootDown"));	//Shoot Down	9
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherDesertShootLeft"));	//Shoot Left	10
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wightArcherDesertShootRight"));	//Shoot Right	11
		SetCurrAnim(0);
	}
}

CArcher::~CArcher(void)
{
	if (StunEffect)
	delete StunEffect;
	
	StunEffect = nullptr;

	if (m_pTarget != nullptr)
		m_pTarget->Release();

	CSGD_EventSystem::GetInstance()->UnregisterClientAll(this);
}

void CArcher::Summon()
{


}

void CArcher::Idle()
{

}

void CArcher::Offensive()
{


}

void CArcher::Retreat()
{
	int nDifX = m_pTarget->GetPosX() - this->GetPosX();
	int nDifY = m_pTarget->GetPosY() - this->GetPosY();

	if ( abs(nDifX) < abs(nDifY))
	{
		SetVelY(0);
		if (m_pTarget->GetPosX() > this->GetPosX())
		{
			SetCurrAnim(2);
			SetVelX((float)-m_nSpeed);
		}
		else if (m_pTarget->GetPosX() < this->GetPosX())
		{
			SetCurrAnim(3);
			SetVelX((float)m_nSpeed);
		}
	}
	else
	{
		SetVelX(0);
		if (m_pTarget->GetPosY() > this->GetPosY())
		{
			SetCurrAnim(0);
			SetVelY((float)-m_nSpeed);
		}
		else if (m_pTarget->GetPosY() < this->GetPosY())
		{
			SetCurrAnim(1);
			SetVelY((float)m_nSpeed);
		}
	}

}

void CArcher::Attack()
{
	if ( !m_bAttacking)
	{
		m_fAttackCooldown = m_fAttackTimer;
		m_bAttacking = true;

		int type = 0;
		
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
	}

	if (this->GetAnimationInformation(GetCurrAnim())->IsOver())
	{
		CreateProjectileMsg* msg = nullptr;
		if (m_bShootX)
		{
			if (GetPosX() > m_pTarget->GetPosX())
				msg = new CreateProjectileMsg(200, 2, 4, GetPosX() - GetWidth() - 50, GetPosY(), false);
			else
				msg = new CreateProjectileMsg(200, 3, 4, GetPosX() + GetWidth() + 50, GetPosY(), false);

		}
		else if	(m_bShootY)
		{
			if (GetPosY() > m_pTarget->GetPosY())
				msg = new CreateProjectileMsg(200, 0, 4, GetPosX(), GetPosY() - GetHeight() - 50, false);
			else
				msg = new CreateProjectileMsg(200, 1, 4, GetPosX(), GetPosY() + 50, false);
		}

		if (msg != nullptr)
			CMessageSystem::GetInstace()->SendMsg(msg);
		msg = nullptr;
		
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
	}
}



void CArcher::Update( float deltaTime )
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


	if (Vector2DLength(vToTarget) <= 200)
		m_bInRange = true;
	else 
		m_bInRange = false;

	m_nRetreat = false;

	m_fTakeFireDamage -= deltaTime;
	if (m_fTakeFireDamage <= 0.01)
		m_fTakeFireDamage = 0;
	if (LevelManager::GetInstance()->OnFire(this) && m_fTakeFireDamage <= 0)
	{
		ModifyHealth(-5);
		m_fTakeFireDamage = 1.0f;
	}

	SetOnIce( LevelManager::GetInstance()->OnIce(this));
	if (GetOnIce())
		m_nSpeed = 63;
	else
		m_nSpeed = 100;


	if (((unsigned)GetPosX() > m_pTarget->GetPosX() - GetWidth()/2 && (unsigned)GetPosX() < m_pTarget->GetPosX() + GetWidth()/2) )
	{
		m_bShootX = false;
		m_bShootY = true;
		m_bCanAttack = true;
	}
	else if ((unsigned)GetPosY() > m_pTarget->GetPosY() - GetHeight() && GetPosY() < m_pTarget->GetPosY())
	{
		m_bShootY = false;
		m_bShootX = true;
		m_bCanAttack = true;
	}
	else
	{
		m_bShootY = false;
		m_bShootX = false;
		m_bCanAttack = false;
	}


	m_fAttackCooldown -= deltaTime;
	if (m_fAttackCooldown <= 0.01)
		m_fAttackCooldown = 0;

	m_fRetreatCooldown -= deltaTime;
	if (m_fRetreatCooldown <= 0.01)
		m_fRetreatCooldown = 0;


	if (m_bInRange && m_bCanAttack && !m_bAttacking && m_fAttackCooldown <= 0)
	{
		Attack();
	}


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

	if (!m_bInRange && !m_bAttacking)
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

	if (m_bAttacking)
	{
		SetVelX(0);
		SetVelY(0);
		Attack();
	}

	CEntity::Update(deltaTime);
}


void CArcher::Render( void )
{
	if (!Aggroed)
		return;

	CEntity::Render();
	RECT temp;

	Camera::GetInstance()->CamRect(&temp);
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

	if (StunEffect)
		StunEffect->Render();
}


bool CArcher::CheckCollision( IEntity* pOther)
{
	return false;
}


void CArcher::SetTarget( CEntity* t )
{
	if (m_pTarget != nullptr)
		m_pTarget->Release();

	//add reference to target
	t->AddRef();
	m_pTarget = t;
}


void CArcher::HandleEvent(CEvent* pEvent)
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
			int* pHealAmount = reinterpret_cast< int* >( pEvent->GetParam() );
			ModifyHealth(*pHealAmount);
		}
	}

	if ( pEvent->GetEventID() == "STUN" )
	{
		if( pEvent->GetDestination() == this			// intended for us
			|| pEvent->GetDestination() == nullptr )	// broadcast for everyone
		Stun();
	}
}

void CArcher::ElementalDamage( int type, int amount )
{
	float modifier = 0.0f;
	switch (type)
	{
	case 0:
		modifier = m_fFireResist;
		break;
	case 1:
		modifier = m_fIceResist;
		if (m_fIceResist > 1.0f && (rand() % 100) < 99 )
			Freeze();
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

void CArcher::TakeDamage( int Damage )
{
	ModifyHealth( Damage );

	if(GetHealth() < 100)
		CSGD_EventSystem::GetInstance()->SendEventNow(
		"HEAL_MONSTER", NULL, NULL, this );

	if (m_nHealth <= 0)
	{
		if (m_bDead)
			return;
		CSGD_EventSystem::GetInstance()->SendEventNow(
					"GAIN_EXPERIENCE", &m_nExperience, m_pTarget, this );

		AddCurrencyMessage* Msg = new AddCurrencyMessage ( m_nCurrency );
		CMessageSystem::GetInstace()->SendMsg(Msg);
		Msg = nullptr;
		m_bDead = true;
	}

}

void CArcher::Stun( void )
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

void CArcher::Freeze( void )
{
	m_fStunTimer = 3.0f;
	if (StunEffect)
		delete StunEffect;

	StunEffect = new CParticleEffect;
	CParticleManager::GetInstance()->GetEffect(*StunEffect, FX_ICESLASH);
	StunEffect->ToggleCamera();
	StunEffect->SetXPosition(GetPosX() - 10);
	StunEffect->SetYPosition(GetPosY() - 40);
	StunEffect->Initialize();
}


void CArcher::SetLevel( int L )
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
		m_nExperience = 30;
		m_nDamage = -9;
		m_nSpeed = 100;
		break;
	case 2:
		m_nMaxHealth = m_nHealth = 65;
		m_nExperience = 300;
		m_nDamage = -12;
		m_nSpeed = 110;
		m_fPhysicalResist = 0.3f;
		m_fIceResist	= 1.5f;
		break;
	case 3:
		m_nMaxHealth = m_nHealth = 95;
		m_nExperience = 3000;
		m_nDamage = -22;
		m_nSpeed = 130;
		m_fIceResist	= 0.3f;
		m_fEarthResist	= 1.5f;
		break;
	case 4:
		m_nMaxHealth = m_nHealth = 150;
		m_nExperience = 30000;
		m_nDamage = -36;
		m_nSpeed = 150;
		m_fIceResist	= 1.5f;
		m_fEarthResist	= 0.3f;
		m_fFireResist	= 0.3f;
		break;
	}
}