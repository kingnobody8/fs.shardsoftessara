/*
	File:		Key.cpp
	Author:		Benjamin "Evan" Cowles
*/

#include "Key.h"
#include "AnimationManager.h"
#include "InputManager.h"
#include "Player.h"
#include "MessageSystem.h"
#include "AddKeyMessage.h"
#include "DeleteEntityMessage.h"

CKey::CKey()
{
	AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("key"));
	SetCurrAnim(0);
	SetType(ENT_ITEM);
	picked = false;
}

CKey::~CKey()
{

}

void CKey::Update(float deltaTime)
{
	CEntity::Update(deltaTime);
}

void CKey::Render()
{
	CEntity::Render();
}

bool CKey::CheckCollision(IEntity* pOther)
{
	RECT rSelf	= GetRect();
	RECT rOther	= pOther->GetRect();
	RECT rOverlap;

	if( pOther->GetType() == ENT_PLAYER)
	{
		Frame tempFrame = AnimationManager::GetInstance()->GetAnimation(this->GetAnimationInformation(this->GetCurrAnim())->GetAnimID()).GetFrame(this->GetAnimationInformation(this->GetCurrAnim())->GetCurFrame());
		POINT pAnchor = tempFrame.GetAnchorPoint();
		RECT rPassive  = *tempFrame.GetPassiveRect();
		RECT rRect;

		RECT rOther = pOther->GetRect();

		int nHeight = rPassive.bottom - rPassive.top;
		int nWidth = rPassive.right - rPassive.left;
		int nX = (int)GetPosX();
		int nY = (int)GetPosY();

		rRect.bottom = nY + (rPassive.bottom - pAnchor.y);
		rRect.top = rRect.bottom - nHeight;
		rRect.right = nX + (rPassive.right - pAnchor.x);
		rRect.left = rRect.right - nWidth; 

		if( IntersectRect( &rOverlap, &rRect, &rOther) == TRUE && !picked)
		{
			AddKeyMessage* msg = new AddKeyMessage();
			CMessageSystem::GetInstace()->SendMsg(msg);
			picked = true;

			CDeleteEntityMessage* msg2 = new CDeleteEntityMessage(this);
			CMessageSystem::GetInstace()->SendMsg(msg2);

			//Delete it from the mapData
			LevelManager::GetInstance()->DelObj( GetPosX(), GetPosY(), "key" );

		}
	}
	return false;
}

void CKey::Create(Area loc)
{
	this->SetPosX(loc.cols);
	this->SetPosY(loc.rows);
}

RECT CKey::GetRect()
{
	RECT rec;
	rec.bottom = (int)GetPosY() + GetHeight();
	rec.left = (int)GetPosX();
	rec.right = (int)GetPosX() + GetWidth();
	rec.top = (int)GetPosY();

	return rec;
}