/*
File:		CLoadGameState.cpp
Author:		Mark Diaz and Benjamin "Evan" Cowles
Course:		SGD 1302
Purpose:	The LoadGameState Presents the user with the three
save slots and loads whatever one they select
*/

#include "LoadGameState.h"
#include "MainMenuState.h"
#include "Game.h"
#include "InputManager.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "PlayState.h"
#include "AudioManager.h"
#include "PlayState.h"
#include <ostream>
#include "..\\TinyXML\tinyxml.h"
#include <ShlObj.h>
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "ItemManager.h"
#include "FireArtifact.h"
#include "SnowArtifact.h"
#include "EarthArtifact.h"
#include "ElectricArtifact.h"

// This class is a singleton.
/*static*/ CLoadGameState* CLoadGameState::GetInstance( void )
{
	static CLoadGameState s_Instance;
	return &s_Instance;
}

CLoadGameState::CLoadGameState(void)
{
	m_nBackgroundImgID	= -1;
	m_nMenuButtonImgID	= -1;
	m_nSelectedIndex	= 1;
	m_bFromPause = false;


	SelectionLarge.LoadFromFile( "Resource/Graphics/Particles/SparklesLarge.xml" );
	SelectionFat.LoadFromFile( "Resource/Graphics/Particles/SparklesFat.xml" );
	Burst.LoadFromFile( "Resource/Graphics/Particles/Burst.xml" );
}


CLoadGameState::~CLoadGameState(void)
{
}

