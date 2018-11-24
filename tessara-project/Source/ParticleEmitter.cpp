/*
	File:		ParticleEmitter.h
	Author:		Mark Diaz
	Purpose:	The CParticleEmitter class enables creation, updating and
				rendering of all the particles that make up a particle emitter

				**Also contains the struct CParticle**
*/

#include "ParticleEmitter.h"
#include "ParticleEffect.h"
#include "Camera.h"
#include <random>
#include <ctime>
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/SGD_Math.h"

CParticleEmitter::CParticleEmitter( CParticleEffect* Parent )
{
	m_pParent = Parent;
	srand((unsigned int)time(NULL));
	m_bDead = false;
	m_nCameraOffsetX = 0;
	m_nCameraOffsetY = 0;
	m_nTextureID = -1;
	m_nMinLife = 0;
	m_nMaxLife = 0;
	m_nMaxParticles = 0;
	m_nXPos = 0;
	m_nYPos = 0;
	m_nWidth = 0;
	m_nRadius = 0;
	m_nHeight = 0;
	m_nCenterX = 0;
	m_nCenterY = 0;
	m_nLeft = 0;
	m_nTop = 0;
	m_nRight = 0;
	m_nBottom = 0;
	m_nMinXVel = 0;
	m_nMaxXVel = 0;
	m_nMinYVel = 0;
	m_nMaxYVel = 0;
	m_nXVelIncrement = 0;
	m_nYVelIncrement = 0;
	m_nSpawnRate = 0;
	m_fSpawnTimer = 0;
	m_fToSpawn = 0;
	m_nSpawned = 0;
	m_cStartColor.A = 0;
	m_cStartColor.R = 0;
	m_cStartColor.G = 0;
	m_cStartColor.B = 0;
	m_cEndColor.A = 0;
	m_cEndColor.R = 0;
	m_cEndColor.G = 0;
	m_cEndColor.B = 0;
	m_fRotationSpeed = 0;
	m_fStartScale = 0;
	m_fEndScale = 0;
	m_fEmitterLife = 0;
	m_bLooping = false;
}

CParticleEmitter& CParticleEmitter::operator= ( const CParticleEmitter& pEmitter)
{
	m_nTextureID = pEmitter.m_nTextureID;
	m_nMinLife = pEmitter.m_nMinLife;
	m_nMaxLife = pEmitter.m_nMaxLife;
	m_nMaxParticles = pEmitter.m_nMaxParticles;
	m_nWidth = pEmitter.m_nWidth;
	m_nRadius = pEmitter.m_nRadius;
	m_nHeight = pEmitter.m_nHeight;
	m_nCenterX = pEmitter.m_nCenterX;
	m_nCenterY = pEmitter.m_nCenterY;
	m_nMinXVel = pEmitter.m_nMinXVel;
	m_nMaxXVel = pEmitter.m_nMaxXVel;
	m_nMinYVel = pEmitter.m_nMinYVel;
	m_nMaxYVel = pEmitter.m_nMaxYVel;
	m_nXVelIncrement = pEmitter.m_nXVelIncrement;
	m_nYVelIncrement = pEmitter.m_nYVelIncrement;
	m_nSpawnRate = pEmitter.m_nSpawnRate;
	m_cStartColor.A = pEmitter.m_cStartColor.A;
	m_cStartColor.R = pEmitter.m_cStartColor.R;
	m_cStartColor.G = pEmitter.m_cStartColor.G;
	m_cStartColor.B = pEmitter.m_cStartColor.B;
	m_cEndColor.A = pEmitter.m_cEndColor.A;
	m_cEndColor.R = pEmitter.m_cEndColor.R;
	m_cEndColor.G = pEmitter.m_cEndColor.G;
	m_cEndColor.B = pEmitter.m_cEndColor.B;
	m_fRotationSpeed = pEmitter.m_fRotationSpeed;
	m_fStartScale = pEmitter.m_fStartScale;
	m_fEndScale = pEmitter.m_fEndScale;
	m_fEmitterLife = pEmitter.m_fEmitterLife;
	m_bLooping = pEmitter.m_bLooping;
	m_eShape = pEmitter.m_eShape;
	m_eFillStyle = pEmitter.m_eFillStyle;

	return *this;
}

CParticleEmitter::~CParticleEmitter(void)
{
	for (unsigned int i = 0; i < Particles.size(); ++i)
		delete Particles[i];
	Particles.clear();
}

