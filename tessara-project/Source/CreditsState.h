/*
	File:		CreditsState.h
	Author:		Mark Diaz
	Course:		SGP 1304
	Purpose:	The CCreditsState Class shows the credits
*/

#pragma once
#include "igamestate.h"

class CBitmapFont;

class CCreditsState : public IGameState
{
public:
	//This class is a singleton
	static CCreditsState* GetInstance( void );
//--------------------------------------------------------------------------------------------------
	
	// IGameState Interface
	virtual void	Enter ( void );					// load resources
	virtual void	Exit  ( void );					// unload resources

	virtual bool	Input ( void );					// handle user input
	virtual void	Update( float fElapsedTime );	// update game entities
	virtual void	Render( void );					// render game entities

//--------------------------------------------------------------------------------------------------

private:

	CCreditsState( void );								// ctor
	virtual ~CCreditsState( void );						// dtor
	CCreditsState( const CCreditsState& );				// copy ctor
	CCreditsState& operator= ( const CCreditsState& );	// assignment op

	CBitmapFont*	m_pBF;
	int				m_nBackgroundImgID;
	int				m_nRender;
	int				m_nAlphaIncrement;
	float			m_fTimer;
	int				m_nAlpha;

};