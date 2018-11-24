/*
File:		InputManager.cpp
Author:		Daniel Habig
Course:		SGP
Purpose:	The input manager handles parsing input for the different input methods
*/

#include "InputManager.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "CXBOXController.h"
#include "Game.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"



// SINGLETON!
/*static*/ CInputManager* CInputManager::GetInstance( void )
{
	// Easy-instantiation: use a static variable
	static CInputManager s_Instance;

	return &s_Instance;
}


CInputManager::CInputManager(void)
{
	//	Set data to 'Clean' values
	this->m_pDI		= nullptr;
	this->m_pXBOX	= nullptr;
	m_bUp = false;
	mTime = -1;

	m_nDeadZone = 4000;
	//state = m_pXBOX->GetState();
	vibe = 0;
	vTime = 0.0f;
	xUP = xDOWN = xLEFT = xRIGHT = xALPHA = xBETA = xYANKEE = xXRAY = xSTART = xSELECT = xLBUMP = xRBUMP = xLTRIG = xRTRIG = false;;

}


CInputManager::~CInputManager(void)
{
}


void CInputManager::Enter( HWND hWnd, HINSTANCE hInstance )
{
	this->m_pDI		= CSGD_DirectInput::GetInstance();
	this->m_pXBOX	= new CXBOXController(1);

	//Initialize Direct Input
	m_pDI->InitDirectInput		( hWnd, hInstance, DI_KEYBOARD );


}


void CInputManager::Exit	( void )
{
	//	Shutdown Direct Input & set pointer to null
	if( m_pDI != nullptr )
	{
		m_pDI->ShutdownDirectInput();
		m_pDI = nullptr;
	}
	//	Delete the xbox controller and set it to null
	delete m_pXBOX;
	this->m_pXBOX	= nullptr;
}


bool CInputManager::Input	( void )
{
	return true;
}


bool	CInputManager::Update  ( float deltaTime )
{
	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-009
	// BUG FIX START
	/////////////////////////////////////////////////////////////////
	if( mTime > 0.0f )
		mTime -= deltaTime;
	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-009
	//////////////////////////////////////////////////////////////////////////

	// Read the hardware
	m_pDI->ReadDevices();
	return CheckFullscreenToggle();
}

bool CInputManager::CheckUp()
{
	if (m_pDI->KeyPressed( DIK_W ) || m_pDI->KeyPressed( DIK_UPARROW ) )
		return true;

	else if (m_pXBOX->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP )
	{
		if( !xUP )
		{
			xUP = true;
			return true;
		}
	}
	else
	{
		xUP = false;
	}
	return false;
}


