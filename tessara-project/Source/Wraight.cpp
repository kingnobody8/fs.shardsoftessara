/*
	Author: Jorge Juaneda
	File: Wraight.cpp
*/


#include "Wraight.h"
#include "LevelManager.h"
#include "..\SGD Wrappers\CEvent.h"
#include "AddCurrencyMessage.h"
#include "..\SGD Wrappers\CSGD_EventSystem.h"
#include "MessageSystem.h"
#include "AnimationManager.h"
#include "DeleteProjectileMessage.h"
#include "CreateProjectileMsg.h"
#include "DeleteEntityMessage.h"
#include "EntityManager.h"
#include "ItemManager.h"
#include "Camera.h"
#include "Player.h"

CWraight::CWraight(void)
{
	SetType(ENT_WRAITH);

	m_bAttacking = false;
	m_bInRange	 = false;
	m_bCanAttack = false;
	m_fAttackTimer = false;
	m_bHealing = false;
	m_bHeal = false;
	m_bHealSelf = false;
	m_bDead = false;

	m_nExperience = 100;
	m_nCurrency = 20;
	m_nHealAmount = 30;
	m_nRange = 150;
	m_nMaxHealth = 300;
	SetHealth(300);
	m_nElement = 2;
	m_fStunTimer = 0.0f;
	StunEffect = nullptr;
	m_nImmuneElement = -1;

	m_fHealCooldown	 = 0.0f;
	m_fPathTimer	 = 0.0f;
	m_fAttackTimer	 = 3.0f;
	m_fAttackCooldown = 0.0f;
	m_fHealTimer	 = 6.0f;
	m_fTakeFireDamage = 0.0f;

	m_pTarget = nullptr;

	CSGD_EventSystem::GetInstance()->RegisterClient( "TAKE_DAMAGE", this );
	CSGD_EventSystem::GetInstance()->RegisterClient( "HEAL_MONSTER", this );
	CSGD_EventSystem::GetInstance()->RegisterClient( "TAKE_FIRE_DAMAGE", this );
	CSGD_EventSystem::GetInstance()->RegisterClient( "TAKE_ICE_DAMAGE", this );
	CSGD_EventSystem::GetInstance()->RegisterClient( "TAKE_ELECTRIC_DAMAGE", this );
	CSGD_EventSystem::GetInstance()->RegisterClient( "TAKE_EARTH_DAMAGE", this );
	CSGD_EventSystem::GetInstance()->RegisterClient( "MONSTER_DELETED", this );
	
	m_nHealthBarBackgroundImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/HUD/enemy_health_bar_background.png"));
	m_nHealthBarRedImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/HUD/enemy_health_bar_red.png"));
	m_nHealthBarGreenImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/HUD/enemy_health_bar_green.png"));
	m_nHealthBarYellowImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/HUD/enemy_health_bar_yellow.png"));
}

