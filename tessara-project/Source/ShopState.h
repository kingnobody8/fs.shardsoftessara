/*
	File:		ShopState.h
	Author:		Benjamin Cowles, Mark Diaz
	Course:		SGP 1304
*/

#pragma once
#include "igamestate.h"
#include "Item.h"
#include <vector>
using namespace std;

class CBitmapFont;
class CSGD_TextureManager;

class CShopState :
	public IGameState
{
public:
	//This class is a singleton
	static CShopState* GetInstance( void );
//--------------------------------------------------------------------------------------------------
	
	// IGameState Interface
	virtual void	Enter ( void );					// load resources
	virtual void	Exit  ( void );					// unload resources

	virtual bool	Input ( void );					// handle user input
	virtual void	Update( float fElapsedTime );	// update game entities
	virtual void	Render( void );					// render game entities

//--------------------------------------------------------------------------------------------------

private:

	CShopState( void );								// ctor
	virtual ~CShopState( void );						// dtor
	CShopState( const CShopState& );				// copy ctor
	CShopState& operator= ( const CShopState& );	// assignment op

	void BuyInput();
	void SellInput();
	void SelectionInput();
	void BuyRender();
	void SellRender();
	void AskForItems();

	CBitmapFont*	m_pBF;
	CSGD_TextureManager* m_pTM;
	vector<CItem *> m_vItems;
	int m_nSellScreenImgID;
	int m_nSelectedIndex;
	int m_nOffset;
	int m_nSubstate;

	// Assets:
	vector<int> m_vShopImgIDs;
	int m_nMenuButtonImgID;	
};