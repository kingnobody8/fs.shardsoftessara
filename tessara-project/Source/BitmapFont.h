/*
	Author: Benjamin "Evan" Cowles
	File BitmapFont.h
*/

#pragma once

#include <Windows.h>
#include <string>
using namespace std;

class CBitmapFont
{
	static CBitmapFont * s_pInstance;
	
	CBitmapFont();
	~CBitmapFont();
	CBitmapFont(const CBitmapFont&);
	CBitmapFont& operator= (const CBitmapFont&);
public:
	static CBitmapFont * GetInstance();
	static void DeleteInstance();

	void Draw(string szText, int nX, int nY, float fScale, DWORD dwColor);
	void Initialize();
	void Shutdown();

private:
	int m_nImageID;
	int m_nSpacing;
	int m_nSpaceWidth;
	int m_nHeight;
	int m_nWidth[256];
	POINT m_pPosition[256];
};