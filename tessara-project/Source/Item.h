/*
	Author: Benjamin "Evan" Cowles
	File: Item.h
*/

#pragma once
#include "Entity.h"
#include <string>
#include <vector>
using namespace std;

class CItem : public CEntity
{
private:
	string m_szName;
	int m_nType;
	int m_nSpecial;
	int m_nBody;
	int m_nMind;
	int m_nChar;
	int m_nSoul;
	int m_nValue;
	int m_nID;
	int m_nImgID;
	int m_nGemImgID;
	bool m_bEquipped;
	CItem* m_pGem;

	float m_fPickUpTimer;

	int m_nPosX;
	int m_nPosY;

	bool picked;

public:
	CItem();
	CItem(int nType, string szName, int nSpecial, int nBody, int nMind, int nChar, int nSoul, int nValue, int nID);
	~CItem();

	int GetType() const { return this->m_nType; }
	int GetID() const { return this->m_nID; }
	int Equipped() const { return this->m_bEquipped; }
	int GetImgID() const { return this->m_nImgID; }
	string GetName() const { return this->m_szName; }
	int GetSpecial() const { return this->m_nSpecial; }
	int GetBody() const;
	int GetMind() const;
	int GetSoul() const;
	int GetChar() const;
	bool HasGem() const { return (this->m_pGem != nullptr) ? true : false; }
	CItem * GetGem() const { return this->m_pGem; }
	int GetGemImgID() const { return this->m_nGemImgID; }

	void SetEquipped(bool bEquipped) { this->m_bEquipped = bEquipped; }
	void SetImgID(int nImgID) { this->m_nImgID = nImgID; }
	void SetGem(CItem * pGem);
	int GetValue();

	//int GetPosX() { return m_nPosX; }
	//int GetPosY() {return m_nPosY; }
	//void SetPosX(int nPosX) {m_nPosX = nPosX; }
	//void SetPosY(int nPosY) {m_nPosY = nPosY; }

	void	Update ( float fElapsedTime );
	void	Render ( void );
	
	RECT	GetRect( void ) const;
	bool	CheckCollision( IEntity* pOther );

	// Reference Counting:
	void	AddRef ( void );
	void	Release( void );
};