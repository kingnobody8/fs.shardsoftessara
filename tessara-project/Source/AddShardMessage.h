#pragma once
#include "message.h"
class AddShardMessage :
	public CMessage
{
private:
	int m_nShard;

public:

	AddShardMessage();
	~AddShardMessage();

	int GetAmount() { return m_nShard; }
};