/*
	File:		ParticleEffect.cpp
	Author:		Mark Diaz
	Purpose:	The CParticleEffect class enables creation, updating and
				rendering of all the emitters that make up a particle effect
*/

#include "ParticleEffect.h"
#include "ParticleEmitter.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../TinyXML/tinyxml.h"
#include "../SGD Wrappers/SGD_String.h"

CParticleEffect::CParticleEffect(void)
{
	m_cFilePath = "Resource/Graphics/Particles/";
	m_bCamera = false;
	m_bDead = false;
	m_bEmit = true;
	m_nXPosition = 0;
	m_nYPosition = 0;
}

CParticleEffect& CParticleEffect::operator= ( const CParticleEffect& pEffect )
{
	for (unsigned int i = 0; i < pEffect.ActiveEmitters.size(); ++i )
	{
		ActiveEmitters.push_back(new CParticleEmitter(this));
		*ActiveEmitters[i] = *pEffect.ActiveEmitters[i];
	}

	return *this;
}

CParticleEffect::~CParticleEffect(void)
{
	for (unsigned int i = 0; i < ActiveEmitters.size(); ++i)
		delete ActiveEmitters[i];

	ActiveEmitters.clear();
}

void CParticleEffect::Update( float deltaTime )
{
	bool AllDead = true;

	for (unsigned int i = 0; i < ActiveEmitters.size(); i++)
	{
		ActiveEmitters[i]->Update(deltaTime);
		if (!ActiveEmitters[i]->IsDead())
			AllDead = false;
	}

	if (AllDead)
		m_bDead = true;
}

void CParticleEffect::Render( void )
{
	for (unsigned int i = 0; i < ActiveEmitters.size(); ++i)
		ActiveEmitters[i]->Render();
}

void CParticleEffect::SetXPosition( int X )
{
	int Diff = X - m_nXPosition;

	for (unsigned int i = 0; i < ActiveEmitters.size(); ++i)
	{
		ActiveEmitters[i]->SetXPos(ActiveEmitters[i]->GetXPosition() + Diff);
		ActiveEmitters[i]->CalculateVertices();
	}

	m_nXPosition = X;
}

void CParticleEffect::SetYPosition( int Y )
{
	int Diff = Y - m_nYPosition;

	for (unsigned int i = 0; i < ActiveEmitters.size(); ++i)
	{
		ActiveEmitters[i]->SetYPos(ActiveEmitters[i]->GetYPosition() + Diff);
		ActiveEmitters[i]->CalculateVertices();
	}

	m_nYPosition = Y;
}

void CParticleEffect::Initialize( void )
{

	for (unsigned int i = 0; i < ActiveEmitters.size(); ++i)
		ActiveEmitters[i]->Initialize();
}

