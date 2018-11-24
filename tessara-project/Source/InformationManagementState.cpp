/*
	Author:		Benjamin "Evan" Cowles
	File:		InformationManagementState.cpp
*/

#include "InformationManagementState.h"
#include "ParticleEffect.h"
#include "InputManager.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "Game.h"
#include "MainMenuState.h"
#include "AudioManager.h"
#include "BitmapFont.h"
#include "..\\TinyXML\tinyxml.h"

#include <ShlObj.h>
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"

InformationManagementState* InformationManagementState::GetInstance()
{
	static InformationManagementState s_Instance;

	return &s_Instance;
}

InformationManagementState::InformationManagementState()
{
	m_pIM		= nullptr;
	m_pTM		= nullptr;
	m_pAM		= nullptr;

	m_nBackgroundImgID	 = -1;
	m_nMenuButtonImgID	 = -1;
	m_nSelectedIndex	 = 0;
	m_nSwordImgID		 = -1;
	m_nPanelImgID		 = -1;
	m_nBackButtonImgID	 = -1;

}

InformationManagementState::~InformationManagementState()
{ }

void InformationManagementState::Enter()
{
	// Store my wrappers
	m_pIM	= CInputManager::GetInstance();
	m_pTM	= CSGD_TextureManager::GetInstance();
	m_pAM	= CAudioManager::GetInstance();
	m_pBF	= CBitmapFont::GetInstance();

	// Load my textures
	m_nBackgroundImgID = CSGD_TextureManager::GetInstance()->LoadTexture( _T("Resource/Graphics/BG.jpg") );
	m_nMenuButtonImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/attribute_button.png"));
	m_nSwordImgID	   = CSGD_TextureManager::GetInstance()->LoadTexture( _T("Resource/Graphics/Sword.png"));
	m_nPanelImgID	   = CSGD_TextureManager::GetInstance()->LoadTexture( _T("Resource/Graphics/Panel_Large.png"));
	m_nBackButtonImgID = CSGD_TextureManager::GetInstance()->LoadTexture( _T("Resource/Graphics/MenuButton2.png"));

	m_nSelectedIndex	 = 1;
	Selection.SetXPosition(100);
	Selection.SetYPosition(125);
	Selection.LoadFromFile( "Resource/Graphics/Particles/SparklesSmall.xml" );
	Selection.Initialize();

	SelectionLarge.SetXPosition(-100);
	SelectionLarge.SetYPosition(-100);
	SelectionLarge.LoadFromFile( "Resource/Graphics/Particles/Sparkles.xml" );
	SelectionLarge.Initialize();

	Burst.SetXPosition(425);
	Burst.SetYPosition(540);
	Burst.LoadFromFile( "Resource/Graphics/Particles/Burst.xml" );
	Burst.Initialize();

	m_bFadingIn = true;
	m_fFadeTimer = 0.0f;
	m_nAlphaOffset = 255;
}

void InformationManagementState::Exit()
{
	if (m_nSwordImgID != -1)
	{
		m_pTM->UnloadTexture( m_nSwordImgID );
		m_nSwordImgID = -1;
	}

	if (m_nPanelImgID != -1)
	{
		m_pTM->UnloadTexture( m_nPanelImgID );
		m_nPanelImgID = -1;
	}

	if (m_nBackgroundImgID != -1)
	{
		m_pTM->UnloadTexture(m_nBackgroundImgID);
		m_nBackgroundImgID = -1;
	}

	if (m_nMenuButtonImgID != -1)
	{
		m_pTM->UnloadTexture(m_nMenuButtonImgID);
		m_nMenuButtonImgID = -1;
	}

	if (m_nBackButtonImgID != -1)
	{
		m_pTM->UnloadTexture(m_nBackButtonImgID);
		m_nBackButtonImgID = -1;
	}


	float m_fMosicV = m_pAM->GetMusicVolume();
	float m_fSoundV = m_pAM->GetSoundVolume();


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

	pathtowrite += "\\Sound.xml";

	TiXmlDocument doc;

	// Create a TinyXML Declaration (version & encoding)
	TiXmlDeclaration* pDecl = new TiXmlDeclaration( "1.0", "utf-8", "" );

	// Write the declaration to the document
	doc.LinkEndChild( pDecl );

	// Create a root node for our "Volume_Settings"
	TiXmlElement* pRoot = new TiXmlElement( "Volume_Settings" );


	pRoot->SetDoubleAttribute("MusicV", m_fMosicV); 
	pRoot->SetDoubleAttribute("SoundV", m_fSoundV); 

	// Write the root node to the document
	doc.LinkEndChild( pRoot );

	doc.SaveFile( pathtowrite.c_str() );


}

