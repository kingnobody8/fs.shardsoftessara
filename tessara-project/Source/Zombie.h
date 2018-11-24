/*
	Author: Jorge Juaneda
	File: Zombie.h
*/

#pragma once
#include "monster.h"
#include "../SGD Wrappers/IListener.h"
#include "AnimationInformation.h"
#include <vector>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace std;

class CParticleEffect;

class CZombie :
	public CMonster, public IListener
{
public:
	CZombie(void);
	~CZombie(void);

	virtual void Update( float deltaTime );
	virtual void Render( void );
	virtual bool CheckCollision( IEntity* pOther);
	void SetTarget( CEntity* t );
	void SetArea();
	void SetLevel( int L );

	// IListener Interface
	virtual void HandleEvent(CEvent* pEvent);

private:

	enum State { IDLE, OFFENSIVE, RETREATING };
	State Currently;

	CParticleEffect* EnrageEffect;
	void ElementalDamage( int type, int amount );
	bool m_bAttacking;
	bool m_bInRange;
	bool m_bCanAttack;
	int   m_nMaxHealth;
	float m_fAttackTimer;
	float m_fAttackCooldown;
	int	m_nExperience;
	int m_nMeleeRange;
	int m_nCurrency;
	float m_fPathTimer;
	float m_fTakeFireDamage;
	bool m_bHasAlreadyHit;
	POINT Goal;
	
	bool m_bDead;

	int m_nHealthBarBackgroundImgID;
	int m_nHealthBarRedImgID;
	int m_nHealthBarGreenImgID;
	int m_nHealthBarYellowImgID;

	RECT m_rAttack;

	bool m_bEnraged;

	void Summon();
	void TakeDamage( int Damage );
	void IdleUpdate( float delatTime );
	void OffensiveUpdate( float deltaTime );
	void RetreatingUpdate( float deltaTime );
	void CalculateAttack( void );
	void Attack();

	CEntity* m_pTarget;
};

