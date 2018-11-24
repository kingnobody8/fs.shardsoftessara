/*
	Author: Jorge Juaneda
	File: Zombie.cpp
*/

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
#include "ParticleEffect.h"
#include "MessageSystem.h"
#include "AddCurrencyMessage.h"
#include "LevelManager.h"
#include "ItemManager.h"

#include "Player.h"
#include "Zombie.h"


CZombie::CZombie(void)
{
	SetType( ENT_ZOMBIE );
	m_pTarget = nullptr;
	EnrageEffect = nullptr;
	m_nHealth = 75;
	m_nMaxHealth = 75;
	m_bAttacking = false;
	m_bInRange = false;
	Currently = IDLE;
	m_nExperience = 120;
	m_bCanAttack = true;
	m_nDamage = -30;
	m_fPathTimer = 3.0f;
	Aggroed = false;
	m_nMeleeRange = 75;
	m_fAttackTimer = 0.0f;
	m_bHasAlreadyHit = false;
	m_nSpeed = 70;
	m_fTakeFireDamage = 0.0f;

	m_bDead = false;

	CSGD_EventSystem::GetInstance()->RegisterClient( "TAKE_DAMAGE", this );
	CSGD_EventSystem::GetInstance()->RegisterClient( "TAKE_FIRE_DAMAGE", this );
	CSGD_EventSystem::GetInstance()->RegisterClient( "TAKE_ICE_DAMAGE", this );
	CSGD_EventSystem::GetInstance()->RegisterClient( "TAKE_ELECTRIC_DAMAGE", this );
	CSGD_EventSystem::GetInstance()->RegisterClient( "TAKE_EARTH_DAMAGE", this );
	CSGD_EventSystem::GetInstance()->RegisterClient( "MODIFY_HEALTH", this);
	
	m_nHealthBarBackgroundImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/HUD/enemy_health_bar_background.png"));
	m_nHealthBarRedImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/HUD/enemy_health_bar_red.png"));
	m_nHealthBarGreenImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/HUD/enemy_health_bar_green.png"));
	m_nHealthBarYellowImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/HUD/enemy_health_bar_yellow.png"));

	SetFacing(1);

	m_nCurrency = 10;
}

void CZombie::SetArea()
{
	if(m_nLevel == 1)
	{
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieForestWalkUp"));	//Walk Up		0
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieForestWalkDown"));	//Walk Down		1
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieForestWalkLeft"));	//Walk Left		2
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieForestWalkRight"));	//Walk Right	3
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieForestStandUp"));	//Stand Up		4
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieForestStandDown"));	//Stand Down	5
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieForestStandLeft"));	//Stand Left	6
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieForestStandRight"));	//Stand Right	7
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieForestAttackUp"));	//Shoot Up		8
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieForestAttackDown"));	//Shoot Down	9
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieForestAttackLeft"));	//Shoot Left	10
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieForestAttackRight"));	//Shoot Right	11
		SetCurrAnim(5);
	}
	else if(m_nLevel == 2)
	{
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieSnowyWalkUp"));	//Walk Up		0
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieSnowyWalkDown"));	//Walk Down		1
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieSnowyWalkLeft"));	//Walk Left		2
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieSnowyWalkRight"));	//Walk Right	3
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieSnowyStandUp"));	//Stand Up		4
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieSnowyStandDown"));	//Stand Down	5
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieSnowyStandLeft"));	//Stand Left	6
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieSnowyStandRight"));	//Stand Right	7
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieSnowyAttackUp"));	//Shoot Up		8
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieSnowyAttackDown"));	//Shoot Down	9
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieSnowyAttackLeft"));	//Shoot Left	10
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieSnowyAttackRight"));	//Shoot Right	11
		SetCurrAnim(5);
	}
	else if(m_nLevel == 3)
	{
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieMountainWalkUp"));	//Walk Up		0
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieMountainWalkDown"));	//Walk Down		1
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieMountainWalkLeft"));	//Walk Left		2
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieMountainWalkRight"));	//Walk Right	3
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieMountainStandUp"));	//Stand Up		4
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieMountainStandDown"));	//Stand Down	5
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieMountainStandLeft"));	//Stand Left	6
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieMountainStandRight"));	//Stand Right	7
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieMountainAttackUp"));	//Shoot Up		8
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieMountainAttackDown"));	//Shoot Down	9
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieMountainAttackLeft"));	//Shoot Left	10
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieMountainAttackRight"));	//Shoot Right	11
		SetCurrAnim(5);
	}
	else
	{
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieDesertWalkUp"));	//Walk Up		0
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieDesertWalkDown"));	//Walk Down		1
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieDesertWalkLeft"));	//Walk Left		2
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieDesertWalkRight"));	//Walk Right	3
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieDesertStandUp"));	//Stand Up		4
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieDesertStandDown"));	//Stand Down	5
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieDesertStandLeft"));	//Stand Left	6
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieDesertStandRight"));	//Stand Right	7
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieDesertAttackUp"));	//Shoot Up		8
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieDesertAttackDown"));	//Shoot Down	9
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieDesertAttackLeft"));	//Shoot Left	10
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("zombieDesertAttackRight"));	//Shoot Right	11
		SetCurrAnim(5);
	}
}


CZombie::~CZombie(void)
{
	if ( EnrageEffect != nullptr )
		delete EnrageEffect;

	if (m_pTarget != nullptr)
		m_pTarget->Release();

	CSGD_EventSystem::GetInstance()->UnregisterClientAll(this);

	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nHealthBarBackgroundImgID);
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nHealthBarYellowImgID);
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nHealthBarRedImgID);
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nHealthBarGreenImgID);
}