void CWraight::SetArea()
{
	if(m_nLevel == 1)
	{
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithForestWalkUp"));	//Walk Up		0
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithForestWalkDown"));	//Walk Down		1
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithForestWalkLeft"));	//Walk Left		2
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithForestWalkRight"));	//Walk Right	3
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithForestStandUp"));	//Stand Up		4
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithForestStandDown"));	//Stand Down	5
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithForestStandLeft"));	//Stand Left	6
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithForestStandRight"));	//Stand Right	7
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithForestCastUp"));	//Shoot Up		8
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithForestCastDown"));	//Shoot Down	9
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithForestCastLeft"));	//Shoot Left	10
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithForestCastRight"));	//Shoot Right	11
		SetCurrAnim(5);
	}
	else if(m_nLevel == 2)
	{
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithSnowyWalkUp"));	//Walk Up		0
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithSnowyWalkDown"));	//Walk Down		1
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithSnowyWalkLeft"));	//Walk Left		2
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithSnowyWalkRight"));	//Walk Right	3
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithSnowyStandUp"));	//Stand Up		4
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithSnowyStandDown"));	//Stand Down	5
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithSnowyStandLeft"));	//Stand Left	6
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithSnowyStandRight"));	//Stand Right	7
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithSnowyCastUp"));	//Shoot Up		8
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithSnowyCastDown"));	//Shoot Down	9
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithSnowyCastLeft"));	//Shoot Left	10
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithSnowyCastRight"));	//Shoot Right	11
		SetCurrAnim(5);
	}
	else if(m_nLevel == 3)
	{
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithMountainWalkUp"));	//Walk Up		0
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithMountainWalkDown"));	//Walk Down		1
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithMountainWalkLeft"));	//Walk Left		2
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithMountainWalkRight"));	//Walk Right	3
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithMountainStandUp"));	//Stand Up		4
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithMountainStandDown"));	//Stand Down	5
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithMountainStandLeft"));	//Stand Left	6
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithMountainStandRight"));	//Stand Right	7
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithMountainCastUp"));	//Shoot Up		8
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithMountainCastDown"));	//Shoot Down	9
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithMountainCastLeft"));	//Shoot Left	10
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithMountainCastRight"));	//Shoot Right	11
		SetCurrAnim(5);
	}
	else
	{
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithDesertWalkUp"));	//Walk Up		0
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithDesertWalkDown"));	//Walk Down		1
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithDesertWalkLeft"));	//Walk Left		2
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithDesertWalkRight"));	//Walk Right	3
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithDesertStandUp"));	//Stand Up		4
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithDesertStandDown"));	//Stand Down	5
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithDesertStandLeft"));	//Stand Left	6
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithDesertStandRight"));	//Stand Right	7
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithDesertCastUp"));	//Shoot Up		8
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithDesertCastDown"));	//Shoot Down	9
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithDesertCastLeft"));	//Shoot Left	10
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("wraithDesertCastRight"));	//Shoot Right	11
		SetCurrAnim(5);
	}
}


CWraight::~CWraight(void)
{
	CSGD_EventSystem::GetInstance()->UnregisterClientAll(this);
	if (StunEffect)
	{
		delete StunEffect;
		StunEffect = nullptr;
	}
}


