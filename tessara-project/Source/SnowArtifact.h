#pragma once
#include "artifact.h"
class CSnowArtifact :
	public CArtifact
{
public:
	CSnowArtifact(void);
	~CSnowArtifact(void);

	void Ability1 (int nX, int nY, int m_nDirec);
	void Ability2 (int nX, int nY, int m_nDirec);
	void Ability3 (int nX, int nY, int m_nDirec);
};

