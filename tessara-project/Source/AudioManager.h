/*
	File:		AudioManager.h
	Author:		Mark Diaz
	Purpose:	The CAudioManager class manages all audio
				resources
*/

#pragma once

// Menu Audio
#define MENU_AUDIO		100
#define	GAMEPLAY_AUDIO  200
#define FOREST_AUDIO	300
#define HUB_AUDIO		400

#define MUSIC_MENU		0
#define SFX_CONFIRM     1
#define SFX_BACK		2
#define SFX_CURSOR      3
#define SFX_PAUSE		4
#define SFX_POINT		5
#define SFX_PAUSEMOVE	6
#define SFX_ERROR		7

#define MUSIC_BATTLE	4
#define MUSIC_WALK		5
#define	MUSIC_CREDITS	6		


// GamePlay Audio
#define SFX_LEVELUP		50
#define SFX_POTION		51
#define SFX_SOCKET		52
#define SFX_SELL		53
#define SFX_WEAPONBUY   54
#define SFX_ARMORBUY	55
#define SFX_WIGHTAGGRO  56
#define SFX_WIGHTATTACK 57
#define SFX_WIGHTDIE	58
#define SFX_FIREBALL    59
#define SFX_DASH		60
#define SFX_LDASH		61
#define SFX_BOLT		62
#define SFX_ICE			63
#define SFX_FREEZE      64
#define SFX_ROCKSMASH   65
#define SFX_CHAKRAM		66
#define SFX_ICESLASH	67
#define SFX_EARTHSHARD  68
#define SFX_LSLASH		69
#define SFX_DEATH		70
#define SFX_SHARD		71
#define SFX_KEY			72
#define SFX_STAB		73


#include <vector>
using namespace std;

class CSGD_XAudio2;

class CAudioManager
{
public:
	// SINGLETON!
	static CAudioManager* GetInstance( void );
	static void DeleteInstance( void );

	void Initialize( void );
	void Update( void );

	// Utilities
	void PlayMusic ( int id );
	void PlaySFX   ( int id );

	// Volume Control
	void AdjustMusicVolume( float amount );
	void AdjustSoundVolume( float amount );

	//Fading in and out
	void OtherMusicVolume( float amount );
	float GetNormalVol(void) { return this->MusVol; }

	// Loaders
	void LoadAudio( int id );
	void UnloadAudio( int id );
	void LoadLevelAudio( string battle, string walk );
	void UnLoadLevelAudio( void );

	//Battle and Walk
	void PlayBattle(void);
	void PlayWalk(void);
	void PlayCredits(void);

	// Accessors
	float GetMusicVolume ( void );
	float GetSoundVolume ( void );

	//Stop
	void StopMus();

private:
	// SINGLETON!
	static CAudioManager* s_pInstance;
	// Constructor and Trilogy
	CAudioManager(void);
	~CAudioManager(void);

	CAudioManager( const CAudioManager& );
	CAudioManager& operator= ( const CAudioManager& );

	float		MusVol;

	// XAudio2
	CSGD_XAudio2* m_pXA;

	bool MenuAudioLoaded;
	bool GameplayAudioLoaded;
	bool ForestAudioLoaded;
	bool HubAudioLoaded;

	// Helper functions
	void LoadMenuAudio();
	void UnloadMenuAudio();
	void LoadGamePlayAudio();
	void UnloadGamePlayAudio();
	void PlayMenuAudio( int id );
	void PlayGamePlayAudio( int id );

	// Music and sound IDs-------------------

	// MENU
	vector<int> MenuAudio;

	// FOREST
	vector<int> ForestAudio;

	// HUB
	vector<int> HubAudio;

	// GAMEPLAY
	vector<int> GameplayAudio;

	// GAMEPLAY
	int BattleMus, WalkMus, CreditsMus;
};