void CParticleEmitter::Update( float deltaTime )
{	
	if (m_pParent->UsingCamera())
	{
		RECT cam;
		Camera::GetInstance()->CamRect(&cam);
		m_nCameraOffsetX = cam.left;
		m_nCameraOffsetY = cam.top;
	}

	m_nLiveParticles = 0;

	for (vector <CParticleEmitter::CParticle*>::iterator Iter = Particles.begin(); 
		Iter != Particles.end(); ++Iter)
	{
		if ((*Iter)->m_fLifetime > 0)
			{
				(*Iter)->Update( deltaTime );
				++m_nLiveParticles;
			}
	}

	m_fSpawnTimer += deltaTime;
    m_fToSpawn += m_nSpawnRate * deltaTime;
	
	if ( m_bLooping && (int)m_fToSpawn > m_nSpawned && m_pParent->IsEmitting() )
	{
		SpawnParticles( (int)(m_fToSpawn - m_nSpawned) );
		m_nSpawned += (int)m_fToSpawn - m_nSpawned;
	}

	if (m_fSpawnTimer >= 1.0f)
	{
		m_fSpawnTimer = 0.0f;
		m_nSpawned = 0;
		m_fToSpawn = 0.0f;
	}

	if ((!m_bLooping || !m_pParent->IsEmitting()) && m_nLiveParticles == 0)
		m_bDead = true;
}

void CParticleEmitter::Render( void )
{
	for (unsigned int i = 0; i < Particles.size(); ++i)
		if (Particles[i]->m_fLifetime > 0)
			Particles[i]->Render();
}

void CParticleEmitter::SpawnParticles( int ToSpawn )
{
	if (ToSpawn <= 0)
		return;

	for (unsigned int i = 0; i < Particles.size(); ++i)
	{
		if (Particles[i]->m_fLifetime <= 0)
		{
			Particles[i]->m_fLifetime = (rand() % (m_nMaxLife - m_nMinLife)) + m_nMinLife;
			Particles[i]->m_fScale = m_fStartScale;
			Particles[i]->m_cColor = m_cStartColor;
			Particles[i]->Reset();

			--ToSpawn;

			if (ToSpawn == 0)
				break;
		}
	}
}

void CParticleEmitter::SetXPos( int X )
{ 
	m_nXPos = X;
}

void CParticleEmitter::SetYPos( int Y )
{ 
	m_nYPos = Y;			  
}

void CParticleEmitter::CalculateVertices( void )
{
	m_nLeft = (int)(m_nXPos - m_nWidth * 0.5f);
	m_nTop = (int)(m_nYPos - m_nHeight * 0.5f);
	m_nRight = (int)(m_nLeft + m_nWidth);
	m_nBottom = (int)(m_nTop + m_nHeight);
}

void CParticleEmitter::Initialize( void )
{
	CalculateVertices();
	m_fSpawnTimer = 0.0f;
    m_fToSpawn = 0.0f;
    m_nSpawned = 0;


	for (int i = 0; i < m_nMaxParticles; ++i)
	{
		int life = 0;
		if (m_bLooping == false)
				life = rand() % (m_nMaxLife - m_nMinLife) + m_nMinLife;

		CParticleEmitter::CParticle* Particle = new CParticleEmitter::CParticle(this,
													m_fStartScale, 1.0f, m_cStartColor,
													life);

		Particles.push_back( Particle );
	}
	
}

int	CParticleEmitter::GetXVelocity()		
{ 
	if (m_nMaxXVel - m_nMinXVel != 0)
		return (rand() % (m_nMaxXVel - m_nMinXVel) + m_nMinXVel); 
	else
		return m_nMinXVel;
}		

int	CParticleEmitter::GetYVelocity()		
{
	if (m_nMaxYVel - m_nMinYVel != 0)
		return (rand() % (m_nMaxYVel - m_nMinYVel) + m_nMinYVel); 
	else
		return m_nMinYVel;
}	

