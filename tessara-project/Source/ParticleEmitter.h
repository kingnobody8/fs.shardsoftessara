/*
	File:		ParticleEmitter.h
	Author:		Mark Diaz
	Purpose:	The CParticleEmitter class enables creation, updating and
				rendering of all the particles that make up a particle emitter

				**Also contains the struct CParticle**
*/

#pragma once

#include <vector>
#include "../SGD Wrappers/CSGD_TextureManager.h"

using namespace std;
class CParticleEffect;

class CParticleEmitter
{
public:

	struct ParticleColor
			{unsigned char A;
			 unsigned char R;
			 unsigned char G;
			 unsigned char B;};
	enum Shape { RECTANGLE, CIRCLE };
	enum FillStyle { FILL, OUTLINE };

	// Constructor & Trilogy
	CParticleEmitter ( CParticleEffect* Parent );
	~CParticleEmitter(void);

	CParticleEmitter( const CParticleEmitter& );
	CParticleEmitter& operator= ( const CParticleEmitter& pEmitter);

	// Methods
	void Update			( float deltaTime );
	void Render			( void );
	void Initialize     ( void );
	void SpawnParticles ( int ToSpawn );

	// Mutators
	void		SetTextureID	 ( int id		   )	{ m_nTextureID = id;	  }
	void		SetStartColor	 ( ParticleColor P )	{ m_cStartColor = P;	  }
	void		SetEndColor		 ( ParticleColor P )	{ m_cEndColor = P;		  }
	void		SetXPos			 ( int X		   );	
	void		SetYPos			 ( int Y		   );
	void		SetMinLife		 ( int Min   	   )	{ m_nMinLife = Min;		  }
	void		SetMaxLife		 ( int Max	       )	{ m_nMaxLife = Max;		  }
	void		SetSpawnRate	 ( int Spawn	   )    { m_nSpawnRate = Spawn;	  }
	void		SetMaxParticles  ( int MaxP		   )    { m_nMaxParticles = MaxP; }
	void		SetWidth		 ( int Width       )	{ m_nWidth = Width;		  }
	void		SetHeight		 ( int Height      )    { m_nHeight = Height;  	  }
	void		SetMinXVelocity	 ( int XVel        )	{ m_nMinXVel = XVel;	  }
	void		SetMinYVelocity	 ( int YVel        )	{ m_nMinYVel = YVel;	  }
	void		SetMaxXVelocity	 ( int XVel        )	{ m_nMaxXVel = XVel;	  }
	void		SetMaxYVelocity	 ( int YVel        )	{ m_nMaxYVel = YVel;	  }
	void		SetRotationSpeed ( float R		   )	{ m_fRotationSpeed = R;	  }
	void		SetStartScale    ( float	S	   )	{ m_fStartScale = S;	  }
	void		SetEndScale      ( float	S	   )	{ m_fEndScale = S;		  }
	void		SetLifetime		 ( float Life      )	{ m_fEmitterLife = Life;  }
	void		SetLooping		 ( bool	Loop       )	{ m_bLooping = Loop;	  }
	void		SetCenterX		 ( int CX		   )	{ m_nCenterX = CX;		  }
	void		SetCenterY		 ( int CY		   )	{ m_nCenterY = CY;		  }
	void		SetXVelIncrement ( int XVel		   )	{ m_nXVelIncrement = XVel;}
	void		SetYVelIncrement ( int YVel		   )	{ m_nYVelIncrement = YVel;}
	void		SetShape		 ( Shape Shape	   )	{ m_eShape = Shape;		  }
	void		SetStyle		 ( FillStyle Style )	{ m_eFillStyle = Style;	  }
	void		SetRadius		 ( int R		   )	{ m_nRadius = R;		  }


	// Accessors
	void			GetSpawnPosition( float &X, float &Y);
	int				GetTextureImageID() { return m_nTextureID;	    }	
	int				GetMaxLife()		{ return m_nMaxLife;		}
	int				GetCenterX()		{ return m_nCenterX;		}
	int				GetCenterY()		{ return m_nCenterY;		}
	ParticleColor	GetStartColor()		{ return m_cStartColor;		}
	ParticleColor	GetEndColor()		{ return m_cEndColor;		}
	float			GetStartScale()		{ return m_fStartScale;		}
	float			GetEndScale()		{ return m_fEndScale;		}
	float			GetRotationSpeed()	{ return m_fRotationSpeed;	}
	bool			IsDead()			{ return m_bDead;			}
	int				GetXVelIncrement()	{ return m_nXVelIncrement;	}
	int				GetYVelIncrement()	{ return m_nYVelIncrement;  }
	int				GetXVelocity();		
	int				GetYVelocity();	
	int				GetXPosition()		{ return m_nXPos;			}
	int				GetYPosition()		{ return m_nYPos;			}

	void			CalculateVertices( void );

private:
	struct CParticle
	{
		// Constructor / Destructor
		CParticleEmitter::CParticle::CParticle( CParticleEmitter* Parent, float Scale,
												float Rotation, ParticleColor Color, int Life);
		~CParticle(void){};

		// Methods
		void Update( float deltaTime );
		void Render(void);
		void Reset(void);

		// Members
		CParticleEmitter*	m_pParent;
		float				m_fScale;
		float				m_nXpos;
		float				m_nYpos;
		float				m_fRotation;
		float				m_nXVel;
		float				m_nYVel;
		ParticleColor		m_cColor;
		ParticleColor		m_cIncrement;
		int					m_fLifetime;
		float				m_cAIncrement;
		float				m_cRIncrement;
		float				m_cGIncrement;
		float				m_cBIncrement;
		float				m_fScaleIncrement;


	};

	vector<CParticle*> Particles;
	int				   m_nLiveParticles;

	Shape m_eShape;
	FillStyle m_eFillStyle;

	// Attributes
	CParticleEffect*	m_pParent;
	int					m_nCameraOffsetX;
	int					m_nCameraOffsetY;
	int					m_nTextureID;
	int					m_nMinLife;
	int					m_nMaxLife;
	int					m_nMaxParticles;
	int					m_nXPos;
	int					m_nYPos;
	int					m_nWidth;
	int					m_nRadius;
	int					m_nHeight;
	int					m_nCenterX;
	int					m_nCenterY;
	int					m_nLeft;
	int					m_nTop;
	int					m_nRight;
	int					m_nBottom;

	// Velocity attributes--------
	int					m_nMinXVel;
	int					m_nMaxXVel;
	int					m_nMinYVel;
	int					m_nMaxYVel;
	int					m_nXVelIncrement;
	int					m_nYVelIncrement;

	// Spawn attributes
	int					m_nSpawnRate;
	float				m_fSpawnTimer;
	float				m_fToSpawn;
	int					m_nSpawned;

	// Particles
	ParticleColor		m_cStartColor;
	ParticleColor		m_cEndColor;
	float				m_fRotationSpeed;
	float				m_fStartScale;
	float				m_fEndScale;
	float				m_fEmitterLife;
	bool				m_bLooping;
	bool				m_bDead;

};

