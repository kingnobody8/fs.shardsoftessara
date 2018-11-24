/*
File:		MainMenuState.h
Author:		Mark Diaz, Daniel Habig
Course:		SGP
Purpose:	The Main Menu State class is the game state for the main menu.
*/

#include "MainMenuState.h"
#include "PlayState.h"
#include "ParticleEffect.h"
#include "CreditsState.h"
#include "EventSystem.h"
#include "InformationManagementState.h"
#include "LoadLevelMsg.h"
#include "MessageSystem.h"
#include "Level.h"

#include "AudioManager.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "InputManager.h"
#include "LoadGameState.h"
#include "Game.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include <ShlObj.h>
#include "../TinyXML/tinyxml.h"

// SINGLETON!
/*static*/ MainMenuState* MainMenuState::GetInstance( void )
{
	// Easy-instantiation: use a static variable
	static MainMenuState s_Instance;

	return &s_Instance;
}


// CONSTRUCTOR
MainMenuState::MainMenuState( void )
{
	m_pIM = nullptr;
	m_nBackgroundImgID = -1;
	m_nMenuButtonImgID = -1;

}

// DESTRUCTOR
MainMenuState::~MainMenuState( void )
{

}

// Enter
/*virtual*/ void	MainMenuState::Enter ( void )
{
	m_pIM = CInputManager::GetInstance();
	m_nBackgroundImgID = CSGD_TextureManager::GetInstance()->LoadTexture( _T("Resource/Graphics/BG.jpg") );
	m_nMenuButtonImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/menubutton2.png"));

	Rain.SetXPosition(205);
	Rain.SetYPosition(267);
	Rain.LoadFromFile( "Resource/Graphics/Particles/Sparkles.xml" );
	Rain.Initialize();

	Burst.SetXPosition(205);
	Burst.SetYPosition(267);
	Burst.LoadFromFile( "Resource/Graphics/Particles/Burst.xml" );

	m_nSelectedIndex = 1;
	m_bTransition = false;
	m_bFadingIn = true;
	m_fFadeTimer = 0.0f;
	m_nAlphaOffset = 255;
	m_bRunning = true;

	char path[MAX_PATH];
	LPWSTR wszPath = NULL;

	HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0,  0, &wszPath);

	size_t tempSize;
	wcstombs_s(&tempSize, path, wszPath, MAX_PATH);

	string pathtowrite = path;
	pathtowrite += "\\Shards of Tessera";
	TCHAR temp[MAX_PATH];

	CSTR_TO_WSTR(temp, MAX_PATH, pathtowrite.c_str());
	CreateDirectory(temp, 0);

	string FilePath;
	bool cont = false;

	for (int i = 0; i < 3; i++)
	{
		FilePath  = pathtowrite;

		if (i == 0)
			FilePath += "\\SaveSlot1.xml";
		else if (i == 1)
			FilePath += "\\SaveSlot2.xml";
		else
			FilePath += "\\SaveSlot3.xml";


		TiXmlDocument doc;
		doc.LoadFile( FilePath.c_str() );

		if ( !doc.Error() )
		{
			cont = true;
		}
	}

	if(cont)
	{
		m_nSelectedIndex = 2;
	}
}

// Exit
/*virtual*/ void	MainMenuState::Exit  ( void )
{
	if (m_nBackgroundImgID != -1)
	{
		CSGD_TextureManager::GetInstance()->UnloadTexture( m_nBackgroundImgID );
		m_nBackgroundImgID = -1;
	}
	if (m_nMenuButtonImgID != -1)
	{
		CSGD_TextureManager::GetInstance()->UnloadTexture( m_nMenuButtonImgID );
		m_nMenuButtonImgID = -1;
	}

	CAudioManager::GetInstance()->StopMus();
	CAudioManager::GetInstance()->UnloadAudio(MENU_AUDIO);
}

// Input
/*virtual*/ bool	MainMenuState::Input ( void )
{
	if (!m_bFadingOut)
	{
		if (m_pIM->CheckDown() )
		{
			++m_nSelectedIndex;
			if (m_nSelectedIndex > 6)
				m_nSelectedIndex = 1;

			CAudioManager::GetInstance()->PlaySFX(SFX_CURSOR);
		}

		if (m_pIM->CheckUp() )
		{
			--m_nSelectedIndex;
			if (m_nSelectedIndex < 1)
				m_nSelectedIndex = 6;

			CAudioManager::GetInstance()->PlaySFX(SFX_CURSOR);
		}

		if (m_pIM->CheckEnter() )
		{
			CAudioManager::GetInstance()->PlaySFX(SFX_CONFIRM);
			m_bTransition = true;
			m_bFadingOut = true;
			Burst.SetYPosition(217 + (m_nSelectedIndex * 50));
			Burst.Initialize();
		}
	}

	return m_bRunning;
}

