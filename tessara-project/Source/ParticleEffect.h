/*
	File:		ParticleEffect.h
	Author:		Mark Diaz
	Purpose:	The CParticleEffect class enables creation, updating and
				rendering of all the emitters that make up a particle effect
*/

#pragma once

#include <vector>
using namespace std;

class CParticleEmitter;

class CParticleEffect
{
public:
	CParticleEffect(void);
	~CParticleEffect(void);

	CParticleEffect( const CParticleEffect& );
	CParticleEffect& operator= ( const CParticleEffect& pEffect );

	// Methods
	void Update( float deltaTime );
	void Render( void );
	void CreateEmitter( void );
	bool LoadFromFile( const char* szFilename );
	void Initialize( void );
	void ToggleCamera( void ) { m_bCamera = !m_bCamera; }
	bool IsDead( void ) { return m_bDead; }
	bool IsEmitting( void ) { return m_bEmit; }

	// Positioning
	void StopEmitting( void ) { m_bEmit = false; }
	void StartEmitting( void ) { m_bEmit = true; }
	int	 GetXPosition ( void )	{ return m_nXPosition; }
	int	 GetYPosition ( void )	{ return m_nYPosition; }
	bool UsingCamera  ( void )  { return m_bCamera;	   }
	void SetXPosition ( int X );
	void SetYPosition ( int Y );

private:
	vector< CParticleEmitter* > ActiveEmitters;

	// Assets
	char*   m_cFilePath;
	bool	m_bCamera;
	bool	m_bDead;
	bool	m_bEmit;
	int		m_nXPosition;
	int		m_nYPosition;

};

