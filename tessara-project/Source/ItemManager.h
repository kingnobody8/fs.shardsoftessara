/*
	Author: Benjamin "Evan" Cowles
	File: ItemManager.h
*/

#pragma once

#include "Item.h"
#include <string>
using namespace std;

class CSGD_TextureManager;

enum {GEM_EMPTY, GEM_BODY, GEM_MIND, GEM_CHAR, GEM_SOUL};

class CItemManager
{
public:
	static CItemManager * GetInstance();
	static void DeleteInstance();
	void Initialize();

	void Load(string szFilename);
	CItem * GetItem(int nID);
	int GetImgID(int nType);
	void GetItems(int nType, vector<CItem> &vItems);
	void DropItem(int nLevel, int nPosX, int nPosY);

private:
	CItemManager();
	~CItemManager();
	CItemManager(const CItemManager&);
	CItemManager& operator= (const CItemManager&);

	static CItemManager * s_pInstance;
	CSGD_TextureManager* m_pTM;

	vector<CItem> m_vItems;
	vector<int> m_vImgIDs;
	vector<CItem> m_vGems;
	vector<CItem> m_nPotions;
};