/*
	Author: Jorge Juaneda
	File: Entity.h
*/
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


class IEntity
{
public:
// Pure virtual method makes this an ABC
	// and it cannot be instantiated
	virtual ~IEntity( void ) = 0
	{	}

	// IEntity Interface:
	virtual void	Update ( float fElapsedTime )	= 0;		// move the entity
	virtual void	Render ( void )					= 0;		// draw the entity
	virtual void	RenderAlpha( int nAlpha) = 0;
	
	virtual RECT	GetRect( void ) const			= 0;		// return bounding box
	virtual int		GetType( void ) const			= 0;		// return the entity's type
	virtual bool	CheckCollision( IEntity* pOther )	= 0;	// handle collision against other entity
	virtual int		GetFacing( void ) const			= 0;

	// Reference Counting:
	virtual void	AddRef ( void )	= 0;
	virtual void	Release( void ) = 0;

};

