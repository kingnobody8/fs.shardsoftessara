//////////////////////////////////////////////////////////////////////////
//	File	:	"IMessage.h"
//
//	Author	:	Doug Monroe (DM)
//
//	Purpose	:	Base class for objects sent through CSGD_MessageSystem to
//				be handled by the registered message proc.
///////////////////////////////////////////////////////////////////////////
#pragma once

typedef		int		MSGID;


// ABSTRACT BASE CLASS
class IMessage
{
public:
	IMessage( void ) {}
	virtual ~IMessage( void ) = 0 {}

	virtual MSGID	GetMessageID( void )	= 0 {}

private:
	IMessage( const IMessage& );
	IMessage& operator= ( const IMessage& );
};
