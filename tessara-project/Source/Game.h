/*
	File:		Game.h
	Author:		Daniel Habig
	Course:		SGP
	Purpose:	The CGame class controls the gameplay & SGD Wrappers
*/

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <vector>
#include <list>
using namespace std;

//	Forward Class Declarations
class CInputManager;

class CSGD_Direct3D;
//class CSGD_DirectInput;
class CSGD_TextureManager;
//class CSGD_XAudio2;
//class FontBMP;
class IGameState;
class CEntityManager;
class CSGD_XAudio2;

class CGame
{
public:
	//	SINGLETON ACCESSOR
	static CGame* GetInstance   ( void );
	static void   DeleteInstance( void );

	// 3 steps to play a game: setup, run, cleanup
	void Initialize	( HWND* hWnd, HINSTANCE hInstance, int nWidth, int nHeight, bool bIsWindowed );
//--------------------------------------------------------------------------------------------------
	bool Main			( void );
	void Shutdown		( void );

	int GetWindowWidth	( void )	const	{ return m_nScreenWidth; }
	int GetWindowHeight	( void )	const	{ return m_nScreenHeight; }
	bool GetIsWindowed	( void )	const	{ return m_bIsWindowed;	}

	// Game State Machine:
	void	ChangeState		( IGameState* pNextState );

	void ChangeToNew( IGameState* pNextState );
	void ChangeToPrev();
	void ChangeToPrev( int nNumBack);
	
	string GetPath();
	string GetSaveSlot();
	void	SetSaveSlot(string nSave);
	void	SetIsWindowed(bool bIsWindowed)	{ m_bIsWindowed = bIsWindowed; }

//--------------------------------------------------------------------------------------------------
private:
	// SINGLETON 
	static CGame* s_pInstance;

	// SINGLETON!
	CGame				( void );				// ctor
	~CGame				( void );				// dtor
	CGame				( const CGame& );		// copy ctor
	CGame& operator=	( const CGame& );		// assignment op

	// Helper Methods:
	bool Input ( void );
	void Update( void );
	void Render( void );

	// Managers
	CSGD_Direct3D*			m_pD3D;
	CInputManager*			m_pIM;
	CSGD_TextureManager*	m_pTM;
	CSGD_XAudio2*			m_pXA;
	int						m_nMenuMusic;
	CEntityManager*         m_pEM;
	
	//FontBMP*				m_pFont;


	// Window attributes
	int						m_nScreenWidth;
	int						m_nScreenHeight;
	bool					m_bIsWindowed;

	// FPS variables
	DWORD					m_dwCurrTime;
	
	// Current Game State
	IGameState*				m_pCurrState;

	//vectore of all the prev states
	list<IGameState*>		m_lPrevStates;
	
	string FilePath;
	string m_sSaveSlot;
	

	HWND* mHWND;
};

