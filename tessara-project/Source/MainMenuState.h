/*
	File:		MainMenuState.h
	Author:		Mark Diaz, Daniel Habig
	Course:		SGP
	Purpose:	The Main Menu State class is the game state for the main menu.
*/

#pragma once
#include "IGameState.h"
#include "AnimationInformation.h"
#include "ParticleEffect.h"
#include "BitmapFont.h"

// Forward class declarations
//class CSGD_DirectInput;
//class CSGD_TextureManager;
//class FontBMP;

class CInputManager;
class CEventSystem;

class MainMenuState : public IGameState
{
public:
	// SINGLETON!
	static MainMenuState* GetInstance( void );
//--------------------------------------------------------------------------------------------------
	
	// IGameState Interface
	virtual void	Enter ( void );					// load resources
	virtual void	Exit  ( void );					// unload resources

	virtual bool	Input ( void );					// handle user input
	virtual void	Update( float fElapsedTime );	// update game entities
	virtual void	Render( void );					// render game entities

//--------------------------------------------------------------------------------------------------

private:
	// SINGLETON!
	AnimationInformation animInfo;
	MainMenuState( void );								// ctor
	virtual ~MainMenuState( void );						// dtor
	MainMenuState( const MainMenuState& );				// copy ctor
	MainMenuState& operator= ( const MainMenuState& );	// assignment op

	CParticleEffect Rain;
	CParticleEffect Burst;
	bool			 m_bTransition;
	CInputManager *  m_pIM;
	CEventSystem  *  m_pMS;

	// Assets
	int						m_nSelectedIndex;
	int						m_nBackgroundImgID;
	int						m_nMenuButtonImgID;
	float					m_nAlphaOffset;
	int						m_nAlpha[6];
	bool					m_bFadingOut;
	float					m_fFadeTimer;
	bool					m_bFadingIn;
	bool					m_bRunning;
};

