/*
Author: Jorge Juaneda
File: Boss.cpp
*/

#include "Boss.h"
#include "AnimationManager.h"
#include "Projectile.h"
#include "AddCurrencyMessage.h"
#include "MessageSystem.h"
#include "Camera.h"
#include "Player.h"
#include "PlayState.h"
#include "TestMsg.h"
#include "MessageSystem.h"
#include "CreateProjectileMsg.h"
#include "DeleteEntityMessage.h"
#include "LevelManager.h"
#include "ItemManager.h"
#include "ParticleEmitter.h"
#include "CreateTripleShootMsg.h"
#include "..\\SGD Wrappers\CSGD_EventSystem.h"
#include "../SGD Wrappers/SGD_Math.h"

enum Animations { bossWalkDown, bossStandDown, bossEarthShieldDown, bossAttackSlashDown, bossAttackLungeDown, bossCastDown, bossAttackSmashDown, bossAttackStabDown,
	bossWalkUp, bossStandUp, bossEarthShieldUp, bossAttackSlashUp, bossAttackLungeUp, bossCastUp, bossAttackSmashUp, bossAttackStabUp,
	bossWalkLeft, bossStandLeft, bossEarthShieldLeft, bossAttackSlashLeft, bossAttackLungeLeft, bossCastLeft, bossAttackSmashLeft, bossAttackStabLeft,
	bossWalkRight, bossStandRight, bossEarthShieldRight, bossAttackSlashRight, bossAttackLungeRight, bossCastRight, bossAttackSmashRight, bossAttackStabRight};

CBoss::CBoss(void) 
{
	m_pTarget	 = nullptr;
	m_bSpawn	 = false;
	m_bAttack	 = false;
	m_bDash		 = false;
	Aggroed		 = false;
	m_bAttacking = false;
	m_bInRange	 = false;
	m_bCanAttack = false;
	m_bShoot = false;
	m_bIce = true;
	m_bShootAb = false;
	m_bShootX  = false;
	m_bShootY  = false;
	m_bDead    = false;

	StunEffect = nullptr;

	SetRefCounter();


	m_fAttackTimer		= 5.0f;
	m_fAttackCooldown	= 0.0f;
	m_fSpawnTimer		= 20.0f;
	m_fSpawnCooldown	= 0.0f;
	m_fShootCooldown	= 0.0f;
	m_fShootTimer		= 3.5f;
	m_fDamageTimer		= 0.0f;
	m_fStunTimer = 0.0f;
	m_fDashCooldown = 0.0f;
	m_fDash = 0.0f;
	m_fTacticTimer = 15.0f;

	m_fOffenceTimer	 = 20.0f;
	m_fRangeTimer	 = 25.0f;
	m_fOffenceCooldown	 = 0.0f;
	m_fRangeCooldown	 = 0.0f;
	m_fCollitionTimer = 0.0f;

	m_bOff = false;

	m_nRange   = 0;
	m_nStatus  = 0;
	m_nLungRange = 300;
	m_nDistance = 0;
	SetHealth( 10);
	m_nMaxHealth = 10;
	m_nExperience = 500;
	m_nCurrency = 200;
	m_nSpeed = 100;
	m_nMeleeRange = 70;
	m_nSpawnRange = 400;
	m_nShootRange = 800;
	m_nElement = 0;

	m_nLevel = 0;
	SetType( ENT_BOSS);
	SetFacing(3);

	SetCurrAnim(9);

	CSGD_EventSystem::GetInstance()->RegisterClient( "TAKE_DAMAGE", this );
	CSGD_EventSystem::GetInstance()->RegisterClient( "TAKE_EARTH_DAMAGE", this );
	CSGD_EventSystem::GetInstance()->RegisterClient( "TAKE_LIGHTNING_DAMAGE", this );
	CSGD_EventSystem::GetInstance()->RegisterClient( "TAKE_ICE_DAMAGE", this );
	CSGD_EventSystem::GetInstance()->RegisterClient( "TAKE_FIRE_DAMAGE", this );
	CSGD_EventSystem::GetInstance()->RegisterClient( "STUN", this );

	m_nHealthBarBackgroundImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/HUD/enemy_health_bar_background.png"));
	m_nHealthBarRedImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/HUD/enemy_health_bar_red.png"));
	m_nHealthBarGreenImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/HUD/enemy_health_bar_green.png"));
	m_nHealthBarYellowImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/HUD/enemy_health_bar_yellow.png"));
}

