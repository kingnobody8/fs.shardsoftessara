/*
File:		InputManager.h
Author:		Daniel Habig
Course:		SGP
Purpose:	The input manager handles parsing input for the different input methods
*/

#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "XInput.h"

#define VIBE 0.5f
#define MTIME 0.25f;

//Forward Class Declaration
class CSGD_DirectInput;
class CXBOXController;

class CInputManager

{
private:

	bool m_bUp;
	int m_nDeadZone;

public:
	// SINGLETON!
	static CInputManager* GetInstance( void );

	//Interface
	void	Enter	( HWND hWnd, HINSTANCE hInstance );					// load resources
	void	Exit	( void );					// unload resources
	bool	Input	( void );					// check for specific input
	bool	Update  ( float deltaTime );
	void	UpdateVibe( float fElapsedTime);


	bool CheckUp();
	bool CheckDown();
	bool CheckRight();
	bool CheckLeft();
	bool CheckEnter();
	bool CheckHotKey();
	bool CheckMelee();
	bool Check3();
	bool CheckAb1();
	bool CheckAb2();
	bool CheckAb3();
	bool CheckCycleLeft();
	bool CheckCycleRight();
	bool CheckPause();
	bool NonePressed();

	bool CheckHeal();

	bool CheckDownD();
	bool CheckUpD();
	bool CheckLeftD();
	bool CheckRightD();

	bool CheckDownEx();
	bool CheckUpEx();
	bool CheckRightEx();
	bool CheckLeftEx();

	bool CheckFullscreenToggle();

	int Direction(int nValue);
	void UpdateSate();

	void Vibrate();

	void NoteMelee(void);
	//--------------------------------------------------------------------------------------------------

private:
	// SINGLETON!
	CInputManager( void );								// ctor
	virtual ~CInputManager( void );						// dtor
	CInputManager( const CInputManager& );				// copy ctor
	CInputManager& operator= ( const CInputManager& );	// assignment op

	//	Direct Input pointer
	CSGD_DirectInput*		m_pDI;
	//	Xbox Controller pointer
	CXBOXController*		m_pXBOX;

	XINPUT_STATE state;

	float m_fTimer;


	bool xUP, xDOWN, xLEFT, xRIGHT, xALPHA, xBETA, xYANKEE, xXRAY, xSTART, xSELECT, xLBUMP, xRBUMP, xLTRIG, xRTRIG;
	long vibe;
	float vTime;

	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-009
	// BUG FIX START
	/////////////////////////////////////////////////////////////////
	float mTime;
	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-009
	//////////////////////////////////////////////////////////////////////////
};

