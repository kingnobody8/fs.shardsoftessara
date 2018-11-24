/*
	Author: Mark Diaz
	File: DeleteEntityMessage.h
*/

#pragma once
#include "message.h"

#include "IEntity.h"

class CDeleteEntityMessage :
	public CMessage
{

private:

	IEntity* m_pEntity;

public:
	CDeleteEntityMessage( IEntity* pEnt);
	~CDeleteEntityMessage(void);

	IEntity* GetEntity() { return m_pEntity; }
};