/*
File:		Game.cpp
Author:		Daniel Habig
Course:		SGP
Purpose:	The CGame class controls the gameplay & SGD Wrappers
*/

#include "Game.h"

//	Managers
#include "InputManager.h"
#include "EntityManager.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"

#include "../SGD Wrappers/CSGD_Direct3D.h"
/*#include "../SGD Wrappers/CSGD_DirectInput.h"

#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/SGD_Util.h"
#include <fstream>
*/
//	Fonts
//#include "FontBMP.h"
//	Game States
#include "MainMenuState.h"
#include "IntroState.h"
#include "PlayState.h"
#include "AudioManager.h"
#include "AnimationManager.h"
#include "ParticleManager.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "..\\TinyXML\tinyxml.h"
#include <cassert>
#include "ItemManager.h"
#include <ShlObj.h>
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"

// SINGLETON 
/*static*/ CGame* CGame::s_pInstance = nullptr;
/*static*/ CGame* CGame::GetInstance   ( void )
{
	if( s_pInstance == nullptr )
		s_pInstance = new CGame;

	return s_pInstance;
}
/*static*/ void   CGame::DeleteInstance( void )
{
	delete s_pInstance;
	s_pInstance = nullptr;
}


// Constructor
CGame::CGame( void )
{

	//	Set the data members to 'clean' values
	m_pIM				= nullptr;
	m_pEM				= nullptr;
	m_pCurrState		= nullptr;

	m_nScreenWidth		= 1;
	m_nScreenHeight		= 1;
	m_bIsWindowed		= true;
	m_dwCurrTime		= 0;
	m_lPrevStates.clear();


}

// Destructor
CGame::~CGame( void )
{

}

// Initialize
void CGame::Initialize( HWND* hWnd, HINSTANCE hInstance, int nWidth, int nHeight, bool bIsWindowed )
{
	mHWND = hWnd;
	// Access the Manager singletons
	m_pIM				= CInputManager::GetInstance();
	m_pD3D				= CSGD_Direct3D::GetInstance();
	m_pTM				= CSGD_TextureManager::GetInstance();	
	m_pEM				= CEntityManager::GetInstance();

	// Store the parameters
	m_nScreenWidth		= nWidth;
	m_nScreenHeight		= nHeight;
	m_bIsWindowed		= bIsWindowed;


	// Initialize the Managers
	m_pIM->Enter( *hWnd, hInstance );
	m_pD3D->InitDirect3D			( *hWnd, m_nScreenWidth, m_nScreenHeight, m_bIsWindowed, false );
	//m_pDI->InitDirectInput		( hWnd, hInstance, DI_KEYBOARD /*| DI_MOUSE | DI_JOYSTICKS*/ );
	m_pTM->InitTextureManager		( m_pD3D->GetDirect3DDevice(), m_pD3D->GetSprite() );
	// Initialize XAudio
	
	CAudioManager::GetInstance()->Initialize();	

	AnimationManager::GetInstance()->Initialize();
	CBitmapFont::GetInstance()->Initialize();
	CItemManager::GetInstance()->Initialize();

	// Store the current time (in milliseconds)
	m_dwCurrTime = GetTickCount();

	// Start in the MainMenuState
	ChangeToNew( CIntroState::GetInstance());

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

	FilePath = pathtowrite;

	FilePath += "\\Sound.xml";

	double m_fMusicV = 0;
	double m_fSoundV = 0;

	TiXmlDocument doc;
	doc.LoadFile( FilePath.c_str() );

	if (!doc.Error() )
	{

		TiXmlElement* pRoot = doc.RootElement();	

		pRoot->Attribute("MusicV", &m_fMusicV);
		pRoot->Attribute("SoundV", &m_fSoundV);


		CAudioManager::GetInstance()->AdjustMusicVolume(  - (CAudioManager::GetInstance()->GetMusicVolume() - (float)m_fMusicV));
		CAudioManager::GetInstance()->AdjustSoundVolume(  - (CAudioManager::GetInstance()->GetSoundVolume() - (float)m_fSoundV));
	}

	CParticleManager::GetInstance()->LoadParticleEffects();
}

// Main
bool CGame::Main( void )
{
	// Exit based on user input?
	if( Input() == false )
		return false;

	// Update & Render the game
	Update();
	Render();

	// Keep running
	return true;
}

