#include "AddShardMessage.h"


AddShardMessage::AddShardMessage() : CMessage(MSG_ADDSHARD)
{
	m_nShard++;
}


AddShardMessage::~AddShardMessage(void)
{
}