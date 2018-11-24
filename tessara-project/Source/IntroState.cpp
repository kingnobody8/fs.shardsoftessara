/*
	File:		IntroState.cpp
	Author:		Mark Diaz
	Course:		SGD 1302
	Purpose:	The CIntroState class shows the full sail and
				studio logo, then switches to the Main Menu State
*/

#include "IntroState.h"
#include "MainMenuState.h"
#include "Game.h"
#include "InputManager.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "AudioManager.h"


// This class is a singleton.
/*static*/ CIntroState* CIntroState::GetInstance( void )
{
	static CIntroState s_Instance;
	return &s_Instance;
}

CIntroState::CIntroState(void)
{
	CAudioManager::GetInstance()->LoadAudio(MENU_AUDIO);
	m_pIM		= nullptr;
	m_pTM		= nullptr;

	m_nFullSailImgID = -1;
	m_nLogoImgID	 = -1;
	m_fTimer		 = 0;
	m_nAlpha		 = 0;
	m_nAlphaIncrement = 255/2;
	
}


CIntroState::~CIntroState(void)
{
}

/*virtual*/ void	CIntroState::Enter ( void )
{
	// Store my wrappers
	m_pIM	= CInputManager::GetInstance();
	m_pTM	= CSGD_TextureManager::GetInstance();

	// Load my textures
	m_nFullSailImgID = m_pTM->LoadTexture(_T("Resource/Graphics/Full_Sail_Logo.png"), D3DCOLOR_ARGB(255,255,255,255));
	m_nLogoImgID = m_pTM->LoadTexture(_T("Resource/Graphics/4DStudios.jpg"));

	m_nRender = m_nFullSailImgID;
	CAudioManager::GetInstance()->PlayMusic(MUSIC_MENU);
}

/*virtual*/ void	CIntroState::Exit  ( void )
{
	// Unload images
	if (m_nFullSailImgID != -1)
	{
		m_pTM->UnloadTexture(m_nFullSailImgID);
		m_nFullSailImgID = -1;
	}
	if (m_nLogoImgID != -1)
	{
		m_pTM->UnloadTexture(m_nLogoImgID);
		m_nLogoImgID = -1;
	}
}

/*virtual*/ bool	CIntroState::Input ( void )
{
	if (m_pIM->CheckEnter())
	{
		CGame::GetInstance()->ChangeState(MainMenuState::GetInstance());
		return true;
	}

	return true;
}

/*virtual*/ void	CIntroState::Update( float fElapsedTime )
{
	m_fTimer += fElapsedTime;
	if (m_fTimer < 2.0f)
		if (m_nAlpha + m_nAlphaIncrement * fElapsedTime <= 255)
			m_nAlpha += (int)(m_nAlphaIncrement * fElapsedTime);

	if (m_fTimer > 3.0f)
		if (m_nAlpha - m_nAlphaIncrement * fElapsedTime >= 0)
			m_nAlpha -= (int)(m_nAlphaIncrement * fElapsedTime);

	if (m_fTimer > 5.0f && m_nRender != m_nLogoImgID)
	{
		m_fTimer = 0.0f;
		m_nRender = m_nLogoImgID;
	}
	else if (m_nLogoImgID == m_nRender && m_fTimer > 5.0f)
		CGame::GetInstance()->ChangeState(MainMenuState::GetInstance());

}

/*virtual*/ void	CIntroState::Render( void )
{
	RECT rRect;
	rRect.top = 0;
	rRect.left = 0;
	rRect.bottom = 600;
	rRect.right = 800;

	m_pTM->Draw( m_nRender,0,0, 1.0f, 1.0f, &rRect,0,0,0,D3DCOLOR_ARGB( m_nAlpha, 255, 255, 255 ));

}