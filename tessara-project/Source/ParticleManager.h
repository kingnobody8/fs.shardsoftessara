/*
	File:		ParticleManager.h
	Author:		Mark Diaz
	Purpose:	The CParticleManager class enables creation, updating and
				rendering of particle effects
*/

#pragma once

#define FX_FIREDASH			0
#define FX_LIGHTNINGDASH	1
#define FX_RAIN				2
#define FX_SNOW				3
#define FX_TILEFIRE			4
#define FX_TILEICE			5
#define FX_ROCKBREAK		6
#define FX_FIRETRANSITION	7
#define FX_ICESLASH			8
#define FX_ROCKSHIELD		9
#define FX_SMASHEFFECT		10
#define FX_LSLASH			11
#define FX_LEVELUP			12
#define FX_FREEZEHEAL		13
#define FX_FIREBALL			14
#define FX_ICEBALL			15
#define FX_LIGHTNINGBOLT	16
#define FX_EARTHSHARD		17
#define FX_ZOMBIEBLOOD		18

#include <vector>
#include "ParticleEffect.h"

using namespace std;

class CParticleEffect;

class CParticleManager
{
public:
	// SINGLETON!
	static CParticleManager* GetInstance( void );
	static void DeleteInstance( void );

	void Update( float deltaTime );
	void Render( void );
	void LoadParticleEffects( void );
	void UnloadParticleEffects( void );

	void GetEffect( CParticleEffect& Destination, int EffectID );
	
	void CreateEffect( int effectID );

private:
	// SINGLETON!
	static CParticleManager* s_pInstance;
	// Constructor and Trilogy
	CParticleManager(void);
	~CParticleManager(void);

	CParticleManager( const CParticleManager& );
	CParticleManager& operator= ( const CParticleManager& );

	// Particle Effects
	void LoadEffectFromFile( const char* szFilename );
	vector< CParticleEffect* > ActiveEffects;

	void LoadEffects();

};

