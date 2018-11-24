/*
	File:		PathFinder.cpp
	Author:		Mark Diaz
	Purpose:	The CPathFinder class enables an entity to populate a stack
				of waypoints avoiding obstacles from a position to any end 
				position 
*/

#include "PathFinder.h"
#include "LevelManager.h"
#include "Level.h"

CPathFinder::CPathFinder(void)
{
	Begin = nullptr;
	End = nullptr;
}


CPathFinder::~CPathFinder(void)
{
	ClearLists();
}

void CPathFinder::ClearLists( void )
{
	delete End;
	End = nullptr;

	list<SearchNode *>::iterator iter = OpenList.begin();
	while (iter != OpenList.end())
	{
		delete *iter;
		*iter = nullptr;
		++iter;
	}
	OpenList.clear();

	iter = ClosedList.begin();
	while (iter != ClosedList.end())
	{
		delete *iter;
		*iter = nullptr;
		++iter;
	}
	ClosedList.clear();

}

bool CPathFinder::FindPath( POINT Start, POINT Goal )
{
	// First clear all lists
	ClearLists();

	// Grab the current level's size
	CurrentLevelRows = LevelManager::GetInstance()->GetLevel()->GetRows();
	CurrentLevelColumns = LevelManager::GetInstance()->GetLevel()->GetColumns();

	// Create the starting searchnode and put it into the Open list
	Begin = new SearchNode;
	Begin->X = Start.x / 64;
	Begin->Y = Start.y / 64;
	Begin->GCost = 0;
	Begin->Parent = nullptr;

	End = new SearchNode;
	End->X = Goal.x / 64;
	End->Y = Goal.y / 64;
	
	// Push the starting node onto the closed list
	ClosedList.push_back(Begin);

	// Find the starting node's edges and add them to the open list
	if (AddNewEdges(Begin)) // if true then End is an edge of begin
	{
		PopulateWaypoints();
		return true;
	}
	else
		return PathFind(); // Initiate pathfinding

}

bool CPathFinder::AddNewEdges( SearchNode* pNode )
{
	int Edges = 4;
	while (Edges != 0)
	{
		// Create a Searchnode to hold the possible edge
		SearchNode* Possible = new SearchNode;

		// Set the Searchnode based on what edges we've already checked
		switch (Edges)
		{
		case 4: // Checks to the right
			Possible->X = pNode->X+1;
			Possible->Y = pNode->Y;
			break;
		case 3: // Checks to the left
			Possible->X = pNode->X-1;
			Possible->Y = pNode->Y;
			break;
		case 2: // Checks below
			Possible->Y = pNode->Y+1;
			Possible->X = pNode->X;
			break;
		case 1: // Checks above
			Possible->Y = pNode->Y-1;
			Possible->X = pNode->X;
			break;
		}

		// Decrement Edges
		--Edges;

		// If X or Y is off the map move on
		if(Possible->X < 0 || Possible->Y < 0 ||
		   Possible->X >= CurrentLevelColumns  ||
		   Possible->Y >= CurrentLevelRows      )
		{
			delete Possible;
			continue;
		}

		// If it is collidable, remove it from consideration
		if(LevelManager::GetInstance()->Collideable(Possible->X, Possible->Y))
		{
			delete Possible;
			continue;
		}
		
		// If it is neither collidable or off the map, attach its parent and
		// calculte its costs
		Possible->Parent = pNode;
		Possible->CalculateFCost( End );

		// If this edge is the End point, put it in the closed list and return true
		if (*Possible == *End)
		{
			End->Parent = pNode;
			ClosedList.push_back(Possible);
			return true;
		}
	
		// Make sure it's not already in the closed list
		list<SearchNode*>::iterator iter = ClosedList.begin();
		while (iter != ClosedList.end())
		{
			if (*(*iter) == *Possible) // If it's already in the list
			{				
				delete Possible;
				Possible = nullptr;
				break;
			}
			else
				++iter;
		}

		// Make sure Possible isn't a nullptr now;
		if (Possible == nullptr)
			continue; // Move on to the next Edge

		// Make sure it's not already in the open list
		iter = OpenList.begin();
		while (iter != OpenList.end())
		{
			if (*(*iter) == *Possible) // If it's already in the list
			{
				// Check if it's GCost is lower
				if (Possible->GCost < (*iter)->GCost)
					(*iter)->Parent = Possible->Parent;
				
				delete Possible;
				Possible = nullptr;
				break;
			}
			else
				++iter;
		}
		// Make sure Possible isn't a nullptr now;
		if (Possible == nullptr)
			continue; // Move on to the next Edge

		// If it's not collidable, not the end square, not off the map, and not already
		// in the open or closed list, add it.
		OpenList.push_back( Possible );
	}

	return false;
}

bool CPathFinder::PathFind( void )
{
	SearchNode* Next = nullptr;
	// Find the SearchNode with the lowest FCost in the open list,
	list<SearchNode*>::iterator iter = OpenList.begin();
	list<SearchNode*>::iterator remove = iter;
	while (iter != OpenList.end())
	{
		if (Next == nullptr)
		{
			Next = *iter;
			remove = iter;
			++iter;
			continue;
		}

		if (Next->FCost > (*iter)->FCost)
		{
			Next = *iter;
			remove = iter;
		}

		++iter;
	}
	
	if (Next == nullptr) // If Open list is empty
		return false; // failed to find path

	else
	{
		ClosedList.push_back(Next); // Add to the closed list
		OpenList.erase(remove);    // Remove from the open list

		if (AddNewEdges(Next)) // Add its edges, checking if its next to the end
		{
			PopulateWaypoints(); // Populate Waypoints
			return true;
		}
		else 
			return PathFind();
	}
}

void CPathFinder::PopulateWaypoints( void )
{
	// Path Found! Start at the end point inside the closed list, adding parents
	// to the stack until you reach the beginning node.
	while(!Waypoints.empty())
		Waypoints.pop();

	SearchNode * P = End;
	while (P != nullptr)
	{
		POINT Waypoint;
		Waypoint.x = (P->X * 64) + 32;
		Waypoint.y = (P->Y * 64) + 32;

		Waypoints.push(Waypoint);
		P = P->Parent;
	}

}

// SearchNode Methods --------------------------------------------------------------
void CPathFinder::SearchNode::CalculateFCost( SearchNode* pEnd )
{
	GCost = Parent->GCost + 10;
	HCost = abs(this->X - pEnd->X) + abs(this->Y - pEnd->Y);
	HCost *= 10;

	FCost = GCost + HCost;
}
