#include "Entity.h"
#include <Windows.h>
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "AnimationManager.h"
#include "Camera.h"
#include "LevelManager.h"

CEntity::CEntity(void)
{
	// Set the entity type
	m_eType		= ENT_BASIC;

	// Set the data members to 'clean' values
	m_nPosX		= 0;
	m_nPosY		= 0;

	m_fVelX		= 0;
	m_fVelY		= 0;

	m_nImageID	= -1;

	m_unWidth	= 1;
	m_unHeight	= 1;

	m_unRefCount = 0;		// starts with 0 references
}

CEntity::~CEntity(void)
{
	for(unsigned int i = 0; i < this->m_vAnimInfos.size(); i++)
	{
		delete this->m_vAnimInfos[i];
	}

	this->m_vAnimInfos.clear();
}

// Update
/*virtual*/ void	CEntity::Update( float fElapsedTime )
{
	m_nPosX += (int)(m_fVelX * fElapsedTime);
	m_nPosY += (int)(m_fVelY * fElapsedTime);

	if(this->m_nCurrAnim != -1)
	{
		this->m_vAnimInfos[this->m_nCurrAnim]->Update(fElapsedTime);
	}

}

// Render
/*virtual*/ void	CEntity::Render( void )
{
	if(this->m_nCurrAnim != -1)
	{
		RECT rRect;
		RECT rRect2 = *AnimationManager::GetInstance()->GetAnimation(this->m_vAnimInfos[this->m_nCurrAnim]->GetAnimID()).GetFrame(this->m_vAnimInfos[this->m_nCurrAnim]->GetCurFrame()).GetDrawRect();

		Camera::GetInstance()->CamRect(&rRect);
		//RECT rOverlap;

		int nTemp1 = AnimationManager::GetInstance()->GetAnimation(this->m_vAnimInfos[this->m_nCurrAnim]->GetAnimID()).GetFrame(this->m_vAnimInfos[this->m_nCurrAnim]->GetCurFrame()).GetPassiveRect()->bottom;
		int nTemp2 = AnimationManager::GetInstance()->GetAnimation(this->m_vAnimInfos[this->m_nCurrAnim]->GetAnimID()).GetFrame(this->m_vAnimInfos[this->m_nCurrAnim]->GetCurFrame()).GetAnchorPoint().y;
		int nTemp = (nTemp1 - nTemp2) * 2;

		/*if(IntersectRect(&rOverlap, &rRect, &rRect2) == TRUE)
		{*/
			AnimationManager::GetInstance()->Draw(*this->m_vAnimInfos[this->m_nCurrAnim], (int)this->GetPosX() - rRect.left, (int)this->GetPosY() - rRect.top);
		/*}*/
	}
}

/*virtual*/ void	CEntity::RenderAlpha(int nAlpha)
{
	if(this->m_nCurrAnim != -1 && this->m_vAnimInfos.size() > 0)
	{
		RECT rRect;
		RECT rRect2 = *AnimationManager::GetInstance()->GetAnimation(this->m_vAnimInfos[this->m_nCurrAnim]->GetAnimID()).GetFrame(this->m_vAnimInfos[this->m_nCurrAnim]->GetCurFrame()).GetDrawRect();

		Camera::GetInstance()->CamRect(&rRect);
		//RECT rOverlap;

		int nTemp1 = AnimationManager::GetInstance()->GetAnimation(this->m_vAnimInfos[this->m_nCurrAnim]->GetAnimID()).GetFrame(this->m_vAnimInfos[this->m_nCurrAnim]->GetCurFrame()).GetPassiveRect()->bottom;
		int nTemp2 = AnimationManager::GetInstance()->GetAnimation(this->m_vAnimInfos[this->m_nCurrAnim]->GetAnimID()).GetFrame(this->m_vAnimInfos[this->m_nCurrAnim]->GetCurFrame()).GetAnchorPoint().y;
		int nTemp = (nTemp1 - nTemp2) * 2;

		/*if(IntersectRect(&rOverlap, &rRect, &rRect2) == TRUE)
		{*/
			AnimationManager::GetInstance()->Draw(*this->m_vAnimInfos[this->m_nCurrAnim], (int)this->GetPosX() - rRect.left, (int)this->GetPosY() - rRect.top, 0.0f, 0.0f, 0.0f, nAlpha);
		/*}*/
	}
}

void	CEntity::AddRef ( void )
{
	m_unRefCount++;
}

void	CEntity::Release( void )
{
	m_unRefCount--;

	if (m_unRefCount <= 0 )
	{
		delete this;
	}
}

/*virtual*/RECT CEntity::GetRect( void ) const
{
	Frame tempFrame = AnimationManager::GetInstance()->GetAnimation(this->m_vAnimInfos[this->m_nCurrAnim]->GetAnimID()).GetFrame(this->m_vAnimInfos[this->m_nCurrAnim]->GetCurFrame());
	POINT pAnchor = tempFrame.GetAnchorPoint();
	RECT rActive = *tempFrame.GetPassiveRect();
	RECT rRect;

	int nHeight = rActive.bottom - rActive.top;
	int nWidth = rActive.right - rActive.left;
	int nX = (int)GetPosX();
	int nY = (int)GetPosY();

	rRect.bottom = nY + (rActive.bottom - pAnchor.y);
	rRect.top = rRect.bottom - nHeight;
	rRect.right = nX + (rActive.right - pAnchor.x);
	rRect.left = rRect.right - nWidth;

	return rRect;
}

// CheckCollision
/*virtual*/ bool CEntity::CheckCollision( IEntity* pOther )
{
	RECT rSelf;

	if(this->GetType() == ENT_PROJECTILE && dynamic_cast<CProjectile *>(this)->IsExplosion())
	{
		rSelf = dynamic_cast<CProjectile *>(this)->GetRect();
	}
	else
	{
		rSelf	= GetRect();
	}

	RECT rOther	= pOther->GetRect();
	RECT rOverlap;

	if( IntersectRect( &rOverlap, &rSelf, &rOther ) == TRUE )
		return true;
	else
		return false;
}

void CEntity::AddAnimInfo(int nAnimationID)
{
	AnimationInformation* tempAnimInfo = new AnimationInformation(nAnimationID);
	this->m_vAnimInfos.push_back(tempAnimInfo);
}
