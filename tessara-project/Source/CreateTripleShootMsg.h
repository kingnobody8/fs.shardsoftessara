/*
	Author: Jorge Juaneda
	File: CreateTripleShootMsg.h
*/

#pragma once
#include "message.h"
#include "Entity.h"

class CreateTripleShootMsg :
	public CMessage
{
private:
	int m_nPosX;
	int m_nPosY;
	int m_nDir;
	int m_nElement;
	float m_fVel;
	CEntity* m_pOwner;

public:
	CreateTripleShootMsg(float fVel, int nDir, int nElement, int nX, int nY, CEntity* pOwner);
	~CreateTripleShootMsg(void);

	int GetPosX() {return m_nPosX; }
	int GetPosY() { return m_nPosY; }
	int GetDirection() { return m_nDir; }
	int GetElement() {return m_nElement; }
	float GetVel() {return m_fVel; }
	CEntity* GetOwner() {return m_pOwner; }
};

