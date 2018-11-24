/*
	Author: Jorge Juaneda
	File: EventSystem.cpp
*/


#include "EventSystem.h"


//register client for event and check that is not registered
void CEventSystem::RegisterClient(EVENTID eventID, IListener* pClient)
{
	if (pClient != nullptr && !AlreadyReg(eventID, pClient))
		m_Clients.insert( make_pair(eventID, pClient) );		
}



void CEventSystem::UnregisterClient(EVENTID eventID, IListener* pClient)
{
	pair< multimap<EVENTID, IListener*>::iterator, multimap<EVENTID, IListener*>::iterator > range;

	range = m_Clients.equal_range(eventID);

	for(multimap<EVENTID, IListener*>::iterator mmIter = range.first; mmIter != range.second; mmIter++)
	{
		if((*mmIter).second == pClient)
		{
			mmIter = m_Clients.erase(mmIter);

			break;
		}
	}


}


//unregister client from all events
void CEventSystem::UnregisterClientAll(IListener* pClient)
{
	multimap< string, IListener *>::iterator mmIter = m_Clients.begin();
	while(mmIter != m_Clients.end())
	{
		if((*mmIter).second == pClient)
			mmIter = m_Clients.erase(mmIter);
		else
			mmIter++;
	}


}




void CEventSystem::SendEventNow( EVENTID eventID, void* pData, void* pDestination, void* pSender )
{
	CEvent newEvent(eventID, pData, pDestination, pSender);
	DispatchEvent( &newEvent );

}


void CEventSystem::SendUniqueEvent( EVENTID eventID, void* pData )
{
	if( HasEventTriggered( eventID ) )
		return;

	SendEvent( eventID, pData );

}



void CEventSystem::SendEvent( EVENTID eventID, void* pData, void* pDestination, void* pSender )
{
	CEvent newEvent(eventID, pData, pDestination, pSender);
	m_CurrentEvents.push_back(newEvent);

}


bool CEventSystem::HasEventTriggered( EVENTID eventID )
{	
	list<CEvent>::iterator iter = m_CurrentEvents.begin();
	for( ; iter != m_CurrentEvents.end(); ++iter )
	{

		if( (*iter).GetEventID() == eventID )
			return true;

	}

	return false;
}


void CEventSystem::ClearEvents(void)
{
	m_CurrentEvents.clear();
}


void CEventSystem::ProcessEvents(void)
{
	while(m_CurrentEvents.size())
	{
		DispatchEvent(&m_CurrentEvents.front());
		m_CurrentEvents.pop_front();
	}


}




void CEventSystem::Shutdown(void)
{
	m_Clients.clear();
}


void CEventSystem::DispatchEvent(CEvent* pEvent)
{
	pair<multimap<EVENTID, IListener*>::iterator, multimap<EVENTID, IListener*>::iterator> range;
	range = m_Clients.equal_range(pEvent->GetEventID());


	for(multimap<EVENTID, IListener*>::iterator mmIter = range.first; mmIter != range.second; mmIter++)
		(*mmIter).second->HandleEvent(pEvent);

}



bool CEventSystem::AlreadyReg(EVENTID eventID, IListener* pClient)
{
	bool bIsAlreadyReg = false;

	pair<multimap<EVENTID, IListener*>::iterator, multimap<EVENTID, IListener*>::iterator> range;
	range = m_Clients.equal_range(eventID);


	for(multimap<EVENTID, IListener*>::iterator mmIter = range.first; mmIter != range.second; mmIter++)
	{
		if((*mmIter).second == pClient)
		{
			bIsAlreadyReg = true;
			break;

		}
	}


	return bIsAlreadyReg;
}