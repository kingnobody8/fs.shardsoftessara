/*
	Author: Benjamin "Evan" Cowles
	File: ItemManager.cpp
*/

#include "ItemManager.h"
#include "../TinyXML/tinyxml.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "MessageSystem.h"
#include "AddItemMsg.h"

CItemManager * CItemManager::s_pInstance = nullptr;

CItemManager::CItemManager()
{

}

CItemManager::~CItemManager()
{

}

CItemManager * CItemManager::GetInstance()
{
	if (s_pInstance == nullptr)
	{
		s_pInstance = new CItemManager;
	}

	return s_pInstance;
}

void CItemManager::DeleteInstance()
{
	delete s_pInstance;
	s_pInstance = nullptr;
}

void CItemManager::Initialize()
{
	this->m_pTM = CSGD_TextureManager::GetInstance();

	this->m_vImgIDs.push_back(this->m_pTM->LoadTexture(_T("Resource/Graphics/Items/gem_empty.png")));
	this->m_vImgIDs.push_back(this->m_pTM->LoadTexture(_T("Resource/Graphics/Items/gem_body.png")));
	this->m_vImgIDs.push_back(this->m_pTM->LoadTexture(_T("Resource/Graphics/Items/gem_mind.png")));
	this->m_vImgIDs.push_back(this->m_pTM->LoadTexture(_T("Resource/Graphics/Items/gem_soul.png")));
	this->m_vImgIDs.push_back(this->m_pTM->LoadTexture(_T("Resource/Graphics/Items/gem_char.png")));
}

void CItemManager::Load(string szFilename)
{
	TiXmlDocument doc;

	// Load the document
	string szFilePath = "Resource/XML/" + szFilename;

	doc.LoadFile(szFilePath.c_str());

	// Access the root node
	TiXmlElement * pRoot = doc.RootElement();
	TiXmlElement * pWeapons = pRoot->FirstChildElement("Weapons");
	TiXmlElement * pWeapon = pWeapons->FirstChildElement("Weapon");

	while(pWeapon != nullptr)
	{
		string szName;
		int nDamage;
		int nBody;
		int nMind;
		int nChar;
		int nSoul;
		int nValue;
		int nID;

		TiXmlElement * pName = pWeapon->FirstChildElement("Name");
		szName = pName->GetText();
		pWeapon->Attribute("damage", &nDamage);
		pWeapon->Attribute("body", &nBody);
		pWeapon->Attribute("mind", &nMind);
		pWeapon->Attribute("char", &nChar);
		pWeapon->Attribute("soul", &nSoul);
		pWeapon->Attribute("value", &nValue);
		pWeapon->Attribute("id", &nID);

		CItem tempItem(0, szName, nDamage, nBody, nMind, nChar, nSoul, nValue, nID);

		m_vItems.push_back(tempItem);

		pWeapon = pWeapon->NextSiblingElement("Weapon");
	}

	TiXmlElement * pArmors = pRoot->FirstChildElement("Armors");
	TiXmlElement * pArmor = pArmors->FirstChildElement("Armor");

	while(pArmor != nullptr)
	{
		string szName;
		int nArmor;
		int nBody;
		int nMind;
		int nChar;
		int nSoul;
		int nValue;
		int nID;
		
		TiXmlElement * pName = pArmor->FirstChildElement("Name");
		szName = pName->GetText();
		pArmor->Attribute("armor", &nArmor);
		pArmor->Attribute("body", &nBody);
		pArmor->Attribute("mind", &nMind);
		pArmor->Attribute("char", &nChar);
		pArmor->Attribute("soul", &nSoul);
		pArmor->Attribute("value", &nValue);
		pArmor->Attribute("id", &nID);

		CItem tempItem(1, szName, nArmor, nBody, nMind, nChar, nSoul, nValue, nID);

		m_vItems.push_back(tempItem);

		pArmor = pArmor->NextSiblingElement("Armor");
	}

	TiXmlElement * pPotion = pRoot->FirstChildElement("Potion");

	string szName;
	int nAmount;
	int nValue;
	
	TiXmlElement * pName = pPotion->FirstChildElement("Name");
	szName = pName->GetText();
	pPotion->Attribute("amount", &nAmount);
	pPotion->Attribute("value", &nValue);

	CItem tempItem(2, szName, nAmount, -1, -1, -1, -1, nValue, 0);

	m_vItems.push_back(tempItem);

	TiXmlElement * pGems = pRoot->FirstChildElement("Gems");
	TiXmlElement * pGem = pGems->FirstChildElement("Gem");

	while(pGem != nullptr)
	{
		string szName;
		int nBody;
		int nMind;
		int nChar;
		int nSoul;
		int nValue;
		int nID;
		
		TiXmlElement * pName = pGem->FirstChildElement("Name");
		szName = pName->GetText();
		pGem->Attribute("body", &nBody);
		pGem->Attribute("mind", &nMind);
		pGem->Attribute("char", &nChar);
		pGem->Attribute("soul", &nSoul);
		pGem->Attribute("value", &nValue);
		pGem->Attribute("id", &nID);

		CItem tempGem(3, szName, -1, nBody, nMind, nChar, nSoul, nValue, nID);

		if(szName == "body")
		{
			tempGem.SetImgID(this->m_vImgIDs[GEM_BODY]);
		}
		else if(szName == "mind")
		{
			tempGem.SetImgID(this->m_vImgIDs[GEM_MIND]);
		}
		else if(szName == "soul")
		{
			tempGem.SetImgID(this->m_vImgIDs[GEM_SOUL]);
		}
		else
		{
			tempGem.SetImgID(this->m_vImgIDs[GEM_CHAR]);
		}


		m_vItems.push_back(tempGem);

		pGem = pGem->NextSiblingElement("Gem");
	}


	GetItems(3, m_vGems);
	GetItems(2, m_nPotions);
}

