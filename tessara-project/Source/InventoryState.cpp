/*
File:		InventoryState.cpp
Author:		Benjamin Cowles, Mark Diaz
Course:		SGP 1304
Purpose:	The CInventoryState Class allows the player to
Interact with their inventory
*/

#include "InventoryState.h"
#include "BitmapFont.h"
#include "Player.h"
#include "PlayState.h"
#include "AudioManager.h"
#include "Item.h"
#include "InputManager.h"
#include "ItemManager.h"
#include "Game.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "SocketingState.h"

enum ImgIDs {STATE_BACKGROUND, EQUIPPED_BACKGROUND, ICON_BACKGROUND, INFORMATION_BACKGROUND, POTION_BACKGROUND, ALL_ITEMS_BACKGROUND};

/*static*/ CInventoryState* CInventoryState::GetInstance( void )
{
	// Easy-instantiation: use a static variable
	static CInventoryState s_Instance;

	return &s_Instance;
}

CInventoryState::CInventoryState(void)
{

}


CInventoryState::~CInventoryState(void)
{

}

/*virtual*/ void	CInventoryState::Enter ( void )
{
	this->m_pTM = CSGD_TextureManager::GetInstance();

	this->m_pBF = CBitmapFont::GetInstance();
	this->m_pPlayer = PlayState::GetInstance()->GetPlayer();

	// Help Message Assets
	ShowHelpMessage = !(m_pPlayer->GetHelpMessages() & 1);
	if (ShowHelpMessage)
		CAudioManager::GetInstance()->PlaySFX(SFX_BACK);
	m_nHelpIndex = 0;
	m_nHelpPanelID = m_pTM->LoadTexture(_T("Resource/Graphics/panel_large.png"));

	// Inventory Assets
	this->m_vInventoryIDs.push_back(this->m_pTM->LoadTexture(_T("Resource/Graphics/Inventory/inv_Background.jpg")));
	this->m_vInventoryIDs.push_back(this->m_pTM->LoadTexture(_T("Resource/Graphics/Inventory/inventory_equipped_background.png")));
	this->m_vInventoryIDs.push_back(this->m_pTM->LoadTexture(_T("Resource/Graphics/Inventory/inventory_icon_background.png")));
	this->m_vInventoryIDs.push_back(this->m_pTM->LoadTexture(_T("Resource/Graphics/Inventory/inventory_information_background_large.png")));
	this->m_vInventoryIDs.push_back(this->m_pTM->LoadTexture(_T("Resource/Graphics/Inventory/inventory_potion_background.png")));
	this->m_vInventoryIDs.push_back(this->m_pTM->LoadTexture(_T("Resource/Graphics/Inventory/inventory_all_items_background.png")));

	this->m_nMenuButtonImgID = this->m_pTM->LoadTexture(_T("Resource/Graphics/menubutton.png"));
	this->m_nPanelImgID = this->m_pTM->LoadTexture(_T("Resource/Graphics/Inventory/inv_Panels.png"));
	this->m_nSelectedIndex = 1;
	this->m_nOffset = 0;
}

/*virtual*/ void	CInventoryState::Exit  ( void )
{
	for(unsigned int i = 0; i < this->m_vInventoryIDs.size(); i++)
	{
		this->m_pTM->UnloadTexture(this->m_vInventoryIDs[i]);
	}

	m_pTM->UnloadTexture( m_nPanelImgID );

	this->m_vInventoryIDs
		;

	this->m_pTM->UnloadTexture(this->m_nMenuButtonImgID);
	if (m_nHelpPanelID != -1)
	{
		m_pTM->UnloadTexture( m_nHelpPanelID );
		m_nHelpPanelID = -1;
	}

	m_pPlayer->CalculateStats();
}

