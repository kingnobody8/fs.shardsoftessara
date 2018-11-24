#pragma once
#include "message.h"
#include "IEntity.h"

class AddEntityToManager :
	public CMessage
{
private:
	IEntity* m_pEntity;

public:
	AddEntityToManager(IEntity* pEntity);
	~AddEntityToManager(void);

	IEntity* GetEntity() {return m_pEntity; }
};

