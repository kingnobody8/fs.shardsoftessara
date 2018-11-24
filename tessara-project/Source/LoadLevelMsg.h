/*
File:		LoadLevelMsg.h
Author:		Daniel Habig
Course:		SGP
Purpose:	This class tells the playstate to load a new level when the player hits a portal
*/

#pragma once
#include "message.h"
#include "Level.h"

class LoadLevelMsg :
	public CMessage
{
private:
	Portal port;
public:
	LoadLevelMsg(Portal port);
	~LoadLevelMsg(void);

	Portal GetPortal()
	{
		return port;
	}
};

