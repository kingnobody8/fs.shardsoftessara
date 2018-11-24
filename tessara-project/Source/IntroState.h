/*
	File:		IntroState.h
	Author:		Mark Diaz
	Course:		SGP 1304
	Purpose:	The CIntroState class shows the full sail and
				studio logo, then switches to the Main Menu State
*/

#pragma once
#include "igamestate.h"

class CInputManager;
class CSGD_TextureManager;

class CIntroState :
	public IGameState
{
public:
	// SINGLETON!
	static CIntroState* GetInstance( void );

	// IGameState Interface
	virtual void	Enter ( void );					// load resources
	virtual void	Exit  ( void );					// unload resources

	virtual bool	Input ( void );					// handle user input
	virtual void	Update( float fElapsedTime );	// update options entities
	virtual void	Render( void );					// render options entities

private:
	CIntroState(void);
	virtual ~CIntroState(void);

	CIntroState( const CIntroState& );				
	CIntroState& operator= ( const CIntroState& );	

	// SGD Wrappers
	CInputManager*			m_pIM;
	CSGD_TextureManager*	m_pTM;

	// Assets
	int						m_nFullSailImgID;
	int						m_nLogoImgID;
	int						m_nRender;
	int						m_nAlphaIncrement;
	float					m_fTimer;
	int						m_nAlpha;
};

