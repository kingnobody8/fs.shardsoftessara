/*
File:		AudioManager.h
Author:		Mark Diaz
Purpose:	The CAudioManager class manages all audio
resources
*/

#include "AudioManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include <cassert>

// SINGLETON!
// - instantiate the static data memeber
/*static*/ CAudioManager* CAudioManager::s_pInstance = nullptr;

/*static*/ CAudioManager* CAudioManager::GetInstance(void)
{
	if ( s_pInstance == nullptr )
		s_pInstance = new CAudioManager;

	return s_pInstance;
}

/*static*/ void CAudioManager::DeleteInstance( void )
{
	
	delete s_pInstance;
	s_pInstance = nullptr;
}

void CAudioManager::Initialize( void )
{
	m_pXA = CSGD_XAudio2::GetInstance();
	m_pXA->InitXAudio2();

	MenuAudioLoaded			= false;
	GameplayAudioLoaded		= false;
	ForestAudioLoaded		= false;
	HubAudioLoaded			= false;

	MusVol					= 0.0f;
}
// Constructor
CAudioManager::CAudioManager(void)
{
	BattleMus = -1;
	WalkMus = -1;
	CreditsMus = -1;
}

// Destructor
CAudioManager::~CAudioManager(void)
{
	UnloadGamePlayAudio();
	UnloadMenuAudio();
	UnLoadLevelAudio();

	if( m_pXA != nullptr )
	{
		m_pXA->ShutdownXAudio2();
		m_pXA = nullptr;
	}
}

void CAudioManager::AdjustMusicVolume( float amount )
{
	if (m_pXA->MusicGetMasterVolume() + amount < 0.0f)
		m_pXA->MusicSetMasterVolume(0.0f);
	else if (m_pXA->MusicGetMasterVolume() + amount > 1.0f)
		m_pXA->MusicSetMasterVolume(1.0f);
	else
		m_pXA->MusicSetMasterVolume( m_pXA->MusicGetMasterVolume() + amount );
	MusVol = GetMusicVolume();
}

void CAudioManager::Update()
{
	m_pXA->Update();
}

void CAudioManager::OtherMusicVolume( float amount )
{
	float vol = m_pXA->MusicGetMasterVolume() + amount;

	if ( vol < 0.0f )
		m_pXA->MusicSetMasterVolume(0.0f);
	else if ( vol > 1.0f )
		m_pXA->MusicSetMasterVolume(1.0f);
	else
		m_pXA->MusicSetMasterVolume( vol );
}

void CAudioManager::AdjustSoundVolume( float amount )
{
	if (m_pXA->SFXGetMasterVolume() + amount < 0.0f)
		m_pXA->SFXSetMasterVolume(0.0f);
	else if (m_pXA->SFXGetMasterVolume() + amount > 1.0f)
		m_pXA->SFXSetMasterVolume(1.0f);
	else
		m_pXA->SFXSetMasterVolume( m_pXA->SFXGetMasterVolume() + amount );
}

float CAudioManager::GetMusicVolume( void )
{
	return m_pXA->MusicGetMasterVolume();
}

float CAudioManager::GetSoundVolume( void )
{
	return m_pXA->SFXGetMasterVolume();
}

void CAudioManager::PlayMusic( int id )
{
	if (id < 4)
		PlayMenuAudio( id );
	else if( id == MUSIC_BATTLE )
		PlayBattle();
	else if( id == MUSIC_WALK )
		PlayWalk();
	else if( id == MUSIC_CREDITS )
		PlayCredits();
}

void CAudioManager::PlaySFX  ( int id )
{
	if (id < 50)
		PlayMenuAudio( id );
	if (id >= 50 && id < 100)
		PlayGamePlayAudio( id );
}

void CAudioManager::LoadAudio( int id )
{
	switch( id )
	{
	case MENU_AUDIO:
		LoadMenuAudio();
		break;
	case GAMEPLAY_AUDIO:
		LoadGamePlayAudio();

	default:
		break;

	}
}

void CAudioManager::UnloadAudio( int id )
{
	switch( id )
	{
	case MENU_AUDIO:
		UnloadMenuAudio();
		break;
	case GAMEPLAY_AUDIO:
		UnloadGamePlayAudio();
		break;
	default:
		break;

	}
}

