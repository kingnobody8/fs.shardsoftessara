/*
Author: Jorge Juaneda
File: Player.cpp
*/

#include "Player.h"

#include "..\\SGD Wrappers\CSGD_Direct3D.h"
#include "AnimationManager.h"
#include "AnimationInformation.h"
#include "AudioManager.h"

#include "../SGD Wrappers/CEvent.h"
#include "../SGD Wrappers/CSGD_EventSystem.h"
#include "PlayState.h"
#include "ParticleEffect.h"
#include "ParticleManager.h"

//Artifacts
#include "Artifact.h"
#include "FireArtifact.h"
#include "SnowArtifact.h"
#include "ElectricArtifact.h"
#include "EarthArtifact.h"

//Messages
#include "MessageSystem.h"
#include "DeleteProjectileMessage.h"
#include "LoadLevelMsg.h"
#include "DeleteProjectileMessage.h"

#include "Camera.h"
#include "Game.h"
#include "BitmapFont.h"
#include "LevelManager.h"
#include "ItemManager.h"
#include "ShopState.h"

#include "../SGD Wrappers/SGD_String.h"
#include "..\\TinyXML\tinyxml.h"

#include "chakram.h"

enum Direction { Up, Down, Left, Right};
enum Animations { playerWalkDown, playerStandDown, playerEarthShieldDown, playerAttackSlashDown, playerAttackLungeDown, playerCastDown, playerAttackSmashDown, playerAttackStabDown,
	playerWalkUp, playerStandUp, playerEarthShieldUp, playerAttackSlashUp, playerAttackLungeUp, playerCastUp, playerAttackSmashUp, playerAttackStabUp,
	playerWalkLeft, playerStandLeft, playerEarthShieldLeft, playerAttackSlashLeft, playerAttackLungeLeft, playerCastLeft, playerAttackSmashLeft, playerAttackStabLeft,
	playerWalkRight, playerStandRight, playerEarthShieldRight, playerAttackSlashRight, playerAttackLungeRight, playerCastRight, playerAttackSmashRight, playerAttackStabRight};
enum HUD { HealthBarBackground, HealthBarGreen, HealthBarYellow, HealthBarRed, HealthBarBorder, HealthBarIcon, BarBackground, BoxNormal, BoxSelected, StatusBackground, Potion, eFireArtifact, eIceArtifact, eLightningArtifact, eEarthArtifact, eMelee, eFireball, eChakram, eFireLunge, eSnowBall, eIceSlash, eIceWall, eLightningBolt, eLightningSlash, eLightningLunge, eEarthShard, eEarthSmash, eEarthShield, Vignette};

