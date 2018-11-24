/*
File:		level.h
Author:		Daniel Habig
Course:		SGP
Purpose:	The level class contains all tiles in a level and performs updates on the tiles.
*/

#pragma once
#include "Tile.h"
#include "../SGD Wrappers/SGD_String.h"
#include "Projectile.h"
#include "Camera.h"
#include "ParticleEffect.h"
#include "ParticleManager.h"
#include <Windows.h>
using namespace std;

#define CTS 10

//	Forward Class Declaration
class IEntity;


struct Area
{
	unsigned int cols;
	unsigned int rows;
};
struct Pair
{
	int X;
	int Y;
};

struct Portal
{
	Pair enter;
	Pair exit;
	string map;
	int port;
};

struct Trigger
{
	Pair start, end, target;
	string note, tag;
	bool loop, monsterMove, notify, used;

	Trigger()
	{
		used = false;
	}
};

struct Shop_Trig
{
	Shop_Trig()
	{
		start.X = start.Y = end.X = end.Y = 0;
	}
	Pair start, end;
	vector<int> stock;
};

enum TileState { NORM, ICED, BURN };

struct Tile_Data
{
	// Base tile info
	Tile* tile;
	// This particular tile's current state
	TileState state;
	// This particular tile's current time in that state
	float time;
	bool Breaking;
	Pair center;


	CParticleEffect* TileEffect;

	Tile_Data()
	{
		Breaking = false;
		TileEffect = nullptr;
	}

	~Tile_Data()
	{
		if (TileEffect != nullptr)
			delete TileEffect;
		TileEffect = nullptr;
	}

	void StartFireEffect()
	{
		if (TileEffect != nullptr)
			delete TileEffect;

		TileEffect = new CParticleEffect;
		CParticleManager::GetInstance()->GetEffect( *TileEffect, FX_TILEFIRE );
		TileEffect->ToggleCamera();
		TileEffect->SetXPosition( center.X );
		TileEffect->SetYPosition( center.Y );
		TileEffect->Initialize();
	}

	void StartIceEffect()
	{
		if (TileEffect != nullptr)
			delete TileEffect;

		TileEffect = new CParticleEffect;
		CParticleManager::GetInstance()->GetEffect( *TileEffect, FX_TILEICE );
		TileEffect->ToggleCamera();
		TileEffect->SetXPosition( center.X );
		TileEffect->SetYPosition( center.Y );
		TileEffect->Initialize();
	}

	bool Update(float fElapsedTime)
	{
		this->time -= fElapsedTime;
		if (Breaking)
		{
			TileEffect->Update( fElapsedTime );
			if (TileEffect->IsDead())
			{
				delete TileEffect;
				TileEffect = nullptr;
				Breaking = false;
			}
		}
		switch(state)
		{
		case NORM:
			break;
		case BURN:
			{
				if (TileEffect != nullptr)
					TileEffect->Update( fElapsedTime );

				if( time <= 0 )
				{
					tile = tile->afterBurn;
					delete TileEffect;
					TileEffect = nullptr;
					state = NORM;
				}
				else
					return true;
				break;
			}
		case ICED:
			{
				if (TileEffect != nullptr)
					TileEffect->Update( fElapsedTime );
			}

			break;
		}
		return false;
	}

	void FireSpread(Tile_Data* up, Tile_Data* down, Tile_Data* left, Tile_Data* right)
	{
		//Up
		long r = rand() % 100000;
		if( r < CTS && up != nullptr  )
			up->BeginBurn();
		//Down
		r = rand() % 100000;
		if( r < CTS && down!= nullptr  )
			down->BeginBurn();
		//Left
		r = rand() % 100000;
		if( r < CTS && left != nullptr  )
			left->BeginBurn();
		//Right
		r = rand() % 100000;
		if( r < CTS && right != nullptr  )
			right->BeginBurn();
	}

	void BeginBurn()
	{
		if( state == NORM && tile->canBurn)
		{
			time = 5.0f;
			state = BURN;
			StartFireEffect();
		}
		else if( state == ICED )
		{
			tile = tile->afterIce;
			state = NORM;
		}
	}
	void BeginIce()
	{
		if( state == NORM && tile->canIce)
		{
			state = ICED;
			StartIceEffect();
		}
		else if ( state == BURN )
		{
			tile = tile->afterBurn;
			state = NORM;
			delete TileEffect;
			TileEffect = nullptr;
		}
	}
	void Break()
	{
		if( this->tile->canBreak)
		{
			if (TileEffect != nullptr)
				delete TileEffect;
			TileEffect = new CParticleEffect;
			CParticleManager::GetInstance()->GetEffect( *TileEffect, FX_ROCKBREAK );
			TileEffect->ToggleCamera();
			TileEffect->SetXPosition( center.X );
			TileEffect->SetYPosition( center.Y );
			TileEffect->Initialize();
			Breaking = true;
			this->tile = this->tile->afterBreak;
		}
	}

	bool CollideEnt()
	{
		if( state == ICED )
			return tile->iced->collideEnt;
		return tile->collideEnt;
	}
	bool CollideObj()
	{
		if( state == ICED )
			return tile->iced->collideObj;
		return tile->collideObj;
	}

};

class Level
{
public:
	Level			(void);
	~Level			(void);

	void Update		( float fElapsedTime );
	void Render		( int alpha );
	void RenderParticles	( void );
	RECT Collision	( IEntity* entity );
	void HitTrigger (int index);
	void TriggerResponse(Trigger* trig);
	void PortalResponse(Portal* port);
	void ProjectileResponse(CProjectile* proj, int index);
	bool Load		( string mapName, vector<Tile*>* tileList, Area nSet, Pair nPix );
	bool Unload		( void );
	bool Tiny		( string mapName, vector<Tile*>* tileList);
	bool Rapid		( string mapName, vector<Tile*>* tileList);

	bool IntersectTrigger( RECT rect, Trigger* trig );
	bool IntersectLine(Pair alpha, Pair beta, Pair charlie, Pair delta );

	bool OnShop(IEntity* ent);
	bool OnFire(IEntity* ent);
	bool OnIce(IEntity* ent);
	void Break(IEntity* ent);
	void IceWall(IEntity* ent);
	void FireDash(IEntity* ent);
	vector<int> ShopStock(void) { return shops[0].stock;}
	bool IsShadowed();


	int GetColumns() { return area.cols; }
	int GetRows()    { return area.rows; }
	Area GetMatrix ( void )
	{
		return this->area;
	}
	vector<Tile_Data*> GetMap(void)
	{
		return map;
	}
	Portal GetPortal(int i )
	{
		return portals[i];
	}
	bool CollidableTile( int x, int y);

	string GetLevelName(void) {return this->name; }

private:
	vector<Tile_Data*>		map;			// Will be filled with the tiles of the map. 1D array representing a 2D matrix
	vector<Trigger>			triggers;		// List of triggers for this level
	vector<Portal>			portals;		// List of portals for the level
	vector<Shop_Trig>		shops;

	Area					area;			// The height and width of the map as a whole (rows and columns)
	Area					Set;			// The height and width of the tileSet img (rows and columns)
	Pair					Pix;			// The height and width of the individual tiles (pixels)
	string					name;			// The level's name
};