/*virtual*/ void	CLoadGameState::Enter ( void )
{
	this->m_vActive.clear();
	this->m_vUnspent.clear();
	this->m_vCharacter.clear();
	this->m_vExperience.clear();
	this->m_vCurrency.clear();
	this->m_vLevel.clear();
	this->m_vBody.clear();
	this->m_vMind.clear();
	this->m_vSoul.clear();
	this->m_vHealth.clear();
	this->m_vArtifact1.clear();
	this->m_vArtifact2.clear();
	this->m_vArtifact3.clear();
	this->m_vArtifact4.clear();
	this->m_vShards.clear();
	this->m_vPotions.clear();
	this->m_vPortal.clear();
	this->m_vItems.clear();
	this->m_vMapName.clear();
	this->m_vKeys.clear();
	this->m_vHelp.clear();

	// Store my wrappers
	m_pIM	= CInputManager::GetInstance();
	m_pTM	= CSGD_TextureManager::GetInstance();
	m_pBF	= CBitmapFont::GetInstance();

	// Load my textures
	m_nBackgroundImgID = m_pTM->LoadTexture(_T("Resource/Graphics/BG.jpg"), D3DCOLOR_ARGB(255,255,255,255));
	m_nMenuButtonImgID = m_pTM->LoadTexture(_T("Resource/Graphics/menubutton2.png"));
	m_nMenuInfoImgID = m_pTM->LoadTexture(_T("Resource/Graphics/load_info.png"));

	m_bFadingIn = true;
	m_fFadeTimer = 0.0f;
	m_nAlphaOffset = 255;
	m_nSelectedIndex = 1;

	SelectionLarge.SetXPosition(250);
	SelectionLarge.SetYPosition(270);
	SelectionLarge.Initialize();

	SelectionFat.SetXPosition(390);
	SelectionFat.SetYPosition(-100);
	SelectionFat.Initialize();

	int nUnspent;
	int nCharacter;
	int nExperience;
	int nCurrency;
	int nLevel;
	int nBody;
	int nMind;
	int nSoul;
	int nHealth;
	int nArtifact1;
	int nArtifact2;
	int nArtifact3;
	int nArtifact4;
	int nShards;
	int nPotions;
	int nPortal;
	int nKeys;
	int nHelp;
	string szMapName;

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
			m_vActive.push_back(1);

			TiXmlElement* pRoot = doc.RootElement();	

			pRoot->Attribute("Unspent", &nUnspent);
			pRoot->Attribute("Character", &nCharacter);
			pRoot->Attribute("Experience", &nExperience);
			pRoot->Attribute("Currency", &nCurrency);
			pRoot->Attribute("Level", &nLevel);
			pRoot->Attribute("Body", &nBody);
			pRoot->Attribute("Mind", &nMind);
			pRoot->Attribute("Soul", &nSoul);
			pRoot->Attribute("Health", &nHealth);
			pRoot->Attribute("Artifact1", &nArtifact1);
			pRoot->Attribute("Artifact2", &nArtifact2);
			pRoot->Attribute("Artifact3", &nArtifact3);
			pRoot->Attribute("Artifact4", &nArtifact4);
			pRoot->Attribute("Shards", &nShards);
			pRoot->Attribute("Potions", &nPotions);
			pRoot->Attribute("Portal", &nPortal);
			pRoot->Attribute("Keys", &nKeys);
			pRoot->Attribute("Help", &nHelp);

			TiXmlElement* pMap = pRoot->FirstChildElement("Map");
			szMapName = pMap->GetText();

			m_vUnspent.push_back(nUnspent);
			m_vCharacter.push_back(nCharacter);
			m_vExperience.push_back(nExperience);
			m_vCurrency.push_back(nCurrency);
			m_vLevel.push_back(nLevel);
			m_vBody.push_back(nBody);
			m_vMind.push_back(nMind);
			m_vSoul.push_back(nSoul);
			m_vHealth.push_back(nHealth);
			m_vArtifact1.push_back(nArtifact1);
			m_vArtifact2.push_back(nArtifact2);
			m_vArtifact3.push_back(nArtifact3);
			m_vArtifact4.push_back(nArtifact4);
			m_vShards.push_back(nShards);
			m_vPotions.push_back(nPotions);
			m_vHelp.push_back(nHelp);
			/////////////////////////////////////////////////////////////////
			// BUG FIX
			// Reference Bug # BB-003
			// BUG FIX START
			/////////////////////////////////////////////////////////////////

			//m_vPortal.push_back(nCharacter);
			m_vPortal.push_back(nPortal);

			///////////////////////////////////////////////////////////////////////////
			// BUG FIX END  Reference # BB-003
			//////////////////////////////////////////////////////////////////////////

			m_vMapName.push_back(szMapName);
			m_vKeys.push_back(nKeys);

			TiXmlElement* pItem = pRoot->FirstChildElement("Item");

			vector<int> vector;

			int i = 0;
			while(pItem != nullptr)
			{
				int nGem;
				int nType;
				int nID;
				int nEquipped;

				pItem->Attribute("Gem", &nGem);
				pItem->Attribute("Type", &nType);
				pItem->Attribute("Equipped", &nEquipped);
				pItem->Attribute("ID", &nID);

				vector.push_back(nEquipped);
				vector.push_back(nGem);
				vector.push_back(nType);
				vector.push_back(nID);

				i++;
				pItem = pItem->NextSiblingElement("Item");
			}

			m_vItems.push_back(vector);
		}
		else
		{
			m_vActive.push_back(0);
			m_vUnspent.push_back(-1);
			m_vCharacter.push_back(-1);
			m_vExperience.push_back(-1);
			m_vCurrency.push_back(-1);
			m_vLevel.push_back(-1);
			m_vBody.push_back(-1);
			m_vMind.push_back(-1);
			m_vSoul.push_back(-1);
			m_vHealth.push_back(-1);
			m_vArtifact1.push_back(-1);
			m_vArtifact2.push_back(-1);
			m_vArtifact3.push_back(-1);
			m_vArtifact4.push_back(-1);
			m_vShards.push_back(-1);
			m_vPotions.push_back(-1);
			m_vPortal.push_back(-1);
			m_vHelp.push_back(0);
			m_vMapName.push_back("-1");
			m_vKeys.push_back(-1);
			vector<int> vector;
			vector.push_back(-1);
			m_vItems.push_back(vector);
		}
	}
}

void CLoadGameState::Exit()
{
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

	if (m_nMenuInfoImgID != -1)
	{
		m_pTM->UnloadTexture(m_nMenuInfoImgID);
		m_nMenuInfoImgID = -1;
	}
}