void CAudioManager::LoadMenuAudio()
{
	if (MenuAudioLoaded)
		return;

	// MUSIC 
	MenuAudio.push_back(m_pXA->MusicLoadSong( _T("Resource/Audio/Menu_Audio/MenuMusic.xwm") ) );
	// CONFIRM
	MenuAudio.push_back( m_pXA->SFXLoadSound( _T("Resource/Audio/Menu_Audio/Confirm.wav") ) );
	// BACK
	MenuAudio.push_back( m_pXA->SFXLoadSound( _T("Resource/Audio/Menu_Audio/Back.wav") ) );
	// CURSOR
	MenuAudio.push_back( m_pXA->SFXLoadSound( _T("Resource/Audio/Menu_Audio/Cursor.wav") ) );
	// PAUSE
	MenuAudio.push_back( m_pXA->SFXLoadSound( _T("Resource/Audio/Menu_Audio/Pause.wav") ) );
	// PAUSE SELECTED INDEX CHANGE
	MenuAudio.push_back( m_pXA->SFXLoadSound( _T("Resource/Audio/Menu_Audio/PauseMove.wav") ) );
	// ATTRIBUTE POINT ADDED
	MenuAudio.push_back( m_pXA->SFXLoadSound( _T("Resource/Audio/Menu_Audio/PointAdd.wav") ) );
	// NOT ENOUGH MONEY / ERROR
	MenuAudio.push_back( m_pXA->SFXLoadSound( _T("Resource/Audio/Menu_Audio/Error.wav") ) );

	MenuAudioLoaded = true;
}


//Load Level Audio
void CAudioManager::LoadLevelAudio( string battle, string walk )
{
	wostringstream woss;
	woss << "Resource/Audio/Level_Music/" << battle.c_str() << ".xwm";
	this->BattleMus = m_pXA->MusicLoadSong( woss.str().c_str() );
	woss.str(_T(""));
	woss << "Resource/Audio/Level_Music/" << walk.c_str() << ".xwm";
	this->WalkMus = m_pXA->MusicLoadSong( woss.str().c_str() );
	woss.str().clear();
}

void CAudioManager::LoadGamePlayAudio()
{
	if (GameplayAudioLoaded)
		return;
	//Credits Music
	this->CreditsMus = m_pXA->MusicLoadSong(_T("Resource/Audio/Level_Music/credits.xwm"));

	// Level Up Sound
	GameplayAudio.push_back(m_pXA->SFXLoadSound( _T("Resource/Audio/SFX/LevelUp.wav") ) );
	// Potion
	GameplayAudio.push_back(m_pXA->SFXLoadSound( _T("Resource/Audio/SFX/Potion.wav") ) );
	// Socket
	GameplayAudio.push_back(m_pXA->SFXLoadSound( _T("Resource/Audio/SFX/Socket.wav") ) );
	// Sell
	GameplayAudio.push_back(m_pXA->SFXLoadSound( _T("Resource/Audio/SFX/Sell.wav") ) );
	// Weapon Buy
	GameplayAudio.push_back(m_pXA->SFXLoadSound( _T("Resource/Audio/SFX/WeaponBuy.wav") ) );
	// Armor Buy
	GameplayAudio.push_back(m_pXA->SFXLoadSound( _T("Resource/Audio/SFX/ArmorBuy.wav") ) );
	// Wight Aggro
	GameplayAudio.push_back(m_pXA->SFXLoadSound( _T("Resource/Audio/SFX/WightAggro.wav") ) );
	// Wight Attack
	GameplayAudio.push_back(m_pXA->SFXLoadSound( _T("Resource/Audio/SFX/WightAttack.wav") ) );
	// Wight Die
	GameplayAudio.push_back(m_pXA->SFXLoadSound( _T("Resource/Audio/SFX/WightDie.wav") ) );
	// Fireball
	GameplayAudio.push_back(m_pXA->SFXLoadSound( _T("Resource/Audio/SFX/Fireball.wav") ) );
	// Dash
	GameplayAudio.push_back(m_pXA->SFXLoadSound( _T("Resource/Audio/SFX/Dash.wav") ) );
	// Lightning Dash
	GameplayAudio.push_back(m_pXA->SFXLoadSound( _T("Resource/Audio/SFX/Dash2.wav") ) );
	// Lightning Bolt
	GameplayAudio.push_back(m_pXA->SFXLoadSound( _T("Resource/Audio/SFX/Lightning.wav") ) );
	// Ice Bolt
	GameplayAudio.push_back(m_pXA->SFXLoadSound( _T("Resource/Audio/SFX/IceBolt.wav") ) );
	// Freeze
	GameplayAudio.push_back(m_pXA->SFXLoadSound( _T("Resource/Audio/SFX/Freeze.wav") ) );
	// Rock Smash
	GameplayAudio.push_back(m_pXA->SFXLoadSound( _T("Resource/Audio/SFX/RockSmash.wav") ) );
	// Chakram
	GameplayAudio.push_back(m_pXA->SFXLoadSound( _T("Resource/Audio/SFX/Chakram.wav") ) );
	// Ice Slash
	GameplayAudio.push_back(m_pXA->SFXLoadSound( _T("Resource/Audio/SFX/IceSlash.wav") ) );
	// Earth Shard
	GameplayAudio.push_back(m_pXA->SFXLoadSound( _T("Resource/Audio/SFX/EarthShard.wav") ) );
	// Lightning Slash
	GameplayAudio.push_back(m_pXA->SFXLoadSound( _T("Resource/Audio/SFX/LightningSlash.wav") ) );
	//	Death SFX
	GameplayAudio.push_back(m_pXA->SFXLoadSound( _T("Resource/Audio/SFX/death.wav") ) );
	//	Shard SFX
	GameplayAudio.push_back(m_pXA->SFXLoadSound( _T("Resource/Audio/SFX/Shard.wav") ) );
	//	Key SFX
	GameplayAudio.push_back(m_pXA->SFXLoadSound( _T("Resource/Audio/SFX/Key.wav") ) );
	//	Stab SFX
	GameplayAudio.push_back(m_pXA->SFXLoadSound( _T("Resource/Audio/SFX/Stab.wav") ) );

	// Loaded Son!
	GameplayAudioLoaded = true;

}

