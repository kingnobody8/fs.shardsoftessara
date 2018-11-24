/*
	Author: Benjamin "Evan" Cowles
	File: Animation.h
*/

#pragma once
#include "Frame.h"
#include <vector>
using namespace std;

class Animation
{
	string m_szName;
	bool m_bIsLooping;
	float m_fSpeed;
	int m_nImgID;
	int m_nFrameCount;
	vector<Frame> m_vFrames;

public:
	Animation();
	Animation(bool bIsLooping, int nImgID, string szName, vector<Frame>& vFrames, int nFrameCount, float fSpeed = 1.0f);

	void Update();
	Frame GetFrame(int nFrame);
	int GetFrameCount();
	bool IsLooping();
	int GetImgID();
	string GetName();
};