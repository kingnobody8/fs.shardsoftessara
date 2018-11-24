/*
	Author: Benjamin "Evan" Cowles
	File: AnimationManager.cpp
*/

#include "AnimationManager.h"
#include "BitmapFont.h"
#include "../TinyXML/tinyxml.h"

AnimationManager * AnimationManager::s_pInstance = nullptr;

AnimationManager::AnimationManager()
{

}

AnimationManager::AnimationManager(const AnimationManager&)
{

}

AnimationManager& AnimationManager::operator= (const AnimationManager&)
{
	return *this;
}

AnimationManager::~AnimationManager()
{
	Unload();

	if(m_pTM != nullptr)
	{
		m_pTM = nullptr;
	}

	if(m_pD3D != nullptr)
	{
		m_pD3D = nullptr;
	}
}

AnimationManager * AnimationManager::GetInstance()
{
	if (s_pInstance == nullptr)
	{
		s_pInstance = new AnimationManager;
	}

	return s_pInstance;
}

void AnimationManager::DeleteInstance()
{
	delete s_pInstance;
	s_pInstance = nullptr;
}

void AnimationManager::Initialize()
{
	this->m_pTM = CSGD_TextureManager::GetInstance();
	this->m_pD3D = CSGD_Direct3D::GetInstance();

	this->m_nTilesetID = -1;
}

void AnimationManager::Draw(AnimationInformation animInfo, int nX, int nY, float fRotX, float fRotY, float fRotC, int nAlpha)
{
	Frame tempFrame = this->m_vAnimations[animInfo.GetAnimID()].GetFrame(animInfo.GetCurFrame());
	POINT pAnchor = tempFrame.GetAnchorPoint();
	RECT rDraw = *tempFrame.GetDrawRect();

	RECT rRect;

	RECT rPassive = *tempFrame.GetPassiveRect();

	int nHeight = rDraw.bottom - rDraw.top;
	int nWidth = rDraw.right - rDraw.left;

	rRect.bottom = nY + (rDraw.bottom - pAnchor.y);
	rRect.top = rRect.bottom - nHeight;
	rRect.right = nX + (rDraw.right - pAnchor.x);
	rRect.left = rRect.right - nWidth;

	this->m_pTM->Draw(this->m_vAnimations[animInfo.GetAnimID()].GetImgID(), rRect.left, rRect.top, 1.0f, 1.0f, 
		this->m_vAnimations[animInfo.GetAnimID()].GetFrame(animInfo.GetCurFrame()).GetDrawRect(), fRotX, fRotY, fRotC, D3DCOLOR_ARGB(nAlpha, 255, 255, 255));
	
	this->m_pD3D->GetSprite()->Flush();
}

