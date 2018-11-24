/*
	File:		PathFinder.h
	Author:		Mark Diaz
	Purpose:	The CPathFinder class enables an entity to populate a stack
				of waypoints avoiding obstacles from a position to any end 
				position 
*/

#pragma once

#include <list>
#include <stack>
#include <Windows.h>
using namespace std;

class CPathFinder
{
public:

	CPathFinder(void);
	virtual ~CPathFinder(void);

	bool FindPath( POINT Start, POINT Goal );
	
private:
	
	struct SearchNode
	{
		int X;
		int Y;
		int FCost;
		int GCost;
		int HCost;

		SearchNode* Parent;
		void CalculateFCost( SearchNode* pEnd );
		bool operator== ( SearchNode& pNode )
		{ return (this->X == pNode.X && this->Y == pNode.Y); }
	};

	list<SearchNode *> OpenList;
	list<SearchNode *> ClosedList;
	
	// Helper assets and functions
	SearchNode* Begin;
	SearchNode* End;
	int CurrentLevelRows;
	int CurrentLevelColumns;
	void ClearLists( void );
	bool AddNewEdges( SearchNode* pNode );
	void PopulateWaypoints( void );
	bool PathFind( void );

protected:

	stack<POINT> Waypoints;
};

