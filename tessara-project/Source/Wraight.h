/*
	Author: Jorge Juaneda
	File: Wraight.h
*/

#pragma once
#include "monster.h"
#include "..\\SGD Wrappers\IListener.h"

class CParticleEffect;

class CWraight :
	public CMonster, public IListener
{
	private:

	bool m_bAttacking;
	bool m_bInRange;
	bool m_bCanAttack;
	bool m_bHealing;
	bool m_bHealSelf;
	bool m_bHeal;
	bool m_bShootX;
	bool m_bShootY;
	bool m_bDead;

	int m_nElement;
	int m_nExperience;
	int m_nCurrency;
	int m_nHealAmount;
	int m_nRange;
	int m_nMaxHealth;
	int m_nImmuneElement;

	float m_fHealCooldown;
	float m_fPathTimer;
	float m_fAttackTimer;
	float m_fAttackCooldown;
	float m_fHealTimer;
	float m_fTakeFireDamage;
	
	int m_nHealthBarBackgroundImgID;
	int m_nHealthBarRedImgID;
	int m_nHealthBarGreenImgID;
	int m_nHealthBarYellowImgID;


	POINT Goal;

	CEntity* m_pPlayer;

	void Idle();
	void Offensive();
	void Heal();
	void Retreat();
	void Attack();
	void ElementalDamage( int type, int amount );

	//Stun and Freeze
	float m_fStunTimer;
	CParticleEffect* StunEffect;
	void Stun( void );
	void Freeze( void );

public:
	CWraight(void);
	~CWraight(void);

	void Render();
	void Update( float deltaTime);
	virtual bool CheckCollision( IEntity* pOther);
	void SetTarget( CEntity* t );
	void SetArea();
	void SetLevel( int L );

	void TakeDamage( int Damage );
	void HealSelf();

	void SetPlayer(CEntity* p) {m_pPlayer = p; }

	// IListener Interface
	virtual void HandleEvent(CEvent* pEvent);
};