/*virtual*/ bool CLoadGameState::Input( void )
{
	if (!m_bFadingOut)
	{
		if (m_pIM->CheckDown() )
		{
			++m_nSelectedIndex;
			if (m_nSelectedIndex > 4)
				m_nSelectedIndex = 1;

			CAudioManager::GetInstance()->PlaySFX(SFX_CURSOR);
		}

		if (m_pIM->CheckUp() )
		{
			--m_nSelectedIndex;
			if (m_nSelectedIndex < 1)
				m_nSelectedIndex = 4;

			CAudioManager::GetInstance()->PlaySFX(SFX_CURSOR);
		}

		if (m_pIM->CheckEnter() )
		{
			if(m_nSelectedIndex != 4 && m_vActive[m_nSelectedIndex - 1] == 0 && !m_bNewGame)
			{
				return true;
			}

			CAudioManager::GetInstance()->PlaySFX(SFX_CONFIRM);
			m_bFadingOut = true;
			m_fFadeTimer = 0.0f;

			if (m_nSelectedIndex <= 3)
			{
				Burst.SetXPosition(250);
				Burst.SetYPosition(190 + (75 * m_nSelectedIndex));
			}
			else
			{
				SelectionFat.SetYPosition(-100);
				Burst.SetXPosition(400);
				Burst.SetYPosition(520);
			}
			Burst.Initialize();
		}
	}

	return true;
}

