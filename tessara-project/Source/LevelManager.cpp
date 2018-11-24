/*
File:		LevelManager.cpp
Author:		Daniel Habig
Course:		SGP
Purpose:	The LevelManager class contains the list of tiles for use in the level, and contains the current level data
*/

#include "LevelManager.h"
#include "IEntity.h"
#include "Entity.h"
#include "EntityManager.h"
#include "../TinyXML/tinyxml.h"
#include "../RapidXML/rapidxml.hpp"
#include "../RapidXML/rapidxml_iterators.hpp"
#include "../RapidXML/rapidxml_print.hpp"
#include "../RapidXML/rapidxml_utils.hpp"
#include "PlayState.h"
using namespace rapidxml;
#include <ShlObj.h>
#include <cassert>
#include "LoadGameState.h"
#include "Game.h"

// SINGLETON!
/*static*/ LevelManager*	LevelManager::GetInstance( void )
{
	// Static local variable
	static LevelManager s_Instance;

	return &s_Instance;
}

LevelManager::LevelManager(void)
{
	usedTemp = false;
}

LevelManager::~LevelManager(void)
{
	for( unsigned int i = 0; i < msData.size(); i++)
	{
		if( msData[i] != nullptr)
			delete msData[i];
	}
	msData.clear();
}

bool LevelManager::Enter( void )
{
	//Load in all the tiles
	LoadTiles();


	return true;
}

bool LevelManager::Exit( void )
{
	//	Unload the level
	level.Unload();

	//	Unload the vector of tile pointers
	for( unsigned int i = 0; i < this->tileList.size(); i++ )
	{
		delete tileList[ i ];
		tileList[ i ] = nullptr;
	}
	tileList.clear();
	this->tile_Num = 0;

	return true;
}

void LevelManager::Update( float fElapsedTime )
{
	for( unsigned int i = 0; i < tileList.size(); i++ )
	{
		tileList[i]->anime.UpdateFrames(fElapsedTime);		
	}

	level.Update( fElapsedTime );
}

void LevelManager::Render( int alpha )
{
	this->level.Render(alpha);
	this->level.RenderParticles();
}

RECT LevelManager::Collision( IEntity* entity )
{
	return level.Collision(entity);
}

bool LevelManager::Collideable( int col, int row )
{
	return this->level.CollidableTile(col, row);
}


