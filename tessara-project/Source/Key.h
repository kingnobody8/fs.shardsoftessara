/*
	File:		Key.h
	Author:		Benjamin "Evan" Cowles
*/

#pragma once
#include "Entity.h"
#include "LevelManager.h"

class CKey : public CEntity
{
private:
	bool picked;

public:
	CKey();
	~CKey();

	virtual void Update(float deltaTime);
	virtual void Render();
	virtual bool CheckCollision(IEntity* pOther);

	void Create(Area loc);
	RECT GetRect();
};