// Update
/*virtual*/ void	MainMenuState::Update( float fElapsedTime )
{
	if (m_bFadingIn || m_bFadingOut)
	{
		m_fFadeTimer += fElapsedTime;
		if (m_bFadingIn)
			m_nAlphaOffset -= (125 * fElapsedTime);
		if (m_bFadingOut)
			m_nAlphaOffset += (125 * fElapsedTime);
	}

	if (m_fFadeTimer >= 2.0f)
	{
		m_fFadeTimer = 0.0f;

		if (m_bFadingIn)
		{
			m_nAlphaOffset = 0;
			m_bFadingIn = false;
		}
		if (m_bFadingOut)
		{
			m_nAlphaOffset = 255;
			m_bFadingOut = false;

			if (m_nSelectedIndex == 1)
			{
				m_bFadingIn = true;
				m_fFadeTimer = 0.0f;
				m_bTransition = false;
				CGame::GetInstance()->ChangeToNew( CLoadGameState::GetInstance() );
				CLoadGameState::GetInstance()->SetNewGame(true);
				return;
			}

			if (m_nSelectedIndex == 2)
			{
				m_bFadingIn = true;
				m_fFadeTimer = 0.0f;
				m_bTransition = false;
				CGame::GetInstance()->ChangeToNew( CLoadGameState::GetInstance() );
				CLoadGameState::GetInstance()->SetNewGame(false);
				return;
			}

			if (m_nSelectedIndex == 3)
			{
				// RUN TUTORIAL
				m_bFadingIn = true;
				m_fFadeTimer = 0.0f;
				m_bTransition = false;
				CGame::GetInstance()->ChangeToNew( CLoadGameState::GetInstance() );
				CGame::GetInstance()->ChangeToNew(PlayState::GetInstance());
				CLoadGameState::GetInstance()->SetNewGame(true);
				
				LevelManager::GetInstance()->LoadMapData(0);
				PlayState::GetInstance()->SetMapName("Tutorial");
				PlayState::GetInstance()->SetPortal(0);
				CGame::GetInstance()->SetSaveSlot("");

				/*Portal port;
				port.map = "Tutorial";
				port.port = 0;

				LoadLevelMsg* pMsg = new LoadLevelMsg( port );
				CMessageSystem::GetInstace()->SendMsg( pMsg );
				pMsg = nullptr;*/

			}

			if (m_nSelectedIndex == 4)
			{
				m_bFadingIn = true;
				m_fFadeTimer = 0.0f;
				m_bTransition = false;
				CGame::GetInstance()->ChangeToNew( InformationManagementState::GetInstance() );
				return;
			}
			if (m_nSelectedIndex == 5)
			{
				m_bFadingIn = true;
				m_fFadeTimer = 0.0f;
				m_bTransition = false;
				CGame::GetInstance()->ChangeToNew( CCreditsState::GetInstance() );
				return;

			}
			if (m_nSelectedIndex == 6)
			{
				m_bRunning = false;
				return;
			}
		}
	}

	if (!m_bTransition)
	{
		Rain.SetYPosition( 217 + (m_nSelectedIndex * 50));
		Rain.Update( fElapsedTime );
	}

	if (m_bTransition)
		Burst.Update( fElapsedTime);

	CEventSystem::GetInstance()->ProcessEvents();
}

// Render
/*virtual*/ void	MainMenuState::Render( void )
{
	CSGD_TextureManager::GetInstance()->Draw(m_nBackgroundImgID,0,0,1.0f,1.0f,0,0,0,0,D3DCOLOR_ARGB(255,255,255,255));

	for (int i = 0; i < 6; ++i)
	{
		m_nAlpha[i] = 100 - (int)m_nAlphaOffset;

		if (i == m_nSelectedIndex - 1)
			m_nAlpha[i] = 200 - (int)m_nAlphaOffset;

		if (m_nAlpha[i] < 0 )
			m_nAlpha[i] = 0;
		if (m_nAlpha[i] > 255)
			m_nAlpha[i] = 255;

		CSGD_TextureManager::GetInstance()->Draw(m_nMenuButtonImgID,90,250 + (i*50),1.2f,0.65f,0,0,0,0,D3DCOLOR_ARGB(m_nAlpha[i],255,255,255));	
	}

	CBitmapFont::GetInstance()->Draw("New Game", 135, 265, 0.5f, D3DCOLOR_ARGB(m_nAlpha[0], 255, 255, 255));
	CBitmapFont::GetInstance()->Draw("Continue", 135, 315, 0.5f, D3DCOLOR_ARGB(m_nAlpha[1], 255, 255, 255));
	CBitmapFont::GetInstance()->Draw("Tutorial", 135, 365, 0.5f, D3DCOLOR_ARGB(m_nAlpha[2], 255, 255, 255));
	CBitmapFont::GetInstance()->Draw("Options", 135, 415, 0.5f, D3DCOLOR_ARGB(m_nAlpha[3], 255, 255, 255));
	CBitmapFont::GetInstance()->Draw("Credits", 135, 465, 0.5f, D3DCOLOR_ARGB(m_nAlpha[4], 255, 255, 255));
	CBitmapFont::GetInstance()->Draw("Exit", 135, 515, 0.5f, D3DCOLOR_ARGB(m_nAlpha[5], 255, 255, 255));

	if (!m_bTransition)
		Rain.Render();
	else 
		Burst.Render();
}