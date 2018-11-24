/*
File:		Door.h
Author:		Daniel Habig
Course:		SGP
Purpose:	The door class is a non-moving entity that acts as a door would
*/

#pragma once
#include "Entity.h"
#include "LevelManager.h"

enum DOORTYPE { KEY, LAMP, POWER };
enum DOORSTATE { OPEN, CLOSED, OPENING, CLOSING };

class Door : public CEntity
{
private:
	enum DOORTYPE dType;
	DOORSTATE state;

	vector<Area> nodes;

	float time;


public:
	Door(void);
	~Door(void);
	virtual void Update( float deltaTime );
	void UpdateKey(float deltaTime);
	void UpdateLamp(float deltaTime);
	void UpdatePower(float deltaTime);
	
	virtual void Render( void );

	virtual bool CheckCollision( IEntity* pOther );
	bool CollisionKey(IEntity* pOther);
	bool CollisionLamp(IEntity* pOther);
	bool CollisionPower(IEntity* pOther);

	virtual RECT GetRect ();

	void Create( int type, Area loc, vector<Area> nodes );

	void Activate();
	void Deactivate();

	void SetOpen(void);
	void SetClosed(void);
	void SetOpening(void);
	void SetClosing(void);


};

