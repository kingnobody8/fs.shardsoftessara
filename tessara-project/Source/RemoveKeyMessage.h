#pragma once
#include "message.h"
class RemoveKeyMessage :
	public CMessage
{
private:
	int m_nKey;

public:

	RemoveKeyMessage();
	~RemoveKeyMessage();

	int GetAmount() { return m_nKey; }
};