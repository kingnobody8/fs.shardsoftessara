/*
File:		level.cpp
Author:		Daniel Habig
Course:		SGP
Purpose:	The level class contains all tiles in a level and performs updates on the tiles.
*/


#include "Level.h"
#include "LoadLevelMsg.h"
#include "MessageSystem.h"
#include "EntityManager.h"
#include "PlayState.h"
#include "LevelManager.h"
#include "Entity.h"
#include "AnimationManager.h"
#include "AudioManager.h"
#include "PlayState.h"
#include "Camera.h"
#include "Power.h"
#include "Door.h"
#include "../SGD Wrappers/CSGD_EventSystem.h"
//XML
//#include "../TinyXML/tinyxml.h"
#include "../RapidXML/rapidxml.hpp"
#include "../RapidXML/rapidxml_iterators.hpp"
#include "../RapidXML/rapidxml_print.hpp"
#include "../RapidXML/rapidxml_utils.hpp"
using namespace rapidxml;

Level::Level(void)
{
}

Level::~Level(void)
{
}

void Level::Update( float fElapsedTime )
{
	for( unsigned int i = 0; i < map.size(); i++)
	{
		if ( map[i]->Update(fElapsedTime) )
		{
			int index = i;
			int col = area.cols;
			Tile_Data* up		= (index - col	>=	0)			? map[i-area.cols] : nullptr;
			Tile_Data* down		= ((unsigned int)(index + col)	<	map.size())	? map[i+area.cols] : nullptr;
			Tile_Data* left		= (index - 1	>=	0 && index/col == (index-1)/col) ? map[i-1] : nullptr;
			Tile_Data* right	= ((unsigned int)(index + 1)	<	map.size() && index/col == (index+1)/col )	? map[i+1] : nullptr;
			map[i]->FireSpread(up, down, left, right);
		}
	}
}
void Level::Render( int alpha )
{
	if( alpha > 255 )
		alpha = 255;
	if( alpha < 0 )
		alpha = 0;

	AnimationManager* AM = AnimationManager::GetInstance();

	RECT cam;
	Camera::GetInstance()->CamRect(&cam);

	Pair start, end;
	start.X = (cam.left >= 0 ) ? cam.left / Pix.X : 0;
	start.Y = (cam.top >= 0 ) ? cam.top / Pix.Y : 0;

	end.X = ((unsigned)cam.right < area.cols * Pix.X) ? cam.right / Pix.X + 1 : area.cols;
	end.Y = ((unsigned)cam.bottom < area.rows * Pix.Y) ? cam.bottom / Pix.Y + 1: area.rows;

	for( int x = start.X; x < end.X; x++ )
		for( int y = start.Y; y < end.Y; y++ )
		{
			int index = y*area.cols + x;
			unsigned int pos;
			switch( this->map[index]->state)
			{
			case NORM:
				pos = this->map[index]->tile->anime.GetCurrFrame();
				break;
			case BURN:
				pos = this->map[index]->tile->burning->anime.GetCurrFrame();
				break;
			case ICED:
				pos = this->map[index]->tile->iced->anime.GetCurrFrame();
				break;
			}
			RECT rect;
			rect.left	= Pix.X*(pos % Set.cols);
			rect.top	= Pix.Y*(pos / Set.cols);
			rect.right	= rect.left + Pix.X;
			rect.bottom = rect.top + Pix.Y;
			AM->RenderTile( rect, x*Pix.X - cam.left, y*Pix.Y - cam.top, alpha);
		}
}
void Level::RenderParticles	( void )
{
	AnimationManager* AM = AnimationManager::GetInstance();

	RECT cam;
	Camera::GetInstance()->CamRect(&cam);

	Pair start, end;
	start.X = (cam.left >= 0 ) ? cam.left / Pix.X : 0;
	start.Y = (cam.top >= 0 ) ? cam.top / Pix.Y : 0;

	end.X = ((unsigned)cam.right < area.cols * Pix.X) ? cam.right / Pix.X + 1 : area.cols;
	end.Y = ((unsigned)cam.bottom < area.rows * Pix.Y) ? cam.bottom / Pix.Y + 1: area.rows;

	for( int x = start.X; x < end.X; x++ )
		for( int y = start.Y; y < end.Y; y++ )
		{
			int index = y*area.cols + x;
			if ((this->map[index]->Breaking || this->map[index]->state == BURN || this->map[index]->state == ICED))
				this->map[index]->TileEffect->Render();
		}
}
RECT Level::Collision( IEntity* entity )
{
	RECT ent = entity->GetRect();
	RECT sect;
	sect.bottom = sect.top = sect.left = sect.right = 0;

	Pair start, end;
	start.X = (ent.left >= 0 ) ? ent.left / Pix.X : 0;
	start.Y = (ent.top >= 0 ) ? ent.top / Pix.Y : 0;

	int col = area.cols;
	int row = area.rows;

	end.X = ( ent.right < col * Pix.X ) ? ent.right / Pix.X : area.cols-1;
	end.Y = ( ent.bottom < row * Pix.Y ) ? ent.bottom / Pix.Y : area.rows-1;

	//Player
	if( entity->GetType() == ENT_PLAYER)
	{
		//Triggers
		for(unsigned int i = 0; i < triggers.size(); i++)
		{
			if(IntersectTrigger(ent, &triggers[i]) && !triggers[i].used)
			{
				TriggerResponse(&triggers[i]);
				if( triggers[i].loop == false )
					LevelManager::GetInstance()->HitTrig(i);
			}
		}

		//Portals
		for(unsigned int i = 0; i < portals.size(); i++)
		{
			if( portals[i].enter.X >= start.X && portals[i].enter.X <= end.X && portals[i].enter.Y >= start.Y && portals[i].enter.Y <= end.Y)
			{
				RECT port, blank;
				port.left = portals[i].enter.X * Pix.X;
				port.top =  portals[i].enter.Y * Pix.Y;
				port.right = port.left + Pix.X;
				port.bottom = port.top + Pix.Y;

				if(IntersectRect(&blank, &port, &ent) == TRUE )
				{
					/////////////////////////////////////////////////////////////////
					// BUG FIX
					// Reference Bug # BB-034
					// BUG FIX START
					/////////////////////////////////////////////////////////////////
					CPlayer* pPlayer = reinterpret_cast<CPlayer*> (entity);
					pPlayer->StopDash();
					///////////////////////////////////////////////////////////////////////////
					// BUG FIX END  Reference # BB-034
					//////////////////////////////////////////////////////////////////////////

					PortalResponse(&portals[i]);
					return sect;
				}
			}
		}
	}

	//Tile Collision
	for( int x = start.X; x <= end.X; x++)
	{
		for( int y = start.Y; y <= end.Y; y++)
		{
			RECT block;
			block.left = x*Pix.X;
			block.top = y*Pix.Y;
			block.right = block.left + Pix.X;
			block.bottom = block.top + Pix.Y;

			switch(entity->GetType())
			{
			case ENT_PLAYER:
				{
					/*if( (strcmp(map[y*area.cols + x]->tile->name.c_str(), "bush" ) == 0 || strcmp(map[y*area.cols + x]->tile->name.c_str(), "bush stump" ) == 0 ) && PlayState::GetInstance()->GetPlayer()->IsDashing() )
					continue;
					else*/ 
					if( map[y*area.cols + x]->CollideEnt() )
						if( IntersectRect(&sect, &block, &ent) == TRUE)
							return sect;
					break;
				}
			case ENT_BOSS:
				{
					if( map[y*area.cols + x]->CollideEnt() )
						if( IntersectRect(&sect, &block, &ent) == TRUE)
							return sect;
					break;
				}
			case ENT_PROJECTILE:
				{
					CProjectile* proj = dynamic_cast<CProjectile*>(entity);
					//is snowy
					if( proj->GetElement() == 1)
					{
						ProjectileResponse(proj, y*area.cols + x);
					}
					if( map[y*area.cols + x]->CollideObj() )
						if( IntersectRect(&sect, &block, &ent) == TRUE)
							ProjectileResponse( proj, y*area.cols + x );
					break;
				}
			}
		}
	}
	return sect;

}
bool Level::IntersectTrigger( RECT rect, Trigger* trig )
{
	Pair alpha,beta;
	//Top
	alpha.X = rect.left;
	alpha.Y = rect.top;
	beta.X = rect.right;
	beta.Y = rect.top;
	if( IntersectLine(alpha, beta, trig->start, trig->end))
		return true;
	//Left
	alpha.X = rect.left;
	alpha.Y = rect.top;
	beta.X = rect.left;
	beta.Y = rect.bottom;
	if( IntersectLine(alpha, beta, trig->start, trig->end))
		return true;
	//Bottom
	alpha.X = rect.left;
	alpha.Y = rect.bottom;
	beta.X = rect.right;
	beta.Y = rect.bottom;
	if( IntersectLine(alpha, beta, trig->start, trig->end))
		return true;
	//Right
	alpha.X = rect.right;
	alpha.Y = rect.top;
	beta.X = rect.right;
	beta.Y = rect.bottom;
	if( IntersectLine(alpha, beta, trig->start, trig->end))
		return true;

	return false;
}
bool Level::IntersectLine(Pair alpha, Pair beta, Pair charlie, Pair delta )
{
	//Slopes
	float s1_x, s1_y, s2_x, s2_y;
	s1_x = (float)(beta.X - alpha.X);     s1_y = (float)(beta.Y - alpha.Y);
	s2_x = (float)(delta.X - charlie.X);  s2_y = (float)(delta.Y - charlie.Y);

	//Determinant
	float s, t;
	s = (-s1_y * (alpha.X - charlie.X) + s1_x * (alpha.Y - charlie.Y)) / (-s2_x * s1_y + s1_x * s2_y);
	t = ( s2_x * (alpha.Y - charlie.Y) - s2_y * (alpha.X - charlie.X)) / (-s2_x * s1_y + s1_x * s2_y);

	//Between the Segment
	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
		// Collision detected
		float i_x = alpha.X + (t * s1_x);
		float i_y = alpha.Y + (t * s1_y);
		return true;
	}

	return false; // No collision
}

