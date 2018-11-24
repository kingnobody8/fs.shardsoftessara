//////////////////////////////////////////////////////////////////////////
//	File	:	"CSGD_EventSystem.h"
//
//	Author	:	David Brown (DB)
//
//	Purpose	:	Handles broadcasting events to registered listeners.
///////////////////////////////////////////////////////////////////////////
#pragma once

#include <list>
#include <map>
using std::multimap;
using std::pair;
using std::list;

#include "CEvent.h"
#include "IListener.h"

class CSGD_EventSystem
{
	private:
		//	Our Database, this will contain clients that can "listen" for events.
		multimap<EVENTID, IListener*>	m_ClientDatabase;
	
		//	Events waiting to be processed.
		list<CEvent>		m_CurrentEvents;

		//	Utility functions - private because they will only ever be used by this class.
		//
		//	Finds the event in the database and then calls each of the registered client's 
		//	EventHandler functions.
		void DispatchEvent(CEvent* pEvent);
		bool AlreadyRegistered(EVENTID eventID, IListener* pClient);

		CSGD_EventSystem() {}
		CSGD_EventSystem(const CSGD_EventSystem&);
		CSGD_EventSystem& operator=(const CSGD_EventSystem&);

		~CSGD_EventSystem() {}

	public:

		static CSGD_EventSystem* GetInstance(void)
		{
			static CSGD_EventSystem instance;
			return &instance;
		}

		//	This adds a client to the database.  This will make new "buckets" if necessary and then
		//	add the client to that given bucket.
		void RegisterClient(EVENTID eventID, IListener* pClient);

		//	Unregisters the client for the specified event only
		void UnregisterClient(EVENTID eventID, IListener* pClient);

		//	Removes the client from the database entirely
		void UnregisterClientAll(IListener* pClient);

		//	Checks if an event is already queued.
		bool HasEventTriggered( EVENTID eventID );

		//	Sends an event to be processed later on.
		void SendEvent( EVENTID eventID, void* pData = NULL, void* pDestination = NULL, void* pSender = NULL );

		//	Sends an event to be processed later on if it has not already been triggered.
		void SendUniqueEvent( EVENTID eventID, void* pData = NULL );
		
		//	Sends an event to be processed immediately.
		void SendEventNow( EVENTID eventID, void* pData = NULL, void* pDestination = NULL, void* pSender = NULL );

		//	Processes all events in our event list.
		void ProcessEvents(void);

		//	Clears all pending events
		void ClearEvents(void);

		//	Unregisters all objects
		void ShutdownEventSystem(void);
};