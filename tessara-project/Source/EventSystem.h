/*
	Author: Jorge Juaneda
	File: EventSystem.h
*/


#pragma once

#include <map>
#include <list>

using std::pair;
using std::list;

using std::multimap;

#include "..\\SGD Wrappers\IListener.h"
#include "..\\SGD Wrappers\CEvent.h"


class CEventSystem
{
public:

	static CEventSystem* GetInstance(void)
	{
		static CEventSystem instance;
		return &instance;
	}


	void RegisterClient(EVENTID eventID, IListener* pClient);
	void UnregisterClient(EVENTID eventID, IListener* pClient);	
	void UnregisterClientAll(IListener* pClient);
	void SendEventNow( EVENTID eventID, void* pData = NULL, void* pDestination = NULL, void* pSender = NULL );	
	void SendEvent( EVENTID eventID, void* pData = NULL, void* pDestination = NULL, void* pSender = NULL );
	void SendUniqueEvent( EVENTID eventID, void* pData = NULL );
	bool HasEventTriggered( EVENTID eventID );
	void ClearEvents(void);
	void ProcessEvents(void);
	void Shutdown(void);


private:

	list<CEvent>		m_CurrentEvents;
	multimap<EVENTID, IListener*>	m_Clients;

	void DispatchEvent(CEvent* pEvent);
	bool AlreadyReg(EVENTID eventID, IListener* pClient);

	CEventSystem() {}
	CEventSystem& operator=(const CEventSystem&);
	CEventSystem(const CEventSystem&);
	~CEventSystem() {}

};

