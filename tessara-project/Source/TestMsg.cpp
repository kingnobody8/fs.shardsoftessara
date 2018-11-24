#include "TestMsg.h"


CTestMsg::CTestMsg(int x, int y, int type)
	: CMessage(MSG_TEST)
{
	pX = x;
	pY = y;
	nType = type;
}

CTestMsg::~CTestMsg(void)
{


}

