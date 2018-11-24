/*
File:		Camera.cpp
Author:		Daniel Habig
Course:		SGP
Purpose:	The Camera class contains the data concerning the camera's location
*/

#include "Camera.h"
#include "Game.h"
#include "Entity.h"
#include <cassert>

// SINGLETON!
/*static*/ Camera*	Camera::GetInstance( void )
{
	// Static local variable
	static Camera s_Instance;

	return &s_Instance;
}

Camera::Camera(void)
{
	shaking = -1.0f;
	next = -1.0f;
	offX = offY = 0;
}

Camera::~Camera(void)
{
}

bool Camera::Enter( void )
{
	return true;
}

bool Camera::Exit( void )
{
	return true;
}

void Camera::Update( float fElapsedTime )
{
	if( shaking >= 0 )
	{
		shaking -= fElapsedTime;
		next -= fElapsedTime;
		if( next < 0 )
		{
			int amount = 20;

			offY = rand()%(amount*2) - amount;
			offX = rand()%(amount*2) - amount;
			next = 0.05f;
		}
	}
	else
	{
		offX = offY = 0;
	}
}

void Camera::Center( int x, int y )
{
	unsigned int screenWidth = CGame::GetInstance()->GetWindowWidth();
	unsigned int screenHeight = CGame::GetInstance()->GetWindowHeight();

	area.left = (long)(x - screenWidth * 0.5f);
	area.top = (long)(y - screenHeight * 0.5f);
	area.right = area.left + screenWidth;
	area.bottom = area.top + screenHeight;
}

void Camera::Center(CEntity* ent)
{
	unsigned int screenWidth = CGame::GetInstance()->GetWindowWidth();
	unsigned int screenHeight = CGame::GetInstance()->GetWindowHeight();

	int x = (int)ent->GetPosX();
	int y = (int)ent->GetPosY();

	area.left = (long)(x - screenWidth * 0.5f);
	area.top = (long)(y - screenHeight * 0.5f);
	area.right = area.left + screenWidth;
	area.bottom = area.top + screenHeight;
}

void Camera::CamRect(RECT* rect)
{
	rect->top		= area.top		+ offY;
	rect->bottom	= area.bottom	+ offY;
	rect->left		= area.left		+ offX;
	rect->right		= area.right	+ offX;
}