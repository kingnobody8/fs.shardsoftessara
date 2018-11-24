#include "RemoveKeyMessage.h"


RemoveKeyMessage::RemoveKeyMessage() : CMessage(MSG_REMOVEKEY)
{
	m_nKey++;
}


RemoveKeyMessage::~RemoveKeyMessage(void)
{
}