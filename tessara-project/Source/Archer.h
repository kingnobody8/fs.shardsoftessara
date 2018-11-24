/*
	Author: Jorge Juaneda
	File: Archer.h
*/

#pragma once
#include "monster.h"
#include "..\\SGD Wrappers\IListener.h"

class CEvent;
class CParticleEffect;

class CArcher :
	public CMonster, public IListener
{

public:
	CArcher(void);
	~CArcher(void);

	virtual void Update( float deltaTime );
	virtual void Render( void );
	virtual bool CheckCollision( IEntity* pOther);
	void SetTarget( CEntity* t );
	void SetLevel( int L );

	// IListener Interface
	virtual void HandleEvent(CEvent* pEvent);
	void TakeDamage( int Damage );
	void SetArea();

	bool m_bAttacking;
	bool m_bInRange;
	bool m_nRetreat;
	bool m_bCanAttack;
	bool m_bShootX;
	bool m_bShootY;
	bool m_bDead;

	float m_fAttackTimer;
	float m_fAttackCooldown;
	float m_fRetreatTimer;
	float m_fRetreatCooldown;
	float m_fTakeFireDamage;

	int	m_nExperience;
	int m_nCurrency;

	int m_nElement;

	int m_nRange;
	int m_nDistance;
	
	int m_nHealthBarBackgroundImgID;
	int m_nHealthBarRedImgID;
	int m_nHealthBarGreenImgID;
	int m_nHealthBarYellowImgID;

	CEntity* m_pTarget;

	POINT Goal;
	float m_fPathTimer;

private:
	void Summon();

	void Idle();
	void Offensive();
	void Retreat();
	void Attack();
	void ElementalDamage( int type, int amount );
	


	//Stun and Freeze
	float m_fStunTimer;
	CParticleEffect* StunEffect;
	void Stun( void );
	void Freeze( void );
};


