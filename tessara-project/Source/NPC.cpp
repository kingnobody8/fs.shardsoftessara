/*
File:		NPC.cpp
Author:		Daniel Habig
Course:		SGP
Purpose:	The NPC class is an entity that has text for the player to talk to
*/

#include "NPC.h"
#include "../SGD Wrappers/SGD_Math.h"
#include <Windows.h>
#include "AnimationManager.h"
#include "InputManager.h"
#include "PlayState.h"
#include <ostream>

NPC::NPC(void)
{
	SetType( ENT_NPC );
	given = false;
	stop = false;
	SetHeight(30);
	SetWidth(30);
}
NPC::~NPC(void)
{
}
void NPC::Update( float deltaTime )
{
	if (nodes.size() == 1)
		return;
	if(stop)
	{
		SetVelX(0);
		SetVelY(0);
		return;
	}

	if (m_fPathTimer > 0.0f)
	{
		//Standing still
		if( this->m_nFacing < 4 && m_nFacing >= 0)
			this->SetCurrAnim( this->m_nFacing + 4 );

		m_fPathTimer -= deltaTime;
		if (m_fPathTimer <= 0.0f)
		{
			++m_nCurrentNode;
			if (m_nCurrentNode == nodes.size())
				m_nCurrentNode = 0;

			m_fPathTimer = 0.0f;

			POINT MyPosition = { GetPosX(), GetPosY() };
			POINT NextPosition = { nodes[m_nCurrentNode].cols * 64 + 32,
				nodes[m_nCurrentNode].rows * 64 + 32};
			FindPath(MyPosition, NextPosition);
			Goal = Waypoints.top();
			Waypoints.pop();
		}
		else
			return;
	}
	else
	{
		//Moving
		if( this->m_nFacing < 4 && m_nFacing >= 0)
			this->SetCurrAnim( this->m_nFacing );
	}

	tVector2D vToWaypoint = 
	{
		(float)(Goal.x - GetPosX()),
		(float)(Goal.y - GetPosY())	};

	if (vToWaypoint.fY > 0)
		SetVelY((float)m_nSpeed);
	else if (vToWaypoint.fY < 0)
		SetVelY((float)-m_nSpeed);

	if (abs(vToWaypoint.fY) < 15)
		SetVelY((float)0);

	if (vToWaypoint.fX > 0)
		SetVelX((float)m_nSpeed);
	else if (vToWaypoint.fX < 0)
		SetVelX((float)-m_nSpeed);

	if (abs(vToWaypoint.fX) < 15)
		SetVelX((float)0);

	if (vToWaypoint.fX > 0)
		this->m_nFacing = (3);
	else if (vToWaypoint.fX < 0)
		this->m_nFacing = (2);

	if (vToWaypoint.fY > 0)
		this->m_nFacing = (1);			
	else if (vToWaypoint.fY < 0)
		this->m_nFacing = (0);

	CEntity::Update( deltaTime );

	vToWaypoint.fX  = float(Goal.x - GetPosX());
	vToWaypoint.fY	= float(Goal.y - GetPosY());

	if ( Vector2DLength( vToWaypoint ) < 50.0f )
	{
		if (!Waypoints.empty())
		{
			Goal = Waypoints.top();
			Waypoints.pop();
		}
		else
			m_fPathTimer = WAIT;
	}

}
void NPC::Render( void )
{
	CEntity::Render();
}

bool NPC::CheckCollision( IEntity* pOther )
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
			stop = true;

			if( CInputManager::GetInstance()->CheckMelee() )
			{
				switch(pOther->GetFacing())
				{
				case 0:
					{
						dynamic_cast<CPlayer *>(pOther)->SetCurrAnim(9 + 32 * dynamic_cast<CPlayer *>(pOther)->GetCurrAtr());
					}
					break;
				case 1:
					{
						dynamic_cast<CPlayer *>(pOther)->SetCurrAnim(1 + 32 * dynamic_cast<CPlayer *>(pOther)->GetCurrAtr());
					}
					break;
				case 2:
					{
						dynamic_cast<CPlayer *>(pOther)->SetCurrAnim(17 + 32 * dynamic_cast<CPlayer *>(pOther)->GetCurrAtr());
					}
					break;
				case 3:
					{
						dynamic_cast<CPlayer *>(pOther)->SetCurrAnim(25 + 32 * dynamic_cast<CPlayer *>(pOther)->GetCurrAtr());
					}
					break;
				}

				ostringstream woss;
				woss << this->name.c_str() << " - " << this->text.c_str();
				string note = woss.str().c_str();

				Trigger* trig = new Trigger();
				trig->note = note;
				PlayState::GetInstance()->MakeNote(trig->note);
				delete trig;
				trig = nullptr;

				//give player the item they carry
				if( this->given == false )
				{
					this->given = true;
					if( strcmp( this->item.c_str(), "artifact") == 0)
						PlayState::GetInstance()->GetPlayer()->AddArtifact();

					//Add other items.
					this->item;

					LevelManager::GetInstance()->Given( this->name, this->text );
				}
			}
			return true;
		}
	}


	stop = false;

	return false;
}

/*virtual*/ RECT NPC::GetRect ()
{
	RECT rec;
	rec.bottom = (int)GetPosY() + GetHeight();
	rec.left = (int)GetPosX();
	rec.right = (int)GetPosX() + GetWidth();
	rec.top = (int)GetPosY();

	return rec;
}