bool LevelManager::LoadTiles( void )
{
	//return Tiny();
	return Rapid();
}
bool LevelManager::Tiny( void )
{
	// Create a TinyXML Document
	TiXmlDocument doc;

	// Attempt to load
	if( doc.LoadFile( "Resource/XML/map.xml" ) == false )
		return false;

	// Access the root node
	TiXmlElement* pRoot = doc.RootElement();	// "Map"
	if( pRoot == nullptr )
		return false;

	// Iterate through the children nodes		"TileList"
	TiXmlElement* eTileList = pRoot->FirstChildElement( "TileList" );

	// Create a TileList info structure
	struct TileList
	{
		int tileSizeX;
		int tileSizeY;
		int tileSetSizeX;
		int tileSetSizeY;
		int tileNum;
	};

	TileList info = { };

	// Read attributes from the TileList node
	eTileList->Attribute( "TileWidth",		&info.tileSizeX );
	eTileList->Attribute( "TileHeight",		&info.tileSizeY );
	eTileList->Attribute( "TileSetWidth",	&info.tileSetSizeX );
	eTileList->Attribute( "TileSetHeight",	&info.tileSetSizeY);
	eTileList->Attribute( "Tile_Num",		&info.tileNum);

	//Store the info in the 'real' data members
	this->Set.cols	= info.tileSetSizeX;
	this->Set.rows	= info.tileSetSizeY;
	this->Pix.X		= info.tileSizeX;
	this->Pix.Y		= info.tileSizeY;
	this->tile_Num	= info.tileNum;

	//Iterate through the list of tiles
	TiXmlElement* eTile = eTileList->FirstChildElement( "Tile" );
	//Create a temporary struct for the xml
	struct Tile_Struct
	{
		int canBurn;
		int canIce;
		int canBreak;
		int collideEnt;
		int collideObj;
		int burning;
		int iced;
		int afterBurn;
		int afterIce;
	};
	//Create a temporary vector to store the temp struct's data
	vector<Tile_Struct> temp;

	//Loop through all of the tiles
	while( eTile != nullptr )
	{
		// Create a tile pointer
		Tile* tile = new Tile();

		//Read in the XML data to the vector of the temp struct
		Tile_Struct info;
		eTile->Attribute( "canBurn",	&info.canBurn		);
		eTile->Attribute( "canIce",		&info.canIce		);
		eTile->Attribute( "canBreak",	&info.canBreak		);
		eTile->Attribute( "collideEnt", &info.collideEnt	);
		eTile->Attribute( "collideObj", &info.collideObj	);
		eTile->Attribute( "burn",		&info.burning		);
		eTile->Attribute( "iced",		&info.iced			);
		eTile->Attribute( "afterburn",	&info.afterBurn		);
		eTile->Attribute( "aftericed",	&info.afterIce		);
		temp.push_back( info );

		//Temp struct for frame Data
		struct TFrame_Struct
		{
			int frameID;
			double time;
		};
		//Create a temporary vector to store the temp struct's data
		vector<Tile_Struct> tempFrames;

		//Get the frame info for the tile
		TiXmlElement* eFrame = eTile->FirstChildElement( "Frame" );
		//Loop through all the frames
		while( eFrame != nullptr )
		{
			TFrame_Struct tinfo;

			// Get the frameID
			tinfo.frameID = -1;
			tinfo.time = (double)0.0f;
			eFrame->Attribute( "img",	&tinfo.frameID );
			eFrame->Attribute( "time",	&tinfo.time );

			// Add the frame data to the tile's animation struct
			TFrame frame;
			frame.frameID = tinfo.frameID;
			frame.time = (float)tinfo.time;
			tile->anime.frame.push_back(frame);

			// Move to the next Frame
			eFrame = eFrame->NextSiblingElement( "Frame" );
		}

		// Set the tile's data
		tile->anime.currFrame = 0;
		tile->canBurn		= ( info.canBurn == 1 )		? true : false;
		tile->canIce		= ( info.canIce == 1 )		? true : false;
		tile->canBreak		= ( info.canBreak == 1 )	? true : false;
		tile->collideEnt	= ( info.collideEnt == 1 )	? true : false;
		tile->collideObj	= ( info.collideObj == 1 )	? true : false;
		tile->burning		= nullptr;
		tile->iced			= nullptr;
		tile->afterBurn		= nullptr;
		tile->afterIce		= nullptr;
		tile->afterBreak	= nullptr;
		this->tileList.push_back( tile );

		//Set the countDown for the tile
		tile->anime.countDown = tile->anime.frame[tile->anime.currFrame].time;

		// Move to the next Tile
		eTile = eTile->NextSiblingElement( "Tile" );
	}

	//Assign the tile pointers with the now filled vector of tiles
	for( unsigned int i = 0; i < this->tileList.size(); i++ )
	{
		this->tileList[i]->burning			= (temp[i].burning != -1)		? this->tileList[	temp[i].burning		] :	nullptr;
		this->tileList[i]->iced				= (temp[i].iced != -1)			? this->tileList[	temp[i].iced		] :	nullptr;
		this->tileList[i]->afterBurn		= (temp[i].afterBurn != -1)		? this->tileList[	temp[i].afterBurn	] :	nullptr;
		this->tileList[i]->afterIce			= (temp[i].afterIce != -1)		? this->tileList[	temp[i].afterIce	] :	nullptr;
	}

	return (this->tileList.size() != 0) ? true : false;
}
bool LevelManager::Rapid( void )
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
	xml_node<> * xTileList = root_node->first_node("TileList");

	//Store the info in the 'real' data members
	this->Set.cols	= atoi(xTileList->first_attribute("TileSetWidth")->value());
	this->Set.rows	= atoi(xTileList->first_attribute("TileSetHeight")->value());
	this->Pix.X		= atoi(xTileList->first_attribute("TileWidth")->value());
	this->Pix.Y		= atoi(xTileList->first_attribute("TileHeight")->value());

	//Create a temporary struct for the xml
	struct Tile_Struct
	{
		int canBurn;
		int canIce;
		int canBreak;
		int collideEnt;
		int collideObj;
		int burning;
		int iced;
		int afterBurn;
		int afterIce;
		int afterBreak;
		string name;
	};
	//Create a temporary vector to store the temp struct's data
	vector<Tile_Struct> temp;

	//Go over tiles
	for( xml_node<> * xTile = xTileList->first_node("Tile"); xTile; xTile = xTile->next_sibling("Tile"))
	{
		// Create a tile pointer
		Tile* tile = new Tile();

		Tile_Struct info;
		info.afterBreak	= 0;
		info.afterBurn	= atoi(xTile->first_attribute("afterburn")->value());
		info.afterIce	= atoi(xTile->first_attribute("aftericed")->value());
		info.burning	= atoi(xTile->first_attribute("burn")->value());
		info.iced		= atoi(xTile->first_attribute("iced")->value());
		info.collideEnt = atoi(xTile->first_attribute("collideEnt")->value());
		info.collideObj	= atoi(xTile->first_attribute("collideObj")->value());
		info.canBreak	= atoi(xTile->first_attribute("canBreak")->value());
		info.canIce		= atoi(xTile->first_attribute("canIce")->value());
		info.canBurn	= atoi(xTile->first_attribute("canBurn")->value());

		xml_node<> * xName = xTile->first_node("Name"); 
		info.name = xName->value();

		temp.push_back(info);

		//Temp struct for frame Data
		struct TFrame_Struct
		{
			int frameID;
			double time;
		};
		//Create a temporary vector to store the temp struct's data
		vector<Tile_Struct> tempFrames;

		//Go through the frames
		for( xml_node<> * xFrame = xTile->first_node("Frame"); xFrame; xFrame = xFrame->next_sibling("Frame"))
		{
			TFrame_Struct tinfo;

			// Get the frameID
			tinfo.frameID	= -1;
			tinfo.time		= (double)0.0f;
			tinfo.frameID	= atoi( xFrame->first_attribute("img")->value());
			tinfo.time		= atof( xFrame->first_attribute("time")->value());

			// Add the frame data to the tile's animation struct
			TFrame frame;
			frame.frameID = tinfo.frameID;
			frame.time = (float)tinfo.time;
			tile->anime.frame.push_back(frame);
		}

		// Set the tile's data
		tile->anime.currFrame = 0;
		tile->canBurn		= ( info.canBurn == 1 )		? true : false;
		tile->canIce		= ( info.canIce == 1 )		? true : false;
		tile->canBreak		= ( info.canBreak == 1 )	? true : false;
		tile->collideEnt	= ( info.collideEnt == 1 )	? true : false;
		tile->collideObj	= ( info.collideObj == 1 )	? true : false;
		tile->burning		= nullptr;
		tile->iced			= nullptr;
		tile->afterBurn		= nullptr;
		tile->afterIce		= nullptr;
		tile->afterBreak	= nullptr;
		tile->name			= info.name;
		this->tileList.push_back( tile );

		//Set the countDown for the tile
		tile->anime.countDown = tile->anime.frame[tile->anime.currFrame].time;

	}

	//Assign the tile pointers with the now filled vector of tiles
	for( unsigned int i = 0; i < this->tileList.size(); i++ )
	{
		this->tileList[i]->burning			= (temp[i].burning != -1)		? this->tileList[	temp[i].burning		] :	nullptr;
		this->tileList[i]->iced				= (temp[i].iced != -1)			? this->tileList[	temp[i].iced		] :	nullptr;
		this->tileList[i]->afterBurn		= (temp[i].afterBurn != -1)		? this->tileList[	temp[i].afterBurn	] :	nullptr;
		this->tileList[i]->afterIce			= (temp[i].afterIce != -1)		? this->tileList[	temp[i].afterIce	] :	nullptr;
		this->tileList[i]->afterBreak		= (temp[i].afterBreak != -1)	? this->tileList[	temp[i].afterBreak	] : nullptr;
	}

	return (this->tileList.size() != 0) ? true : false;
}


