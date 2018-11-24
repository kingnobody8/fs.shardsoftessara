/*
	Author: Jorge Juaneda
	File: Monster.h
*/

#pragma once
#include "Entity.h"
#include "PathFinder.h"

class CMonster : public CEntity, public CPathFinder

{
protected:
	int m_nHealth;
	int m_nMaxHealth;
	int m_nLevel;

	bool Aggroed;

	float m_fFireResist;
	float m_fIceResist;
	float m_fLightResist;
	float m_fEarthResist;
	float m_fPhysicalResist;

	int m_nDamage;
	int m_nWeakness;
	int m_nElementWeakness;
	int m_nSpeed;

	int m_nResistance;
	int m_nElementResistance;

	bool m_bOnIce;
	bool m_bSlowed;

	int m_nAttckRange;
	float m_fVelocity;
	double m_dAttackCooldown;
	unsigned int m_unRefCount;

	//int m_nFacing;


	CEntity* m_pTarget;

public:

	CMonster(void);
	virtual ~CMonster(void);

	virtual void Update( float deltaTime );
	virtual void Render( void );
	virtual bool CheckCollision( IEntity* pOther );

	CMonster(int nHeight, int nWidth, int nX, int nY);
	virtual void ModifyHealth(int nAmount);
	
	virtual RECT GetRect ();

	bool GetOnIce() { return m_bOnIce; }
	bool GetSlowed() {return m_bSlowed; }
	int GetFacing() {return m_nFacing; }
	int GetHealth() {return m_nHealth; }
	int GetLevel()	{ return m_nLevel; }
	bool GetAggro() { return Aggroed; }


	void SetOnIce(bool bIce) { m_bOnIce  = bIce; }
	void SetSlowed(bool bSlowed) {m_bSlowed = bSlowed; }
	void SetFacing(int nFacing) { m_nFacing = nFacing; }
	void SetHealth(int nHealth) {m_nHealth = nHealth; }

};