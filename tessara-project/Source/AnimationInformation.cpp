/*
	Author: Benjamin "Evan" Cowles
	File: AnimationInformation.cpp
*/

#include "AnimationInformation.h"
#include "AnimationManager.h"
#include "../SGD Wrappers/CSGD_EventSystem.h"

AnimationInformation::AnimationInformation()
{

}

AnimationInformation::AnimationInformation(int m_nAnimID, int nCurFrame, float fTimeOnFrame)
{
	this->m_fTimeOnFrame = fTimeOnFrame;
	this->m_nAnimID = m_nAnimID;
	this->m_nCurFrame = nCurFrame;
}

int AnimationInformation::GetAnimID()
{
	return this->m_nAnimID;
}

int AnimationInformation::GetCurFrame()
{
	return this->m_nCurFrame;
}

void AnimationInformation::Update(float fElapsedTime)
{
	if(this->m_bTriggerHit == false && AnimationManager::GetInstance()->GetAnimation(this->m_nAnimID).GetFrame(this->m_nCurFrame).HasTrigger())
	{
		this->m_bTriggerHit = true;
		string szTrigger = AnimationManager::GetInstance()->GetAnimation(this->m_nAnimID).GetFrame(this->m_nCurFrame).GetTrigger();

		CSGD_EventSystem::GetInstance()->SendEventNow(szTrigger);
	}

	if(this->m_fTimeOnFrame != -1.0f)
	{
		this->m_fTimeOnFrame += fElapsedTime;

		if(this->m_fTimeOnFrame * 500 >= AnimationManager::GetInstance()->GetAnimation(this->m_nAnimID).GetFrame(this->m_nCurFrame).GetDuration())
		{
			this->m_fTimeOnFrame = 0.0f;
			this->m_nCurFrame++;
			this->m_bTriggerHit = false;

			if(this->m_nCurFrame >= AnimationManager::GetInstance()->GetAnimation(this->m_nAnimID).GetFrameCount())
			{
				if(AnimationManager::GetInstance()->GetAnimation(this->m_nAnimID).IsLooping())
				{
					this->m_nCurFrame = 0;
				}
				else
				{
					this->m_fTimeOnFrame = -1.0f;
					this->m_nCurFrame--;
				}
			}
		}
	}
}

void AnimationInformation::Reset()
{
	this->m_nCurFrame = 0;
	this->m_fTimeOnFrame = 0.0f;
}

bool AnimationInformation::IsOver()
{
	return (this->m_fTimeOnFrame == -1.0f)? true : false;
}