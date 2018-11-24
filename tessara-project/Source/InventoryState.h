/*
	File:		InventoryState.h
	Author:		Benjamin Cowles, Mark Diaz
	Course:		SGP 1304
	Purpose:	The CInventoryState Class allows the player to
				Interact with their inventory
*/

#pragma once
#include "igamestate.h"
#include <vector>
using namespace std;

class CBitmapFont;
class CPlayer;
class CSGD_TextureManager;

class CInventoryState :
	public IGameState
{
public:
	//This class is a singleton
	static CInventoryState* GetInstance( void );
//--------------------------------------------------------------------------------------------------
	
	// IGameState Interface
	virtual void	Enter ( void );					// load resources
	virtual void	Exit  ( void );					// unload resources

	virtual bool	Input ( void );					// handle user input
	virtual void	Update( float fElapsedTime );	// update game entities
	virtual void	Render( void );					// render game entities

//--------------------------------------------------------------------------------------------------

private:

	CInventoryState( void );								// ctor
	virtual ~CInventoryState( void );						// dtor
	CInventoryState( const CInventoryState& );				// copy ctor
	CInventoryState& operator= ( const CInventoryState& );	// assignment op

	CBitmapFont*	m_pBF;
	CPlayer* m_pPlayer;
	CSGD_TextureManager* m_pTM;
	int m_nSelectedIndex;
	int m_nOffset;

	// Assets:
	vector<int> m_vInventoryIDs;
	int m_nMenuButtonImgID;
	int m_nPanelImgID;

	// Help Message Assets:
	bool ShowHelpMessage;
	int m_nHelpIndex;
	int m_nHelpPanelID;
};

