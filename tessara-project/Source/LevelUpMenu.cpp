/*
File:		LevelUpMenu.cpp
Author:		Mark Diaz
Course:		SGP 1304
Purpose:	The CLevelUpMenu class allows the player to
Allocate attribute points
*/

#include "LevelUpMenu.h"
#include "Player.h"
#include "PlayState.h"
#include "Game.h"
#include "BitmapFont.h"
#include "AudioManager.h"
#include "InputManager.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "AudioManager.h"

// This class is a singleton.
/*static*/ CLevelUpMenu* CLevelUpMenu::GetInstance( void )
{
	static CLevelUpMenu s_Instance;
	return &s_Instance;
}

CLevelUpMenu::CLevelUpMenu(void)
{
	m_pIM		= nullptr;
	m_pTM		= nullptr;

	m_nBackgroundImgID = -1;
	m_nMenuButtonImgID = -1;
	m_nSelectedIndex = 1;
	m_nAttributeButtonImgID = -1;
	m_nShardImgID = -1;
	m_nPanelImgID = -1;
}


CLevelUpMenu::~CLevelUpMenu(void)
{
}

void CLevelUpMenu::Enter( void )
{
	ShowPlayerHelp = !(PlayState::GetInstance()->GetPlayer()->GetHelpMessages() & 1<<1);
	ShowArtifactHelp = !(PlayState::GetInstance()->GetPlayer()->GetHelpMessages() & 1<<2);

	m_pTM = CSGD_TextureManager::GetInstance();
	m_pIM = CInputManager::GetInstance();
	m_pBF = CBitmapFont::GetInstance();

	artCount = PlayState::GetInstance()->GetPlayer()->GetArtifactNumber();
	if (artCount == 0)
		artIndex = -1;
	else
		artIndex = 0;
	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-021
	// BUG FIX START
	/////////////////////////////////////////////////////////////////
	if (PlayState::GetInstance()->GetPlayer()->GetUnspent() == 0)
		m_nSelectedIndex = 5;
	else
		m_nSelectedIndex = 1;
	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-021
	//////////////////////////////////////////////////////////////////////////

	m_nBackgroundImgID = m_pTM->LoadTexture(_T("Resource/Graphics/Inventory/inv_Background.jpg"));
	m_nMenuButtonImgID = m_pTM->LoadTexture(_T("Resource/Graphics/menubutton.png"));
	m_nAttributeButtonImgID = m_pTM->LoadTexture(_T("Resource/Graphics/attribute_button.png"));
	m_nShardImgID = m_pTM->LoadTexture(_T("Resource/Graphics/Shard_Upres.png"));
	m_nPanelImgID = m_pTM->LoadTexture( _T("Resource/Graphics/Panel_Large.png"));

	AttributesToString();
	ExpBar = ((float)PlayState::GetInstance()->GetPlayer()->GetExperience() / 
		(float)PlayState::GetInstance()->GetPlayer()->GetToNextLevel() ) * 2.0f;
	if (ShowPlayerHelp)
		CAudioManager::GetInstance()->PlaySFX(SFX_BACK);

}
void CLevelUpMenu::Exit( void )
{
	if (m_nBackgroundImgID != -1)
	{
		m_pTM->UnloadTexture( m_nBackgroundImgID );
		m_nBackgroundImgID = -1;
	}

	if (m_nPanelImgID != -1)
	{
		m_pTM->UnloadTexture( m_nPanelImgID );
		m_nPanelImgID = -1;
	}

	if (m_nShardImgID != -1)
	{
		m_pTM->UnloadTexture( m_nShardImgID );
		m_nShardImgID = -1;
	}

	if (m_nMenuButtonImgID != -1)
	{
		m_pTM->UnloadTexture(m_nMenuButtonImgID);
		m_nMenuButtonImgID = -1;
	}

	if (m_nAttributeButtonImgID != -1)
	{
		m_pTM->UnloadTexture(m_nAttributeButtonImgID);
		m_nAttributeButtonImgID = -1;
	}

}

bool	CLevelUpMenu::Input ( void )
{
	if (m_bArtifactMode)
		return ArtifactInput();
	else
		return PlayerInput();
}

void	CLevelUpMenu::Update( float fElapsedTime )
{
	if (m_bArtifactMode)
		ArtifactUpdate( fElapsedTime );
	else
		PlayerUpdate( fElapsedTime );
}

void	CLevelUpMenu::Render( void )
{
	m_pTM->Draw(m_nBackgroundImgID,0,0,1.0f,1.0f,0,0,0,0,D3DCOLOR_ARGB(255,185,185,185));

	if (m_bArtifactMode)
		ArtifactRender();
	else
		PlayerRender();
}