bool Level::OnShop(IEntity* ent)
{
	for( unsigned int i = 0; i < shops.size(); i++)
	{
		Trigger trig;
		trig.start = this->shops[i].start;
		trig.end = this->shops[i].end;
		if( IntersectTrigger( ent->GetRect(), &trig ) )
			return true;
	}

	return false;
}
bool Level::OnFire(IEntity* entity)
{
	RECT ent = entity->GetRect();
	RECT sect;
	sect.bottom = sect.top = sect.left = sect.right = 0;

	Pair start, end;
	start.X = (ent.left >= 0 ) ? ent.left / Pix.X : 0;
	start.Y = (ent.top >= 0 ) ? ent.top / Pix.Y : 0;

	end.X = ( (unsigned)ent.right < area.cols * Pix.X ) ? ent.right / Pix.X : area.cols-1;
	end.Y = ( (unsigned)ent.bottom < area.rows * Pix.Y ) ? ent.bottom / Pix.Y : area.rows-1;

	for( int x = start.X; x <= end.X; x++)
	{
		for( int y = start.Y; y <= end.Y; y++)
		{
			RECT block;
			block.left = x*Pix.X;
			block.top = y*Pix.Y;
			block.right = block.left + Pix.X;
			block.bottom = block.top + Pix.Y;

			if( map[y*area.cols + x]->state == BURN )
				if( IntersectRect(&sect, &block, &ent) == TRUE)
					return true;
		}
	}
	return false;
}
bool Level::OnIce(IEntity* entity)
{
	RECT ent = entity->GetRect();
	RECT sect;
	sect.bottom = sect.top = sect.left = sect.right = 0;

	Pair start, end;
	start.X = (ent.left >= 0 ) ? ent.left / Pix.X : 0;
	start.Y = (ent.top >= 0 ) ? ent.top / Pix.Y : 0;

	end.X = ( (unsigned)ent.right < area.cols * Pix.X ) ? ent.right / Pix.X : area.cols-1;
	end.Y = ( (unsigned)ent.bottom < area.rows * Pix.Y ) ? ent.bottom / Pix.Y : area.rows-1;

	for( int x = start.X; x <= end.X; x++)
	{
		for( int y = start.Y; y <= end.Y; y++)
		{
			RECT block;
			block.left = x*Pix.X;
			block.top = y*Pix.Y;
			block.right = block.left + Pix.X;
			block.bottom = block.top + Pix.Y;

			if( map[y*area.cols + x]->state == ICED )
				if( IntersectRect(&sect, &block, &ent) == TRUE)
					return true;
		}
	}
	return false;
}
void Level::Break(IEntity* entity)
{
	RECT ent = entity->GetRect();
	ent.left -= 32;
	ent.right += 32;
	ent.top -= 32;
	ent.bottom += 32;
	RECT sect;
	sect.bottom = sect.top = sect.left = sect.right = 0;

	Pair start, end;
	start.X = (ent.left >= 0 ) ? ent.left / Pix.X : 0;
	start.Y = (ent.top >= 0 ) ? ent.top / Pix.Y : 0;

	end.X = ( (unsigned)ent.right < area.cols * Pix.X ) ? ent.right / Pix.X : area.cols-1;
	end.Y = ( (unsigned)ent.bottom < area.rows * Pix.Y ) ? ent.bottom / Pix.Y : area.rows-1;

	for( int x = start.X; x <= end.X; x++)
	{
		for( int y = start.Y; y <= end.Y; y++)
		{
			RECT block;
			block.left = x*Pix.X;
			block.top = y*Pix.Y;
			block.right = block.left + Pix.X;
			block.bottom = block.top + Pix.Y;

			if( IntersectRect(&sect, &block, &ent) == TRUE)
				map[y*area.cols + x]->Break();
		}
	}
}
void Level::IceWall(IEntity* entity)
{
	if( entity->GetType() == ENT_PLAYER)
	{
		CPlayer* play = dynamic_cast<CPlayer*>(entity);
		RECT ent = entity->GetRect();
		Pair size;
		size.X = ent.right - ent.left;
		size.Y = ent.bottom - ent.top;

		switch(play->GetFacing())
		{
			//UP
		case 0:
			{
				ent.top		-= size.Y/2;
				ent.bottom	-= size.Y;
				ent.left	-= size.X/4;
				ent.right	+= size.X/4;
				break;
			}
			//DOWN
		case 1:
			{
				ent.top		+= size.Y;
				ent.bottom	+= size.Y/2;
				ent.left	-= size.X/4;
				ent.right	+= size.X/4;
				break;
			}
			//LEFT
		case 2:
			{
				ent.top		-= size.Y/2;
				ent.bottom	+= size.Y/2;
				ent.left	-= size.X/2;
				ent.right	-= size.X;
				break;
			}
			//RIGHT
		case 3:
			{
				ent.top		-= size.Y/2;
				ent.bottom	+= size.Y/2;
				ent.left	+= size.X;
				ent.right	+= size.X/2;
				break;
			}
		}

		RECT sect;
		sect.bottom = sect.top = sect.left = sect.right = 0;

		Pair start, end;
		start.X = (ent.left >= 0 ) ? ent.left / Pix.X : 0;
		start.Y = (ent.top >= 0 ) ? ent.top / Pix.Y : 0;

		end.X = ( (unsigned)ent.right < area.cols * Pix.X ) ? ent.right / Pix.X : area.cols-1;
		end.Y = ( (unsigned)ent.bottom < area.rows * Pix.Y ) ? ent.bottom / Pix.Y : area.rows-1;

		for( int x = start.X; x <= end.X; x++)
		{
			for( int y = start.Y; y <= end.Y; y++)
			{
				RECT block;
				block.left = x*Pix.X;
				block.top = y*Pix.Y;
				block.right = block.left + Pix.X;
				block.bottom = block.top + Pix.Y;

				if( IntersectRect(&sect, &block, &ent) == TRUE)
					map[y*area.cols + x]->BeginIce();
			}
		}
	}
}
void Level::FireDash(IEntity* entity)
{
	RECT ent = entity->GetRect();
	RECT sect;
	sect.bottom = sect.top = sect.left = sect.right = 0;

	Pair start, end;
	start.X = (ent.left >= 0 ) ? ent.left / Pix.X : 0;
	start.Y = (ent.top >= 0 ) ? ent.top / Pix.Y : 0;

	end.X = ( (unsigned)ent.right < area.cols * Pix.X ) ? ent.right / Pix.X : area.cols-1;
	end.Y = ( (unsigned)ent.bottom < area.rows * Pix.Y ) ? ent.bottom / Pix.Y : area.rows-1;

	for( int x = start.X; x <= end.X; x++)
	{
		for( int y = start.Y; y <= end.Y; y++)
		{
			RECT block;
			block.left = x*Pix.X;
			block.top = y*Pix.Y;
			block.right = block.left + Pix.X;
			block.bottom = block.top + Pix.Y;

			if( IntersectRect(&sect, &block, &ent) == TRUE)
			{
				map[y*area.cols + x]->BeginBurn();
			}
		}
	}
}

