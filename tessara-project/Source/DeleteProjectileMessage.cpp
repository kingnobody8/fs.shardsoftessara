/*
	Author: Jorge Juaneda
	File: DeleteProjectileMessage.cpp
*/

#include "DeleteProjectileMessage.h"


CDeleteProjectileMessage::CDeleteProjectileMessage(CProjectile* pProj) : CMessage(MSG_DELETEPROJ)
{
	m_pProjectile = pProj;

}


CDeleteProjectileMessage::~CDeleteProjectileMessage(void)
{
}