void CZombie::SetTarget( CEntity* t )
{
	if (m_pTarget != nullptr)
		m_pTarget->Release();

	m_pTarget = t;
	m_pTarget->AddRef();
}

void CZombie::IdleUpdate( float deltaTime )
{
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
			Currently = OFFENSIVE;
		}
		else
		{
			switch(m_nFacing)
			{
			case 0:
				{
					SetCurrAnim(4);
				}
			case 1:
				{
					SetCurrAnim(5);
				}
			case 2:
				{
					SetCurrAnim(6);
				}
			case 3:
				{
					SetCurrAnim(7);
				}
			}
			return;
		}
	}
}

void CZombie::OffensiveUpdate( float deltaTime )
{
	if (EnrageEffect != nullptr)
	{
		EnrageEffect->SetXPosition(GetPosX());
		EnrageEffect->SetYPosition(GetPosY());
		EnrageEffect->Update( deltaTime );
	}

	tVector2D vToTarget = 
		{
			(float)(m_pTarget->GetPosX() - this->GetPosX()),
			(float)(m_pTarget->GetPosY() - this->GetPosY())
		};

	if  (Vector2DLength(vToTarget) > 900.0f)
	{
		Aggroed = false;
		Currently = IDLE;
		return;
	}


	m_fTakeFireDamage -= deltaTime;
	if (m_fTakeFireDamage <= 0.01)
		m_fTakeFireDamage = 0;

	if (LevelManager::GetInstance()->OnFire(this) && m_fTakeFireDamage <= 0)
	{
		ModifyHealth(-5);
		m_fTakeFireDamage = 1.0f;
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
		CAudioManager::GetInstance()->PlaySFX( SFX_WIGHTATTACK );
		m_bAttacking = true;
		m_fAttackTimer = 0.0f;
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

/*virtual*/ void CZombie::Update( float deltaTime )
{
	switch (Currently)
	{
	case IDLE: IdleUpdate( deltaTime );
		break;
	case OFFENSIVE: OffensiveUpdate( deltaTime );
		break;
	case RETREATING: RetreatingUpdate( deltaTime );
		break;
	}
}

void CZombie::RetreatingUpdate( float deltaTime )
{


}

void CZombie::CalculateAttack( void )
{
	RECT rRect = CEntity::GetRect();
		RECT Cam;
		Camera::GetInstance()->CamRect(&Cam);

		m_rAttack.left = rRect.left - 50 - Cam.left;
		m_rAttack.right = rRect.right + 50 - Cam.left;
		m_rAttack.top = rRect.top - 50 - Cam.top;
		m_rAttack.bottom = rRect.bottom + 50 -Cam.top;
}

/*virtual*/ void CZombie::HandleEvent( CEvent* pEvent )
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
			|| pEvent->GetDestination() == nullptr)	// broadcast for everyone
		{
			int* pAmount = reinterpret_cast< int* >( pEvent->GetParam() );
			int Amount = *pAmount;
			ModifyHealth(Amount);
		}

	}
	 
}

void CZombie::ElementalDamage( int type, int amount )
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
	
	int Damage = (int)(amount * modifier);
	TakeDamage(Damage);

}
/*virtual*/ void CZombie::Render( void )
{
	if (!Aggroed)
		return;

	if (EnrageEffect != nullptr)
		EnrageEffect->Render();

	/*if (m_bAttacking)
	{
		CalculateAttack();
		CSGD_Direct3D::GetInstance()->DrawRect(m_rAttack,255,0,0);
	}*/

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
	rRect2.right = (LONG)(70 * (this->m_nHealth /(float)m_nMaxHealth));

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
}

bool CZombie::CheckCollision( IEntity* pOther )
{

	return false;
}

void CZombie::TakeDamage( int Damage )
{
	ModifyHealth( Damage );
	if (m_nHealth <= 0)
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

	if (m_nHealth <= .40f * m_nMaxHealth)
		CSGD_EventSystem::GetInstance()->SendEventNow(
		"HEAL_MONSTER", NULL, NULL, this );

	if (m_nHealth <= .40f * m_nMaxHealth && EnrageEffect == nullptr )
	{
		m_nDamage = (int)(m_nDamage * 1.5);
		m_nSpeed += 50;
		EnrageEffect = new CParticleEffect;
		EnrageEffect->LoadFromFile("Resource/Graphics/Particles/Enrage.xml" );
		EnrageEffect->ToggleCamera();
		EnrageEffect->SetXPosition(GetPosX());
		EnrageEffect->SetYPosition(GetPosY());
		EnrageEffect->Initialize();

	}
}

void CZombie::Attack()
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

void CZombie::SetLevel( int L )
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
		m_nMaxHealth = m_nHealth = 70;
		m_nExperience = 50;
		m_nDamage = -12;
		m_nSpeed = 70;
		m_fFireResist	= 1.5f;
		break;
	case 2:
		m_nMaxHealth = m_nHealth = 130;
		m_nExperience = 500;
		m_nDamage = -15;
		m_nSpeed = 80;
		m_fFireResist	= 0.3f;
		m_fIceResist = 1.5f;
		break;
	case 3:
		m_nMaxHealth = m_nHealth = 180;
		m_nExperience = 5000;
		m_nDamage = -36;
		m_nSpeed = 90;
		m_fIceResist	= 1.5f;
		break;
	case 4:
		m_nMaxHealth = m_nHealth = 250;
		m_nExperience = 50000;
		m_nDamage = -53;
		m_nSpeed = 100;
		m_fLightResist	= 0.3f;
		m_fEarthResist	= 0.3f;
		m_fFireResist	= 1.5f;
		break;
	}
}
