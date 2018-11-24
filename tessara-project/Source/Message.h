/*
	Author: Jorge Juaneda
	File: MessageSystem.cpp
	Purpose: Base class for all messages
*/

#pragma once

# include "..//SGD Wrappers/IMessage.h"

// Enumeration of message types
enum	MsgType		{ MSG_TEST, MSG_DELETEPROJ, MSG_DELETEENT, MSG_ADDCURRENCY, MSG_LOADLEVEL, MSG_CREATEPROJECTILE, MSG_ADDKEY, MSG_REMOVEKEY, MSG_ADDSHARD, MSG_TREIPLESHOOT, MSG_REMOVESHARD, MSG_ADDITEM, MSG_ADDENTITYTOMANAGER};


class CMessage : public IMessage
{

public:

	CMessage( MsgType id )				{	m_eMessageID = id;		}
	virtual ~CMessage(void)				{ }

	// IMessage Interface:
	virtual MSGID	GetMessageID( void ){	return m_eMessageID;	}

private:

	MsgType		m_eMessageID;
};

