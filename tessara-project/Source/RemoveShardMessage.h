#pragma once
#include "message.h"
class RemoveShardMessage :
	public CMessage
{
private:
	int m_nShard;

public:

	RemoveShardMessage();
	~RemoveShardMessage();

	int GetAmount() { return m_nShard; }
};