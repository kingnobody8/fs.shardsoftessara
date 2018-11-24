#pragma once
#include "message.h"
class AddKeyMessage :
	public CMessage
{
private:
	int m_nKey;

public:

	AddKeyMessage();
	~AddKeyMessage();

	int GetAmount() { return m_nKey; }
};