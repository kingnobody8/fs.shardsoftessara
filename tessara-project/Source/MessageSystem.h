/*
	Author: Jorge Juaneda
	File: MessageSystem.h
*/

#pragma once

#include "..//SGD Wrappers/IMessage.h"

#include <queue>
using namespace std;


typedef void (*MESSAGEPROC)(IMessage*);

class CMessageSystem
{

public:

	static CMessageSystem*  GetInstace( );
	static void  DeleteInstance ();
	//initialize the Message system 
	void InitMeassageSys( MESSAGEPROC pMesgProc);

	//send message to MsgProc
	void SendMsg( IMessage * Msg);

	void ProcessMessages( );

	void ClearMessageQ ( );

	void ShutdownSystem( );

private:

	static CMessageSystem * m_pInstance;

	CMessageSystem () { m_pMesgProc = nullptr; }
	CMessageSystem ( const CMessageSystem & );
	CMessageSystem operator = ( CMessageSystem& );
	~CMessageSystem(void);


	//queue of messages
	queue<IMessage*> m_qMsgQueue;
	//function handles the messages 
	MESSAGEPROC m_pMesgProc;


};

