/*
File:		LevelManager.h
Author:		Daniel Habig
Course:		SGP
Purpose:	The LevelManager class contains the list of tiles for use in the level, and contains the current level data
*/

#pragma once
#include "Level.h"
#include "Tile.h"
#include "MapSaveData.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/SGD_Math.h"
#include <vector>
using namespace std;

//	Forward Declarations
class IEntity;

class LevelManager
{
public:
	// SINGLETON!
	static LevelManager*	GetInstance( void );
	//--------------------------------------------------------------------------------------------------

	bool Enter			( void );	// Load Resources
	bool Exit			( void );	// UnLoad Resources

	bool LoadTiles		( void );
	bool LoadLevel		( string levelName );
	void LoadMapData	(int slot);

	void DelObj			(int posX, int posY, string type);
	void Given			(string name, string text);
	void Open			(int posX, int posY );
	void HitTrig		(int index);
	void AdjustLevel	( string levelName );
	void SaveTemp		( int slot );	

	bool Tiny			( void );
	bool Rapid			( void );

	void Update			( float fElapsedTime );
	void Render			( int alpha );
	RECT Collision		( IEntity* entity );

	Pair GetPair		(void) { return Pix; }
	Portal GetPortal	(int i) { return this->level.GetPortal(i); }

	string levelName;

	Level* GetLevel() { return  &this->level;}
	bool Collideable( int col, int row );

	bool OnShop(IEntity* ent);
	bool OnFire(IEntity* ent);
	bool OnIce(IEntity* ent);
	void Break(IEntity* ent);
	void IceWall(IEntity* ent);
	void FireDash(IEntity* ent);
	string GetLevelName(void) { return this->level.GetLevelName(); }
	vector<int> ShopStock(void) { return this->level.ShopStock(); }


private:
	// SINGLETON!
	LevelManager( void );								// ctor
	virtual ~LevelManager( void );						// dtor
	LevelManager( const LevelManager& );				// copy ctor
	LevelManager& operator= ( const LevelManager& );	// assignment op

	//	List of tiles
	unsigned int	tile_Num;		// The number of tiles available
	vector<Tile*>	tileList;		// The vector of tiles
	Area			Set;			// The height and width of the tileSet img (rows and columns)
	Pair			Pix;			// The height and width of the individual tiles (pixels)

	//	Current Level
	Level			level;

	bool			usedTemp;
	vector<MapSaveData*>	msData;


	unsigned int spawnPortal;



};

