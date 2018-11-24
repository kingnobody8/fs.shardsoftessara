#include "AddKeyMessage.h"


AddKeyMessage::AddKeyMessage() : CMessage(MSG_ADDKEY)
{
	m_nKey++;
}


AddKeyMessage::~AddKeyMessage(void)
{
}