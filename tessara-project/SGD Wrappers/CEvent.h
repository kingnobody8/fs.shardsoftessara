//////////////////////////////////////////////////////////////////////////
//	File	:	"CEvent.h"
//
//	Author	:	David Brown (DB)
//
//	Purpose	:	Events get broadcast to registered listeners by the
//				CSGD_EventSystem. 
//
//				Each event type should have a unique EVENTID.
//				Events can have any type of data attached using the void*.
///////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>
using std::string;

typedef string EVENTID;		//	could be an int or enum


class CEvent
{
private:
	EVENTID			m_EventID;		//	The event's id type.
	void*			m_pParam;		//	Store any variable type (but only one)
	void*			m_pDestination;	// Intended listener
	void*			m_pSender;		// Object that sent the event

public:
	CEvent(EVENTID eventID, void* pParam, void* pDestination, void* pSender )
	{
		m_EventID		= eventID;
		m_pParam		= pParam;
		m_pDestination	= pDestination;
		m_pSender		= pSender;
	}

	~CEvent() {}

	//	Accessors:
	inline EVENTID  GetEventID		( void )	{	return m_EventID;		}
	inline void*	GetParam		( void )	{	return m_pParam;		}
	inline void*	GetDestination	( void )	{	return m_pDestination;	}
	inline void*	GetSender		( void )	{	return m_pSender;		}
};