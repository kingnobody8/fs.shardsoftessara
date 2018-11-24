/*
File:		LoadLevelMsg.cpp
Author:		Daniel Habig
Course:		SGP
Purpose:	This class tells the playstate to load a new level when the player hits a portal
*/

#include "LoadLevelMsg.h"


LoadLevelMsg::LoadLevelMsg(Portal port): CMessage(MSG_LOADLEVEL)
{
	this->port = port;
}


LoadLevelMsg::~LoadLevelMsg(void)
{
}
