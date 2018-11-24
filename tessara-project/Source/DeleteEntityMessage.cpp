/*
	Author: Mark Diaz
	File: DeleteEntityMessage.cpp
*/

#include "DeleteEntityMessage.h"


CDeleteEntityMessage::CDeleteEntityMessage(IEntity* pEnt) : CMessage(MSG_DELETEENT)
{
	m_pEntity = pEnt;
}


CDeleteEntityMessage::~CDeleteEntityMessage(void)
{
}
