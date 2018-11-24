/*
File:		Power.h
Author:		Daniel Habig
Course:		SGP
Purpose:	The power class acts as an entity that can be charged with lightning to power doors
*/

#pragma once
#include "Entity.h"
#include "LevelManager.h"
#include "Door.h"

#define APT 5.0f
class CParticleEffect;

class Power : public CEntity
{
private:
	Door* m_pDoor;
	bool active;
	float time;
	CParticleEffect* ActiveEffect;

public:
	Power(void);
	~Power(void);

	virtual void Update( float deltaTime );
	virtual void Render( void );
	virtual bool CheckCollision( IEntity* pOther );
	virtual RECT GetRect ();

	void Activate(void);
	void Deactivate(void);
	void SetDoor(Door* door) { this->m_pDoor = door; }



};

