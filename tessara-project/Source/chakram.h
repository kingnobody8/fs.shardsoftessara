/*
	Author: Jorge Juaneda
	File: chakram.h
*/


#pragma once
#include "projectile.h"

class CParticleEffect;

class chakram :
	public CProjectile
{
private:

	bool m_bCollide;

	int m_nNumOfBounce;
	int m_nBouncLimit;
	CParticleEffect* ChakramEffect;

	int m_nDir;

public:
	chakram(void);
	~chakram(void);

	virtual void Render();
	virtual void Update( float deltaTime );
	virtual bool CheckCollision( IEntity* pOther);

	void TestCol();
	void SetBounceLimit( int nLimit ) { m_nBouncLimit = nLimit; }

	void ResolveCollision( IEntity* pOther);
};