bool LevelManager::LoadLevel( string levelName )
{
	this->levelName = levelName;
	this->level.Unload();
	bool asdf = this->level.Load( levelName, &tileList, this->Set, this->Pix );
	//Get saved data
	AdjustLevel(levelName);

	return asdf;
	//send event to spawn player, entities, and monsters
}

void LevelManager::LoadMapData(int slot)
{
	for( unsigned int i = 0; i < msData.size(); i++)
	{
		if( msData[i] != nullptr)
			delete msData[i];
	}
	msData.clear();

	string szMapName;

	char path[MAX_PATH];
	LPWSTR wszPath = NULL;

	HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0,  0, &wszPath);

	size_t tempSize;
	wcstombs_s(&tempSize, path, wszPath, MAX_PATH);

	string pathtowrite = path;
	pathtowrite += "\\Shards of Tessera";
	TCHAR temp[MAX_PATH];

	CSTR_TO_WSTR(temp, MAX_PATH, pathtowrite.c_str());
	CreateDirectory(temp, 0);

	string FilePath;

	FilePath = pathtowrite;

	switch ( slot )
	{
	case 0: // Load Blank
		FilePath = "Resource\\XML\\MapSaveBlank.xml"; break;
	case 1:	// Load Slot 1
		FilePath += "\\MapSave1.xml"; break;
	case 2:	// Load Slot 2
		FilePath += "\\MapSave2.xml"; break;
	case 3:	// Load Slot 3
		FilePath += "\\MapSave3.xml"; break;
	}

	xml_document<> doc;
	xml_node<> * root_node;
	// Read the xml file into a vector
	ifstream theFile (FilePath.c_str());
	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	// Parse the buffer using the xml file parsing library into doc 
	doc.parse<0>(&buffer[0]);
	// Find our root node
	root_node = doc.first_node("MapSave");

	//Get LevelList
	xml_node<> * xLevelList = root_node->first_node("LevelList");


	//Go through each level
	for( xml_node<> * xLevel = xLevelList->first_node("Level"); xLevel; xLevel = xLevel->next_sibling("Level"))
	{
		MapSaveData*	data = new MapSaveData();

		xml_node<> * xName = xLevel->first_node("Name");
		data->used	= (atoi(xName->first_attribute("used")->value()) == 1) ? true : false;
		data->mapname = xName->value();

		//Go through each Object
		for( xml_node<> * xObj = xLevel->first_node("Object"); xObj; xObj = xObj->next_sibling("Object"))
		{
			MapObject	obj;
			obj.row				= atoi(xObj->first_attribute("row")->value());
			obj.col				= atoi(xObj->first_attribute("col")->value());
			obj.got				= (atoi(xObj->first_attribute("got")->value()) == 1) ? true : false;
			xml_node<> * xOName = xObj->first_node("Name");
			obj.name			= xOName->value();
			data->ObjectsPicked.push_back( obj );
		}
		//Go through each Trigger
		for( xml_node<> * xTrig = xLevel->first_node("Trigger"); xTrig; xTrig = xTrig->next_sibling("Trigger"))
		{
			MapTrigger			trig;
			trig.activated		= bool(atoi(xTrig->first_attribute("activated")->value()) ==1) ? true : false;

			data->TriggersUsed.push_back( trig );
		}
		//Go through each NPC
		for( xml_node<> * xEnt = xLevel->first_node("Entity"); xEnt; xEnt = xEnt->next_sibling("Entity"))
		{
			MapEnt		ent;
			ent.given		= bool(atoi(xEnt->first_attribute("given")->value()) == 1) ? true : false;
			xml_node<> * xEName = xEnt->first_node("Name");
			xml_node<> * xEText = xEnt->first_node("Text");

			ent.name = xEName->value();
			ent.text = xEText->value();

			data->NpcGiven.push_back( ent );
		}
		//Go through each Door
		for( xml_node<> * xDoor = xLevel->first_node("Door"); xDoor; xDoor = xDoor->next_sibling("Door"))
		{
			MapDoor		door;
			door.row	= atoi(xDoor->first_attribute("row")->value());
			door.col	= atoi(xDoor->first_attribute("col")->value());
			door.open	= (atoi(xDoor->first_attribute("open")->value()) == 1) ? true : false;
			data->DoorsOpened.push_back( door );
		}
		this->msData.push_back( data );
	}
}


