/*
	File:		LoadGameState.h
	Author:		Mark Diaz
	Course:		SGD 1302
	Purpose:	The LoadGameState Presents the user with the three
				save slots and loads whatever one they select
*/

#pragma once
#include "igamestate.h"
#include "ParticleEffect.h"

class CInputManager;
class CSGD_TextureManager;
class CBitmapFont;
class CPlayer;

class CLoadGameState :
	public IGameState
{
public:
	// Este clase esta un singleton!
	static CLoadGameState* GetInstance( void );

	// IGameState Interface
	virtual void	Enter ( void );					// load resources
	virtual void	Exit  ( void );					// unload resources

	virtual bool	Input ( void );					// handle user input
	virtual void	Update( float fElapsedTime );	// update options entities
	virtual void	Render( void );					// render options entities

	void UpdatePlayer(CPlayer& pPlayer);
	void UpdateMap(void);
	void SetNewGame(bool bNewGame) { m_bNewGame = bNewGame; }
	string GetMapName(int nIndex) { return m_vMapName[nIndex]; }
	int GetPortal(int nIndex) { return m_vPortal[nIndex]; }
	bool NewGame() { return m_bNewGame; }
	void FromPause(bool bFromPause) { m_bFromPause = bFromPause; }

private:
	CLoadGameState(void);
	virtual ~CLoadGameState(void);

	CLoadGameState( const CLoadGameState& );				
	CLoadGameState& operator= ( const CLoadGameState& );	

	// SGD Wrappers
	CInputManager*			m_pIM;
	CSGD_TextureManager*	m_pTM;
	CBitmapFont *			m_pBF;

	// Assets
	int						m_nBackgroundImgID;
	int						m_nMenuButtonImgID;
	int						m_nSelectedIndex;
	int						m_nMenuInfoImgID;
	CParticleEffect			SelectionFat;
	CParticleEffect			SelectionLarge;
	CParticleEffect			Burst;
	int						m_nAlpha[4];
	float					m_nAlphaOffset;
	bool					m_bFadingOut;
	float					m_fFadeTimer;
	bool					m_bFadingIn;
	bool					m_bNewGame;
	bool					m_bFromPause;

	vector<int> m_vActive;
	vector<int> m_vUnspent;
	vector<int> m_vCharacter;
	vector<int> m_vExperience;
	vector<int> m_vCurrency;
	vector<int> m_vLevel;
	vector<int> m_vBody;
	vector<int> m_vMind;
	vector<int> m_vSoul;
	vector<int> m_vKeys;
	vector<int> m_vHealth;
	vector<int> m_vArtifact1;
	vector<int> m_vArtifact2;
	vector<int> m_vArtifact3;
	vector<int> m_vArtifact4;
	vector<int> m_vShards;
	vector<int> m_vPotions;
	vector<int> m_vPortal;
	vector<int> m_vHelp;
	vector<vector<int>> m_vItems;
	vector<string> m_vMapName;
};