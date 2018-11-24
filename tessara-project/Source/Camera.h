/*
File:		Camera.h
Author:		Daniel Habig
Course:		SGP
Purpose:	The Camera class contains the data concerning the camera's location
*/

#pragma once
#include <Windows.h>
#define SHAKETIME 0.35f;

class CEntity;

class Camera
{
public:
// SINGLETON!
	static Camera*	GetInstance( void );
	//--------------------------------------------------------------------------------------------------

	bool Enter			( void );	// Load Resources
	bool Exit			( void );	// UnLoad Resources

	void Center			( int x, int y );
	void Center			(CEntity* ent);

	void CamRect(RECT* rect);
	int GetTop( void )  { return area.top;	}
	int GetLeft( void ) { return area.left; }
	void Shake( void )	{ shaking = SHAKETIME; }
	void Update( float fElapsedTime );


private:
	// SINGLETON!
	Camera( void );							// ctor
	virtual ~Camera( void );				// dtor
	Camera( const Camera& );				// copy ctor
	Camera& operator= ( const Camera& );	// assignment op

	RECT area;

	float shaking;
	float next;
	int offX, offY;

};

