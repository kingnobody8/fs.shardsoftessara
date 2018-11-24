/*
	File:		LevelUpMenu.h
	Author:		Mark Diaz
	Course:		SGP 1304
	Purpose:	The CLevelUpMenu class allows the player to
				Allocate attribute points
*/

#pragma once
#include "igamestate.h"
#include "../SGD Wrappers\SGD_String.h"

class CInputManager;
class CSGD_TextureManager;
class CBitmapFont;

class CLevelUpMenu :
	public IGameState
{
public:
	static CLevelUpMenu* GetInstance( void );

	// IGameState Interface
	virtual void	Enter ( void );					// load resources
	virtual void	Exit  ( void );					// unload resources

	virtual bool	Input ( void );					// handle user input
	virtual void	Update( float fElapsedTime );	// update options entities
	virtual void	Render( void );					// render options entities


private:
	CLevelUpMenu(void);
	virtual ~CLevelUpMenu(void);

	CLevelUpMenu( const CLevelUpMenu& );				
	CLevelUpMenu& operator= ( const CLevelUpMenu& );	

	// SGD Wrappers
	CInputManager*			m_pIM;
	CSGD_TextureManager*	m_pTM;
	CBitmapFont*			m_pBF;

	// Assets
	int				m_nBackgroundImgID;
	int				m_nMenuButtonImgID;
	int				m_nAttributeButtonImgID;
	int				m_nPanelImgID;
	int				m_nSelectedIndex;
	int				Alpha[6];
	std::string		Attributes[14];
	float			ExpBar;
	int				m_nShardImgID;


	// Artifact mode or Player mode
	bool		m_bArtifactMode;

	//Artifact values;
	int			artCount;
	int			ArtLevels[4];
	int			artIndex;
	int			artAlpha[5];

	// State Modes
	void PlayerUpdate( float fElapsedTime );
	bool PlayerInput( void );
	void PlayerRender(void );

	void ArtifactUpdate( float fElapsedTime );
	bool ArtifactInput( void );
	void ArtifactRender( void );

	// Helper functions
	void AttributesToString( void );
	void Body( void );
	void Mind( void );
	void Soul( void );
	void Char( void );

	void Fire	( void );
	void Earth	( void );
	void Ice	( void );
	void Light	( void );

	// Help Message Assets:
	bool ShowPlayerHelp;
	bool ShowArtifactHelp;
	int m_nHelpIndex;

};

