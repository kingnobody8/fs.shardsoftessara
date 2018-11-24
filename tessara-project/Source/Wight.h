/*
	Author: Jorge Juaneda
	File: Wight.h
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

class CWight :
	public CMonster, public IListener
{
public:
	CWight(void);
	~CWight(void);

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

	void ElementalDamage( int type, int amount );
	bool m_bAttacking;
	bool m_bInRange;
	bool m_bCanAttack;
	float m_fAttackTimer;
	float m_fAttackCooldown;
	float m_fTakeFireDamage;

	bool m_bDead;

	//Stun
	float m_fStunTimer;
	CParticleEffect* StunEffect;
	void Stun();

	int	m_nExperience;
	int m_nMeleeRange;
	int m_nCurrency;
	float m_fPathTimer;
	CParticleEffect* BloodEffect;
	float BloodTimer;
	POINT Goal;
	
	int m_nHealthBarBackgroundImgID;
	int m_nHealthBarRedImgID;
	int m_nHealthBarGreenImgID;
	int m_nHealthBarYellowImgID;
	bool m_bHasAlreadyHit;
	int m_nMaxHealth;

	RECT m_rAttack;

	State Currently;

	void Summon();
	void TakeDamage( int Damage );
	void IdleUpdate();
	void OffensiveUpdate();
	void RetreatingUpdate();
	void CalculateAttack( void );
	
	void Attack();

	CEntity* m_pTarget;
	void UpdateEffects( float deltaTime );
};