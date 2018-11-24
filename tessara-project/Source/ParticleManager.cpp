/*
	File:		ParticleManager.cpp
	Author:		Mark Diaz
	Purpose:	The CParticleManager class enables creation, updating and
				rendering of particle effects
*/

#include "ParticleManager.h"
#include "ParticleEffect.h"


// SINGLETON!
// - instantiate the static data memeber
/*static*/ CParticleManager* CParticleManager::s_pInstance = nullptr;

/*static*/ CParticleManager* CParticleManager::GetInstance(void)
{
	if ( s_pInstance == nullptr )
		s_pInstance = new CParticleManager;

	return s_pInstance;
}

/*static*/ void CParticleManager::DeleteInstance( void )
{
	delete s_pInstance;
	s_pInstance = nullptr;
}

CParticleManager::CParticleManager(void)
{
}

CParticleManager::~CParticleManager(void)
{
}

void CParticleManager::Update( float deltaTime )
{
}

void CParticleManager::Render(void)
{
}

void CParticleManager::LoadEffectFromFile( const char* szFilename )
{
}

void CParticleManager::CreateEffect( int effectID )
{
}
void CParticleManager::GetEffect( CParticleEffect& Destination, int EffectID )
{
	Destination = *(ActiveEffects[EffectID]);
}

void CParticleManager::LoadParticleEffects( void )
{
	if (ActiveEffects.size() > 0)
		return;

	ActiveEffects.push_back( new CParticleEffect );
	ActiveEffects[0]->LoadFromFile("Resource/Graphics/Particles/Dash.xml" );
	ActiveEffects.push_back( new CParticleEffect );
	ActiveEffects[1]->LoadFromFile("Resource/Graphics/Particles/Dash2.xml");
	ActiveEffects.push_back( new CParticleEffect );
	ActiveEffects[2]->LoadFromFile("Resource/Graphics/Particles/Rain.xml");
	ActiveEffects.push_back( new CParticleEffect );
	ActiveEffects[3]->LoadFromFile("Resource/Graphics/Particles/Snow.xml");
	ActiveEffects.push_back( new CParticleEffect );
	ActiveEffects[4]->LoadFromFile("Resource/Graphics/Particles/Tile_Fire.xml");
	ActiveEffects.push_back( new CParticleEffect );
	ActiveEffects[5]->LoadFromFile("Resource/Graphics/Particles/Tile_Ice.xml");
	ActiveEffects.push_back( new CParticleEffect );
	ActiveEffects[6]->LoadFromFile("Resource/Graphics/Particles/RockBreak.xml");
	ActiveEffects.push_back( new CParticleEffect );
	ActiveEffects[7]->LoadFromFile("Resource/Graphics/Particles/FireTransition.xml");
	ActiveEffects.push_back( new CParticleEffect );
	ActiveEffects[8]->LoadFromFile("Resource/Graphics/Particles/IceSlash.xml" );
	ActiveEffects.push_back( new CParticleEffect );
	ActiveEffects[9]->LoadFromFile("Resource/Graphics/Particles/EarthShield.xml" );
	ActiveEffects.push_back( new CParticleEffect );
	ActiveEffects[10]->LoadFromFile("Resource/Graphics/Particles/EarthSmash.xml" );
	ActiveEffects.push_back( new CParticleEffect );
	ActiveEffects[11]->LoadFromFile("Resource/Graphics/Particles/LightningSlash.xml" );
	ActiveEffects.push_back( new CParticleEffect );
	ActiveEffects[12]->LoadFromFile("Resource/Graphics/Particles/LevelUp.xml" );
	ActiveEffects.push_back( new CParticleEffect );
	ActiveEffects[13]->LoadFromFile("Resource/Graphics/Particles/FreezeHeal.xml" );
	ActiveEffects.push_back( new CParticleEffect );
	ActiveEffects[14]->LoadFromFile("Resource/Graphics/Particles/Fireball.xml" );
	ActiveEffects.push_back( new CParticleEffect );
	ActiveEffects[15]->LoadFromFile("Resource/Graphics/Particles/Iceball.xml" );
	ActiveEffects.push_back( new CParticleEffect );
	ActiveEffects[16]->LoadFromFile("Resource/Graphics/Particles/LightningBolt.xml" );
	ActiveEffects.push_back( new CParticleEffect );
	ActiveEffects[17]->LoadFromFile("Resource/Graphics/Particles/EarthShard.xml" );
	ActiveEffects.push_back( new CParticleEffect );
	ActiveEffects[18]->LoadFromFile("Resource/Graphics/Particles/ZombieBlood.xml" );

}

void CParticleManager::UnloadParticleEffects( void )
{
	if (ActiveEffects.size() == 0)
		return;

	for (unsigned int i = 0; i < ActiveEffects.size(); ++i)
		delete ActiveEffects[i];

	ActiveEffects.clear();
}