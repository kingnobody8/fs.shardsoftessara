/*
	Author: Benjamin "Evan" Cowles
	File: AnimationInformation.h
*/

#pragma once

class AnimationInformation
{
	int m_nAnimID;
	int m_nCurFrame;
	float m_fTimeOnFrame;
	bool m_bTriggerHit;

public:
	AnimationInformation();
	AnimationInformation(int m_nAnimID, int nCurFrame = 0, float fTimeOnFrame = 0.0f);

	int GetAnimID();
	int GetCurFrame();
	void Update(float fElapsedTime);
	void Reset();
	bool IsOver();
};