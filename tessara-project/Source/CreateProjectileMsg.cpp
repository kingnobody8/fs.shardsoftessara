#include "CreateProjectileMsg.h"


CreateProjectileMsg::CreateProjectileMsg(float fVel, int nDir, int nElement, int nX, int nY, bool bCh) : CMessage(MSG_CREATEPROJECTILE)
{
	m_nDir = nDir;
	m_fVel = fVel;
	m_nPosX = nX;
	m_nPosY = nY;
	m_nElement = nElement;
	m_bIsChakram = bCh;
	Level = 0;
	/*m_nDamage = nDamage;*/
}


CreateProjectileMsg::~CreateProjectileMsg(void)
{
}