void Level::HitTrigger (int index)
{
	this->triggers[index].used = true;
}
void Level::TriggerResponse(Trigger* trig)
{
	PlayState::GetInstance()->MakeNote(trig->note);
	trig->used = true;
}
void Level::PortalResponse(Portal* port)
{
	LoadLevelMsg* pMsg = new LoadLevelMsg( *port );
	CMessageSystem::GetInstace()->SendMsg( pMsg );
	pMsg = nullptr;

	/*Portal temp = *port;
	Unload();
	LevelManager::GetInstance()->LoadLevel( temp.map );

	Portal other = portals[temp.port];
	Pair loc;
	loc.X = other.exit.X * Pix.X + Pix.X / 2;
	loc.Y = other.exit.Y * Pix.Y + Pix.Y;
	PlayState::GetInstance()->MovePlayer(loc.X, loc.Y);*/
}
void Level::ProjectileResponse(CProjectile* proj, int index)
{
	switch( proj->GetElement() )
	{
		//Fire
	case 0:
		{
			map[index]->BeginBurn();
			break;
		}
		//Ice
	case 1:
		{
			map[index]->BeginIce();
			break;
		}
		//Lightning
	case 2:
		{
			break;
		}
		//Earth
	case 3:
		{
			break;
		}
	}
}