void CBoss::SetArea()
{
	if(m_nLevel == 1)
	{
		// Fire
		// Down
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireWalkDown")); 
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireStandDown")); 
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireBlockDown"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireSlashDown"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireLungeDown"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireCastDown")); 
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireSmashDown"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireStabDown")); 

		// Up
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireWalkUp"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireStandUp")); 
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireBlockUp")); 
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireSlashUp"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireLungeUp"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireCastUp")); 
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireSmashUp")); 
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireStabUp")); 

		// Left
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireWalkLeft"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireStandLeft"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireBlockLeft"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireSlashLeft"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireLungeLeft"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireCastLeft"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireSmashLeft"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireStabLeft"));

		// Right
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireWalkRight"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireStandRight"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireBlockRight"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireSlashRight"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireLungeRight"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireCastRight"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireSmashRight"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossFireStabRight"));

		SetCurrAnim(bossStandDown);
	}
	else if(m_nLevel == 2)
	{
		// Ice
		// Down
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceWalkDown")); 
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceStandDown")); 
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceBlockDown"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceSlashDown"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceLungeDown"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceCastDown")); 
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceSmashDown"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceStabDown")); 

		// Up
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceWalkUp"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceStandUp")); 
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceBlockUp")); 
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceSlashUp"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceLungeUp"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceCastUp")); 
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceSmashUp")); 
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceStabUp")); 

		// Left
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceWalkLeft"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceStandLeft"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceBlockLeft"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceSlashLeft"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceLungeLeft"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceCastLeft"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceSmashLeft"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceStabLeft"));

		// Right
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceWalkRight"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceStandRight"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceBlockRight"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceSlashRight"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceLungeRight"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceCastRight"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceSmashRight"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossIceStabRight"));

		SetCurrAnim(bossStandDown);
	}
	else if(m_nLevel == 3)
	{
		// Earth
		// Down
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthWalkDown")); 
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthStandDown")); 
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthBlockDown"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthSlashDown"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthLungeDown"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthCastDown")); 
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthSmashDown"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthStabDown")); 

		// Up
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthWalkUp"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthStandUp")); 
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthBlockUp")); 
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthSlashUp"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthLungeUp"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthCastUp")); 
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthSmashUp")); 
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthStabUp")); 

		// Left
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthWalkLeft"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthStandLeft"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthBlockLeft"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthSlashLeft"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthLungeLeft"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthCastLeft"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthSmashLeft"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthStabLeft"));

		// Right
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthWalkRight"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthStandRight"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthBlockRight"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthSlashRight"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthLungeRight"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthCastRight"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthSmashRight"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossEarthStabRight"));

		SetCurrAnim(bossStandDown);
	}
	else
	{
		// Lightning
		// Down
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningWalkDown")); 
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningStandDown")); 
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningBlockDown"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningSlashDown"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningLungeDown"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningCastDown")); 
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningSmashDown"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningStabDown")); 

		// Up
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningWalkUp"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningStandUp")); 
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningBlockUp")); 
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningSlashUp"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningLungeUp"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningCastUp")); 
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningSmashUp")); 
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningStabUp")); 

		// Left
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningWalkLeft"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningStandLeft"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningBlockLeft"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningSlashLeft"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningLungeLeft"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningCastLeft"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningSmashLeft"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningStabLeft"));

		// Right
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningWalkRight"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningStandRight"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningBlockRight"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningSlashRight"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningLungeRight"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningCastRight"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningSmashRight"));
		CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("bossLightningStabRight"));
	}
}

CBoss::~CBoss(void)
{
	CSGD_EventSystem::GetInstance()->UnregisterClientAll(this);

	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nHealthBarBackgroundImgID);
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nHealthBarYellowImgID);
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nHealthBarRedImgID);
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nHealthBarGreenImgID);
}


void CBoss::SetBoss(int nLevel)
{
	switch(nLevel)
	{
	case 1:
		{
			SetHealth( 150);
			m_nMaxHealth = 150;
			m_nExperience = 300;
			m_nDamage = 10;
			m_nCurrency = 100;
			m_nSpeed = 100;
			m_nElement = 2;
		}
		break;

	case 2:
		{
			SetHealth( 250);
			m_nMaxHealth = 250;
			m_nExperience = 500;
			m_nCurrency = 150;
			m_nSpeed = 125;
			m_nDamage = 30;
			m_nElement = 1;

		}
		break;

	case 3:
		{

			SetHealth( 300);
			m_nMaxHealth = 300;
			m_nExperience = 700;
			m_nCurrency = 200;
			m_nSpeed = 150;
			m_nDamage = 40;
			m_nElement = 2;
		}
		break;

	case 4:
		{
			SetHealth( 400);
			m_nMaxHealth = 400;
			m_nExperience = 1000;
			m_nCurrency = 300;
			m_nSpeed = 150;
			m_nDamage = 50;
			m_nElement = 3;

		}
		break;
	}
}


