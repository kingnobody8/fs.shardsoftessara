/*
	File:		CreditsState.h
	Author:		Mark Diaz
	Course:		SGP
	Purpose:	The CCreditsState Class shows the credits
*/

#include "CreditsState.h"
#include "BitmapFont.h"
#include "Game.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "InputManager.h"
#include "AudioManager.h"


// Este clase esta un singleton
/*static*/ CCreditsState* CCreditsState::GetInstance( void )
{
	// Easy-instantiation: use a static variable
	static CCreditsState s_Instance;

	return &s_Instance;
}

CCreditsState::CCreditsState(void)
{
	m_nBackgroundImgID = -1;	
	m_pBF = nullptr;
	m_fTimer		 = 0;
	m_nAlpha		 = 0;
	m_nRender		 = 1;
	m_nAlphaIncrement = 255/2;
}


CCreditsState::~CCreditsState(void)
{
}

/*virtual*/ void	CCreditsState::Enter ( void )
{
	m_pBF = CBitmapFont::GetInstance();

	m_nBackgroundImgID = CSGD_TextureManager::GetInstance()->LoadTexture( _T("Resource/Graphics/BG.jpg"));
	m_fTimer		 = 0;
	m_nAlpha		 = 0;
	m_nRender		 = 1;
	m_nAlphaIncrement = 255/2;
	CAudioManager::GetInstance()->PlayCredits();
}

/*virtual*/ void	CCreditsState::Exit  ( void )
{
	if (m_nBackgroundImgID != -1)
	{
		CSGD_TextureManager::GetInstance()->UnloadTexture( m_nBackgroundImgID );
		m_nBackgroundImgID = -1;
	}
	
	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-001
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	// Remove the stop playing music call, as music is the same as in main menu
	// CAudioManager::GetInstance()->StopMus();
	
	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-001
	//////////////////////////////////////////////////////////////////////////
}

/*virtual*/ bool	CCreditsState::Input ( void )
{
	if (CInputManager::GetInstance()->CheckEnter())
	{
		m_fTimer = 0.0f;
		++m_nRender;
	}

	return true;
}

/*virtual*/ void	CCreditsState::Update( float fElapsedTime )
{
	if (m_nRender > 6)
		{
			CGame::GetInstance()->ChangeToPrev();
			return;
		}

	m_fTimer += fElapsedTime;
	if (m_fTimer < 2.0f)
		if (m_nAlpha + m_nAlphaIncrement * fElapsedTime <= 255)
			m_nAlpha += (int)(m_nAlphaIncrement * fElapsedTime);

	if (m_fTimer > 3.25f)
		if (m_nAlpha - m_nAlphaIncrement * fElapsedTime >= 0)
			m_nAlpha -= (int)(m_nAlphaIncrement * fElapsedTime);

	if (m_fTimer > 5.25f)
	{
		m_fTimer = 0.0f;
		++m_nRender;
	}

}

/*virtual*/ void	CCreditsState::Render( void )
{
	switch (m_nRender)
	{
	case 1:
		m_pBF->Draw("Executive Producer", 150,180,1.0f,D3DCOLOR_ARGB( m_nAlpha, 255, 255, 255 ));
		m_pBF->Draw(" John Oleske", 250,250,1.0f,D3DCOLOR_ARGB( m_nAlpha, 255, 255, 255 ));
		break;
	case 2:
		m_pBF->Draw("Associate Producer", 150,180,1.0f,D3DCOLOR_ARGB( m_nAlpha, 255, 255, 255 ));
		m_pBF->Draw("Shawn Paris", 250,250,1.0f,D3DCOLOR_ARGB( m_nAlpha, 255, 255, 255 ));
		break;
	case 3:
		m_pBF->Draw("Programmers", 220,180,1.0f,D3DCOLOR_ARGB( m_nAlpha, 255, 255, 255 ));
		m_pBF->Draw("Daniel Habig", 240,250,1.0f,D3DCOLOR_ARGB( m_nAlpha, 255, 255, 255 ));
		m_pBF->Draw("Benjamin 'Evan' Cowles", 120,300,1.0f,D3DCOLOR_ARGB( m_nAlpha, 255, 255, 255 ));
		m_pBF->Draw("Jorge Juaneda", 210,350,1.0f,D3DCOLOR_ARGB( m_nAlpha, 255, 255, 255 ));
		m_pBF->Draw("Mark Diaz", 250,400,1.0f,D3DCOLOR_ARGB( m_nAlpha, 255, 255, 255 ));
		break;
	case 4:
		m_pBF->Draw(" Artwork By", 220,180,1.0f,D3DCOLOR_ARGB( m_nAlpha, 255, 255, 255 ));
		m_pBF->Draw(" Maher Sagrillo", 200,250,1.0f,D3DCOLOR_ARGB( m_nAlpha, 255, 255, 255 ));
		m_pBF->Draw("Samuel Slover", 210,300,1.0f,D3DCOLOR_ARGB( m_nAlpha, 255, 255, 255 ));
		m_pBF->Draw(" Devon Welch", 220,350,1.0f,D3DCOLOR_ARGB( m_nAlpha, 255, 255, 255 ));
		break;
	case 5:
		m_pBF->Draw(" Music By",		250,180,1.0f,D3DCOLOR_ARGB( m_nAlpha, 255, 255, 255 ));
		m_pBF->Draw(" Tyler Habig",		230,250,1.0f,D3DCOLOR_ARGB( m_nAlpha, 255, 255, 255 ));
		break;
	case 6:
		m_pBF->Draw("Thank you for playing!", 120,210,1.1f,D3DCOLOR_ARGB( m_nAlpha, 255, 255, 255 ));
		
	}

}


