/*
	Author: Benjamin "Evan" Cowles
	File: Frame.h
*/

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
using namespace std;

class Frame
{
	bool m_bHasActiveRect;
	bool m_bHasTrigger;
	float m_fDuration;
	POINT m_pAnchor;
	RECT m_rActive;
	RECT m_rDraw;
	RECT m_rPassive;
	string m_szTrigger;

public:
	Frame(bool bHasActiveRect, RECT rActive, RECT rDraw, RECT rPassive, POINT pAnchor, float fDuration, bool bHasTrigger, string szTrigger);

	POINT GetAnchorPoint();
	RECT * GetDrawRect();
	RECT * GetActiveRect();
	RECT * GetPassiveRect();
	float GetDuration();
	bool HasTrigger();
	string GetTrigger();
	bool HasActiveRect();
};