void CParticleEmitter::GetSpawnPosition(float &X, float &Y)
{
	if (m_eShape == RECTANGLE)
	{
			X = (float)(rand() % (m_nRight - m_nLeft) + m_nLeft);
			Y = (float)(rand() % (m_nBottom - m_nTop) + m_nTop);

		if (m_eFillStyle == OUTLINE)
		{
			int Side = rand() % 4;

			switch(Side)
			{
			case 0:	X = (float)(m_nLeft);
				break;
			case 1:	X = (float)(m_nRight);
				break;
			case 2:	Y = (float)(m_nTop);
				break;
			case 3:	Y= (float)(m_nBottom);
				break;
			}
		}

		return;
	}

	if (m_eShape == CIRCLE)
	{
		int R1 = m_nRadius;
		int R2 = R1;

		if (m_eFillStyle == FILL)
		{
			R1 = (rand() % m_nRadius) + 1;
			R2 = (rand() % m_nRadius) + 1;
		}

		float angle = SGD_PI * (rand() % 2001);
		angle *= 0.001f;
		

		X = (m_nXPos + R1 * cos(angle));
		Y = (m_nYPos + R2 * sin(angle));
		return;
	}
}

// CParticle---------------------------------------------

// Constructor
CParticleEmitter::CParticle::CParticle( CParticleEmitter* Parent, float Scale,
									    float Rotation, ParticleColor Color, int Life)
{
	m_pParent = Parent;
	m_fScale = Scale;
	m_pParent->GetSpawnPosition(m_nXpos, m_nYpos);
	m_fRotation = Rotation;
	m_cColor = Color;
	m_fLifetime = Life;
	m_nXpos = 0;
	m_nYpos = 0;
	m_nXVel = 0;
	m_nYVel = 0;
	m_cIncrement.A = 0;
	m_cIncrement.R = 0;
	m_cIncrement.G = 0;
	m_cIncrement.B = 0;
	m_cAIncrement = 0;
	m_cRIncrement = 0;
	m_cGIncrement = 0;
	m_cBIncrement = 0;
	m_fScaleIncrement = 0;

	Reset();
}

void CParticleEmitter::CParticle::Update( float deltaTime )
{
	m_fLifetime -= (int)(deltaTime * 1000);
	m_nXVel += (m_pParent->GetXVelIncrement() * deltaTime);
	m_nYVel += (m_pParent->GetYVelIncrement() * deltaTime);
	m_nXpos += (m_nXVel * deltaTime);
	m_nYpos += (m_nYVel * deltaTime);
	
		
	if ( m_cColor.A + m_cAIncrement * deltaTime > 255)
		m_cColor.A = 255;
	else if (m_cColor.A + m_cAIncrement * deltaTime < 0)
		m_cColor.A = 0;
	else
	m_cColor.A += unsigned char(m_cAIncrement * deltaTime);

	m_cColor.R += unsigned char(m_cRIncrement * deltaTime);
	m_cColor.G += unsigned char(m_cGIncrement * deltaTime);
	m_cColor.B += unsigned char(m_cBIncrement * deltaTime);
	m_fScale += m_fScaleIncrement * deltaTime;

	m_fRotation += m_pParent->GetRotationSpeed() * deltaTime;
}

void CParticleEmitter::CParticle::Render( void )
{
	CSGD_TextureManager::GetInstance()->Draw( m_pParent->GetTextureImageID(),
											  (int)m_nXpos - m_pParent->m_nCameraOffsetX, (int)m_nYpos - m_pParent->m_nCameraOffsetY, m_fScale, m_fScale,0,
											  (float)m_pParent->GetCenterX(),(float)m_pParent->GetCenterX(),m_fRotation, D3DCOLOR_ARGB(m_cColor.A,m_cColor.R,m_cColor.G,m_cColor.B));
}

void CParticleEmitter::CParticle::Reset( void )
{
	float SecondsToLive = m_fLifetime * 0.001f;
	
	if (SecondsToLive < 0)
			SecondsToLive = 0;

	m_cAIncrement = (m_pParent->GetEndColor().A - m_pParent->GetStartColor().A) / SecondsToLive;
	m_cRIncrement = (m_pParent->GetEndColor().R - m_pParent->GetStartColor().R) / SecondsToLive;
	m_cGIncrement = (m_pParent->GetEndColor().G - m_pParent->GetStartColor().G) / SecondsToLive;
	m_cBIncrement = (m_pParent->GetEndColor().B - m_pParent->GetStartColor().B) / SecondsToLive;
	m_fScaleIncrement = (m_pParent->GetEndScale() - m_pParent->GetStartScale()) / SecondsToLive;

	m_pParent->GetSpawnPosition(m_nXpos, m_nYpos);

	m_nXVel = (float)(m_pParent->GetXVelocity());
	m_nYVel = (float)(m_pParent->GetYVelocity());
}