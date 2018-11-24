
/*
	Author: Jorge Juaneda
	File: PickUp.cpp
*/

#include "PickUp.h"
#include "EntityManager.h"
#include "../SGD Wrappers/CSGD_EventSystem.h"
#include "ItemManager.h"



CPickUp::CPickUp(CItem* pItem)
{
}


CPickUp::~CPickUp(void)
{


}

void	CPickUp::Update ( float fElapsedTime )
{


}		
void	CPickUp::Render ( void )
{


}							

RECT	CPickUp::GetRect( void ) const
{
	RECT r;
	
	r.top = -1;
	r.bottom = -1;
	r.left = -1;
	r.right = -1;

	return r;
}	

int		CPickUp::GetType( void ) const
{

	return 1;
}	

bool	CPickUp::CheckCollision( IEntity* pOther )
{

	return true;
}

	void	CPickUp::AddRef ( void )
	{

	}

	void	CPickUp::Release( void )
	{

	}