/*virtual*/ bool	CInventoryState::Input ( void )
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
				m_pPlayer->SetHelpMessages(m_pPlayer->GetHelpMessages() ^ 1);
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

		switch(this->m_nSelectedIndex)
		{
		case 0:
			{
				this->m_nOffset--;
				if(this->m_nOffset < 0)
					this->m_nOffset++;
				this->m_nSelectedIndex = 1;

				/////////////////////////////////////////////////////////////////
				// BUG FIX
				// Reference Bug # BB-077
				// BUG FIX START
				/////////////////////////////////////////////////////////////////

				// Play sound effect
				CAudioManager::GetInstance()->PlaySFX( SFX_PAUSE );

				///////////////////////////////////////////////////////////////////////////
				// BUG FIX END  Reference # BB-077
				//////////////////////////////////////////////////////////////////////////
			}
			break;
		case 1:
		case 3:
		case 5:
		case 7:
			{
				if(this->m_pPlayer->GetEquippableItems()[this->m_nSelectedIndex / 2 + this->m_nOffset * 4]->GetType() == 0)
				{
					CAudioManager::GetInstance()->PlaySFX(SFX_WEAPONBUY);	
					iEquippedWeapon->SetEquipped(false);
				}
				else
				{
					CAudioManager::GetInstance()->PlaySFX(SFX_ARMORBUY);	
					iEquippedArmor->SetEquipped(false);
				}

				this->m_pPlayer->GetEquippableItems()[this->m_nSelectedIndex / 2 + this->m_nOffset * 4]->SetEquipped(true);

				/*this->m_pPlayer->SetBody(this->m_pPlayer->GetBody() - iEquippedWeapon->GetBody() - iEquippedArmor->GetBody());
				this->m_pPlayer->SetMind(this->m_pPlayer->GetMind() - iEquippedWeapon->GetMind() - iEquippedArmor->GetMind());
				this->m_pPlayer->SetSoul(this->m_pPlayer->GetSoul() - iEquippedWeapon->GetSoul() - iEquippedArmor->GetSoul());
				this->m_pPlayer->SetCharacter(this->m_pPlayer->GetCharacter() - iEquippedWeapon->GetChar() - iEquippedArmor->GetChar());*/
			}
			break;
		case 2:
		case 4:
		case 6:
		case 8:
			{
				if(!this->m_pPlayer->GetEquippableItems()[this->m_nSelectedIndex / 2 - 1 + this->m_nOffset * 4]->HasGem())
				{
					CAudioManager::GetInstance()->PlaySFX(SFX_PAUSE);
					this->m_pPlayer->SetItemToSocket(this->m_pPlayer->GetEquippableItems()[this->m_nSelectedIndex / 2 - 1 + this->m_nOffset * 4]);
					CGame::GetInstance()->ChangeToNew(CSocketingState::GetInstance());
				}
			}
			break;
		case 9:
			{
				this->m_nOffset++;
				if((unsigned int)this->m_nOffset > ((float)(this->m_pPlayer->GetEquippableItems().size()) / 4.0f))
					this->m_nOffset--;
				this->m_nSelectedIndex = 1;

				/////////////////////////////////////////////////////////////////
				// BUG FIX
				// Reference Bug # BB-077
				// BUG FIX START
				/////////////////////////////////////////////////////////////////

				// Play sound effect
				CAudioManager::GetInstance()->PlaySFX( SFX_PAUSE );

				///////////////////////////////////////////////////////////////////////////
				// BUG FIX END  Reference # BB-077
				//////////////////////////////////////////////////////////////////////////
			}
			break;
		case 10:
			{
				CAudioManager::GetInstance()->PlaySFX(SFX_PAUSE);
				CGame::GetInstance()->ChangeToPrev();
			}
			break;
		}
	}

	if(CInputManager::GetInstance()->CheckRight())
	{
		CAudioManager::GetInstance()->PlaySFX(SFX_PAUSEMOVE);
		this->m_nSelectedIndex++;
		if(this->m_nSelectedIndex > 10)
		{
			this->m_nSelectedIndex--;
			if((float)((this->m_pPlayer->GetEquippableItems().size()) / 4.0f) <= this->m_nOffset)
			{
				this->m_nSelectedIndex++;
			}
		}
				
		if(this->m_nSelectedIndex == 2 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 < 1)
		{
			this->m_nSelectedIndex++;
		}
		
		if(this->m_nSelectedIndex == 3 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 < 2)
		{
			this->m_nSelectedIndex++;
		}
		
		if(this->m_nSelectedIndex == 4 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 < 2)
		{
			this->m_nSelectedIndex++;
		}
		
		if(this->m_nSelectedIndex == 5 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 < 3)
		{
			this->m_nSelectedIndex++;
		}
				
		if(this->m_nSelectedIndex == 6 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 < 3)
		{
			this->m_nSelectedIndex++;
		}
		
		if(this->m_nSelectedIndex == 7 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 < 4)
		{
			this->m_nSelectedIndex++;
		}
		
		if(this->m_nSelectedIndex == 8 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 < 4)
		{
			this->m_nSelectedIndex++;
		}
		
		if(this->m_nSelectedIndex == 9 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset <= 4)
		{
			this->m_nSelectedIndex++;
		}
	}
	
	if(CInputManager::GetInstance()->CheckDown())
	{
		CAudioManager::GetInstance()->PlaySFX(SFX_PAUSEMOVE);
		
		this->m_nSelectedIndex++;

		if(this->m_nSelectedIndex != 1 && this->m_nSelectedIndex != 9)
		{
			this->m_nSelectedIndex++;
		}

		if(this->m_nSelectedIndex > 10)
		{
			this->m_nSelectedIndex--;

			if(this->m_nSelectedIndex != 10)
			{
				this->m_nSelectedIndex--;
			}

			if((float)((this->m_pPlayer->GetEquippableItems().size()) / 4.0f) <= this->m_nOffset)
			{
				this->m_nSelectedIndex++;
			}
		}
				
		if(this->m_nSelectedIndex == 2 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 < 1)
		{
			this->m_nSelectedIndex++;
		}
		
		if(this->m_nSelectedIndex == 3 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 < 2)
		{
			this->m_nSelectedIndex++;
		}
		
		if(this->m_nSelectedIndex == 4 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 < 2)
		{
			this->m_nSelectedIndex++;
		}
		
		if(this->m_nSelectedIndex == 5 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 < 3)
		{
			this->m_nSelectedIndex++;
		}
				
		if(this->m_nSelectedIndex == 6 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 < 3)
		{
			this->m_nSelectedIndex++;
		}
		
		if(this->m_nSelectedIndex == 7 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 < 4)
		{
			this->m_nSelectedIndex++;
		}
		
		if(this->m_nSelectedIndex == 8 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 < 4)
		{
			this->m_nSelectedIndex++;
		}
		
		if(this->m_nSelectedIndex == 9 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset <= 4)
		{
			this->m_nSelectedIndex++;
		}
	}

	if(CInputManager::GetInstance()->CheckLeft())
	{
		CAudioManager::GetInstance()->PlaySFX(SFX_PAUSEMOVE);
		this->m_nSelectedIndex--;
		if(this->m_nSelectedIndex < 0)
		{
			this->m_nSelectedIndex++;
		}
		if(this->m_nOffset == 0 && this->m_nSelectedIndex == 0)
		{
			this->m_nSelectedIndex++;
		}
		
		if(this->m_nSelectedIndex == 9 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 <= 4)
		{
			this->m_nSelectedIndex--;
		}
		
		if(this->m_nSelectedIndex == 8 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 < 4)
		{
			this->m_nSelectedIndex--;
		}
		
		if(this->m_nSelectedIndex == 7 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 < 4)
		{
			this->m_nSelectedIndex--;
		}
		
		if(this->m_nSelectedIndex == 6 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 < 3)
		{
			this->m_nSelectedIndex--;
		}		
		
		if(this->m_nSelectedIndex == 5 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 < 3)
		{
			this->m_nSelectedIndex--;
		}
		
		if(this->m_nSelectedIndex == 4 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 < 2)
		{
			this->m_nSelectedIndex--;
		}
		
		if(this->m_nSelectedIndex == 3 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 < 2)
		{
			this->m_nSelectedIndex--;
		}
		
		if(this->m_nSelectedIndex == 2 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 < 1)
		{
			this->m_nSelectedIndex--;
		}

		if(this->m_nOffset == 0 && this->m_nSelectedIndex == 0)
		{
			this->m_nSelectedIndex++;
		}
	}
	
	if(CInputManager::GetInstance()->CheckUp())
	{
		CAudioManager::GetInstance()->PlaySFX(SFX_PAUSEMOVE);
		this->m_nSelectedIndex--;
		
		if(m_nSelectedIndex != 9)
		{
			this->m_nSelectedIndex--;
		}

		if(this->m_nSelectedIndex < 0)
		{
			this->m_nSelectedIndex++;

			if(this->m_nSelectedIndex != 0)
			{
				this->m_nSelectedIndex++;
			}
		}
		if(this->m_nOffset == 0 && this->m_nSelectedIndex == 0)
		{
			this->m_nSelectedIndex++;
		}
		
		if(this->m_nSelectedIndex == 9 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 <= 4)
		{
			this->m_nSelectedIndex--;
		}
		
		if(this->m_nSelectedIndex == 8 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 < 4)
		{
			this->m_nSelectedIndex--;
		}
		
		if(this->m_nSelectedIndex == 7 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 < 4)
		{
			this->m_nSelectedIndex--;
		}
		
		if(this->m_nSelectedIndex == 6 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 < 3)
		{
			this->m_nSelectedIndex--;
		}		
		
		if(this->m_nSelectedIndex == 5 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 < 3)
		{
			this->m_nSelectedIndex--;
		}
		
		if(this->m_nSelectedIndex == 4 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 < 2)
		{
			this->m_nSelectedIndex--;
		}
		
		if(this->m_nSelectedIndex == 3 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 < 2)
		{
			this->m_nSelectedIndex--;
		}
		
		if(this->m_nSelectedIndex == 2 && this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 < 1)
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

/*virtual*/ void	CInventoryState::Update( float fElapsedTime )
{

}

/*virtual*/ void	CInventoryState::Render( void )
{
	int alpha;

	// State background
	this->m_pTM->Draw(this->m_vInventoryIDs[STATE_BACKGROUND], 0, 0);

	this->m_pTM->Draw(this->m_nPanelImgID, 0, 0,1.0f,1.0f,0,0,0,0,D3DCOLOR_ARGB(180,255,255,255));

	this->m_pBF->Draw("Inventory", 300, 32, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 200));

	// Equipped background
	int nX = 50;
	int nY = 155;
	//this->m_pTM->Draw(this->m_vInventoryIDs[EQUIPPED_BACKGROUND], nX, nY);
	this->m_pBF->Draw("Equipped", nX + 45, nY + 10, 0.75f, D3DCOLOR_ARGB(255, 0, 0, 0));

	// Equipped weapon
	// Background
	//this->m_pTM->Draw(this->m_vInventoryIDs[ICON_BACKGROUND], nX + 35, nY + 75);
	//this->m_pTM->Draw(this->m_vInventoryIDs[INFORMATION_BACKGROUND], nX + 100, nY + 75);
	this->m_pBF->Draw("Weapon", nX + 15, nY + 50, 0.6f, D3DCOLOR_ARGB(255, 100, 0, 0));

	// Item
	vector<CItem *> vWeapons = this->m_pPlayer->GetWeapons();
	CItem * iEquippedWeapon;

	for(unsigned int i = 0; i < vWeapons.size(); i++)
	{
		if(vWeapons[i]->Equipped())
		{
			iEquippedWeapon = vWeapons[i];
			i = vWeapons.size();
		}
	}

	this->m_pTM->Draw(iEquippedWeapon->GetImgID(), nX + 46, nY + 81);
	this->m_pBF->Draw(iEquippedWeapon->GetName(), nX + 105, nY + 80, 0.3f, D3DCOLOR_ARGB(255, 0, 0, 0));
	this->m_pBF->Draw("Damage: " + to_string(iEquippedWeapon->GetSpecial()), nX + 105, nY + 93, 0.3f, D3DCOLOR_ARGB(255, 0, 0, 0));
	this->m_pBF->Draw("Body +" + to_string(iEquippedWeapon->GetBody()), nX + 105, nY + 106, 0.3f, D3DCOLOR_ARGB(255, 0, 0, 0));
	this->m_pBF->Draw("Mind +" + to_string(iEquippedWeapon->GetMind()), nX + 185, nY + 106, 0.3f, D3DCOLOR_ARGB(255, 0, 0, 0));
	this->m_pBF->Draw("Soul +" + to_string(iEquippedWeapon->GetSoul()), nX + 105, nY + 119, 0.3f, D3DCOLOR_ARGB(255, 0, 0, 0));
	this->m_pBF->Draw("Char +" + to_string(iEquippedWeapon->GetChar()), nX + 185, nY + 119, 0.3f, D3DCOLOR_ARGB(255, 0, 0, 0));
	if(iEquippedWeapon->HasGem())
	{
		this->m_pTM->Draw(iEquippedWeapon->GetGem()->GetImgID(), nX + 61, nY + 100);
	}
	else
	{
		this->m_pTM->Draw(iEquippedWeapon->GetGemImgID(), nX + 61, nY + 100);
	}


	// Equipped armor
	// Background
	//this->m_pTM->Draw(this->m_vInventoryIDs[ICON_BACKGROUND], nX + 35, nY + 165);
	//this->m_pTM->Draw(this->m_vInventoryIDs[INFORMATION_BACKGROUND], nX + 100, nY + 165);
	this->m_pBF->Draw("Armor", nX + 15, nY + 140, 0.6f, D3DCOLOR_ARGB(255, 100, 0, 0));

	// Item
	vector<CItem *> vArmors = this->m_pPlayer->GetArmors();
	CItem * iEquippedArmor;

	for(unsigned int i = 0; i < vArmors.size(); i++)
	{
		if(vArmors[i]->Equipped())
		{
			iEquippedArmor = vArmors[i];
			i = vArmors.size();
		}
	}

	this->m_pTM->Draw(iEquippedArmor->GetImgID(), nX + 46, nY + 171);
	this->m_pBF->Draw(iEquippedArmor->GetName(), nX + 105, nY + 170, 0.3f, D3DCOLOR_ARGB(255, 0, 0, 0));
	this->m_pBF->Draw("Armor: " + to_string(iEquippedArmor->GetSpecial()), nX + 105, nY + 183, 0.3f, D3DCOLOR_ARGB(255, 0, 0, 0));
	this->m_pBF->Draw("Body +" + to_string(iEquippedArmor->GetBody()), nX + 105, nY + 196, 0.3f, D3DCOLOR_ARGB(255, 0, 0, 0));
	this->m_pBF->Draw("Mind +" + to_string(iEquippedArmor->GetMind()), nX + 185, nY + 196, 0.3f, D3DCOLOR_ARGB(255, 0, 0, 0));
	this->m_pBF->Draw("Soul +" + to_string(iEquippedArmor->GetSoul()), nX + 105, nY + 209, 0.3f, D3DCOLOR_ARGB(255, 0, 0, 0));
	this->m_pBF->Draw("Char +" + to_string(iEquippedArmor->GetChar()), nX + 185, nY + 209, 0.3f, D3DCOLOR_ARGB(255, 0, 0, 0));
	if(iEquippedArmor->HasGem())
	{
		this->m_pTM->Draw(iEquippedArmor->GetGem()->GetImgID(), nX + 61, nY + 190);
	}
	else
	{
		this->m_pTM->Draw(iEquippedArmor->GetGemImgID(), nX + 61, nY + 190);
	}

	// Potion
	nY = 415;
	//this->m_pTM->Draw(this->m_vInventoryIDs[POTION_BACKGROUND], nX, nY);
	nY = 155;
	//this->m_pTM->Draw(this->m_vInventoryIDs[ICON_BACKGROUND], nX + 35, nY + 290);
	//this->m_pTM->Draw(this->m_vInventoryIDs[INFORMATION_BACKGROUND], nX + 100, nY + 290);
	this->m_pBF->Draw("Potion x" + to_string(this->m_pPlayer->GetPotionCount()), nX + 40, nY + 275, 0.75f, D3DCOLOR_ARGB(255, 0, 0, 0));

	// Currency
	//this->m_pTM->Draw(this->m_vInventoryIDs[POTION_BACKGROUND], nX, nY + 370, 1.0f, 0.3f);
	this->m_pBF->Draw(to_string(this->m_pPlayer->GetCurrency()), nX + 60, nY + 388, 0.7f, D3DCOLOR_ARGB(255, 255, 255, 0));

	// Keys
	this->m_pBF->Draw(to_string(this->m_pPlayer->GetKeys()), nX + 410, nY + 388, 0.7f, D3DCOLOR_ARGB(255, 255, 255, 0));

	// Item
	CItem * iPotion = CItemManager::GetInstance()->GetItem(0);

	this->m_pTM->Draw(iPotion->GetImgID(), nX + 56, nY + 306);
	this->m_pBF->Draw("Heals " + to_string(iPotion->GetSpecial()) + "%", nX + 105, nY + 305, 0.4f, D3DCOLOR_ARGB(255, 0, 0, 0));
	this->m_pBF->Draw("of max health", nX + 105, nY + 318, 0.4f, D3DCOLOR_ARGB(255, 0, 0, 0));
	//this->m_pBF->Draw("Count: " + to_string(this->m_pPlayer->GetPotionCount()), nX + 110, nY + 339, 0.3f, D3DCOLOR_ARGB(255, 0, 0, 0));

	if(this->m_nSelectedIndex == 10)
	{
		alpha = 200;
	}
	else
	{
		alpha = 100;
	}

	this->m_pTM->Draw(this->m_nMenuButtonImgID, 620, 535, 0.5f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
	this->m_pBF->Draw("Back", 630, 540, 0.65f, D3DCOLOR_ARGB(alpha, 0, 0, 0));

	// All items
	// Background
	nX = 330;
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
	if(this->m_nSelectedIndex == 9)
	{
		alpha = 200;
	}
	else
	{
		alpha = 100;
	}

	if(this->m_pPlayer->GetEquippableItems().size() - this->m_nOffset * 4 > 4)
	{
		this->m_pTM->Draw(this->m_nMenuButtonImgID, nX + 270, nY + 310, 0.5f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
		this->m_pBF->Draw("Next Page", nX + 275, nY + 315, 0.4f, D3DCOLOR_ARGB(alpha, 0, 0, 0));
	}

	// Item 1
	if((int)this->m_pPlayer->GetEquippableItems().size() > 0 + 4 * this->m_nOffset)
	{
		this->m_pTM->Draw(this->m_vInventoryIDs[ICON_BACKGROUND], nX + 20, nY + 15);
		this->m_pTM->Draw(this->m_vInventoryIDs[INFORMATION_BACKGROUND], nX + 80, nY + 15);
		this->m_pTM->Draw(this->m_pPlayer->GetEquippableItems()[this->m_nOffset * 4]->GetImgID(), nX + 26, nY + 20);
		this->m_pBF->Draw(this->m_pPlayer->GetEquippableItems()[this->m_nOffset * 4]->GetName(), nX + 90, nY + 24, 0.3f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Damage: " + to_string(this->m_pPlayer->GetEquippableItems()[this->m_nOffset * 4]->GetSpecial()), nX + 90, nY + 37, 0.3f, D3DCOLOR_ARGB(255, 0, 0, 0));
		
		/////////////////////////////////////////////////////////////////
		// BUG FIX
		// Reference Bug # BB-051, Part 1/4
		// BUG FIX START
		/////////////////////////////////////////////////////////////////
					
		// Shrank text size from 0.3f to 0.25f and moved Mind and Character offset to the left 15px from 170 to 155
		this->m_pBF->Draw("Body +" + to_string(this->m_pPlayer->GetEquippableItems()[this->m_nOffset * 4]->GetBody()), nX + 90, nY + 50, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Mind +" + to_string(this->m_pPlayer->GetEquippableItems()[this->m_nOffset * 4]->GetMind()), nX + 155, nY + 50, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Soul +" + to_string(this->m_pPlayer->GetEquippableItems()[this->m_nOffset * 4]->GetSoul()), nX + 90, nY + 63, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Char +" + to_string(this->m_pPlayer->GetEquippableItems()[this->m_nOffset * 4]->GetChar()), nX + 155, nY + 63, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		
		///////////////////////////////////////////////////////////////////////////
		// BUG FIX END  Reference # BB-051, Part 1/4
		//////////////////////////////////////////////////////////////////////////

		if(this->m_pPlayer->GetEquippableItems()[this->m_nOffset * 4]->HasGem())
		{
			this->m_pTM->Draw(this->m_pPlayer->GetEquippableItems()[this->m_nOffset * 4]->GetGem()->GetImgID(), nX + 41, nY + 39);
		}
		else
		{
			this->m_pTM->Draw(this->m_pPlayer->GetEquippableItems()[this->m_nOffset * 4]->GetGemImgID(), nX + 41, nY + 39);
		}

		if(this->m_nSelectedIndex == 1)
		{
			alpha = 200;
		}
		else
		{
			alpha = 100;
		}
		this->m_pTM->Draw(this->m_nMenuButtonImgID, nX + 245, nY + 30, 0.25f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
		this->m_pBF->Draw("Equip", nX + 250, nY + 35, 0.4f, D3DCOLOR_ARGB(alpha, 0, 0, 0));

		if(this->m_nSelectedIndex == 2)
		{
			alpha = 200;
		}
		else
		{
			alpha = 100;
		}
		this->m_pTM->Draw(this->m_nMenuButtonImgID, nX + 320, nY + 30, 0.3f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
		this->m_pBF->Draw("Socket", nX + 325, nY + 35, 0.4f, D3DCOLOR_ARGB(alpha, 0, 0, 0));
	}

	// Item 2
	if((int)this->m_pPlayer->GetEquippableItems().size() > 1 + 4 * this->m_nOffset)
	{
		this->m_pTM->Draw(this->m_vInventoryIDs[ICON_BACKGROUND], nX + 20, nY + 90);
		this->m_pTM->Draw(this->m_vInventoryIDs[INFORMATION_BACKGROUND], nX + 80, nY + 90);
		this->m_pTM->Draw(this->m_pPlayer->GetEquippableItems()[1 + this->m_nOffset * 4]->GetImgID(), nX + 26, nY + 95);
		this->m_pBF->Draw(this->m_pPlayer->GetEquippableItems()[1 + this->m_nOffset * 4]->GetName(), nX + 90, nY + 99, 0.3f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Damage: " + to_string(this->m_pPlayer->GetEquippableItems()[1 + this->m_nOffset * 4]->GetSpecial()), nX + 90, nY + 112, 0.3f, D3DCOLOR_ARGB(255, 0, 0, 0));
		
		/////////////////////////////////////////////////////////////////
		// BUG FIX
		// Reference Bug # BB-051, Part 2/4
		// BUG FIX START
		/////////////////////////////////////////////////////////////////

		// Shrank text size from 0.3f to 0.25f and moved Mind and Character offset to the left 15px from 170 to 155
		this->m_pBF->Draw("Body +" + to_string(this->m_pPlayer->GetEquippableItems()[1 + this->m_nOffset * 4]->GetBody()), nX + 90, nY + 125, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Mind +" + to_string(this->m_pPlayer->GetEquippableItems()[1 + this->m_nOffset * 4]->GetMind()), nX + 155, nY + 125, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Soul +" + to_string(this->m_pPlayer->GetEquippableItems()[1 + this->m_nOffset * 4]->GetSoul()), nX + 90, nY + 138, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Char +" + to_string(this->m_pPlayer->GetEquippableItems()[1 + this->m_nOffset * 4]->GetChar()), nX + 155, nY + 138, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		
		///////////////////////////////////////////////////////////////////////////
		// BUG FIX END  Reference # BB-051, Part 2/4
		//////////////////////////////////////////////////////////////////////////

		if(this->m_pPlayer->GetEquippableItems()[1 + this->m_nOffset * 4]->HasGem())
		{
			this->m_pTM->Draw(this->m_pPlayer->GetEquippableItems()[1 + this->m_nOffset * 4]->GetGem()->GetImgID(), nX + 41, nY + 109);
		}
		else
		{
			this->m_pTM->Draw(this->m_pPlayer->GetEquippableItems()[1 + this->m_nOffset * 4]->GetGemImgID(), nX + 41, nY + 109);
		}

		if(this->m_nSelectedIndex == 3)
		{
			alpha = 200;
		}
		else
		{
			alpha = 100;
		}
		this->m_pTM->Draw(this->m_nMenuButtonImgID, nX + 245, nY + 105, 0.25f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
		this->m_pBF->Draw("Equip", nX + 250, nY + 110, 0.4f, D3DCOLOR_ARGB(alpha, 0, 0, 0));

		if(this->m_nSelectedIndex == 4)
		{
			alpha = 200;
		}
		else
		{
			alpha = 100;
		}
		this->m_pTM->Draw(this->m_nMenuButtonImgID, nX + 320, nY + 105, 0.3f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
		this->m_pBF->Draw("Socket", nX + 325, nY + 110, 0.4f, D3DCOLOR_ARGB(alpha, 0, 0, 0));
	}

	// Item 3
	if((int)this->m_pPlayer->GetEquippableItems().size() > 2 + 4 * this->m_nOffset)
	{
		this->m_pTM->Draw(this->m_vInventoryIDs[ICON_BACKGROUND], nX + 20, nY + 170);
		this->m_pTM->Draw(this->m_vInventoryIDs[INFORMATION_BACKGROUND], nX + 80, nY + 170);
		this->m_pTM->Draw(this->m_pPlayer->GetEquippableItems()[2 + this->m_nOffset * 4]->GetImgID(), nX + 26, nY + 175);
		this->m_pBF->Draw(this->m_pPlayer->GetEquippableItems()[2 + this->m_nOffset * 4]->GetName(), nX + 90, nY + 179, 0.3f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Damage: " + to_string(this->m_pPlayer->GetEquippableItems()[2 + this->m_nOffset * 4]->GetSpecial()), nX + 90, nY + 192, 0.3f, D3DCOLOR_ARGB(255, 0, 0, 0));
		
		/////////////////////////////////////////////////////////////////
		// BUG FIX
		// Reference Bug # BB-051, Part 3/4
		// BUG FIX START
		/////////////////////////////////////////////////////////////////

		// Shrank text size from 0.3f to 0.25f and moved Mind and Character offset to the left 15px from 170 to 155
		this->m_pBF->Draw("Body +" + to_string(this->m_pPlayer->GetEquippableItems()[2 + this->m_nOffset * 4]->GetBody()), nX + 90, nY + 205, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Mind +" + to_string(this->m_pPlayer->GetEquippableItems()[2 + this->m_nOffset * 4]->GetMind()), nX + 155, nY + 205, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Soul +" + to_string(this->m_pPlayer->GetEquippableItems()[2 + this->m_nOffset * 4]->GetSoul()), nX + 90, nY + 218, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Char +" + to_string(this->m_pPlayer->GetEquippableItems()[2 + this->m_nOffset * 4]->GetChar()), nX + 155, nY + 218, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		
		///////////////////////////////////////////////////////////////////////////
		// BUG FIX END  Reference # BB-051, Part 3/4
		//////////////////////////////////////////////////////////////////////////

		if(this->m_pPlayer->GetEquippableItems()[2 + this->m_nOffset * 4]->HasGem())
		{
			this->m_pTM->Draw(this->m_pPlayer->GetEquippableItems()[2 + this->m_nOffset * 4]->GetGem()->GetImgID(), nX + 41, nY + 194);
		}
		else
		{
			this->m_pTM->Draw(this->m_pPlayer->GetEquippableItems()[2 + this->m_nOffset * 4]->GetGemImgID(), nX + 41, nY + 194);
		}

		if(this->m_nSelectedIndex == 5)
		{
			alpha = 200;
		}
		else
		{
			alpha = 100;
		}
		this->m_pTM->Draw(this->m_nMenuButtonImgID, nX + 245, nY + 185, 0.25f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
		this->m_pBF->Draw("Equip", nX + 250, nY + 190, 0.4f, D3DCOLOR_ARGB(alpha, 0, 0, 0));

		if(this->m_nSelectedIndex == 6)
		{
			alpha = 200;
		}
		else
		{
			alpha = 100;
		}
		this->m_pTM->Draw(this->m_nMenuButtonImgID, nX + 320, nY + 185, 0.3f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
		this->m_pBF->Draw("Socket", nX + 330, nY + 190, 0.4f, D3DCOLOR_ARGB(alpha, 0, 0, 0));
	}

	// Item 4
	if((int)this->m_pPlayer->GetEquippableItems().size() > 3 + 4 * this->m_nOffset)
	{
		this->m_pTM->Draw(this->m_vInventoryIDs[ICON_BACKGROUND], nX + 20, nY + 255);
		this->m_pTM->Draw(this->m_vInventoryIDs[INFORMATION_BACKGROUND], nX + 80, nY + 255);
		this->m_pTM->Draw(this->m_pPlayer->GetEquippableItems()[3 + this->m_nOffset * 4]->GetImgID(), nX + 26, nY + 260);
		this->m_pBF->Draw(this->m_pPlayer->GetEquippableItems()[3 + this->m_nOffset * 4]->GetName(), nX + 90, nY + 264, 0.3f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Damage: " + to_string(this->m_pPlayer->GetEquippableItems()[3 + this->m_nOffset * 4]->GetSpecial()), nX + 90, nY + 277, 0.3f, D3DCOLOR_ARGB(255, 0, 0, 0));
		
		/////////////////////////////////////////////////////////////////
		// BUG FIX
		// Reference Bug # BB-051, Part 4/4
		// BUG FIX START
		/////////////////////////////////////////////////////////////////

		// Shrank text size from 0.3f to 0.25f and moved Mind and Character offset to the left 15px from 170 to 155
		this->m_pBF->Draw("Body +" + to_string(this->m_pPlayer->GetEquippableItems()[3 + this->m_nOffset * 4]->GetBody()), nX + 90, nY + 290, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Mind +" + to_string(this->m_pPlayer->GetEquippableItems()[3 + this->m_nOffset * 4]->GetMind()), nX + 155, nY + 290, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Soul +" + to_string(this->m_pPlayer->GetEquippableItems()[3 + this->m_nOffset * 4]->GetSoul()), nX + 90, nY + 306, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		this->m_pBF->Draw("Char +" + to_string(this->m_pPlayer->GetEquippableItems()[3 + this->m_nOffset * 4]->GetChar()), nX + 155, nY + 306, 0.25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		
		///////////////////////////////////////////////////////////////////////////
		// BUG FIX END  Reference # BB-051, Part 4/4
		//////////////////////////////////////////////////////////////////////////

		if(this->m_pPlayer->GetEquippableItems()[3 + this->m_nOffset * 4]->HasGem())
		{
			this->m_pTM->Draw(this->m_pPlayer->GetEquippableItems()[3 + this->m_nOffset * 4]->GetGem()->GetImgID(), nX + 41, nY + 279);
		}
		else
		{
			this->m_pTM->Draw(this->m_pPlayer->GetEquippableItems()[3 + this->m_nOffset * 4]->GetGemImgID(), nX + 41, nY + 279);
		}

		if(this->m_nSelectedIndex == 7)
		{
			alpha = 200;
		}
		else
		{
			alpha = 100;
		}
		this->m_pTM->Draw(this->m_nMenuButtonImgID, nX + 245, nY + 270, 0.25f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
		this->m_pBF->Draw("Equip", nX + 250, nY + 275, 0.4f, D3DCOLOR_ARGB(alpha, 0, 0, 0));

		if(this->m_nSelectedIndex == 8)
		{
			alpha = 200;
		}
		else
		{
			alpha = 100;
		}
		this->m_pTM->Draw(this->m_nMenuButtonImgID, nX + 320, nY + 270, 0.3f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
		this->m_pBF->Draw("Socket", nX + 330, nY + 275, 0.4f, D3DCOLOR_ARGB(alpha, 0, 0, 0));
	}

	if (ShowHelpMessage) // Help Message Render
	{
		m_pTM->Draw(m_nHelpPanelID, 50,50,0.8f,0.7f,0,0,0,0,D3DCOLOR_ARGB(195,255,255,255));
		m_pBF->Draw("This is the Inventory screen. You can",95,75,0.6f,D3DCOLOR_ARGB(255,255,255,255));
		m_pBF->Draw("equip weapons and armor here as well ",95,95,0.6f,D3DCOLOR_ARGB(255,255,255,255));
		m_pBF->Draw(" as access each item's individual    ",95,115,0.6f,D3DCOLOR_ARGB(255,255,255,255));
		m_pBF->Draw("Socket screen.",95,135,0.6f,D3DCOLOR_ARGB(255,255,255,255));

		m_pTM->Draw(m_nMenuButtonImgID, 250,400,0.1f,0.5f,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
		m_pBF->Draw("Don't show this message again",280,410,0.4f,D3DCOLOR_ARGB(255,255,255,255));

		if (m_pPlayer->GetHelpMessages() & 1)
			m_pBF->Draw("X",255,405,0.4f,D3DCOLOR_ARGB(100,0,0,0));

		m_pTM->Draw(m_nMenuButtonImgID, 350,500,0.5f,0.5f,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
		m_pBF->Draw("  Continue",360,510,0.4f,D3DCOLOR_ARGB(100,0,0,0));

		if (m_nHelpIndex == 0)
		{
			m_pTM->Draw(m_nMenuButtonImgID, 250,400,0.1f,0.5f,0,0,0,0,D3DCOLOR_ARGB(200,255,255,255));
			if (m_pPlayer->GetHelpMessages() & 1)
				m_pBF->Draw("X",255,405,0.4f,D3DCOLOR_ARGB(255,0,0,0));
		}
		else
		{
			m_pTM->Draw(m_nMenuButtonImgID, 350,500,0.5f,0.5f,0,0,0,0,D3DCOLOR_ARGB(200,255,255,255));
			m_pBF->Draw("  Continue",360,510,0.4f,D3DCOLOR_ARGB(200,0,0,0));
		}

	}
}