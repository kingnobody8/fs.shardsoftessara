#include "AddCurrencyMessage.h"


AddCurrencyMessage::AddCurrencyMessage(int nCur) : CMessage(MSG_ADDCURRENCY)
{
	m_nCur = nCur;
}


AddCurrencyMessage::~AddCurrencyMessage(void)
{
}