bool InformationManagementState::Input()
{
/////////////////////////////////////////////////////////////////
// BUG FIX
// Reference Bug # BB-005
// BUG FIX START
/////////////////////////////////////////////////////////////////

if (m_bFadingOut)
		return true;

///////////////////////////////////////////////////////////////////////////
// BUG FIX END  Reference # BB-005
//////////////////////////////////////////////////////////////////////////
	


	if (m_pIM->CheckEnter())
	{
		switch (m_nSelectedIndex)
		{
		case 1:
			m_pAM->AdjustMusicVolume(-0.05f);
			break;
		case 2:
			m_pAM->AdjustMusicVolume(0.05f);
			break;
		case 3:
			m_pAM->AdjustSoundVolume(-0.05f);
			break;
		case 4:
			m_pAM->AdjustSoundVolume(0.05f);
			break;
		case 5:
			m_pAM->PlaySFX( SFX_CONFIRM );
			DoFullscreen(!CGame::GetInstance()->GetIsWindowed());
			break;
		case 6:
			{
				CAudioManager::GetInstance()->PlaySFX(SFX_CONFIRM);
				m_bFadingOut = true;
				m_fFadeTimer = 0.0f;
				return true;
			}
		}
	}

	if (m_pIM->CheckRight())
	{
		++m_nSelectedIndex;
		if (m_nSelectedIndex > 6)
			m_nSelectedIndex = 1;
		m_pAM->PlaySFX(SFX_CURSOR);
	}
	if (m_pIM->CheckUp())
	{
		if (m_nSelectedIndex == 6)
			m_nSelectedIndex = 5;
		else
			m_nSelectedIndex -= 2;
		if (m_nSelectedIndex < 1)
			m_nSelectedIndex = 6;
		m_pAM->PlaySFX(SFX_CURSOR);
	}

	if (m_pIM->CheckLeft())
	{
		--m_nSelectedIndex;
		if (m_nSelectedIndex < 1)
			m_nSelectedIndex = 6;
		m_pAM->PlaySFX(SFX_CURSOR);
	}
	if (m_pIM->CheckDown())
	{
		if (m_nSelectedIndex == 4)
			m_nSelectedIndex = 5;
		else if (m_nSelectedIndex == 5)
			m_nSelectedIndex = 6;
		else
			m_nSelectedIndex += 2;

		if (m_nSelectedIndex > 6)
			m_nSelectedIndex = 1;
		m_pAM->PlaySFX(SFX_CURSOR);
	}

	return true;
}