void LevelManager::DelObj(int posX, int posY, string type)
{
	int index = -1;
	for(unsigned int i = 0; i < msData.size(); i++)
	{
		if( strcmp( msData[i]->mapname.c_str(), this->levelName.c_str() ) == 0 )
		{
			index = i; 
			break;
		}
	}
	assert( index != -1 && "LevelManager::AdjustLevel - Level name is invalid" );

	MapSaveData* ptr = msData[index];

	//Go through the objects
	for( unsigned int i = 0; i < ptr->ObjectsPicked.size(); i++)
	{
		string name = ptr->ObjectsPicked[i].name;
		int locX = ptr->ObjectsPicked[i].col * Pix.X + int(Pix.X * 0.5f);
		int locY = ptr->ObjectsPicked[i].row * Pix.Y + int(Pix.Y * 0.5f);

		if (  (strcmp( name.c_str(), type.c_str() ) == 0)  &&  (locX == posX)  &&  (locY == posY)  )
		{
			ptr->ObjectsPicked[i].got = true;
			break;
		}
	}

}
void LevelManager::Given(string name, string text)
{
	int index = -1;
	for(unsigned int i = 0; i < msData.size(); i++)
	{
		if( strcmp( msData[i]->mapname.c_str(), this->levelName.c_str() ) == 0 )
		{
			index = i; 
			break;
		}
	}
	assert( index != -1 && "LevelManager::AdjustLevel - Level name is invalid" );

	MapSaveData* ptr = msData[index];

	//Go through the NPC's
	for( unsigned int i = 0; i < ptr->NpcGiven.size(); i++)
	{
		string m_name = ptr->NpcGiven[i].name;
		string m_text = ptr->NpcGiven[i].text;
		if( strcmp( m_name.c_str(), name.c_str() ) == 0 && strcmp( m_text.c_str(), text.c_str() ) == 0 )
		{
			ptr->NpcGiven[i].given = true;
			break;
		}
	}

}
void LevelManager::Open(int posX, int posY )
{
	int index = -1;
	for(unsigned int i = 0; i < msData.size(); i++)
	{
		if( strcmp( msData[i]->mapname.c_str(), this->levelName.c_str() ) == 0 )
		{
			index = i; 
			break;
		}
	}
	assert( index != -1 && "LevelManager::AdjustLevel - Level name is invalid" );

	MapSaveData* ptr = msData[index];

	//Go through the Doors
	for( unsigned int i = 0; i < ptr->DoorsOpened.size(); i++)
	{
		int locX = ptr->DoorsOpened[i].col * Pix.X + int(Pix.X * 0.5f);
		int locY = ptr->DoorsOpened[i].row * Pix.Y + Pix.Y;

		if( locX == posX && locY == posY )
		{
			ptr->DoorsOpened[i].open = true;
			break;
		}
	}
}
void LevelManager::HitTrig(int index)
{
	int val = -1;
	for(unsigned int i = 0; i < msData.size(); i++)
	{
		if( strcmp( msData[i]->mapname.c_str(), this->levelName.c_str() ) == 0 )
		{
			val = i; 
			break;
		}
	}
	assert( val != -1 && "LevelManager::AdjustLevel - Level name is invalid" );

	MapSaveData* ptr = msData[val];

	//Hit the Trigger
	ptr->TriggersUsed[index].activated = true;

}
void LevelManager::AdjustLevel	(string levelName)
{
	//Find the level in the data and see if it has been used yet

	//Get the slot to use
	int index = -1;
	for(unsigned int i = 0; i < msData.size(); i++)
	{
		if( strcmp( msData[i]->mapname.c_str(), levelName.c_str() ) == 0 )
		{
			index = i; 
			break;
		}
	}
	assert( index != -1 && "LevelManager::AdjustLevel - Level name is invalid" );

	//get rid of the things in the level that shouldn't be there.
	MapSaveData* ptr = msData[index];

	if( !ptr->used )	//if the map has not yet been visited
	{
		ptr->used = true;
		return;
	}

	//Go through the objects
	unsigned int offset = 0;
	for( unsigned int i = 0; i < ptr->ObjectsPicked.size(); i++)
	{
		if( ptr->ObjectsPicked[i].got == true )
		{
			CEntityManager::GetInstance()->RemoveTypeAtIndex(ENT_ITEM, i-offset);
			//if( i-offset == 0 )
				offset++;
		}
	}

	//Go through the NPC's
	for( unsigned int i = 0; i < ptr->NpcGiven.size(); i++)
	{
		if( ptr->NpcGiven[i].given == true )
			CEntityManager::GetInstance()->Given(i);
	}

	//Go through the Doors
	for( unsigned int i = 0; i < ptr->DoorsOpened.size(); i++)
	{
		if( ptr->DoorsOpened[i].open == true )
			CEntityManager::GetInstance()->Open(i);
	}

	//Go through the Triggers
	for( unsigned int i = 0; i < ptr->TriggersUsed.size(); i++)
	{
		if( ptr->TriggersUsed[i].activated == true )
			this->level.HitTrigger(i);
	}
}