bool Level::Load( string levelName, vector<Tile*>* tileList, Area nSet, Pair nPix   )
{
	this->Set = nSet;
	this->Pix = nPix;
	this->name = levelName;
	//return Tiny(levelName, tileList);
	return Rapid(levelName, tileList);
}
bool Level::Unload( void )
{
	//Delete all the map data
	for( unsigned int i = 0; i < map.size(); i++)
	{
		delete map[i];
	}
	map.clear();

	//Unload other vectors
	this->portals.clear();
	this->triggers.clear();
	this->shops.clear();

	//Unload the music
	CAudioManager::GetInstance()->UnLoadLevelAudio();

	//Unload all other entities
	CEntityManager::GetInstance()->RemoveNPE();

	return true;
}

bool Level::CollidableTile( int x, int y)
{
	return this->map[y*area.cols + x]->CollideEnt();
}

bool Level::Tiny( string levelName, vector<Tile*>* tileList)
{
	/*
	// Create a TinyXML Document
	TiXmlDocument doc;

	// Attempt to load
	if( doc.LoadFile( "Resource/XML/map.xml" ) == false )
	return false;

	// Access the root node
	TiXmlElement* pRoot = doc.RootElement();	// "Map"
	if( pRoot == nullptr )
	return false;

	// Iterate through the children nodes		"LevelList"
	TiXmlElement* eLevelList = pRoot->FirstChildElement( "LevelList" );

	// Iterate through the children nodes of LevelList
	TiXmlElement* eLevel = eLevelList->FirstChildElement( "Level" );

	int rows;
	int cols;
	int triggers;

	while( eLevel != nullptr )
	{
	eLevel->Attribute( "cols",		&cols		);
	eLevel->Attribute( "rows",		&rows		);
	eLevel->Attribute( "triggers",	&triggers	);

	TiXmlElement* eName = eLevel->FirstChildElement( "Name" );
	string name = eName->GetText();
	TiXmlElement* eBattle = eLevel->FirstChildElement( "Battle" );
	string battle;
	if( eBattle->GetText() != nullptr)
	battle = eBattle->GetText();
	TiXmlElement* eWalk = eLevel->FirstChildElement( "Walk" );
	string walk;
	if( eWalk->GetText() != nullptr)
	walk = eWalk->GetText();

	if( strcmp( name.c_str(), levelName.c_str() ) == 0 )
	{
	this->area.cols = cols;
	this->area.rows = rows;
	CAudioManager::GetInstance()->LoadLevelAudio( battle, walk );
	break;
	}

	eLevel = eLevel->NextSiblingElement( "Level" );
	}

	assert( eLevel != nullptr && "Level::Load - Level name is invalid" );

	//Get the player position
	TiXmlElement* eObject = eLevel->FirstChildElement( "Object" );
	while( eObject != nullptr )
	{
	eObject->Attribute( "col",		&cols		);
	eObject->Attribute( "row",		&rows		);

	TiXmlElement* eName = eObject->FirstChildElement( "Name" );

	string name = eName->GetText();

	if( strcmp( name.c_str(), "spawn" ) == 0 )
	{
	//Spawn player
	PlayState::GetInstance()->CreatePlayer(cols*Pix.X, rows*Pix.Y);
	}
	else if( strcmp( name.c_str(), "shard" ) == 0 )
	{
	//create shard object
	}

	eObject = eObject->NextSiblingElement( "Object" );
	}

	//Get the monsters
	TiXmlElement* eMon = eLevel->FirstChildElement( "Monster" );
	while( eMon != nullptr )
	{
	int num_nodes, level, type, col, row;
	eMon->Attribute( "num_nodes",		&num_nodes	);
	eMon->Attribute( "level",			&level		);
	eMon->Attribute( "type",			&type		);

	TiXmlElement* eNode = eMon->FirstChildElement( "Node" );
	//Just get first node for first sprint
	eNode->Attribute( "col", &col );
	eNode->Attribute( "row", &row );

	PlayState::GetInstance()->CreateWight(col*Pix.X, row*Pix.Y);

	eMon = eMon->NextSiblingElement( "Monster" );
	}

	//Get the portals
	TiXmlElement* ePort = eLevel->FirstChildElement( "Portal" );
	while( ePort != nullptr )
	{
	Portal port;
	ePort->Attribute( "EnterCol",		&port.enter.X	);
	ePort->Attribute( "EnterRow",		&port.enter.Y	);
	ePort->Attribute( "ExitCol",		&port.exit.X	);
	ePort->Attribute( "ExitRow",		&port.exit.Y	);
	ePort->Attribute( "PortNum",		&port.port		);

	TiXmlElement* ePMap = ePort->FirstChildElement( "Map" );
	port.map = ePMap->GetText();
	this->portals.push_back(port);
	ePort = ePort->NextSiblingElement( "Portal" );
	}
	//Get the Triggers
	TiXmlElement* eTrig = eLevel->FirstChildElement( "Trigger" );
	while( eTrig != nullptr )
	{
	Trigger trig;
	int notify, move, loop;
	eTrig->Attribute( "StartX",		&trig.start.X	);
	eTrig->Attribute( "StartY",		&trig.start.Y	);
	eTrig->Attribute( "EndX",		&trig.end.X		);
	eTrig->Attribute( "EndY",		&trig.end.Y		);
	eTrig->Attribute( "TargetX",	&trig.target.X	);
	eTrig->Attribute( "TargetY",	&trig.target.Y	);
	eTrig->Attribute( "Notify",		&notify			);
	trig.notify = (notify == 0) ? true : false;
	eTrig->Attribute( "MonsterMove",&move			);
	trig.monsterMove = (move == 0) ? true : false;
	eTrig->Attribute( "Loop",		&loop			);
	trig.loop = (loop == 0) ? true : false;

	TiXmlElement* eNote = eTrig->FirstChildElement( "Note" );
	trig.note = (!eNote->GetText()) ? "" : eNote->GetText();
	TiXmlElement* eTag = eTrig->FirstChildElement( "Tag" );
	trig.tag = (!eTag->GetText()) ? "" : eTag->GetText();

	this->triggers.push_back(trig);
	eTrig = eTrig->NextSiblingElement( "Trigger" );
	}

	//Get the shops
	TiXmlElement* eShop = eLevel->FirstChildElement( "Shop" );
	while( eShop != nullptr )
	{
	Shop_Trig temp;
	eShop->Attribute( "StartX",		&temp.start.X	);
	eShop->Attribute( "StartY",		&temp.start.Y	);
	eShop->Attribute( "EndX",		&temp.end.X		);
	eShop->Attribute( "EndY",		&temp.end.Y		);

	this->shop = temp;
	eShop = eShop->NextSiblingElement( "Shop" );
	}


	//Build the level
	TiXmlElement* eTileIndex = eLevel->FirstChildElement( "TileIndex" );

	//Loop through all the tiles
	struct Tile_Struct
	{
	int index;
	int row;
	int col;
	};

	vector<Tile_Struct> list;

	while( eTileIndex != nullptr )
	{
	Tile_Struct info;
	eTileIndex->Attribute( "id", &info.index );
	eTileIndex->Attribute("col", &info.col );
	eTileIndex->Attribute("row", &info.row );

	list.push_back(info);

	eTileIndex = eTileIndex->NextSiblingElement( "TileIndex" );
	}

	//Connect the numbered indexes to the tile pointers
	for( unsigned int y = 0; y < area.rows; y++)
	{
	for( unsigned int x = 0; x < area.cols; x++)
	{
	Tile_Data* data		= new Tile_Data();
	for(unsigned int i = 0; i < list.size(); i++)
	{
	if( list[i].col == x && list[i].row == y )
	data->tile = (*tileList)[ list[i].index ];
	}
	data->state			= NORM;
	data->time			= 0.0f;

	map.push_back( data );
	}
	}

	//Start music
	CAudioManager::GetInstance()->PlayWalk();
	*/
	return ( map.size() != 0 ) ? true : false;
}