void CBoss::Update(float deltaTime)
{

	m_fDamageTimer -= deltaTime;
	if (m_fDamageTimer <= 0.01f)
		m_fDamageTimer = 0.0f;


	if (m_pTarget == nullptr)
		return;

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


	//if (GetHealth() < 250)
	//	m_nElement = 0;
	//else if (GetHealth() < 400)
	//	m_nElement = 1;

	//SetOnIce( LevelManager::GetInstance()->OnIce(this));
	//if (GetOnIce() && m_nElement != 1)
	//	m_nSpeed = 63;
	//else
	//	m_nSpeed = 100;

	tVector2D vToTarget = 
	{
		(float)(m_pTarget->GetPosX() - this->GetPosX()),
		(float)(m_pTarget->GetPosY() - this->GetPosY())
	};

	if (!Aggroed)
	{
		if (Vector2DLength(vToTarget) < 300.0f)
		{
			Aggroed = true;
		}
		else
		{
			switch(m_nFacing)
			{
			case 0:
				{
					SetCurrAnim(bossStandUp);
				}
			case 1:
				{
					SetCurrAnim(bossStandDown);
				}
			case 2:
				{
					SetCurrAnim(bossStandLeft);
				}
			case 3:
				{
					SetCurrAnim(bossStandRight);
				}
			}
			return;
		}
	}
	else if  (Vector2DLength(vToTarget) > 900.0f)
		Aggroed = false;


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

	m_fAttackCooldown -= deltaTime;
	if (m_fAttackCooldown <= 0.01f)
		m_fAttackCooldown = 0.0f;

	m_fSpawnCooldown -= deltaTime;
	if (m_fSpawnCooldown <= 0.01f)
		m_fSpawnCooldown = 0.0f;

	m_fShootCooldown -= deltaTime;
	if (m_fShootCooldown <= 0.01f)
		m_fShootCooldown = 0.0f;

	m_fDamageTimer -= deltaTime;
	if (m_fDamageTimer <= 0.01f)
		m_fDamageTimer = 0.0f;

	m_fCollitionTimer -= deltaTime;
	if (m_fCollitionTimer <= 0.01f)
		m_fCollitionTimer = 0.0f;

	m_fDashCooldown -= deltaTime;
	if (m_fDashCooldown <= 0.01f)
		m_fDashCooldown = 0.0f;

	//if (m_nLevel > 2)
	//{
	//	m_fTacticTimer -= deltaTime;
	//	if (m_fTacticTimer <= 0)
	//	{
	//		m_bOff  = !m_bOff;
	//		m_fTacticTimer = 15.0f;
	//	} 
	//}

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


	//if (m_nHealth > (m_nHealth * 50)/ 100 || m_bAttacking)
	//{
	//	CloseRangeAttacks();
	//	return;
	//}

	//if (Vector2DLength(vToTarget) < m_nMeleeRange && m_fAttackCooldown <= 0 && !m_bAttacking)
	//	m_bAttacking = true;


	if( Vector2DLength(vToTarget) < 75 )
		m_bInRange = true;
	else
		m_bInRange = false;


	if (Vector2DLength(vToTarget) < 75 && m_fAttackCooldown <= 0 && !m_bDash && !m_bShoot)
		Meele();

	if (!m_bOff)
	{
		if (Vector2DLength(vToTarget) < 500 && m_fShootCooldown <= 0 && !m_bCanAttack && !m_bAttack && !m_bDash)
		{
			int r = 0;
			if (m_nLevel > 1)
				r = rand()% 2;

			if (r == 0)
				m_bIce = true;
			else
				m_bIce = false;

			ShootAbility();

		}
		if (Vector2DLength(vToTarget) < m_nShootRange && m_fShootCooldown <= 0 && m_bCanAttack && !m_bShoot && !m_bAttack && !m_bDash)
		{
			Shoot();
			m_bCanAttack = false;
		}

	}
	else if (m_bOff)
	{
		if ( (Vector2DLength(vToTarget) > 150 && m_fDashCooldown <= 0))
		Lunge();
	}


	if (m_bDash)
	{
		Lunge();
		m_fDash -= deltaTime;

		LevelManager* LM = LevelManager::GetInstance();
		RECT rOverlap;

		rOverlap = LM->Collision(this);

		if (rOverlap.left != 0 && rOverlap.right != 0)
		{
			int nOVerWidth = rOverlap.right - rOverlap.left;
			int nOverHeight = rOverlap.bottom - rOverlap.top;

			int nCenterOverX = nOVerWidth + rOverlap.left;
			int nCenterOverY = nOverHeight + rOverlap.top;

			if (nOverHeight < nOVerWidth)
			{
				//move up or down
				if (GetRect().top == rOverlap.top)
					SetPosY( GetPosY() + nOverHeight);
				else
					SetPosY( GetPosY() - nOverHeight);

			}
			else
			{
				//move left or right
				if (GetRect().left == rOverlap.left)
					SetPosX( GetPosX() + nOVerWidth);
				else
					SetPosX( GetPosX() - nOVerWidth);
			}

				m_bDash = false;
				SetVelX(0);
				SetVelY(0);
				m_fPathTimer = 0.0f;
		}

		CEntity::Update(deltaTime);
		return;
	}


	//if (Vector2DLength(vToTarget) >= m_nSpawnRange && m_fSpawnCooldown == 0 && !m_bShoot && !m_bAttack)
	//	SpawnMonsters();

	if (!m_bInRange && !m_bDash && !m_bAttack && !m_bShoot && !m_bShootAb)
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
			SetCurrAnim(bossWalkDown);
		}
		else if (vToWaypoint.fY < 0)
		{
			SetVelY((float)-m_nSpeed);
			SetFacing(0);
			SetCurrAnim(bossWalkUp);
		}

		if (abs(vToWaypoint.fY) < 15)
			SetVelY(0);

		if (vToWaypoint.fX > 0)
		{
			SetVelX((float)m_nSpeed);
			SetFacing(3);
			SetCurrAnim(bossWalkRight);
		}
		else if (vToWaypoint.fX < 0)
		{
			SetVelX((float)-m_nSpeed);
			SetFacing(2);
			SetCurrAnim(bossWalkLeft);
		}

		if (abs(vToWaypoint.fX) < 15)
			SetVelX(0);


		//else if (m_bSpawn)
		//{
		//	SpawnMonsters();
		//	SetVelX(0);
		//	SetVelY(0);
		//}
		//else if (m_bShoot)
		//{
		//	Shoot();
		//	SetVelX(0);
		//	SetVelY(0);
		//}

		CEntity::Update(deltaTime);


		vToWaypoint.fX  = (float)(Goal.x - GetPosX());
		vToWaypoint.fY	= (float)(Goal.y - GetPosY());

		if ( Vector2DLength( vToWaypoint ) < 50.0f )
		{
			if (!Waypoints.empty())
			{
				Goal = Waypoints.top();
				Waypoints.pop();
			}
		}
	}
	else 
	{
		if (m_bShoot)
		{
			Shoot();
			SetVelX(0);
			SetVelY(0);
		}
		else if (m_bShootAb)
		{
			ShootAbility();
			SetVelX(0);
			SetVelY(0);
		}
		else if (m_bAttack)
		{
			Meele();
			SetVelX(0);
			SetVelY(0);
		}

		CEntity::Update(deltaTime);
	}



}

