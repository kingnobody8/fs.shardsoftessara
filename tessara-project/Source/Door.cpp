/*
File:		Door.cpp
Author:		Daniel Habig
Course:		SGP
Purpose:	The door class is a non-moving entity that acts as a door would
*/

#include "Door.h"
#include "AnimationManager.h"
#include "InputManager.h"
#include "PlayState.h"
#include "Player.h"
#include "RemoveKeyMessage.h"
#include "MessageSystem.h"
#include "LevelManager.h"
#include <ostream>

Door::Door(void)
{

	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-011
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	// Changed closed door's active rect
	// Note: Not in code anywhere, just in .XMLs

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-011
	//////////////////////////////////////////////////////////////////////////

	AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("doorGrassClosed"));				//0
	AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("doorGrassOpen"));				//1
	AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("doorGrassTransition"));			//2
	AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("doorGrassTransitionBackwards"));//3

	SetCurrAnim(0);

	SetType( ENT_DOOR );
	state = CLOSED;
	time = 0.0f;
	dType = KEY;
}

Door::~Door(void)
{
}

void Door::Update( float deltaTime )
{
	CEntity::Update(deltaTime);

	switch( dType )
	{
	case KEY:
		UpdateKey(deltaTime); break;
	case LAMP:
		UpdateLamp(deltaTime); break;
	case POWER:
		UpdatePower(deltaTime); break;
	}

	if( m_vAnimInfos[m_nCurrAnim]->IsOver() && state == OPENING )
	{
		state = OPEN;
		GetAnimationInformation(GetCurrAnim())->Reset();
		SetOpen();
	}
	if( m_vAnimInfos[m_nCurrAnim]->IsOver() && state == CLOSING )
	{
		state = CLOSED;
		GetAnimationInformation(GetCurrAnim())->Reset();
		SetClosed();
	}
}
void Door::UpdateKey(float deltaTime)
{
}
void Door::UpdateLamp(float deltaTime)
{
	//Check if the lamps are lit
	LevelManager* lm = LevelManager::GetInstance();

	int COLS = lm->GetLevel()->GetColumns();
	int ROWS = lm->GetLevel()->GetRows();

	bool good = true;
	int count = this->nodes.size();
	for( int i = 0; i < count; i++)
	{
		string name =  lm->GetLevel()->GetMap()[ nodes[i].rows* COLS + nodes[i].cols ]->tile->name;
		bool burn = (lm->GetLevel()->GetMap()[ nodes[i].rows* COLS + nodes[i].cols ]->state == BURN);
		bool iced = (lm->GetLevel()->GetMap()[ nodes[i].rows* COLS + nodes[i].cols ]->state == ICED);
		if( (strcmp( name.c_str(), "lamp small burn") != 0 && !burn) || iced )
			good = false;
	}

	if( good )	//OPEN
		Activate();
	else		// CLOSE
		Deactivate();

}
void Door::UpdatePower(float deltaTime)
{
	//Check if the power ent is on or off


}

void Door::Render( void )
{
	CEntity::Render();
}

