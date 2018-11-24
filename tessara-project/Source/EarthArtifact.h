#pragma once
#include "artifact.h"
class CEarthArtifact :
	public CArtifact
{
public:
	CEarthArtifact(void);
	~CEarthArtifact(void);

	void Ability1 (int nX, int nY, int m_nDirec);
	void Ability2 (int nX, int nY, int m_nDirec);
	void Ability3 (int nX, int nY, int m_nDirec);
};

