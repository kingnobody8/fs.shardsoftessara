/*
	File:		SocketingState.cpp
	Author:		Benjamin Cowles, Mark Diaz
	Course:		SGP 1304
	Purpose:	The CSocketingState Class allows the player to
				socket items from their inventory with gems
*/

#include "SocketingState.h"
#include "BitmapFont.h"
#include "AudioManager.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "PlayState.h"
#include "Game.h"

enum ImgIDs {STATE_BACKGROUND, ITEM_BACKGROUND, ICON_BACKGROUND, INFORMATION_BACKGROUND, ALL_ITEMS_BACKGROUND};

/*static*/ CSocketingState* CSocketingState::GetInstance( void )
{
	// Easy-instantiation: use a static variable
	static CSocketingState s_Instance;

	return &s_Instance;
}

CSocketingState::CSocketingState(void)
{
}


CSocketingState::~CSocketingState(void)
{
}

/*virtual*/ void	CSocketingState::Enter ( void )
{
	m_pBF = CBitmapFont::GetInstance();
	
	this->m_pTM = CSGD_TextureManager::GetInstance();

	this->m_pBF = CBitmapFont::GetInstance();
	this->m_pPlayer = PlayState::GetInstance()->GetPlayer();

	ShowHelpMessage = !(m_pPlayer->GetHelpMessages() & 1<<3);
	m_nHelpIndex = 0;
	m_nHelpPanelID = m_pTM->LoadTexture(_T("Resource/Graphics/panel_large.png"));
	if (ShowHelpMessage)
		CAudioManager::GetInstance()->PlaySFX(SFX_BACK);

	// STATE_BACKGROUND
	this->m_vSocketingImgIDs.push_back(this->m_pTM->LoadTexture(_T("Resource/Graphics/Socket.png")));
	// ITEM_BACKGROUND
	this->m_vSocketingImgIDs.push_back(this->m_pTM->LoadTexture(_T("Resource/Graphics/Inventory/inventory_potion_background.png")));
	// ICON_BACKGROUND
	this->m_vSocketingImgIDs.push_back(this->m_pTM->LoadTexture(_T("Resource/Graphics/Inventory/inventory_icon_background.png")));
	// INFORMATION_BACKGROUND
	this->m_vSocketingImgIDs.push_back(this->m_pTM->LoadTexture(_T("Resource/Graphics/Inventory/inventory_information_background_large.png")));
	// ALL_ITEMS_BACKGROUND
	this->m_vSocketingImgIDs.push_back(this->m_pTM->LoadTexture(_T("Resource/Graphics/Inventory/inventory_all_items_background.png")));
	
	this->m_nMenuButtonImgID = this->m_pTM->LoadTexture(_T("Resource/Graphics/menubutton.png"));

	this->m_iItemToSocket = this->m_pPlayer->GetItemToSocket();

	if(this->m_pPlayer->GetGems().size() > 0)
	{
		this->m_nSelectedIndex = 1;
	}
	else
	{
		this->m_nSelectedIndex = 6;
	}

	this->m_nOffset = 0;
}

/*virtual*/ void	CSocketingState::Exit  ( void )
{
	for(unsigned int i = 0; i < this->m_vSocketingImgIDs.size(); i++)
	{
		this->m_pTM->UnloadTexture(this->m_vSocketingImgIDs[i]);
	}

	this->m_vSocketingImgIDs.clear();

	this->m_pTM->UnloadTexture(this->m_nMenuButtonImgID);
	if (m_nHelpPanelID != -1)
	{
		m_pTM->UnloadTexture( m_nHelpPanelID );
		m_nHelpPanelID = -1;
	}
}

