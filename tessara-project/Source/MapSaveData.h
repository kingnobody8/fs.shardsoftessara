/*
	File:		MapSaveData.h
	Author:		Daniel Habig
	Course:		SGP
	Purpose:	This struct holds onto the the data that will be used for saving and loading maps
*/

#pragma once
#include <vector>
using namespace std;

struct MapObject
{
	string name;
	int col;
	int row;
	bool got;
};
struct MapTrigger
{
	bool activated;
};
struct MapEnt
{
	bool given;
	string name, text;
};
struct MapDoor
{
	bool open;
	int col, row;
};


struct MapSaveData
{
	string mapname;
	bool used;
	vector<MapObject>	ObjectsPicked;
	vector<MapTrigger>	TriggersUsed;
	vector<MapEnt>		NpcGiven;
	vector<MapDoor>		DoorsOpened;
};