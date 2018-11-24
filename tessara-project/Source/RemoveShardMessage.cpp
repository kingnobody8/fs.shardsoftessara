#include "RemoveShardMessage.h"


RemoveShardMessage::RemoveShardMessage() : CMessage(MSG_REMOVESHARD)
{
	m_nShard++;
}


RemoveShardMessage::~RemoveShardMessage(void)
{
}