void CWraight::Update( float deltaTime)
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

	if (LevelManager::GetInstance()->OnFire(this) && m_fTakeFireDamage <= 0)
	{
		ModifyHealth(-5);
		m_fTakeFireDamage = 1.0f;
	}

	if (m_pTarget == nullptr || !CEntityManager::GetInstance()->IsEntityAlive(m_pTarget))
	{
		m_pTarget = m_pPlayer;
		m_bHeal = false;
	}
	else
	{
		tVector2D vToTarget = 
		{
			(float)(m_pTarget->GetPosX() - this->GetPosX()),
			(float)(m_pTarget->GetPosY() - this->GetPosY())
		};

		if (!Aggroed)
		{
			if (Vector2DLength(vToTarget) < 300.0f)
				Aggroed = true;
			else 
				return;
		}
		else if  (Vector2DLength(vToTarget) > 900.0f)
			Aggroed = false;


		SetOnIce( LevelManager::GetInstance()->OnIce(this));
		if (GetOnIce())
			m_nSpeed = 65;
		else
			m_nSpeed = 150;

		//Cooldowns
		m_fAttackCooldown -= deltaTime;
		if (m_fAttackCooldown <= 0.01)
			m_fAttackCooldown = 0;

		m_fHealCooldown -= deltaTime;
		if (m_fHealCooldown <= 0.01)
			m_fHealCooldown = 0;

		m_fTakeFireDamage -= deltaTime;
		if (m_fTakeFireDamage <= 0.01)
			m_fTakeFireDamage = 0;

		if (GetHealth() < 100 &&  m_fHealCooldown <= 0 && !m_bHealSelf && !m_bHeal)
		{
			m_fHealCooldown = m_fAttackTimer;
			HealSelf();
			m_bInRange = true;
		}
		else if (m_bHeal && m_pTarget->GetType() != ENT_PLAYER)
		{
			if (Vector2DLength(vToTarget) < m_nRange)
				m_bInRange = true;
			else
				m_bInRange = false;

			if ( m_fHealCooldown <= 0 && !m_bHealing)
			{
				Heal();
			}
		}
		else 
		{
			if (Vector2DLength(vToTarget) < m_nRange + 100)
				m_bInRange = true;
			else
				m_bInRange = false;

			if (((unsigned)GetPosX() > m_pTarget->GetPosX() - GetWidth() && (unsigned)GetPosX() < m_pTarget->GetPosX() + GetWidth()) )
			{
				m_bShootX = false;
				m_bShootY = true;
				m_bCanAttack = true;
			}
			else if ((unsigned)GetPosY() > m_pTarget->GetPosY() - GetHeight() - 30 && GetPosY() < m_pTarget->GetPosY() + 30)
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

			if (m_bInRange && m_bCanAttack && !m_bAttacking && m_fAttackCooldown <= 0)
			{
				Attack();
			}


		}


		m_fPathTimer += deltaTime;
		if (m_fPathTimer > 2.0f || Waypoints.empty())
		{
			POINT MyPosition;
			POINT TargetPosition;

			MyPosition.x = GetPosX();
			MyPosition.y = GetPosY();
			TargetPosition.x = m_pTarget->GetPosX();
			TargetPosition.y = m_pTarget->GetPosY() - 32;

			if (FindPath(MyPosition, TargetPosition))
			{
				Goal = Waypoints.top();
				Waypoints.pop();
				m_fPathTimer = 0.0f;
			}
		}

		/////////////////////////////////////////////////////////////////
		// BUG FIX
		// Reference Bug # BB-056
		// BUG FIX START
		/////////////////////////////////////////////////////////////////
		if (!m_bAttacking && !m_bHeal)
		{
		///////////////////////////////////////////////////////////////////////////
		// BUG FIX END  Reference # BB-056
		//////////////////////////////////////////////////////////////////////////

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
					SetFacing(1);			
					SetCurrAnim(1);
				}
				else if (vToWaypoint.fY < 0)
				{
					SetVelY((float)-m_nSpeed);
					SetFacing(0);
					SetCurrAnim(0);
				}

				if (abs(vToWaypoint.fY) < 15)
				{
					SetVelY(0);
				}

				if (vToWaypoint.fX > 0)
				{
					SetVelX((float)m_nSpeed);
					SetFacing(3);
					SetCurrAnim(3);
				}
				else if (vToWaypoint.fX < 0)
				{
					SetVelX((float)-m_nSpeed);
					SetFacing(2);
					SetCurrAnim(2);
				}

				if (abs(vToWaypoint.fX) < 15)
				{
					SetVelX(0);
				}

				vToWaypoint.fX  = (float)(Goal.x - GetPosX());
				vToWaypoint.fY	= (float)(Goal.y - GetPosY());

				CEntity::Update(deltaTime);

				if ( Vector2DLength( vToWaypoint ) < 50.0f )
				{
					if (!Waypoints.empty())
					{
						Goal = Waypoints.top();
						Waypoints.pop();
					}
				}
			} 
		}

		if (m_bHealing)
		{
			SetVelX(0);
			SetVelY(0);
			Heal();
			this->m_vAnimInfos[this->m_nCurrAnim]->Update(deltaTime);
		}
		else if(m_bHealSelf)
		{
			SetVelX(0);
			SetVelY(0);
			HealSelf();

			this->m_vAnimInfos[this->m_nCurrAnim]->Update(deltaTime);

		}
		else if (m_bAttacking)
		{
			SetVelX(0);
			SetVelY(0);
			Attack();
			this->m_vAnimInfos[this->m_nCurrAnim]->Update(deltaTime);
		}

	}


}


