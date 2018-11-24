/*
File:		ShopState.cpp
Author:		Benjamin Cowles, Mark Diaz
Course:		SGP 1304
*/

#include "ShopState.h"
#include "BitmapFont.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "ItemManager.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "Game.h"
#include "PlayState.h"

enum {STATE_BACKGROUND, EQUIPPED_BACKGROUND, ICON_BACKGROUND, INFORMATION_BACKGROUND, CURRENCY_BACKGROUND, ALL_ITEMS_BACKGROUND};
enum {SUB_STATE_SELECTION, BUY_SUB_STATE, SELL_SUB_STATE};
/*static*/ CShopState* CShopState::GetInstance( void )
{
	// Easy-instantiation: use a static variable
	static CShopState s_Instance;

	return &s_Instance;
}

CShopState::CShopState(void)
{
}


CShopState::~CShopState(void)
{
}

/*virtual*/ void	CShopState::Enter ( void )
{
	m_pBF = CBitmapFont::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();

	this->m_nSelectedIndex = 1;
	this->m_nOffset = 0;

	m_nSellScreenImgID = m_pTM->LoadTexture(_T("Resource/Graphics/Sell_Screen.png"));
	// STATE_BACKGROUND
	this->m_vShopImgIDs.push_back(this->m_pTM->LoadTexture(_T("Resource/Graphics/Shop_background.png")));
	this->m_vShopImgIDs.push_back(this->m_pTM->LoadTexture(_T("Resource/Graphics/Inventory/inventory_equipped_background.png")));
	this->m_vShopImgIDs.push_back(this->m_pTM->LoadTexture(_T("Resource/Graphics/Inventory/inventory_icon_background.png")));
	this->m_vShopImgIDs.push_back(this->m_pTM->LoadTexture(_T("Resource/Graphics/Inventory/inventory_information_background_large.png")));
	this->m_vShopImgIDs.push_back(this->m_pTM->LoadTexture(_T("Resource/Graphics/Inventory/inventory_potion_background.png")));
	this->m_vShopImgIDs.push_back(this->m_pTM->LoadTexture(_T("Resource/Graphics/Inventory/inventory_all_items_background.png")));

	this->m_nMenuButtonImgID = this->m_pTM->LoadTexture(_T("Resource/Graphics/menubutton2.png"));

	this->m_nSelectedIndex = SUB_STATE_SELECTION;
	
	AskForItems();
}

/*virtual*/ void	CShopState::Exit  ( void )
{
	for(unsigned int i = 0; i < this->m_vShopImgIDs.size(); i++)
	{
		this->m_pTM->UnloadTexture(this->m_vShopImgIDs[i]);
	}

	m_pTM->UnloadTexture( m_nSellScreenImgID );

	if(m_vItems.size() != 0)
	{
		this->m_vItems.clear();
	}
}

/*virtual*/ bool	CShopState::Input ( void )
{
	if(this->m_nSubstate == SUB_STATE_SELECTION)
	{
		SelectionInput();
	}
	else if(this->m_nSubstate == BUY_SUB_STATE)
	{
		BuyInput();
	}
	else
	{
		SellInput();
	}

	return true;
}

/*virtual*/ void	CShopState::Update( float fElapsedTime )
{

}

/*virtual*/ void	CShopState::Render( void )
{
	int alpha = 100;

	// State background
	if (m_nSubstate == SELL_SUB_STATE)
		m_pTM->Draw(m_nSellScreenImgID, 0,0,1.0f,1.0f,0,0,0,0,D3DCOLOR_ARGB(255,255,255,255));
	else
	this->m_pTM->Draw(this->m_vShopImgIDs[STATE_BACKGROUND], 0, 0);

	if(this->m_nSubstate == SUB_STATE_SELECTION)
	{
		if(m_nSelectedIndex == 0)
		{
			alpha = 225;
		}
		else
		{
			alpha = 85;
		}

		CSGD_TextureManager::GetInstance()->Draw(this->m_nMenuButtonImgID,60,200,1.0f,1.0f,0,0,0,0,D3DCOLOR_ARGB(alpha,255,255,255));	

		m_pBF->Draw("Buy", 110, 220, 0.7f, D3DCOLOR_ARGB(alpha, 255, 255, 255));
		
		if(m_nSelectedIndex == 1)
		{
			alpha = 225;
		}
		else
		{
			alpha = 85;
		}

		CSGD_TextureManager::GetInstance()->Draw(this->m_nMenuButtonImgID,60,280,1.0f,1.0f,0,0,0,0,D3DCOLOR_ARGB(alpha,255,255,255));	
		
		m_pBF->Draw("Sell", 110, 300, 0.7f, D3DCOLOR_ARGB(alpha, 255, 255, 255));
		
		if(m_nSelectedIndex == 2)
		{
			alpha = 225;
		}
		else
		{
			alpha = 85;
		}

		CSGD_TextureManager::GetInstance()->Draw(this->m_nMenuButtonImgID,60,360,1.0f,1.0f,0,0,0,0,D3DCOLOR_ARGB(alpha,255,255,255));
		
		m_pBF->Draw("Leave", 110, 380, 0.7f, D3DCOLOR_ARGB(alpha, 255, 255, 255));
	}
	else if(this->m_nSubstate == BUY_SUB_STATE)
	{
		BuyRender();
	}
	else
	{
		SellRender();
	}
}

