#include "AddItemMsg.h"


AddItemMsg::AddItemMsg(CItem* pItem, bool bRemove) : CMessage(MSG_ADDITEM)
{
	m_pItem = pItem;
	m_bRemove = bRemove;
}


AddItemMsg::~AddItemMsg(void)
{
}