bool CParticleEffect::LoadFromFile( const char* szFilename )
{
	TiXmlDocument doc;

	if ( doc.LoadFile( szFilename ) == false )
		return false;

	TiXmlElement* pRoot = doc.RootElement();

	if (pRoot == nullptr)
		return false;

	// Clear out the ActiveEmitters vector
	ActiveEmitters.clear();

	// Iterate through the children nodes "Emitter"
	TiXmlElement* pEmitter = pRoot->FirstChildElement( "Emitter" );

	while (pEmitter != nullptr)
	{
		CParticleEmitter* Emitter = new CParticleEmitter(this);

		int StartA, StartR, StartG, StartB;
		CParticleEmitter::ParticleColor Start;
		pEmitter->Attribute( "StartA",		&StartA );
		pEmitter->Attribute( "StartR",		&StartR );
		pEmitter->Attribute( "StartG",		&StartG );
		pEmitter->Attribute( "StartB",		&StartB );
		Start.A = StartA; Start.R = StartR; Start.G = StartG; Start.B = StartB;
		Emitter->SetStartColor(Start);

		int EndA, EndR, EndG, EndB;
		CParticleEmitter::ParticleColor End;
		pEmitter->Attribute( "EndA",		&EndA );
		pEmitter->Attribute( "EndR",		&EndR );
		pEmitter->Attribute( "EndG",		&EndG );
		pEmitter->Attribute( "EndB",		&EndB );
		End.A = EndA; End.R = EndR; End.G = EndG; End.B = EndB;
		Emitter->SetEndColor(End);		

		int Xpos, Ypos;
		pEmitter->Attribute( "XPos",		&Xpos );
		pEmitter->Attribute( "YPos",		&Ypos );
		Emitter->SetXPos(m_nXPosition + Xpos);
		Emitter->SetYPos(m_nYPosition + Ypos);	

		int MinLife, MaxLife;
		pEmitter->Attribute( "MinLife",		&MinLife );
		pEmitter->Attribute( "MaxLife",		&MaxLife );
		Emitter->SetMinLife(MinLife);		
		Emitter->SetMaxLife(MaxLife);	

		int Spawn, MaxP;
		pEmitter->Attribute( "Spawn",		&Spawn );
		pEmitter->Attribute( "MaxP",		&MaxP );
		Emitter->SetSpawnRate(Spawn); 
		Emitter->SetMaxParticles(MaxP); 

		int Width, Height;
		pEmitter->Attribute( "Width",		&Width );
		pEmitter->Attribute( "Height",		&Height );
		Emitter->SetWidth(Width);		
		Emitter->SetHeight(Height);

		int CenterX, CenterY;
		pEmitter->Attribute( "CenterX",		&CenterX );
		pEmitter->Attribute( "CenterY",		&CenterY );
		Emitter->SetCenterX(CenterX);
		Emitter->SetCenterY(CenterY);

		int MinXVel, MinYVel, MaxXVel, MaxYVel;
		pEmitter->Attribute( "MinXVel",		&MinXVel );
		pEmitter->Attribute( "MinYVel",		&MinYVel );
		pEmitter->Attribute( "MaxXVel",		&MaxXVel );
		pEmitter->Attribute( "MaxYVel",		&MaxYVel );
		Emitter->SetMinXVelocity(MinXVel);	
		Emitter->SetMinYVelocity(MinYVel);	
		Emitter->SetMaxXVelocity(MaxXVel);	
		Emitter->SetMaxYVelocity(MaxYVel);	

		int XVelI, YVelI;
		pEmitter->Attribute( "XVelI",		&XVelI   );
		pEmitter->Attribute( "YVelI",		&YVelI   );
		Emitter->SetXVelIncrement( XVelI );
		Emitter->SetYVelIncrement( YVelI );

		double RotateSpeed, StartScale, EndScale, Lifetime;
		pEmitter->Attribute( "RotateSpeed",		&RotateSpeed );
		pEmitter->Attribute( "StartScale",		&StartScale );
		pEmitter->Attribute( "EndScale",		&EndScale );
		pEmitter->Attribute( "Lifetime",		&Lifetime );
		Emitter->SetRotationSpeed((float)RotateSpeed);
		Emitter->SetStartScale((float)StartScale);   
		Emitter->SetEndScale((float)EndScale);     
		Emitter->SetLifetime((float)Lifetime);	

		int Shape, Fill, Radius;
		pEmitter->Attribute("Shape",	&Shape);
		pEmitter->Attribute("Fill",		&Fill);
		pEmitter->Attribute("Radius",	&Radius);
		Emitter->SetRadius(Radius);
		Emitter->SetShape( (CParticleEmitter::Shape)(Shape - 1) );
		Emitter->SetStyle( (CParticleEmitter::FillStyle)(Fill - 1) );

		int Loop;
		pEmitter->Attribute( "Loop",		&Loop );
		if (Loop == 0)
			Emitter->SetLooping(false);
		if (Loop == 1)
			Emitter->SetLooping(true);

		char Filebuffer[1024] = {};
		const char * file = pEmitter->GetText();
		strcat_s(Filebuffer,m_cFilePath);
		strcat_s(Filebuffer,file);
		TCHAR test[1024];
		CSTR_TO_WSTR(test, 1024, Filebuffer);
		Emitter->SetTextureID(CSGD_TextureManager::GetInstance()->LoadTexture(test));

		ActiveEmitters.push_back(Emitter);
	
		pEmitter = pEmitter->NextSiblingElement( "Emitter" );
	}

	return true;
}

