/*
Author: Jorge Juaneda
File: chakram.cpp
*/

#include "chakram.h"
#include "EntityManager.h"
#include "ParticleEffect.h"
#include "MessageSystem.h"
#include "AudioManager.h"
#include "DeleteProjectileMessage.h"
#include "AnimationManager.h"
#include "LevelManager.h"

chakram::chakram(void)
{
	m_bCollide = false;

	m_nNumOfBounce = 0;
	m_nBouncLimit  = 5;

	SetWidth( 20 );
	SetHeight( 20 );
	m_nDir = 0;

	CAudioManager::GetInstance()->PlaySFX( SFX_CHAKRAM );
	ChakramEffect = nullptr;
	SetCurrAnim(AnimationManager::GetInstance()->GetAnimID("chakram"));
	ChakramEffect = new CParticleEffect;
	ChakramEffect->LoadFromFile("Resource/Graphics/Particles/Chakram.xml" );
	ChakramEffect->SetXPosition(GetPosX());
	ChakramEffect->SetYPosition(GetPosY());
	ChakramEffect->ToggleCamera();
	ChakramEffect->Initialize();

	CEntity::AddAnimInfo(GetCurrAnim());
}

chakram::~chakram( void )
{
	delete ChakramEffect;
	ChakramEffect = nullptr;
}

/*virtual*/ void chakram::Render()
{
	if (ChakramEffect != nullptr)
		ChakramEffect->Render();
	CProjectile::Render();
}


/*virtual*/ void chakram::Update( float deltaTime )
{
	//CEntity::Update(deltaTime);
	if (ChakramEffect != nullptr)
	{
		ChakramEffect->SetXPosition(GetPosX());
		ChakramEffect->SetYPosition(GetPosY());

		ChakramEffect->Update( deltaTime );
	}

	//collition whith Tiles
	RECT rOverlap;
	LevelManager* LM = LevelManager::GetInstance();
	rOverlap = LM->Collision(this);

	if( rOverlap.left != 0 && rOverlap.right != 0 )
	{
		m_nDir = rand()% 2;

		int nOVerWidth = rOverlap.right - rOverlap.left;
		int nOverHeight = rOverlap.bottom - rOverlap.top;

		int nCenterOverX = nOVerWidth + rOverlap.left;
		int nCenterOverY = nOverHeight + rOverlap.top;

		if (nOverHeight < nOVerWidth)
		{
			//move up or down
			int t = GetRect().top;

			if (GetDirection() == 0)
			{
				SetPosY( GetPosY() + nOverHeight);
				SetAccY( 300);

				if (m_nDir == 1)
					SetAccX( -300);
				else
					SetAccX( +300);

				SetDirection(1);
			}
			else
			{
				SetPosY( GetPosY() - nOverHeight);
				SetAccY( -300);

				if (m_nDir == 1)
					SetAccX( -300);
				else
					SetAccX( +300);

				SetDirection(0);
			}

		}
		else
		{
			//move left or right
			if (GetDirection() == 2)
			{
				SetPosX( GetPosX() + nOVerWidth);
				if (m_nDir == 1)
					SetAccY( -300);
				else
					SetAccY( +300);

				SetAccX( +300);
				SetDirection(3);

			}
			else
			{
				SetPosX( GetPosX() - nOVerWidth);

				if (m_nDir == 1)
					SetAccY( -300);
				else
					SetAccY( +300);

				SetAccX( -300);
				SetDirection(2);
			}
		}

		m_nNumOfBounce++;	
		CAudioManager::GetInstance()->PlaySFX( SFX_CHAKRAM );

		if (m_nNumOfBounce >= m_nBouncLimit )
		{
			CDeleteProjectileMessage* pMsg = new CDeleteProjectileMessage( this );
			CMessageSystem::GetInstace()->SendMsg( pMsg );
			pMsg = nullptr;
		}
	}
	else
	{

		if (GetAccX() == 0 && GetAccY() == 0 )
		{
			switch( GetDirection() )
			{

			case 0:
				SetAccY( GetAccY() -GetVel());
				break;

			case 1:
				SetAccY( GetAccY() +GetVel());

				break;

			case 2:
				SetAccX( GetAccX() -GetVel());
				break;

			case 3:
				SetAccX( GetAccX() +GetVel());
				break;
			}
		}

		SetPosX( GetPosX() + (int)(GetAccX() * deltaTime));
		SetPosY( GetPosY() + (int)(GetAccY() * deltaTime));

		this->m_vAnimInfos[0]->Update(deltaTime);

	}


}

/*virtual*/ bool chakram::CheckCollision( IEntity* pOther)
{

	//if collision is true 
	if (CProjectile::CheckCollision(pOther))
	{

		//if is the first collision
		if (!m_bCollide)
		{
			if (GetAccX() == 0)
			{
				SetAccX( GetAccY() );
				SetAccY( -GetAccY() );
			}
			else
			{
				SetAccY( -GetAccX() );
				SetAccX( -GetAccX() );
			}

			m_bCollide = true;
		}
		else if( (GetAccX() > 0 && GetAccY() > 0) || ( GetAccX() < 0 && GetAccY() < 0 ) )
			SetAccY( - GetAccY() );
		else 
			SetAccX( - GetAccX());

		m_nNumOfBounce++;

		if (m_nNumOfBounce >= m_nBouncLimit )
		{
			CDeleteProjectileMessage* pMsg = new CDeleteProjectileMessage( this );
			CMessageSystem::GetInstace()->SendMsg( pMsg );
			pMsg = nullptr;
		}


		return true;
	}


	return false;
}


void chakram::TestCol()
{

	if (!m_bCollide)
	{
		if (GetAccX() == 0)
			SetAccX( -GetAccY() );
		else
			SetAccY( -GetAccX() );

		m_bCollide = true;
	}


	//flip
	SetAccY( - GetAccY() );
	m_nNumOfBounce++;

	if (m_nNumOfBounce > m_nBouncLimit )
	{
		CEntityManager::GetInstance()->RemoveEntity( this);
	}

} 


void chakram::ResolveCollision( IEntity* pOther)
{

	CEntity* Temp = dynamic_cast<CEntity*> (pOther);

	RECT rOverlap;
	RECT rOther;

	rOther = pOther->GetRect();

	IntersectRect( &rOverlap, &GetRect(), &rOther );


	int nOVerWidth = rOverlap.right - rOverlap.left;
	int nOverHeight = rOverlap.bottom - rOverlap.top;

	int nCenterOverX = nOVerWidth + rOverlap.left;
	int nCenterOverY = nOverHeight + rOverlap.top;

	int nCenterOtherX = (rOther.right - rOther.left) + rOther.left;
	int nCebterOtherY =  (rOther.bottom - rOther.top) + rOther.top;

	if (nOverHeight < nOVerWidth)
	{
		//move up or down
		if (nCenterOverY < nCebterOtherY)
			SetPosY( GetPosY() - nOverHeight);
		else
			SetPosY( GetPosY() + nOverHeight);

	}
	else
	{
		//move left or right
		if (nCenterOverX < nCenterOtherX)
			SetPosX( GetPosX() - nOVerWidth);
		else
			SetPosX( GetPosX() + nOVerWidth);
	}

}