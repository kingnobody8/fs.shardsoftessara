#pragma once
#include "projectile.h"


class CLightning :
	public CProjectile
{
private:

	bool m_bCollided;
	int m_nStunChance;

public:
	CLightning(void);
	~CLightning(void);

	void Render();
	void Update( float deltaTime );
	bool CheckCollision( IEntity* pOther);
	RECT GetRect ();
	void SetStunChance( int nValue ) { m_nStunChance = nValue; }

	void SetProjectile(float fVel, int nDirection, int nX, int nY);

};

