/*
Author: Jorge Juaneda
File: PLayer.h
*/
#pragma once

#include "Entity.h"

#include "InputManager.h"
#include "AnimationInformation.h"

#include "../SGD Wrappers\SGD_String.h"
#include "../SGD Wrappers/IListener.h"
#include "..\\SGD Wrappers\CSGD_TextureManager.h"
#include "..\\SGD Wrappers\CEvent.h"
#include "..\\SGD Wrappers\IListener.h"
#include "Item.h"

class CArtifact;
class Camera;
class CBitmapFont;
class CParticleEffect;

class CPlayer : public CEntity, public IListener
{
private:

	// Helper Function:
	void LevelUp( void );
	void UpdateEmitters( float deltatime );
	void ClearEmitters( void );
	int m_nUnspent;
	int m_nBody;
	int m_nMind;
	int m_nSoul;
	int m_nCharacter;
	int m_nMeleeDamage;
	int m_nShards;
	int m_nHelpMessages;

	CParticleEffect* LevelUpEffect;
	CParticleEffect* DashEffect;
	CParticleEffect* FreezeHeal;
	CParticleEffect* SmashEffect;
	CParticleEffect* IceSlashEffect;
	CParticleEffect* LightningSlash;
	CParticleEffect* RockShield;

	// Transition effects
	CParticleEffect* TransitionEffect;
	float TransitionTimer;
	void ArtifactTransition( void );
	float m_fDashEffectTimer;
	float m_fLevelUpTimer;

	int m_nLevelUpAlpha;
	int m_nLevelWindowID;
	string LevelMessage;

	int m_nCritChance;
	int m_nDodgeChance;
	int m_nResistance;

	int m_nCurrency;

	int m_nExperience;
	int m_nToNextLevel;
	int m_nLevel;

	int m_nCurrHealth;
	int m_nMaxHealth;

	int m_nKeys;

	//int m_nFacing;
	float m_fVelocityY;
	float m_fVelocityX;
	float m_fAcceleration;
	int m_nArtifact;
	float m_fTakeFireDamage;

	vector<int> m_vHUDImgIDs;
	vector<int> m_vInventoryIDs;

	//Status
	bool m_bOnFire;
	bool m_bOnIce;
	bool m_bSlowed;

	//Ability
	bool m_bLunge;
	bool m_bEarthWall;
	bool m_bEarthSmash;
	bool m_bHealWall;
	bool m_bStab;
	bool m_bSwipe;
	bool m_bShoot;
	bool m_bMelee;
	bool m_bChakram;
	RECT rEWall;
	float m_fLungeTimer;
	float m_fEarthWalltimer;


	float m_fAbTimers[4];
	float m_fAbCooldown[4];
	float m_fHealTimer;
	float m_fHealCooldown;


	float m_fArtifactTimers[4];
	float m_fArtifactCooldown[4];

	int m_nNumArt; 
	int m_nCurrAtr;

	CInputManager*  m_pIM;
	CBitmapFont*	m_pBF;

	//TEST
	CSGD_TextureManager* m_pTM;
	int imgID;

	int m_nPortalID;



	//artifact
	vector <CArtifact*>  m_vArtifacts;

	// Inventory
	vector<CItem*> m_vItems;
	CItem * m_iItemToSocket;
	CItem * m_iPotion;
	int m_nPotionCount;

	//Camera
	Camera*  m_pCamera;

	//test
	RECT SamshDraw;

public:

	CPlayer(void);
	~CPlayer(void);

	virtual void Render();
	virtual void Update( float deltaTime );
	virtual bool CheckCollision(IEntity* pOther);
	void ModifyHealth( int nAmount);
	bool Input();
	void IncreaseArtifact(int nIndex);

	//update all timers
	void UpdateCooldowns(float deltaTime);

	void AddAnimInfo(int nAnimationID);
	void DrawHUD();
	void DrawVignette();
	void InventoryRender();

	// Player Leveling 
	void GainXP( int amount );

	// IListener interface
	virtual void HandleEvent(CEvent* pEvent);

	virtual RECT GetRect ();
	void SetPortalID(int nPortalID) { m_nPortalID = nPortalID; }
	void SetCurrAtr(int nIndex) { m_nCurrAtr = nIndex; }
	int GetCurrAtr() { return m_nCurrAtr; }

