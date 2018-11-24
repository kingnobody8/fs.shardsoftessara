#pragma once
#include "message.h"
class AddCurrencyMessage :
	public CMessage
{
private:
	int m_nCur;

public:

	AddCurrencyMessage( int nCur);
	~AddCurrencyMessage(void);

	int GetAmount() {return m_nCur; }
};