void CLevelUpMenu::PlayerUpdate( float fElapsedTime )
{
	for (int i = 0; i < 6; ++i)
		Alpha[i] = 85;

	Alpha[m_nSelectedIndex - 1] = 255;
}

bool CLevelUpMenu::PlayerInput( void )
{
	if (ShowPlayerHelp) // Help Message Input
	{
		if (CInputManager::GetInstance()->CheckUp())
		{
			CAudioManager::GetInstance()->PlaySFX(SFX_CURSOR);
			m_nHelpIndex = 0;
		}
		if (CInputManager::GetInstance()->CheckDown())
		{
			CAudioManager::GetInstance()->PlaySFX(SFX_CURSOR);
			m_nHelpIndex = 1;
		}

		if (CInputManager::GetInstance()->CheckEnter())
		{
			if (m_nHelpIndex == 0)
			{
				CAudioManager::GetInstance()->PlaySFX(SFX_PAUSE);
				PlayState::GetInstance()->GetPlayer()->SetHelpMessages(PlayState::GetInstance()->GetPlayer()->GetHelpMessages() ^ 1<<1);
			}

			if (m_nHelpIndex == 1)
			{
				CAudioManager::GetInstance()->PlaySFX(SFX_CONFIRM);
				m_nHelpIndex = 0;
				ShowPlayerHelp = false;
			}
		}
		return true;
	}

	if (m_pIM->CheckUp())
	{
		CAudioManager::GetInstance()->PlaySFX( SFX_PAUSEMOVE );
		--m_nSelectedIndex;
		if (m_nSelectedIndex < 1)
			m_nSelectedIndex = 6;

		if (PlayState::GetInstance()->GetPlayer()->GetUnspent() <= 0)
			m_nSelectedIndex = 5;
	}

	if (m_pIM->CheckDown())
	{
		CAudioManager::GetInstance()->PlaySFX( SFX_PAUSEMOVE );
		++m_nSelectedIndex;
		if (m_nSelectedIndex > 6)
			m_nSelectedIndex = 1;

		if (PlayState::GetInstance()->GetPlayer()->GetUnspent() <= 0)
			m_nSelectedIndex = 6;
	}

	if (m_pIM->CheckCycleRight() || m_pIM->CheckCycleRight())
	{
		m_nHelpIndex = 0;
		m_bArtifactMode = true;
	}

	

	if (m_pIM->CheckEnter())
	{
		if (m_nSelectedIndex != 5)
			CAudioManager::GetInstance()->PlaySFX( SFX_POINT );
		else
			CAudioManager::GetInstance()->PlaySFX( SFX_PAUSE );

		switch(m_nSelectedIndex)
		{
		case 1: Body(); 
			if (PlayState::GetInstance()->GetPlayer()->GetUnspent() <= 0)
			m_nSelectedIndex = 5; 
			break;
		case 2: Mind(); 
			if (PlayState::GetInstance()->GetPlayer()->GetUnspent() <= 0)
			m_nSelectedIndex = 5;
			break;
		case 3: Soul(); 
			if (PlayState::GetInstance()->GetPlayer()->GetUnspent() <= 0)
			m_nSelectedIndex = 5;
			break;
		case 4: Char(); 
			if (PlayState::GetInstance()->GetPlayer()->GetUnspent() <= 0)
				m_nSelectedIndex = 5;
			break;
		case 5: m_bArtifactMode = true; m_nHelpIndex = 0; 
			if (ShowArtifactHelp)
				CAudioManager::GetInstance()->PlaySFX(SFX_BACK);
			break;
		case 6:
			CGame::GetInstance()->ChangeToPrev();
			break;
		}
	}

	return true;
}