// Shutdown
void CGame::Shutdown( void )
{
	CParticleManager::GetInstance()->UnloadParticleEffects();
	CParticleManager::GetInstance()->DeleteInstance();
	//clear the list of prev statates

	std::list<IGameState*>::iterator iter  = m_lPrevStates.begin();
	for( ; iter != m_lPrevStates.end(); iter++)
		(*iter)->Exit();


	m_pCurrState->Exit();

	// Shutdown the Managers in REVERSE order!
	m_pIM->Exit();
	CAudioManager::GetInstance()->DeleteInstance();
	AnimationManager::GetInstance()->DeleteInstance();
	CBitmapFont::GetInstance()->DeleteInstance();
	CItemManager::GetInstance()->DeleteInstance();

	if( m_pTM != nullptr )
	{
		m_pTM->ShutdownTextureManager();
		m_pTM = nullptr;
	}

	if( m_pD3D != nullptr )
	{
		m_pD3D->ShutdownDirect3D();
		m_pD3D = nullptr;
	}

	if( m_pEM != nullptr )
	{
		m_pEM->DeleteInstance();
		m_pEM = nullptr;
	}
}


// Input
bool CGame::Input ( void )
{
	

	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-009
	// BUG FIX 
	/////////////////////////////////////////////////////////////////

	// Calculate the elapsed time since the last frame (but don't update now - update is already doing that)
	DWORD NOW = GetTickCount();
	float fElapsedTime = (NOW - m_dwCurrTime) / 1000.0f;

	// Clamp the elapsed time to 0.125 secs if the elaspsed time is greater than that
	if( fElapsedTime > 0.125f )
		fElapsedTime = 0.125f;

	if(!m_pIM->Update(fElapsedTime))
		return m_pCurrState->Input();	
	return true;

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-009
	//////////////////////////////////////////////////////////////////////////
}


// Update
void CGame::Update( void )
{
	// Calculate the elapsed time since the last frame
	DWORD NOW = GetTickCount();
	float fElapsedTime = (NOW - m_dwCurrTime) / 1000.0f;
	m_dwCurrTime = NOW;

	// Clamp the elapsed time to 0.125 secs if the elaspsed time is greater than that
	if( fElapsedTime > 0.125f )
		fElapsedTime = 0.125f;

	// Refresh the audio
	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-002
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	// Refresh audio for looping
	CAudioManager::GetInstance()->Update();
	
	///////////////////////////////////////////////////////////////////////////	
	// BUG FIX END  Reference # BB-002
	//////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-013
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	// Same as above

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-013
	//////////////////////////////////////////////////////////////////////////
	
	// Update the current state
	m_pCurrState->Update( fElapsedTime );

}

// Render
void CGame::Render( void )
{

	// Clear the background
	m_pD3D->Clear( 0, 0, 0 );

	// Start the D3D rendering
	m_pD3D->DeviceBegin();
	m_pD3D->SpriteBegin();

	// Flush the sprites to the backbuffer
	// before RECTS, Lines, & Text
	m_pD3D->GetSprite()->Flush();

	// Render the current state
	m_pCurrState->Render();

	m_pD3D->GetSprite()->Flush();

	// End the rendering
	m_pD3D->SpriteEnd();
	m_pD3D->DeviceEnd();

	// Present the backbuffer to the screen
	m_pD3D->Present();




}

// Game State Machine:
void CGame::ChangeState( IGameState* pNextState )
{
	// Exit the current state (if any)
	if( m_pCurrState != nullptr )
	{
		m_pCurrState->Exit();

	}


	// Store the parameter
	m_pCurrState = pNextState;


	// Enter the new state (if any)
	if( m_pCurrState != nullptr )
		m_pCurrState->Enter();
}


//changes state to a new state and adds the prev one to the list
void CGame::ChangeToNew( IGameState* pNextState )
{
	//check if is the first time
	if (m_pCurrState != nullptr)
	{
		//save the curr state in the list of prev states
		m_lPrevStates.push_front(m_pCurrState);

		m_pCurrState = pNextState;
		m_pCurrState->Enter();

	}
	else
	{
		m_pCurrState = pNextState;
		m_pCurrState->Enter();
	}

}


//change states to the prev state 
void CGame::ChangeToPrev()
{
	m_pCurrState->Exit();

	//get the first state in the list and make it the current state
	m_pCurrState = *(m_lPrevStates.begin());
	m_lPrevStates.pop_front();
}


void CGame::ChangeToPrev( int nNumBack)
{
	for (int i = 0; i < nNumBack; i++)
	{
		m_pCurrState->Exit();

		//get the first state in the list and make it the current state
		m_pCurrState = *(m_lPrevStates.begin());
		m_lPrevStates.pop_front();
	}
}



string CGame::GetPath()
{
	return FilePath;
}

string CGame::GetSaveSlot()
{
	return m_sSaveSlot;
}

void	CGame::SetSaveSlot( string nSave)
{
	m_sSaveSlot = nSave;
}
