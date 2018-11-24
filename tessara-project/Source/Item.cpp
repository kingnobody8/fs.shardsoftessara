/*
	Author: Benjamin "Evan" Cowles
	File: Item.cpp
*/

#include "Item.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "..\SGD Wrappers\CSGD_Direct3D.h"
#include "MessageSystem.h"
#include "AddItemMsg.h"
#include "ItemManager.h"
#include "Entity.h"
#include "Camera.h"
#include "Player.h"
#include "DeleteEntityMessage.h"


CItem::CItem()
{
	m_nPosX = 0;
	m_nPosY = 0;
	m_szName = "";
	m_nType = 0;
	m_nSpecial= 0;
	m_nBody= 0;
	m_nMind= 0;
	m_nChar= 0;
	m_nSoul= 0;
	m_nValue= 0;
	m_nID= 0;
	m_nImgID= 0;
	m_nGemImgID= 0;
	m_bEquipped= 0;
	m_pGem = nullptr;

	float m_fPickUpTimer;

	//int m_nPosX;
	//int m_nPosY;

	bool picked;
	this->SetType(ENT_ITEM);
	picked = false;
	m_fPickUpTimer = 0.0f;
	m_nCurrAnim = -1;
}

CItem::~CItem()
{

}

CItem::CItem(int nType, string szName, int nSpecial, int nBody, int nMind, int nChar, int nSoul, int nValue, int nID)
{
	this->m_szName = szName;
	this->m_nType = nType;
	this->m_nSpecial = nSpecial;
	this->m_nBody = nBody;
	this->m_nMind = nMind;
	this->m_nChar = nChar;
	this->m_nSoul = nSoul;
	this->m_nValue = nValue;
	this->m_nID = nID;
	this->m_bEquipped = false;
	this->m_pGem = nullptr;

	SetType( ENT_ITEM);

	if(this->m_szName != "-1")
	{
		if(this->m_szName == "body")
		{
			this->m_nImgID = CItemManager::GetInstance()->GetImgID(1);
			this->m_nGemImgID = CItemManager::GetInstance()->GetImgID(0);
		}
		else if(this->m_szName == "mind")
		{
			this->m_nImgID = CItemManager::GetInstance()->GetImgID(2);
			this->m_nGemImgID = CItemManager::GetInstance()->GetImgID(0);
		}
		else if(this->m_szName == "soul")
		{
			this->m_nImgID = CItemManager::GetInstance()->GetImgID(3);
			this->m_nGemImgID = CItemManager::GetInstance()->GetImgID(0);
		}
		else if(this->m_szName == "char")
		{
			this->m_nImgID = CItemManager::GetInstance()->GetImgID(4);
			this->m_nGemImgID = CItemManager::GetInstance()->GetImgID(0);
		}
		else
		{
			string szImageFilePathAlpha = "Resource/Graphics/Items/" + this->m_szName + ".png";
			const char * szImageFilePathBeta = szImageFilePathAlpha.c_str();

			TCHAR * szImageFilePathGold = new TCHAR[100];

			mbstowcs_s(nullptr, szImageFilePathGold, 100, szImageFilePathBeta, _TRUNCATE);

			this->m_nImgID = CSGD_TextureManager::GetInstance()->LoadTexture(szImageFilePathGold);

			delete szImageFilePathGold;
			szImageFilePathGold = nullptr;

			this->m_nGemImgID = CItemManager::GetInstance()->GetImgID(0);
		}
	}
}

int CItem::GetBody() const
{
	if(this->m_pGem == nullptr)
	{
		return this->m_nBody;
	}
	else
	{
		return (this->m_nBody + this->m_pGem->GetBody());
	}
}

int CItem::GetMind() const
{
	if(this->m_pGem == nullptr)
	{
		return this->m_nMind;
	}
	else
	{
		return (this->m_nMind + this->m_pGem->GetMind());
	}
}

int CItem::GetSoul() const
{
	if(this->m_pGem == nullptr)
	{
		return this->m_nSoul;
	}
	else
	{
		return (this->m_nSoul + this->m_pGem->GetSoul());
	}
}

int CItem::GetChar() const
{
	if(this->m_pGem == nullptr)
	{
		return this->m_nChar;
	}
	else
	{
		return (this->m_nChar + this->m_pGem->GetChar());
	}
}

void CItem::SetGem(CItem * pGem)
{
	if(pGem != nullptr)
	{
		this->m_pGem = new CItem(*pGem);
	}
	else
	{
		this->m_pGem = nullptr;
	}
}

int CItem::GetValue()
{
	if(this->m_pGem == nullptr)
	{
		return this->m_nValue;
	}
	else
	{
		return (this->m_nValue + this->m_pGem->GetValue());
	}
}


void	CItem::Update ( float fElapsedTime )
{
	m_fPickUpTimer -= fElapsedTime;
	if (m_fPickUpTimer <= 0.01)
	{
		picked = false;
		m_fPickUpTimer = 0.4f;
	}
}

void	CItem::Render ( void )
{
	RECT rec; 
	RECT rCamera;
	Camera::GetInstance()->CamRect(&rCamera);
	rec.top = GetPosY() - rCamera.top;
	rec.bottom = rec.top + 30;
	rec.left = GetPosX() - rCamera.left;
	rec.right = rec.left  + 30;

	CSGD_TextureManager::GetInstance()->Draw(m_nImgID, rec.left, rec.top, 1.0f, 1.0f);
	//CSGD_Direct3D::GetInstance()->DrawRect(rec, 255, 0, 0);
}

RECT	CItem::GetRect( void ) const
{
	RECT r;
	r.top = GetPosY();
	r.bottom = r.top + 30;
	r.left = GetPosX();
	r.right = r.left + 30;

	return r;
}


bool	CItem::CheckCollision( IEntity* pOther )
{
	RECT rOther = pOther->GetRect();
	RECT rItem = this->GetRect();
	RECT rOver;

	if (IntersectRect(&rOver, &rOther, &rItem) && !picked)
	{
		if (pOther->GetType() == ENT_PLAYER)
		{
			picked = true;
			m_fPickUpTimer = 0.4f;
			CPlayer* pl = reinterpret_cast<CPlayer*> (pOther);
			pl->AddItem(this);

			AddItemMsg* Msg = new AddItemMsg(this, false);
			CMessageSystem::GetInstace()->SendMsg(Msg);
			Msg= nullptr;

			return true;
		}
	}
	return true;
}

// Reference Counting:
void	CItem::AddRef ( void )
{

}

void	CItem::Release( void )
{

}