void CLevelUpMenu::PlayerRender(void )
{
	m_pBF->Draw("Attributes", 330, 30, 0.8f, D3DCOLOR_ARGB(255, 255, 255, 0));

	m_pTM->Draw(m_nMenuButtonImgID,190,80,2.0f,0.5f,0,0,0,0,D3DCOLOR_ARGB(200,185,185,185));
	m_pTM->Draw(m_nMenuButtonImgID,190,80,ExpBar,0.5f,0,0,0,0,D3DCOLOR_ARGB(200,0,200,200));
	m_pBF->Draw(Attributes[6], 250, 85, 0.5f, D3DCOLOR_ARGB(200, 150, 255, 255));

	m_pBF->Draw("Level", 50, 80, 0.7f, D3DCOLOR_ARGB(255, 255, 255, 0));
	m_pBF->Draw(Attributes[5], 160, 80, 0.7f, D3DCOLOR_ARGB(255, 255, 255, 0));

	m_pTM->Draw(m_nMenuButtonImgID,50,150,0.8f,0.5f,0,0,0,0,D3DCOLOR_ARGB(200,185,185,185));
	m_pBF->Draw("Body", 60, 160, 0.5f, D3DCOLOR_ARGB(200, 255, 0, 0));
	m_pTM->Draw(m_nMenuButtonImgID,50,200,0.8f,0.5f,0,0,0,0,D3DCOLOR_ARGB(200,185,185,185));
	m_pBF->Draw("Mind", 60, 210, 0.5f, D3DCOLOR_ARGB(200, 0, 0, 255));
	m_pTM->Draw(m_nMenuButtonImgID,50,250,0.8f,0.5f,0,0,0,0,D3DCOLOR_ARGB(200,185,185,185));
	m_pBF->Draw("Soul", 60, 260, 0.5f, D3DCOLOR_ARGB(200, 255, 255, 255));
	m_pTM->Draw(m_nMenuButtonImgID,50,300,0.8f,0.5f,0,0,0,0,D3DCOLOR_ARGB(200,185,185,185));
	m_pBF->Draw("Char", 60, 310, 0.5f, D3DCOLOR_ARGB(200, 0, 255, 0));
	m_pTM->Draw(m_nMenuButtonImgID,50,400,0.8f,0.5f,0,0,0,0,D3DCOLOR_ARGB(200,185,185,185));
	m_pBF->Draw("Unspent", 60, 410, 0.5f, D3DCOLOR_ARGB(200, 0, 0, 0));
	m_pTM->Draw(m_nMenuButtonImgID,50,485,0.5f,0.5f,0,0,0,0,D3DCOLOR_ARGB(Alpha[4],185,185,185));
	
	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-055
	// BUG FIX START
	/////////////////////////////////////////////////////////////////
	
	// Moved text to the left 5px from 60 to 55
	m_pBF->Draw("Artifacts", 55, 490, 0.5f, D3DCOLOR_ARGB(Alpha[4], 0, 0, 0));
	
	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-055
	//////////////////////////////////////////////////////////////////////////
	
	m_pTM->Draw(m_nMenuButtonImgID,50,520,0.5f,0.5f,0,0,0,0,D3DCOLOR_ARGB(Alpha[5],185,185,185));
	m_pBF->Draw("Back", 55, 530, 0.5f, D3DCOLOR_ARGB(Alpha[5], 0, 0, 0));


	m_pTM->Draw(m_nMenuButtonImgID,485,150,0.8f,0.5f,0,0,0,0,D3DCOLOR_ARGB(200,185,185,185));
	m_pTM->Draw(m_nMenuButtonImgID,485,200,0.8f,0.5f,0,0,0,0,D3DCOLOR_ARGB(200,185,185,185));
	m_pTM->Draw(m_nMenuButtonImgID,485,250,0.8f,0.5f,0,0,0,0,D3DCOLOR_ARGB(200,185,185,185));
	m_pTM->Draw(m_nMenuButtonImgID,485,300,0.8f,0.5f,0,0,0,0,D3DCOLOR_ARGB(200,185,185,185));
	m_pTM->Draw(m_nMenuButtonImgID,485,350,0.8f,0.5f,0,0,0,0,D3DCOLOR_ARGB(200,185,185,185));
	m_pTM->Draw(m_nMenuButtonImgID,485,400,0.8f,0.5f,0,0,0,0,D3DCOLOR_ARGB(200,185,185,185));

	m_pBF->Draw(Attributes[8], 495,160,0.5f,D3DCOLOR_ARGB(200,255,0,0));
	m_pBF->Draw(Attributes[9], 495,210,0.5f,D3DCOLOR_ARGB(200,255,0,0));
	m_pBF->Draw(Attributes[10], 495,260,0.5f,D3DCOLOR_ARGB(200,0,0,255));
	m_pBF->Draw(Attributes[11], 495,310,0.5f,D3DCOLOR_ARGB(200,0,0,255));
	m_pBF->Draw(Attributes[12], 495,360,0.5f,D3DCOLOR_ARGB(200,255,255,255));
	m_pBF->Draw(Attributes[7], 495,410,0.5f,D3DCOLOR_ARGB(200,0,255,0));


	for (int i = 0; i < 4; ++i)
	{
		m_pBF->Draw(Attributes[i], 200, 160 + (i * 50), 0.5f, D3DCOLOR_ARGB(200, 0, 0, 0));
		if (PlayState::GetInstance()->GetPlayer()->GetUnspent() > 0)
		{
			m_pTM->Draw(m_nAttributeButtonImgID,270,150 + (50 * i),0.12f,0.12f,0,0,0,0,D3DCOLOR_ARGB(Alpha[i],185,185,185));
			m_pBF->Draw("+", 283, 160 + (50 * i), 0.5f, D3DCOLOR_ARGB(Alpha[i], 255, 255, 0));
		}
	}

	m_pBF->Draw(Attributes[4], 200, 410, 0.5f, D3DCOLOR_ARGB(200, 0, 0, 0));

	if (ShowPlayerHelp) // Help Message Render
	{
		m_pTM->Draw(m_nPanelImgID, 50,50,0.8f,0.7f,0,0,0,0,D3DCOLOR_ARGB(195,255,255,255));
		m_pBF->Draw("This is the Player screen. You can",95,75,0.6f,D3DCOLOR_ARGB(255,255,255,255));
		m_pBF->Draw("view your stats and experience as ",95,95,0.6f,D3DCOLOR_ARGB(255,255,255,255));
		m_pBF->Draw("well as allocate any stat points ",95,115,0.6f,D3DCOLOR_ARGB(255,255,255,255));
		m_pBF->Draw("you gained from leveling up.",95,135,0.6f,D3DCOLOR_ARGB(255,255,255,255));
		m_pBF->Draw("You can also access the artifact",95,175,0.6f,D3DCOLOR_ARGB(255,255,255,255));
		m_pBF->Draw("leveling screen from here.",95,195,0.6f,D3DCOLOR_ARGB(255,255,255,255));

		m_pTM->Draw(m_nMenuButtonImgID, 250,400,0.1f,0.5f,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
		m_pBF->Draw("Don't show this message again",280,410,0.4f,D3DCOLOR_ARGB(255,255,255,255));

		if (PlayState::GetInstance()->GetPlayer()->GetHelpMessages() & 1 << 1)
			m_pBF->Draw("X",255,405,0.4f,D3DCOLOR_ARGB(100,0,0,0));

		m_pTM->Draw(m_nMenuButtonImgID, 350,500,0.5f,0.5f,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
		m_pBF->Draw("  Continue",360,510,0.4f,D3DCOLOR_ARGB(100,0,0,0));

		if (m_nHelpIndex == 0)
		{
			m_pTM->Draw(m_nMenuButtonImgID, 250,400,0.1f,0.5f,0,0,0,0,D3DCOLOR_ARGB(200,255,255,255));
			if (PlayState::GetInstance()->GetPlayer()->GetHelpMessages() & 1 << 1)
				m_pBF->Draw("X",255,405,0.4f,D3DCOLOR_ARGB(255,0,0,0));
		}
		else
		{
			m_pTM->Draw(m_nMenuButtonImgID, 350,500,0.5f,0.5f,0,0,0,0,D3DCOLOR_ARGB(200,255,255,255));
			m_pBF->Draw("  Continue",360,510,0.4f,D3DCOLOR_ARGB(200,0,0,0));
		}

	}
}


// ARTIFACT MODE -----------------------------------------------------------------
void CLevelUpMenu::ArtifactUpdate( float fElapsedTime )
{

	for(int i = 0; i < artCount; i++)
	{
		ArtLevels[i] = PlayState::GetInstance()->GetPlayer()->GetArtifactLevel(i);
	}

	for( int i = 0; i < 5; i++)
		artAlpha[i] = 85;

	if (artIndex != -1)
		artAlpha[artIndex] = 255;
}

bool CLevelUpMenu::ArtifactInput( void )
{
	if (ShowArtifactHelp) // Help Message Input
	{
		if (CInputManager::GetInstance()->CheckUp())
		{
			CAudioManager::GetInstance()->PlaySFX(SFX_CURSOR);
			m_nHelpIndex = 0;
		}
		if (CInputManager::GetInstance()->CheckDown())
		{
			CAudioManager::GetInstance()->PlaySFX(SFX_CURSOR);
			m_nHelpIndex = 1;
		}

		if (CInputManager::GetInstance()->CheckEnter())
		{
			if (m_nHelpIndex == 0)
			{
				CAudioManager::GetInstance()->PlaySFX(SFX_PAUSE);
				PlayState::GetInstance()->GetPlayer()->SetHelpMessages(PlayState::GetInstance()->GetPlayer()->GetHelpMessages() ^ 1<<2);
			}

			if (m_nHelpIndex == 1)
			{
				CAudioManager::GetInstance()->PlaySFX(SFX_CONFIRM);
				m_nHelpIndex = 0;
				ShowArtifactHelp = false;
			}
		}
		return true;
	}

	//Go to Player Leveling
	if (m_pIM->CheckCycleRight() || m_pIM->CheckCycleLeft())
	{
		m_bArtifactMode = false;
		m_nHelpIndex = 0;
	}

	if (artCount == 0 || PlayState::GetInstance()->GetPlayer()->GetShards() <= 0)
	{
		artIndex = -1;
		if (m_pIM->CheckEnter())
		{
			m_bArtifactMode = false;
			m_nHelpIndex = 0;
		}

		return true;
	}

	//Go UP an Index
	if (m_pIM->CheckRight())
	{
		CAudioManager::GetInstance()->PlaySFX( SFX_PAUSEMOVE );
		++artIndex;
		if (artIndex > artCount - 1)
			artIndex = -1;
	}

	//Go DOWN an Index
	if (m_pIM->CheckLeft())
	{
		CAudioManager::GetInstance()->PlaySFX( SFX_PAUSEMOVE );
		--artIndex;
		if (artIndex < -1)
			artIndex = artCount - 1;
	}

	if (artCount == 0)
		artIndex = -1;

	//Put the cursor at back when there is nothing to spend
	//if (PlayState::GetInstance()->GetPlayer()->GetUnspent() <= 0)
	//	artIndex = 4;

	//Click
	if (m_pIM->CheckEnter())
	{
		if (artIndex != 4)
			CAudioManager::GetInstance()->PlaySFX( SFX_POINT );
		else
			CAudioManager::GetInstance()->PlaySFX( SFX_PAUSE );

		switch(artIndex)
		{
		case 0: Fire(); break;
		case 1: Ice(); break;
		case 2: Earth(); break;
		case 3: Light(); break;
		default:
			{
				m_bArtifactMode = false;
				if (ShowPlayerHelp)
					CAudioManager::GetInstance()->PlaySFX(SFX_BACK);
			}
			break;
		}
	}

	return true;
}

void CLevelUpMenu::ArtifactRender( void )
{
	// Render Panel
	CSGD_TextureManager::GetInstance()->Draw(m_nPanelImgID, 25,65,0.9f,0.7f,0,0,0,0,D3DCOLOR_ARGB( 200,255,255,255));

	CPlayer* play = PlayState::GetInstance()->GetPlayer();
	m_pBF->Draw("Artifacts", 330, 30, 0.8f, D3DCOLOR_ARGB(255, 85, 255, 255));

	for (int i = 0; i < 4; ++i)
	{
		m_pTM->Draw(m_nShardImgID, 60 + (i * 170), 70,0.7f,0.7f,0,0,0,0,D3DCOLOR_ARGB(200,0,0,0));
		m_pTM->Draw(m_nAttributeButtonImgID,120 + (i * 170), 430, 0.12f,	0.12f, 0,0,0,0,	D3DCOLOR_ARGB(artAlpha[i],185,185,185));
		m_pBF->Draw("+",130 + (i * 170), 440, 0.5f,	D3DCOLOR_ARGB(artAlpha[i], 255, 255, 0));

		if (i < artCount)
		{
			switch (i)
			{
			case 0:
				m_pTM->Draw(m_nShardImgID, 60 + (i * 170), 70,0.7f,0.7f,0,0,0,0,D3DCOLOR_ARGB(100 + 50 * PlayState::GetInstance()->GetPlayer()->GetArtifactLevel(i)/3 ,200,50,50));
				break;
			case 1:
				m_pTM->Draw(m_nShardImgID, 60 + (i * 170), 70,0.7f,0.7f,0,0,0,0,D3DCOLOR_ARGB(100 + 50 * PlayState::GetInstance()->GetPlayer()->GetArtifactLevel(i)/3 ,50,50,200));
				break;
			case 2:
				m_pTM->Draw(m_nShardImgID, 60 + (i * 170), 70,0.7f,0.7f,0,0,0,0,D3DCOLOR_ARGB(100 + 50 * PlayState::GetInstance()->GetPlayer()->GetArtifactLevel(i)/3 ,139,90,43));
				break;
			case 3:
				m_pTM->Draw(m_nShardImgID, 60 + (i * 170), 70,0.7f,0.7f,0,0,0,0,D3DCOLOR_ARGB(100 + 50 * PlayState::GetInstance()->GetPlayer()->GetArtifactLevel(i)/3 ,200,200,50));
				break;
			}

		}

	}

	if (artCount > 0)
	{
		m_pTM->Draw(m_nMenuButtonImgID, 50,200,0.7f,0.45f,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
		m_pBF->Draw("Fire - " + std::to_string(PlayState::GetInstance()->GetPlayer()->GetArtifactLevel(0)),55, 200, 0.8f,	D3DCOLOR_ARGB(100, 255, 0, 0));
	}
	if (artCount > 1)
	{
		m_pTM->Draw(m_nMenuButtonImgID, 230,200,0.65f,0.45f,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
		m_pBF->Draw("Ice - " + std::to_string(PlayState::GetInstance()->GetPlayer()->GetArtifactLevel(1)),235, 200, 0.8f,	D3DCOLOR_ARGB(100, 0, 0, 255));
	}
	if (artCount > 2)
	{
		m_pTM->Draw(m_nMenuButtonImgID, 395,200,0.70f,0.45f,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
		m_pBF->Draw("Earth - " + std::to_string(PlayState::GetInstance()->GetPlayer()->GetArtifactLevel(2)),400, 200, 0.65f,	D3DCOLOR_ARGB(100, 120, 70, 23));
	}
	if (artCount > 3)
	{
		m_pTM->Draw(m_nMenuButtonImgID, 575,200,0.7f,0.45f,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
		m_pBF->Draw("Light - " + std::to_string(PlayState::GetInstance()->GetPlayer()->GetArtifactLevel(3)),580, 200, 0.7f,	D3DCOLOR_ARGB(100, 255, 255, 0));
	}

	m_pTM->Draw(m_nMenuButtonImgID,550,520,0.5f,0.5f,0,0,0,0,D3DCOLOR_ARGB(Alpha[4],185,185,185));
	m_pBF->Draw("Back", 560, 530, 0.5f, D3DCOLOR_ARGB(100, 0, 0, 0));

	switch (artIndex)
	{
	case 0:
		m_pTM->Draw(m_nMenuButtonImgID, 50,200,0.7f,0.45f,0,0,0,0,D3DCOLOR_ARGB(220,255,255,255));
		m_pBF->Draw("Fire - " + std::to_string(PlayState::GetInstance()->GetPlayer()->GetArtifactLevel(0)),55, 200, 0.8f,	D3DCOLOR_ARGB(220, 255, 0, 0));
		break;
	case 1:
		m_pTM->Draw(m_nMenuButtonImgID, 230,200,0.65f,0.45f,0,0,0,0,D3DCOLOR_ARGB(220,255,255,255));
		m_pBF->Draw("Ice - " + std::to_string(PlayState::GetInstance()->GetPlayer()->GetArtifactLevel(1)),235, 200, 0.8f,	D3DCOLOR_ARGB(220, 0, 0, 255));
		break;
	case 2:
		m_pTM->Draw(m_nMenuButtonImgID, 395,200,0.70f,0.45f,0,0,0,0,D3DCOLOR_ARGB(220,255,255,255));
		m_pBF->Draw("Earth - " + std::to_string(PlayState::GetInstance()->GetPlayer()->GetArtifactLevel(2)),400, 200, 0.65f,	D3DCOLOR_ARGB(220, 120, 70, 23));
		break;
	case 3:
		m_pTM->Draw(m_nMenuButtonImgID, 575,200,0.7f,0.45f,0,0,0,0,D3DCOLOR_ARGB(220,255,255,255));
		m_pBF->Draw("Light - " + std::to_string(PlayState::GetInstance()->GetPlayer()->GetArtifactLevel(3)),580, 200, 0.7f,	D3DCOLOR_ARGB(220, 255, 255, 0));
		break;
	default:
		m_pTM->Draw(m_nMenuButtonImgID,550,520,0.5f,0.5f,0,0,0,0,D3DCOLOR_ARGB(Alpha[4],185,185,185));
		m_pBF->Draw("Back", 560, 530, 0.5f, D3DCOLOR_ARGB(220, 0, 0, 0));
		break;
	}


	m_pBF->Draw("Shards - " + std::to_string(PlayState::GetInstance()->GetPlayer()->GetShards()),100, 500, 0.9f,	D3DCOLOR_ARGB(200, 255, 255, 255));

	if (ShowArtifactHelp) // Help Message Render
	{
		m_pTM->Draw(m_nPanelImgID, 50,50,0.8f,0.7f,0,0,0,0,D3DCOLOR_ARGB(195,255,255,255));
		m_pBF->Draw("This is the Artifact screen. If you",95,75,0.6f,D3DCOLOR_ARGB(255,255,255,255));
		m_pBF->Draw("have Shards, you can spend them to",95,95,0.6f,D3DCOLOR_ARGB(255,255,255,255));
		m_pBF->Draw("level up your artifacts up to a max",95,115,0.6f,D3DCOLOR_ARGB(255,255,255,255));
		m_pBF->Draw("level of 3.",95,135,0.6f,D3DCOLOR_ARGB(255,255,255,255));
		m_pBF->Draw("Leveling up artifacts increases the",95,175,0.6f,D3DCOLOR_ARGB(255,255,255,255));
		m_pBF->Draw("power and changes the behavior of",95,195,0.6f,D3DCOLOR_ARGB(255,255,255,255));
		m_pBF->Draw("their abilities!",95,215,0.6f,D3DCOLOR_ARGB(255,255,255,255));

		m_pTM->Draw(m_nMenuButtonImgID, 250,400,0.1f,0.5f,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
		m_pBF->Draw("Don't show this message again",280,410,0.4f,D3DCOLOR_ARGB(255,255,255,255));

		if (PlayState::GetInstance()->GetPlayer()->GetHelpMessages() & 1 << 2)
			m_pBF->Draw("X",255,405,0.4f,D3DCOLOR_ARGB(100,0,0,0));

		m_pTM->Draw(m_nMenuButtonImgID, 350,500,0.5f,0.5f,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
		m_pBF->Draw("  Continue",360,510,0.4f,D3DCOLOR_ARGB(100,0,0,0));

		if (m_nHelpIndex == 0)
		{
			m_pTM->Draw(m_nMenuButtonImgID, 250,400,0.1f,0.5f,0,0,0,0,D3DCOLOR_ARGB(200,255,255,255));
			if (PlayState::GetInstance()->GetPlayer()->GetHelpMessages() & 1 << 2)
				m_pBF->Draw("X",255,405,0.4f,D3DCOLOR_ARGB(255,0,0,0));
		}
		else
		{
			m_pTM->Draw(m_nMenuButtonImgID, 350,500,0.5f,0.5f,0,0,0,0,D3DCOLOR_ARGB(200,255,255,255));
			m_pBF->Draw("  Continue",360,510,0.4f,D3DCOLOR_ARGB(200,0,0,0));
		}

	}

}

// ARTIFACT MODE -----------------------------------------------------------------


// HELPER FUNCTIONS---------------------------------------------------------------

void CLevelUpMenu::AttributesToString( void )
{
	PlayState::GetInstance()->GetPlayer()->CalculateStats();
	Attributes[0] = std::to_string(PlayState::GetInstance()->GetPlayer()->GetBodyE());
	Attributes[1] = std::to_string(PlayState::GetInstance()->GetPlayer()->GetMindE());
	Attributes[2] = std::to_string(PlayState::GetInstance()->GetPlayer()->GetSoulE());
	Attributes[3] = std::to_string(PlayState::GetInstance()->GetPlayer()->GetCharacterE());
	Attributes[4] = std::to_string(PlayState::GetInstance()->GetPlayer()->GetUnspent());
	Attributes[5] = std::to_string(PlayState::GetInstance()->GetPlayer()->GetLevel());
	Attributes[6] = std::to_string(PlayState::GetInstance()->GetPlayer()->GetExperience()) + 
		" / " + std::to_string(PlayState::GetInstance()->GetPlayer()->GetToNextLevel());
	Attributes[7] = "HP: " + std::to_string(PlayState::GetInstance()->GetPlayer()->GetCurrHealth()) + 
		" / " + std::to_string(PlayState::GetInstance()->GetPlayer()->GetMaxHealth());
	Attributes[8] = "Melee: " + std::to_string(PlayState::GetInstance()->GetPlayer()->GetMeleeDamage());
	Attributes[9] = "Crit: " + std::to_string(PlayState::GetInstance()->GetPlayer()->GetCritChance()) + "%";
	Attributes[10] = "Speed: " + std::to_string((int)PlayState::GetInstance()->GetPlayer()->GetSpeed());
	Attributes[11] = "Dodge: " + std::to_string(PlayState::GetInstance()->GetPlayer()->GetDodgeChance()) + "%";
	Attributes[12] = "Resist: "  + std::to_string(PlayState::GetInstance()->GetPlayer()->GetResistance()) + "%";

}

void CLevelUpMenu::Body( void )
{
	if (PlayState::GetInstance()->GetPlayer()->GetUnspent() > 0)
	{
		PlayState::GetInstance()->GetPlayer()->SetUnspent(PlayState::GetInstance()->GetPlayer()->GetUnspent() - 1);
		PlayState::GetInstance()->GetPlayer()->SetBody(PlayState::GetInstance()->GetPlayer()->GetBody() + 1);
		Attributes[0] = std::to_string(PlayState::GetInstance()->GetPlayer()->GetBodyE());
		Attributes[4] = std::to_string(PlayState::GetInstance()->GetPlayer()->GetUnspent());

		Attributes[8] = "Melee: " + std::to_string(PlayState::GetInstance()->GetPlayer()->GetMeleeDamage());
		Attributes[9] = "Crit: " + std::to_string(PlayState::GetInstance()->GetPlayer()->GetCritChance()) + "%";
	}
}
void CLevelUpMenu::Mind( void )
{
	if (PlayState::GetInstance()->GetPlayer()->GetUnspent() > 0)
	{
		PlayState::GetInstance()->GetPlayer()->SetUnspent(PlayState::GetInstance()->GetPlayer()->GetUnspent() - 1);
		PlayState::GetInstance()->GetPlayer()->SetMind(PlayState::GetInstance()->GetPlayer()->GetMind() + 1);
		Attributes[1] = std::to_string(PlayState::GetInstance()->GetPlayer()->GetMindE());
		Attributes[4] = std::to_string(PlayState::GetInstance()->GetPlayer()->GetUnspent());
		Attributes[10] = "Speed: " + std::to_string((int)PlayState::GetInstance()->GetPlayer()->GetSpeed());
		Attributes[11] = "Dodge: " + std::to_string(PlayState::GetInstance()->GetPlayer()->GetDodgeChance()) + "%";
	}
}
void CLevelUpMenu::Soul( void )
{
	if (PlayState::GetInstance()->GetPlayer()->GetUnspent() > 0)
	{
		PlayState::GetInstance()->GetPlayer()->SetUnspent(PlayState::GetInstance()->GetPlayer()->GetUnspent() - 1);
		PlayState::GetInstance()->GetPlayer()->SetSoul(PlayState::GetInstance()->GetPlayer()->GetSoul() + 1);
		Attributes[2] = std::to_string(PlayState::GetInstance()->GetPlayer()->GetSoulE());
		Attributes[4] = std::to_string(PlayState::GetInstance()->GetPlayer()->GetUnspent());
		Attributes[12] = "Resist: "  + std::to_string(PlayState::GetInstance()->GetPlayer()->GetResistance()) + "%";
	}
}
void CLevelUpMenu::Char( void )
{
	if (PlayState::GetInstance()->GetPlayer()->GetUnspent() > 0)
	{
		PlayState::GetInstance()->GetPlayer()->SetUnspent(PlayState::GetInstance()->GetPlayer()->GetUnspent() - 1);
		PlayState::GetInstance()->GetPlayer()->SetCharacter(PlayState::GetInstance()->GetPlayer()->GetCharacter() + 1);
		Attributes[3] = std::to_string(PlayState::GetInstance()->GetPlayer()->GetCharacterE());
		Attributes[4] = std::to_string(PlayState::GetInstance()->GetPlayer()->GetUnspent());

		Attributes[7] = "HP: " + std::to_string(PlayState::GetInstance()->GetPlayer()->GetCurrHealth()) + 
			" / " + std::to_string(PlayState::GetInstance()->GetPlayer()->GetMaxHealth());
	}
}

void CLevelUpMenu::Fire( void )
{
	if (PlayState::GetInstance()->GetPlayer()->GetArtifactLevel(0) < 3)
	{
		PlayState::GetInstance()->GetPlayer()->LevelUpArtifact(0);
		PlayState::GetInstance()->GetPlayer()->SetShards( -1 + PlayState::GetInstance()->GetPlayer()->GetShards());
		CAudioManager::GetInstance()->PlaySFX(SFX_FIREBALL);
	}
}
void CLevelUpMenu::Earth( void )
{
	if (PlayState::GetInstance()->GetPlayer()->GetArtifactLevel(2) < 3)
	{
		PlayState::GetInstance()->GetPlayer()->LevelUpArtifact(2);
		PlayState::GetInstance()->GetPlayer()->SetShards( -1 + PlayState::GetInstance()->GetPlayer()->GetShards());
		CAudioManager::GetInstance()->PlaySFX(SFX_ROCKSMASH);
	}
}
void CLevelUpMenu::Ice( void )
{
	if (PlayState::GetInstance()->GetPlayer()->GetArtifactLevel(1) < 3)
	{
		PlayState::GetInstance()->GetPlayer()->LevelUpArtifact(1);
		PlayState::GetInstance()->GetPlayer()->SetShards( -1 + PlayState::GetInstance()->GetPlayer()->GetShards());
		CAudioManager::GetInstance()->PlaySFX(SFX_FREEZE);
	}
}
void CLevelUpMenu::Light( void )
{
	if (PlayState::GetInstance()->GetPlayer()->GetArtifactLevel(3) < 3)
	{
		PlayState::GetInstance()->GetPlayer()->LevelUpArtifact(3);
		PlayState::GetInstance()->GetPlayer()->SetShards( -1 + PlayState::GetInstance()->GetPlayer()->GetShards());
		CAudioManager::GetInstance()->PlaySFX(SFX_LDASH);
	}
}