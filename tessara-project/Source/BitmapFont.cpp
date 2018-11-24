/*
	Author: Benjamin "Evan" Cowles
	File BitmapFont.cpp
*/

#include "BitmapFont.h"
#include "..\SGD Wrappers\CSGD_TextureManager.h"

CBitmapFont * CBitmapFont::s_pInstance = nullptr;

CBitmapFont::CBitmapFont()
{
	this->m_nImageID = -1;
	
	this->m_nWidth['A'] = 37;
	this->m_pPosition['A'].x = 0;
	this->m_pPosition['A'].y = 9;
	this->m_nWidth['B'] = 35;
	this->m_pPosition['B'].x = 39;
	this->m_pPosition['B'].y = 9;
	this->m_nWidth['C'] = 36;
	this->m_pPosition['C'].x = 74;
	this->m_pPosition['C'].y = 9;
	this->m_nWidth['D'] = 37;
	this->m_pPosition['D'].x = 110;
	this->m_pPosition['D'].y = 9;
	this->m_nWidth['E'] = 36;
	this->m_pPosition['E'].x = 148;
	this->m_pPosition['E'].y = 9;
	this->m_nWidth['F'] = 34;
	this->m_pPosition['F'].x = 187;
	this->m_pPosition['F'].y = 9;
	this->m_nWidth['G'] = 36;
	this->m_pPosition['G'].x = 222;
	this->m_pPosition['G'].y = 9;
	this->m_nWidth['H'] = 36;
	this->m_pPosition['H'].x = 260;
	this->m_pPosition['H'].y = 9;
	this->m_nWidth['I'] = 8;
	this->m_pPosition['I'].x = 297;
	this->m_pPosition['I'].y = 9;
	this->m_nWidth['J'] = 26;
	this->m_pPosition['J'].x = 306;
	this->m_pPosition['J'].y = 9;
	this->m_nWidth['K'] = 35;
	this->m_pPosition['K'].x = 337;
	this->m_pPosition['K'].y = 9;
	this->m_nWidth['L'] = 26;
	this->m_pPosition['L'].x = 375;
	this->m_pPosition['L'].y = 9;
	this->m_nWidth['M'] = 44;
	this->m_pPosition['M'].x = 401;
	this->m_pPosition['M'].y = 9;
	this->m_nWidth['N'] = 35;
	this->m_pPosition['N'].x = 447;
	this->m_pPosition['N'].y = 9;
	this->m_nWidth['O'] = 36;
	this->m_pPosition['O'].x = 483;
	this->m_pPosition['O'].y = 9;
	this->m_nWidth['P'] = 35;
	this->m_pPosition['P'].x = 521;
	this->m_pPosition['P'].y = 9;
	this->m_nWidth['Q'] = 37;
	this->m_pPosition['Q'].x = 0;
	this->m_pPosition['Q'].y = 82;
	this->m_nWidth['R'] = 35;
	this->m_pPosition['R'].x = 38;
	this->m_pPosition['R'].y = 82;
	this->m_nWidth['S'] = 35;
	this->m_pPosition['S'].x = 75;
	this->m_pPosition['S'].y = 82;
	this->m_nWidth['T'] = 36;
	this->m_pPosition['T'].x = 111;
	this->m_pPosition['T'].y = 82;
	this->m_nWidth['U'] = 35;
	this->m_pPosition['U'].x = 149;
	this->m_pPosition['U'].y = 82;
	this->m_nWidth['V'] = 36;
	this->m_pPosition['V'].x = 185;
	this->m_pPosition['V'].y = 82;
	this->m_nWidth['W'] = 44;
	this->m_pPosition['W'].x = 222;
	this->m_pPosition['W'].y = 82;
	this->m_nWidth['X'] = 36;
	this->m_pPosition['X'].x = 267;
	this->m_pPosition['X'].y = 82;
	this->m_nWidth['Y'] = 35;
	this->m_pPosition['Y'].x = 305;
	this->m_pPosition['Y'].y = 82;
	this->m_nWidth['Z'] = 36;
	this->m_pPosition['Z'].x = 341;
	this->m_pPosition['Z'].y = 82;
	
	this->m_nWidth['a'] = 24;
	this->m_pPosition['a'].x = 379;
	this->m_pPosition['a'].y = 82;
	this->m_nWidth['b'] = 24;
	this->m_pPosition['b'].x = 406;
	this->m_pPosition['b'].y = 82;
	this->m_nWidth['c'] = 24;
	this->m_pPosition['c'].x = 431;
	this->m_pPosition['c'].y = 82;
	this->m_nWidth['d'] = 25;
	this->m_pPosition['d'].x = 457;
	this->m_pPosition['d'].y = 82;
	this->m_nWidth['e'] = 24;
	this->m_pPosition['e'].x = 483;
	this->m_pPosition['e'].y = 82;
	this->m_nWidth['f'] = 22;
	this->m_pPosition['f'].x = 510;
	this->m_pPosition['f'].y = 82;
	this->m_nWidth['g'] = 24;
	this->m_pPosition['g'].x = 533;
	this->m_pPosition['g'].y = 82;
	this->m_nWidth['h'] = 24;
	this->m_pPosition['h'].x = 40;
	this->m_pPosition['h'].y = 153;
	this->m_nWidth['i'] = 9;
	this->m_pPosition['i'].x = 66;
	this->m_pPosition['i'].y = 153;
	this->m_nWidth['j'] = 10;
	this->m_pPosition['j'].x = 77;
	this->m_pPosition['j'].y = 153;
	this->m_nWidth['k'] = 24;
	this->m_pPosition['k'].x = 90;
	this->m_pPosition['k'].y = 153;
	this->m_nWidth['l'] = 12;
	this->m_pPosition['l'].x = 116;
	this->m_pPosition['l'].y = 153;
	this->m_nWidth['m'] = 36;
	this->m_pPosition['m'].x = 130;
	this->m_pPosition['m'].y = 153;
	this->m_nWidth['n'] = 24;
	this->m_pPosition['n'].x = 169;
	this->m_pPosition['n'].y = 153;
	this->m_nWidth['o'] = 25;
	this->m_pPosition['o'].x = 194;
	this->m_pPosition['o'].y = 153;
	this->m_nWidth['p'] = 25;
	this->m_pPosition['p'].x = 220;
	this->m_pPosition['p'].y = 153;
	this->m_nWidth['q'] = 24;
	this->m_pPosition['q'].x = 246;
	this->m_pPosition['q'].y = 153;
	this->m_nWidth['r'] = 24;
	this->m_pPosition['r'].x = 273;
	this->m_pPosition['r'].y = 153;
	this->m_nWidth['s'] = 25;
	this->m_pPosition['s'].x = 298;
	this->m_pPosition['s'].y = 153;
	this->m_nWidth['t'] = 25;
	this->m_pPosition['t'].x = 324;
	this->m_pPosition['t'].y = 153;
	this->m_nWidth['u'] = 24;
	this->m_pPosition['u'].x = 350;
	this->m_pPosition['u'].y = 153;
	this->m_nWidth['v'] = 26;
	this->m_pPosition['v'].x = 375;
	this->m_pPosition['v'].y = 153;
	this->m_nWidth['w'] = 39;
	this->m_pPosition['w'].x = 402;
	this->m_pPosition['w'].y = 153;
	this->m_nWidth['x'] = 26;
	this->m_pPosition['x'].x = 442;
	this->m_pPosition['x'].y = 153;
	this->m_nWidth['y'] = 24;
	this->m_pPosition['y'].x = 469;
	this->m_pPosition['y'].y = 153;
	this->m_nWidth['z'] = 26;
	this->m_pPosition['z'].x = 494;
	this->m_pPosition['z'].y = 153;

	this->m_nWidth['-'] = 32;
	this->m_pPosition['-'].x = 76;
	this->m_pPosition['-'].y = 221;
	this->m_nWidth['+'] = 32;
	this->m_pPosition['+'].x = 116;
	this->m_pPosition['+'].y = 221;
	this->m_nWidth['0'] = 36;
	this->m_pPosition['0'].x = 150;
	this->m_pPosition['0'].y = 221;
	this->m_nWidth['1'] = 8;
	this->m_pPosition['1'].x = 189;
	this->m_pPosition['1'].y = 221;
	this->m_nWidth['2'] = 25;
	this->m_pPosition['2'].x = 200;
	this->m_pPosition['2'].y = 221;
	this->m_nWidth['3'] = 26;
	this->m_pPosition['3'].x = 225;
	this->m_pPosition['3'].y = 221;
	this->m_nWidth['4'] = 25;
	this->m_pPosition['4'].x = 252;
	this->m_pPosition['4'].y = 221;
	this->m_nWidth['5'] = 25;
	this->m_pPosition['5'].x = 278;
	this->m_pPosition['5'].y = 221;
	this->m_nWidth['6'] = 25;
	this->m_pPosition['6'].x = 304;
	this->m_pPosition['6'].y = 221;
	this->m_nWidth['7'] = 24;
	this->m_pPosition['7'].x = 331;
	this->m_pPosition['7'].y = 221;
	this->m_nWidth['8'] = 25;
	this->m_pPosition['8'].x = 356;
	this->m_pPosition['8'].y = 221;
	this->m_nWidth['9'] = 25;
	this->m_pPosition['9'].x = 382;
	this->m_pPosition['9'].y = 221;
	
	this->m_nWidth['.'] = 9;
	this->m_pPosition['.'].x = 87;
	this->m_pPosition['.'].y = 287;
	this->m_nWidth[','] = 9;
	this->m_pPosition[','].x = 99;
	this->m_pPosition[','].y = 287;
	this->m_nWidth[':'] = 9;
	this->m_pPosition[':'].x = 111;
	this->m_pPosition[':'].y = 287;
	this->m_nWidth[';'] = 9;
	this->m_pPosition[';'].x = 123;
	this->m_pPosition[';'].y = 287;
	this->m_nWidth['\''] = 9;
	this->m_pPosition['\''].x = 134;
	this->m_pPosition['\''].y = 287;
	this->m_nWidth['\"'] = 18;
	this->m_pPosition['\"'].x = 147;
	this->m_pPosition['\"'].y = 287;
	this->m_nWidth['!'] = 9;
	this->m_pPosition['!'].x = 168;
	this->m_pPosition['!'].y = 287;
	this->m_nWidth['?'] = 14;
	this->m_pPosition['?'].x = 178;
	this->m_pPosition['?'].y = 287;
	this->m_nWidth['@'] = 36;
	this->m_pPosition['@'].x = 193;
	this->m_pPosition['@'].y = 287;
	this->m_nWidth['#'] = 25;
	this->m_pPosition['#'].x = 232;
	this->m_pPosition['#'].y = 287;
	this->m_nWidth['$'] = 35;
	this->m_pPosition['$'].x = 259;
	this->m_pPosition['$'].y = 287;
	this->m_nWidth['%'] = 22;
	this->m_pPosition['%'].x = 296;
	this->m_pPosition['%'].y = 287;
	this->m_nWidth['&'] = 28;
	this->m_pPosition['&'].x = 319;
	this->m_pPosition['&'].y = 287;
	this->m_nWidth['*'] = 22;
	this->m_pPosition['*'].x = 346;
	this->m_pPosition['*'].y = 287;
	this->m_nWidth['{'] = 11;
	this->m_pPosition['{'].x = 370;
	this->m_pPosition['{'].y = 287;
	this->m_nWidth['('] = 10;
	this->m_pPosition['('].x = 382;
	this->m_pPosition['('].y = 287;
	this->m_nWidth['/'] = 22;
	this->m_pPosition['/'].x = 394;
	this->m_pPosition['/'].y = 287;
	this->m_nWidth['|'] = 6;
	this->m_pPosition['|'].x = 421;
	this->m_pPosition['|'].y = 287;
	this->m_nWidth['\\'] = 20;
	this->m_pPosition['\\'].x = 429;
	this->m_pPosition['\\'].y = 287;
	this->m_nWidth[')'] = 10;
	this->m_pPosition[')'].x = 450;
	this->m_pPosition[')'].y = 287;
	this->m_nWidth['}'] = 10;
	this->m_pPosition['}'].x = 462;
	this->m_pPosition['}'].y = 287;
	
	this->m_nSpaceWidth = 15;
	this->m_nSpacing = 4;
	this->m_nHeight = 49;
}

