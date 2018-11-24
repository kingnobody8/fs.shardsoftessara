/*
	File:		IGameState.h
	Author:		Daniel Habig
	Course:		SGP
	Purpose:	The IGameState interface class is the base class for game states.
*/

#pragma once

class IGameState
{
public:

	// Abstract Base Class
	virtual ~IGameState( void )	= 0
	{	/* do nothing */	}


	// IGameState Interface
	virtual void	Enter ( void )				 = 0;	// load resources
	virtual void	Exit  ( void )				 = 0;	// unload resources

	virtual bool	Input ( void )				 = 0;	// handle user input
	virtual void	Update( float fElapsedTime ) = 0;	// update game entities
	virtual void	Render( void )				 = 0;	// render game entities

};