void LevelManager::SaveTemp(int slot)
{
	//Save out the temp data
	char path[MAX_PATH];
	LPWSTR wszPath = NULL;

	HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0,  0, &wszPath);

	size_t tempSize;
	wcstombs_s(&tempSize, path, wszPath, MAX_PATH);

	string pathtowrite = path;
	pathtowrite += "\\Shards of Tessera";
	TCHAR temp[MAX_PATH];

	CSTR_TO_WSTR(temp, MAX_PATH, pathtowrite.c_str());
	CreateDirectory(temp, 0);

	string FilePath;

	FilePath = pathtowrite;

	switch ( slot )
	{
	case 1:	// Load Slot 1
		FilePath += "\\MapSave1.xml"; break;
	case 2:	// Load Slot 2
		FilePath += "\\MapSave2.xml"; break;
	case 3:	// Load Slot 3
		FilePath += "\\MapSave3.xml"; break;
	}

	xml_document<> doc;
	xml_node<>* decl = doc.allocate_node(node_declaration);
	decl->append_attribute(doc.allocate_attribute("version", "1.0"));
	decl->append_attribute(doc.allocate_attribute("encoding", "UTF-8"));
	doc.append_node(decl);  

	xml_node<> *xRoot = doc.allocate_node(node_element, "MapSave");
	doc.append_node(xRoot);
	//xml_attribute<> *attr = doc.allocate_attribute("dummy", "google.com");
	// files->append_attribute(attr);

	xml_node<> *xLevelList = doc.allocate_node(node_element, "LevelList");
	xRoot->append_node(xLevelList);

	string word;

	//For each Level
	for( unsigned int i = 0; i < msData.size(); i++ )
	{
		MapSaveData* ptr = msData[i];
		//Create Level Node
		xml_node<> *xLevel = doc.allocate_node(node_element, "Level");
		xLevelList->append_node(xLevel);
		//Create Name Node
		xml_node<> *xName = doc.allocate_node(node_element, "Name", ptr->mapname.c_str());
		xLevel->append_node(xName);
		//Set Name's attributes
		/*xml_attribute<> *xUsed = nullptr;
		if( ptr->used )
		xUsed = doc.allocate_attribute("used", "1");
		else
		xUsed = doc.allocate_attribute("used", "0");*/

		word = (ptr->used) ? "1" : "0";
		char *node_name = doc.allocate_string(word.c_str());					// Allocate string and copy name into it
		xml_attribute<> *xUsed = doc.allocate_attribute("used", node_name);
		xName->append_attribute(xUsed);

		//For Each Object
		for( unsigned int j = 0; j < ptr->ObjectsPicked.size(); j++ )
		{
			ostringstream oss;
			xml_node<> *xObject = doc.allocate_node(node_element, "Object");
			xLevel->append_node(xObject);

			//col
			oss.str("");
			oss << ptr->ObjectsPicked[j].col;
			word = oss.str().c_str();
			char *node_ocol = doc.allocate_string(word.c_str());					// Allocate string and copy name into it
			xml_attribute<> *xCol = doc.allocate_attribute("col", node_ocol);
			xObject->append_attribute(xCol);

			//row
			oss.str("");
			oss << ptr->ObjectsPicked[j].row;
			word = oss.str().c_str();
			char *node_orow = doc.allocate_string(word.c_str());					// Allocate string and copy name into it
			xml_attribute<> *xRow = doc.allocate_attribute("row", node_orow);
			xObject->append_attribute(xRow);

			//got
			word = (ptr->ObjectsPicked[j].got) ? "1" : "0";
			char *node_got = doc.allocate_string(word.c_str());					// Allocate string and copy name into it
			xml_attribute<> *xGot = doc.allocate_attribute("got", node_got);
			xObject->append_attribute(xGot);

			//name
			xml_node<> *xOName = doc.allocate_node(node_element, "Name", ptr->ObjectsPicked[j].name.c_str());
			xObject->append_node(xOName);
		}

		//For Each Trigger
		for( unsigned int j = 0; j < ptr->TriggersUsed.size(); j++ )
		{
			xml_node<> *xTrigger = doc.allocate_node(node_element, "Trigger");
			xLevel->append_node(xTrigger);

			word = (ptr->TriggersUsed[j].activated) ? "1" : "0";
			char *node_bool = doc.allocate_string(word.c_str());					// Allocate string and copy name into it
			xml_attribute<> *xActive = doc.allocate_attribute("activated", node_bool);
			xTrigger->append_attribute(xActive);
		}

		//For Each NPC
		for( unsigned int j = 0; j < ptr->NpcGiven.size(); j++ )
		{
			xml_node<> *xEnt = doc.allocate_node(node_element, "Entity");
			xLevel->append_node(xEnt);

			//Given
			word = (ptr->NpcGiven[j].given) ? "1" : "0";
			char *node_bool = doc.allocate_string(word.c_str());					// Allocate string and copy name into it
			xml_attribute<> *xActive = doc.allocate_attribute("given", node_bool);
			xEnt->append_attribute(xActive);

			//Name
			xml_node<> *xEName = doc.allocate_node(node_element, "Name", ptr->NpcGiven[j].name.c_str());
			xEnt->append_node(xEName);
			//Text
			xml_node<> *xEText = doc.allocate_node(node_element, "Text", ptr->NpcGiven[j].text.c_str());
			xEnt->append_node(xEText);
		}

		//For Each Door
		for( unsigned int j = 0; j < ptr->DoorsOpened.size(); j++ )
		{
			ostringstream oss;

			xml_node<> *xDoor = doc.allocate_node(node_element, "Door");
			xLevel->append_node(xDoor);

			//col
			oss.str("");
			oss << ptr->DoorsOpened[j].col;
			word = oss.str().c_str();
			char *node_dcol = doc.allocate_string(word.c_str());					// Allocate string and copy name into it
			xml_attribute<> *xCol = doc.allocate_attribute("col", node_dcol);
			xDoor->append_attribute(xCol);
			//row
			oss.str("");
			oss << ptr->DoorsOpened[j].row;
			word = oss.str().c_str();
			char *node_orow = doc.allocate_string(word.c_str());					// Allocate string and copy name into it
			xml_attribute<> *xRow = doc.allocate_attribute("row", node_orow);
			xDoor->append_attribute(xRow);
			//open
			word = (ptr->DoorsOpened[j].open) ? "1" : "0";
			char *node_bool = doc.allocate_string(word.c_str());					// Allocate string and copy name into it
			xml_attribute<> *xActive = doc.allocate_attribute("open", node_bool);
			xDoor->append_attribute(xActive);

		}
	}

	std::ofstream myfile;
	myfile.open (FilePath);
	myfile << doc;

}


//Entity accessors
bool LevelManager::OnShop(IEntity* ent)
{
	return level.OnShop(ent);
}
bool LevelManager::OnFire(IEntity* ent)
{
	return this->level.OnFire(ent);
}
bool LevelManager::OnIce(IEntity* ent)
{
	return this->level.OnIce(ent);
}
void LevelManager::Break(IEntity* ent)
{
	this->level.Break(ent);
}
void LevelManager::IceWall(IEntity* ent)
{
	this->level.IceWall(ent);
}
void LevelManager::FireDash(IEntity* ent)
{
	this->level.FireDash(ent);
}