/*virtual*/ bool	CSocketingState::Input ( void )
{
	if (ShowHelpMessage) // Help Message Input
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
				m_pPlayer->SetHelpMessages(m_pPlayer->GetHelpMessages() ^ 1<<3);
			}

			if (m_nHelpIndex == 1)
			{
				CAudioManager::GetInstance()->PlaySFX(SFX_CONFIRM);
				m_nHelpIndex = 0;
				ShowHelpMessage = false;
			}
			}
		return true;
	}

	if (CInputManager::GetInstance()->CheckEnter())
	{
		switch(this->m_nSelectedIndex)
		{
		case 0:
			{
				this->m_nOffset--;
				if(this->m_nOffset < 0)
					this->m_nOffset++;
				this->m_nSelectedIndex = 1;
			}
			break;
		case 1:
		case 2:
		case 3:
		case 4:
			{
				vector<CItem *> vWeapons = this->m_pPlayer->GetWeapons();
				vector<CItem *> vArmors = this->m_pPlayer->GetArmors();
				CItem * iEquippedWeapon;
				CItem * iEquippedArmor;

				for(unsigned int i = 0; i < vWeapons.size(); i++)
				{
					if(vWeapons[i]->Equipped())
					{
						iEquippedWeapon = vWeapons[i];
						i = vWeapons.size();
					}
				}

				for(unsigned int i = 0; i < vArmors.size(); i++)
				{
					if(vArmors[i]->Equipped())
					{
						iEquippedArmor = vArmors[i];
						i = vArmors.size();
					}
				}
				CAudioManager::GetInstance()->PlaySFX( SFX_SOCKET );
				this->m_iItemToSocket->SetGem(this->m_pPlayer->GetGems()[this->m_nSelectedIndex + this->m_nOffset * 4 - 1]);
				this->m_pPlayer->RemoveGem(this->m_pPlayer->GetGems()[this->m_nSelectedIndex + this->m_nOffset * 4 - 1]);

				/*this->m_pPlayer->SetBody(this->m_pPlayer->GetBody() - iEquippedWeapon->GetBody() - iEquippedArmor->GetBody() + this->m_iItemToSocket->GetGem()->GetBody());
				this->m_pPlayer->SetMind(this->m_pPlayer->GetMind() - iEquippedWeapon->GetMind() - iEquippedArmor->GetMind() + this->m_iItemToSocket->GetGem()->GetMind());
				this->m_pPlayer->SetSoul(this->m_pPlayer->GetSoul() - iEquippedWeapon->GetSoul() - iEquippedArmor->GetSoul() + this->m_iItemToSocket->GetGem()->GetSoul());
				this->m_pPlayer->SetCharacter(this->m_pPlayer->GetCharacter() - iEquippedWeapon->GetChar() - iEquippedArmor->GetChar() + this->m_iItemToSocket->GetGem()->GetChar());*/
				
				CGame::GetInstance()->ChangeToPrev();
			}
			break;
		case 5:
			{
				this->m_nOffset++;
				if((unsigned int)this->m_nOffset > ((float)(this->m_pPlayer->GetGems().size()) / 4.0f))
					this->m_nOffset--;
				this->m_nSelectedIndex = 1;
			}
			break;
		case 6:
			{
				CGame::GetInstance()->ChangeToPrev();
			}
			break;
		}
	}

	if(CInputManager::GetInstance()->CheckRight() || CInputManager::GetInstance()->CheckDown())
	{
		CAudioManager::GetInstance()->PlaySFX(SFX_PAUSEMOVE);
		this->m_nSelectedIndex++;
		if(this->m_nSelectedIndex > 6)
		{
			this->m_nSelectedIndex--;
		}
		
		if(this->m_nSelectedIndex == 2 && this->m_pPlayer->GetGems().size() - this->m_nOffset * 4 <= 1)
		{
			this->m_nSelectedIndex++;
		}
		
		if(this->m_nSelectedIndex == 3 && this->m_pPlayer->GetGems().size() - this->m_nOffset * 4 <= 2)
		{
			this->m_nSelectedIndex++;
		}
		
		if(this->m_nSelectedIndex == 4 && this->m_pPlayer->GetGems().size() - this->m_nOffset * 4 <= 3)
		{
			this->m_nSelectedIndex++;
		}
		
		if(this->m_nSelectedIndex == 5 && this->m_pPlayer->GetGems().size() - this->m_nOffset * 4 <= 4)
		{
			this->m_nSelectedIndex++;
		}
	}

	if(CInputManager::GetInstance()->CheckLeft() || CInputManager::GetInstance()->CheckUp())
	{
		CAudioManager::GetInstance()->PlaySFX(SFX_PAUSEMOVE);
		this->m_nSelectedIndex--;
		if(this->m_nSelectedIndex < 0)
		{
			this->m_nSelectedIndex++;
		}

		if(this->m_pPlayer->GetGems().size() == 0)
		{
			this->m_nSelectedIndex++;
		}
		
		if(this->m_nSelectedIndex == 5 && this->m_pPlayer->GetGems().size() - this->m_nOffset * 4 <= 4)
		{
			this->m_nSelectedIndex--;
		}
		
		if(this->m_nSelectedIndex == 4 && this->m_pPlayer->GetGems().size() - this->m_nOffset * 4 <= 3)
		{
			this->m_nSelectedIndex--;
		}
		
		if(this->m_nSelectedIndex == 3 && this->m_pPlayer->GetGems().size() - this->m_nOffset * 4 <= 2)
		{
			this->m_nSelectedIndex--;
		}
		
		if(this->m_nSelectedIndex == 2 && this->m_pPlayer->GetGems().size() - this->m_nOffset * 4 <= 1)
		{
			this->m_nSelectedIndex--;
		}

		if(this->m_nOffset == 0 && this->m_nSelectedIndex == 0)
		{
			this->m_nSelectedIndex++;
		}
	}
	return true;
}

