/*
	Author: Jorge Juaneda
	File: CreateTripleShootMsg.cpp
*/

#include "CreateTripleShootMsg.h"


CreateTripleShootMsg::CreateTripleShootMsg(float fVel, int nDir, int nElement, int nX, int nY, CEntity* pOwner) : CMessage(MSG_TREIPLESHOOT)
{
	m_nDir = nDir;
	m_fVel = fVel;
	m_nPosX = nX;
	m_nPosY = nY;
	m_nElement = nElement;
	m_pOwner = pOwner;
	
}


CreateTripleShootMsg::~CreateTripleShootMsg(void)
{
}