bool Level::Rapid( string levelName, vector<Tile*>* tileList)
{
	xml_document<> doc;
	xml_node<> * root_node;
	// Read the xml file into a vector
	ifstream theFile ("Resource/XML/map.xml");
	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	// Parse the buffer using the xml file parsing library into doc 
	doc.parse<0>(&buffer[0]);
	// Find our root node
	root_node = doc.first_node("Map");

	//Get LevelList
	xml_node<> * xLevelList = root_node->first_node("LevelList");

	//Iterate over the Levels
	bool bob = false;
	for( xml_node<> * xLevel = xLevelList->first_node("Level"); xLevel && !bob; xLevel = xLevel->next_sibling("Level"))
	{
		int rows = atoi(xLevel->first_attribute("rows")->value());
		int cols = atoi(xLevel->first_attribute("cols")->value());

		xml_node<> * xName = xLevel->first_node("Name");
		xml_node<> * xBattle = xLevel->first_node("Battle");
		xml_node<> * xWalk = xLevel->first_node("Walk");

		string name = xName->value();
		string battle = xBattle->value();
		string walk = xWalk->value();

		if( strcmp( name.c_str(), levelName.c_str() ) == 0 )
		{
			this->area.cols = cols;
			this->area.rows = rows;
			CAudioManager::GetInstance()->LoadLevelAudio( battle, walk );
			bob = true;
		}
		if( !bob )
			continue;


		assert( bob != false && "Level::Load - Level name is invalid" );

		//Get the player position
		for( xml_node<> * xObject = xLevel->first_node("Object"); xObject; xObject = xObject->next_sibling("Object"))
		{
			cols = atoi(xObject->first_attribute("col")->value());
			rows = atoi(xObject->first_attribute("row")->value());
			xName = xObject->first_node("Name");
			string name = xName->value();
			if( strcmp( name.c_str(), "spawn" ) == 0 )
			{
				//Spawn player
				PlayState::GetInstance()->MovePlayer(cols*Pix.X, rows*Pix.Y);
			}
			else if( strcmp( name.c_str(), "shard" ) == 0 )
			{
				//create shard object
				PlayState::GetInstance()->CreateShard(cols*Pix.X + Pix.X/2, rows*Pix.Y + Pix.Y/2);
			}
			else if( strcmp( name.c_str(), "key" ) == 0 )
			{
				//create key
				PlayState::GetInstance()->CreateKey(cols*Pix.X + Pix.X/2, rows*Pix.Y + Pix.Y/2);
			}
		}

		//Get the monsters
		for( xml_node<> * xMon = xLevel->first_node("Monster"); xMon; xMon = xMon->next_sibling("Monster"))
		{
			int num_nodes, level, type;
			num_nodes	= atoi(xMon->first_attribute("num_nodes")->value());
			level		= atoi(xMon->first_attribute("level")->value());
			type		= atoi(xMon->first_attribute("type")->value());

			vector<Area> nodes;

			//Get the nodes
			for( xml_node<> * xNode = xMon->first_node("Node"); xNode; xNode = xNode->next_sibling("Node"))
			{
				Area a;
				a.cols = atoi(xNode->first_attribute("col")->value());
				a.rows = atoi(xNode->first_attribute("row")->value());
				nodes.push_back(a);
			}

			switch( type )
			{
			case 0://Wight
				PlayState::GetInstance()->CreateWight(nodes[0].cols*Pix.X, nodes[0].rows*Pix.Y, level); break;
			case 1://Wraith Fire
			case 2://Wraith Ice
			case 3://Wraith Earth
			case 4://Wraith Lightning
				PlayState::GetInstance()->CreateWraight(nodes[0].cols*Pix.X, nodes[0].rows*Pix.Y, level); break;
			case 5://Zombie
				PlayState::GetInstance()->CreateZombie(nodes[0].cols*Pix.X, nodes[0].rows*Pix.Y, level); break;
			case 6://Archer
				PlayState::GetInstance()->CreateArcher(nodes[0].cols*Pix.X, nodes[0].rows*Pix.Y, level); break;
			case 7://Shaman
			case 8://Boss
				PlayState::GetInstance()->CreateBoss(nodes[0].cols*Pix.X, nodes[0].rows*Pix.Y, level); break;
			}
		}
		//Get the NPC's
		for( xml_node<> * xEnt = xLevel->first_node("Entity"); xEnt; xEnt = xEnt->next_sibling("Entity"))
		{
			xml_node<> * xEName = xEnt->first_node("Name");
			string name = xEName->value();
			xml_node<> * xEText = xEnt->first_node("Text");
			string text = xEText->value();
			xml_node<> * xEItem = xEnt->first_node("Item");
			string item = xEItem->value();

			vector<Area> nodes;

			//Get the nodes
			for( xml_node<> * xNode = xEnt->first_node("Node"); xNode; xNode = xNode->next_sibling("Node"))
			{
				Area a;
				a.cols = atoi(xNode->first_attribute("col")->value());
				a.rows = atoi(xNode->first_attribute("row")->value());
				nodes.push_back(a);
			}

			//Creat entity
			PlayState::GetInstance()->CreateEnt(nodes, name, text, item);
		}

		//Get the portals
		for( xml_node<> * xPort = xLevel->first_node("Portal"); xPort; xPort = xPort->next_sibling("Portal"))
		{
			Portal port;
			port.enter.X	= atoi(xPort->first_attribute("EnterCol")->value());
			port.enter.Y	= atoi(xPort->first_attribute("EnterRow")->value());
			port.exit.X		= atoi(xPort->first_attribute("ExitCol")->value());
			port.exit.Y		= atoi(xPort->first_attribute("ExitRow")->value());
			port.port		= atoi(xPort->first_attribute("PortNum")->value());

			xName = xPort->first_node("Map");
			port.map = xName->value();

			this->portals.push_back(port);
		}

		//Get the Triggers
		for( xml_node<> * xTrig = xLevel->first_node("Trigger"); xTrig; xTrig = xTrig->next_sibling("Trigger"))
		{
			Trigger trig;
			int notify, move, loop;

			trig.start.X	= atoi(xTrig->first_attribute("StartX")->value());
			trig.start.Y	= atoi(xTrig->first_attribute("StartY")->value());
			trig.end.X		= atoi(xTrig->first_attribute("EndX")->value());
			trig.end.Y		= atoi(xTrig->first_attribute("EndY")->value());
			trig.target.X	= atoi(xTrig->first_attribute("TargetX")->value());
			trig.target.Y	= atoi(xTrig->first_attribute("TargetY")->value());
			notify			= atoi(xTrig->first_attribute("Notify")->value());
			move			= atoi(xTrig->first_attribute("MonsterMove")->value());
			loop			= atoi(xTrig->first_attribute("Loop")->value());
			trig.notify			= (notify != 0) ? true : false;
			trig.monsterMove	= (move != 0) ? true : false;
			trig.loop			= (loop != 0) ? true : false;

			xml_node<> * xNote	= xTrig->first_node("Note");
			trig.note			= xNote->value();
			xml_node<> * xTag	= xTrig->first_node("Tag");
			trig.tag			= xTag->value();

			this->triggers.push_back(trig);
		}

		//Get the shops
		for( xml_node<> * xShop = xLevel->first_node("Shop"); xShop; xShop = xShop->next_sibling("Shop"))
		{
			Shop_Trig temp;
			temp.start.X = atoi( xShop->first_attribute("StartX")->value());
			temp.start.Y = atoi( xShop->first_attribute("StartY")->value());
			temp.end.X = atoi( xShop->first_attribute("EndX")->value());
			temp.end.Y = atoi( xShop->first_attribute("EndY")->value());

			for( xml_node<> * xShopLev = xShop->first_node("Lev"); xShopLev; xShopLev = xShopLev->next_sibling("Lev"))
			{
				temp.stock.push_back( atoi( xShopLev->first_attribute("num")->value()));
			}

			this->shops.push_back(temp);
		}
		//Get the doors
		for( xml_node<> * xDoor = xLevel->first_node("Door"); xDoor; xDoor = xDoor->next_sibling("Door"))
		{
			int type;
			Area loc;
			vector<Area> nodes;

			type = atoi( xDoor->first_attribute("type")->value());
			loc.cols = atoi( xDoor->first_attribute("col")->value());
			loc.rows = atoi( xDoor->first_attribute("row")->value());

			//Get the nodes
			for( xml_node<> * xNode = xDoor->first_node("Node"); xNode; xNode = xNode->next_sibling("Node"))
			{
				Area a;
				a.cols = atoi(xNode->first_attribute("col")->value());
				a.rows = atoi(xNode->first_attribute("row")->value());
				nodes.push_back(a);
			}

			if( type == 2 )//Lightning object ptr
			{
				Door* door = PlayState::GetInstance()->CreateDoor( loc.cols * Pix.X + Pix.X / 2, loc.rows * Pix.Y + Pix.Y, type, nodes, true ); 
				for( unsigned int i = 0; i < nodes.size(); i++)
					PlayState::GetInstance()->CreatePower( nodes[i].cols * Pix.X + Pix.X / 2, nodes[i].rows * Pix.Y + Pix.Y, door );
			}
			else
			{
				PlayState::GetInstance()->CreateDoor( loc.cols * Pix.X + Pix.X / 2, loc.rows * Pix.Y + Pix.Y, type, nodes, false ); 
			}
		}

		//Build the level
		//Loop through all the tiles
		struct Tile_Struct
		{
			int index;
			int row;
			int col;
		};

		map.resize(area.cols * area.rows); 

		for( xml_node<> * xTileIndex = xLevel->first_node("TileIndex"); xTileIndex; xTileIndex = xTileIndex->next_sibling("TileIndex"))
		{
			Tile_Struct info;
			info.index = atoi(xTileIndex->first_attribute("id")->value());
			info.col = atoi(xTileIndex->first_attribute("col")->value());
			info.row = atoi(xTileIndex->first_attribute("row")->value());

			Tile_Data* data = new Tile_Data();
			data->tile = (*tileList)[info.index];
			data->state			= NORM;
			data->time			= 0.0f;
			data->center.X		= info.col * Pix.X + Pix.X / 2;
			data->center.Y		= info.row * Pix.Y + Pix.Y / 2;
			map[ info.row * area.cols + info.col ] = data;
		}

		//Start music
		CAudioManager::GetInstance()->PlayWalk();
	}

	return ( map.size() != 0 ) ? true : false;
}

bool Level::IsShadowed()
{
	if(name == "Forest" || name == "ForestDungeon" || name == "Mountain Dungeon" || name == "Desert Dungeon")
		return true;

	return false;
}