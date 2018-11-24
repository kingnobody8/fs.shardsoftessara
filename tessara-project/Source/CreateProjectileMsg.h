#pragma once
#include "message.h"
class CreateProjectileMsg :
	public CMessage
{
private:

	int m_nPosX;
	int m_nPosY;
	int m_nDir;
	int m_nElement;
	float m_fVel;
	int m_nDamage;
	bool m_bIsChakram;
	int Level;

public:

	CreateProjectileMsg(float fVel, int nDir, int nElement, int nX, int nY, bool bCh);
	~CreateProjectileMsg(void);

	void SetLevel( int L ) { Level = L; }
	int GetPosX() {return m_nPosX; }
	int GetPosY() { return m_nPosY; }
	int GetDirection() { return m_nDir; }
	int GetElement() {return m_nElement; }
	int GetLevel() { return Level; }
	float GetVel() {return m_fVel; }
	bool GetIsCh() {return m_bIsChakram; }
	int GetDamage() {return m_nDamage;}
};