void AnimationManager::Load(string szFilename)
{
	bool bIsLooping;
	int nIsLooping;
	int nImgID;
	int nNumFrames;
	int nAnimationCount;
	string szImageFilename;
	string szFilePath;
	string szName;
	vector<Frame> vFrames;
	const char * pText;

	// Create a TinyXML document
	TiXmlDocument doc;

	// Load the document
	szFilePath = "Resource/Graphics/Animation/" + szFilename;

	doc.LoadFile(szFilePath.c_str());

	// Access the root node
	TiXmlElement * pRoot = doc.RootElement();
	pRoot->Attribute("Value", &nAnimationCount);

	TiXmlElement * pFilename = pRoot->FirstChildElement("Filename");

	pText = pFilename->GetText();
	string szImageFilePathAlpha = pText;
	szImageFilePathAlpha = "Resource/Graphics/Animation/" + szImageFilePathAlpha;

	const char * szImageFilePathBeta = szImageFilePathAlpha.c_str();

	TCHAR * szImageFilePathGold = new TCHAR[100];

	mbstowcs_s(nullptr, szImageFilePathGold, 100, szImageFilePathBeta, _TRUNCATE);

	nImgID = CSGD_TextureManager::GetInstance()->LoadTexture(szImageFilePathGold);

	delete szImageFilePathGold;
	szImageFilePathGold = nullptr;

	// Get Animation information
	TiXmlElement * pAnimation = pRoot->FirstChildElement("Animation");

	for(int a = 0; a < nAnimationCount; a++)
	{
		pAnimation->Attribute("IsLooping", &nIsLooping);

		if(nIsLooping)
		{
			bIsLooping = true;
		}
		else
		{
			bIsLooping = false;
		}

		TiXmlElement * pName = pAnimation->FirstChildElement("Name");

		pText = pName->GetText();
		if(pText)
		{
			szName = pText;
		}
		
		// Get Frames information
		TiXmlElement * pFrames = pAnimation->FirstChildElement("Frames");

		pFrames->Attribute("Value", &nNumFrames);

		// Get Frame information
		TiXmlElement * pFrame = pFrames->FirstChildElement("Frame");

		for(int i = 0; i < nNumFrames; i++)
		{
			RECT rActive;
			RECT rPassive;
			RECT rRender;
			POINT pAnchor;
			double dDuration;
			float fDuration;
			bool bHasTrigger;
			bool bHasActiveRect;
			int nHasTrigger;
			int nHasActiveRect;
			int nPX;
			int nPY;
			int nX;
			int nY;
			int nHeight;
			int nWidth;
			string szTrigger;
			const char * pText;

			pFrame->Attribute("AnchorPointX", &nPX);		
			pAnchor.x = nPX;

			pFrame->Attribute("AnchorPointY", &nPY);
			pAnchor.y = nPY;

			pFrame->Attribute("Duration", &dDuration);
			fDuration = (float)dDuration;

			pFrame->Attribute("hasTrigger", &nHasTrigger);

			if(nHasTrigger)
			{
				bHasTrigger = true;

				TiXmlElement * tempTrigger = pFrame->FirstChildElement("Trigger");

				pText = tempTrigger->GetText();
				szTrigger = pText;
			}
			else
			{
				bHasTrigger = false;
				szTrigger = "-1";
			}

			TiXmlElement * tempActiveRect = pFrame->FirstChildElement("ActiveCollisionRect");

			tempActiveRect->Attribute("hasActiveRect", &nHasActiveRect);

			if(nHasActiveRect)
			{
				bHasActiveRect = true;

				tempActiveRect->Attribute("x", &nX);
				tempActiveRect->Attribute("y", &nY);
				tempActiveRect->Attribute("width", &nWidth);
				tempActiveRect->Attribute("height", &nHeight);

				rActive.left = nX;
				rActive.top = nY;
				rActive.right = nWidth;
				rActive.bottom = nHeight;
			}
			else
			{
				bHasActiveRect = false;

				rActive.left = -1;
				rActive.top = -1;
				rActive.right = -1;
				rActive.bottom = -1;
			}

			TiXmlElement * tempPassiveRect = pFrame->FirstChildElement("PassiveCollisionRect");

			tempPassiveRect->Attribute("x", &nX);
			tempPassiveRect->Attribute("y", &nY);
			tempPassiveRect->Attribute("width", &nWidth);
			tempPassiveRect->Attribute("height", &nHeight);

			rPassive.left = nX;
			rPassive.top = nY;
			rPassive.right = nWidth;
			rPassive.bottom = nHeight;
			//rPassive.right = rPassive.left + nWidth;
			//rPassive.bottom = rPassive.top + nHeight;

			TiXmlElement * tempRenderRect = pFrame->FirstChildElement("RenderRect");

			tempRenderRect->Attribute("x", &nX);
			tempRenderRect->Attribute("y", &nY);
			tempRenderRect->Attribute("width", &nWidth);
			tempRenderRect->Attribute("height", &nHeight);

			rRender.left = nX;
			rRender.top = nY;
			rRender.right = nWidth;
			rRender.bottom = nHeight;
			//rRender.right = rRender.left + nWidth;
			//rRender.bottom = rRender.top + nHeight;

			Frame tempFrame(bHasActiveRect, rActive, rRender, rPassive, pAnchor, fDuration, bHasTrigger, szTrigger);

			// Add frame to Frames vector
			vFrames.push_back(tempFrame);

			// Get next Frame information
			pFrame = pFrame->NextSiblingElement("Frame");
		}

		Animation tempAnimation(bIsLooping, nImgID, szName, vFrames, nNumFrames);

		this->m_vAnimations.push_back(tempAnimation);
		vFrames.clear();
		pAnimation = pAnimation->NextSiblingElement("Animation");
	}

	this->LoadTileSet("tileset.png");
	
}

void AnimationManager::Unload()
{
	for(unsigned int i = 0; i < this->m_vAnimations.size(); i++)
	{
		this->m_pTM->UnloadTexture(this->m_vAnimations[i].GetImgID());
	}

	this->m_vAnimations.clear();

	if(this->m_nTilesetID != -1)
	{
		this->m_pTM->UnloadTexture(this->m_nTilesetID);
	}
}

void AnimationManager::Update(AnimationInformation & animInfo, float fElapsedTime)
{
	animInfo.Update(fElapsedTime);
}

Animation AnimationManager::GetAnimation(int nAnimID)
{
	return this->m_vAnimations[nAnimID];
}

int AnimationManager::GetAnimID(string szName)
{
	for(unsigned int i = 0; i < this->m_vAnimations.size(); i++)
	{
		if(this->m_vAnimations[i].GetName() == szName)
		{
			return i;
		}
	}

	return -1;
}

void AnimationManager::LoadTileSet(string szFilename)
{
	string szImageFilePathAlpha = "Resource/Graphics/Tile/" + szFilename;

	const char * szImageFilePathBeta = szImageFilePathAlpha.c_str();

	TCHAR * szImageFilePathGold = new TCHAR[50];

	mbstowcs_s(nullptr, szImageFilePathGold, 50, szImageFilePathBeta, _TRUNCATE);

	this->m_nTilesetID = CSGD_TextureManager::GetInstance()->LoadTexture(szImageFilePathGold);

	delete szImageFilePathGold;
	szImageFilePathGold = nullptr;
}

void AnimationManager::RenderTile( RECT tile, int nX, int nY, int alpha )
{
	m_pTM->Draw( m_nTilesetID, nX, nY, 1.0f, 1.0f, &tile, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(alpha,255,255,255));
}