void InformationManagementState::Update(float fElapsedTime)
{
	if (m_bFadingIn || m_bFadingOut)
	{
		m_fFadeTimer += fElapsedTime;
		if (m_bFadingIn)
			m_nAlphaOffset -= (125 * fElapsedTime);
		if (m_bFadingOut)
		{
			Burst.Update( fElapsedTime );
			m_nAlphaOffset += (125 * fElapsedTime);
		}
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
			CGame::GetInstance()->ChangeToPrev();
			return;
		}
	}

	Selection.Update( fElapsedTime );
	SelectionLarge.Update( fElapsedTime );

}
void InformationManagementState::Render()
{
	RECT rRect;
	rRect.left = 0;
	rRect.top = 0;
	rRect.right = 800;
	rRect.bottom = 600;

	CSGD_TextureManager::GetInstance()->Draw(m_nBackgroundImgID,0,0,1.0f,1.0f,&rRect,0,0,0,D3DCOLOR_ARGB(255,185,185,185));

	for (int i = 0; i < 7; ++i)
	{
		if (i != m_nSelectedIndex - 1 )
			m_nAlpha[i] = 75 - (int)m_nAlphaOffset;
		else
			m_nAlpha[i] = 200 - (int)m_nAlphaOffset;

		if (m_nAlpha[i] < 0 )
			m_nAlpha[i] = 0;
		if (m_nAlpha[i] > 255)
			m_nAlpha[i] = 255;
	}

	SelectionLarge.SetXPosition(-100);
	SelectionLarge.SetYPosition(-100);

	switch( m_nSelectedIndex )
	{
	case 1:
		{
			Selection.SetXPosition(100);
			Selection.SetYPosition(125);
		}
		break;
	case 2:
		{
			Selection.SetXPosition(750);
			Selection.SetYPosition(125);
		}
		break;
	case 3:
		{
			Selection.SetXPosition(100);
			Selection.SetYPosition(325);
		}
		break;
	case 4:
		{
			Selection.SetXPosition(750);
			Selection.SetYPosition(325);
		}
		break;
	case 6:
		{
			Selection.SetXPosition(-100);
			Selection.SetYPosition(-100);
			SelectionLarge.SetXPosition(425);
			SelectionLarge.SetYPosition(540);
		}
		break;

	}

	// Render Panel
	CSGD_TextureManager::GetInstance()->Draw(m_nPanelImgID, 50,50,0.9f,0.7f,0,0,0,0,D3DCOLOR_ARGB( m_nAlpha[m_nSelectedIndex - 1],255,255,255));

	// Render Buttons
	CSGD_TextureManager::GetInstance()->Draw(m_nMenuButtonImgID,75,115,0.14f,0.14f,0,0,0,0,D3DCOLOR_ARGB(m_nAlpha[0],255,255,100));
	m_pBF->Draw("-", 87, 125, 0.7f, D3DCOLOR_ARGB(m_nAlpha[0], 0, 0, 0));
	CSGD_TextureManager::GetInstance()->Draw(m_nMenuButtonImgID,725,115,0.14f,0.14f,0,0,0,0,D3DCOLOR_ARGB(m_nAlpha[1],255,255,100));
	m_pBF->Draw("+", 737, 125, 0.7f, D3DCOLOR_ARGB(m_nAlpha[1], 0, 0, 0));
	CSGD_TextureManager::GetInstance()->Draw(m_nMenuButtonImgID,75,210,0.14f,0.14f,0,0,0,0,D3DCOLOR_ARGB(m_nAlpha[2],255,255,100));
	m_pBF->Draw("-", 87, 220, 0.7f, D3DCOLOR_ARGB(m_nAlpha[2], 0, 0, 0));
	CSGD_TextureManager::GetInstance()->Draw(m_nMenuButtonImgID,725,210,0.14f,0.14f,0,0,0,0,D3DCOLOR_ARGB(m_nAlpha[3],255,255,100));
	m_pBF->Draw("+", 737, 220, 0.7f, D3DCOLOR_ARGB(m_nAlpha[3],  0, 0, 0));
	CSGD_TextureManager::GetInstance()->Draw(m_nMenuButtonImgID,300,350,0.18f,0.18f,0,0,0,0,D3DCOLOR_ARGB(m_nAlpha[4],255,255,100));
	if (!CGame::GetInstance()->GetIsWindowed())
		m_pBF->Draw("X",315,360,1.0f,D3DCOLOR_ARGB(m_nAlpha[m_nSelectedIndex-1],0,0,0));
	m_pBF->Draw("Fullscreen",370,365,0.7f,D3DCOLOR_ARGB(m_nAlpha[m_nSelectedIndex-1], 255, 255, 255));
	CSGD_TextureManager::GetInstance()->Draw(m_nBackButtonImgID,315,515,1.2f,0.7f,0,0,0,0,D3DCOLOR_ARGB(m_nAlpha[5],185,185,185));
	m_pBF->Draw("Back", 380, 525, 0.8f, D3DCOLOR_ARGB(m_nAlpha[5], 255, 255, 255));

	// Draw Empty Bars
	RECT Empty = {0,0,580,130};
	CSGD_TextureManager::GetInstance()->Draw(m_nSwordImgID,130,70,1.0f,1.0f,0,0,0,0,D3DCOLOR_ARGB(m_nAlpha[m_nSelectedIndex-1],0,0,0));
	CSGD_TextureManager::GetInstance()->Draw(m_nSwordImgID,130,170,1.0f,1.0f,&Empty,290,65,3.14f,D3DCOLOR_ARGB(m_nAlpha[m_nSelectedIndex-1],0,0,0));
	
	// Draw Full Bars
	RECT FullBar1, FullBar2;
	FullBar1.left = 0; FullBar2.left = 580 - (int)(580 * m_pAM->GetSoundVolume());
	FullBar1.top = 0;  FullBar2.top = 0;
	FullBar1.bottom = 130; FullBar2.bottom = 130;
	FullBar1.right = (int)(580 * m_pAM->GetMusicVolume());
	FullBar2.right = 580;

	CSGD_TextureManager::GetInstance()->Draw(m_nSwordImgID,130,70,1.0f,1.0f,&FullBar1,0,0,0,D3DCOLOR_ARGB(m_nAlpha[m_nSelectedIndex-1],255,255,255));
	m_pBF->Draw("Music", 360, 90, 0.8f, D3DCOLOR_ARGB(m_nAlpha[m_nSelectedIndex - 1], 255, 255, 255));
	CSGD_TextureManager::GetInstance()->Draw(m_nSwordImgID,-450 + (int)(580 * m_pAM->GetSoundVolume()),170,1.0f,1.0f,&FullBar2,290,65,3.14f,D3DCOLOR_ARGB(m_nAlpha[m_nSelectedIndex-1],255,255,255));
	m_pBF->Draw("SFX", 370, 180, 0.8f, D3DCOLOR_ARGB(m_nAlpha[m_nSelectedIndex - 1], 255, 255, 255));

	if (!m_bFadingOut)
	{
	Selection.Render();
	SelectionLarge.Render();
	}

	if (m_bFadingOut)
		Burst.Render();

}

void InformationManagementState::DoFullscreen(bool bIsFullscreen)
{
	CGame::GetInstance()->SetIsWindowed(bIsFullscreen);
	CSGD_Direct3D::GetInstance()->ChangeDisplayParam(CGame::GetInstance()->GetWindowWidth() , CGame::GetInstance()->GetWindowHeight(), CGame::GetInstance()->GetIsWindowed());
}