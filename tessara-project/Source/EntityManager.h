/*
	Author: Jorge Juaneda, Mark Diaz
	File: EnctityManger.h
	Purpose: The CEntityManager class will provide
			 an interface to update and render all
			 game entities
*/

#pragma once
#include <vector>
#include "Entity.h"

class IEntity;
class CEntityManager
{
public:
	// This is a singleon.
	static CEntityManager* GetInstance		( void );
	static void			   DeleteInstance	( void );

	// Entity Management
	void AddEntity			( IEntity * pEntity);
	void RemoveEntity		( IEntity * pEntity);
	void RemoveAll			( void );

	//For map data
	void RemoveTypeAtIndex	( EntityType type, int index );
	void Given				( int index );
	void Open				( int index );

	// Interface
	void UpdateAll			( float deltaTime );
	void RenderAll			( void );
	void FindCollisions		( void );

	void RenderPlayer		( void );
	void RenderDoor			( int nAlpha);

	void RemoveNPE			( void );
	bool IsEntityAlive		(IEntity* pEntity);

	bool Aggroed			(void);


private:
	// This is a singleton.
	static CEntityManager* s_pInstance;
	CEntityManager(void);
	~CEntityManager(void);
	CEntityManager( const CEntityManager& );
	CEntityManager* operator= ( const CEntityManager& );



	// Entity data structure:
	std::vector<IEntity *> m_vEntities;
	// Bool to ensure nothing is added or deleted while Iterating
	bool				   m_bIterating;

};

