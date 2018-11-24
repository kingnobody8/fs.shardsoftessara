/*
	Author: Jorge Juaneda
	File: Boss.h
*/

#pragma once
#include "monster.h"
#include "Entity.h"
#include "..\\SGD Wrappers\IListener.h"

#include <vector>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
class CParticleEffect;

class CBoss :
	public CMonster, public IListener
{
private:
	

	bool m_bSpawn;
	bool m_bAttack;
	bool m_bDash;
	bool m_bAttacking;
	bool m_bInRange;
	bool m_bCanAttack;
	bool m_bShoot;
	bool m_bShootAb;
	bool m_bIce;
	bool m_bDead;

	bool m_bShootX;
	bool m_bShootY;

	float m_fAttackTimer;
	float m_fAttackCooldown;
	float m_fSpawnTimer;
	float m_fSpawnCooldown;
	int m_nMeleeRange;
	float m_fShootCooldown;
	float m_fShootTimer;
	float m_fDamageTimer;
	float m_fStunTimer;
	float m_fDashCooldown;
	float m_fDash;
	float m_fTacticTimer;

	float m_fOffenceTimer;
	float m_fRangeTimer;
	float m_fOffenceCooldown;
	float m_fRangeCooldown;
	bool m_bOff;

	float m_fCollitionTimer;

	int m_nRange;
	int m_nStatus;
	int m_nLungRange;
	int m_nDistance;
	int m_nExperience;
	int m_nCurrency;
	int m_nSpawnRange;
	int m_nShootRange;
	
	int m_nElement;
	int m_nMaxHealth;

	int m_nHealthBarBackgroundImgID;
	int m_nHealthBarRedImgID;
	int m_nHealthBarGreenImgID;
	int m_nHealthBarYellowImgID;

	float m_fPathTimer;
	POINT Goal;

	CParticleEffect* StunEffect;

	CEntity* m_pTarget;

public:
	CBoss(void);
	virtual ~CBoss(void);

	void Summon();
	void Update(float deltaTime);
	bool CheckCollision( IEntity* pOther);
	RECT GetRect( void );

	void SetBoss(int nLevel);
	void Idle();
	void Offensive();
	void Retreat();
	void Meele();
	void Shoot();
	void SpawnMonsters();
	void Lunge();
	void GetInPosition();
	void CloseRangeAttacks();
	void LongRangeAttacks();
	void Stun();

	void ShootAbility();

	void SetTarget(CEntity* pTarget);

	void TakeDamage(int pDamage);
	void HandleEvent(CEvent* pEvent);
	void Render( void );
	void SetArea();

	void SetLevel(int nLevel) {m_nLevel = nLevel; }
	int GetLevel() {return m_nLevel; }
};