CBitmapFont::~CBitmapFont()
{

}

void CBitmapFont::Draw(string szText, int nX, int nY, float fScale, DWORD dwColor)
{
	for(unsigned int i = 0; i < szText.length(); i++)
	{
		char c = szText[i];

		if(c == _T(' '))
		{
			nX += (int)((this->m_nSpaceWidth + this->m_nSpacing) * fScale);
			continue;
		}

		RECT rChar;
		rChar.top = this->m_pPosition[c].y;
		rChar.left = this->m_pPosition[c].x;
		rChar.bottom = rChar.top + this->m_nHeight;
		rChar.right = rChar.left + this->m_nWidth[c];
		
		CSGD_TextureManager::GetInstance()->Draw(this->m_nImageID, nX, nY, fScale, fScale, &rChar, 0.0f, 0.0f, 0.0f, dwColor);
		
		nX += (int)((this->m_nWidth[c] + this->m_nSpacing) * fScale);
	}
}

void CBitmapFont::Initialize()
{
	this->m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/Font/Font.png"));
}

CBitmapFont * CBitmapFont::GetInstance()
{
	if (s_pInstance == nullptr)
	{
		s_pInstance = new CBitmapFont;
	}

	return s_pInstance;
}
	
void CBitmapFont::DeleteInstance()
{
	delete s_pInstance;
	s_pInstance = nullptr;
}

void CBitmapFont::Shutdown()
{
	if(this->m_nImageID != -1)
	{
		CSGD_TextureManager::GetInstance()->UnloadTexture(this->m_nImageID);
		this->m_nImageID = -1;
	}
}