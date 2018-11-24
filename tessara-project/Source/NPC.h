/*
File:		NPC.h
Author:		Daniel Habig
Course:		SGP
Purpose:	The NPC class is an entity that has text for the player to talk to
*/

#pragma once
#include "Entity.h"
#include "PathFinder.h"
#include "Level.h"
#include "AnimationManager.h"

#define WAIT 3.0f

class NPC : public CEntity, public CPathFinder
{
private:
	float m_fPathTimer;
	int	   m_nSpeed;
	string name;
	string text;
	string item;
	bool given;
	bool stop;
	POINT Goal;
	vector<Area> nodes;
	int	   m_nCurrentNode;

public:
	NPC(void);
	~NPC(void);

	virtual void Update( float deltaTime );
	virtual void Render( void );
	virtual bool CheckCollision( IEntity* pOther );

	virtual RECT GetRect ();

	void SetNPC(vector<Area> nodes, string name, string text, string item)
	{
		for(unsigned int i = 0;i < nodes.size(); i++)
		{
			this->nodes.push_back( nodes[i]);
		}

		SetPosX(nodes[0].cols * 64 + 32);
		SetPosY(nodes[0].rows * 64 + 32);

		this->m_fPathTimer = WAIT;
		this->name = name;
		this->text = text;
		this->item = item;
		if( item.length() == 0 )
			this->given = true;
		this->m_nSpeed = 90;
		m_nCurrentNode = 0;

		ostringstream oss;

		oss << "npc" << name.c_str() << "WalkUp";

		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID(oss.str()));	//Walk Up		0
		oss.str("");
		oss << "npc" << name.c_str() << "WalkDown";
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID(oss.str()));	//Walk Down		1
		oss.str("");
		oss << "npc" << name.c_str() << "WalkLeft";
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID(oss.str()));	//Walk Left		2
		oss.str("");
		oss << "npc" << name.c_str() << "WalkRight";
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID(oss.str()));	//Walk Right	3
		oss.str("");
		oss << "npc" << name.c_str() << "StandUp";
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID(oss.str()));	//Stand Up		4
		oss.str("");
		oss << "npc" << name.c_str() << "StandDown";
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID(oss.str()));	//Stand Down	5
		oss.str("");
		oss << "npc" << name.c_str() << "StandLeft";
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID(oss.str()));	//Stand Left	6
		oss.str("");
		oss << "npc" << name.c_str() << "StandRight";
		AddAnimInfo(AnimationManager::GetInstance()->GetAnimID(oss.str()));	//Stand Right	7
		SetCurrAnim(5);
	}

	void SetGiven(bool set) { this->given = set;}
};

