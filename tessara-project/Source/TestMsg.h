#pragma once
#include "message.h"
class CTestMsg :
	public CMessage
{
private:
	int pX;
	int pY;
	int nType;

public:
	CTestMsg(int x, int y, int type);
	~CTestMsg(void);

	int GetPX() { return pX;}
	int GetPY() { return pY;}
	int GetType() {return nType;}
};