void CShopState::BuyInput()
{
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

				/////////////////////////////////////////////////////////////////
				// BUG FIX
				// Reference Bug # BB-078
				// BUG FIX START
				/////////////////////////////////////////////////////////////////

				// Play sound effect
				CAudioManager::GetInstance()->PlaySFX( SFX_PAUSE );

				///////////////////////////////////////////////////////////////////////////
				// BUG FIX END  Reference # BB-078
				//////////////////////////////////////////////////////////////////////////
			}
			break;
		case 1:
		case 2:
		case 3:
		case 4:
			{
				if(PlayState::GetInstance()->GetPlayer()->GetCurrency() >= this->m_vItems[this->m_nSelectedIndex - 1 + this->m_nOffset * 4]->GetValue())
				{
					PlayState::GetInstance()->GetPlayer()->AddCurrency(-this->m_vItems[this->m_nSelectedIndex - 1 + this->m_nOffset * 4]->GetValue());
					
					if(this->m_vItems[this->m_nSelectedIndex - 1 + this->m_nOffset * 4]->GetType() != 2)
					{
						PlayState::GetInstance()->GetPlayer()->AddItem(this->m_vItems[this->m_nSelectedIndex - 1 + this->m_nOffset * 4]);
					}
					else
					{
						PlayState::GetInstance()->GetPlayer()->GainPotion();
					}
				}
				else
					CAudioManager::GetInstance()->PlaySFX( SFX_ERROR );
			}
			break;
		case 5:
			{
				this->m_nOffset++;
				if((unsigned int)this->m_nOffset > ((float)(this->m_vItems.size()) / 4.0f))
					this->m_nOffset--;
				this->m_nSelectedIndex = 1;

				/////////////////////////////////////////////////////////////////
				// BUG FIX
				// Reference Bug # BB-078
				// BUG FIX START
				/////////////////////////////////////////////////////////////////

				// Play sound effect
				CAudioManager::GetInstance()->PlaySFX( SFX_PAUSE );

				///////////////////////////////////////////////////////////////////////////
				// BUG FIX END  Reference # BB-078
				//////////////////////////////////////////////////////////////////////////
			}
			break;
		case 6:
			{
				CAudioManager::GetInstance()->PlaySFX( SFX_PAUSE );
				this->m_nSubstate = SUB_STATE_SELECTION;
				this->m_nOffset = 0;
				this->m_nSelectedIndex = 0;
			}
			break;
		}
	}

	if(CInputManager::GetInstance()->CheckRight() || CInputManager::GetInstance()->CheckDown())
	{
		CAudioManager::GetInstance()->PlaySFX( SFX_PAUSEMOVE );
		this->m_nSelectedIndex++;
		if(this->m_nSelectedIndex > 6)
		{
			this->m_nSelectedIndex--;
		}

		if(this->m_nSelectedIndex == 2 && this->m_vItems.size() - this->m_nOffset * 4 <= 1)
		{
			this->m_nSelectedIndex++;
		}

		if(this->m_nSelectedIndex == 3 && this->m_vItems.size() - this->m_nOffset * 4 <= 2)
		{
			this->m_nSelectedIndex++;
		}

		if(this->m_nSelectedIndex == 4 && this->m_vItems.size() - this->m_nOffset * 4 <= 3)
		{
			this->m_nSelectedIndex++;
		}

		if(this->m_nSelectedIndex == 5 && this->m_vItems.size() - this->m_nOffset * 4 <= 4)
		{
			this->m_nSelectedIndex++;
		}
	}

	if(CInputManager::GetInstance()->CheckLeft() || CInputManager::GetInstance()->CheckUp())
	{
		CAudioManager::GetInstance()->PlaySFX( SFX_PAUSEMOVE );
		this->m_nSelectedIndex--;
		if(this->m_nSelectedIndex < 0)
		{
			this->m_nSelectedIndex++;
		}

		if(this->m_nSelectedIndex == 5 && this->m_vItems.size() - this->m_nOffset * 4 <= 4)
		{
			this->m_nSelectedIndex--;
		}

		if(this->m_nSelectedIndex == 4 && this->m_vItems.size() - this->m_nOffset * 4 <= 3)
		{
			this->m_nSelectedIndex--;
		}

		if(this->m_nSelectedIndex == 3 && this->m_vItems.size() - this->m_nOffset * 4 <= 2)
		{
			this->m_nSelectedIndex--;
		}

		if(this->m_nSelectedIndex == 2 && this->m_vItems.size() - this->m_nOffset * 4 <= 1)
		{
			this->m_nSelectedIndex--;
		}

		if(this->m_nOffset == 0 && this->m_nSelectedIndex == 0)
		{
			this->m_nSelectedIndex++;
		}
	}
}

void CShopState::SellInput()
{
	CPlayer * pPlayer = PlayState::GetInstance()->GetPlayer();
	vector<CItem *> pItems;

	for(unsigned int i = 0; i < pPlayer->GetEquippableItems().size(); i++)
	{
		if(!pPlayer->GetEquippableItems()[i]->Equipped())
		{
			pItems.push_back(pPlayer->GetEquippableItems()[i]);
		}
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

				/////////////////////////////////////////////////////////////////
				// BUG FIX
				// Reference Bug # BB-078
				// BUG FIX START
				/////////////////////////////////////////////////////////////////

				// Play sound effect
				CAudioManager::GetInstance()->PlaySFX( SFX_PAUSE );

				///////////////////////////////////////////////////////////////////////////
				// BUG FIX END  Reference # BB-078
				//////////////////////////////////////////////////////////////////////////
			}
			break;
		case 1:
		case 2:
		case 3:
		case 4:
			{
				PlayState::GetInstance()->GetPlayer()->AddCurrency(pItems[this->m_nSelectedIndex - 1 + this->m_nOffset * 4]->GetValue());
				PlayState::GetInstance()->GetPlayer()->RemoveItem(pItems[this->m_nSelectedIndex - 1 + this->m_nOffset * 4]);
				CAudioManager::GetInstance()->PlaySFX( SFX_SELL );
			}
			break;
		case 5:
			{
				this->m_nOffset++;
				if((unsigned int)this->m_nOffset > ((float)(pItems.size()) / 4.0f))
					this->m_nOffset--;
				this->m_nSelectedIndex = 1;

				/////////////////////////////////////////////////////////////////
				// BUG FIX
				// Reference Bug # BB-078
				// BUG FIX START
				/////////////////////////////////////////////////////////////////

				// Play sound effect
				CAudioManager::GetInstance()->PlaySFX( SFX_PAUSE );

				///////////////////////////////////////////////////////////////////////////
				// BUG FIX END  Reference # BB-078
				//////////////////////////////////////////////////////////////////////////
			}
			break;
		case 6:
			{
				CAudioManager::GetInstance()->PlaySFX( SFX_PAUSE );
				this->m_nSubstate = SUB_STATE_SELECTION;
				this->m_nOffset = 0;
				this->m_nSelectedIndex = 1;
			}
			break;
		}
	}

	if(CInputManager::GetInstance()->CheckRight() || CInputManager::GetInstance()->CheckDown())
	{
		CAudioManager::GetInstance()->PlaySFX( SFX_PAUSEMOVE );
		this->m_nSelectedIndex++;
		if(this->m_nSelectedIndex > 6)
		{
			this->m_nSelectedIndex--;
		}

		if(this->m_nSelectedIndex == 2 && pItems.size() - this->m_nOffset * 4 <= 1)
		{
			this->m_nSelectedIndex++;
		}

		if(this->m_nSelectedIndex == 3 && pItems.size() - this->m_nOffset * 4 <= 2)
		{
			this->m_nSelectedIndex++;
		}

		if(this->m_nSelectedIndex == 4 && pItems.size() - this->m_nOffset * 4 <= 3)
		{
			this->m_nSelectedIndex++;
		}

		if(this->m_nSelectedIndex == 5 && pItems.size() - this->m_nOffset * 4 <= 4)
		{
			this->m_nSelectedIndex++;
		}
	}

	if(CInputManager::GetInstance()->CheckLeft() || CInputManager::GetInstance()->CheckUp())
	{
		CAudioManager::GetInstance()->PlaySFX( SFX_PAUSEMOVE );
		this->m_nSelectedIndex--;
		if(this->m_nSelectedIndex < 0)
		{
			this->m_nSelectedIndex++;
		}

		if(pItems.size() == 0)
		{
			this->m_nSelectedIndex++;
		}

		if(this->m_nSelectedIndex == 5 && pItems.size() - this->m_nOffset * 4 <= 4)
		{
			this->m_nSelectedIndex--;
		}

		if(this->m_nSelectedIndex == 4 && pItems.size() - this->m_nOffset * 4 <= 3)
		{
			this->m_nSelectedIndex--;
		}

		if(this->m_nSelectedIndex == 3 && pItems.size() - this->m_nOffset * 4 <= 2)
		{
			this->m_nSelectedIndex--;
		}

		if(this->m_nSelectedIndex == 2 && pItems.size() - this->m_nOffset * 4 <= 1)
		{
			this->m_nSelectedIndex--;
		}

		if(this->m_nOffset == 0 && this->m_nSelectedIndex == 0)
		{
			this->m_nSelectedIndex++;
		}
	}
}

