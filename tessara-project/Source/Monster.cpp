/*
Author: Jorge Juaneda
File: Monster.cpp
*/

#include "Monster.h"
#include "DeleteEntityMessage.h"
#include "MessageSystem.h"

CMonster::CMonster(void)
{
	m_fFireResist	= 1.0f;
	m_fIceResist	= 1.0f;
	m_fLightResist	= 1.0f;
	m_fEarthResist	= 1.0f;
	m_fPhysicalResist = 1.0f;
	m_nMaxHealth = 0;
	m_nLevel = 0;

	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-027
	// BUG FIX START
	/////////////////////////////////////////////////////////////////
	this->Aggroed = false;
	/////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-027
	/////////////////////////////////////////////////////////////////

}

CMonster::~CMonster(void)
{

}

/*virtual*/ void CMonster::ModifyHealth(int nAmount)
{
	m_nHealth += nAmount;
	if (m_nHealth <= 0)
	{
		CDeleteEntityMessage* pMsg = new CDeleteEntityMessage( this );
		CMessageSystem::GetInstace()->SendMsg( pMsg );
		pMsg = nullptr;
	}

}


/*virtual*/ void CMonster::Update( float deltaTime )
{

	CEntity::Update( deltaTime );
}


/*virtual*/void CMonster::Render( void )
{
	CEntity::Render();

}

/*virtual*/ bool CMonster::CheckCollision( IEntity* pOther )
{
	RECT rSelf	= GetRect();
	RECT rOther	= pOther->GetRect();
	RECT rOverlap;

	if( IntersectRect( &rOverlap, &rSelf, &rOther ) == TRUE )
	{
		if (pOther->GetType() == ENT_PROJECTILE)
			return true;

		int nOVerWidth = rOverlap.right - rOverlap.left;
		int nOverHeight = rOverlap.bottom - rOverlap.top;

		int nCenterOverX = nOVerWidth + rOverlap.left;
		int nCenterOverY = nOverHeight + rOverlap.top;

		int nCenterOtherX = (rOther.right - rOther.left) + rOther.left;
		int nCebterOtherY =  (rOther.bottom - rOther.top) + rOther.top;

		if (nOverHeight < nOVerWidth)
		{
			//move up or down
			if (nCenterOverY < nCebterOtherY)
				SetPosY( GetPosY() - nOverHeight);
			else
				SetPosY( GetPosY() + nOverHeight);

		}
		else
		{
			//move left or right
			if (nCenterOverX < nCenterOtherX)
				SetPosX( GetPosX() - nOVerWidth);
			else
				SetPosX( GetPosX() + nOVerWidth);
		}
		return true;
	}
	else
		return false;
}

/*virtual*/ RECT CMonster::GetRect ()
{
	RECT rec;
	rec.bottom = (int)GetPosY() + GetHeight();
	rec.left = (int)GetPosX();
	rec.right = (int)GetPosX() + GetWidth();
	rec.top = (int)GetPosY();

	return rec;
}