/*
	Author: Jorge Juaneda
	File: Entity.cpp
*/

#include "IEntity.h"
#include "..\\SGD Wrappers\CSGD_Direct3D.h"

IEntity::IEntity(void)
{
	m_nHeight = m_nWidth = m_unRefCount = 0;
}


IEntity::IEntity(int nHeight, int nWidth, int nX, int nY, EntityType nID)
{
	m_nHeight = nHeight;
	m_nWidth = nWidth;
	m_eType = nID;
	m_nPositionX = nX;
	m_nPositionY = nY;
	m_unRefCount = 0;
}


IEntity::~IEntity(void)
{
}


void IEntity::Render()
{
	CSGD_Direct3D::GetInstance()->DrawRect(GetRect(), 0, 255, 0);
}

void IEntity::Update( float deltaTime )
{

}

bool IEntity::CheckCollision(IEntity* pOther)
{
	RECT rSelf	= GetRect();
	RECT rOther	= pOther->GetRect();
	RECT rOverlap;

	if( IntersectRect( &rOverlap, &rSelf, &rOther ) == TRUE )
		return true;
	else
		return false;
}


void	IEntity::AddRef ( void )
{
	m_unRefCount++;
}

void	IEntity::Release( void )
{
	m_unRefCount--;

	if (m_unRefCount <= 0 )
	{
		delete this;
	}
}


/*virtual*/RECT IEntity::GetRect ()
{
	RECT rec;
	rec.bottom = GetPosY() + GetHeight();
	rec.left = GetPosX();
	rec.right = GetPosX() + GetWidth();
	rec.top = GetPosY();


	return rec;
}