/*virtual*/ void	CSocketingState::Update( float fElapsedTime )
{
	
}

/*virtual*/ void	CSocketingState::Render( void )
{
	int alpha = 100;

	// State background
	this->m_pTM->Draw(this->m_vSocketingImgIDs[STATE_BACKGROUND], 0, 0);
	
	this->m_pBF->Draw("Socket", 300, 32, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 200));

	int nX = 50;
	int nY = 155;
	this->m_pTM->Draw(this->m_vSocketingImgIDs[ITEM_BACKGROUND], nX, nY);
	this->m_pBF->Draw("Item to Socket:", nX + 10, nY + 10, 0.65f, D3DCOLOR_ARGB(255, 0, 0, 0));
	
	this->m_pTM->Draw(this->m_vSocketingImgIDs[ICON_BACKGROUND], nX + 35, nY + 35);
	this->m_pTM->Draw(this->m_vSocketingImgIDs[INFORMATION_BACKGROUND], nX + 100, nY + 35);
	this->m_pTM->Draw(this->m_iItemToSocket->GetImgID(), nX + 46, nY + 41);
	this->m_pBF->Draw(this->m_iItemToSocket->GetName(), nX + 110, nY + 45, 0.3f, D3DCOLOR_ARGB(255, 0, 0, 0));
	this->m_pBF->Draw("Damage: " + to_string(this->m_iItemToSocket->GetSpecial()), nX + 110, nY + 58, 0.3f, D3DCOLOR_ARGB(255, 0, 0, 0));
	this->m_pBF->Draw("Body +" + to_string(this->m_iItemToSocket->GetBody()), nX + 110, nY + 71, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
	this->m_pBF->Draw("Mind +" + to_string(this->m_iItemToSocket->GetMind()), nX + 175, nY + 71, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
	this->m_pBF->Draw("Soul +" + to_string(this->m_iItemToSocket->GetSoul()), nX + 110, nY + 84, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
	this->m_pBF->Draw("Char +" + to_string(this->m_iItemToSocket->GetChar()), nX + 175, nY + 84, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
	this->m_pTM->Draw(this->m_iItemToSocket->GetGemImgID(), nX + 61, nY + 60);

	// All items
	// Background
	nX = 330;
	this->m_pTM->Draw(this->m_vSocketingImgIDs[ALL_ITEMS_BACKGROUND], nX, nY);

	nY += 15;

	// Up	
	if(this->m_nSelectedIndex == 0)
	{
		alpha = 200;
	}
	else
	{
		alpha = 100;
	}

	if(this->m_nOffset != 0)
	{
		this->m_pTM->Draw(this->m_nMenuButtonImgID, nX + 270, nY - 5, 0.5f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
		this->m_pBF->Draw("Prev. Page", nX + 275, nY, 0.4f, D3DCOLOR_ARGB(alpha, 0, 0, 0));
	}

	// Down
	if(this->m_nSelectedIndex == 5)
	{
		alpha = 200;
	}
	else
	{
		alpha = 100;
	}

	if(this->m_pPlayer->GetGems().size() - this->m_nOffset * 4 > 4)
	{
		this->m_pTM->Draw(this->m_nMenuButtonImgID, nX + 270, nY + 310, 0.5f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
		this->m_pBF->Draw("Next Page", nX + 275, nY + 315, 0.4f, D3DCOLOR_ARGB(alpha, 0, 0, 0));
	}

	// Item 1
	if((int)this->m_pPlayer->GetGems().size() > 0 + 4 * this->m_nOffset)
	{
		this->m_pTM->Draw(this->m_vSocketingImgIDs[ICON_BACKGROUND], nX + 20, nY + 15);
		this->m_pTM->Draw(this->m_vSocketingImgIDs[INFORMATION_BACKGROUND], nX + 80, nY + 15);
		this->m_pTM->Draw(this->m_pPlayer->GetGems()[this->m_nOffset * 4]->GetImgID(), nX + 35, nY + 30);
		this->m_pBF->Draw("Gem Type: " + this->m_pPlayer->GetGems()[this->m_nOffset * 4]->GetName(), nX + 90, nY + 24, 0.3f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Body +" + to_string(this->m_pPlayer->GetGems()[this->m_nOffset * 4]->GetBody()), nX + 90, nY + 50, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Mind +" + to_string(this->m_pPlayer->GetGems()[this->m_nOffset * 4]->GetMind()), nX + 155, nY + 50, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Soul +" + to_string(this->m_pPlayer->GetGems()[this->m_nOffset * 4]->GetSoul()), nX + 90, nY + 63, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Char +" + to_string(this->m_pPlayer->GetGems()[this->m_nOffset * 4]->GetChar()), nX + 155, nY + 63, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));

		if(this->m_nSelectedIndex == 1)
		{
			alpha = 200;
		}
		else
		{
			alpha = 100;
		}
		this->m_pTM->Draw(this->m_nMenuButtonImgID, nX + 330, nY + 30, 0.3f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
		this->m_pBF->Draw("Insert", nX + 335, nY + 35, 0.4f, D3DCOLOR_ARGB(alpha, 0, 0, 0));
	}

	// Item 2
	if((int)this->m_pPlayer->GetGems().size() > 1 + 4 * this->m_nOffset)
	{
		this->m_pTM->Draw(this->m_vSocketingImgIDs[ICON_BACKGROUND], nX + 20, nY + 90);
		this->m_pTM->Draw(this->m_vSocketingImgIDs[INFORMATION_BACKGROUND], nX + 80, nY + 90);
		this->m_pTM->Draw(this->m_pPlayer->GetGems()[1 + this->m_nOffset * 4]->GetImgID(), nX + 35, nY + 105);
		this->m_pBF->Draw("Gem Type: " + this->m_pPlayer->GetGems()[1 + this->m_nOffset * 4]->GetName(), nX + 90, nY + 99, 0.3f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Body +" + to_string(this->m_pPlayer->GetGems()[1 + this->m_nOffset * 4]->GetBody()), nX + 90, nY + 125, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Mind +" + to_string(this->m_pPlayer->GetGems()[1 + this->m_nOffset * 4]->GetMind()), nX + 155, nY + 125, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Soul +" + to_string(this->m_pPlayer->GetGems()[1 + this->m_nOffset * 4]->GetSoul()), nX + 90, nY + 138, 0.25, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Char +" + to_string(this->m_pPlayer->GetGems()[1 + this->m_nOffset * 4]->GetChar()), nX + 155, nY + 138, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		
		if(this->m_nSelectedIndex == 2)
		{
			alpha = 200;
		}
		else
		{
			alpha = 100;
		}
		this->m_pTM->Draw(this->m_nMenuButtonImgID, nX + 330, nY + 105, 0.3f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
		this->m_pBF->Draw("Insert", nX + 335, nY + 110, 0.4f, D3DCOLOR_ARGB(alpha, 0, 0, 0));
	}

	// Item 3
	if((int)this->m_pPlayer->GetGems().size() > 2 + 4 * this->m_nOffset)
	{
		this->m_pTM->Draw(this->m_vSocketingImgIDs[ICON_BACKGROUND], nX + 20, nY + 170);
		this->m_pTM->Draw(this->m_vSocketingImgIDs[INFORMATION_BACKGROUND], nX + 80, nY + 170);
		this->m_pTM->Draw(this->m_pPlayer->GetGems()[2 + this->m_nOffset * 4]->GetImgID(), nX + 35, nY + 185);
		this->m_pBF->Draw("Gem Type: " + this->m_pPlayer->GetGems()[2 + this->m_nOffset * 4]->GetName(), nX + 90, nY + 179, 0.3f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Body +" + to_string(this->m_pPlayer->GetGems()[2 + this->m_nOffset * 4]->GetBody()), nX + 90, nY + 205, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Mind +" + to_string(this->m_pPlayer->GetGems()[2 + this->m_nOffset * 4]->GetMind()), nX + 155, nY + 205, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Soul +" + to_string(this->m_pPlayer->GetGems()[2 + this->m_nOffset * 4]->GetSoul()), nX + 90, nY + 218, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Char +" + to_string(this->m_pPlayer->GetGems()[2 + this->m_nOffset * 4]->GetChar()), nX + 155, nY + 218, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));

		if(this->m_nSelectedIndex == 3)
		{
			alpha = 200;
		}
		else
		{
			alpha = 100;
		}
		this->m_pTM->Draw(this->m_nMenuButtonImgID, nX + 330, nY + 185, 0.3f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
		this->m_pBF->Draw("Insert", nX + 335, nY + 190, 0.4f, D3DCOLOR_ARGB(alpha, 0, 0, 0));
	}

	// Item 4
	if((int)this->m_pPlayer->GetGems().size() > 3 + 4 * this->m_nOffset)
	{
		this->m_pTM->Draw(this->m_vSocketingImgIDs[ICON_BACKGROUND], nX + 20, nY + 255);
		this->m_pTM->Draw(this->m_vSocketingImgIDs[INFORMATION_BACKGROUND], nX + 80, nY + 255);
		this->m_pTM->Draw(this->m_pPlayer->GetGems()[3 + this->m_nOffset * 4]->GetImgID(), nX + 35, nY + 270);
		this->m_pBF->Draw("Gem Type: " + this->m_pPlayer->GetGems()[3 + this->m_nOffset * 4]->GetName(), nX + 90, nY + 264, 0.3f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Body +" + to_string(this->m_pPlayer->GetGems()[3 + this->m_nOffset * 4]->GetBody()), nX + 90, nY + 290, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Mind +" + to_string(this->m_pPlayer->GetGems()[3 + this->m_nOffset * 4]->GetMind()), nX + 155, nY + 290, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Soul +" + to_string(this->m_pPlayer->GetGems()[3 + this->m_nOffset * 4]->GetSoul()), nX + 90, nY + 306, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Char +" + to_string(this->m_pPlayer->GetGems()[3 + this->m_nOffset * 4]->GetChar()), nX + 155, nY + 306, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));

		if(this->m_nSelectedIndex == 4)
		{
			alpha = 200;
		}
		else
		{
			alpha = 100;
		}
		this->m_pTM->Draw(this->m_nMenuButtonImgID, nX + 330, nY + 270, 0.3f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
		this->m_pBF->Draw("Insert", nX + 335, nY + 275, 0.4f, D3DCOLOR_ARGB(alpha, 0, 0, 0));
	}
	
	// Back
	if(this->m_nSelectedIndex == 6)
	{
		alpha = 200;
	}
	else
	{
		alpha = 100;
	}

	this->m_pTM->Draw(this->m_nMenuButtonImgID, 620, 535, 0.5f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
	this->m_pBF->Draw("Back", 630, 540, 0.65f, D3DCOLOR_ARGB(alpha, 0, 0, 0));

	if (ShowHelpMessage) // Help Message Render
	{
		m_pTM->Draw(m_nHelpPanelID, 50,50,0.8f,0.7f,0,0,0,0,D3DCOLOR_ARGB(195,255,255,255));
		m_pBF->Draw("This is the Socketing screen. You can",95,75,0.6f,D3DCOLOR_ARGB(255,255,255,255));
		m_pBF->Draw("socket your weapons and armor with   ",95,95,0.6f,D3DCOLOR_ARGB(255,255,255,255));
		m_pBF->Draw("any gems you have found to boost their",95,115,0.6f,D3DCOLOR_ARGB(255,255,255,255));
		m_pBF->Draw("bonuses.",95,135,0.6f,D3DCOLOR_ARGB(255,255,255,255));
		m_pBF->Draw("Be careful though! Socketing a gem",95,175,0.6f,D3DCOLOR_ARGB(255,255,255,255));
		m_pBF->Draw("cannot be undone!",95,195,0.6f,D3DCOLOR_ARGB(255,255,255,255));

		m_pTM->Draw(m_nMenuButtonImgID, 250,400,0.1f,0.5f,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
		m_pBF->Draw("Don't show this message again",280,410,0.4f,D3DCOLOR_ARGB(255,255,255,255));

		if (m_pPlayer->GetHelpMessages() & 1<<3)
			m_pBF->Draw("X",255,405,0.4f,D3DCOLOR_ARGB(100,0,0,0));

		m_pTM->Draw(m_nMenuButtonImgID, 350,500,0.5f,0.5f,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
		m_pBF->Draw("  Continue",360,510,0.4f,D3DCOLOR_ARGB(100,0,0,0));

		if (m_nHelpIndex == 0)
		{
			m_pTM->Draw(m_nMenuButtonImgID, 250,400,0.1f,0.5f,0,0,0,0,D3DCOLOR_ARGB(200,255,255,255));
			if (m_pPlayer->GetHelpMessages() & 1<<3)
				m_pBF->Draw("X",255,405,0.4f,D3DCOLOR_ARGB(255,0,0,0));
		}
		else
		{
			m_pTM->Draw(m_nMenuButtonImgID, 350,500,0.5f,0.5f,0,0,0,0,D3DCOLOR_ARGB(200,255,255,255));
			m_pBF->Draw("  Continue",360,510,0.4f,D3DCOLOR_ARGB(200,0,0,0));
		}

	}
}