void CShopState::SelectionInput()
{
	if (CInputManager::GetInstance()->CheckEnter())
	{
		CAudioManager::GetInstance()->PlaySFX( SFX_PAUSE );
		switch(this->m_nSelectedIndex)
		{
		case 0:
			{
				this->m_nSubstate = BUY_SUB_STATE;
				this->m_nSelectedIndex = 1;
			}
			break;
		case 1:
			{
				this->m_nSubstate = SELL_SUB_STATE;

				CPlayer * pPlayer = PlayState::GetInstance()->GetPlayer();
				vector<CItem *> pItems;

				for(unsigned int i = 0; i < pPlayer->GetEquippableItems().size(); i++)
				{
					if(!pPlayer->GetEquippableItems()[i]->Equipped())
					{
						pItems.push_back(pPlayer->GetEquippableItems()[i]);
					}
				}

				if(pItems.size() > 0)
				{
					this->m_nSelectedIndex = 1;
				}
				else
				{
					this->m_nSelectedIndex = 6;
				}
			}
			break;
		case 2:
			{
				CGame::GetInstance()->ChangeToPrev();
			}
			break;
		}
	}

	if(CInputManager::GetInstance()->CheckRight() || CInputManager::GetInstance()->CheckDown())
	{
		CAudioManager::GetInstance()->PlaySFX( SFX_PAUSEMOVE );
		this->m_nSelectedIndex++;

		if(this->m_nSelectedIndex > 2)
		{
			this->m_nSelectedIndex--;
		}
	}

	if(CInputManager::GetInstance()->CheckLeft() || CInputManager::GetInstance()->CheckUp())
	{
		CAudioManager::GetInstance()->PlaySFX( SFX_PAUSEMOVE );
		this->m_nSelectedIndex--;

		if(this->m_nSelectedIndex < 0)
		{
			this->m_nSelectedIndex++;
		}
	}
}