// Update
/*virtual*/ void	CLoadGameState::Update( float fElapsedTime )
{
	if (m_bFadingIn || m_bFadingOut)
	{
		Burst.Update( fElapsedTime );
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
			Burst.Update( fElapsedTime );
			m_nAlphaOffset = 255;
			m_bFadingOut = false;

			/////////////////////////////////////////////////////////////////
			// BUG FIX
			// Reference Bug # BB-003
			// BUG FIX START
			/////////////////////////////////////////////////////////////////
			switch( m_nSelectedIndex )
			{
			case 1:	//Save Slot 1
				CGame::GetInstance()->SetSaveSlot("SaveSlot1.xml");

				// If it's from playstate go back so it doesn't add a second playstate to stack -Evan
				if(m_bFromPause)
				{
					CGame::GetInstance()->ChangeToPrev(2);
				}

				CGame::GetInstance()->ChangeToNew(PlayState::GetInstance());
				PlayState::GetInstance()->UsedSaveSlot();

				break;
			case 2:	//Save Slot 2
				CGame::GetInstance()->SetSaveSlot("SaveSlot2.xml");
				
				// If it's from playstate go back so it doesn't add a second playstate to stack -Evan
				if(m_bFromPause)
				{
					CGame::GetInstance()->ChangeToPrev(2);
				}

				CGame::GetInstance()->ChangeToNew(PlayState::GetInstance());
				PlayState::GetInstance()->UsedSaveSlot();
				break;
			case 3: //Save Slot 3
				CGame::GetInstance()->SetSaveSlot("SaveSlot3.xml");
				
				// If it's from playstate go back so it doesn't add a second playstate to stack -Evan
				if(m_bFromPause)
				{
					CGame::GetInstance()->ChangeToPrev(2);
				}

				CGame::GetInstance()->ChangeToNew(PlayState::GetInstance());
				PlayState::GetInstance()->UsedSaveSlot();
				break;
			case 4: // Back
				m_nAlphaOffset = 255;
				m_bFadingOut = false;
				/////////////////////////////////////////////////////////////////
				// BUG FIX
				// Reference Bug # BB-018
				// BUG FIX START
				/////////////////////////////////////////////////////////////////

				// If it's from playstate, pause the game when going back -Evan
				if(m_bFromPause)
				{
					PlayState::GetInstance()->PauseState();
					m_bFromPause = false;
				}
				
				///////////////////////////////////////////////////////////////////////////
				// BUG FIX END  Reference # BB-018
				//////////////////////////////////////////////////////////////////////////
				
				CGame::GetInstance()->ChangeToPrev();
				break;
			}

			//Load the correct map data
			if( this->m_bNewGame )	//Load blank data
			{
				LevelManager::GetInstance()->LoadMapData(0);
				PlayState::GetInstance()->SetMapName("Home");
				PlayState::GetInstance()->SetPortal(0);
			}
			else if( m_nSelectedIndex != 4 )
			{
				LevelManager::GetInstance()->LoadMapData(m_nSelectedIndex);
				string szMapName = this->GetMapName(m_nSelectedIndex - 1);
				PlayState::GetInstance()->SetMapName(szMapName);
			}

			return;

			///////////////////////////////////////////////////////////////////////////
			// BUG FIX END  Reference # BB-003
			//////////////////////////////////////////////////////////////////////////

		}
	}

	SelectionLarge.SetYPosition( 195 + (75 * m_nSelectedIndex)  );

	if (m_nSelectedIndex == 4)
	{
		SelectionLarge.SetYPosition(-100);
		SelectionFat.SetYPosition(530);
	}
	else
		SelectionFat.SetYPosition(-100);

	SelectionLarge.Update( fElapsedTime );
	SelectionFat.Update( fElapsedTime );

}
/*Virtual*/ void CLoadGameState::Render()
{

	CSGD_TextureManager::GetInstance()->Draw(m_nBackgroundImgID,0,0,1.0f,1.0f,0,0,0,0,D3DCOLOR_ARGB(255,255,255,255));

	if (m_bFadingOut)
		Burst.Render();

	m_nAlpha[0] = 100 - (int)m_nAlphaOffset;
	m_nAlpha[1] = 100 - (int)m_nAlphaOffset;
	m_nAlpha[2] = 100 - (int)m_nAlphaOffset;
	m_nAlpha[3] = 100 - (int)m_nAlphaOffset;

	m_nAlpha[m_nSelectedIndex - 1] = 200 - (int)m_nAlphaOffset;

	for (int i = 0; i < 4; ++i)
	{
		if (m_nAlpha[i] < 0 )
			m_nAlpha[i] = 0;
		if (m_nAlpha[i] > 255)
			m_nAlpha[i] = 255;
	}

	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();

	int x = 450;
	int y = 100;
	CSGD_TextureManager::GetInstance()->Draw(m_nMenuInfoImgID,x,y,1.1f,1.2f,0,0,0,0,D3DCOLOR_ARGB(255,255,255,255));	

	if(m_nSelectedIndex != 4 && m_vActive[m_nSelectedIndex - 1])
	{
		ostringstream str;

		str << "Level: " << m_vLevel[m_nSelectedIndex - 1];
		m_pBF->Draw(str.str(), x + 45, y + 15, 0.5f, D3DCOLOR_ARGB(255, 255, 255, 255));
		str.str("");

		str << "Map: " << m_vMapName[m_nSelectedIndex - 1];
		m_pBF->Draw(str.str(), x + 45, y + 35, 0.4f, D3DCOLOR_ARGB(255, 255, 255, 255));
		str.str("");
		
		/////////////////////////////////////////////////////////////////
		// BUG FIX
		// Reference Bug # BB-040
		// BUG FIX START
		/////////////////////////////////////////////////////////////////

		// Increased font scale to 0.3f
		if(m_vArtifact1[m_nSelectedIndex - 1] != -1)
		{
			str << "Fire: " << m_vArtifact1[m_nSelectedIndex - 1];
			m_pBF->Draw(str.str(), x + 45, y + 55, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
			str.str("");
		}
		else
		{
			m_pBF->Draw("Fire: -", x + 45, y + 55, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		if(m_vArtifact2[m_nSelectedIndex - 1] != -1)
		{
			str << "Ice: " << m_vArtifact2[m_nSelectedIndex - 1];
			m_pBF->Draw(str.str(), x + 135, y + 55, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
			str.str("");
		}
		else
		{
			m_pBF->Draw("Ice: -", x + 135, y + 55, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		if(m_vArtifact3[m_nSelectedIndex - 1] != -1)
		{
			str << "Lightning: " << m_vArtifact3[m_nSelectedIndex - 1];
			m_pBF->Draw(str.str(), x + 45, y + 70, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
			str.str("");
		}
		else
		{
			m_pBF->Draw("Lightning: -", x + 45, y + 70, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		if(m_vArtifact4[m_nSelectedIndex - 1] != -1)
		{
			str << "Earth: " << m_vArtifact4[m_nSelectedIndex - 1];
			m_pBF->Draw(str.str(), x + 135, y + 70, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
			str.str("");
		}
		else
		{
			m_pBF->Draw("Earth: -", x + 135, y + 70, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		str << "Currency: " << m_vCurrency[m_nSelectedIndex - 1];
		m_pBF->Draw(str.str(), x + 45, y + 85, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		str.str("");

		str << "Keys: " << m_vKeys[m_nSelectedIndex - 1];
		m_pBF->Draw(str.str(), x + 135, y + 100, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		str.str("");

		str << "Shards: " << m_vShards[m_nSelectedIndex - 1];
		m_pBF->Draw(str.str(), x + 45, y + 100, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		str.str("");
	}
	else
	{
		m_pBF->Draw("Level: -", x + 45, y + 15, 0.5f, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pBF->Draw("Map: -", x + 45, y + 35, 0.4f, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pBF->Draw("Fire: -", x + 45, y + 55, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pBF->Draw("Ice: -", x + 135, y + 55, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pBF->Draw("Lightning: -", x + 45, y + 70, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pBF->Draw("Earth: -", x + 135, y + 70, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pBF->Draw("Currency: -", x + 45, y + 85, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pBF->Draw("Keys: -", x + 135, y + 100, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pBF->Draw("Shards: -", x + 45, y + 100, 0.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	CSGD_TextureManager::GetInstance()->Draw(m_nMenuButtonImgID,60,250,2.0f,0.7f,0,0,0,0,D3DCOLOR_ARGB(m_nAlpha[0],255,255,255));	

	if(!m_vActive[0])
		m_pBF->Draw("Slot 1 - Empty", 120, 260, 0.6f, D3DCOLOR_ARGB(m_nAlpha[0], 255, 255, 255));
	else	
		m_pBF->Draw("Slot 1 - Active", 120, 260, 0.6f, D3DCOLOR_ARGB(m_nAlpha[0], 255, 255, 255));


	CSGD_TextureManager::GetInstance()->Draw(m_nMenuButtonImgID,60,325,2.0f,0.7f,0,0,0,0,D3DCOLOR_ARGB(m_nAlpha[1],255,255,255));	
	if(!m_vActive[1])
		m_pBF->Draw("Slot 2 - Empty", 120, 335, 0.6f, D3DCOLOR_ARGB(m_nAlpha[1], 255, 255, 255));
	else	
		m_pBF->Draw("Slot 2 - Active", 120, 335, 0.6f, D3DCOLOR_ARGB(m_nAlpha[1], 255, 255, 255));

	CSGD_TextureManager::GetInstance()->Draw(m_nMenuButtonImgID,60,400,2.0f,0.7f,0,0,0,0,D3DCOLOR_ARGB(m_nAlpha[2],255,255,255));
	if(!m_vActive[2])
		m_pBF->Draw("Slot 3 - Empty", 120, 410, 0.6f, D3DCOLOR_ARGB(m_nAlpha[2], 255, 255, 255));
	else	
		m_pBF->Draw("Slot 3 - Active", 120, 410, 0.6f, D3DCOLOR_ARGB(m_nAlpha[2], 255, 255, 255));

	CSGD_TextureManager::GetInstance()->Draw(m_nMenuButtonImgID,300,500,1.0f,1.0f,0,0,0,0,D3DCOLOR_ARGB(m_nAlpha[3],255,255,255));	
	m_pBF->Draw("Back", 340, 520, 0.8f, D3DCOLOR_ARGB(m_nAlpha[3], 255, 255, 255));

	if (!m_bFadingOut)
	{
		SelectionLarge.Render();
		/////////////////////////////////////////////////////////////////
		// BUG FIX
		// Reference Bug # BB-087
		// BUG FIX START
		/////////////////////////////////////////////////////////////////
		// Added a conditional for rendering the Fat particle effect
		if (m_nSelectedIndex == 4)
		SelectionFat.Render();
	}

}

void CLoadGameState::UpdatePlayer(CPlayer& pPlayer)
{
	int index;

	if(CGame::GetInstance()->GetSaveSlot() == "SaveSlot1.xml")
	{
		index = 0;
	}
	else if(CGame::GetInstance()->GetSaveSlot() == "SaveSlot2.xml")
	{
		index = 1;
	}
	else
	{
		index = 2;
	}

	if(!m_bNewGame)
	{
		pPlayer.SetBodyEx(m_vBody[index]);
		pPlayer.SetMindEx(m_vMind[index]);
		pPlayer.SetCharacterEx(m_vCharacter[index]);
		pPlayer.SetSoulEx(m_vSoul[index]);

		pPlayer.ClearEquips();

		for(unsigned int i = 0; i < this->m_vItems[index].size() / 4; i++)
		{
			CItem * iItem = new CItem(*CItemManager::GetInstance()->GetItem(m_vItems[index][i * 4 + 3]));
			CItem * iGem = nullptr;

			if(m_vItems[index][i * 4 + 1] != -1)
			{
				iGem = new CItem(*CItemManager::GetInstance()->GetItem(m_vItems[index][i * 4 + 1]));
				iItem->SetGem(iGem);
			}

			iItem->SetEquipped(m_vItems[index][i * 4] == 1 ? true : false);

			pPlayer.AddItem(iItem);

			if(iGem != nullptr)
				delete iGem;
			delete iItem;
		}

		pPlayer.SetCurrency(m_vCurrency[index]);
		pPlayer.SetLevel(m_vLevel[index]);
		
		/////////////////////////////////////////////////////////////////
		// BUG FIX
		// Reference Bug # BB-044
		// BUG FIX START
		/////////////////////////////////////////////////////////////////

		// Sets the exp needed to level to the correct amount
		pPlayer.SetExpNeeded(m_vLevel[index]);

		///////////////////////////////////////////////////////////////////////////
		// BUG FIX END  Reference # BB-044
		//////////////////////////////////////////////////////////////////////////
		
		pPlayer.GainXP(m_vExperience[index]);
		pPlayer.SetPotion(m_vPotions[index]);
		pPlayer.SetUnspent(m_vUnspent[index]);
		pPlayer.SetShards(m_vShards[index]);

		if(m_vArtifact1[index] != -1)
		{
			pPlayer.AddArtifact(true);

			for(int i = 0; i < m_vArtifact1[index]; i++)
			{
				pPlayer.IncreaseArtifact(0);
			}

			if(m_vArtifact2[index] != -1)
			{
				pPlayer.AddArtifact(true);

				for(int i = 0; i < m_vArtifact2[index]; i++)
				{
					pPlayer.IncreaseArtifact(1);
				}

				if(m_vArtifact3[index] != -1)
				{
					pPlayer.AddArtifact(true);

					for(int i = 0; i < m_vArtifact3[index]; i++)
					{
						pPlayer.IncreaseArtifact(2);
					}

					if(m_vArtifact4[index] != -1)
					{
						pPlayer.AddArtifact(true);

						for(int i = 0; i < m_vArtifact4[index]; i++)
						{
							pPlayer.IncreaseArtifact(3);
						}
					}
				}
			}
		}

		for(int i = 0; i < m_vKeys[index]; i++)
		{
			pPlayer.AddKey();
		}

		/////////////////////////////////////////////////////////////////
		// BUG FIX
		// Reference Bug # BB-084
		// BUG FIX START
		/////////////////////////////////////////////////////////////////

		// Set help message display
		pPlayer.SetHelpMessages(m_vHelp[index]);

		///////////////////////////////////////////////////////////////////////////
		// BUG FIX END  Reference # BB-084
		//////////////////////////////////////////////////////////////////////////

		pPlayer.CalculateStats();
		pPlayer.ModifyHealth(pPlayer.GetMaxHealth());
	}
}

void CLoadGameState::UpdateMap(void)
{
	if( !this->NewGame() )
	{
		int index;

		if(CGame::GetInstance()->GetSaveSlot() == "SaveSlot1.xml")
			index = 0;
		else if(CGame::GetInstance()->GetSaveSlot() == "SaveSlot2.xml")
			index = 1;
		else
			index = 2;

		PlayState::GetInstance()->SetMapName( this->m_vMapName[index] );
		PlayState::GetInstance()->SetPortal( this->m_vPortal[index] );
	}
}