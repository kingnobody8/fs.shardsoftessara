/*
	File:		Key.cpp
	Author:		Benjamin "Evan" Cowles
*/

#include "Shard.h"
#include "AnimationManager.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "PlayState.h"
#include "Player.h"
#include "MessageSystem.h"
#include "AddShardMessage.h"
#include "DeleteEntityMessage.h"

CShard::CShard()
{
	AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("shard"));
	SetCurrAnim(0);
	SetType(ENT_ITEM);
	picked = false;
}

CShard::~CShard()
{

}

void CShard::Update(float deltaTime)
{
	CEntity::Update(deltaTime);
}

void CShard::Render()
{
	CEntity::Render();
}

bool CShard::CheckCollision(IEntity* pOther)
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

		if( IntersectRect( &rOverlap, &rRect, &rOther) == TRUE && !picked )
		{
			picked = false;
			PlayState::GetInstance()->GetPlayer()->AddShard();
			CAudioManager::GetInstance()->PlaySFX( SFX_SHARD );

			CDeleteEntityMessage* msg2 = new CDeleteEntityMessage(this);
			CMessageSystem::GetInstace()->SendMsg(msg2);

			//Delete it from the mapData
			LevelManager::GetInstance()->DelObj( GetPosX(), GetPosY(), "shard" );

		}
	}
	return false;
}

void CShard::Create(Area loc)
{
	this->SetPosX(loc.cols);
	this->SetPosY(loc.rows);
}

RECT CShard::GetRect()
{
	RECT rec;
	rec.bottom = (int)GetPosY() + GetHeight();
	rec.left = (int)GetPosX();
	rec.right = (int)GetPosX() + GetWidth();
	rec.top = (int)GetPosY();

	return rec;
}