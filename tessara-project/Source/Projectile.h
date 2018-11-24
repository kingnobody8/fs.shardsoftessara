/*
	Author: Jorge Juaneda
	File: Projectile.h
*/

#pragma once
#include "Entity.h"
#include <vector>

class CParticleEffect;

class CProjectile :
	public CEntity
{
protected:
	enum eElement { FIRE, SNOW, ELECTRIC, EARTH };

private:

	float m_fVelocity;
	int m_nDirecction;
	float m_fAccX;
	float m_fAccY;

	float m_fDuration;
	float m_fTimer;

	bool m_bExplosion;
	float m_fExplosionTimer;

	bool m_bPierce;

	int m_nDamage;
	int m_nLevel;
	bool m_bOwnerP;
	eElement m_eElement;
	CParticleEffect* PEffect;
	std::vector<IEntity*> ExplosionVec;

public:

	CProjectile(void);
	virtual ~CProjectile(void);

	void SetProjectile(float fVel, int nDirection, int nX, int nY);

	virtual void Render();
	virtual void Update( float deltaTime );
	virtual bool CheckCollision( IEntity* pOther);

	virtual RECT GetRect ();

	bool IsExplosion() { return m_bExplosion; }
	void SetAccX( float fAcc) { m_fAccX = fAcc; }
	void SetAccY( float fAcc) { m_fAccY = fAcc; }
	void SetDamage(int nDamage) {m_nDamage = nDamage;}
	void SetDirection( int nDir) {m_nDirecction = nDir; }
	void SetElement( int eElm );
	void SetDuration(float fDur) {m_fDuration = fDur; }
	void SetOwnerp( bool nP ) { m_bOwnerP = nP;}
	void SetPierce( bool bP ) { m_bPierce = bP;}
	void SetVelocity( float f ) { m_fVelocity = f; }
	void SetLevel( int L ) { m_nLevel = L; }
	float GetAccX( ) { return m_fAccX; }
	float GetAccY( ) { return m_fAccY; }
	int GetDamage() { return  m_nDamage;}
	int GetDirection() { return m_nDirecction; }
	int GetElement() { return (int)m_eElement; }
	int GetVel() { return (int)m_fVelocity; };
	bool GetOwnerp() {return m_bOwnerP; }
	
	
};

