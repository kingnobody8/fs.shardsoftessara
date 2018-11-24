/*
	Author:		Mark Diaz and Benjamin "Evan" Cowles
	File:		InformationManagementState.h
*/

#pragma once
#include "IGameState.h"
#include "ParticleEffect.h"

class CInputManager;
class CSGD_TextureManager;
class CAudioManager;
class CBitmapFont;

class InformationManagementState : public IGameState
{
public:
	// This is a singleton.
	static InformationManagementState* GetInstance();

	//IGameState Interface
	virtual void Enter	( void );
	virtual void Exit	( void );

	virtual bool	Input	( void );
	virtual void	Update	( float fElapsedTime );
	virtual void	Render	( void );
	
	void			DoFullscreen ( bool bIsFullscreen );

private:
	InformationManagementState( void );
	virtual ~InformationManagementState( void );

	InformationManagementState(const InformationManagementState&);
	InformationManagementState& operator= (const InformationManagementState&);

	// SGD Wrappers
	CInputManager*			m_pIM;
	CSGD_TextureManager*	m_pTM;
	CAudioManager*			m_pAM;
	CBitmapFont *			m_pBF;

	// Assets
	int					m_nBackgroundImgID;
	int					m_nMenuButtonImgID;
	int					m_nBackButtonImgID;
	int					m_nSwordImgID;
	int					m_nPanelImgID;
	int					m_nSelectedIndex;
	int					m_nAlpha[7];
	float				m_nAlphaOffset;
	bool				m_bFadingOut;
	float				m_fFadeTimer;
	bool				m_bFadingIn;
	CParticleEffect		Selection;
	CParticleEffect		SelectionLarge;
	CParticleEffect		Burst;

};