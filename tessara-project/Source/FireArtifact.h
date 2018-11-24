#pragma once
#include "artifact.h"
class FireArtifact :
	public CArtifact
{
public:
	FireArtifact(void);
	~FireArtifact(void);

	void Ability1 (int nX, int nY, int m_nDirec);
	void Ability2 (int nX, int nY, int m_nDirec);
	void Ability3 (int nX, int nY, int m_nDirec);
};

