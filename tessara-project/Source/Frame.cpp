/*
	Author: Benjamin "Evan" Cowles
	File: Frame.cpp
*/

#include "Frame.h"

Frame::Frame(bool bHasActiveRect, RECT rActive, RECT rDraw, RECT rPassive, POINT pAnchor, float fDuration, bool bHasTrigger, string szTrigger)
{
	this->m_bHasActiveRect = bHasActiveRect;
	this->m_bHasTrigger = bHasTrigger;
	this->m_fDuration = fDuration;
	this->m_pAnchor = pAnchor;
	this->m_rDraw = rDraw;
	this->m_rPassive = rPassive;
	this->m_rActive = rActive;
	this->m_szTrigger = szTrigger;
}

POINT Frame::GetAnchorPoint()
{
	return this->m_pAnchor;
}

RECT * Frame::GetDrawRect()
{
	return &this->m_rDraw;
}

RECT * Frame::GetActiveRect()
{
	return &this->m_rActive;
}

RECT * Frame::GetPassiveRect()
{
	return &this->m_rPassive;
}

float Frame::GetDuration()
{
	return this->m_fDuration;
}

bool Frame::HasTrigger()
{
	return this->m_bHasTrigger;
}

string Frame::GetTrigger()
{
	return this->m_szTrigger;
}

bool Frame::HasActiveRect()
{
	return this->m_bHasActiveRect;
}