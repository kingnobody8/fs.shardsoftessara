/*
File:		Tile.h
Author:		Daniel Habig
Course:		SGP
Purpose:	The tile class holds the data for each tile in a level
*/

#pragma once
#include <vector>
using namespace std;

struct TFrame
{
	int frameID;
	float time;
};

struct TAnime
{
	float countDown;
	unsigned int currFrame;
	vector<TFrame> frame;

	unsigned int GetCurrFrame (void)
	{
		return frame[currFrame].frameID;
	}
	void UpdateFrames( float fElapsedTime )
	{
		countDown -= fElapsedTime;
		if( countDown <= 0)
		{
			currFrame++;
			if(currFrame >= frame.size())
				currFrame = 0;
			countDown = frame[currFrame].time;
		}
	}

};

struct Tile
{
	bool canBurn;
	bool canIce;
	bool canBreak;
	bool collideEnt;
	bool collideObj;

	TAnime anime;

	Tile* iced;
	Tile* burning;
	Tile* afterBurn;
	Tile* afterIce;
	Tile* afterBreak;

	string name;

};

