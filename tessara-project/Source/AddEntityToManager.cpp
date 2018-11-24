#include "AddEntityToManager.h"


AddEntityToManager::AddEntityToManager(IEntity* pEntity) : CMessage(MSG_ADDENTITYTOMANAGER)
{
	m_pEntity = pEntity;
}


AddEntityToManager::~AddEntityToManager(void)
{
}
