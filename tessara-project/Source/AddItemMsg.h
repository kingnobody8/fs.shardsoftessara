#pragma once
#include "message.h"
#include "Item.h"

class AddItemMsg :
	public CMessage
{
private:
	CItem* m_pItem;
	bool m_bRemove;
public:
	AddItemMsg(CItem* pItem, bool bRemove);
	~AddItemMsg(void);
	
	CItem* GetItem() {return m_pItem;}
	bool GetRemove() {return m_bRemove; }
};