CItem * CItemManager::GetItem(int nID)
{
	for(unsigned int i = 0; i < this->m_vItems.size(); i++)
	{
		if(this->m_vItems[i].GetID() == nID)
		{
			return &this->m_vItems[i];
		}
	}

	return nullptr;
}

int CItemManager::GetImgID(int nType)
{
	if(nType == 1)
	{
		return this->m_vImgIDs[GEM_BODY];
	}
	else if(nType == 2)
	{
		return this->m_vImgIDs[GEM_MIND];
	}
	if(nType == 3)
	{
		return this->m_vImgIDs[GEM_CHAR];
	}
	if(nType == 4)
	{
		return this->m_vImgIDs[GEM_SOUL];
	}
	else
	{
		return this->m_vImgIDs[GEM_EMPTY];
	}
}


void CItemManager::GetItems(int nType, vector<CItem> &vItems)
{
	std::vector< CItem >::iterator iter = m_vItems.begin();
	for ( ; iter != m_vItems.end(); ++iter )
	{
		if ( (*iter).GetType() == nType)
			vItems.push_back((*iter));
	}

}


void CItemManager::DropItem(int nLevel, int nPosX, int nPosY)
{
	int r = rand()% 3;
	//find whitch item to drop 

	if (r == 0)
	{
		int  l = nLevel * 4;
		int Item = rand()% 4;
		m_vGems[Item + l].SetPosX(nPosX);
		m_vGems[Item + l].SetPosY(nPosY);
		AddItemMsg* Msg = new AddItemMsg(&m_vGems[Item + l], true);
		CMessageSystem::GetInstace()->SendMsg(Msg);
	}
	else if (r == 1)
	{
		m_nPotions[0].SetPosX(nPosX);
		m_nPotions[0].SetPosY(nPosY);
		AddItemMsg* Msg = new AddItemMsg(&m_nPotions[0], true);
		CMessageSystem::GetInstace()->SendMsg(Msg);
	}

}