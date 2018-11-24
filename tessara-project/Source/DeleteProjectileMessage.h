/*
	Author: Jorge Juaneda
	File: DeleteProjectileMessage.h
*/

#pragma once
#include "message.h"

#include "Projectile.h"

class CDeleteProjectileMessage :
	public CMessage
{

private:



	CProjectile* m_pProjectile;


public:
	CDeleteProjectileMessage( CProjectile* pProj);
	~CDeleteProjectileMessage(void);

	CProjectile* GetProjectile() { return m_pProjectile; }
};