void CShopState::BuyRender()
{
	int alpha = 85;

	// Equipped background
	int nX = 50;
	int nY = 155;
	this->m_pTM->Draw(this->m_vShopImgIDs[EQUIPPED_BACKGROUND], nX, nY);
	this->m_pBF->Draw("Equipped:", nX + 10, nY + 10, 0.75f, D3DCOLOR_ARGB(255, 255, 255, 255));

	// Equipped weapon
	// Background
	this->m_pTM->Draw(this->m_vShopImgIDs[ICON_BACKGROUND], nX + 35, nY + 75);
	this->m_pTM->Draw(this->m_vShopImgIDs[INFORMATION_BACKGROUND], nX + 100, nY + 75);
	this->m_pBF->Draw("Weapon", nX + 35, nY + 50, 0.6f, D3DCOLOR_ARGB(255, 255, 255, 255));

	// Item
	vector<CItem *> vWeapons = PlayState::GetInstance()->GetPlayer()->GetWeapons();
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
	this->m_pBF->Draw(iEquippedWeapon->GetName(), nX + 110, nY + 85, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	this->m_pBF->Draw("Damage: " + to_string(iEquippedWeapon->GetSpecial()), nX + 110, nY + 98, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	this->m_pBF->Draw("Body +" + to_string(iEquippedWeapon->GetBody()), nX + 110, nY + 111, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
	this->m_pBF->Draw("Mind +" + to_string(iEquippedWeapon->GetMind()), nX + 175, nY + 111, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
	this->m_pBF->Draw("Soul +" + to_string(iEquippedWeapon->GetSoul()), nX + 110, nY + 124, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
	this->m_pBF->Draw("Char +" + to_string(iEquippedWeapon->GetChar()), nX + 175, nY + 124, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
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
	this->m_pTM->Draw(this->m_vShopImgIDs[ICON_BACKGROUND], nX + 35, nY + 165);
	this->m_pTM->Draw(this->m_vShopImgIDs[INFORMATION_BACKGROUND], nX + 100, nY + 165);
	this->m_pBF->Draw("Armor", nX + 35, nY + 140, 0.6f, D3DCOLOR_ARGB(255, 255, 255, 255));

	// Item
	vector<CItem *> vArmors = PlayState::GetInstance()->GetPlayer()->GetArmors();
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
	this->m_pBF->Draw(iEquippedArmor->GetName(), nX + 110, nY + 175, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	this->m_pBF->Draw("Damage: " + to_string(iEquippedArmor->GetSpecial()), nX + 110, nY + 188, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	this->m_pBF->Draw("Body +" + to_string(iEquippedArmor->GetBody()), nX + 110, nY + 201, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
	this->m_pBF->Draw("Mind +" + to_string(iEquippedArmor->GetMind()), nX + 175, nY + 201, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
	this->m_pBF->Draw("Soul +" + to_string(iEquippedArmor->GetSoul()), nX + 110, nY + 214, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
	this->m_pBF->Draw("Char +" + to_string(iEquippedArmor->GetChar()), nX + 175, nY + 214, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
	if(iEquippedArmor->HasGem())
	{
		this->m_pTM->Draw(iEquippedArmor->GetGem()->GetImgID(), nX + 61, nY + 190);
	}
	else
	{
		this->m_pTM->Draw(iEquippedArmor->GetGemImgID(), nX + 61, nY + 190);
	}
	
	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-057
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	// Added render section for potion count
	this->m_pTM->Draw(this->m_vShopImgIDs[CURRENCY_BACKGROUND], nX, nY + 330, 1.0f, 0.3f);
	this->m_pBF->Draw("Potions: " + to_string(PlayState::GetInstance()->GetPlayer()->GetPotionCount()), nX + 10, nY + 333, 0.7f, D3DCOLOR_ARGB(255, 255, 255, 0));
	
	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-057
	//////////////////////////////////////////////////////////////////////////

	// Currency
	this->m_pTM->Draw(this->m_vShopImgIDs[CURRENCY_BACKGROUND], nX, nY + 370, 1.0f, 0.3f);
	this->m_pBF->Draw("Coins: " + to_string(PlayState::GetInstance()->GetPlayer()->GetCurrency()), nX + 10, nY + 373, 0.7f, D3DCOLOR_ARGB(255, 255, 255, 0));

	if(this->m_nSelectedIndex == 6)
	{
		alpha = 225;
	}
	else
	{
		alpha = 85;
	}

	this->m_pTM->Draw(this->m_nMenuButtonImgID, 620, 535, 0.5f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
	this->m_pBF->Draw("Back", 630, 540, 0.65f, D3DCOLOR_ARGB(alpha, 255, 255, 255));

	// All items
	// Background
	nX = 330;
	this->m_pTM->Draw(this->m_vShopImgIDs[ALL_ITEMS_BACKGROUND], nX, nY);

	nY += 15;

	// Up	
	if(this->m_nSelectedIndex == 0)
	{
		alpha = 225;
	}
	else
	{
		alpha = 85;
	}

	if(this->m_nOffset != 0)
	{
		this->m_pTM->Draw(this->m_nMenuButtonImgID, nX + 270, nY - 5, 0.8f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
		this->m_pBF->Draw("Previous", nX + 295, nY + 5, 0.4f, D3DCOLOR_ARGB(alpha, 255, 255, 255));
	}

	// Down
	if(this->m_nSelectedIndex == 5)
	{
		alpha = 225;
	}
	else
	{
		alpha = 85;
	}

	if(this->m_vItems.size() - this->m_nOffset * 4 > 4)
	{
		this->m_pTM->Draw(this->m_nMenuButtonImgID, nX + 270, nY + 310, 0.6f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
		this->m_pBF->Draw("More", nX + 295, nY + 320, 0.4f, D3DCOLOR_ARGB(alpha, 255, 255, 255));
	}

	// Item 1
	if((int)this->m_vItems.size() > 0 + 4 * this->m_nOffset)
	{
		this->m_pTM->Draw(this->m_vShopImgIDs[ICON_BACKGROUND], nX + 20, nY + 15);
		this->m_pTM->Draw(this->m_vShopImgIDs[INFORMATION_BACKGROUND], nX + 80, nY + 15);
		this->m_pTM->Draw(this->m_vItems[this->m_nOffset * 4]->GetImgID(), nX + 26, nY + 20);
		if(m_vItems[this->m_nOffset * 4]->GetType() != 3) {this->m_pBF->Draw(this->m_vItems[this->m_nOffset * 4]->GetName(), nX + 90, nY + 24, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));}
		else{this->m_pBF->Draw(this->m_vItems[this->m_nOffset * 4]->GetName() + " gem", nX + 90, nY + 24, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));}
		if(m_vItems[this->m_nOffset * 4]->GetType() == 2)this->m_pBF->Draw("Heals: " + to_string(this->m_vItems[this->m_nOffset * 4]->GetSpecial()) + "%", nX + 90, nY + 37, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		else if(m_vItems[this->m_nOffset * 4]->GetType() != 3)this->m_pBF->Draw("Damage: " + to_string(this->m_vItems[this->m_nOffset * 4]->GetSpecial()), nX + 90, nY + 37, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		if(m_vItems[this->m_nOffset * 4]->GetType() != 2)this->m_pBF->Draw("Body +" + to_string(this->m_vItems[this->m_nOffset * 4]->GetBody()), nX + 90, nY + 50, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
		if(m_vItems[this->m_nOffset * 4]->GetType() != 2)this->m_pBF->Draw("Mind +" + to_string(this->m_vItems[this->m_nOffset * 4]->GetMind()), nX + 155, nY + 50, 0.25f, D3DCOLOR_ARGB(255,255, 255, 255));
		if(m_vItems[this->m_nOffset * 4]->GetType() != 2)this->m_pBF->Draw("Soul +" + to_string(this->m_vItems[this->m_nOffset * 4]->GetSoul()), nX + 90, nY + 63, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
		if(m_vItems[this->m_nOffset * 4]->GetType() != 2)this->m_pBF->Draw("Char +" + to_string(this->m_vItems[this->m_nOffset * 4]->GetChar()), nX + 155, nY + 63, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));

		if(this->m_vItems[this->m_nOffset * 4]->HasGem())
		{
			this->m_pTM->Draw(this->m_vItems[this->m_nOffset * 4]->GetGem()->GetImgID(), nX + 41, nY + 39);
		}
		else
		{
			this->m_pTM->Draw(this->m_vItems[this->m_nOffset * 4]->GetGemImgID(), nX + 41, nY + 39);
		}

		this->m_pBF->Draw(to_string(this->m_vItems[this->m_nOffset* 4]->GetValue()), nX + 240, nY + 35, 0.55f, D3DCOLOR_ARGB(200, 255, 255, 0));

		if(this->m_nSelectedIndex == 1)
		{
			alpha = 225;
		}
		else
		{
			alpha = 85;
		}
		this->m_pTM->Draw(this->m_nMenuButtonImgID, nX + 330, nY + 30, 0.4f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
		this->m_pBF->Draw("Buy", nX + 345, nY + 40, 0.4f, D3DCOLOR_ARGB(alpha, 255, 255, 255));
	}

	// Item 2
	if((int)this->m_vItems.size() > 1 + 4 * this->m_nOffset)
	{
		this->m_pTM->Draw(this->m_vShopImgIDs[ICON_BACKGROUND], nX + 20, nY + 90);
		this->m_pTM->Draw(this->m_vShopImgIDs[INFORMATION_BACKGROUND], nX + 80, nY + 90);
		this->m_pTM->Draw(this->m_vItems[1 + this->m_nOffset * 4]->GetImgID(), nX + 26, nY + 95);
		if(m_vItems[1 + this->m_nOffset * 4]->GetType() != 3){
			this->m_pBF->Draw(this->m_vItems[1 + this->m_nOffset * 4]->GetName(), nX + 90, nY + 99, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));}
		else{this->m_pBF->Draw(this->m_vItems[1 + this->m_nOffset * 4]->GetName() + " gem", nX + 90, nY + 99, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));}
		if(m_vItems[1 + this->m_nOffset * 4]->GetType() != 3)this->m_pBF->Draw("Damage: " + to_string(this->m_vItems[1 + this->m_nOffset * 4]->GetSpecial()), nX + 90, nY + 112, 0.3f, D3DCOLOR_ARGB(255,255, 255, 255));
		this->m_pBF->Draw("Body +" + to_string(this->m_vItems[1 + this->m_nOffset * 4]->GetBody()), nX + 90, nY + 125, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
		this->m_pBF->Draw("Mind +" + to_string(this->m_vItems[1 + this->m_nOffset * 4]->GetMind()), nX + 155, nY + 125, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
		this->m_pBF->Draw("Soul +" + to_string(this->m_vItems[1 + this->m_nOffset * 4]->GetSoul()), nX + 90, nY + 138, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
		this->m_pBF->Draw("Char +" + to_string(this->m_vItems[1 + this->m_nOffset * 4]->GetChar()), nX + 155, nY + 138, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));

		if(this->m_vItems[1 + this->m_nOffset * 4]->HasGem())
		{
			this->m_pTM->Draw(this->m_vItems[1 + this->m_nOffset * 4]->GetGem()->GetImgID(), nX + 41, nY + 109);
		}
		else
		{
			this->m_pTM->Draw(this->m_vItems[1 + this->m_nOffset * 4]->GetGemImgID(), nX + 41, nY + 109);
		}

		this->m_pBF->Draw(to_string(this->m_vItems[1 + this->m_nOffset * 4]->GetValue()), nX + 240, nY + 110, 0.55f, D3DCOLOR_ARGB(200, 255, 255, 0));

		if(this->m_nSelectedIndex == 2)
		{
			alpha = 225;
		}
		else
		{
			alpha = 85;
		}
		this->m_pTM->Draw(this->m_nMenuButtonImgID, nX + 330, nY + 105, 0.4f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
		this->m_pBF->Draw("Buy", nX + 345, nY + 115, 0.4f, D3DCOLOR_ARGB(alpha,255, 255, 255));
	}

	// Item 3
	if((int)this->m_vItems.size() > 2 + 4 * this->m_nOffset)
	{
		this->m_pTM->Draw(this->m_vShopImgIDs[ICON_BACKGROUND], nX + 20, nY + 170);
		this->m_pTM->Draw(this->m_vShopImgIDs[INFORMATION_BACKGROUND], nX + 80, nY + 170);
		this->m_pTM->Draw(this->m_vItems[2 + this->m_nOffset * 4]->GetImgID(), nX + 26, nY + 175);
		if(m_vItems[2 + this->m_nOffset * 4]->GetType() != 3){this->m_pBF->Draw(this->m_vItems[2 + this->m_nOffset * 4]->GetName(), nX + 90, nY + 179, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));}
		else {this->m_pBF->Draw(this->m_vItems[2 + this->m_nOffset * 4]->GetName() + " gem", nX + 90, nY + 179, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));}
		if(m_vItems[2 + this->m_nOffset * 4]->GetType() != 3)this->m_pBF->Draw("Damage: " + to_string(this->m_vItems[2 + this->m_nOffset * 4]->GetSpecial()), nX + 90, nY + 192, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		this->m_pBF->Draw("Body +" + to_string(this->m_vItems[2 + this->m_nOffset * 4]->GetBody()), nX + 90, nY + 205, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
		this->m_pBF->Draw("Mind +" + to_string(this->m_vItems[2 + this->m_nOffset * 4]->GetMind()), nX + 155, nY + 205, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
		this->m_pBF->Draw("Soul +" + to_string(this->m_vItems[2 + this->m_nOffset * 4]->GetSoul()), nX + 90, nY + 218, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
		this->m_pBF->Draw("Char +" + to_string(this->m_vItems[2 + this->m_nOffset * 4]->GetChar()), nX + 155, nY + 218, 0.25f, D3DCOLOR_ARGB(255,255, 255, 255));

		if(this->m_vItems[2 + this->m_nOffset * 4]->HasGem())
		{
			this->m_pTM->Draw(this->m_vItems[2 + this->m_nOffset * 4]->GetGem()->GetImgID(), nX + 41, nY + 194);
		}
		else
		{
			this->m_pTM->Draw(this->m_vItems[2 + this->m_nOffset * 4]->GetGemImgID(), nX + 41, nY + 194);
		}

		this->m_pBF->Draw(to_string(this->m_vItems[2 + this->m_nOffset * 4]->GetValue()), nX + 240, nY + 190, 0.55f, D3DCOLOR_ARGB(200, 255, 255, 0));

		if(this->m_nSelectedIndex == 3)
		{
			alpha = 225;
		}
		else
		{
			alpha = 85;
		}
		this->m_pTM->Draw(this->m_nMenuButtonImgID, nX + 330, nY + 185, 0.4f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
		this->m_pBF->Draw("Buy", nX + 345, nY + 195, 0.4f, D3DCOLOR_ARGB(alpha, 255, 255, 255));
	}

	// Item 4
	if((int)this->m_vItems.size() > 3 + 4 * this->m_nOffset)
	{
		this->m_pTM->Draw(this->m_vShopImgIDs[ICON_BACKGROUND], nX + 20, nY + 255);
		this->m_pTM->Draw(this->m_vShopImgIDs[INFORMATION_BACKGROUND], nX + 80, nY + 255);
		this->m_pTM->Draw(this->m_vItems[3 + this->m_nOffset * 4]->GetImgID(), nX + 26, nY + 260);
		if(m_vItems[3 + this->m_nOffset * 4]->GetType() != 3){this->m_pBF->Draw(this->m_vItems[3 + this->m_nOffset * 4]->GetName(), nX + 90, nY + 264, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));}
		else {this->m_pBF->Draw(this->m_vItems[3 + this->m_nOffset * 4]->GetName() + " gem", nX + 90, nY + 264, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));}
		if(m_vItems[3 + this->m_nOffset * 4]->GetType() != 3)this->m_pBF->Draw("Damage: " + to_string(this->m_vItems[3 + this->m_nOffset * 4]->GetSpecial()), nX + 90, nY + 277, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		this->m_pBF->Draw("Body +" + to_string(this->m_vItems[3 + this->m_nOffset * 4]->GetBody()), nX + 90, nY + 290, 0.25f, D3DCOLOR_ARGB(255,255, 255, 255));
		this->m_pBF->Draw("Mind +" + to_string(this->m_vItems[3 + this->m_nOffset * 4]->GetMind()), nX + 155, nY + 290, 0.25f, D3DCOLOR_ARGB(255,255, 255, 255));
		this->m_pBF->Draw("Soul +" + to_string(this->m_vItems[3 + this->m_nOffset * 4]->GetSoul()), nX + 90, nY + 306, 0.25f, D3DCOLOR_ARGB(255,255, 255, 255));
		this->m_pBF->Draw("Char +" + to_string(this->m_vItems[3 + this->m_nOffset * 4]->GetChar()), nX + 155, nY + 306, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
		if(this->m_vItems[3 + this->m_nOffset * 4]->HasGem())
		{
			this->m_pTM->Draw(this->m_vItems[3 + this->m_nOffset * 4]->GetGem()->GetImgID(), nX + 41, nY + 279);
		}
		else
		{
			this->m_pTM->Draw(this->m_vItems[3 + this->m_nOffset * 4]->GetGemImgID(), nX + 41, nY + 279);
		}

		this->m_pBF->Draw(to_string(this->m_vItems[3 + this->m_nOffset * 4]->GetValue()), nX + 240, nY + 275, 0.55f, D3DCOLOR_ARGB(200, 255, 255, 0));

		if(this->m_nSelectedIndex == 4)
		{
			alpha = 225;
		}
		else
		{
			alpha = 85;
		}

		this->m_pTM->Draw(this->m_nMenuButtonImgID, nX + 330, nY + 270, 0.4f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
		this->m_pBF->Draw("Buy", nX + 345, nY + 280, 0.4f, D3DCOLOR_ARGB(alpha, 255, 255, 255));
	}
}

void CShopState::SellRender()
{
	int alpha = 100;

	// Equipped background
	int nX = 50;
	int nY = 155;
	this->m_pTM->Draw(this->m_vShopImgIDs[EQUIPPED_BACKGROUND], nX, nY);
	this->m_pBF->Draw("Equipped:", nX + 10, nY + 10, 0.75f, D3DCOLOR_ARGB(255, 255, 255, 255));

	// Equipped weapon
	// Background
	this->m_pTM->Draw(this->m_vShopImgIDs[ICON_BACKGROUND], nX + 35, nY + 75);
	this->m_pTM->Draw(this->m_vShopImgIDs[INFORMATION_BACKGROUND], nX + 100, nY + 75);
	this->m_pBF->Draw("Weapon", nX + 35, nY + 50, 0.6f, D3DCOLOR_ARGB(255, 255, 255, 255));

	// Item
	vector<CItem *> vWeapons = PlayState::GetInstance()->GetPlayer()->GetWeapons();
	CItem * iEquippedWeapon;
	CPlayer * pPlayer = PlayState::GetInstance()->GetPlayer();
	
	vector<CItem *> pItems;

	for(unsigned int i = 0; i < pPlayer->GetEquippableItems().size(); i++)
	{
		if(!pPlayer->GetEquippableItems()[i]->Equipped())
		{
			pItems.push_back(pPlayer->GetEquippableItems()[i]);
		}
	}

	for(unsigned int i = 0; i < vWeapons.size(); i++)
	{
		if(vWeapons[i]->Equipped())
		{
			iEquippedWeapon = vWeapons[i];
			i = vWeapons.size();
		}
	}

	this->m_pTM->Draw(iEquippedWeapon->GetImgID(), nX + 46, nY + 81);
	this->m_pBF->Draw(iEquippedWeapon->GetName(), nX + 110, nY + 85, 0.3f, D3DCOLOR_ARGB(255,255, 255, 255));
	this->m_pBF->Draw("Damage: " + to_string(iEquippedWeapon->GetSpecial()), nX + 110, nY + 98, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	this->m_pBF->Draw("Body +" + to_string(iEquippedWeapon->GetBody()), nX + 110, nY + 111, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
	this->m_pBF->Draw("Mind +" + to_string(iEquippedWeapon->GetMind()), nX + 175, nY + 111, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
	this->m_pBF->Draw("Soul +" + to_string(iEquippedWeapon->GetSoul()), nX + 110, nY + 124, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
	this->m_pBF->Draw("Char +" + to_string(iEquippedWeapon->GetChar()), nX + 175, nY + 124, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
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
	this->m_pTM->Draw(this->m_vShopImgIDs[ICON_BACKGROUND], nX + 35, nY + 165);
	this->m_pTM->Draw(this->m_vShopImgIDs[INFORMATION_BACKGROUND], nX + 100, nY + 165);
	this->m_pBF->Draw("Armor", nX + 35, nY + 140, 0.6f, D3DCOLOR_ARGB(255, 255, 255, 255));
	// Item
	vector<CItem *> vArmors = PlayState::GetInstance()->GetPlayer()->GetArmors();
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
	this->m_pBF->Draw(iEquippedArmor->GetName(), nX + 110, nY + 175, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	this->m_pBF->Draw("Damage: " + to_string(iEquippedArmor->GetSpecial()), nX + 110, nY + 188, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	this->m_pBF->Draw("Body +" + to_string(iEquippedArmor->GetBody()), nX + 110, nY + 201, 0.25f, D3DCOLOR_ARGB(255,255, 255, 255));
	this->m_pBF->Draw("Mind +" + to_string(iEquippedArmor->GetMind()), nX + 175, nY + 201, 0.25f, D3DCOLOR_ARGB(255,255, 255, 255));
	this->m_pBF->Draw("Soul +" + to_string(iEquippedArmor->GetSoul()), nX + 110, nY + 214, 0.25f, D3DCOLOR_ARGB(255,255, 255, 255));
	this->m_pBF->Draw("Char +" + to_string(iEquippedArmor->GetChar()), nX + 175, nY + 214, 0.25f, D3DCOLOR_ARGB(255,255, 255, 255));
	if(iEquippedArmor->HasGem())
	{
		this->m_pTM->Draw(iEquippedArmor->GetGem()->GetImgID(), nX + 61, nY + 190);
	}
	else
	{
		this->m_pTM->Draw(iEquippedArmor->GetGemImgID(), nX + 61, nY + 190);
	}

	// Currency
	this->m_pTM->Draw(this->m_vShopImgIDs[CURRENCY_BACKGROUND], nX, nY + 370, 1.0f, 0.3f);
	this->m_pBF->Draw("Coins: " + to_string(PlayState::GetInstance()->GetPlayer()->GetCurrency()), nX + 10, nY + 373, 0.7f, D3DCOLOR_ARGB(255, 255, 255, 255));

	if(this->m_nSelectedIndex == 6)
	{
		alpha = 225;
	}
	else
	{
		alpha = 85;
	}
	
	this->m_pTM->Draw(this->m_nMenuButtonImgID, 620, 535, 0.5f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
	this->m_pBF->Draw("Back", 630, 540, 0.65f, D3DCOLOR_ARGB(alpha, 255, 255, 255));

	// All items
	// Background
	nX = 330;
	this->m_pTM->Draw(this->m_vShopImgIDs[ALL_ITEMS_BACKGROUND], nX, nY);

	nY += 15;

	// Up	
	if(this->m_nSelectedIndex == 0)
	{
		alpha = 225;
	}
	else
	{
		alpha = 85;
	}

	if(this->m_nOffset != 0)
	{
		this->m_pTM->Draw(this->m_nMenuButtonImgID, nX + 270, nY - 5, 0.8f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
		this->m_pBF->Draw("Previous", nX + 295, nY + 5, 0.4f, D3DCOLOR_ARGB(alpha, 255, 255, 255));
	}

	// Down
	if(this->m_nSelectedIndex == 5)
	{
		alpha = 225;
	}
	else
	{
		alpha = 85;
	}

	if(pItems.size() - this->m_nOffset * 4 > 4)
	{
		this->m_pTM->Draw(this->m_nMenuButtonImgID, nX + 270, nY + 310, 0.5f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
		this->m_pBF->Draw("Next", nX + 285, nY + 320, 0.4f, D3DCOLOR_ARGB(alpha, 255, 255, 255));
	}

	// Item 1
	if((int)pItems.size() > 0 + 4 * this->m_nOffset)
	{
		this->m_pTM->Draw(this->m_vShopImgIDs[ICON_BACKGROUND], nX + 20, nY + 15);
		this->m_pTM->Draw(this->m_vShopImgIDs[INFORMATION_BACKGROUND], nX + 80, nY + 15);
		this->m_pTM->Draw(pItems[this->m_nOffset * 4]->GetImgID(), nX + 26, nY + 20);
		this->m_pBF->Draw(pItems[this->m_nOffset * 4]->GetName(), nX + 90, nY + 24, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		this->m_pBF->Draw("Damage: " + to_string(pItems[this->m_nOffset * 4]->GetSpecial()), nX + 90, nY + 37, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		this->m_pBF->Draw("Body +" + to_string(pItems[this->m_nOffset * 4]->GetBody()), nX + 90, nY + 50, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
		this->m_pBF->Draw("Mind +" + to_string(pItems[this->m_nOffset * 4]->GetMind()), nX + 155, nY + 50, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
		this->m_pBF->Draw("Soul +" + to_string(pItems[this->m_nOffset * 4]->GetSoul()), nX + 90, nY + 63, 0.25f, D3DCOLOR_ARGB(255,255, 255, 255));
		this->m_pBF->Draw("Char +" + to_string(pItems[this->m_nOffset * 4]->GetChar()), nX + 155, nY + 63, 0.25f, D3DCOLOR_ARGB(255,255, 255, 255));

		if(pItems[this->m_nOffset * 4]->HasGem())
		{
			this->m_pTM->Draw(pItems[this->m_nOffset * 4]->GetGem()->GetImgID(), nX + 41, nY + 39);
		}
		else
		{
			this->m_pTM->Draw(pItems[this->m_nOffset * 4]->GetGemImgID(), nX + 41, nY + 39);
		}

		this->m_pBF->Draw(to_string(pItems[this->m_nOffset* 4]->GetValue()), nX + 240, nY + 35, 0.55f, D3DCOLOR_ARGB(200, 255, 255, 0));

		if(this->m_nSelectedIndex == 1)
		{
			alpha = 225;
		}
		else
		{
			alpha = 85;
		}
		this->m_pTM->Draw(this->m_nMenuButtonImgID, nX + 330, nY + 30, 0.4f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
		this->m_pBF->Draw("Sell", nX + 345, nY + 40, 0.4f, D3DCOLOR_ARGB(alpha, 255, 255, 255));
	}

	// Item 2
	if((int)pItems.size() > 1 + 4 * this->m_nOffset)
	{
		this->m_pTM->Draw(this->m_vShopImgIDs[ICON_BACKGROUND], nX + 20, nY + 90);
		this->m_pTM->Draw(this->m_vShopImgIDs[INFORMATION_BACKGROUND], nX + 80, nY + 90);
		this->m_pTM->Draw(pItems[1 + this->m_nOffset * 4]->GetImgID(), nX + 26, nY + 95);
		this->m_pBF->Draw(pItems[1 + this->m_nOffset * 4]->GetName(), nX + 90, nY + 99, 0.3f, D3DCOLOR_ARGB(255,255, 255, 255));
		this->m_pBF->Draw("Damage: " + to_string(pItems[1 + this->m_nOffset * 4]->GetSpecial()), nX + 90, nY + 112, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		this->m_pBF->Draw("Body +" + to_string(pItems[1 + this->m_nOffset * 4]->GetBody()), nX + 90, nY + 125, 0.25f, D3DCOLOR_ARGB(255,255, 255, 255));
		this->m_pBF->Draw("Mind +" + to_string(pItems[1 + this->m_nOffset * 4]->GetMind()), nX + 155, nY + 125, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
		this->m_pBF->Draw("Soul +" + to_string(pItems[1 + this->m_nOffset * 4]->GetSoul()), nX + 90, nY + 138, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
		this->m_pBF->Draw("Char +" + to_string(pItems[1 + this->m_nOffset * 4]->GetChar()), nX + 155, nY + 138, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));

		if(pItems[1 + this->m_nOffset * 4]->HasGem())
		{
			this->m_pTM->Draw(pItems[1 + this->m_nOffset * 4]->GetGem()->GetImgID(), nX + 41, nY + 109);
		}
		else
		{
			this->m_pTM->Draw(pItems[1 + this->m_nOffset * 4]->GetGemImgID(), nX + 41, nY + 109);
		}

		this->m_pBF->Draw(to_string(pItems[1 + this->m_nOffset * 4]->GetValue()), nX + 240, nY + 110, 0.55f, D3DCOLOR_ARGB(200, 255, 255, 0));

		if(this->m_nSelectedIndex == 2)
		{
			alpha = 225;
		}
		else
		{
			alpha = 85;
		}
		this->m_pTM->Draw(this->m_nMenuButtonImgID, nX + 330, nY + 105, 0.4f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
		this->m_pBF->Draw("Sell", nX + 345, nY + 115, 0.4f, D3DCOLOR_ARGB(alpha,255, 255, 255));
	}

	// Item 3
	if((int)pItems.size() > 2 + 4 * this->m_nOffset)
	{
		this->m_pTM->Draw(this->m_vShopImgIDs[ICON_BACKGROUND], nX + 20, nY + 170);
		this->m_pTM->Draw(this->m_vShopImgIDs[INFORMATION_BACKGROUND], nX + 80, nY + 170);
		this->m_pTM->Draw(pItems[2 + this->m_nOffset * 4]->GetImgID(), nX + 26, nY + 175);
		this->m_pBF->Draw( pItems[2 + this->m_nOffset * 4]->GetName(), nX + 90, nY + 179, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		this->m_pBF->Draw("Damage: " + to_string(pItems[2 + this->m_nOffset * 4]->GetSpecial()), nX + 90, nY + 192, 0.3f, D3DCOLOR_ARGB(255,255, 255, 255));
		this->m_pBF->Draw("Body +" + to_string(pItems[2 + this->m_nOffset * 4]->GetBody()), nX + 90, nY + 205, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
		this->m_pBF->Draw("Mind +" + to_string(pItems[2 + this->m_nOffset * 4]->GetMind()), nX + 155, nY + 205, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
		this->m_pBF->Draw("Soul +" + to_string(pItems[2 + this->m_nOffset * 4]->GetSoul()), nX + 90, nY + 218, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
		this->m_pBF->Draw("Char +" + to_string(pItems[2 + this->m_nOffset * 4]->GetChar()), nX + 155, nY + 218, 0.25f, D3DCOLOR_ARGB(255,255, 255, 255));

		if(pItems[2 + this->m_nOffset * 4]->HasGem())
		{
			this->m_pTM->Draw(pItems[2 + this->m_nOffset * 4]->GetGem()->GetImgID(), nX + 41, nY + 194);
		}
		else
		{
			this->m_pTM->Draw(pItems[2 + this->m_nOffset * 4]->GetGemImgID(), nX + 41, nY + 194);
		}

		this->m_pBF->Draw(to_string(pItems[2 + this->m_nOffset * 4]->GetValue()), nX + 240, nY + 190, 0.55f, D3DCOLOR_ARGB(200, 255, 255, 0));

		if(this->m_nSelectedIndex == 3)
		{
			alpha = 225;
		}
		else
		{
			alpha = 85;
		}
		this->m_pTM->Draw(this->m_nMenuButtonImgID, nX + 330, nY + 185, 0.4f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
		this->m_pBF->Draw("Sell", nX + 345, nY + 195, 0.4f, D3DCOLOR_ARGB(alpha, 255, 255, 255));
	}

	// Item 4
	if((int)pItems.size() > 3 + 4 * this->m_nOffset)
	{
		this->m_pTM->Draw(this->m_vShopImgIDs[ICON_BACKGROUND], nX + 20, nY + 255);
		this->m_pTM->Draw(this->m_vShopImgIDs[INFORMATION_BACKGROUND], nX + 80, nY + 255);
		this->m_pTM->Draw(pItems[3 + this->m_nOffset * 4]->GetImgID(), nX + 26, nY + 260);
		this->m_pBF->Draw(pItems[3 + this->m_nOffset * 4]->GetName(), nX + 90, nY + 264, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		this->m_pBF->Draw("Damage: " + to_string(pItems[3 + this->m_nOffset * 4]->GetSpecial()), nX + 90, nY + 277, 0.3f, D3DCOLOR_ARGB(255,255, 255, 255));
		this->m_pBF->Draw("Body +" + to_string(pItems[3 + this->m_nOffset * 4]->GetBody()), nX + 90, nY + 290, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
		this->m_pBF->Draw("Mind +" + to_string(pItems[3 + this->m_nOffset * 4]->GetMind()), nX + 155, nY + 290, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
		this->m_pBF->Draw("Soul +" + to_string(pItems[3 + this->m_nOffset * 4]->GetSoul()), nX + 90, nY + 306, 0.25f, D3DCOLOR_ARGB(255,255, 255, 255));
		this->m_pBF->Draw("Char +" + to_string(pItems[3 + this->m_nOffset * 4]->GetChar()), nX + 155, nY + 306, 0.25f, D3DCOLOR_ARGB(255, 255, 255, 255));
		if(pItems[3 + this->m_nOffset * 4]->HasGem())
		{
			this->m_pTM->Draw(pItems[3 + this->m_nOffset * 4]->GetGem()->GetImgID(), nX + 41, nY + 279);
		}					
		else				
		{					
			this->m_pTM->Draw(pItems[3 + this->m_nOffset * 4]->GetGemImgID(), nX + 41, nY + 279);
		}

		this->m_pBF->Draw(to_string(pItems[3 + this->m_nOffset * 4]->GetValue()), nX + 240, nY + 275, 0.55f, D3DCOLOR_ARGB(200, 255, 255, 0));

		if(this->m_nSelectedIndex == 4)
		{
			alpha = 225;
		}
		else
		{
			alpha = 85;
		}

		this->m_pTM->Draw(this->m_nMenuButtonImgID, nX + 330, nY + 270, 0.4f, 0.5f, 0, 0, 0, 0, D3DCOLOR_ARGB(alpha, 185, 185, 185));
		this->m_pBF->Draw("Sell", nX + 340, nY + 280, 0.4f, D3DCOLOR_ARGB(alpha,255, 255, 255));
	}
}

void CShopState::AskForItems()
{
	int i = 0;
	
	if(PlayState::GetInstance()->GetPlayer()->GetArtifactNumber() >= 4)
	{
		for(; i <= LevelManager::GetInstance()->ShopStock()[4]; i++)
		{
			m_vItems.push_back(CItemManager::GetInstance()->GetItem(i));
		}
	}

	if(PlayState::GetInstance()->GetPlayer()->GetArtifactNumber() >= 3)
	{
		for(; i <= LevelManager::GetInstance()->ShopStock()[3]; i++)
		{
			m_vItems.push_back(CItemManager::GetInstance()->GetItem(i));
		}
	}

	if(PlayState::GetInstance()->GetPlayer()->GetArtifactNumber() >= 2)
	{
		for(; i <= LevelManager::GetInstance()->ShopStock()[2]; i++)
		{
			m_vItems.push_back(CItemManager::GetInstance()->GetItem(i));
		}
	}
	
	if(PlayState::GetInstance()->GetPlayer()->GetArtifactNumber() >= 1)
	{
		for(; i <= LevelManager::GetInstance()->ShopStock()[1]; i++)
		{
			m_vItems.push_back(CItemManager::GetInstance()->GetItem(i));
		}
	}
	
	if(PlayState::GetInstance()->GetPlayer()->GetArtifactNumber() >= 0)
	{
		for(; i <= LevelManager::GetInstance()->ShopStock()[0]; i++)
		{
			m_vItems.push_back(CItemManager::GetInstance()->GetItem(i));
		}
	}
}