void CBoss::Render( void )
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

bool CBoss::CheckCollision( IEntity* pOther)
{

	//if (AnimationManager::GetInstance()->GetAnimation(this->GetAnimationInformation(this->GetCurrAnim())->GetAnimID()).GetFrame(this->GetAnimationInformation(this->GetCurrAnim())->GetCurFrame()).HasActiveRect())
	//{

	//	Frame tempFrame = AnimationManager::GetInstance()->GetAnimation(this->GetAnimationInformation(this->GetCurrAnim())->GetAnimID()).GetFrame(this->GetAnimationInformation(this->GetCurrAnim())->GetCurFrame());
	//	POINT pAnchor = tempFrame.GetAnchorPoint();
	//	RECT rActiv  = *tempFrame.GetActiveRect();
	//	RECT rRect, rOver;

	//	RECT rOther = pOther->GetRect();

	//	int nHeight = rActiv.bottom - rActiv.top;
	//	int nWidth = rActiv.right - rActiv.left;
	//	int nX = (int)GetPosX();
	//	int nY = (int)GetPosY();

	//	rRect.bottom = nY + (rActiv.bottom - pAnchor.y);
	//	rRect.top = rRect.bottom - nHeight;
	//	rRect.right = nX + (rActiv.right - pAnchor.x);
	//	rRect.left = rRect.right - nWidth; 


	//	if( IntersectRect( &rOver, &rRect, &rOther ) == TRUE )
	//	{
	//		if (m_pTarget->GetType() == ENT_PLAYER)
	//		{
	//			CPlayer* Temp = dynamic_cast<CPlayer*> (m_pTarget);

	//			if (Temp->GetEarthWall())
	//			{
	//				if (IntersectRect(&rOver, &Temp->GetActiveRec(), &rRect))
	//					return true;
	//			}

	//			if (m_fCollitionTimer <= 0)
	//			{
	//				int d = -m_nDamage;
	//				CSGD_EventSystem::GetInstance()->SendEventNow(
	//					"TAKE_DAMAGE", &d, m_pTarget, this );
	//				m_fCollitionTimer = 1.0f;
	//			}
	//		}
	//	}
	//}

	if (m_bAttack && pOther->GetType() == ENT_PLAYER)
	{
		CPlayer* pl = reinterpret_cast<CPlayer*>(pOther);

		tVector2D vToTarget = 
		{
			(float)(m_pTarget->GetPosX() - this->GetPosX()),
			(float)(m_pTarget->GetPosY() - this->GetPosY())
		};

		if (Vector2DLength(vToTarget) < 80)
		{
			if (m_fCollitionTimer <= 0 && !pl->GetEarthWall())
			{
				int d = -m_nDamage;
				CSGD_EventSystem::GetInstance()->SendEventNow(
					"TAKE_DAMAGE", &d, m_pTarget, this );
				m_fCollitionTimer = 1.0f;
			}
		}

	}


	RECT Other = pOther->GetRect();
	RECT En = GetRect();
	RECT over;
	if( IntersectRect(&over, &En, &Other) == TRUE)
	{
		m_bDash = false;
		SetVelX(0);
		SetVelY(0);
	}

	return false;
}

