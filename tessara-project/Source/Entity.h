#pragma once
#include "ientity.h"
#include "AnimationInformation.h"
#include <vector>
using namespace std;

enum	EntityType		{ ENT_BASIC, ENT_PLAYER, ENT_WIGHT, ENT_ARCHER, ENT_WRAITH, ENT_ZOMBIE, ENT_BOSS, ENT_PROJECTILE, ENT_NPC, ENT_DOOR, ENT_POW, ENT_ITEM};

class CEntity :
	public IEntity
{
	// entity type
protected:
	EntityType			m_eType;
	int					m_nFacing;

public:
	CEntity( void );
	virtual ~CEntity( void );		// MUST be virtual!!!

	// IEntity interface:
	virtual void	Update ( float fElapsedTime );		// move the entity
	virtual void	Render ( void );					// draw the entity
	virtual void	RenderAlpha (int nAlpha);

	virtual RECT	GetRect( void ) const;				// return bounding box
	virtual int		GetType( void ) const				{	return m_eType;		}
	virtual bool	CheckCollision( IEntity* pOther );	// handle collision against other entity
	virtual int	GetFacing() const { return m_nFacing; }

	virtual void	AddRef ( void );
	virtual void	Release( void );
	void AddAnimInfo(int nAnimationID);

	// Accessors:
	int				GetPosX		( void ) const		{	return m_nPosX;		}
	int				GetPosY		( void ) const		{	return m_nPosY;		}
	float			GetVelX		( void ) const		{	return m_fVelX;		}
	float			GetVelY		( void ) const		{	return m_fVelY;		}
	int				GetImageID	( void ) const		{	return m_nImageID;	}
	unsigned int	GetWidth	( void ) const		{	return m_unWidth;	}
	unsigned int	GetHeight	( void ) const		{	return m_unHeight;	}
	int				GetCurrAnim ( void ) const		{   return m_nCurrAnim;	}
	AnimationInformation * GetAnimationInformation(int nCurrAnim) const { return m_vAnimInfos[nCurrAnim]; }

	// Mutators:
	void			SetPosX		( int x  )			{	m_nPosX		= x ;	}
	void			SetPosY		( int y  )			{	m_nPosY		= y ;	}
	void			SetVelX		( float vx )		{	m_fVelX		= vx;	}
	void			SetVelY		( float vy )		{	m_fVelY		= vy;	}
	void			SetImageID	( int id )			{	m_nImageID	= id;	}
	void			SetWidth	( unsigned int w )	{	m_unWidth	= w;	}
	void			SetHeight	( unsigned int h )	{	m_unHeight	= h;	}
	void			SetType		( EntityType eType) {   m_eType		= eType;}
	void			SetCurrAnim ( int n)			{	m_nCurrAnim = n;	}
	void			SetRefCounter()					{	m_unRefCount = 0;	}

private:

	// position
	int				m_nPosX;
	int				m_nPosY;

	// velocity
	float			m_fVelX;
	float			m_fVelY;

	// image
	int				m_nImageID;

	// size
	unsigned int	m_unWidth;
	unsigned int	m_unHeight;

	// reference count
	unsigned int	m_unRefCount;

protected:
	// AnimationAddition
	vector<AnimationInformation *> m_vAnimInfos;
	int m_nCurrAnim;
};