bool CWraight::CheckCollision( IEntity* pOther)
{
	if (AnimationManager::GetInstance()->GetAnimation(this->GetAnimationInformation(this->GetCurrAnim())->GetAnimID()).GetFrame(this->GetAnimationInformation(this->GetCurrAnim())->GetCurFrame()).HasActiveRect())
	{

		Frame tempFrame = AnimationManager::GetInstance()->GetAnimation(this->GetAnimationInformation(this->GetCurrAnim())->GetAnimID()).GetFrame(this->GetAnimationInformation(this->GetCurrAnim())->GetCurFrame());
		POINT pAnchor = tempFrame.GetAnchorPoint();
		RECT rActiv  = *tempFrame.GetActiveRect();
		RECT rRect, rOver;

		RECT rOther = pOther->GetRect();

		int nHeight = rActiv.bottom - rActiv.top;
		int nWidth = rActiv.right - rActiv.left;
		int nX = (int)GetPosX();
		int nY = (int)GetPosY();

		rRect.bottom = nY + (rActiv.bottom - pAnchor.y);
		rRect.top = rRect.bottom - nHeight;
		rRect.right = nX + (rActiv.right - pAnchor.x);
		rRect.left = rRect.right - nWidth; 


		if( IntersectRect( &rOver, &rRect, &rOther ) == TRUE )
		{
			if (m_pTarget->GetType() == ENT_PLAYER)
			{
				CPlayer* Temp = dynamic_cast<CPlayer*> (m_pTarget);

				if (Temp->GetEarthWall())
				{
					if (IntersectRect(&rOver, &Temp->GetActiveRec(), &rRect))
						return true;
				}

				CSGD_EventSystem::GetInstance()->SendEventNow(
					"TAKE_DAMAGE", &m_nDamage, m_pTarget, this );
			}
		}


	}

	return false;
}

void CWraight::SetTarget( CEntity* t )
{
	m_pTarget = t;
}

void CWraight::HandleEvent(CEvent* pEvent)
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

	if (pEvent->GetEventID() == "HEAL_MONSTER")
	{
		if (!m_bHealing)
		{
			CMonster* t = reinterpret_cast< CMonster* >( pEvent->GetSender() );
			if (t != nullptr)
			{
				SetTarget(t);
				m_bHeal = true;
			}
		}
	}

	if (pEvent->GetEventID() == "MONSTER_DELETED")
	{
		CMonster* t = reinterpret_cast< CMonster* >( pEvent->GetSender() );
		if (m_pTarget == t)
		{
			m_pTarget = nullptr;
			m_bHeal = false;
		}
	}

}

void CWraight::ElementalDamage( int type, int amount )
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

	if (m_nLevel == 4)
	{
		if (type != m_nImmuneElement)
		{
			TakeDamage( (int)(amount * modifier) );
			m_nImmuneElement = type;
		}
	}
	else
		TakeDamage( (int)(amount * modifier) );
}

void CWraight::Idle()
{

}

void CWraight::Offensive()
{


}

void CWraight::Retreat()
{


}

void CWraight::Stun( void )
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

void CWraight::Freeze( void )
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

void CWraight::Attack()
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
		m_fAttackCooldown = m_fAttackTimer;
	}

	if (this->GetAnimationInformation(GetCurrAnim())->IsOver() || m_fAttackCooldown <= 0)
	{
		m_bAttacking = false;
		
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

		m_fAttackCooldown = m_fAttackTimer;

		CreateProjectileMsg* msg = nullptr;
		int type = 0;

		if (m_bShootX)
		{
			if (GetPosX() > m_pTarget->GetPosX())
				msg = new CreateProjectileMsg(300, 2, m_nElement, GetPosX() - GetWidth() - 20, GetPosY(), false);
			else
				msg = new CreateProjectileMsg(300, 3, m_nElement, GetPosX() + GetWidth() + 20, GetPosY(), false);

		}
		else if	(m_bShootY)
		{
			if (GetPosY() > m_pTarget->GetPosY())
				msg = new CreateProjectileMsg(300, 0, m_nElement, GetPosX(), GetPosY() - GetHeight() - 20, false);
			else
				msg = new CreateProjectileMsg(300, 1, m_nElement, GetPosX(), GetPosY() + 20, false);
		}

		if (msg != nullptr)
			CMessageSystem::GetInstace()->SendMsg(msg);
		msg = nullptr;
	}
}


