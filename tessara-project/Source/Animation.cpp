/*
	Author: Benjamin "Evan" Cowles
	File: Animation.cpp
*/

#include "Animation.h"

Animation::Animation()
{

}

Animation::Animation(bool bIsLooping, int nImgID, string szName, vector<Frame>& vFrames, int nFrameCount, float fSpeed)
{
	this->m_bIsLooping = bIsLooping;
	this->m_fSpeed = fSpeed;
	this->m_nFrameCount = nFrameCount;
	this->m_nImgID = nImgID;
	this->m_szName = szName;
	this->m_vFrames = vFrames;
}

void Animation::Update()
{

}

Frame Animation::GetFrame(int nFrame)
{
	return this->m_vFrames[nFrame];
}

bool Animation::IsLooping()
{
	return this->m_bIsLooping;
}

int Animation::GetFrameCount()
{
	return this->m_nFrameCount;
}

int Animation::GetImgID()
{
	return this->m_nImgID;
}

string Animation::GetName()
{
	return this->m_szName;
}