CPlayer::CPlayer(void)
{
	m_nUnspent = 0;
	m_nLevel = 1;
	m_nExperience = 0;
	m_nToNextLevel = 100;
	// Set below after giving items
	//m_nBody = 10;
	//m_nMind = 20;
	//m_nSoul = 10;
	//m_nCharacter = 10;
	//m_nMaxHealth = m_nCurrHealth = 30 + (m_nCharacter * 10);
	m_nHelpMessages = 0;
	m_nKeys = 0;
	m_nCurrency = 0;
	m_nShards = 0;
	TransitionTimer = 0.0f;
	m_nFacing = Down;
	m_fVelocityY = 0.0f;
	m_fVelocityX = 0.0f;
	m_nArtifact	 = 0;
	TransitionEffect = nullptr;
	DashEffect = nullptr;
	FreezeHeal = nullptr;
	SmashEffect = nullptr;
	IceSlashEffect = nullptr;
	LightningSlash = nullptr;
	RockShield = nullptr;
	m_nNumArt =	 4;
	m_nCurrAtr = 1;
	SetCurrAnim(-1);
	m_pIM = CInputManager::GetInstance();

	m_fTakeFireDamage = 0.0f;

	LevelUpEffect = new CParticleEffect;
	CParticleManager::GetInstance()->GetEffect(*LevelUpEffect, FX_LEVELUP);
	m_nLevelWindowID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/menubutton2.png"));
	m_fLevelUpTimer = 0.0f;
	m_nLevelUpAlpha = 0;

	m_nPotionCount = 1;

	//Status
	m_bOnFire = false;
	m_bOnIce  = false;
	m_bSlowed = false;

	//Ability
	m_bLunge = false;
	m_fLungeTimer = 0.0f;
	m_bEarthWall = false;
	m_bHealWall = false;
	m_bEarthSmash = false;
	m_bStab = false;
	m_bSwipe = false;
	m_bShoot = false;
	m_bChakram = false;
	m_bMelee = false;
	m_fEarthWalltimer = 0.0f;


	for ( int i = 0; i < 4; i++)
	{
		m_fAbCooldown[i] = 0.0f;
		m_fAbTimers[i] = 3.0f;

		m_fArtifactTimers[i] = 2.0f;
		m_fArtifactCooldown[i] = 0.0f;
	}

	m_fHealTimer = 3.0f;
	m_fHealCooldown = 0.0f;
	m_fDashEffectTimer = 0.0f;
	m_pBF			= CBitmapFont::GetInstance();
	//TEST
	m_pTM = CSGD_TextureManager::GetInstance();
	imgID = m_pTM->LoadTexture( _T("Resource/Graphics/BG.jpg") );

	// HUD Images
	// HealthBarBackground
	m_vHUDImgIDs.push_back(m_pTM->LoadTexture(_T("Resource/Graphics/HUD/health_bar_background.png")));
	// HealthBarGreen
	m_vHUDImgIDs.push_back(m_pTM->LoadTexture(_T("Resource/Graphics/HUD/health_bar_green.png")));
	// HealthBarYellow
	m_vHUDImgIDs.push_back(m_pTM->LoadTexture(_T("Resource/Graphics/HUD/health_bar_yellow.png")));
	// HealthBarRed
	m_vHUDImgIDs.push_back(m_pTM->LoadTexture(_T("Resource/Graphics/HUD/health_bar_red.png")));
	// HealthBarBorder
	m_vHUDImgIDs.push_back(m_pTM->LoadTexture(_T("Resource/Graphics/HUD/health_bar_border.png")));
	// HealthBarIcon
	m_vHUDImgIDs.push_back(m_pTM->LoadTexture(_T("Resource/Graphics/HUD/health_bar_icon.png")));
	// BarBackground
	m_vHUDImgIDs.push_back(m_pTM->LoadTexture(_T("Resource/Graphics/HUD/bar_background.png")));
	// BoxNormal
	m_vHUDImgIDs.push_back(m_pTM->LoadTexture(_T("Resource/Graphics/HUD/box_normal.png")));
	// BoxSelected
	m_vHUDImgIDs.push_back(m_pTM->LoadTexture(_T("Resource/Graphics/HUD/box_selected.png")));
	// StatusBackground
	m_vHUDImgIDs.push_back(m_pTM->LoadTexture(_T("Resource/Graphics/HUD/status_background.png")));
	// Potion
	m_vHUDImgIDs.push_back(m_pTM->LoadTexture(_T("Resource/Graphics/Items/Potion.png")));
	// FireArtifact
	m_vHUDImgIDs.push_back(m_pTM->LoadTexture(_T("Resource/Graphics/HUD/artifact_fire.png")));
	// IceArtifact
	m_vHUDImgIDs.push_back(m_pTM->LoadTexture(_T("Resource/Graphics/HUD/artifact_ice.png")));
	// LightningArtifact
	m_vHUDImgIDs.push_back(m_pTM->LoadTexture(_T("Resource/Graphics/HUD/artifact_lightning.png")));
	// EarthArtifact
	m_vHUDImgIDs.push_back(m_pTM->LoadTexture(_T("Resource/Graphics/HUD/artifact_earth.png")));
	// Melee
	m_vHUDImgIDs.push_back(m_pTM->LoadTexture(_T("Resource/Graphics/HUD/ability_melee.png")));
	// Fireball
	m_vHUDImgIDs.push_back(m_pTM->LoadTexture(_T("Resource/Graphics/HUD/ability_fireball.png")));
	// Chakram
	m_vHUDImgIDs.push_back(m_pTM->LoadTexture(_T("Resource/Graphics/HUD/ability_chakram.png")));
	// FireLunge
	m_vHUDImgIDs.push_back(m_pTM->LoadTexture(_T("Resource/Graphics/HUD/ability_firelunge.png")));
	// SnowBall
	m_vHUDImgIDs.push_back(m_pTM->LoadTexture(_T("Resource/Graphics/HUD/ability_snowball.png")));
	// IceSlash
	m_vHUDImgIDs.push_back(m_pTM->LoadTexture(_T("Resource/Graphics/HUD/ability_iceslash.png")));
	// IceWall
	m_vHUDImgIDs.push_back(m_pTM->LoadTexture(_T("Resource/Graphics/HUD/ability_icewall.png")));
	// LightningBolt
	m_vHUDImgIDs.push_back(m_pTM->LoadTexture(_T("Resource/Graphics/HUD/ability_lightningbolt.png")));
	// LightningSlash
	m_vHUDImgIDs.push_back(m_pTM->LoadTexture(_T("Resource/Graphics/HUD/ability_lightningslash.png")));
	// LightningLunge
	m_vHUDImgIDs.push_back(m_pTM->LoadTexture(_T("Resource/Graphics/HUD/ability_lightninglunge.png")));
	// EarthShard
	m_vHUDImgIDs.push_back(m_pTM->LoadTexture(_T("Resource/Graphics/HUD/ability_earthshard.png")));
	// EarthSmash
	m_vHUDImgIDs.push_back(m_pTM->LoadTexture(_T("Resource/Graphics/HUD/ability_earthsmash.png")));
	// EarthShield
	m_vHUDImgIDs.push_back(m_pTM->LoadTexture(_T("Resource/Graphics/HUD/ability_earthshield.png")));
	// Vignette
	m_vHUDImgIDs.push_back(m_pTM->LoadTexture(_T("Resource/Graphics/HUD/vignette.png")));
	
	//Artifacts
	m_nNumArt = 0;
	m_nCurrAtr = 0;

	SetWidth( 40);
	SetHeight( 60);
	SetType( ENT_PLAYER);

	// Animations:

	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-004
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	// Changed walking and standing animation passive rects
	// Note: Not in code anywhere, just in .XMLs

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-004
	//////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-008
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	// Changed walking and standing animation passive rects
	// Note: Not in code anywhere, just in .XMLs

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-008
	//////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-067
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	// Changed all passive rects for player animations
	// Note: Not in code anywhere, just in .XMLs

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-067
	//////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-064
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	// Changed all animation frame durations from 100 to 75 
	// Note: Not in code anywhere, just in .XMLs

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-064
	//////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-033
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	// Changed slashing down (and up) animation active rects
	// Note: Not in code anywhere, just in .XMLs

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-033
	//////////////////////////////////////////////////////////////////////////

	// None
	// Down
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneWalkDown")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneStandDown")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneBlockDown"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneSlashDown"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneLungeDown"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneCastDown")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneSmashDown"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneStabDown")); 

	// Up
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneWalkUp"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneStandUp")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneBlockUp")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneSlashUp"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneLungeUp"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneCastUp")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneSmashUp")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneStabUp")); 

	// Left
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneWalkLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneStandLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneBlockLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneSlashLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneLungeLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneCastLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneSmashLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneStabLeft"));

	// Right
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneWalkRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneStandRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneBlockRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneSlashRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneLungeRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneCastRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneSmashRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerNoneStabRight"));
	
	// Fire
	// Down
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireWalkDown")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireStandDown")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireBlockDown"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireSlashDown"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireLungeDown"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireCastDown")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireSmashDown"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireStabDown")); 

	// Up
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireWalkUp"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireStandUp")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireBlockUp")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireSlashUp"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireLungeUp"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireCastUp")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireSmashUp")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireStabUp")); 

	// Left
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireWalkLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireStandLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireBlockLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireSlashLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireLungeLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireCastLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireSmashLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireStabLeft"));

	// Right
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireWalkRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireStandRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireBlockRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireSlashRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireLungeRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireCastRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireSmashRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerFireStabRight"));
	
	// Ice
	// Down
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceWalkDown")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceStandDown")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceBlockDown"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceSlashDown"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceLungeDown"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceCastDown")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceSmashDown"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceStabDown")); 

	// Up
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceWalkUp"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceStandUp")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceBlockUp")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceSlashUp"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceLungeUp"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceCastUp")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceSmashUp")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceStabUp")); 

	// Left
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceWalkLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceStandLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceBlockLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceSlashLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceLungeLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceCastLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceSmashLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceStabLeft"));

	// Right
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceWalkRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceStandRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceBlockRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceSlashRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceLungeRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceCastRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceSmashRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerIceStabRight"));
	
	// Earth
	// Down
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthWalkDown")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthStandDown")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthBlockDown"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthSlashDown"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthLungeDown"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthCastDown")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthSmashDown"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthStabDown")); 

	// Up
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthWalkUp"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthStandUp")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthBlockUp")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthSlashUp"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthLungeUp"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthCastUp")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthSmashUp")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthStabUp")); 

	// Left
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthWalkLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthStandLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthBlockLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthSlashLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthLungeLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthCastLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthSmashLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthStabLeft"));

	// Right
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthWalkRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthStandRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthBlockRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthSlashRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthLungeRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthCastRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthSmashRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerEarthStabRight"));
	
	// Lightning
	// Down
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningWalkDown")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningStandDown")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningBlockDown"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningSlashDown"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningLungeDown"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningCastDown")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningSmashDown"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningStabDown")); 

	// Up
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningWalkUp"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningStandUp")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningBlockUp")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningSlashUp"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningLungeUp"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningCastUp")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningSmashUp")); 
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningStabUp")); 

	// Left
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningWalkLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningStandLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningBlockLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningSlashLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningLungeLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningCastLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningSmashLeft"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningStabLeft"));

	// Right
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningWalkRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningStandRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningBlockRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningSlashRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningLungeRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningCastRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningSmashRight"));
	CEntity::AddAnimInfo(AnimationManager::GetInstance()->GetAnimID("playerLightningStabRight"));

	SetCurrAnim(playerStandDown + 32 * 0);

	// Inventory
	CItem * tempWeapon = new CItem(*CItemManager::GetInstance()->GetItem(1));
	tempWeapon->SetEquipped(true);

	this->m_vItems.push_back(tempWeapon);

	CItem * tempArmor = new CItem(*CItemManager::GetInstance()->GetItem(2));
	tempArmor->SetEquipped(true);

	this->m_vItems.push_back(tempArmor);

	this->m_iPotion = CItemManager::GetInstance()->GetItem(0);

	SetBody(5);
	SetMind(5);
	SetSoul(5);
	SetCharacter(5);

	m_fAcceleration = (float)(150 + (m_nMind * 10));

	m_nMaxHealth = m_nCurrHealth = 30 + (m_nCharacter * 10);

	AddCurrency(50);

	CSGD_EventSystem::GetInstance()->RegisterClient("TAKE_DAMAGE", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("GAIN_EXPERIENCE", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("ADD_ARTIFACT", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("CAMERA_SHAKE", this);

	m_pCamera = Camera::GetInstance();
	m_pCamera->Enter();
}


CPlayer::~CPlayer(void)
{
	delete FreezeHeal;
	FreezeHeal = nullptr;
	
	delete TransitionEffect;
	TransitionEffect = nullptr;

	delete LevelUpEffect;
	LevelUpEffect = nullptr;

	delete DashEffect;
	DashEffect = nullptr;

	delete SmashEffect;
	SmashEffect = nullptr;

	delete IceSlashEffect;
	IceSlashEffect = nullptr;

	delete LightningSlash;
	LightningSlash = nullptr;

	delete RockShield;
	RockShield = nullptr;

	if (m_nLevelWindowID != -1)
	{
		CSGD_TextureManager::GetInstance()->UnloadTexture(m_nLevelWindowID);
		m_nLevelWindowID = -1;
	}

	for (unsigned int i = 0; i < m_vArtifacts.size(); i++)
	{
		delete m_vArtifacts[i];
		m_vArtifacts[i] = nullptr;
	}	

	this->m_vArtifacts.clear();

	for(unsigned int i = 0; i < m_vHUDImgIDs.size(); i++)
	{
		this->m_pTM->UnloadTexture(this->m_vHUDImgIDs[i]);
	}

	this->m_vHUDImgIDs.clear();

	for(unsigned int i = 0; i < this->m_vItems.size(); i++)
	{
		if(this->m_vItems[i]->HasGem())
		{
			delete this->m_vItems[i]->GetGem();
			this->m_vItems[i]->SetGem(nullptr);
		}

		delete this->m_vItems[i];
		this->m_vItems[i] = nullptr;
	}

	CSGD_EventSystem::GetInstance()->UnregisterClientAll(this);

	CEntity::~CEntity();
}


void CPlayer::ClearParticleEffects()
{
	if (DashEffect != nullptr)
	{
		delete DashEffect;
		DashEffect = nullptr;
	}
}


void CPlayer::Render()
{
	if (SmashEffect != nullptr)
		SmashEffect->Render();

	int x = CGame::GetInstance()->GetWindowWidth() / 2;
	int y = CGame::GetInstance()->GetWindowHeight() / 2;

	bool RenderPlayer = true;
	if (m_nCurrAtr == 4 && m_bLunge == true)
		RenderPlayer = false;

	if (RenderPlayer)
	AnimationManager::GetInstance()->Draw(*this->GetAnimationInformation(this->GetCurrAnim()), x,y);

	if (m_fLevelUpTimer != 0.0f)
	{
		RECT Cam;
		Camera::GetInstance()->CamRect(&Cam);
		LevelUpEffect->SetXPosition(GetPosX() - Cam.left);
		LevelUpEffect->SetYPosition(GetPosY() - Cam.top);

		CSGD_TextureManager::GetInstance()->Draw(m_nLevelWindowID, GetPosX() - 150 - Cam.left, GetPosY() - 200 - Cam.top, 1.7f,
			1.0f,0,0,0,0,D3DCOLOR_ARGB(m_nLevelUpAlpha,255,255,100));
		m_pBF->Draw(LevelMessage, GetPosX() - 70 - Cam.left, GetPosY() - 180 - Cam.top, 0.8f,D3DCOLOR_ARGB(m_nLevelUpAlpha,255,255,255));

		LevelUpEffect->Render();
	}

	if (DashEffect != nullptr)
		DashEffect->Render();

	if (IceSlashEffect != nullptr)
		IceSlashEffect->Render();
	if (FreezeHeal != nullptr)
		FreezeHeal->Render();
	if (LightningSlash != nullptr)
		LightningSlash->Render();
	if (RockShield != nullptr)
		RockShield->Render();
	if (TransitionEffect != nullptr)
		TransitionEffect->Render();

}

void CPlayer::Update( float deltaTime )
{
	UpdateEmitters( deltaTime );
	UpdateCooldowns(deltaTime);
	
	if(m_fVelocityX == 0.0f && m_fVelocityY == 0.0f && (m_nCurrAnim == playerWalkDown + 32 * 0 || m_nCurrAnim == playerWalkDown + 32 * 1 || m_nCurrAnim == playerWalkDown + 32 * 2 || m_nCurrAnim == playerWalkDown + 32 * 3 || m_nCurrAnim == playerWalkDown + 32 * 4 || m_nCurrAnim == playerWalkUp + 32 * 0 || m_nCurrAnim == playerWalkUp + 32 * 1 || m_nCurrAnim == playerWalkUp + 32 * 2 || m_nCurrAnim == playerWalkUp + 32 * 3 || m_nCurrAnim == playerWalkUp + 32 * 4 || m_nCurrAnim == playerWalkLeft + 32 * 0 || m_nCurrAnim == playerWalkLeft + 32 * 1 || m_nCurrAnim == playerWalkLeft + 32 * 2 || m_nCurrAnim == playerWalkLeft + 32 * 3 || m_nCurrAnim == playerWalkLeft + 32 * 4 || m_nCurrAnim == playerWalkRight + 32 * 0 || m_nCurrAnim == playerWalkRight + 32 * 1 || m_nCurrAnim == playerWalkRight + 32 * 2 || m_nCurrAnim == playerWalkRight + 32 * 3 || m_nCurrAnim == playerWalkRight + 32 * 4))
	{
		switch(m_nFacing)
		{
		case 0:
			{
				SetCurrAnim(playerStandUp + 32 * m_nCurrAtr);
			}
			break;
		case 1:
			{
				SetCurrAnim(playerStandDown + 32 * m_nCurrAtr);
			}
			break;
		case 2:
			{
				SetCurrAnim(playerStandLeft + 32 * m_nCurrAtr);
			}
			break;
		case 3:
			{
				SetCurrAnim(playerStandRight + 32 * m_nCurrAtr);
			}
			break;
		}
	}

	SetPosX(GetPosX() + (int)(m_fVelocityX * deltaTime));
	SetPosY(GetPosY() + (int)(m_fVelocityY * deltaTime));
	
	//Check players status
	m_bOnIce =  LevelManager::GetInstance()->OnIce(this);
	m_bOnFire = LevelManager::GetInstance()->OnFire(this);


	if (m_bOnFire && m_nCurrAtr != 1 && m_fTakeFireDamage <= 0)
	{
		m_fTakeFireDamage = 1.0f;
		ModifyHealth(-5);
	}
		

	if(this->GetCurrAnim() != -1)
	{
		this->GetAnimationInformation(this->GetCurrAnim())->Update(deltaTime);
	}

	m_pCamera->Center( this);


	if (m_bLunge)
		Lunge();

	else if ( !m_bOnIce ||  m_nCurrAtr == 2)
		m_fVelocityX = m_fVelocityY = 0;
	else
	{
		if (m_fVelocityY > m_fAcceleration)
			m_fVelocityY = m_fAcceleration;
		else if (m_fVelocityY < -m_fAcceleration)
			m_fVelocityY = -m_fAcceleration;

		if (m_fVelocityX > m_fAcceleration)
			m_fVelocityX = m_fAcceleration;
		else if (m_fVelocityX < -m_fAcceleration)
			m_fVelocityX = -m_fAcceleration;

	}

	if (m_bEarthWall)
		CreateRockWall();
	else if (m_bEarthSmash)
		EarthSmash();
	else if (m_bStab)
		Stab();
	else if(m_bHealWall)
		HealWall();
	else if (m_bSwipe)
		SwordSwipe();
	else if (m_bShoot)
		Shoot();
	else if (m_bMelee)
		Melee();
	else if (m_bChakram)
		Chakram();

	//Tile Collision
	LevelManager* LM = LevelManager::GetInstance();
	RECT rOverlap;

	rOverlap = LM->Collision(this);

	if (rOverlap.left != 0 && rOverlap.right != 0)
	{
		m_fLungeTimer = 0.0f;
		if (DashEffect != nullptr)
			DashEffect->StopEmitting();

		int nOVerWidth = rOverlap.right - rOverlap.left;
		int nOverHeight = rOverlap.bottom - rOverlap.top;

		int nCenterOverX = nOVerWidth + rOverlap.left;
		int nCenterOverY = nOverHeight + rOverlap.top;

		if (m_bLunge)
		{	
			/////////////////////////////////////////////////////////////////
			// BUG FIX
			// Reference Bug # BB-028
			// BUG FIX START
			/////////////////////////////////////////////////////////////////
			StopDash();
			///////////////////////////////////////////////////////////////////////////
			// BUG FIX END  Reference # BB-028
			//////////////////////////////////////////////////////////////////////////
			LevelManager* LM = LevelManager::GetInstance();
			RECT rOverlap;

			rOverlap = LM->Collision(this);

			while (rOverlap.left != 0 && rOverlap.right != 0)
			{
				int nOVerWidth = rOverlap.right - rOverlap.left;
				int nOverHeight = rOverlap.bottom - rOverlap.top;

				int nCenterOverX = nOVerWidth + rOverlap.left;
				int nCenterOverY = nOverHeight + rOverlap.top;

				switch(GetFacing())
				{
				case 0:
					{
						SetPosY( GetPosY() + nOverHeight);
					}
					break;

				case 1:
					{
						SetPosY( GetPosY() - nOverHeight);
					}
					break;
				case 2:
					{
						SetPosX( GetPosX() + nOVerWidth);

					}
					break;
				case 3:
					{
						SetPosX( GetPosX() - nOVerWidth);
					}
					break;
				}

				SetVelX(0);
				SetVelY(0);
				rOverlap = LM->Collision(this);
			}
		}
		else
		{
			if (m_bLunge)
				StopDash();

			if (nOverHeight < nOVerWidth)
			{
				//move up or down
				if (GetRect().top == rOverlap.top)
					SetPosY( GetPosY() + nOverHeight);
				else
					SetPosY( GetPosY() - nOverHeight);

			}
			else
			{
				//move left or right
				if (GetRect().left == rOverlap.left)
					SetPosX( GetPosX() + nOVerWidth);
				else
					SetPosX( GetPosX() - nOVerWidth);
			}

			SetVelX(0);
			SetVelY(0);
		}
	}

	if(m_nCurrHealth <= 0)
	{
		//Open up 'Death' state
		PlayState::GetInstance()->GoToDeath();

		/////////////////////////////////////////////////////////////////
		// BUG FIX
		// Reference Bug # BB-025
		// BUG FIX START
		/////////////////////////////////////////////////////////////////

		m_nFacing = Down; // Resets the facing of the player
		SetCurrAnim(playerStandDown + 32 * m_nCurrAtr); // Resets the animation of the player to the standing animation

		///////////////////////////////////////////////////////////////////////////
		// BUG FIX END  Reference # BB-025
		//////////////////////////////////////////////////////////////////////////
		
		ModifyHealth(m_nMaxHealth);
	}
}

void CPlayer::LevelUpArtifact( int index )
{
	if (index < m_nNumArt)
	m_vArtifacts[index]->LevelUp();
}

void CPlayer::ClearEmitters( void )
{
	TransitionEffect = nullptr;
	DashEffect = nullptr;
	FreezeHeal = nullptr;
	SmashEffect = nullptr;
	IceSlashEffect = nullptr;
	LightningSlash = nullptr;
	RockShield = nullptr;
}


void CPlayer::UpdateEmitters( float deltaTime )
{
	if (DashEffect != nullptr)
	{
		DashEffect->SetXPosition(GetPosX() - 15);
		DashEffect->SetYPosition(GetPosY() - 64);
		DashEffect->Update( deltaTime );
		if (DashEffect->IsDead())
		{
			delete DashEffect;
			DashEffect = nullptr;
		}
	}

	if (RockShield != nullptr)
	{
		RockShield->SetXPosition(GetPosX());
		RockShield->SetYPosition(GetPosY() - 32);
		RockShield->Update( deltaTime );
		if (RockShield->IsDead())
		{
			delete RockShield;
			RockShield = nullptr;
		}
	}

	if (FreezeHeal != nullptr)
	{
		FreezeHeal->Update( deltaTime );
		if (FreezeHeal->IsDead())
		{
			delete FreezeHeal;
			FreezeHeal = nullptr;
		}

	}

	if (IceSlashEffect != nullptr)
	{
		IceSlashEffect->Update( deltaTime );
		if (IceSlashEffect->IsDead())
		{
			delete IceSlashEffect;
			IceSlashEffect = nullptr;
		}

	}

	if (LightningSlash != nullptr)
	{
		LightningSlash->Update( deltaTime );
		if (LightningSlash->IsDead())
		{
			delete LightningSlash;
			LightningSlash = nullptr;
		}

	}

	if (SmashEffect != nullptr)
	{
		SmashEffect->Update( deltaTime );
		if (SmashEffect->IsDead())
		{
			delete SmashEffect;
			SmashEffect = nullptr;
		}
	}

	if (TransitionEffect != nullptr)
	{

		if (m_nCurrAtr == 4)
		{
			TransitionEffect->SetXPosition(GetPosX() - 15);
			TransitionEffect->SetYPosition(GetPosY() - 55);
		}
		else
		{
			TransitionEffect->SetXPosition(GetPosX());
			TransitionEffect->SetYPosition(GetPosY());
		}

		TransitionEffect->Update( deltaTime );
		if (TransitionEffect->IsDead())
		{
			delete TransitionEffect;
			TransitionEffect = nullptr;
		}
	}

	if (TransitionTimer > 0.0f)
	{
		TransitionTimer -= deltaTime;
		if (TransitionTimer <= 0.0f)
		{
			TransitionTimer = 0.0f;
			if (TransitionEffect)
				TransitionEffect->StopEmitting();
		}
	}

}

void CPlayer::UpdateCooldowns(float deltaTime)
{
	if (m_fLevelUpTimer != 0.0f)
	{
		LevelUpEffect->Update( deltaTime );
		m_fLevelUpTimer -= deltaTime;
		m_nLevelUpAlpha = (int)(255 * (m_fLevelUpTimer / 3.0f));
		if (m_fLevelUpTimer <= 0.0f)
			m_fLevelUpTimer = 0.0f;
	}

	for (int i = 0; i < 4; i++)
	{
		m_fAbCooldown[i] -= deltaTime;
		m_fArtifactCooldown[i] -= deltaTime;

		if ( m_fArtifactCooldown[i] <= 0.01)
			m_fArtifactCooldown[i] = 0.0f;

		if ( m_fAbCooldown[i] <= 0.01)
			m_fAbCooldown[i] = 0.0f;
	}

	m_fHealCooldown -= deltaTime;
	if(m_fHealCooldown <= 0.01)
		m_fHealCooldown = 0.0f;

	m_fLungeTimer -= deltaTime;
	if(m_fLungeTimer <= 0.001)
		m_fLungeTimer = 0.0f;

	m_fEarthWalltimer -= deltaTime;
	if(m_fEarthWalltimer <= 0.001)
		m_fEarthWalltimer = 0.0f;


	m_fTakeFireDamage -= deltaTime;
	if (m_fTakeFireDamage <= 0.01)
		m_fTakeFireDamage = 0;
}

bool CPlayer::CheckCollision(IEntity* pOther )
{
	RECT rOverlap;
	RECT rOther;

	rOther = pOther->GetRect();

	if(ActiveRecCollision(pOther))
		return true;


	if (m_bEarthWall)
	{
		if (CollidingWith(rEWall, pOther, &rOverlap))
		{
			CEntity* Temp = dynamic_cast<CEntity*> (pOther);
			if (Temp->GetType() == ENT_PROJECTILE)
			{
				CProjectile* Pr = dynamic_cast<CProjectile*> (pOther);

				CDeleteProjectileMessage* Msg = new CDeleteProjectileMessage(Pr);
				CMessageSystem::GetInstace()->SendMsg(Msg);
			}
		}
	}

	while (CollidingWith(pOther, &rOverlap))
	{
		
		CEntity* Temp = dynamic_cast<CEntity*> (pOther);

		if (Temp->GetType() == ENT_PROJECTILE)
		{
			CProjectile* Pr = dynamic_cast<CProjectile*> (pOther);

			if(dynamic_cast<chakram *>(Pr) != nullptr && m_nCurrAtr == 1)
			{
				// Nothing
			}
			else
			{
				/////////////////////////////////////////////////////////////////
				// BUG FIX
				// Reference Bug # BB-043
				// BUG FIX START
				/////////////////////////////////////////////////////////////////
				if (!m_bEarthWall)
					ModifyHealth(-Pr->GetDamage());
				///////////////////////////////////////////////////////////////////////////
				// BUG FIX END  Reference # BB-043
				//////////////////////////////////////////////////////////////////////////
			}

			CDeleteProjectileMessage* Msg = new CDeleteProjectileMessage(Pr);
			CMessageSystem::GetInstace()->SendMsg(Msg);

			return true;
		}

		pOther->CheckCollision(this);

		int nOVerWidth = rOverlap.right - rOverlap.left;
		int nOverHeight = rOverlap.bottom - rOverlap.top;

		int nCenterOverX = nOVerWidth + rOverlap.left;
		int nCenterOverY = nOverHeight + rOverlap.top;

		int nCenterOtherX = (rOther.right - rOther.left) + rOther.left;
		int nCebterOtherY =  (rOther.bottom - rOther.top) + rOther.top;

		if (nOverHeight < nOVerWidth)
		{
			//move up or down
			if (nCenterOverY < nCebterOtherY)
				SetPosY( GetPosY() - nOverHeight);
			else
				SetPosY( GetPosY() + nOverHeight);
		}
		else
		{
			//move left or right
			if (nCenterOverX < nCenterOtherX)
				SetPosX( GetPosX() - nOVerWidth);
			else
				SetPosX( GetPosX() + nOVerWidth);
		}


		if(m_bLunge)
		{
			int damage;

			if (DashEffect)
			DashEffect->StopEmitting();

			if (m_nCurrAtr == 1)
				{
				switch(m_vArtifacts[m_nCurrAtr-1]->GetLevel())
				{
				case 3:
					
				case 2:
					{
						damage = -30;
						if (m_nCurrAtr == 4)
							CSGD_EventSystem::GetInstance()->SendEvent(
							"TAKE_FIRE_DAMAGE", &damage, pOther, this);
						damage = -m_nMeleeDamage;
						if (rand() % 100 < m_nCritChance)
							damage = (int)(damage * 1.5f);
						CSGD_EventSystem::GetInstance()->SendEventNow(
							"TAKE_DAMAGE", &damage, pOther, this );
						m_bLunge = false;
					}
					break;
				case 1:
					{
						damage = -10;
						if (m_nCurrAtr == 4)
							CSGD_EventSystem::GetInstance()->SendEvent(
							"TAKE_FIRE_DAMAGE", &damage, pOther, this);
					}
				case 0:
					{
						damage = -m_nMeleeDamage;
						if (rand() % 100 < m_nCritChance)
							damage = (int)(damage * 1.5f);
						CSGD_EventSystem::GetInstance()->SendEventNow(
							"TAKE_DAMAGE", &damage, pOther, this );
						m_bLunge = false;
					}
					break;
				}

				switch(m_nFacing)
				{
				case 0:
					{
						SetCurrAnim(playerStandUp + 32 * m_nCurrAtr);
					}
					break;
				case 1:
					{
						SetCurrAnim(playerStandDown + 32 * m_nCurrAtr);
					}
					break;
				case 2:
					{
						SetCurrAnim(playerStandLeft + 32 * m_nCurrAtr);
					}
					break;
				case 3:
					{
						SetCurrAnim(playerStandRight + 32 * m_nCurrAtr);
					}
					break;
				}

				return true;
			}

			if (m_nCurrAtr == 4)
			{
				switch(m_vArtifacts[m_nCurrAtr-1]->GetLevel())
				{
				case 3:
					{
						damage = -30;
						if (m_nCurrAtr == 4)
							CSGD_EventSystem::GetInstance()->SendEventNow(
							"TAKE_LIGHTNING_DAMAGE", &damage, pOther, this);
						damage = -0;
						if (40 > rand() % 100)
							CSGD_EventSystem::GetInstance()->SendEventNow(
							"STUN", &damage, pOther, this );

					}
					break;
				case 2:
					{
						damage = -0;
						if (20 > rand() % 100)
							CSGD_EventSystem::GetInstance()->SendEventNow(
							"STUN", &damage, pOther, this );
						m_bLunge = false;
					}
				case 1:
					{
						damage = -m_nMeleeDamage;
						if (rand() % 100 < m_nCritChance)
							damage = (int)(damage * 1.5f);
						if (m_nCurrAtr == 4)
							CSGD_EventSystem::GetInstance()->SendEvent(
							"TAKE_LIGHTNING_DAMAGE", &damage, pOther, this);
						m_bLunge = false;
					}
					break;
				case 0:
					{
						damage = -m_nMeleeDamage;
						if (rand() % 100 < m_nCritChance)
							damage = (int)(damage * 1.5f);
						CSGD_EventSystem::GetInstance()->SendEventNow(
							"TAKE_DAMAGE", &damage, pOther, this );
						m_bLunge = false;
					}
					break;
				}
				return true;
			}

			m_bLunge = false;

			m_fLungeTimer = 0.0f;
			if (DashEffect != nullptr)
				DashEffect->StopEmitting();
		}

		/////////////////////////////////////////////////////////////////
		// BUG FIX
		// Reference Bug # BB-094
		// BUG FIX START
		/////////////////////////////////////////////////////////////////

		// Reset animation only if walking (or dashing (above))
		if(!(m_bEarthWall || m_bEarthSmash || m_bStab || m_bHealWall || m_bSwipe || m_bShoot || m_bMelee || m_bChakram))
		{
			switch(m_nFacing)
			{
			case 0:
				{
					SetCurrAnim(playerStandUp + 32 * m_nCurrAtr);
				}
				break;
			case 1:
				{
					SetCurrAnim(playerStandDown + 32 * m_nCurrAtr);
				}
				break;
			case 2:
				{
					SetCurrAnim(playerStandLeft + 32 * m_nCurrAtr);
				}
				break;
			case 3:
				{
					SetCurrAnim(playerStandRight + 32 * m_nCurrAtr);
				}
				break;
			}

			///////////////////////////////////////////////////////////////////////////
			// BUG FIX END  Reference # BB-094
			//////////////////////////////////////////////////////////////////////////
		}

		return true;
	}


	return false;
}

bool CPlayer::CollidingWith ( IEntity* pOther, RECT* rOver )
{

	RECT rSelf = GetRect();
	RECT rOther	= pOther->GetRect();

	if( IntersectRect( rOver, &rSelf, &rOther ) == TRUE )
		return true;
	else
		return false;

}

bool CPlayer::CollidingWith ( RECT rRec ,IEntity* pOther, RECT* rOver  )
{
	RECT rOther	= pOther->GetRect();

	if( IntersectRect( rOver, &rRec, &rOther ) == TRUE )
		return true;
	else
		return false;

}

void CPlayer::ModifyHealth(int nAmount)
{
	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-048
	// BUG FIX START
	/////////////////////////////////////////////////////////////////
	if (nAmount < 0)
	{
		if (rand() % 100  < m_nDodgeChance)
			return; // Player has dodged. Need sound effect / animation / message
	}
	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-048
	//////////////////////////////////////////////////////////////////////////

	m_nCurrHealth += nAmount;

	if (nAmount < 0)
		CInputManager::GetInstance()->Vibrate();

	if (m_nCurrHealth <= 0)
	{
		m_nCurrHealth = 0;
	}

	if (m_nCurrHealth > m_nMaxHealth)
		m_nCurrHealth = m_nMaxHealth;

}

bool CPlayer::Input( )
{

	if (m_pIM->CheckPause())
	{
		//Go to pause menu
	}

	if(m_pIM->CheckHeal())
	{
		if(m_fHealCooldown <= 0 && this->m_nPotionCount > 0 && this->m_nCurrHealth != this->m_nMaxHealth)
		{
			CAudioManager::GetInstance()->PlaySFX(SFX_POTION);
			// Possible notification
			int nHealed = (int)(GetMaxHealth() * ( m_iPotion->GetSpecial() / 100.0f));
			ModifyHealth(nHealed);
			this->m_nPotionCount--;
			m_fHealCooldown = m_fHealTimer;
		}
	}


	if (m_bLunge || m_bEarthSmash || m_bLunge || m_bStab || m_bHealWall || m_bSwipe || m_bShoot || m_bMelee || m_bChakram)
		return true;


	if (m_pIM->CheckUpD())
	{
		m_nFacing = Up;

		if (m_bOnIce && m_nCurrAtr != 2)
			m_fVelocityY -= m_fAcceleration/100;
		else
			m_fVelocityY = -m_fAcceleration;

		m_fVelocityX = 0;

		this->SetCurrAnim(playerWalkUp + 32 * m_nCurrAtr);

		if(m_pIM->CheckUpEx())
			this->GetAnimationInformation(this->GetCurrAnim())->Reset();
	}
	else if (m_pIM->CheckDownD())
	{
		m_nFacing = Down;

		if (m_bOnIce && m_nCurrAtr != 2)
			m_fVelocityY += m_fAcceleration/100;
		else
			m_fVelocityY = +m_fAcceleration;
		m_fVelocityX = 0;

		this->SetCurrAnim(playerWalkDown + 32 * m_nCurrAtr);

		if(m_pIM->CheckDownEx())
			this->GetAnimationInformation(this->GetCurrAnim())->Reset();
	}
	else if (m_pIM->CheckRightD())
	{
		m_nFacing = Right;

		if (m_bOnIce && m_nCurrAtr != 2)
			m_fVelocityX += m_fAcceleration/100;
		else
			m_fVelocityX = +m_fAcceleration;
		m_fVelocityY = 0;

		this->SetCurrAnim(playerWalkRight + 32 * m_nCurrAtr);

		if(m_pIM->CheckRightEx())
			this->GetAnimationInformation(this->GetCurrAnim())->Reset();
	}
	else if (m_pIM->CheckLeftD())
	{
		m_nFacing = Left;

		if (m_bOnIce && m_nCurrAtr != 2)
			m_fVelocityX -= m_fAcceleration/100;
		else
			m_fVelocityX = -m_fAcceleration;
		m_fVelocityY = 0;

		this->SetCurrAnim(playerWalkLeft + 32 * m_nCurrAtr);

		if(m_pIM->CheckLeftEx())
			this->GetAnimationInformation(this->GetCurrAnim())->Reset();
	}

	if ( m_bEarthWall)
		return true;

	if (m_pIM->CheckMelee())
	{
		if(LevelManager::GetInstance()->OnShop(this))
		{
			CGame::GetInstance()->ChangeToNew(CShopState::GetInstance());
		}
		else if (m_fAbCooldown[0] <= 0)
		{
			Melee();
		}
	}
	
	if (m_nNumArt > 0)
	{
		if (m_pIM->CheckCycleLeft())
		{		
			m_nCurrAtr--;

			if (m_nCurrAtr == 0)
				m_nCurrAtr = m_nNumArt;

			for( int i = 1; i < 4; i++)
				m_fAbCooldown[i] = m_fAbTimers[i];
			ArtifactTransition();

			/////////////////////////////////////////////////////////////////
			// BUG FIX
			// Reference Bug # BB-036, Part 1/2
			// BUG FIX START
			/////////////////////////////////////////////////////////////////

			switch(m_nFacing) // Set player animation to standing based on facing
			{
			case Up:
				{
					SetCurrAnim(playerStandUp + 32 * m_nCurrAtr);
				}
				break;
			case Down:
				{
					SetCurrAnim(playerStandDown + 32 * m_nCurrAtr);
				}
				break;
			case Left:
				{
					SetCurrAnim(playerStandLeft + 32 * m_nCurrAtr);
				}
				break;
			case Right:
				{
					SetCurrAnim(playerStandRight + 32 * m_nCurrAtr);
				}
				break;
			}

			///////////////////////////////////////////////////////////////////////////
			// BUG FIX END  Reference # BB-036, Part 1/2
			//////////////////////////////////////////////////////////////////////////
		}
		else if (m_pIM->CheckCycleRight())
		{
			m_nCurrAtr++;

			if (m_nCurrAtr > m_nNumArt)
				m_nCurrAtr = 1;

			for( int i = 1; i < 4; i++)
				m_fAbCooldown[i] = m_fAbTimers[i];
			ArtifactTransition();

			/////////////////////////////////////////////////////////////////
			// BUG FIX
			// Reference Bug # BB-036, Part 2/2
			// BUG FIX START
			/////////////////////////////////////////////////////////////////			

			switch(m_nFacing) // Set player animation based on facing
			{
			case Up:
				{
					SetCurrAnim(playerStandUp + 32 * m_nCurrAtr);
				}
				break;
			case Down:
				{
					SetCurrAnim(playerStandDown + 32 * m_nCurrAtr);
				}
				break;
			case Left:
				{
					SetCurrAnim(playerStandLeft + 32 * m_nCurrAtr);
				}
				break;
			case Right:
				{
					SetCurrAnim(playerStandRight + 32 * m_nCurrAtr);
				}
				break;
			}

			///////////////////////////////////////////////////////////////////////////
			// BUG FIX END  Reference # BB-036, Part 2/2
			//////////////////////////////////////////////////////////////////////////
		}

		if (m_pIM->CheckAb1())
		{
			if (m_fAbCooldown[1] <= 0)
			{
				Shoot();
			}

		}

		if (m_pIM->CheckAb2())
		{
			if (m_fAbCooldown[2] <= 0)
			{
				switch( m_nCurrAtr )
				{
				case 1:
					Chakram();
					break;

				case 2:
					Stab();
					break;

				case 3:
					EarthSmash();
					break;

				case 4:
					SwordSwipe();					
					break;
				}
				m_fAbCooldown[2] = m_fAbTimers[2];
			}
		}


		if (m_pIM->CheckAb3())
		{
			if (m_fAbCooldown[3] <= 0)
			{
				//activate ability 3
				switch( m_nCurrAtr )
				{
				case 1:
					Lunge();
					break;

				case 2:
					HealWall();
					break;

				case 3:
					CreateRockWall();
					break;
				case 4:
					Lunge();
					break;

				}
				m_fAbCooldown[3] = m_fAbTimers[3];
			}
		}
	}

	if(m_pIM->NonePressed())
	{
		switch( m_nFacing )
		{
		case 0:
			SetCurrAnim(playerStandUp + 32 * m_nCurrAtr);
			break;

		case 1:
			SetCurrAnim(playerStandDown + 32 * m_nCurrAtr);
			break;

		case 2:
			SetCurrAnim(playerStandLeft + 32 * m_nCurrAtr);
			break;

		case 3:
			SetCurrAnim(playerStandRight + 32 * m_nCurrAtr);
			break;
		}
	}

	return true;
}

void CPlayer::DrawVignette()
{
	if(LevelManager::GetInstance()->GetLevel()->IsShadowed())
	{
		m_pTM->Draw(m_vHUDImgIDs[Vignette], 0, 0);
	}
}

void CPlayer::DrawHUD()
{
	// Health bar
	int nPosX = 40;
	int nPosY = 475;

	m_pTM->Draw(m_vHUDImgIDs[HealthBarBackground], nPosX + 8, nPosY + 31);
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();

	RECT HealthBr;
	HealthBr.bottom = nPosY + 36 + 46;
	HealthBr.left = nPosX + 11;
	HealthBr.top = nPosY + 32;

	HealthBr.right = (LONG)(HealthBr.left + ( 171 * (m_nCurrHealth / (float)m_nMaxHealth)));

	RECT rRect;
	rRect.top = 0;
	rRect.bottom = 52;
	rRect.left = 0;
	rRect.right = (LONG)( 176 * (m_nCurrHealth / (float)m_nMaxHealth));

	if(m_nCurrHealth / (float)m_nMaxHealth >= 0.66f)
	{
		m_pTM->Draw(m_vHUDImgIDs[HealthBarGreen], nPosX + 7, nPosY + 30, 1.0f, 1.0f, &rRect);
	}
	else if(m_nCurrHealth / (float)m_nMaxHealth >= 0.33f)
	{
		m_pTM->Draw(m_vHUDImgIDs[HealthBarYellow], nPosX + 7, nPosY + 30, 1.0f, 1.0f, &rRect);
	}
	else
	{
		m_pTM->Draw(m_vHUDImgIDs[HealthBarRed], nPosX + 7, nPosY + 30, 1.0f, 1.0f, &rRect);
	}

	m_pTM->Draw(m_vHUDImgIDs[HealthBarBorder], nPosX + 7, nPosY + 30);
	m_pTM->Draw(m_vHUDImgIDs[HealthBarIcon], nPosX + 15, nPosY + 45);

	string szString = "HP: " + to_string(GetCurrHealth()) + " / " + to_string(GetMaxHealth());

	CBitmapFont::GetInstance()->Draw(szString, nPosX + 40, nPosY + 50, 0.4f, D3DCOLOR_ARGB(255, 0, 0, 0));

	// Ability bar and Cooldowns
	RECT ability;
	RECT abcooldown;

	m_pTM->Draw(m_vHUDImgIDs[BarBackground], nPosX + 195, nPosY + 30);
	m_pTM->Draw(m_vHUDImgIDs[BoxNormal], nPosX + 206, nPosY + 38);
	m_pTM->Draw(m_vHUDImgIDs[BoxNormal], nPosX + 255, nPosY + 38);
	m_pTM->Draw(m_vHUDImgIDs[BoxNormal], nPosX + 304, nPosY + 38);
	m_pTM->Draw(m_vHUDImgIDs[BoxNormal], nPosX + 353, nPosY + 38);

	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();

	RECT melee;
	melee.top = 5;
	melee.left = 100;
	melee.bottom = 33;
	melee.right = 127;

	CSGD_TextureManager::GetInstance()->Draw(m_vHUDImgIDs[eMelee], nPosX + 210, nPosY + 41);

    if(this->m_nCurrAtr == 1)
	{
	CSGD_TextureManager::GetInstance()->Draw(m_vHUDImgIDs[eFireball], nPosX + 255, nPosY + 38);
	CSGD_TextureManager::GetInstance()->Draw(m_vHUDImgIDs[eChakram], nPosX + 304, nPosY + 38);
	CSGD_TextureManager::GetInstance()->Draw(m_vHUDImgIDs[eFireLunge], nPosX + 353, nPosY + 38);
	}
	else if(this->m_nCurrAtr == 2)
	{
	CSGD_TextureManager::GetInstance()->Draw(m_vHUDImgIDs[eSnowBall], nPosX + 255, nPosY + 38);
	CSGD_TextureManager::GetInstance()->Draw(m_vHUDImgIDs[eIceSlash], nPosX + 304, nPosY + 38);
	CSGD_TextureManager::GetInstance()->Draw(m_vHUDImgIDs[eIceWall], nPosX + 353, nPosY + 38);
	}
	else if(this->m_nCurrAtr == 3)
	{
		CSGD_TextureManager::GetInstance()->Draw(m_vHUDImgIDs[eEarthShard], nPosX + 255, nPosY + 38);
		CSGD_TextureManager::GetInstance()->Draw(m_vHUDImgIDs[eEarthSmash], nPosX + 304, nPosY + 38);
		CSGD_TextureManager::GetInstance()->Draw(m_vHUDImgIDs[eEarthShield], nPosX + 353, nPosY + 38);
	}
	else if (this->m_nCurrAtr  == 4)
	{
		CSGD_TextureManager::GetInstance()->Draw(m_vHUDImgIDs[eLightningBolt], nPosX + 255, nPosY + 38);
		CSGD_TextureManager::GetInstance()->Draw(m_vHUDImgIDs[eLightningSlash], nPosX + 304, nPosY + 38);
		CSGD_TextureManager::GetInstance()->Draw(m_vHUDImgIDs[eLightningLunge], nPosX + 353, nPosY + 38);
	}

	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();

	for ( int i = 0; i < 4; i++)
	{
		ability.bottom = 560;
		ability.left = (270 + (50 * i));
		ability.right = (270 + (50 * (i + 1)));
		ability.top = 510;

		if (m_fAbCooldown[i] > 0.01f)
		{
			abcooldown.left = (nPosX + 208 + (50 * i) - (1 * i));
			abcooldown.right = abcooldown.left + 41;
			abcooldown.top = nPosY + 39;
			abcooldown.bottom = (LONG)(nPosY + 39 + (35 * ( m_fAbCooldown[i] / m_fAbTimers[i])));

			CSGD_Direct3D::GetInstance()->DrawRect(abcooldown, 75, 75, 75);
		}

	}

	// Potions
	m_pTM->Draw(m_vHUDImgIDs[StatusBackground], nPosX + 423, nPosY + 31);

	if(this->m_nPotionCount != 0)
	{
		m_pTM->Draw(m_vHUDImgIDs[Potion], nPosX + 440, nPosY + 40);
	}
	else
	{
		m_pTM->Draw(m_vHUDImgIDs[Potion], nPosX + 440, nPosY + 40, 1.0f, 1.0f, 0, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(100, 255, 255, 255));
	}

	m_pBF->Draw(to_string(this->m_nPotionCount) + "x", nPosX + 430, nPosY + 53, 0.6f, D3DCOLOR_ARGB(255, 255, 255, 255));

	// Artifacts
	m_pTM->Draw(m_vHUDImgIDs[BarBackground], nPosX + 490, nPosY + 30);

	if(m_nCurrAtr == 1)
	{
		m_pTM->Draw(m_vHUDImgIDs[BoxSelected], nPosX + 497, nPosY + 34);
	}
	else
	{
		m_pTM->Draw(m_vHUDImgIDs[BoxNormal], nPosX + 501, nPosY + 38);
	}

	if(m_nCurrAtr == 2)
	{
		m_pTM->Draw(m_vHUDImgIDs[BoxSelected], nPosX + 546, nPosY + 34);
	}
	else
	{
		m_pTM->Draw(m_vHUDImgIDs[BoxNormal], nPosX + 550, nPosY + 38);
	}

	if(m_nCurrAtr == 3)
	{
		m_pTM->Draw(m_vHUDImgIDs[BoxSelected], nPosX + 595, nPosY + 34);				
	}
	else
	{
		m_pTM->Draw(m_vHUDImgIDs[BoxNormal], nPosX + 599, nPosY + 38);
	}

	if(m_nCurrAtr == 4)
	{
		m_pTM->Draw(m_vHUDImgIDs[BoxSelected], nPosX + 644, nPosY + 34);
	}
	else
	{
		m_pTM->Draw(m_vHUDImgIDs[BoxNormal], nPosX + 648, nPosY + 38);
	}



	if(m_vArtifacts.size() > 0)
	{
		CSGD_TextureManager::GetInstance()->Draw(m_vHUDImgIDs[eFireArtifact], nPosX + 501, nPosY + 38);
	}

	if(m_vArtifacts.size() > 1)
	{
		CSGD_TextureManager::GetInstance()->Draw(m_vHUDImgIDs[eIceArtifact], nPosX + 550, nPosY + 38);
	}

	if(m_vArtifacts.size() > 2)
	{
		CSGD_TextureManager::GetInstance()->Draw(m_vHUDImgIDs[eEarthArtifact], nPosX + 599, nPosY + 38);
	}

	if(m_vArtifacts.size() > 3)
	{
		CSGD_TextureManager::GetInstance()->Draw(m_vHUDImgIDs[eLightningArtifact], nPosX + 648, nPosY + 38);
	}

	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();

	for (int i = 0; i < 4; i++)
	{
		if (m_fArtifactCooldown[i] > 0.01f)
		{
			abcooldown.left = (545 + (27 * i));
			abcooldown.right = (long)(545 + (27 * (i + 1)) - 8);
			abcooldown.top = 512;
			abcooldown.bottom = (LONG)(543 - (34 * ( m_fArtifactCooldown[i] / m_fArtifactTimers[i])));

			CSGD_Direct3D::GetInstance()->DrawRect(abcooldown, 75, 75, 75);
		}
	}
}

RECT CPlayer::GetRect ()
{
	Frame tempFrame = AnimationManager::GetInstance()->GetAnimation(this->GetAnimationInformation(this->GetCurrAnim())->GetAnimID()).GetFrame(this->GetAnimationInformation(this->GetCurrAnim())->GetCurFrame());
	POINT pAnchor = tempFrame.GetAnchorPoint();
	RECT rPassive = *tempFrame.GetPassiveRect();
	RECT rActiv  = *tempFrame.GetActiveRect();
	RECT rRect;

	int nHeight = rPassive.bottom - rPassive.top;
	int nWidth = rPassive.right - rPassive.left;
	int nX = (int)GetPosX();
	int nY = (int)GetPosY();

	rRect.bottom = nY + (rPassive.bottom - pAnchor.y);
	rRect.top = rRect.bottom - nHeight;
	rRect.right = nX + (rPassive.right - pAnchor.x);
	rRect.left = rRect.right - nWidth;

	return rRect;
}

void CPlayer::CalculateStats()
{
	m_nMaxHealth = 30 + (GetCharacterE() * 10);
	m_fAcceleration = (float)(150 + (GetMindE() * 10));
	m_nMeleeDamage = GetWeaponDamage() + (GetBodyE() * 2);

	m_nCritChance = 4 + (GetBodyE());
	if (m_nCritChance >= 30)
		m_nCritChance = 30;

	m_nDodgeChance = (4 + GetMindE());

	if (m_nDodgeChance >= 30)
		m_nDodgeChance = 30;

	m_nResistance = GetSoulE();

}

void CPlayer::SetCharacter( int nChar)
{
	m_nCharacter = nChar;

	float percent = m_nCurrHealth / (float)m_nMaxHealth;
	CalculateStats();
	m_nCurrHealth = (int)(percent * m_nMaxHealth);
}

void CPlayer::SetMind( int nMind)
{
	m_nMind = nMind;

	CalculateStats();
}

void CPlayer::SetBody( int nBody )
{
	m_nBody = nBody;

	CalculateStats();
}

void CPlayer::SetSoul( int nSoul )
{
	m_nSoul = nSoul;

	CalculateStats();
}

void CPlayer::HandleEvent(CEvent* pEvent)
{
	if(pEvent->GetEventID() == "TAKE_DAMAGE")
	{
		if( pEvent->GetDestination() == this			// intended for us
			|| pEvent->GetDestination() == nullptr )	// broadcast for everyone
		{
			int* pDamage = reinterpret_cast< int* >( pEvent->GetParam() );
			ModifyHealth( *pDamage );
		}
	}
	if(pEvent->GetEventID() == "GAIN_EXPERIENCE")
	{
		int* pExperience = reinterpret_cast< int* >( pEvent->GetParam() );
		GainXP( *pExperience );
	}
	if(pEvent->GetEventID() == "ADD_ARTIFACT")
	{
		AddArtifact();
	}

	if(pEvent->GetEventID() == "CAMERA_SHAKE")
	{
		Camera::GetInstance()->Shake();
	}
}

void CPlayer::InventoryRender()
{
	if(this->m_vInventoryIDs.size() > 0 && this->m_vInventoryIDs[0] != -1)
	{
		CSGD_TextureManager::GetInstance()->Draw(this->m_vInventoryIDs[0], 0, 0);
	}

	if(this->m_vInventoryIDs.size() > 1 && this->m_vInventoryIDs[1] != -1)
	{
		CSGD_TextureManager::GetInstance()->Draw(this->m_vInventoryIDs[1], 185, 185);
		CSGD_TextureManager::GetInstance()->Draw(this->m_vInventoryIDs[1], 185, 275);
	}

	if(this->m_vInventoryIDs.size() > 2 && this->m_vInventoryIDs[2] != -1)
	{
		CSGD_TextureManager::GetInstance()->Draw(this->m_vInventoryIDs[2], 245, 185);
		CSGD_TextureManager::GetInstance()->Draw(this->m_vInventoryIDs[2], 245, 275);
	}
}

// Player Leveling
void CPlayer::GainXP( int amount )
{
/////////////////////////////////////////////////////////////////
// BUG FIX
// Reference Bug # BB-052
// BUG FIX START
// LEVEL CAPPED AT 25
/////////////////////////////////////////////////////////////////
	if (m_nLevel < 25)
	{
///////////////////////////////////////////////////////////////////////////
// BUG FIX END  Reference # BB-052
//////////////////////////////////////////////////////////////////////////
		m_nExperience += amount;

	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-016
	// BUG FIX START
	/////////////////////////////////////////////////////////////////
	
	// Changed from 'if' to 'while' to allow for multiple leveling at once
	while (m_nExperience >= m_nToNextLevel)
	{
		LevelUp();
	}
	
	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-016
	//////////////////////////////////////////////////////////////////////////
	}
	
}

void CPlayer::LevelUp( void )
{
	// TODO: PLAY LEVEL UP SOUND
	if (!(m_nHelpMessages & 1 << 5))
		PlayState::GetInstance()->HelpMessage();

	CAudioManager::GetInstance()->PlaySFX( SFX_LEVELUP );
	RECT Cam;
	Camera::GetInstance()->CamRect(&Cam);
	m_nLevelUpAlpha = 255;
	LevelUpEffect->SetXPosition(GetPosX() - Cam.left);
	LevelUpEffect->SetYPosition(GetPosY() - Cam.top - 64);
	LevelUpEffect->Initialize();

	m_fLevelUpTimer = 3.0f;

	m_nCurrHealth = m_nMaxHealth;

	// TODO: SHOW MESSAGE TO PLAYER THAT THEY HAVE LEVELED UP

	int Diff = m_nExperience - m_nToNextLevel;
	m_nExperience = Diff;
	++m_nLevel;

	if (m_nLevel == 25)
		m_nExperience = 0;
			
	LevelMessage = "Level " + std::to_string(m_nLevel);

	m_nUnspent += 2;
	if (m_nLevel <= 25)
	{
		switch (m_nLevel)
		{
		case 2: m_nToNextLevel = 125; break;
		case 3: m_nToNextLevel = 150; break;
		case 4: m_nToNextLevel = 175; break;
		case 5: m_nToNextLevel = 200; break;
		case 6: m_nToNextLevel = 1000; break;
		case 7: m_nToNextLevel = 1250; break;
		case 8: m_nToNextLevel = 1500; break;
		case 9: m_nToNextLevel = 1750; break;
		case 10: m_nToNextLevel = 2000; break;
		case 11: m_nToNextLevel = 10000; break;
		case 12: m_nToNextLevel = 12500; break;
		case 13: m_nToNextLevel = 15000; break;
		case 14: m_nToNextLevel = 17500; break;
		case 15: m_nToNextLevel = 20000; break;
		case 16: m_nToNextLevel = 100000; break;
		case 17: m_nToNextLevel = 125000; break;
		case 18: m_nToNextLevel = 150000; break;
		case 19: m_nToNextLevel = 175000; break;
		case 20: m_nToNextLevel = 200000; break;
		case 21: m_nToNextLevel = 200000; break;
		case 22: m_nToNextLevel = 200000; break;
		case 23: m_nToNextLevel = 200000; break;
		case 24: m_nToNextLevel = 200000; break;
		case 25: m_nToNextLevel = 200000; break;
		}
	}
}

int CPlayer::GetWeaponDamage()
{
	CItem * iWeapon;

	for(unsigned int i = 0; i < this->m_vItems.size(); i++)
	{
		if(this->m_vItems[i]->GetType() == 0 && this->m_vItems[i]->Equipped())
		{
			iWeapon = this->m_vItems[i];
			i = this->m_vItems.size();
		}
	}

	return iWeapon->GetSpecial();
}

vector<CItem *> CPlayer::GetWeapons()
{
	vector<CItem *> vWeapons;

	for(unsigned int i = 0; i < this->m_vItems.size(); i++)
	{
		if(this->m_vItems[i]->GetType() == 0)
		{
			CItem * tempWeapon;

			tempWeapon = this->m_vItems[i];

			vWeapons.push_back(tempWeapon);
		}
	}

	return vWeapons;
}

vector<CItem *> CPlayer::GetArmors()
{
	vector<CItem *> vArmors;

	for(unsigned int i = 0; i < this->m_vItems.size(); i++)
	{
		if(this->m_vItems[i]->GetType() == 1)
		{
			CItem * tempArmor;

			tempArmor = this->m_vItems[i];

			vArmors.push_back(tempArmor);
		}
	}

	return vArmors;
}

vector<CItem *> CPlayer::GetGems()
{
	vector<CItem *> vGems;

	for(unsigned int i = 0; i < this->m_vItems.size(); i++)
	{
		if(this->m_vItems[i]->GetType() == 3)
		{
			CItem * tempGem;

			tempGem = this->m_vItems[i];

			vGems.push_back(tempGem);
		}
	}

	return vGems;
}

vector<CItem *> CPlayer::GetEquippableItems()
{
	vector<CItem *> vEquippableItems;

	for(unsigned int i = 0; i < this->m_vItems.size(); i++)
	{
		if(this->m_vItems[i]->GetType() == 0 || this->m_vItems[i]->GetType() == 1)
		{
			CItem * tempItem;

			tempItem = this->m_vItems[i];

			vEquippableItems.push_back(tempItem);
		}
	}

	return vEquippableItems;
}

void CPlayer::RemoveGem(CItem* pItem)
{
	vector<CItem *>::iterator iter = this->m_vItems.begin();

	int i = 0;
	for ( ; iter != this->m_vItems.end(); ++iter )
	{
		if (*iter == pItem)
		{
			if(pItem->GetGem() != nullptr)
			{
				delete pItem->GetGem();
				pItem->SetGem(nullptr);
			}

			delete this->m_vItems[i];

			// Remove from list
			iter = this->m_vItems.erase(iter);
			return;
		}
		i++;
	}
}

void CPlayer::AddItem(CItem * pItem)
{
	CItem * tempItem = new CItem(*pItem);
	this->m_vItems.push_back(tempItem);

	if (tempItem->GetType() == 0)
		CAudioManager::GetInstance()->PlaySFX( SFX_WEAPONBUY );
	if (tempItem->GetType() == 1)
		CAudioManager::GetInstance()->PlaySFX( SFX_ARMORBUY );
	if (tempItem->GetType() == 2)
	{
		m_nPotionCount++;
		CAudioManager::GetInstance()->PlaySFX( SFX_POTION );
	}
	if (tempItem->GetType() == 3)
		CAudioManager::GetInstance()->PlaySFX( SFX_SOCKET );
}

/////////////////////////////////////////////////////////////////
// BUG FIX
// Reference Bug # BB-072
// BUG FIX START
/////////////////////////////////////////////////////////////////

// Potion increase makes a sound
void CPlayer::GainPotion()
{
	m_nPotionCount++;
	CAudioManager::GetInstance()->PlaySFX( SFX_POTION );
}

///////////////////////////////////////////////////////////////////////////
// BUG FIX END  Reference # BB-072
//////////////////////////////////////////////////////////////////////////

void CPlayer::RemoveItem(CItem * pItem)
{
	vector<CItem *>::iterator iter = this->m_vItems.begin();

	int i = 0;
	for ( ; iter != this->m_vItems.end(); ++iter )
	{
		if (*iter == pItem)
		{
			if(pItem->GetGem() != nullptr)
			{
				delete pItem->GetGem();
				pItem->SetGem(nullptr);
			}

			delete this->m_vItems[i];

			// Remove from list
			iter = this->m_vItems.erase(iter);
			return;
		}
		i++;
	}
}

bool CPlayer::ActiveRecCollision(IEntity* pOther)
{

	if (AnimationManager::GetInstance()->GetAnimation(this->GetAnimationInformation(this->GetCurrAnim())->GetAnimID()).GetFrame(this->GetAnimationInformation(this->GetCurrAnim())->GetCurFrame()).HasActiveRect())
	{

		Frame tempFrame = AnimationManager::GetInstance()->GetAnimation(this->GetAnimationInformation(this->GetCurrAnim())->GetAnimID()).GetFrame(this->GetAnimationInformation(this->GetCurrAnim())->GetCurFrame());
		POINT pAnchor = tempFrame.GetAnchorPoint();
		RECT rActiv  = *tempFrame.GetActiveRect();
		RECT rRect, rOver;

		RECT rOther = pOther->GetRect();

		int nHeight = rActiv.bottom - rActiv.top;
		int nWidth = rActiv.right - rActiv.left;
		int nX = (int)GetPosX();
		int nY = (int)GetPosY();

		rRect.bottom = nY + (rActiv.bottom - pAnchor.y);
		rRect.top = rRect.bottom - nHeight;
		rRect.right = nX + (rActiv.right - pAnchor.x);
		rRect.left = rRect.right - nWidth; 

		if (CollidingWith(rRect, pOther, &rOver))
		{
		
			if (m_bStab || m_bEarthSmash || m_bLunge || m_bSwipe || m_bMelee)
			{
				CEntity* Temp = dynamic_cast<CEntity*> (pOther);

				if (Temp->GetType() != ENT_PROJECTILE && Temp->GetType() != ENT_ITEM)
				{

					if (m_bSwipe)
					{
						int damage;
						switch(m_vArtifacts[m_nCurrAtr - 1]->GetLevel())
						{
						case 3:
						case 2:
							{
								if (20 > rand() % 100)
									CSGD_EventSystem::GetInstance()->SendEventNow(
									"STUN", nullptr, pOther, this );
							}
						case 1:
							{
								damage = -50;
								CSGD_EventSystem::GetInstance()->SendEventNow(
									"TAKE_LIGHTNING_DAMAGE", &damage, pOther, this);
							}
						case 0:
							{
								damage = -m_nBody;
								CSGD_EventSystem::GetInstance()->SendEventNow(
									"TAKE_DAMAGE", &damage, pOther, this );
								return true;
							}
						}
					}
					else if (m_bStab)
					{
						int damage;
						switch(m_vArtifacts[m_nCurrAtr - 1]->GetLevel())
						{
						case 3:
							{
								damage = -60;
								CSGD_EventSystem::GetInstance()->SendEventNow(
									"TAKE_ICE_DAMAGE", &damage, pOther, this);
								if (40 > rand() % 100)
									CSGD_EventSystem::GetInstance()->SendEventNow(
									"FREEZE", nullptr, pOther, this );
							}
							break;
						case 2:
							{
								if (30 > rand() % 100)
									CSGD_EventSystem::GetInstance()->SendEventNow(
									"FREEZE", nullptr, pOther, this );
							}
						case 1:
							{
								damage = -30;
								CSGD_EventSystem::GetInstance()->SendEventNow(
									"TAKE_ICE_DAMAGE", &damage, pOther, this);
							}
						case 0:
							{
								damage = -m_nBody;
								CSGD_EventSystem::GetInstance()->SendEventNow(
									"TAKE_DAMAGE", &damage, pOther, this );
								return true;
							}
						}
					}
					else if (m_bEarthSmash)
					{
						int damage;
						switch(m_vArtifacts[m_nCurrAtr - 1]->GetLevel())
						{
						case 3:
						case 2:
							{
								damage = -100;
								CSGD_EventSystem::GetInstance()->SendEventNow(
									"TAKE_EARTH_DAMAGE", &damage, pOther, this);
								break;
							}
						case 1:
							{
								damage = -40;
								CSGD_EventSystem::GetInstance()->SendEventNow(
									"TAKE_EARTH_DAMAGE", &damage, pOther, this);
							}
						case 0:
							{
								damage = -m_nBody;
								CSGD_EventSystem::GetInstance()->SendEventNow(
									"TAKE_DAMAGE", &damage, pOther, this );
							}
						}
						return true;
					}

					int damage = -m_nMind;
					CSGD_EventSystem::GetInstance()->SendEventNow(
						"TAKE_DAMAGE", &damage, pOther, this );

					return true;
				}

			}
			else if (m_bEarthWall)
			{
				CEntity* Temp = dynamic_cast<CEntity*> (pOther);

				if (Temp->GetType() == ENT_PROJECTILE)
				{
					int damage = -m_nBody;
					CSGD_EventSystem::GetInstance()->SendEventNow(
						"TAKE_DAMAGE", &damage, pOther, this );

					return true;
				}
			}
		}

	}

	return false;
}

//Lunge Attack
void CPlayer::Lunge()
{
	if(!m_bLunge)
	{
		m_bLunge = true;
		m_fLungeTimer = 0.25f;

		if (DashEffect != nullptr)
		{
			delete DashEffect;
			DashEffect = nullptr;
		}

		DashEffect = new CParticleEffect;

		if (m_nCurrAtr == 1)
		{
			CAudioManager::GetInstance()->PlaySFX( SFX_DASH );
			CParticleManager::GetInstance()->GetEffect( *(DashEffect), FX_FIREDASH );
		}
		if (m_nCurrAtr == 4)
		{
			CAudioManager::GetInstance()->PlaySFX( SFX_LDASH );
			CParticleManager::GetInstance()->GetEffect( *(DashEffect), FX_LIGHTNINGDASH );
		}

		DashEffect->ToggleCamera();
		DashEffect->SetXPosition(GetPosX());
		DashEffect->SetYPosition(GetPosY() - 32);
		DashEffect->Initialize();

		switch(this->m_nFacing)
		{
		case Up:
			{
				this->SetCurrAnim(playerAttackLungeUp + 32 * m_nCurrAtr);
			}							
			break;						
		case Down:						
			{							
				this->SetCurrAnim(playerAttackLungeDown + 32 * m_nCurrAtr);
			}							
			break;						
		case Left:						
			{							
				this->SetCurrAnim(playerAttackLungeLeft + 32 * m_nCurrAtr);
			}						
			break;					
		case Right:					
			{						
				this->SetCurrAnim(playerAttackLungeRight + 32 * m_nCurrAtr);
			}
			break;
		}
	}

	

	if (m_fLungeTimer <= 0.01)
	{
		if (DashEffect != nullptr)
			DashEffect->StopEmitting();

		m_bLunge = false;
		SetVelX(0);
		SetVelY(0);

		switch(this->m_nFacing)
		{
		case Up:
			{
				this->SetCurrAnim(playerStandUp + 32 * m_nCurrAtr);
			}							
			break;						
		case Down:						
			{							
				this->SetCurrAnim(playerStandDown + 32 * m_nCurrAtr);
			}							
			break;						
		case Left:						
			{							
				this->SetCurrAnim(playerStandLeft + 32 * m_nCurrAtr);
			}						
			break;					
		case Right:					
			{						
				this->SetCurrAnim(playerStandRight + 32 * m_nCurrAtr);
			}
			break;
		}

		return;
	}

	float BaseSpeed = m_fAcceleration;
		if (m_fAcceleration > 250.0f)
			BaseSpeed = 250.0f;

	switch( m_nFacing )
	{
	case Up:
		m_fVelocityY = -BaseSpeed * (3 + m_vArtifacts[m_nCurrAtr -1]->GetLevel());
		m_fVelocityX = 0;
		break;

	case Down:
		m_fVelocityY = BaseSpeed * (3 + m_vArtifacts[m_nCurrAtr -1]->GetLevel());
		m_fVelocityX = 0;
		break;

	case Left:
		m_fVelocityY = 0;
		m_fVelocityX = -BaseSpeed * (3 + m_vArtifacts[m_nCurrAtr -1]->GetLevel());
		break;

	case Right:
		m_fVelocityY = 0;
		m_fVelocityX = BaseSpeed * (3 + m_vArtifacts[m_nCurrAtr -1]->GetLevel());
		break;
	}

	if (m_nCurrAtr == 1 && GetArtifactLevel(0) == 3)
	LevelManager::GetInstance()->FireDash(this);

}

void CPlayer::Stab()
{
	if (!m_bStab)
	{
		if (IceSlashEffect != nullptr)
		{
			delete IceSlashEffect;
			IceSlashEffect = nullptr;
		}

		IceSlashEffect = new CParticleEffect;
		CParticleManager::GetInstance()->GetEffect( *IceSlashEffect, FX_ICESLASH );
		IceSlashEffect->ToggleCamera();
		IceSlashEffect->SetXPosition(GetPosX());
		IceSlashEffect->SetYPosition(GetPosY());
		IceSlashEffect->Initialize();

		m_bStab = true;
		CAudioManager::GetInstance()->PlaySFX( SFX_ICESLASH );

		//Start anim for stab
		switch(this->m_nFacing)
		{
		case Up:
			{
				this->SetCurrAnim(playerAttackSlashUp + 32 * m_nCurrAtr);
			}
			break;
		case Down:
			{
				this->SetCurrAnim(playerAttackSlashDown + 32 * m_nCurrAtr);
			}
			break;
		case Left:
			{
				this->SetCurrAnim(playerAttackSlashLeft + 32 * m_nCurrAtr);
			}
			break;
		case Right:
			{
				this->SetCurrAnim(playerAttackSlashRight + 32 * m_nCurrAtr);
			}
			break;
		}

		this->GetAnimationInformation(this->GetCurrAnim())->Reset();
	}

	if (this->GetAnimationInformation(GetCurrAnim())->IsOver())
	{
		IceSlashEffect->StopEmitting();
		m_bStab = false;

		switch(this->m_nFacing)
		{
		case Up:
			{
				this->SetCurrAnim(playerStandUp + 32 * m_nCurrAtr);
			}							
			break;						
		case Down:						
			{							
				this->SetCurrAnim(playerStandDown + 32 * m_nCurrAtr);
			}							
			break;						
		case Left:						
			{							
				this->SetCurrAnim(playerStandLeft + 32 * m_nCurrAtr);
			}						
			break;					
		case Right:					
			{						
				this->SetCurrAnim(playerStandRight + 32 * m_nCurrAtr);
			}
			break;
		}
	}
}

//Rock Wall
void CPlayer::CreateRockWall()
{
	if (!m_bEarthWall)
	{
		m_fEarthWalltimer = 6.0f;
		m_bEarthWall = true;

		if (RockShield != nullptr)
		{
			delete RockShield;
			RockShield = nullptr;
		}
		CAudioManager::GetInstance()->PlaySFX( SFX_EARTHSHARD );
		RockShield = new CParticleEffect;
		CParticleManager::GetInstance()->GetEffect(*RockShield, FX_ROCKSHIELD);
		RockShield->ToggleCamera();
		RockShield->SetXPosition(GetPosX());
		RockShield->SetYPosition(GetPosY());
		RockShield->Initialize();
	}


	if (m_fEarthWalltimer > 0.01)
	{
		RECT rWall;
		RECT rCam;

		m_pCamera->CamRect(&rCam);

		int X = this->GetPosX() - rCam.left;
		int Y = this->GetPosY() - rCam.top;

		switch(m_nFacing)
		{
		case 0:
			rWall.bottom = Y - GetHeight();
			rWall.top = rWall.bottom - 20;
			rWall.left = X - GetWidth()/2;
			rWall.right = X + GetWidth()/2;
			this->SetCurrAnim(playerEarthShieldUp + 32 * m_nCurrAtr);
			break;

		case 1:
			rWall.top = Y;
			rWall.bottom = rWall.top + 20;
			rWall.left = X - GetWidth()/2;
			rWall.right = X + GetWidth()/2;
			this->SetCurrAnim(playerEarthShieldDown + 32 * m_nCurrAtr);
			break;

		case 2:
			rWall.top = Y - GetHeight();
			rWall.bottom = Y;
			rWall.right = (X - GetWidth()/2);
			rWall.left = rWall.right - 20;
			this->SetCurrAnim(playerEarthShieldLeft + 32 * m_nCurrAtr);
			break;

		case 3:
			rWall.top = Y - GetHeight();
			rWall.bottom = Y;
			rWall.left = (X + GetWidth()/2);
			rWall.right = rWall.left + 20;
			this->SetCurrAnim(playerEarthShieldRight + 32 * m_nCurrAtr);
			break;
		}

		this->GetAnimationInformation(this->GetCurrAnim())->Reset();		

		rEWall = rWall;
	}
	else
	{
		RockShield->StopEmitting();
		m_bEarthWall = false;

		switch(this->m_nFacing)
		{
		case Up:
			{
				this->SetCurrAnim(playerStandUp + 32 * m_nCurrAtr);
			}							
			break;						
		case Down:						
			{							
				this->SetCurrAnim(playerStandDown + 32 * m_nCurrAtr);
			}							
			break;						
		case Left:						
			{							
				this->SetCurrAnim(playerStandLeft + 32 * m_nCurrAtr);
			}						
			break;					
		case Right:					
			{						
				this->SetCurrAnim(playerStandRight + 32 * m_nCurrAtr);
			}
			break;
		}
	}

}

void CPlayer::EarthSmash()
{
	if(!m_bEarthSmash)
	{
		if (SmashEffect != nullptr)
		{
			delete SmashEffect;
			SmashEffect = nullptr;
		}
		SmashEffect = new CParticleEffect;
		CParticleManager::GetInstance()->GetEffect( *SmashEffect, FX_SMASHEFFECT );
		SmashEffect->ToggleCamera();
		SmashEffect->SetXPosition(GetPosX());
		SmashEffect->SetYPosition(GetPosY());
		SmashEffect->Initialize();
		m_bEarthSmash = true;
		CAudioManager::GetInstance()->PlaySFX(SFX_ROCKSMASH);

		switch(this->m_nFacing)
		{
		case Up:
			{
				this->SetCurrAnim(playerAttackSmashUp + 32 * m_nCurrAtr);
			}
			break;
		case Down:
			{
				this->SetCurrAnim(playerAttackSmashDown + 32 * m_nCurrAtr);
			}
			break;
		case Left:
			{
				this->SetCurrAnim(playerAttackSmashLeft + 32 * m_nCurrAtr);
			}
			break;
		case Right:
			{
				this->SetCurrAnim(playerAttackSmashRight + 32 * m_nCurrAtr);
			}
			break;
		}

		this->GetAnimationInformation(this->GetCurrAnim())->Reset();		
	}


	if (this->GetAnimationInformation(GetCurrAnim())->IsOver())
	{
		SmashEffect->StopEmitting();
		LevelManager::GetInstance()->Break(this);
		m_bEarthSmash = false;

		switch(this->m_nFacing)
		{
		case Up:
			{
				this->SetCurrAnim(playerStandUp + 32 * m_nCurrAtr);
			}							
			break;						
		case Down:						
			{							
				this->SetCurrAnim(playerStandDown + 32 * m_nCurrAtr);
			}							
			break;						
		case Left:						
			{							
				this->SetCurrAnim(playerStandLeft + 32 * m_nCurrAtr);
			}						
			break;					
		case Right:					
			{						
				this->SetCurrAnim(playerStandRight + 32 * m_nCurrAtr);
			}
			break;
		}
	}

}

void CPlayer::HealWall()
{
	if(!m_bHealWall)
	{
		if (FreezeHeal != nullptr)
		{
			delete FreezeHeal;
			FreezeHeal = nullptr;
		}
		CAudioManager::GetInstance()->PlaySFX(SFX_FREEZE);
		m_bHealWall = true;
		FreezeHeal = new CParticleEffect;
		CParticleManager::GetInstance()->GetEffect(*FreezeHeal, FX_FREEZEHEAL);
		FreezeHeal->ToggleCamera();
		FreezeHeal->SetXPosition(GetPosX() - 20);
		FreezeHeal->SetYPosition(GetPosY() - 20);
		FreezeHeal->Initialize();

		switch(this->m_nFacing)
		{
		case Up:
			{
				this->SetCurrAnim(playerCastUp + 32 * m_nCurrAtr);
			}
			break;
		case Down:
			{
				this->SetCurrAnim(playerCastDown + 32 * m_nCurrAtr);
			}
			break;
		case Left:
			{
				this->SetCurrAnim(playerCastLeft + 32 * m_nCurrAtr);
			}
			break;
		case Right:
			{
				this->SetCurrAnim(playerCastRight + 32 * m_nCurrAtr);
			}
			break;
		}

		this->GetAnimationInformation(this->GetCurrAnim())->Reset();		
	}


	if (this->GetAnimationInformation(GetCurrAnim())->IsOver())
	{
		LevelManager::GetInstance()->IceWall(this);
		FreezeHeal->StopEmitting();
		
		int Heal;

		switch(m_vArtifacts[m_nCurrAtr - 1]->GetLevel())
		{
		case 0:
			Heal = (GetMaxHealth() * 20) / 100;
			break;
		case 1:
			Heal = (GetMaxHealth() * 40) / 100;
			break;
		case 2:
			Heal = (GetMaxHealth() * 60) / 100;
			break;
		case 3:
			Heal = (GetMaxHealth() * 100) / 100;
			break;
		}

		ModifyHealth( Heal);

		m_bHealWall = false;

		switch(this->m_nFacing)
		{
		case Up:
			{
				this->SetCurrAnim(playerStandUp + 32 * m_nCurrAtr);
			}							
			break;						
		case Down:						
			{							
				this->SetCurrAnim(playerStandDown + 32 * m_nCurrAtr);
			}							
			break;						
		case Left:						
			{							
				this->SetCurrAnim(playerStandLeft + 32 * m_nCurrAtr);
			}						
			break;					
		case Right:					
			{						
				this->SetCurrAnim(playerStandRight + 32 * m_nCurrAtr);
			}
			break;
		}
	}
}

void CPlayer::SwordSwipe() 
{
	if(!m_bSwipe)
	{
		m_bSwipe = true;

		if (LightningSlash != nullptr)
		{
			delete LightningSlash;
			LightningSlash = nullptr;
		}
		CAudioManager::GetInstance()->PlaySFX( SFX_LSLASH );
		LightningSlash = new CParticleEffect;
		CParticleManager::GetInstance()->GetEffect(*LightningSlash, FX_LSLASH );
		LightningSlash->ToggleCamera();
		LightningSlash->SetXPosition(GetPosX());
		LightningSlash->SetYPosition(GetPosY());
		LightningSlash->Initialize();

		switch(this->m_nFacing)
		{
		case Up:
			{
				this->SetCurrAnim(playerAttackSlashUp + 32 * m_nCurrAtr);
			}
			break;
		case Down:
			{
				this->SetCurrAnim(playerAttackSlashDown + 32 * m_nCurrAtr);
			}
			break;
		case Left:
			{
				this->SetCurrAnim(playerAttackSlashLeft + 32 * m_nCurrAtr);
			}
			break;
		case Right:
			{
				this->SetCurrAnim(playerAttackSlashRight + 32 * m_nCurrAtr);
			}
			break;
		}

		this->GetAnimationInformation(this->GetCurrAnim())->Reset();		
	}

	if (this->GetAnimationInformation(GetCurrAnim())->IsOver())
	{
		m_bSwipe = false;
		m_nCurrAnim = m_nFacing;
		LightningSlash->StopEmitting();

		switch(this->m_nFacing)
		{
		case Up:
			{
				this->SetCurrAnim(playerStandUp + 32 * m_nCurrAtr);
			}
			break;
		case Down:
			{
				this->SetCurrAnim(playerStandDown + 32 * m_nCurrAtr);
			}
			break;
		case Left:
			{
				this->SetCurrAnim(playerStandLeft + 32 * m_nCurrAtr);
			}
			break;
		case Right:
			{
				this->SetCurrAnim(playerStandRight + 32 * m_nCurrAtr);
			}
			break;
		}
	}

}

void CPlayer::Shoot()
{
	if (!m_bShoot)
	{
		m_bShoot = true;

		switch(this->m_nFacing)
		{
		case Up:
			{
				this->SetCurrAnim(playerCastUp + 32 * m_nCurrAtr);
			}
			break;
		case Down:
			{
				this->SetCurrAnim(playerCastDown + 32 * m_nCurrAtr);
			}
			break;
		case Left:
			{
				this->SetCurrAnim(playerCastLeft + 32 * m_nCurrAtr);
			}
			break;
		case Right:
			{
				this->SetCurrAnim(playerCastRight + 32 * m_nCurrAtr);
			}
			break;
		}

		this->GetAnimationInformation(this->GetCurrAnim())->Reset();		

	}

	if (this->GetAnimationInformation(GetCurrAnim())->IsOver())
	{
		m_bShoot = false;

		switch( m_nFacing )
		{
		case Up:
			m_vArtifacts[m_nCurrAtr - 1]->Ability1((int)GetPosX(), ((int)GetPosY() - GetHeight()) - 50, m_nFacing);
			break;

		case Down:
			m_vArtifacts[m_nCurrAtr - 1]->Ability1((int)GetPosX(), (int)GetPosY()  + 50, m_nFacing);
			break;

		case Left:
			m_vArtifacts[m_nCurrAtr - 1]->Ability1( ((int)GetPosX() - GetWidth()/2) - 50, (int)GetPosY() - GetHeight()/2, m_nFacing);
			break;

		case Right:
			m_vArtifacts[m_nCurrAtr - 1]->Ability1(((int)GetPosX() + GetWidth()/2) + 75, (int)GetPosY() - GetHeight()/2, m_nFacing);
			break;
		}
		m_fAbCooldown[1] = m_fAbTimers[1];

		switch(this->m_nFacing)
		{
		case Up:
			{
				this->SetCurrAnim(playerStandUp + 32 * m_nCurrAtr);
			}							
			break;						
		case Down:						
			{							
				this->SetCurrAnim(playerStandDown + 32 * m_nCurrAtr);
			}							
			break;						
		case Left:						
			{							
				this->SetCurrAnim(playerStandLeft + 32 * m_nCurrAtr);
			}						
			break;					
		case Right:					
			{						
				this->SetCurrAnim(playerStandRight + 32 * m_nCurrAtr);
			}
			break;
		}

	}

}

void CPlayer::Chakram()
{
	if (!m_bChakram)
	{
		m_bChakram = true;

		switch(this->m_nFacing)
		{
		case Up:
			{
				this->SetCurrAnim(playerCastUp + 32 * m_nCurrAtr);
			}
			break;
		case Down:
			{
				this->SetCurrAnim(playerCastDown + 32 * m_nCurrAtr);
			}
			break;
		case Left:
			{
				this->SetCurrAnim(playerCastLeft + 32 * m_nCurrAtr);
			}
			break;
		case Right:
			{
				this->SetCurrAnim(playerCastRight + 32 * m_nCurrAtr);
			}
			break;
		}

		this->GetAnimationInformation(this->GetCurrAnim())->Reset();		

	}

	if (this->GetAnimationInformation(GetCurrAnim())->IsOver())
	{
		m_bChakram = false;

		switch( m_nFacing )
		{
		case Up:
			m_vArtifacts[m_nCurrAtr - 1]->Ability2((int)GetPosX(), ((int)GetPosY() - GetHeight()) - 15, m_nFacing);
			break;

		case Down:
			m_vArtifacts[m_nCurrAtr - 1]->Ability2((int)GetPosX(), (int)GetPosY()  + 15, m_nFacing);
			break;

		case Left:
			m_vArtifacts[m_nCurrAtr - 1]->Ability2( ((int)GetPosX() - GetWidth()/2) - 40, (int)GetPosY() - GetHeight()/2, m_nFacing);
			break;

		case Right:
			m_vArtifacts[m_nCurrAtr - 1]->Ability2(((int)GetPosX() + GetWidth()/2) + 75, (int)GetPosY() - GetHeight()/2, m_nFacing);
			break;
		}
		m_fAbCooldown[2] = m_fAbTimers[2];

		switch(this->m_nFacing)
		{
		case Up:
			{
				this->SetCurrAnim(playerStandUp + 32 * m_nCurrAtr);
			}							
			break;						
		case Down:						
			{							
				this->SetCurrAnim(playerStandDown + 32 * m_nCurrAtr);
			}							
			break;						
		case Left:						
			{							
				this->SetCurrAnim(playerStandLeft + 32 * m_nCurrAtr);
			}							
			break;						
		case Right:						
			{							
				this->SetCurrAnim(playerStandRight + 32 * m_nCurrAtr);
			}
			break;
		}

	}

}

void CPlayer::Melee()
{
	if (!m_bMelee)
	{
		m_bMelee = true;
		
		/////////////////////////////////////////////////////////////////
		// BUG FIX
		// Reference Bug # BB-079
		// BUG FIX START
		/////////////////////////////////////////////////////////////////

		// Play sound effect
		CAudioManager::GetInstance()->PlaySFX( SFX_STAB );

		///////////////////////////////////////////////////////////////////////////
		// BUG FIX END  Reference # BB-079
		//////////////////////////////////////////////////////////////////////////

		switch(this->m_nFacing)
		{
		case Up:
			{
				this->SetCurrAnim(playerAttackStabUp + 32 * m_nCurrAtr);
			}
			break;
		case Down:
			{
				this->SetCurrAnim(playerAttackStabDown + 32 * m_nCurrAtr);
			}
			break;
		case Left:
			{
				this->SetCurrAnim(playerAttackStabLeft + 32 * m_nCurrAtr);
			}
			break;
		case Right:
			{
				this->SetCurrAnim(playerAttackStabRight + 32 * m_nCurrAtr);
			}
			break;
		}

		this->GetAnimationInformation(this->GetCurrAnim())->Reset();		

	}

	if (this->GetAnimationInformation(GetCurrAnim())->IsOver())
	{
		m_bMelee = false;

		m_fAbCooldown[0] = m_fAbTimers[0];

		switch(this->m_nFacing)
		{
		case Up:
			{
				this->SetCurrAnim(playerStandUp + 32 * m_nCurrAtr);
			}							
			break;						
		case Down:						
			{							
				this->SetCurrAnim(playerStandDown + 32 * m_nCurrAtr);
			}							
			break;						
		case Left:						
			{							
				this->SetCurrAnim(playerStandLeft + 32 * m_nCurrAtr);
			}						
			break;					
		case Right:					
			{						
				this->SetCurrAnim(playerStandRight + 32 * m_nCurrAtr);
			}
			break;
		}

	}

}

int CPlayer::GetArtifactLevel(int nIndex)
{
	if (nIndex < m_nNumArt)
		return m_vArtifacts[nIndex]->GetLevel();
	else 
		return -1;
}

void CPlayer::ClearEquips()
{
	for(unsigned int i = 0; i < this->m_vItems.size(); i++)
	{
		delete this->m_vItems[i];
	}

	this->m_vItems.clear();
}

int CPlayer::GetCharacter()
{
	return m_nCharacter;
}

int CPlayer::GetMind()
{
	return m_nMind;
}

int CPlayer::GetBody()
{
	return m_nBody;
}

int CPlayer::GetSoul()
{
	return m_nSoul;
}

int CPlayer::GetCharacterE()
{
	int nChar = m_nCharacter;

	CItem * iWeapon = nullptr;

	for(unsigned int i = 0; i < this->m_vItems.size(); i++)
	{
		if(this->m_vItems[i]->GetType() == 0 && this->m_vItems[i]->Equipped())
		{
			iWeapon = this->m_vItems[i];
			i = this->m_vItems.size();
		}
	}

	CItem * iArmor = nullptr;

	for(unsigned int i = 0; i < this->m_vItems.size(); i++)
	{
		if(this->m_vItems[i]->GetType() == 1 && this->m_vItems[i]->Equipped())
		{
			iArmor = this->m_vItems[i];
			i = this->m_vItems.size();
		}
	}

	if(iWeapon != nullptr)
	{
		nChar += iWeapon->GetChar();
	}

	if(iArmor != nullptr)
	{
		nChar += iArmor->GetChar();
	}

	return nChar;
}

int CPlayer::GetMindE()
{
	int nMind = m_nMind;

	CItem * iWeapon = nullptr;

	for(unsigned int i = 0; i < this->m_vItems.size(); i++)
	{
		if(this->m_vItems[i]->GetType() == 0 && this->m_vItems[i]->Equipped())
		{
			iWeapon = this->m_vItems[i];
			i = this->m_vItems.size();
		}
	}

	CItem * iArmor = nullptr;

	for(unsigned int i = 0; i < this->m_vItems.size(); i++)
	{
		if(this->m_vItems[i]->GetType() == 1 && this->m_vItems[i]->Equipped())
		{
			iArmor = this->m_vItems[i];
			i = this->m_vItems.size();
		}
	}

	if(iWeapon != nullptr)
	{
		nMind += iWeapon->GetMind();
	}

	if(iArmor != nullptr)
	{
		nMind += iArmor->GetMind();
	}

	return nMind;
}

int CPlayer::GetBodyE()
{
	int nBody = m_nBody;

	CItem * iWeapon = nullptr;

	for(unsigned int i = 0; i < this->m_vItems.size(); i++)
	{
		if(this->m_vItems[i]->GetType() == 0 && this->m_vItems[i]->Equipped())
		{
			iWeapon = this->m_vItems[i];
			i = this->m_vItems.size();
		}
	}

	CItem * iArmor = nullptr;

	for(unsigned int i = 0; i < this->m_vItems.size(); i++)
	{
		if(this->m_vItems[i]->GetType() == 1 && this->m_vItems[i]->Equipped())
		{
			iArmor = this->m_vItems[i];
			i = this->m_vItems.size();
		}
	}

	if(iWeapon != nullptr)
	{
		nBody += iWeapon->GetBody();
	}

	if(iArmor != nullptr)
	{
		nBody += iArmor->GetBody();
	}

	return nBody;
}

int CPlayer::GetSoulE()
{
	int nSoul = m_nSoul;

	CItem * iWeapon = nullptr;

	for(unsigned int i = 0; i < this->m_vItems.size(); i++)
	{
		if(this->m_vItems[i]->GetType() == 0 && this->m_vItems[i]->Equipped())
		{
			iWeapon = this->m_vItems[i];
			i = this->m_vItems.size();
		}
	}

	CItem * iArmor = nullptr;

	for(unsigned int i = 0; i < this->m_vItems.size(); i++)
	{
		if(this->m_vItems[i]->GetType() == 1 && this->m_vItems[i]->Equipped())
		{
			iArmor = this->m_vItems[i];
			i = this->m_vItems.size();
		}
	}

	if(iWeapon != nullptr)
	{
		nSoul += iWeapon->GetSoul();
	}

	if(iArmor != nullptr)
	{
		nSoul += iArmor->GetSoul();
	}

	return nSoul;
}

RECT CPlayer::GetActiveRec()
{
	RECT rec;
	rec.left = rec.right = rec.top = rec.bottom = 0;
	if (m_bEarthWall)
		return rEWall;
	return rec;
}

void CPlayer::AddKey( void )
{
	++m_nKeys; 
	CAudioManager::GetInstance()->PlaySFX( SFX_KEY ); 
}

void CPlayer::ArtifactTransition()
{
	if (TransitionEffect != nullptr)
		delete TransitionEffect;

	TransitionEffect = new CParticleEffect;
	TransitionTimer = 0.75f;

	switch (m_nCurrAtr)
	{
	case 1:
		CParticleManager::GetInstance()->GetEffect( *TransitionEffect, FX_FIRETRANSITION );
		CAudioManager::GetInstance()->PlaySFX( SFX_FIREBALL );
		TransitionEffect->ToggleCamera();
		TransitionEffect->SetXPosition(GetPosX());
		TransitionEffect->SetYPosition(GetPosY());
		break;
	case 2:
		CParticleManager::GetInstance()->GetEffect( *TransitionEffect,  FX_ICESLASH);
		CAudioManager::GetInstance()->PlaySFX( SFX_FREEZE );
		TransitionEffect->ToggleCamera();
		TransitionEffect->SetXPosition(GetPosX());
		TransitionEffect->SetYPosition(GetPosY());
		break;
	case 3:
		CParticleManager::GetInstance()->GetEffect( *TransitionEffect, FX_ROCKSHIELD );
		CAudioManager::GetInstance()->PlaySFX( SFX_EARTHSHARD ) ;
		TransitionEffect->ToggleCamera();
		TransitionEffect->SetXPosition(GetPosX());
		TransitionEffect->SetYPosition(GetPosY());
		break;
	case 4:
		CParticleManager::GetInstance()->GetEffect( *TransitionEffect, FX_LIGHTNINGDASH );
		CAudioManager::GetInstance()->PlaySFX( SFX_LDASH );
		TransitionEffect->ToggleCamera();
		TransitionEffect->SetXPosition(GetPosX() - 15);
		TransitionEffect->SetYPosition(GetPosY() - 55);
		break;
	}

	
	TransitionEffect->Initialize();
}

void CPlayer::AddArtifact(bool bUpdating)
{
	if (m_nNumArt == 0)
	{
		FireArtifact* fire = new FireArtifact();
		fire->SetPlayer(this);
		m_vArtifacts.push_back(fire);
		m_nNumArt = 1;
		m_nCurrAtr = 1;
	}
	else if (m_nNumArt == 1)
	{
		CSnowArtifact* snow = new CSnowArtifact();
		m_vArtifacts.push_back(snow); 
		snow->SetPlayer(this);
		m_nNumArt = 2;
		m_nCurrAtr = 2;
	}
	else if (m_nNumArt == 2)
	{
		CEarthArtifact* earth = new CEarthArtifact();
		m_vArtifacts.push_back(earth); 
		earth->SetPlayer(this);
		m_nNumArt = 3;
		m_nCurrAtr = 3;
	}
	else if (m_nNumArt == 3)
	{
		CElectricArtifact* elec = new CElectricArtifact();
		m_vArtifacts.push_back(elec); 
		elec->SetPlayer(this);
		m_nNumArt = 4;
		m_nCurrAtr = 4;
	}

	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-089
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	// Only do particle effect if the game isn't updating the player from a load file
	if(!bUpdating)
	{
		ArtifactTransition();
	}

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-089
	//////////////////////////////////////////////////////////////////////////
}

void CPlayer::IncreaseArtifact(int nIndex)
{
	m_vArtifacts[nIndex]->LevelUp();
}

void CPlayer::StopDash()
{
	m_bLunge = false;

	SetVelX(0);
	SetVelY(0);

	m_fLungeTimer = 0.0f;
	if (DashEffect != nullptr)
		DashEffect->StopEmitting();

	switch(this->m_nFacing)
	{
	case Up:
		{
			this->SetCurrAnim(playerStandUp + 32 * m_nCurrAtr);
		}							
		break;						
	case Down:						
		{							
			this->SetCurrAnim(playerStandDown + 32 * m_nCurrAtr);
		}							
		break;						
	case Left:						
		{							
			this->SetCurrAnim(playerStandLeft + 32 * m_nCurrAtr);
		}						
		break;					
	case Right:					
		{						
			this->SetCurrAnim(playerStandRight + 32 * m_nCurrAtr);
		}
		break;
	}
}

void CPlayer::SetExpNeeded(int nLevel)
{
	switch (nLevel)
	{
	case 2: m_nToNextLevel = 125; break;
	case 3: m_nToNextLevel = 150; break;
	case 4: m_nToNextLevel = 175; break;
	case 5: m_nToNextLevel = 200; break;
	case 6: m_nToNextLevel = 1000; break;
	case 7: m_nToNextLevel = 1250; break;
	case 8: m_nToNextLevel = 1500; break;
	case 9: m_nToNextLevel = 1750; break;
	case 10: m_nToNextLevel = 2000; break;
	case 11: m_nToNextLevel = 10000; break;
	case 12: m_nToNextLevel = 12500; break;
	case 13: m_nToNextLevel = 15000; break;
	case 14: m_nToNextLevel = 17500; break;
	case 15: m_nToNextLevel = 20000; break;
	case 16: m_nToNextLevel = 100000; break;
	case 17: m_nToNextLevel = 125000; break;
	case 18: m_nToNextLevel = 150000; break;
	case 19: m_nToNextLevel = 175000; break;
	case 20: m_nToNextLevel = 200000; break;
	case 21: m_nToNextLevel = 200000; break;
	case 22: m_nToNextLevel = 200000; break;
	case 23: m_nToNextLevel = 200000; break;
	case 24: m_nToNextLevel = 200000; break;
	case 25: m_nToNextLevel = 200000; break;
	}
}