/*
Author: Jorge Juaneda
File: Wraight.cpp
*/

#include "EntityManager.h"
#include <cassert>
#include "IEntity.h"
#include "LevelManager.h"
#include "Monster.h"
#include "NPC.h"
#include "Door.h"

// This is a singleton.
/* This part instantiates the static data member and handles the 
sole instantiation of the CEntityManager*/
/*static*/CEntityManager* CEntityManager::s_pInstance = nullptr;
/*static*/CEntityManager* CEntityManager::GetInstance( void )
{
	if ( s_pInstance == nullptr )
		s_pInstance = new CEntityManager;

	return s_pInstance;
}

/* This function needs to be called in the destructor or shutdown
function of the GamePlayState. It deletes the instance of
CEntityManager */
/*static*/void CEntityManager::DeleteInstance( void )
{
	delete s_pInstance;
	s_pInstance = nullptr;
}


// Constructor:
// 
//
//
CEntityManager::CEntityManager(void)
{
	m_bIterating = false;
}

// Destructor:
//
//
//
CEntityManager::~CEntityManager(void)
{
	RemoveAll();
}

// AddEntity:
// - Adds an entity to the manager's data structure
// - Ensures we are not iterating at the time
//
void CEntityManager::AddEntity( IEntity* pEntity )
{
	// Make sure we have a valid parameter
	assert( pEntity != nullptr && "CEntityManager::AddEntity - parameter is nullptr" );
	assert( m_bIterating == false && "CEntityManager::AddEntity - Entity being added during iteration" );

	// Add the entity
	m_vEntities.push_back( pEntity );

	//Add a reference to the entity!
	pEntity->AddRef();
}

// RemoveEntity:
//	- Removes an entity from the manager's data structure
//	- Ensures we're not iterating at the time
//
void CEntityManager::RemoveEntity( IEntity* pEntity)
{
	assert( pEntity != nullptr && "CEntityManager::RemoveEntity - parameter is nullptr" );
	assert( m_bIterating == false && "CEntityManager::RemoveEntity - Entity being removed during iteration" );

	// Find the entity
	std::vector< IEntity* >::iterator iter = m_vEntities.begin();
	for ( ; iter != m_vEntities.end(); ++iter )
	{
		if ( *iter == pEntity )
		{
			// Found the entity
			// Release the reference to the Entity!
			pEntity->Release();

			// Remove from list
			m_vEntities.erase(iter);
			break;
		}
	}
}

// RemoveAll
//	- Removes all entities from the CEntityManger
//	- Ensures we're not iterating at the time
//
void CEntityManager::RemoveAll( void )
{
	assert( m_bIterating == false && "CEntityManager::RemoveAll - Being called while iterating" );


	std::vector< IEntity* >::iterator iter = m_vEntities.begin();
	for ( ; iter != m_vEntities.end(); ++iter )
		(*iter)->Release();

	// Clear the data structure
	m_vEntities.clear();
}


//RemoveTypeAtIndex
//	- Removes the entity that is the 'index' of that type
//	- Does nothing if their is no entity to remove
//
void CEntityManager::RemoveTypeAtIndex	( EntityType type, int index )
{
	assert( m_bIterating == false && "CEntityManager::RemoveEntity - Entity being removed during iteration" );

	int count = 0;

	// Find the entity
	std::vector< IEntity* >::iterator iter = m_vEntities.begin();
	for ( ; iter != m_vEntities.end(); ++iter )
	{
		if ( (*iter)->GetType() == type )
		{
			if( count == index )
			{
				// Found the entity
				// Release the reference to the Entity!
				(*iter)->Release();
				// Remove from list
				m_vEntities.erase(iter);
				break;
			}
			count++;
		}
	}
}

//Give
//	- Takes the NPC at the index and changes it's bool to given
//	-
void CEntityManager::Given( int index )
{
	int count = 0;

	// Find the entity
	std::vector< IEntity* >::iterator iter = m_vEntities.begin();
	for ( ; iter != m_vEntities.end(); ++iter )
	{
		if ( (*iter)->GetType() == ENT_NPC )
		{
			if( count == index )
			{
				NPC* ptr = dynamic_cast<NPC*>(*iter);

				if( ptr != nullptr )	//Change to given
					ptr->SetGiven( true );
				break;
			}
			count++;
		}
	}
}

void CEntityManager::Open( int index )
{
	int count = 0;

	// Find the entity
	std::vector< IEntity* >::iterator iter = m_vEntities.begin();
	for ( ; iter != m_vEntities.end(); ++iter )
	{
		if ( (*iter)->GetType() == ENT_DOOR )
		{
			if( count == index )
			{
				Door* ptr = dynamic_cast<Door*>(*iter);

				if( ptr != nullptr )	//Change to open
					ptr->Activate();
				break;
			}
			count++;
		}
	}
}

