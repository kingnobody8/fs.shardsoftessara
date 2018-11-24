#pragma once
#include "artifact.h"
class CLightning;

class CElectricArtifact :
	public CArtifact
{
public:
	CElectricArtifact(void);
	~CElectricArtifact(void);

	void Ability1 (int nX, int nY, int m_nDirec);
	void Ability2 (int nX, int nY, int m_nDirec);
};

