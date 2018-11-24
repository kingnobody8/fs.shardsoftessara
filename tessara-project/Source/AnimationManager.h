/*
	Author: Benjamin "Evan" Cowles
	File: AnimationManager.h
*/

#pragma once
#include "AnimationInformation.h"
#include "Animation.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include <vector>
using namespace std;

class AnimationManager
{
	static AnimationManager * s_pInstance;
	
	CSGD_TextureManager * m_pTM;
	CSGD_Direct3D * m_pD3D;
	vector<Animation> m_vAnimations;
	int m_nTilesetID;

	AnimationManager();
	~AnimationManager();
	AnimationManager(const AnimationManager&);
	AnimationManager& operator= (const AnimationManager&);
public:
	static AnimationManager * GetInstance();
	static void DeleteInstance();
	void Initialize();
	void Draw(AnimationInformation animInfo, int nX, int nY, float fRotX = 0.0f, float fRotY = 0.0f, float fRotC = 0.0f, int nAlpha = 255);
	void Load(string szFilename);
	void Unload();
	void Update(AnimationInformation & animInfo, float fElapsedTime);
	void LoadTileSet(string szFilename);
	void RenderTile( RECT tile, int nX, int nY, int alpha );

	Animation GetAnimation(int nAnimID);
	int GetAnimID(string szName);
};