bool Door::CheckCollision( IEntity* pOther )
{
	if( state != OPEN )
	{
		switch( dType )
		{
		case KEY:
			//check if the player has the key
			return CollisionKey(pOther);
		case LAMP:
			return CollisionLamp(pOther);
		case POWER:
			return CollisionPower(pOther);
		}
	}
	return false;
}
bool Door::CollisionKey(IEntity* pOther)
{
	//Need to check player for the key.

	RECT rSelf	= GetRect();
	RECT rOther	= pOther->GetRect();
	RECT rOverlap;

	if( pOther->GetType() == ENT_PLAYER)
	{
		Frame tempFrame = AnimationManager::GetInstance()->GetAnimation(this->GetAnimationInformation(this->GetCurrAnim())->GetAnimID()).GetFrame(this->GetAnimationInformation(this->GetCurrAnim())->GetCurFrame());
		POINT pAnchor = tempFrame.GetAnchorPoint();
		RECT rActiv  = *tempFrame.GetActiveRect();
		RECT rRect;

		RECT rOther = pOther->GetRect();

		int nHeight = rActiv.bottom - rActiv.top;
		int nWidth = rActiv.right - rActiv.left;
		int nX = (int)GetPosX();
		int nY = (int)GetPosY();

		rRect.bottom = nY + (rActiv.bottom - pAnchor.y);
		rRect.top = rRect.bottom - nHeight;
		rRect.right = nX + (rActiv.right - pAnchor.x);
		rRect.left = rRect.right - nWidth; 

		if( IntersectRect( &rOverlap, &rRect, &rOther) == TRUE )
		{
			CPlayer* play = dynamic_cast<CPlayer*>(pOther);
			if( play->GetKeys() > 0 )	//Does the player have a key
			{
				//open door
				//get rid of a key
				RemoveKeyMessage* ptr = new RemoveKeyMessage();
				CMessageSystem::GetInstace()->SendMsg( ptr );
				ptr = nullptr;
				Activate();
			}


			if( CInputManager::GetInstance()->CheckMelee() )
			{
				string note = "You need to find a key to go through this door.";

				Trigger* trig = new Trigger();
				trig->note = note;
				PlayState::GetInstance()->MakeNote(trig->note);
				delete trig;
				trig = nullptr;
			}
		}
	}

	if( IntersectRect( &rOverlap, &rSelf, &rOther ) == TRUE )
		return true;
	else
		return false;
}
bool Door::CollisionLamp(IEntity* pOther)
{
	RECT rSelf	= GetRect();
	RECT rOther	= pOther->GetRect();
	RECT rOverlap;

	if( pOther->GetType() == ENT_PLAYER)
	{
		Frame tempFrame = AnimationManager::GetInstance()->GetAnimation(this->GetAnimationInformation(this->GetCurrAnim())->GetAnimID()).GetFrame(this->GetAnimationInformation(this->GetCurrAnim())->GetCurFrame());
		POINT pAnchor = tempFrame.GetAnchorPoint();
		RECT rActiv  = *tempFrame.GetActiveRect();
		RECT rRect;

		RECT rOther = pOther->GetRect();

		int nHeight = rActiv.bottom - rActiv.top;
		int nWidth = rActiv.right - rActiv.left;
		int nX = (int)GetPosX();
		int nY = (int)GetPosY();

		rRect.bottom = nY + (rActiv.bottom - pAnchor.y);
		rRect.top = rRect.bottom - nHeight;
		rRect.right = nX + (rActiv.right - pAnchor.x);
		rRect.left = rRect.right - nWidth; 

		if( IntersectRect( &rOverlap, &rRect, &rOther) == TRUE )
		{
			if( CInputManager::GetInstance()->CheckMelee() )
			{
				string note = "You need to light all of the lamps in this area to go through this door.";

				Trigger* trig = new Trigger();
				trig->note = note;
				PlayState::GetInstance()->MakeNote(trig->note);
				delete trig;
				trig = nullptr;
			}
		}
	}

	if( IntersectRect( &rOverlap, &rSelf, &rOther ) == TRUE )
		return true;
	else
		return false;
}
bool Door::CollisionPower(IEntity* pOther)
{
	RECT rSelf	= GetRect();
	RECT rOther	= pOther->GetRect();
	RECT rOverlap;

	if( pOther->GetType() == ENT_PLAYER)
	{
		Frame tempFrame = AnimationManager::GetInstance()->GetAnimation(this->GetAnimationInformation(this->GetCurrAnim())->GetAnimID()).GetFrame(this->GetAnimationInformation(this->GetCurrAnim())->GetCurFrame());
		POINT pAnchor = tempFrame.GetAnchorPoint();
		RECT rActiv  = *tempFrame.GetActiveRect();
		RECT rRect;

		RECT rOther = pOther->GetRect();

		int nHeight = rActiv.bottom - rActiv.top;
		int nWidth = rActiv.right - rActiv.left;
		int nX = (int)GetPosX();
		int nY = (int)GetPosY();

		rRect.bottom = nY + (rActiv.bottom - pAnchor.y);
		rRect.top = rRect.bottom - nHeight;
		rRect.right = nX + (rActiv.right - pAnchor.x);
		rRect.left = rRect.right - nWidth; 

		if( IntersectRect( &rOverlap, &rRect, &rOther) == TRUE )
		{
			if( CInputManager::GetInstance()->CheckMelee() )
			{
				string note = "You need to find an object with a lightning symbol on it to go through this door. Make sure to be quick about it. The power won't last forever";

				Trigger* trig = new Trigger();
				trig->note = note;
				PlayState::GetInstance()->MakeNote(trig->note);
				delete trig;
				trig = nullptr;
			}
		}
	}

	if( IntersectRect( &rOverlap, &rSelf, &rOther ) == TRUE )
		return true;
	else
		return false;
}

/*virtual*/ RECT Door::GetRect ()
{
	RECT rec;
	rec.bottom = (int)GetPosY() + GetHeight();
	rec.left = (int)GetPosX();
	rec.right = (int)GetPosX() + GetWidth();
	rec.top = (int)GetPosY();

	return rec;
}

void Door::Create( int ty, Area loc, vector<Area> nodes )
{
	this->dType = (DOORTYPE)ty;
	this->SetPosX(loc.cols);
	this->SetPosY(loc.rows);
	this->nodes.clear();

	for(unsigned int i = 0; i < nodes.size(); i++)
	{
		this->nodes.push_back( nodes[i]);
	}
}

void Door::Activate()
{
	switch(state)
	{
	case OPEN:
		break;
	case OPENING:
		break;
	case CLOSED:
		state = OPENING;
		SetOpening();
		break;
	case CLOSING:
		state = OPENING;
		SetOpening();
		break;
	}

	if( this->dType == KEY )
	{
		//Set the door to open in the mapData
		LevelManager::GetInstance()->Open( GetPosX(), GetPosY() );
	}
}
void Door::Deactivate()
{
	switch(state)
	{
	case OPEN:
		state = CLOSING;
		SetClosing();
		break;
	case OPENING:
		state = CLOSING;
		SetClosing();
		break;
	case CLOSED:
		break;
	case CLOSING:
		break;
	}
}

void Door::SetOpen(void)
{
	SetCurrAnim( 1 );
}
void Door::SetClosed(void)
{
	SetCurrAnim( 0 );
}
void Door::SetOpening(void)
{
	SetCurrAnim( 3 );
}
void Door::SetClosing(void)
{
	SetCurrAnim( 2 );
}
