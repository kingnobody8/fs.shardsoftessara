//////////////////////////////////////////////////////////////////////////
//	File	:	"IListener.h"
//
//	Author	:	David Brown (DB)
//
//	Purpose	:	Base class for objects registered with CSGD_EventSystem to
//				handle event broadcasts.
///////////////////////////////////////////////////////////////////////////
#pragma once

// Forward declaration does not require header includes.
class CEvent;

// ABSTRACT BASE CLASS
class IListener
{
public:
	IListener(void) {}
	virtual ~IListener(void) {}

	//	Blue print function - MUST be defined in the derived class.
	//	Mini-proc function for whenever this object receives an event
	virtual void HandleEvent(CEvent* pEvent) = 0;
};
