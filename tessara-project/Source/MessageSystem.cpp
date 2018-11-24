/*
	Author: Jorge Juaneda
	File: MessageSystem.cpp
*/

#include "MessageSystem.h"


CMessageSystem * CMessageSystem::m_pInstance = nullptr;

/*static*/ CMessageSystem*  CMessageSystem::GetInstace( )
{
	if ( m_pInstance == nullptr)
	{
		m_pInstance = new CMessageSystem();
	}
	
	return m_pInstance;
}



/*static*/ void CMessageSystem::DeleteInstance ()
{
	delete m_pInstance;

	m_pInstance = nullptr;
}

//initialize the Message system 
void CMessageSystem::InitMeassageSys( MESSAGEPROC pMesgProc)
{
	if (pMesgProc != nullptr)
			m_pMesgProc = pMesgProc;
}

//send message to MsgProc
void CMessageSystem::SendMsg( IMessage * Msg)
{
	if (Msg != nullptr)
		m_qMsgQueue.push(Msg);

}

//process all messgaes in queue
void CMessageSystem::ProcessMessages( )
{
	if (m_pMesgProc != nullptr)
	{

		while(!m_qMsgQueue.empty())
		{
			m_pMesgProc(m_qMsgQueue.front());
			delete m_qMsgQueue.front();
			m_qMsgQueue.pop();
		}

	}

}



void CMessageSystem::ClearMessageQ ( )
{
	while (!m_qMsgQueue.empty())
	{	
		delete m_qMsgQueue.front();
		m_qMsgQueue.pop();
	}
}



void CMessageSystem::ShutdownSystem( )
{
	ClearMessageQ();
	DeleteInstance();
}


 CMessageSystem::~CMessageSystem(void)
{

}