RECT CBoss::GetRect( void ) 
{
	Frame tempFrame = AnimationManager::GetInstance()->GetAnimation(this->GetAnimationInformation(this->GetCurrAnim())->GetAnimID()).GetFrame(this->GetAnimationInformation(this->GetCurrAnim())->GetCurFrame());
	POINT pAnchor = tempFrame.GetAnchorPoint();
	RECT rPassive = *tempFrame.GetPassiveRect();
	RECT rActiv  = *tempFrame.GetActiveRect();
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

void CBoss::Lunge()
{
	if (!m_bDash)
	{
		m_bDash = true;
		m_fDashCooldown = 5.0f;
		m_fDash = 1.0f;

		if (m_bShootX)
		{
			if (GetPosX() > m_pTarget->GetPosX())
			{
				this->SetCurrAnim(bossWalkLeft);
				SetFacing(2);
			}
			else
			{
				this->SetCurrAnim(bossWalkRight);
				SetFacing(3);
			}

		}
		else if	(m_bShootY)
		{
			if (GetPosY() > m_pTarget->GetPosY())
			{
				this->SetCurrAnim(bossWalkUp);
				SetFacing(0);
			}
			else
			{
				this->SetCurrAnim(bossWalkDown);
				SetFacing(1);
			}
		}
		else
			SetCurrAnim(bossWalkUp);
	}

	if (m_fDash > 0.01)
	{

		switch(m_nFacing)
		{
		case 0:
			SetVelX(0);
			SetVelY(-m_nSpeed * 2.0f);
			break;

		case 1:
			SetVelX(0);
			SetVelY(m_nSpeed * 2.0f);
			break;

		case 2:
			SetVelX(-m_nSpeed * 2.0f);
			SetVelY(0);
			break;

		case 3:
			SetVelX(m_nSpeed * 2.0f);
			SetVelY(0);
			break;
		}
	}
	else
	{
		m_bDash = false;
		SetVelX(0);
		SetVelY(0);
	}

}

void CBoss::Summon()
{


}

void CBoss::Idle()
{

}

void CBoss::Offensive()
{


}
void CBoss::Retreat()
{


}


void CBoss::Shoot()
{

	if (!m_bShoot)
	{
		m_bShoot = true;

		if (m_bShootX)
		{
			if (GetPosX() > m_pTarget->GetPosX())
				this->SetCurrAnim(bossCastLeft);
			else
				this->SetCurrAnim(bossCastRight);

		}
		else if	(m_bShootY)
		{
			if (GetPosY() > m_pTarget->GetPosY())
				this->SetCurrAnim(bossCastUp);
			else
				this->SetCurrAnim(bossCastDown);
		}
		else
			SetCurrAnim(bossCastUp);
		this->GetAnimationInformation(this->GetCurrAnim())->Reset();	

		m_fShootCooldown = m_fShootTimer + 2.0f;
	}

	if (this->GetAnimationInformation(GetCurrAnim())->IsOver())
	{
		m_bShoot = false;

		CreateProjectileMsg* msg = nullptr;
		int type = 0;


		switch(GetFacing())
		{
		case 0:
			this->SetCurrAnim(bossStandUp);
			break;

		case 1:
			this->SetCurrAnim(bossStandDown);
			break;

		case 2:
			this->SetCurrAnim(bossStandLeft);
			break;

		case 3:
			this->SetCurrAnim(bossStandRight);
			break;
		}

		CreateTripleShootMsg* Msg;

		if (m_nLevel == 4)
		{
			if (m_bShootX)
			{
				if (GetPosX() > m_pTarget->GetPosX())
				{
					Msg = new CreateTripleShootMsg(300, 2, 3, GetPosX() - GetWidth() - 50, GetPosY(), false);
					CMessageSystem::GetInstace()->SendMsg(Msg);
				}
				else
				{
					Msg = new CreateTripleShootMsg(300, 3, 3, GetPosX() + GetWidth() + 50, GetPosY(), false);
					CMessageSystem::GetInstace()->SendMsg(Msg);
				}

			}
			else if	(m_bShootY)
			{
				if (GetPosY() > m_pTarget->GetPosY())
				{
					Msg = new CreateTripleShootMsg(300, 0, 3, GetPosX(), GetPosY() - GetHeight() - 50, false);
					CMessageSystem::GetInstace()->SendMsg(Msg);
				}
				else
				{
					Msg = new CreateTripleShootMsg(300, 1, 3, GetPosX(), GetPosY() + 50, false);
					CMessageSystem::GetInstace()->SendMsg(Msg);
				}
			}
			return;
		}


		if (m_bShootX)
		{
			if (GetPosX() > m_pTarget->GetPosX())
				msg = new CreateProjectileMsg(300, 2, m_nElement, GetPosX() - GetWidth() - 50, GetPosY(), false);
			else
				msg = new CreateProjectileMsg(300, 3, m_nElement, GetPosX() + GetWidth() + 50, GetPosY(), false);

		}
		else if	(m_bShootY)
		{
			if (GetPosY() > m_pTarget->GetPosY())
				msg = new CreateProjectileMsg(300, 0, m_nElement, GetPosX(), GetPosY() - GetHeight() - 50, false);
			else
				msg = new CreateProjectileMsg(300, 1, m_nElement, GetPosX(), GetPosY() + 50, false);
		}

		if (msg != nullptr)
			CMessageSystem::GetInstace()->SendMsg(msg);
		msg = nullptr;

	}
}

void CBoss::ShootAbility()
{

	if (!m_bShootAb)
	{
		m_bShootAb = true;

		switch(m_nFacing)
		{
		case 0:
			this->SetCurrAnim(bossCastUp);
			break;

		case 1:
			this->SetCurrAnim(bossCastDown);
			break;

		case 2:
			this->SetCurrAnim(bossCastLeft);
			break;

		case 3:
			this->SetCurrAnim(bossCastRight);
			break;
		}
		this->GetAnimationInformation(this->GetCurrAnim())->Reset();	

		m_fShootCooldown = m_fShootTimer + 2.0f;
	}

	if (this->GetAnimationInformation(GetCurrAnim())->IsOver())
	{
		m_bShootAb = false;
		m_nCurrAnim = 0;

		CreateProjectileMsg* msg = nullptr;
		int type = 0;

		CPlayer* pl = reinterpret_cast< CPlayer*> (m_pTarget);

		if (GetPosX() > m_pTarget->GetPosX())
		{
			//player to the left of enemie
			if (GetPosY() > m_pTarget->GetPosY())
			{
				//player up 
				switch(pl->GetFacing())
				{
				case 0:
				case 3:
					{
						msg = new CreateProjectileMsg(300, 0, 1, GetPosX(), GetPosY() - GetHeight() - 50, m_bIce);
					}
					break;

				case 1:
				case 2:
					{
						msg = new CreateProjectileMsg(300, 2, 1, GetPosX() - GetWidth() - 50, GetPosY(), m_bIce);
					}
					break;
				}
			}
			else
			{
				switch(pl->GetFacing())
				{
				case 0:
				case 2:
					{
						msg = new CreateProjectileMsg(300, 3, 1, GetPosX() + GetWidth() + 50, GetPosY(), m_bIce);
					}
					break;

				case 1:
				case 3:
					{
						msg = new CreateProjectileMsg(300, 1, 1, GetPosX(), GetPosY() + 50, m_bIce);
					}
					break;
				}
			}

		}
		else
		{

			if (GetPosY() > m_pTarget->GetPosY())
			{
				switch(pl->GetFacing())
				{
				case 0:
				case 2:
					{
						msg = new CreateProjectileMsg(300, 0, 1, GetPosX(), GetPosY() - GetHeight() - 50, m_bIce);
					}
					break;

				case 1:
				case 3:
					{
						msg = new CreateProjectileMsg(300, 3, 1, GetPosX() + GetWidth() + 50, GetPosY(), m_bIce);
					}
					break;
				}
			}
			else
			{
				switch(pl->GetFacing())
				{
				case 0:
				case 3:
					{
						msg = new CreateProjectileMsg(200, 3, 1, GetPosX() + GetWidth() + 50, GetPosY(), m_bIce);
					}
					break;

				case 1:
				case 2:
					{
						msg = new CreateProjectileMsg(200, 3, 1, GetPosX() + GetWidth() + 50, GetPosY(), m_bIce);
					}
					break;
				}
			}

		}

		if (msg != nullptr)
			CMessageSystem::GetInstace()->SendMsg(msg);
		msg = nullptr;

	}


}

void CBoss::Meele()
{
	if (!m_bAttack)
	{
		m_bAttack = true;


		if (m_bShootX)
		{
			if (GetPosX() > m_pTarget->GetPosX())
				this->SetCurrAnim(bossAttackSlashLeft);
			else
				this->SetCurrAnim(bossAttackSlashRight);

		}
		else if	(m_bShootY)
		{
			if (GetPosY() > m_pTarget->GetPosY())
				this->SetCurrAnim(bossAttackSlashUp);
			else
				this->SetCurrAnim(bossAttackSlashDown);
				
		}
		else
			SetCurrAnim(bossAttackSlashUp);

		this->GetAnimationInformation(this->GetCurrAnim())->Reset();
	}

	if (this->GetAnimationInformation(GetCurrAnim())->IsOver())
	{
		m_bAttack = false;

		switch(this->m_nFacing)
		{
		case 0:
			{
				this->SetCurrAnim(bossStandUp);
			}							
			break;						
		case 1:						
			{							
				this->SetCurrAnim(bossStandDown);
			}							
			break;						
		case 2:						
			{							
				this->SetCurrAnim(bossStandLeft);
			}						
			break;					
		case 3:					
			{						
				this->SetCurrAnim(bossStandRight);
			}
			break;
		}

		m_fAttackCooldown = m_fAttackTimer;
	}
}

void CBoss::SpawnMonsters()
{

	if (!m_bSpawn)
	{
		m_bSpawn = true;

		switch(this->m_nFacing)
		{
		case 0:
			{
				this->SetCurrAnim(bossCastUp);
			}							
			break;						
		case 1:						
			{							
				this->SetCurrAnim(bossCastDown);
			}							
			break;						
		case 2:						
			{							
				this->SetCurrAnim(bossCastLeft);
			}						
			break;					
		case 3:					
			{						
				this->SetCurrAnim(bossCastRight);
			}
			break;
		}

		this->GetAnimationInformation(this->GetCurrAnim())->Reset();	
	}

	if (this->GetAnimationInformation(GetCurrAnim())->IsOver())
	{
		m_bSpawn = false;

		switch(this->m_nFacing)
		{
		case 0:
			{
				this->SetCurrAnim(bossStandUp);
			}							
			break;						
		case 1:						
			{							
				this->SetCurrAnim(bossStandDown);
			}							
			break;						
		case 2:						
			{							
				this->SetCurrAnim(bossStandLeft);
			}						
			break;					
		case 3:					
			{						
				this->SetCurrAnim(bossStandRight);
			}
			break;
		}

		m_fSpawnCooldown = m_fSpawnTimer;

		CTestMsg* msg;
		int type = 0;

		switch(m_nFacing)
		{
		case 0:
			{
				msg = new CTestMsg(GetPosX(), GetPosY() - GetHeight() - 20, type);
			}
			break;

		case 1:
			{
				msg = new CTestMsg(GetPosX(), GetPosY() + GetHeight(), type);
			}
			break;

		case 2:
			{
				msg = new CTestMsg(GetPosX() - GetWidth() - 20, GetPosY(), type);
			}
			break;

		case 3:
			{
				msg = new CTestMsg(GetPosX() + GetWidth() + 20, GetPosY(), type);
			}
			break;
		}

		CMessageSystem::GetInstace()->SendMsg(msg);
		msg = nullptr;

	}

}

void CBoss::HandleEvent(CEvent* pEvent)
{
	// What is the event id?
	if ( pEvent->GetEventID() == "TAKE_DAMAGE" )
	{
		// Is the event for us?
		if( pEvent->GetDestination() == this			// intended for us
			|| pEvent->GetDestination() == nullptr )	// broadcast for everyone
		{
			int* pDamage = reinterpret_cast< int* >( pEvent->GetParam() );
			CEntity* pPr = reinterpret_cast< CEntity* >( pEvent->GetSender() );

			if (m_fDamageTimer <= 0)
			{
				TakeDamage( *pDamage );
				m_fDamageTimer = 1.0f;
			}

		}

	}

	if ( pEvent->GetEventID() == "TAKE_EARTH_DAMAGE" )
	{
		// Is the event for us?
		if( pEvent->GetDestination() == this			// intended for us
			|| pEvent->GetDestination() == nullptr )	// broadcast for everyone
		{
			int* pDamage = reinterpret_cast< int* >( pEvent->GetParam() );

			if (m_fDamageTimer <= 0)
			{
				TakeDamage( *pDamage );
				m_fDamageTimer = 1.0f;
			}

		}

	}

	if ( pEvent->GetEventID() == "TAKE_FIRE_DAMAGE" )
	{
		// Is the event for us?
		if( pEvent->GetDestination() == this			// intended for us
			|| pEvent->GetDestination() == nullptr )	// broadcast for everyone
		{
			int* pDamage = reinterpret_cast< int* >( pEvent->GetParam() );

			if (m_fDamageTimer <= 0)
			{
				TakeDamage( *pDamage );
				m_fDamageTimer = 1.0f;
			}

		}

	}

	if ( pEvent->GetEventID() == "TAKE_ICE_DAMAGE" )
	{
		// Is the event for us?
		if( pEvent->GetDestination() == this			// intended for us
			|| pEvent->GetDestination() == nullptr )	// broadcast for everyone
		{
			int* pDamage = reinterpret_cast< int* >( pEvent->GetParam() );

			if (m_fDamageTimer <= 0)
			{
				TakeDamage( *pDamage );
				m_fDamageTimer = 1.0f;
			}

		}

	}

	if ( pEvent->GetEventID() == "TAKE_LIGHTNING_DAMAGE" )
	{
		// Is the event for us?
		if( pEvent->GetDestination() == this			// intended for us
			|| pEvent->GetDestination() == nullptr )	// broadcast for everyone
		{
			int* pDamage = reinterpret_cast< int* >( pEvent->GetParam() );

			if (m_fDamageTimer <= 0)
			{
				TakeDamage( *pDamage );
				m_fDamageTimer = 1.0f;
			}

		}

	}

	if ( pEvent->GetEventID() == "STUN" )
	{
		if( pEvent->GetDestination() == this			// intended for us
			|| pEvent->GetDestination() == nullptr )	// broadcast for everyone
			Stun();
	}

}

void CBoss::TakeDamage(int pDamage)
{
	if (m_bDead)
		return;

	m_nHealth += pDamage;
	if (GetHealth() <= 0)
	{
		m_bDead = true;
		CSGD_EventSystem::GetInstance()->SendEventNow(
			"GAIN_EXPERIENCE", &m_nExperience, m_pTarget, this );

		AddCurrencyMessage* Msg = new AddCurrencyMessage ( m_nCurrency );
		CMessageSystem::GetInstace()->SendMsg(Msg);
		Msg = nullptr;

		if( PlayState::GetInstance()->GetPlayer()->GetArtifactNumber() == this->m_nLevel && this->m_nLevel != 4)
		{
			CSGD_EventSystem::GetInstance()->SendEventNow("ADD_ARTIFACT", NULL, m_pTarget, this);
		}
		if( this->m_nLevel == 4 )
		{
			//Go to ending
			PlayState::GetInstance()->GoToEnd();
		}

		CItemManager::GetInstance()->DropItem(2, GetPosX(), GetPosY());
		CDeleteEntityMessage* pMsg = new CDeleteEntityMessage( this );
		CMessageSystem::GetInstace()->SendMsg( pMsg );
		pMsg = nullptr;
	}
}

void CBoss::SetTarget(CEntity* pTarget)
{
	if (pTarget != nullptr)
		m_pTarget = pTarget;

}


//get the enemi in position to attack
void CBoss::GetInPosition()
{






}


void CBoss::CloseRangeAttacks()
{
	m_bAttacking = true;
	if (!m_bShootX && !m_bShootY)
	{
		GetInPosition();
		return;
	}
}


void CBoss::Stun()
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