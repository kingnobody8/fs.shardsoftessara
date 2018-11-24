/*
	Author: Jorge Juaneda
	File: Artifact.h
*/
#pragma once
class chakram;

#include <Windows.h>
#include "Player.h"

class CArtifact
{
private:

	int m_nLevel;
	RECT m_rAbility1;
	RECT m_rAbility2;
	RECT m_rAbility3;
	CPlayer* m_pPlayer;

public:

	CArtifact();
	virtual ~CArtifact(void);

	void LevelUp();
	int GetLevel() { return m_nLevel; }

	virtual void Ability1 (int nX, int nY, int m_nDirec);
	virtual void Ability2 (int nX, int nY, int m_nDirec);
	virtual void Ability3 (int nX, int nY, int m_nDirec);

	virtual RECT GetAbility1Rect() { return m_rAbility1; }
	virtual RECT GetAbility2Rect() { return m_rAbility2; }
	virtual RECT GetAbility3Rect() { return m_rAbility3; }

	void SetAbility1Rect( RECT rRec) {  m_rAbility1 = rRec; }
	void SetAbility2Rect( RECT rRec) {  m_rAbility2 = rRec; }
	void SetAbility3Rect( RECT rRec) {  m_rAbility3 = rRec; }

	void SetPlayer( CPlayer* pPlayer) { m_pPlayer = pPlayer;}
	CPlayer* GetPlayer() { return m_pPlayer; }

	
};

