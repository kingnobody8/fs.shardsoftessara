/*
	File:		SocketingState.h
	Author:		Benjamin Cowles, Mark Diaz
	Course:		SGP 1304
	Purpose:	The CSocketingState Class allows the player to
				socket items from their inventory with gems
*/

#pragma once
#include "igamestate.h"
#include <vector>
using namespace std;

class CBitmapFont;
class CPlayer;
class CSGD_TextureManager;
class CItem;

class CSocketingState :
	public IGameState
{
public:
	//This class is a singleton
	static CSocketingState* GetInstance( void );
//--------------------------------------------------------------------------------------------------
	
	// IGameState Interface
	virtual void	Enter ( void );					// load resources
	virtual void	Exit  ( void );					// unload resources

	virtual bool	Input ( void );					// handle user input
	virtual void	Update( float fElapsedTime );	// update game entities
	virtual void	Render( void );					// render game entities

//--------------------------------------------------------------------------------------------------

private:

	CSocketingState( void );								// ctor
	virtual ~CSocketingState( void );						// dtor
	CSocketingState( const CSocketingState& );				// copy ctor
	CSocketingState& operator= ( const CSocketingState& );	// assignment op

	CBitmapFont*	m_pBF;
	CPlayer* m_pPlayer;
	CSGD_TextureManager* m_pTM;
	int m_nSelectedIndex;
	int m_nOffset;
	CItem * m_iItemToSocket;

	// Assets:
	vector<int> m_vSocketingImgIDs;
	int m_nMenuButtonImgID;

	// Help Message Assets:
	bool ShowHelpMessage;
	int m_nHelpIndex;
	int m_nHelpPanelID;
};