void CAudioManager::UnloadGamePlayAudio()
{
	if (!GameplayAudioLoaded)
		return;

	for (unsigned int i = 0; i < GameplayAudio.size(); ++i)
	{
		if (m_pXA->SFXIsSoundPlaying( GameplayAudio[i] ))
			m_pXA->SFXStopSound(GameplayAudio[i] );

		m_pXA->SFXUnloadSound( GameplayAudio[i] );
	}

	//Unload credits song
	if (m_pXA->MusicIsSongPlaying( this->CreditsMus ))
		m_pXA->MusicStopSong(this->CreditsMus );
	m_pXA->MusicUnloadSong( this->CreditsMus );
	this->CreditsMus = -1;


	GameplayAudio.clear();
	GameplayAudioLoaded = false;
}


//UnLoad Level Audio
void CAudioManager::UnLoadLevelAudio( void )
{
	if( BattleMus != -1 )
	{
		if( m_pXA->MusicIsSongPlaying(this->BattleMus))
			m_pXA->MusicStopSong(this->BattleMus);
		m_pXA->MusicUnloadSong(this->BattleMus);
		BattleMus = -1;
	}
	if( WalkMus != -1 )
	{
		if( m_pXA->MusicIsSongPlaying(this->WalkMus))
			m_pXA->MusicStopSong(this->WalkMus);
		m_pXA->MusicUnloadSong(this->WalkMus);
		WalkMus = -1;
	}
}

void CAudioManager::PlayBattle(void)
{
	if( BattleMus != -1 )
		m_pXA->MusicPlaySong(BattleMus, true);
}
void CAudioManager::PlayWalk(void)
{
	if( WalkMus != -1 )
		m_pXA->MusicPlaySong(WalkMus, true);
}
void CAudioManager::PlayCredits(void)
{
	if( WalkMus != -1 )
		m_pXA->MusicPlaySong(CreditsMus, false);
}

void CAudioManager::PlayMenuAudio( int id )
{
	if (!MenuAudioLoaded)
		return;
	switch ( id )
	{
	case MUSIC_MENU:
		{
			if (!m_pXA->MusicIsSongPlaying(MenuAudio[0]))
				m_pXA->MusicPlaySong(MenuAudio[0],true);
		}
		break;
	default:
		m_pXA->SFXPlaySound(MenuAudio[id]);
	}
}

void CAudioManager::PlayGamePlayAudio( int id )
{
	if (!GameplayAudioLoaded)
		return;

	m_pXA->SFXPlaySound(GameplayAudio[id - 50]);

}

void CAudioManager::StopMus()
{
	m_pXA->MusicStopSong( MenuAudio[MUSIC_MENU]);

	if( BattleMus != -1 )
		m_pXA->MusicStopSong( BattleMus );

	if( WalkMus != -1 )
		m_pXA->MusicStopSong( WalkMus );

	if( CreditsMus != -1 )
		m_pXA->MusicStopSong( CreditsMus );
}

void CAudioManager::UnloadMenuAudio()
{
	if (!MenuAudioLoaded)
		return;

	if (m_pXA->MusicIsSongPlaying(MenuAudio[0]))
		m_pXA->MusicStopSong( MenuAudio[0] );

	m_pXA->MusicUnloadSong( MenuAudio[0] );

	for (unsigned int i = 1; i < MenuAudio.size(); ++i)
	{
		if (m_pXA->SFXIsSoundPlaying( MenuAudio[i] ))
			m_pXA->SFXStopSound(MenuAudio[i] );

		m_pXA->SFXUnloadSound( MenuAudio[i] );
	}

	MenuAudio.clear();
	MenuAudioLoaded = false;
}