// UpdateAll
//	- Iterates through the data structure and updates all entities
//	- Locks and Unlocks the iterator during this process
//
void CEntityManager::UpdateAll( float deltaTime )
{
	assert( m_bIterating == false && "CEntityManager::UpdateAll - Trying to update while iterating" );

	// Lock the iterator
	m_bIterating = true;

	// Update each entity individually
	std::vector< IEntity* >::iterator iter = m_vEntities.begin();
	for ( ; iter != m_vEntities.end(); ++iter )
		(*iter)->Update( deltaTime );

	// Unlock the iterator
	m_bIterating = false;
}

// RenderAll
//	- Iterates through the data structure and renders all entities
//	- Locks and Unlocks the iterator during this process
//
void CEntityManager::RenderAll( void )
{
	assert( m_bIterating == false && "CEntityManager::RenderAll - Trying to render while iterating" );

	// Lock the iterator
	m_bIterating = true;
	IEntity* player;

	// Update each entity individually
	std::vector< IEntity* >::iterator iter = m_vEntities.begin();
	for ( ; iter != m_vEntities.end(); ++iter )
	{
		if ((*iter)->GetType() == ENT_PLAYER)
			player = (*iter);
		else
			(*iter)->Render();
	}

	player->Render();
	player = nullptr;

	// Unlock the iterator
	m_bIterating = false;
}

// FindCollisions
//	- Check entities for collision
//	- Locks and unlocks the iterator during this process
//
void CEntityManager::FindCollisions( void )
{
	assert( m_bIterating == false && "CEntityManager::FindCollisions - being called while iterating" );

	// Lock the iterator
	m_bIterating = true;

	// Check entities against each other based on type
	// entities wil the same type will no register collision
	for( unsigned int i = 0; i < m_vEntities.size(); ++i )
	{
		for ( unsigned int j = 0; j < m_vEntities.size(); ++j )
		{

			if ( m_vEntities[i]->GetType() != m_vEntities[j]->GetType())
			{

				if ( m_vEntities[i]->CheckCollision( m_vEntities[j] ) == true )	
					// Break after 1 collision is found
						break; 		

			}
		}
	}




	// Unlock the iterator
	m_bIterating = false;
}

void CEntityManager::RenderPlayer( void )
{
	// Lock the iterator
	m_bIterating = true;
	IEntity* player;

	// Update each entity individually
	std::vector< IEntity* >::iterator iter = m_vEntities.begin();
	for ( ; iter != m_vEntities.end(); ++iter )
	{
		if ((*iter)->GetType() == ENT_PLAYER)
			player = (*iter);
	}

	player->Render();
	player = nullptr;

	// Unlock the iterator
	m_bIterating = false;
}

void CEntityManager::RenderDoor(int nAlpha)
{
	// Lock the iterator
	m_bIterating = true;
	IEntity* door = nullptr;

	// Update each entity individually
	std::vector< IEntity* >::iterator iter = m_vEntities.begin();
	for ( ; iter != m_vEntities.end(); ++iter )
	{
		if ((*iter)->GetType() != ENT_PLAYER && (*iter)->GetType() != ENT_ITEM)
			door = (*iter);
	}

	if(door != nullptr)
	{
		door->RenderAlpha(nAlpha);
	}

	door = nullptr;

	// Unlock the iterator
	m_bIterating = false;
}

//Remove all non player entity
void CEntityManager::RemoveNPE	( void )
{
	assert( m_bIterating == false && "CEntityManager::RemoveEntity - Entity being removed during iteration" );

	// Find the entity
	std::vector< IEntity* >::iterator iter = m_vEntities.begin();
	for ( ; iter != m_vEntities.end(); ++iter )
	{
		if ( (*iter)->GetType() != 1 )
		{
			// Found the entity
			// Release the reference to the Entity!
			(*iter)->Release();

			// Remove from list
			iter = m_vEntities.erase(iter);
			iter--;
			//break;
		}
	}

}

bool CEntityManager::IsEntityAlive	(IEntity* pEntity)
{

	std::vector< IEntity* >::iterator iter = m_vEntities.begin();
	for ( ; iter != m_vEntities.end(); ++iter )
	{
		if ((*iter) == pEntity)
			return true;
	}
	return false;
}

bool CEntityManager::Aggroed(void)
{
	// Find the entity
	std::vector< IEntity* >::iterator iter = m_vEntities.begin();
	for ( ; iter != m_vEntities.end(); ++iter )
	{
		EntityType type = (EntityType)(*iter)->GetType();
		if( type == ENT_ARCHER || type == ENT_BOSS || type == ENT_WIGHT || type == ENT_WRAITH || type == ENT_ZOMBIE )
		{
			CMonster* mon = dynamic_cast<CMonster*>(*iter);
			if( mon != nullptr )
				if( mon->GetAggro() )
					return true;
		}
	}
	return false;
}