void CWraight::Heal()
{
	if (!m_bHealing)
	{
		m_bHealing = true;
		
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
		m_bHealing = false;
		m_fHealCooldown = m_fHealTimer;

		if (CEntityManager::GetInstance()->IsEntityAlive(m_pTarget) && m_pTarget->GetType() != ENT_PLAYER)
			CSGD_EventSystem::GetInstance()->SendEventNow(
			"MODIFY_HEALTH", &m_nHealAmount, m_pTarget, this );

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

		m_pTarget = nullptr;
	}
}

void CWraight::HealSelf()
{
	if (!m_bHealSelf)
	{
		m_bHealSelf = true;
		
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
		m_bHealSelf = false;
		m_fHealCooldown = m_fHealTimer;
		ModifyHealth(m_nHealAmount);
		if (m_nHealth > m_nMaxHealth)
			m_nHealth = m_nMaxHealth;

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
	}

}


void CWraight::TakeDamage( int Damage )
{
	ModifyHealth( Damage );
	if (m_nHealth <= 0)
	{
		if (m_bDead)
			return;
		CSGD_EventSystem::GetInstance()->SendEventNow(
					"GAIN_EXPERIENCE", &m_nExperience, m_pTarget, this );

		AddCurrencyMessage* Msg = new AddCurrencyMessage ( m_nCurrency );
		CMessageSystem::GetInstace()->SendMsg(Msg);
		Msg = nullptr;

		CItemManager::GetInstance()->DropItem(2, GetPosX(), GetPosY());
		m_bDead = true;
	}

}

void CWraight::Render()
{
	if (!Aggroed)
		return;

	RECT temp;
	Camera::GetInstance()->CamRect(&temp);

	RECT rRect2;
	rRect2.top = 0;
	rRect2.left = 0;
	rRect2.bottom = 12;
	rRect2.right = (LONG)(70 * (this->m_nHealth / (float)m_nMaxHealth));

	CSGD_TextureManager::GetInstance()->Draw(this->m_nHealthBarBackgroundImgID, GetPosX() - temp.left - 32, GetPosY() - temp.top - 80);

	if((this->m_nHealth / (float)m_nMaxHealth) >= 0.66)
	{
		CSGD_TextureManager::GetInstance()->Draw(this->m_nHealthBarGreenImgID, GetPosX() - temp.left - 32, GetPosY() - temp.top - 80, 1.0f, 1.0f, &rRect2);
	}
	else if((this->m_nHealth / (float)m_nMaxHealth) >= 0.33)
	{
		CSGD_TextureManager::GetInstance()->Draw(this->m_nHealthBarYellowImgID, GetPosX() - temp.left - 32, GetPosY() - temp.top - 80, 1.0f, 1.0f, &rRect2);
	}
	else
	{
		CSGD_TextureManager::GetInstance()->Draw(this->m_nHealthBarRedImgID, GetPosX() - temp.left - 32, GetPosY() - temp.top - 80,1.0f, 1.0f, &rRect2);
	}

	CEntity::Render();

	if (StunEffect)
		StunEffect->Render();
}

void CWraight::SetLevel( int L )
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
		m_nMaxHealth = m_nHealth = 25;
		m_nExperience = 35;
		m_nDamage = -9;
		m_nSpeed = 110;
		m_nElement = 0;
		m_fFireResist	= 0.5f;
		m_fPhysicalResist = 1.5f;
		m_nHealAmount = 50;
		break;
	case 2:
		m_nMaxHealth = m_nHealth = 55;
		m_nExperience = 350;
		m_nDamage = -15;
		m_nSpeed = 130;
		m_nElement = 1;
		m_fFireResist	= 1.5f;
		m_fIceResist = 0.3f;
		m_nHealAmount = 60;
		break;
	case 3:
		m_nMaxHealth = m_nHealth = 75;
		m_nExperience = 3500;
		m_nDamage = -24;
		m_nSpeed = 140;
		m_nElement = 3;
		m_fPhysicalResist	= 0.3f;
		m_fIceResist		= 1.5f;
		m_nHealAmount = 70;
		break;
	case 4:
		m_nMaxHealth = m_nHealth = 100;
		m_nExperience = 35000;
		m_nDamage = -45;
		m_nSpeed = 150;
		m_nElement = 2;
		m_fPhysicalResist	= 1.5f;
		m_nHealAmount = 90;
		break;
	}
}