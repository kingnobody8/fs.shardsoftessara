/*
	File:		PlayState.h
	Author:		Daniel Habig
	Course:		SGP
	Purpose:	The Play State class is the game state for normal gameplay.
*/

#pragma once
#include "IGameState.h"
#include "Level.h"

//#include "Anime.h"
#include <string>
using namespace std;

class CEventSystem;
class CMessageSystem;
class IMessage;
class LevelManager;
class CInputManager;
class CEntityManager;
class CWight;

#include "Player.h"
#include "Door.h"
#include "Key.h"
#include "Shard.h"

/*
class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_XAudio2;

class CEntity;
class CEntityManager;
class Level;
class Player;
class FontBMP;
*/

enum playstatestate { PLAY, PAUSE, NOTE, DEATH, END, TRANS };
enum DEATHPOINT { CHECK, TOWN, RELOAD, EXIT };
enum AudioState { FADEIN, FADEOUT, REGULAR };

class CParticleEffect;

class PlayState : public IGameState
{
public:
	// SINGLETON!
	static PlayState*	GetInstance( void );
//--------------------------------------------------------------------------------------------------

	// IGameState Interface
	virtual void	Enter ( void );					// load resources
	virtual void	Exit  ( void );					// unload resources

	virtual bool	Input ( void );					// handle user input
	virtual void	Update( float fElapsedTime );	// update game entities
	virtual void	Render( void );					// render game entities
	bool			IsPaused( void )	{ return m_bPaused; }
	void			GoToDeath(void);
	void			GoToEnd(void);
	void			GoToTrans(void);
	
	// Message Procedure
	static void MessageProc( IMessage* pMsg );

	// Pause
	void TogglePaused( void )	{ m_bPaused = !m_bPaused; }
	void QuitToMenu( void )     { m_bQuitToMenu = true; }
	void LevelUp( void )		{ m_bLevelUp = true;	}

	void CreatePlayer( int nPosX, int nPosY);
	void MovePlayer( int nPosX, int nPosY);

	CKey* CreateKey(int nPosX, int nPosY);
	CShard* CreateShard(int nPosX, int nPosY);
	Door* CreateDoor(int nPosX, int nPoxY, int type, vector<Area> nodes, bool back);
	void  CreatePower(int nPosX, int nPosY, Door *door);

	//Create enemies
	void CreateWight( int nPosX, int nPosY, int lev);
	void CreateArcher(int nPosX, int nPosY, int lev);
	void CreateZombie( int nPosX, int nPosY, int lev);
	void CreateWraight( int nPosX, int nPosY, int lev);
	void CreateBoss(int nPosX, int nPosY, int lev);

	//Set Weather
	void Weather_Rain( void );
	void Weather_Sandstorm( void );
	void Weather_Snow( void );
	void Weather_None( void );

	//Create Talking Entity
	void CreateEnt( vector<Area> nNodes, string name, string text, string item);

	CPlayer* GetPlayer()	{ return m_pPlayer; }
	void UsedSaveSlot();
	void SetMapName(string szMapName)	{ m_szMapName = szMapName; }
	void SetPortal(int val) { this->m_nPortal = val; }
	void PauseState() { currState = PAUSE; } 
	void HelpMessage( void );

	//Notification
	void MakeNote(string note);
	//Go back to main menu
	void GoBackToMenu(void);


//--------------------------------------------------------------------------------------------------
private:

	// SINGLETON!
	PlayState( void );							// ctor
	virtual ~PlayState( void );					// dtor
	PlayState( const PlayState& );				// copy ctor
	PlayState& operator= ( const PlayState& );	// assignment op

	CMessageSystem*		m_pMS;
	LevelManager*		m_pLM;
	CInputManager*		m_pIM;
	CEventSystem*		m_pES;
	CEntityManager*		m_pEM;

	CPlayer*			m_pPlayer;
	//CWight*				m_pEnemyW;

	bool				PauseInput( void );
	void				PauseUpdate( float deltaTime );
	void				PauseRender( void );
	void				SavePlayer( void );
	void				LoadingRender( void );
	void				LoadingUpdate( void );

	playstatestate		currState;

	// Weather
	CParticleEffect*	WeatherEffect;
	//Pause Menu 
	int					m_nBoxImgID;
	int					m_nMenuButtonID;
	int					Alpha[7];
	int					m_nSelectedIndex;
	CBitmapFont*		m_pBF;
	bool				m_bPaused;
	bool				m_bQuitToMenu;
	bool				m_bUsedSaveSlot;
	bool				m_bLevelUp;
	bool				m_bGainedAnItem;

	//Notification
	int					noteImgID;
	bool				NoteInput( void );
	void				NoteUpdate( float deltaTime );
	void				NoteRender( void );
	string				note;
	unsigned int		char_num;

	//Death
	int					deathBoxImgID;
	int					deathSelect;
	bool DeathInput( void );
	void DeathUpdate( float deltaTime );
	void DeathRender( void );

	//End
	float				endAlpha;
	bool				endBool;
	bool				EndInput(void);
	void				EndUpdate(float deltaTime);
	void				EndRender( void );

	float				transTimer;
	Portal				transPort;
	bool				transLoaded;
	bool				TransInput(void);
	void				TransUpdate(float deltaTime);
	void				TransRender(void);

	// Help Message Assets:
	void HelpMessageRender( void );
	bool HelpMessageInput( void );
	bool ShowHelpMessage;
	int m_nHelpIndex;
	int m_nHelpPanelID;


	// Loading
	bool m_bLoading;
	int m_nLoadingBackgroundImgID;
	int m_nLoadingBarImgID;
	int m_nLoaded;
	int m_nLoadingIndex;

	bool	exit;

	void				LoadLevel(Portal port);

	//Audio Updates (fade in, fade out, change)
	bool				agro;
	AudioState			aState;
	float				volume;
	float				fadeRate;
	void				AudioUpdate(float fElapsedTime);

	// Saving
	string m_szMapName;
	int m_nPortal;

};