	// Accessors
	int GetHelpMessages()		{ return this->m_nHelpMessages; }
	int GetFacing()				{ return this->m_nFacing; }
	int GetUnspent()			{ return m_nUnspent;	}
	int GetCharacter();
	int GetMind();
	int GetBody();
	int GetSoul();
	int GetCharacterE();
	int GetMindE();
	int GetBodyE();
	int GetSoulE();
	int GetLevel()				{ return m_nLevel;		}
	int GetExperience()			{ return m_nExperience;	}
	int GetResistance()			{ return m_nResistance; }
	int GetMeleeDamage( void )  { return m_nMeleeDamage;} 
	float GetSpeed( void )		{ return m_fAcceleration;}
	int GetCritChance( void )   { return m_nCritChance; }
	int GetDodgeChance( void )  { return m_nDodgeChance;}
	int GetToNextLevel()		{ return m_nToNextLevel;}
	int GetCurrHealth()			{ return m_nCurrHealth; }
	int GetMaxHealth()			{ return m_nMaxHealth;	}

	int GetShards()				{ return m_nShards;		}
	int GetPortalID()			{ return m_nPortalID;	}
	vector<CItem *> GetItems()  { return m_vItems;		}
	vector<CItem *> GetWeapons();
	vector<CItem *> GetArmors();
	vector<CItem *> GetGems();
	vector<CItem *> GetEquippableItems();
	int GetPotionCount() { return m_nPotionCount; }
	CItem * GetItemToSocket() { return m_iItemToSocket; }
	void RemoveGem(CItem* pItem);
	void SetHelpMessages( int x )	{ m_nHelpMessages = x; }
	int GetArtifactNumber() { return m_vArtifacts.size(); }

	int GetArtifactLevel(int nIndex);

	// Helpers
	void CalculateStats();
	void AddShard()			{ ++m_nShards; }
	void ClearParticleEffects();

	// Mutators
	void SetUnspent( int u )	{ m_nUnspent = u; }
	void SetCharacter( int nChar);
	void SetMind( int nMind);
	void SetBody(int nBody);		 
	void SetSoul(int nSoul);		
	void SetCharacterEx( int nChar) { m_nCharacter = nChar; }
	void SetMindEx( int nMind) { m_nMind = nMind; }
	void SetBodyEx(int nBody) { m_nBody = nBody; }
	void SetSoulEx(int nSoul) { m_nSoul = nSoul; }
	void SetCurrHealth(int nHealth) {m_nCurrHealth = nHealth; }
	void AddCurrency( int nAmount) { m_nCurrency += nAmount; }
	int  GetCurrency( ) { return m_nCurrency; }
	void SetItemToSocket(CItem * iItemToSocket) { m_iItemToSocket = iItemToSocket; }
	void SetLevel(int nLevel) { m_nLevel = nLevel; }
	void SetShards(int nShards) { m_nShards = nShards; }
	void SetCurrency(int nCurrency) { m_nCurrency = nCurrency; }
	void SetPotion(int nPotions) { m_nPotionCount = nPotions; }
	void ClearEquips();
	void AddKey();
	void RemoveKey() { m_nKeys--; }
	int GetKeys() { return m_nKeys; }
	void RemoveShard() { m_nShards--; }

	void GainPotion();

	void LevelUpArtifact( int index );

	int GetWeaponDamage();
	void AddItem(CItem * pItem);
	void RemoveItem(CItem * pItem);
	void AddArtifact(bool bUpdating = false);
	void SetExpNeeded(int nLevel);

	//Abilyties
	void Chakram();
	void Lunge();
	void CreateRockWall();
	void EarthSmash();
	void Stab();
	void HealWall();
	void SwordSwipe();
	void Shoot();
	bool GetEarthWall() {return m_bEarthWall;}
	RECT GetActiveRec();
	void Melee();

	void StopDash();

	bool ActiveRecCollision(IEntity* pOther);

	bool CollidingWith ( IEntity* pOther, RECT* rOver  );
	bool CollidingWith ( RECT rRec ,IEntity* pOther, RECT* rOver  );
	void HandleCollision( void );

	bool IsDashing() {return m_bLunge; }

	void GoToEndAnime(void) { this->SetCurrAnim(6 /*Player Attack Stab Down*/); this->GetAnimationInformation(this->GetCurrAnim())->Reset(); }
};