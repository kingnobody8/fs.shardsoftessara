/*
	Author: Jorge Juaneda
	File: PickUp.h
*/
#pragma once
#include <vector>
#include "Item.h"
#include "IEntity.h"
#include "..\SGD Wrappers\IListener.h"


class CPickUp : public IEntity
{
private:
	CItem* m_pItem;

public:
	CPickUp(CItem* pItem);
	~CPickUp(void);

	void	Update ( float fElapsedTime );			
	void	Render ( void );							
	
	RECT	GetRect( void ) const;					
	int		GetType( void ) const;					
	bool	CheckCollision( IEntity* pOther );

	void	AddRef ( void );
	void	Release( void );
};