bool CInputManager::CheckDown()
{
	if( m_pDI->KeyPressed( DIK_S ) || m_pDI->KeyPressed( DIK_DOWNARROW) )
		return true;

	else if(	m_pXBOX->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
	{
		if( !xDOWN )
		{
			xDOWN = true;
			return true;
		}
	}
	else
	{
		xDOWN = false;
	}
	return false;
}

bool CInputManager::CheckUpD()
{
	if (Direction(m_pXBOX->GetState().Gamepad.sThumbLY) == 1)
		return true;

	return (m_pDI->KeyDown( DIK_W ) || m_pDI->KeyDown( DIK_UPARROW ) ||
		m_pXBOX->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
}

bool CInputManager::CheckUpEx()
{
	if (m_pDI->KeyPressedEx( DIK_W ) || m_pDI->KeyPressedEx( DIK_UPARROW ) )
		return true;

	else if (m_pXBOX->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP )
	{
		if( !xUP )
		{
			xUP = true;
			return true;
		}
	}
	else
	{
		xUP = false;
	}
	return false;
}


bool CInputManager::CheckDownD()
{
	if (Direction(m_pXBOX->GetState().Gamepad.sThumbLY) == -1)
		return true;

	return (m_pDI->KeyDown( DIK_S ) || m_pDI->KeyDown( DIK_DOWNARROW) ||
		m_pXBOX->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
}

bool CInputManager::CheckRight()
{
	if (m_pDI->KeyPressed( DIK_D )  || m_pDI->KeyPressed( DIK_RIGHTARROW ) )
		return true;
	else if( m_pXBOX->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		if( !xRIGHT )
		{
			xRIGHT = true;
			return true;
		}
	}
	else
	{
		xRIGHT = false;
	}
	return false;
}


bool CInputManager::CheckLeft()
{
	if (m_pDI->KeyPressed( DIK_A ) || m_pDI->KeyPressed( DIK_LEFTARROW ) )
		return true;
	else if( m_pXBOX->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
	{
		if( !xLEFT )
		{
			xLEFT = true;
			return true;
		}
	}
	else
	{
		xLEFT = false;
	}
	return false;
}


bool CInputManager::CheckRightD()
{
	if (Direction(m_pXBOX->GetState().Gamepad.sThumbLX) == 1)
		return true;

	return (m_pDI->KeyDown( DIK_D )  || m_pDI->KeyDown( DIK_RIGHTARROW ) || 
		m_pXBOX->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
}

bool CInputManager::CheckRightEx()
{
	if (m_pDI->KeyPressedEx( DIK_D )  || m_pDI->KeyPressedEx( DIK_RIGHTARROW ) )
		return true;
	else if( m_pXBOX->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		if( !xRIGHT )
		{
			xRIGHT = true;
			return true;
		}
	}
	else
	{
		xRIGHT = false;
	}
	return false;
}


bool CInputManager::CheckLeftD()
{
	if (Direction(m_pXBOX->GetState().Gamepad.sThumbLX) == -1)
		return true;

	return (m_pDI->KeyDown( DIK_A ) || m_pDI->KeyDown( DIK_LEFTARROW ) || 
		m_pXBOX->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
}

bool CInputManager::CheckLeftEx()
{
	if (m_pDI->KeyPressedEx( DIK_A ) || m_pDI->KeyPressedEx( DIK_LEFTARROW ) )
		return true;
	else if( m_pXBOX->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
	{
		if( !xLEFT )
		{
			xLEFT = true;
			return true;
		}
	}
	else
	{
		xLEFT = false;
	}
	return false;
}

bool CInputManager::CheckDownEx()
{
	if( m_pDI->KeyPressedEx( DIK_S ) || m_pDI->KeyPressedEx( DIK_DOWNARROW) )
		return true;

	else if(	m_pXBOX->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
	{
		if( !xDOWN )
		{
			xDOWN = true;
			return true;
		}
	}
	else
	{
		xDOWN = false;
	}
	return false;
}

bool CInputManager::CheckEnter()
{
	if (m_pDI->KeyPressed( DIK_RETURN ) && !(m_pDI->KeyPressed(DIK_LALT) || m_pDI->KeyPressed(DIK_RALT)))
		return true;
	if( m_pXBOX->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A  )
	{
		if( !xALPHA )
		{
			xALPHA = true;
			return true;
		}
	}
	else
	{
		xALPHA = false;
	}
	if( m_pXBOX->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_START )
	{
		if( !xSTART )
		{
			xSTART = true;
			return true;
		}
	}
	else
	{
		xSTART = false;
	}
	return false;
}


bool  CInputManager::CheckPause()
{
	if(m_pXBOX->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_START) 
	{
		if( !xSTART )
		{
			xSTART = true;
			return true;
		}
	}
	else
	{
		xSTART = false;
	}

	if ( m_pDI->KeyPressed( DIK_ESCAPE ) || (m_pDI->KeyDown(DIK_LALT) || m_pDI->KeyDown(DIK_RALT)) && m_pDI->KeyPressed(DIK_TAB) )
		return true;
	return false;
}

bool CInputManager::CheckHotKey()
{
	return (m_pDI->KeyPressed( DIK_NUMPAD1 ) );
}

/////////////////////////////////////////////////////////////////
// BUG FIX
// Reference Bug # BB-009
// BUG FIX START
/////////////////////////////////////////////////////////////////
void CInputManager::NoteMelee(void)
{
	mTime = MTIME;
}
///////////////////////////////////////////////////////////////////////////
// BUG FIX END  Reference # BB-009
//////////////////////////////////////////////////////////////////////////
bool CInputManager::CheckMelee()
{
	if( mTime > 0.0f )
		return false;
	if( m_pXBOX->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_X)
	{
		if( !xXRAY )
		{
			xXRAY = true;
			return true;
		}
	}
	else
	{
		xXRAY = false;
	}
	return (m_pDI->KeyPressed( DIK_NUMPAD2 ) );
}


bool CInputManager::Check3()
{
	return (m_pDI->KeyPressed( DIK_NUMPAD3 ) );
}


bool CInputManager::CheckAb1()
{
	return (m_pDI->KeyPressed( DIK_NUMPAD4 ) || m_pXBOX->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A );
}


bool CInputManager::CheckAb2()
{
	return (m_pDI->KeyPressed( DIK_NUMPAD5 ) || m_pXBOX->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B );
}


bool CInputManager::CheckAb3()
{
	return (m_pDI->KeyPressed( DIK_NUMPAD6 ) || m_pXBOX->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_Y );
}


bool CInputManager::CheckCycleLeft()
{
	if( m_pXBOX->GetState().Gamepad.bLeftTrigger >= 200 )
	{
		if( !xLTRIG )
		{
			xLTRIG = true;
			UpdateSate();
			return true;
		}
	}
	else
	{
		xLTRIG = false;
	}

	UpdateSate();
	return (m_pDI->KeyPressed( DIK_NUMPAD7 ));
}

bool CInputManager::CheckCycleRight()
{
	if( m_pXBOX->GetState().Gamepad.bRightTrigger >= 200 )
	{
		if( !xRTRIG )
		{
			xRTRIG = true;
			UpdateSate();
			return true;
		}
	}
	else
	{
		xRTRIG = false;
	}

	UpdateSate();
	return (m_pDI->KeyPressed( DIK_NUMPAD8 ) );
}

bool CInputManager::CheckFullscreenToggle()
{
	if((m_pDI->KeyDown(DIK_LALT) || m_pDI->KeyDown(DIK_RALT)) && m_pDI->KeyPressed(DIK_RETURN))
	{
		CGame::GetInstance()->SetIsWindowed(!CGame::GetInstance()->GetIsWindowed());
		CSGD_Direct3D::GetInstance()->ChangeDisplayParam(CGame::GetInstance()->GetWindowWidth() , CGame::GetInstance()->GetWindowHeight(), CGame::GetInstance()->GetIsWindowed());

		return true;
	}

	return false;
}

bool CInputManager::CheckHeal()
{
	UpdateSate();
	// Needs Arcade/XBox additions
	if (m_pXBOX->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
	{
		if( !xRBUMP )
		{
			xRBUMP = true;
			return true;
		}
	}
	else
	{
		xRBUMP = false;
	}
	return (m_pDI->KeyDown(DIK_NUMPAD3));
}


int CInputManager::Direction(int nValue)
{
	if (nValue > m_nDeadZone || nValue < -m_nDeadZone)
	{
		if (nValue > 0)
			return 1;
		else if (nValue < 0)
			return -1;
	}
	return 0;
}

void CInputManager::UpdateSate()
{
	state = m_pXBOX->GetState();
}

//bool CInputManager::Update  ( float deltaTime )
//{
//	m_fTimer += deltaTime;
//
//	if (m_fTimer >= 2.0f)
//		UpdateSate();
//}

void CInputManager::Vibrate()
{
	vTime = VIBE;
}
void CInputManager::UpdateVibe( float fElapsedTime)
{
	if( vTime > 0.0f )
	{
		vibe = 65535;
		vTime -= fElapsedTime;
	}
	if( vTime <= 0 )
		vibe = 0;
	m_pXBOX->Vibrate( vibe, vibe );
}

bool CInputManager::NonePressed()
{
	return false;
}