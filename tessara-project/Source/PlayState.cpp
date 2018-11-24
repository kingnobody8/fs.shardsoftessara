/*
File:		PlayState.cpp
Author:		Daniel Habig
Course:		SGP
Purpose:	The Play State class is the game state for normal gameplay.
*/

#include "PlayState.h"
#include "InventoryState.h"
#include "SocketingState.h"

#include "Game.h"
#include "LevelManager.h"
#include "AnimationManager.h"
#include "DeleteEntityMessage.h"
#include "ItemManager.h"
#include "LoadGameState.h"
#include "Camera.h"

#include "TestMsg.h"

//Messages
#include "MessageSystem.h"
#include "InputManager.h"
#include "EventSystem.h"
#include "EntityManager.h"
#include "AddCurrencyMessage.h"
#include "DeleteProjectileMessage.h"
#include "CreateProjectileMsg.h"
#include "CreateTripleShootMsg.h"
#include "AddItemMsg.h"
#include "LoadLevelMsg.h"
#include "AudioManager.h"
#include "ParticleManager.h"
#include "AddEntityToManager.h"

// Other States
#include "MainMenuState.h"
#include "LevelUpMenu.h"
#include "InformationManagementState.h"
#include "CreditsState.h"

// Assert
#include <cassert>

#include "Projectile.h"
#include "chakram.h"
#include "Lightning.h"

// Save/Load
#include "..\\TinyXML\tinyxml.h"
#include "..\SGD Wrappers\CSGD_EventSystem.h"
#include <ShlObj.h>

// Enemies
#include "Wight.h"
#include "Archer.h"
#include "Zombie.h"
#include "Wraight.h"
#include "NPC.h"
#include "Boss.h"
#include "Power.h"

// SINGLETON!
/*static*/ PlayState*	PlayState::GetInstance( void )
{
	// Static local variable
	static PlayState s_Instance;

	return &s_Instance;
}

PlayState::PlayState(void)
{
	WeatherEffect   = nullptr;
	m_pMS			= nullptr;
	m_pLM			= nullptr;
	m_pIM			= nullptr;
	m_pES			= nullptr;
	m_pEM			= nullptr;

	m_pPlayer		= nullptr;
	m_bPaused		= false;
	m_bQuitToMenu	= false;
	m_bLevelUp		= false;
	exit			= false;
	endBool			= false;
	agro			= false;
	transLoaded		= false;
	aState			= REGULAR;
	fadeRate		= 0.0f;
	transTimer		= 0.0f;
	m_nMenuButtonID = -1;
	m_nBoxImgID		= -1;
	deathBoxImgID	= -1;
	endAlpha		= 5.0f;
	deathSelect		= 0;
	currState		= PLAY;
	m_nHelpPanelID	= -1;

}

PlayState::~PlayState(void)
{
	if (WeatherEffect != nullptr)
		delete WeatherEffect;
}

/*virtual*/ void	PlayState::Enter ( void )
{
	// Help Message Assets
	ShowHelpMessage = false;
	m_nHelpIndex = 0;
	m_nHelpPanelID =  CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/panel_large.png"));

	this->m_nLoadingBackgroundImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/Loading/loading_background.png"));
	this->m_nLoadingBarImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/Loading/loading_bar.png"));
	this->noteImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/menuButton.png"));
	deathBoxImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/boxWeird.png"));
	this->m_nLoaded = 0;
	this->m_bLoading = true;
	this->m_nLoadingIndex = 0;
	CAudioManager::GetInstance()->LoadAudio( GAMEPLAY_AUDIO );
}

/*virtual*/ void	PlayState::Exit  ( void )
{
	CAudioManager::GetInstance()->UnloadAudio( GAMEPLAY_AUDIO );

	if (m_nHelpPanelID != -1)
	{
		CSGD_TextureManager::GetInstance()->UnloadTexture( m_nHelpPanelID );
		m_nHelpPanelID = -1;
	}

	if (deathBoxImgID != -1)
	{
		CSGD_TextureManager::GetInstance()->UnloadTexture( deathBoxImgID );
		deathBoxImgID = -1;
	}
	if (noteImgID != -1)
	{
		CSGD_TextureManager::GetInstance()->UnloadTexture( noteImgID );
		noteImgID = -1;
	}
	if (m_nBoxImgID != -1)
	{
		CSGD_TextureManager::GetInstance()->UnloadTexture( m_nBoxImgID );
		m_nBoxImgID = -1;
	}
	if (m_nMenuButtonID != -1)
	{
		CSGD_TextureManager::GetInstance()->UnloadTexture( m_nMenuButtonID  );
		m_nBoxImgID = -1;
	}
	if (m_nLoadingBackgroundImgID != -1)
	{
		CSGD_TextureManager::GetInstance()->UnloadTexture( m_nLoadingBackgroundImgID  );
		m_nLoadingBackgroundImgID = -1;
	}
	if (m_nLoadingBarImgID != -1)
	{
		CSGD_TextureManager::GetInstance()->UnloadTexture( m_nLoadingBarImgID  );
		m_nLoadingBarImgID = -1;
	}

	m_pLM->Exit();
	m_pLM = nullptr;

	if( m_pMS != nullptr )
	{
		m_pMS->ShutdownSystem();
		m_pMS = nullptr;
	}


	if( m_pES != nullptr )
	{
		m_pES->Shutdown();
		m_pES = nullptr;
	}

	if( m_pEM != nullptr )
	{
		m_pEM->DeleteInstance();
		m_pEM = nullptr;
	}

	m_pIM = nullptr;

	if ( m_pPlayer != nullptr)
	{
		m_pPlayer->Release();
		m_pPlayer = nullptr;
	}


}

/*virtual*/ bool	PlayState::Input ( void )
{
	if(!this->m_bLoading)
	{
		if (ShowHelpMessage) // Help Message Input
			return HelpMessageInput();

		switch( currState )
		{
		case PLAY:
			if (m_pIM->CheckPause())
			{
				CAudioManager::GetInstance()->PlaySFX( SFX_PAUSE );
				m_nSelectedIndex = 1;
				currState = PAUSE;
				return true;
			}
			return m_pPlayer->Input();
			break;
		case PAUSE:
			return PauseInput();
			break;
		case NOTE:
			return NoteInput();
			break;
		case DEATH:
			return DeathInput();
			break;
		case END:
			{
				return EndInput();
				break;
			}
		case TRANS:
			{
				return TransInput();
				break;
			}
		}
	}
	return true;
}

/*virtual*/ void	PlayState::Update( float fElapsedTime )
{
	if( exit )
	{
		GoBackToMenu();
	}
	else
	{
		if(this->m_bLoading)
		{
			LoadingUpdate();
		}
		else
		{
			switch( currState )
			{
			case PLAY:
				{
					if (WeatherEffect != nullptr)
					{
						if (WeatherEffect->UsingCamera())
						{
							WeatherEffect->SetXPosition(Camera::GetInstance()->GetLeft() + 400);
							WeatherEffect->SetYPosition(Camera::GetInstance()->GetTop());
						}
						WeatherEffect->Update( fElapsedTime );
					}

					m_pLM->Update( fElapsedTime );
					m_pEM->UpdateAll(fElapsedTime);
					m_pEM->FindCollisions();
					m_pMS->ProcessMessages();
					m_pES->ProcessEvents();
					m_pIM->UpdateVibe( fElapsedTime );
					AudioUpdate(fElapsedTime);
					Camera::GetInstance()->Update( fElapsedTime );
					break;
				}
			case PAUSE:
				{
					PauseUpdate( fElapsedTime );
					Camera::GetInstance()->Update( fElapsedTime );

					break;
				}
			case NOTE:
				{
					NoteUpdate(fElapsedTime);
					Camera::GetInstance()->Update( fElapsedTime );

					break;
				}
			case DEATH:
				DeathUpdate(fElapsedTime);
				Camera::GetInstance()->Update( fElapsedTime );

				m_pIM->UpdateVibe( fElapsedTime );
				{ break; }
			case END:
				EndUpdate(fElapsedTime);
				m_pIM->UpdateVibe( fElapsedTime );
				Camera::GetInstance()->Update( fElapsedTime );

				break;
			case TRANS:
				{
					TransUpdate( fElapsedTime);
					Camera::GetInstance()->Update( fElapsedTime );
					break;
				}
			}

		}
	}
}

/*virtual*/ void	PlayState::Render( void )
{
	if(this->m_bLoading)
	{
		LoadingRender();
	}
	else
	{
		switch( currState )
		{
		case PLAY:
			{
				m_pLM->Render(255);
				m_pEM->RenderAll();
				if (WeatherEffect != nullptr)
					WeatherEffect->Render();
				m_pPlayer->DrawVignette();
				m_pPlayer->DrawHUD();
				break;
			}
		case PAUSE:
			{
				/////////////////////////////////////////////////////////////////
				// BUG FIX
				// Reference Bug # BB-095
				// BUG FIX START
				/////////////////////////////////////////////////////////////////

				// Changed render() to always do particles, render weather, renders vignette
				m_pLM->Render(128);
				m_pEM->RenderAll();
				if (WeatherEffect != nullptr)
					WeatherEffect->Render();
				m_pPlayer->DrawVignette();
				PauseRender();
				break;

				///////////////////////////////////////////////////////////////////////////
				// BUG FIX END  Reference # BB-095
				//////////////////////////////////////////////////////////////////////////
			}
		case NOTE:
			{
				m_pLM->Render(128);
				m_pEM->RenderAll();
				if (WeatherEffect != nullptr)
					WeatherEffect->Render();
				m_pPlayer->DrawVignette();
				NoteRender();
				break;
			}
		case DEATH:
			DeathRender();
			{ break; }
		case END:
			EndRender();
			break;
		case TRANS:
			{
				TransRender();
				break;
			}
		}

		if (ShowHelpMessage)
			HelpMessageRender();
	}
}

bool PlayState::HelpMessageInput( void )
{
	if (CInputManager::GetInstance()->CheckUp())
	{
		CAudioManager::GetInstance()->PlaySFX(SFX_CURSOR);
		m_nHelpIndex = 0;
	}
	if (CInputManager::GetInstance()->CheckDown())
	{
		CAudioManager::GetInstance()->PlaySFX(SFX_CURSOR);
		m_nHelpIndex = 1;
	}

	if (CInputManager::GetInstance()->CheckEnter())
	{
		if (m_nHelpIndex == 0)
		{
			CAudioManager::GetInstance()->PlaySFX(SFX_PAUSE);
			m_pPlayer->SetHelpMessages(m_pPlayer->GetHelpMessages() ^ 1 << 5);
		}

		if (m_nHelpIndex == 1)
		{
			CAudioManager::GetInstance()->PlaySFX(SFX_CONFIRM);
			ShowHelpMessage = false;
		}
	}
	return true;
}

void PlayState::HelpMessageRender()
{
	CSGD_TextureManager::GetInstance()->Draw(m_nHelpPanelID, 50,50,0.8f,0.7f,0,0,0,0,D3DCOLOR_ARGB(195,255,255,255));
	m_pBF->Draw("You leveled up! Every level you gain",95,75,0.6f,D3DCOLOR_ARGB(255,255,255,255));
	m_pBF->Draw("earns you two free attribute points.",95,95,0.6f,D3DCOLOR_ARGB(255,255,255,255));
	m_pBF->Draw("Head to the Player section of the   ",95,115,0.6f,D3DCOLOR_ARGB(255,255,255,255));
	m_pBF->Draw("Pause menu to allocate your points!",95,135,0.6f,D3DCOLOR_ARGB(255,255,255,255));

	CSGD_TextureManager::GetInstance()->Draw(noteImgID, 250,400,0.1f,0.5f,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
	m_pBF->Draw("Don't show this message again",280,410,0.4f,D3DCOLOR_ARGB(255,255,255,255));

	if (m_pPlayer->GetHelpMessages() & 1 << 5)
		m_pBF->Draw("X",255,405,0.4f,D3DCOLOR_ARGB(100,0,0,0));

	CSGD_TextureManager::GetInstance()->Draw(noteImgID, 350,500,0.5f,0.5f,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
	m_pBF->Draw("  Continue",360,510,0.4f,D3DCOLOR_ARGB(100,0,0,0));

	if (m_nHelpIndex == 0)
	{
		CSGD_TextureManager::GetInstance()->Draw(noteImgID, 250,400,0.1f,0.5f,0,0,0,0,D3DCOLOR_ARGB(200,255,255,255));
		if (m_pPlayer->GetHelpMessages() & 1 << 5)
			m_pBF->Draw("X",255,405,0.4f,D3DCOLOR_ARGB(255,0,0,0));
	}
	else
	{
		CSGD_TextureManager::GetInstance()->Draw(noteImgID, 350,500,0.5f,0.5f,0,0,0,0,D3DCOLOR_ARGB(200,255,255,255));
		m_pBF->Draw("  Continue",360,510,0.4f,D3DCOLOR_ARGB(200,0,0,0));
	}
}

// Handles all the messages
/*virtual*/ void PlayState::MessageProc( IMessage* pMsg )
{
	//invoking object
	PlayState * self = PlayState::GetInstance();


	// What type of message?
	switch( pMsg->GetMessageID() )
	{


	case MSG_DELETEPROJ:
		{
			CDeleteProjectileMessage * pCreateMsg = dynamic_cast< CDeleteProjectileMessage* >( pMsg );

			self->m_pEM->RemoveEntity( pCreateMsg->GetProjectile() );

		}
		break;

	case MSG_CREATEPROJECTILE:
		{
			CreateProjectileMsg * pCreateMsg = dynamic_cast< CreateProjectileMsg* >( pMsg );

			if (pCreateMsg->GetIsCh())
			{
				chakram* pr = new chakram();
				pr->SetProjectile(pCreateMsg->GetVel(), pCreateMsg->GetDirection(), pCreateMsg->GetPosX(), pCreateMsg->GetPosY());
				pr->SetElement(0);
				pr->SetOwnerp(false);
				if (pCreateMsg->GetLevel() != 0)
				{
					switch (pCreateMsg->GetLevel())
					{
					case 1:
						pr->SetDamage( pr->GetDamage() + 10 );
						break;
					case 2:
						pr->SetDamage(pr->GetDamage() + 10 );
						pr->SetBounceLimit(7);
					case 3:
						pr->SetDamage(pr->GetDamage() + 10 );
						pr->SetBounceLimit(7);
						pr->SetVelocity(pCreateMsg->GetVel() + 150.0f);
						break;
					}
				}
				self->m_pEM->AddEntity(pr);
			}
			else if (pCreateMsg->GetElement() == 2)
			{
				CLightning* pr = new CLightning();
				pr->SetProjectile(pCreateMsg->GetVel(), pCreateMsg->GetDirection(), pCreateMsg->GetPosX(), pCreateMsg->GetPosY());
				pr->SetElement(pCreateMsg->GetElement());
				pr->SetOwnerp(false);
				self->m_pEM->AddEntity(pr);
			}

			else if (pCreateMsg->GetElement() == 3)
			{
				CProjectile* pr = new CProjectile();
				pr->SetProjectile(pCreateMsg->GetVel(), pCreateMsg->GetDirection(), pCreateMsg->GetPosX(), pCreateMsg->GetPosY());
				pr->SetElement(pCreateMsg->GetElement());
				pr->SetPierce(true);
				pr->SetOwnerp(false);
				self->m_pEM->AddEntity(pr);
			}
			else
			{
				CProjectile* pr = new CProjectile();

				if (pCreateMsg->GetElement() == 0)
					pr->SetLevel( pCreateMsg->GetLevel());

				pr->SetProjectile(pCreateMsg->GetVel(), pCreateMsg->GetDirection(), pCreateMsg->GetPosX(), pCreateMsg->GetPosY());
				pr->SetElement(pCreateMsg->GetElement());
				pr->SetOwnerp(false);
				self->m_pEM->AddEntity(pr);
			}
		}
		break;


	case MSG_TREIPLESHOOT:
		{
			CreateTripleShootMsg * pCreateMsg = dynamic_cast< CreateTripleShootMsg* >( pMsg );

			CLightning* pr = new CLightning();
			pr->SetProjectile(pCreateMsg->GetVel(), pCreateMsg->GetDirection(), pCreateMsg->GetPosX(), pCreateMsg->GetPosY());
			pr->SetElement(pCreateMsg->GetElement());
			pr->SetOwnerp(false);
			self->m_pEM->AddEntity(pr);

			if (pCreateMsg->GetDirection() >= 2)
			{
				CLightning* pr1 = new CLightning();
				pr1->SetProjectile(pCreateMsg->GetVel(), pCreateMsg->GetDirection(), pCreateMsg->GetPosX(), pCreateMsg->GetPosY());
				pr1->SetElement(pCreateMsg->GetElement());
				pr1->SetAccY(-200);
				pr1->SetOwnerp(false);
				self->m_pEM->AddEntity(pr1);

				CLightning* pr2 = new CLightning();
				pr2->SetProjectile(pCreateMsg->GetVel(), pCreateMsg->GetDirection(), pCreateMsg->GetPosX(), pCreateMsg->GetPosY());
				pr2->SetElement(pCreateMsg->GetElement());
				pr2->SetAccY(200);
				pr2->SetOwnerp(false);
				self->m_pEM->AddEntity(pr2);
			}
			else	{
				CLightning* pr1 = new CLightning();
				pr1->SetProjectile(pCreateMsg->GetVel(), pCreateMsg->GetDirection(), pCreateMsg->GetPosX(), pCreateMsg->GetPosY());
				pr1->SetElement(pCreateMsg->GetElement());
				pr1->SetAccX(-200);
				pr1->SetOwnerp(false);
				self->m_pEM->AddEntity(pr1);

				CLightning* pr2 = new CLightning();
				pr2->SetProjectile(pCreateMsg->GetVel(), pCreateMsg->GetDirection(), pCreateMsg->GetPosX(), pCreateMsg->GetPosY());
				pr2->SetElement(pCreateMsg->GetElement());
				pr2->SetAccX(200);
				pr2->SetOwnerp(false);
				self->m_pEM->AddEntity(pr2);
			}


		}
		break;


	case MSG_DELETEENT:
		{
			CDeleteEntityMessage * pCreateMsg = dynamic_cast< CDeleteEntityMessage* >( pMsg );
			self->m_pEM->RemoveEntity( pCreateMsg->GetEntity() );

		}
		break;

	case MSG_ADDCURRENCY:
		{
			AddCurrencyMessage * pCreateMsg = dynamic_cast< AddCurrencyMessage* >( pMsg );

			self->m_pPlayer->AddCurrency( pCreateMsg->GetAmount());

		}
		break;

	case MSG_ADDKEY:
		{
			self->m_pPlayer->AddKey();
		}
		break;

	case MSG_REMOVEKEY:
		{
			self->m_pPlayer->RemoveKey();
		}
		break;

	case MSG_ADDSHARD:
		{
			self->m_pPlayer->AddShard();
		}
		break;

		//Add or remove item from entity manager
	case MSG_ADDITEM:
		{	
			AddItemMsg * pCreateMsg = dynamic_cast< AddItemMsg* >( pMsg );

			if (pCreateMsg->GetRemove())
				CEntityManager::GetInstance()->AddEntity(pCreateMsg->GetItem());
			else
				CEntityManager::GetInstance()->RemoveEntity(pCreateMsg->GetItem());
		}
		break;

	case MSG_ADDENTITYTOMANAGER:
		{
			AddEntityToManager* pW = dynamic_cast< AddEntityToManager* >( pMsg );
			CEntityManager::GetInstance()->AddEntity(pW->GetEntity());
		}
		break;

	case MSG_REMOVESHARD:
		{
			self->m_pPlayer->RemoveShard();
		}
		break;

	case MSG_LOADLEVEL:
		{
			LoadLevelMsg* pLLM = dynamic_cast< LoadLevelMsg* >( pMsg );
			self->transPort = pLLM->GetPortal();
			self->GoToTrans();
		}
		break;

	case MSG_TEST:
		{
			CTestMsg* pW = dynamic_cast< CTestMsg* >( pMsg );
			self->CreateArcher(pW->GetPX(), pW->GetPY(), 1);
		}
		break;

	default:
		{
			assert( false && "PlayState::MessageProc - unknown message type" );
		}
		break;

	}
}

void PlayState::CreatePlayer( int nPosX, int nPosY)
{
	if(m_pPlayer == nullptr)
	{
		m_pPlayer = new CPlayer();

		CLoadGameState::GetInstance()->UpdatePlayer(*m_pPlayer);

		/////////////////////////////////////////////////////////////////
		// BUG FIX
		// Reference Bug # BB-031
		// BUG FIX START
		/////////////////////////////////////////////////////////////////

		// If the player has any artifacts, set the player's animation to "fire" standing instead of "none" standing
		if(m_pPlayer->GetArtifactNumber() > 0)
		{
			m_pPlayer->SetCurrAnim(33);

			/////////////////////////////////////////////////////////////////
			// BUG FIX
			// Reference Bug # BB-066
			// BUG FIX START
			/////////////////////////////////////////////////////////////////

			// Sets the currently selected artifact to fire
			m_pPlayer->SetCurrAtr(1);

			///////////////////////////////////////////////////////////////////////////
			// BUG FIX END  Reference # BB-066
			//////////////////////////////////////////////////////////////////////////
		}

		///////////////////////////////////////////////////////////////////////////
		// BUG FIX END  Reference # BB-031
		//////////////////////////////////////////////////////////////////////////

		m_pEM->AddEntity(m_pPlayer);
		m_pPlayer->AddRef();

		m_pPlayer->SetPosX(nPosX);
		m_pPlayer->SetPosY(nPosY);
	}
}

void PlayState::MovePlayer( int nPosX, int nPosY)
{
	m_pPlayer->SetPosX(nPosX);
	m_pPlayer->SetPosY(nPosY);
}

void PlayState::CreateEnt( vector<Area> nNodes, string name, string text, string item)
{
	NPC* temp = new NPC();
	temp->SetNPC(nNodes, name, text, item);

	m_pEM->AddEntity( temp );
	temp = nullptr;
}

void PlayState::CreateWight( int nPosX, int nPosY, int lev)
{

	CWight* m_pEnemyW = new CWight();
	m_pEnemyW->SetTarget(m_pPlayer);
	m_pEnemyW->SetHeight(40);
	m_pEnemyW->SetWidth(40);
	m_pEnemyW->SetPosX(nPosX);
	m_pEnemyW->SetPosY(nPosY);	
	m_pEnemyW->SetLevel( lev );
	m_pEnemyW->SetArea();
	m_pEM->AddEntity( m_pEnemyW );
	m_pEnemyW = nullptr;
}

void PlayState::CreateArcher(int nPosX, int nPosY, int lev)
{
	CArcher* m_pEnemyA = new CArcher();
	m_pEnemyA->SetTarget(m_pPlayer);
	m_pEnemyA->SetHeight(60);
	m_pEnemyA->SetWidth(40);
	m_pEnemyA->SetPosX(nPosX);
	m_pEnemyA->SetPosY(nPosY);	
	m_pEnemyA->SetLevel( lev );
	m_pEnemyA->SetArea();
	m_pEM->AddEntity( m_pEnemyA );
	m_pEnemyA = nullptr;

}

void PlayState::CreateZombie( int nPosX, int nPosY, int lev)
{
	CZombie* m_pEnemyW = new CZombie();
	m_pEnemyW->SetTarget(m_pPlayer);
	m_pEnemyW->SetHeight(40);
	m_pEnemyW->SetWidth(40);
	m_pEnemyW->SetPosX(nPosX);
	m_pEnemyW->SetPosY(nPosY);	
	m_pEnemyW->SetLevel( lev );
	m_pEnemyW->SetArea();
	m_pEM->AddEntity( m_pEnemyW );
	m_pEnemyW = nullptr;
}

void PlayState::CreateWraight( int nPosX, int nPosY, int lev)
{
	CWraight * m_pEnemyW = new CWraight();
	m_pEnemyW->SetPlayer(m_pPlayer);
	m_pEnemyW->SetHeight(40);
	m_pEnemyW->SetWidth(40);
	m_pEnemyW->SetPosX(nPosX);
	m_pEnemyW->SetPosY(nPosY);	
	m_pEnemyW->SetLevel(lev);	
	m_pEnemyW->SetArea();
	m_pEM->AddEntity( m_pEnemyW );
	m_pEnemyW = nullptr;
}

void PlayState::CreateBoss(int nPosX, int nPosY, int lev)
{
	CBoss* m_pEnemyA = new CBoss();
	m_pEnemyA->SetTarget(m_pPlayer);
	m_pEnemyA->SetHeight(60);
	m_pEnemyA->SetWidth(40);
	m_pEnemyA->SetPosX(nPosX);
	m_pEnemyA->SetPosY(nPosY);	
	//This is a little different for the boss
	//---------------------------
	m_pEnemyA->SetBoss(lev);
	m_pEnemyA->SetLevel(lev);	
	m_pEnemyA->SetArea();
	//---------------------------
	m_pEM->AddEntity( m_pEnemyA );
	m_pEnemyA = nullptr;
}

//Doors and lightning objects
Door* PlayState::CreateDoor(int nPosX, int nPosY, int type, vector<Area> nodes, bool back)
{
	Door* door = new Door();
	Area loc;
	loc.cols = nPosX;
	loc.rows = nPosY;
	door->Create(type, loc, nodes);

	m_pEM->AddEntity( door );
	if( !back )
	{
		door = nullptr;
		return door;
	}
	else
		door->AddRef(); //For the lightning object
	return door;

}

void PlayState::CreatePower(int nPosX, int nPosY, Door *door)
{
	Power* pow = new Power();
	pow->SetPosX( nPosX );
	pow->SetPosY( nPosY );
	pow->SetDoor( door );
	m_pEM->AddEntity( pow );
}

CKey* PlayState::CreateKey(int nPosX, int nPosY)
{
	CKey* key = new CKey();
	Area loc;
	loc.cols = nPosX;
	loc.rows = nPosY;
	key->Create(loc);

	m_pEM->AddEntity(key);

	return key;
}

CShard* PlayState::CreateShard(int nPosX, int nPosY)
{
	CShard* shard = new CShard();
	Area loc;
	loc.cols = nPosX;
	loc.rows = nPosY;
	shard->Create(loc);

	m_pEM->AddEntity(shard);

	return shard;
}

void PlayState::UsedSaveSlot()
{
	m_bUsedSaveSlot = !m_bUsedSaveSlot;
}

void PlayState::PauseUpdate( float deltaTime )
{
	for (int i = 0; i < 7; ++i)
		Alpha[i] = 100;

	Alpha[m_nSelectedIndex - 1] = 255;
}

void PlayState::PauseRender( void )
{
	CSGD_TextureManager::GetInstance()->Draw(m_nBoxImgID,215,20,1.0f,1.0f,0,0,0,0,D3DCOLOR_ARGB(255,255,255,255));

	CSGD_TextureManager::GetInstance()->Draw(m_nMenuButtonID,335,100,0.95f,0.5f,0,0,0,0,D3DCOLOR_ARGB(Alpha[0],185,185,185));
	m_pBF->Draw("  Resume", 360, 110, 0.5f, D3DCOLOR_ARGB(Alpha[0], 255, 255, 255));
	CSGD_TextureManager::GetInstance()->Draw(m_nMenuButtonID,335,150,0.95f,0.5f,0,0,0,0,D3DCOLOR_ARGB(Alpha[1],185,185,185));
	m_pBF->Draw("Inventory", 360, 160, 0.5f, D3DCOLOR_ARGB(Alpha[1], 255, 255, 255));
	CSGD_TextureManager::GetInstance()->Draw(m_nMenuButtonID,335,200,0.95f,0.5f,0,0,0,0,D3DCOLOR_ARGB(Alpha[2],185,185,185));
	m_pBF->Draw("   Player", 360, 210, 0.5f, D3DCOLOR_ARGB(Alpha[2], 255, 255, 255));
	CSGD_TextureManager::GetInstance()->Draw(m_nMenuButtonID,335,250,0.95f,0.5f,0,0,0,0,D3DCOLOR_ARGB(Alpha[3],185,185,185));
	m_pBF->Draw("  Options", 360, 260, 0.5f, D3DCOLOR_ARGB(Alpha[3], 255, 255, 255));
	CSGD_TextureManager::GetInstance()->Draw(m_nMenuButtonID,335,300,0.95f,0.5f,0,0,0,0,D3DCOLOR_ARGB(Alpha[4],185,185,185));
	m_pBF->Draw("    Save", 360, 310, 0.5f, D3DCOLOR_ARGB(Alpha[4], 255, 255, 255));
	CSGD_TextureManager::GetInstance()->Draw(m_nMenuButtonID,335,350,0.95f,0.5f,0,0,0,0,D3DCOLOR_ARGB(Alpha[5],185,185,185));
	m_pBF->Draw("    Load", 360, 360, 0.5f, D3DCOLOR_ARGB(Alpha[5], 255, 255, 255));
	CSGD_TextureManager::GetInstance()->Draw(m_nMenuButtonID,335,400,0.95f,0.5f,0,0,0,0,D3DCOLOR_ARGB(Alpha[6],185,185,185));
	m_pBF->Draw("    Quit", 360, 410, 0.5f, D3DCOLOR_ARGB(Alpha[6], 255, 255, 255));
}

void PlayState::Weather_Rain( void )
{
	if (WeatherEffect != nullptr)
	{
		delete WeatherEffect;
		WeatherEffect = nullptr;
	}

	WeatherEffect = new CParticleEffect;
	CParticleManager::GetInstance()->GetEffect(*WeatherEffect, FX_RAIN);
	WeatherEffect->SetXPosition(400);
	WeatherEffect->SetYPosition( -10);
	WeatherEffect->Initialize();
}

void PlayState::Weather_Sandstorm( void )
{
	if (WeatherEffect != nullptr)
	{
		delete WeatherEffect;
		WeatherEffect = nullptr;
	}
}

void PlayState::Weather_Snow( void )
{
	if (WeatherEffect != nullptr)
	{
		delete WeatherEffect;
		WeatherEffect = nullptr;
	}

	WeatherEffect = new CParticleEffect;
	CParticleManager::GetInstance()->GetEffect(*WeatherEffect, FX_SNOW);
	WeatherEffect->ToggleCamera();
	WeatherEffect->SetXPosition(Camera::GetInstance()->GetLeft());
	WeatherEffect->SetYPosition(Camera::GetInstance()->GetTop() -10);
	WeatherEffect->Initialize();
}

void PlayState::Weather_None( void )
{
	if (WeatherEffect != nullptr)
	{
		delete WeatherEffect;
		WeatherEffect = nullptr;
	}
}

bool PlayState::PauseInput( void )
{
	if (m_pIM->CheckUp())
	{
		CAudioManager::GetInstance()->PlaySFX( SFX_PAUSEMOVE );
		--m_nSelectedIndex;
	}
	if (m_pIM->CheckDown())
	{
		CAudioManager::GetInstance()->PlaySFX( SFX_PAUSEMOVE );
		++m_nSelectedIndex;
	}

	if (m_nSelectedIndex > 7)
		m_nSelectedIndex = 1;
	if (m_nSelectedIndex < 1)
		m_nSelectedIndex = 7;

	if (m_pIM->CheckEnter())
	{
		CAudioManager::GetInstance()->PlaySFX( SFX_PAUSE );

		switch( m_nSelectedIndex )
		{
		case 1:
			{
				currState = PLAY;
				return true;
				break;
			}

		case 2:
			{
				// INVENTORY
				CGame::GetInstance()->ChangeToNew( CInventoryState::GetInstance() );
				break;
			}
		case 3:
			{
				// Level Up
				CGame::GetInstance()->ChangeToNew( CLevelUpMenu::GetInstance() );
				break;
			}
		case 4:
			{
				// Options
				CGame::GetInstance()->ChangeToNew( InformationManagementState::GetInstance() );
				return true;
				break;
			}
		case 5:
			{
				// SAVE
				SavePlayer();
				return true;
				break;
			}
		case 6:
			{
				// LOAD

				/////////////////////////////////////////////////////////////////
				// BUG FIX
				// Reference Bug # BB-018
				// BUG FIX START
				/////////////////////////////////////////////////////////////////

				//CGame::GetInstance()->ChangeToPrev(2);

				///////////////////////////////////////////////////////////////////////////
				// BUG FIX END  Reference # BB-018
				//////////////////////////////////////////////////////////////////////////

				/////////////////////////////////////////////////////////////////
				// BUG FIX
				// Reference Bug # BB-003
				// BUG FIX START
				/////////////////////////////////////////////////////////////////

				this->currState = PLAY;
				CLoadGameState::GetInstance()->FromPause(true);

				///////////////////////////////////////////////////////////////////////////
				// BUG FIX END  Reference # BB-003
				//////////////////////////////////////////////////////////////////////////

				CLoadGameState::GetInstance()->SetNewGame(false);
				CGame::GetInstance()->ChangeToNew(CLoadGameState::GetInstance());

				break;
			}
		case 7:
			{
				m_bPaused = false;

				/////////////////////////////////////////////////////////////////
				// BUG FIX
				// Reference Bug # BB-003
				// BUG FIX START
				/////////////////////////////////////////////////////////////////

				this->currState = PLAY;

				///////////////////////////////////////////////////////////////////////////
				// BUG FIX END  Reference # BB-003
				//////////////////////////////////////////////////////////////////////////

				/////////////////////////////////////////////////////////////////
				// BUG FIX
				// Reference Bug # BB-063
				// BUG FIX START
				/////////////////////////////////////////////////////////////////

				// Set from pause back to false, is set to true when entering pause state
				// Not 100% sure it's the fix, but it fixes a crash at the least.
				// Actual steps to recreate the #63 crash were undetermined, but verified that it was an actual bug - Evan
				CLoadGameState::GetInstance()->FromPause(false);

				///////////////////////////////////////////////////////////////////////////
				// BUG FIX END  Reference # BB-063
				//////////////////////////////////////////////////////////////////////////

				CGame::GetInstance()->ChangeToPrev(2);
				return true;
				break;
			}

		}
	}

	return true;
}

void PlayState::SavePlayer( void )
{
	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-068
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	// Doesn't save if in the tutorial, save lsot is set to "" when entering tutorial
	if(CGame::GetInstance()->GetSaveSlot() == "")
		return;

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-068
	//////////////////////////////////////////////////////////////////////////

	char path[MAX_PATH];
	LPWSTR wszPath = NULL;

	HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0,  0, &wszPath);

	size_t tempSize;
	wcstombs_s(&tempSize, path, wszPath, MAX_PATH);
	string pathtowrite = path;
	pathtowrite += "\\Shards of Tessera";
	TCHAR temp[MAX_PATH];

	CSTR_TO_WSTR(temp, MAX_PATH, pathtowrite.c_str());
	CreateDirectory(temp, 0);

	string save = CGame::GetInstance()->GetSaveSlot();

	pathtowrite += "\\" + save;

	TiXmlDocument doc;

	// Create a TinyXML Declaration (version & encoding)
	TiXmlDeclaration* pDecl = new TiXmlDeclaration( "1.0", "utf-8", "" );

	// Write the declaration to the document
	doc.LinkEndChild( pDecl );

	// Create a root node
	TiXmlElement* pRoot = new TiXmlElement( "SaveSlot" );

	pRoot->SetAttribute("Unspent", m_pPlayer->GetUnspent());
	pRoot->SetAttribute("Character", m_pPlayer->GetCharacterE());
	pRoot->SetAttribute("Experience", m_pPlayer->GetExperience());
	pRoot->SetAttribute("Currency", m_pPlayer->GetCurrency());
	pRoot->SetAttribute("Level", m_pPlayer->GetLevel());
	pRoot->SetAttribute("Body", m_pPlayer->GetBodyE());
	pRoot->SetAttribute("Mind", m_pPlayer->GetMindE());
	pRoot->SetAttribute("Soul", m_pPlayer->GetSoulE());
	pRoot->SetAttribute("Health", m_pPlayer->GetCurrHealth());
	pRoot->SetAttribute("Artifact1", m_pPlayer->GetArtifactLevel(0));
	pRoot->SetAttribute("Artifact2", m_pPlayer->GetArtifactLevel(1));
	pRoot->SetAttribute("Artifact3", m_pPlayer->GetArtifactLevel(2));
	pRoot->SetAttribute("Artifact4", m_pPlayer->GetArtifactLevel(3));
	pRoot->SetAttribute("Shards", m_pPlayer->GetShards());
	pRoot->SetAttribute("Potions", m_pPlayer->GetPotionCount());
	pRoot->SetAttribute("Portal", m_nPortal);
	pRoot->SetAttribute("Keys", m_pPlayer->GetKeys());
	pRoot->SetAttribute("Help", m_pPlayer->GetHelpMessages());

	TiXmlElement* pMap = new TiXmlElement("Map");

	TiXmlText* pText = new TiXmlText(m_szMapName.c_str());
	pMap->LinkEndChild(pText);
	pRoot->LinkEndChild(pMap);

	for(unsigned int i = 0; i < m_pPlayer->GetItems().size(); i++)
	{
		TiXmlElement* pItem = new TiXmlElement( "Item" );
		pItem->SetAttribute("ID", m_pPlayer->GetItems()[i]->GetID());
		pItem->SetAttribute("Type", m_pPlayer->GetItems()[i]->GetType());
		pItem->SetAttribute("Equipped", m_pPlayer->GetItems()[i]->Equipped());

		if(m_pPlayer->GetItems()[i]->HasGem())
		{
			pItem->SetAttribute("Gem", m_pPlayer->GetItems()[i]->GetGem()->GetID());
		}
		else
		{
			pItem->SetAttribute("Gem", -1);
		}

		pRoot->LinkEndChild(pItem);
	}

	doc.LinkEndChild( pRoot );

	doc.SaveFile( pathtowrite.c_str() );

	int slot = -1;
	if( strcmp( save.c_str(), "SaveSlot1.xml" ) == 0 )
		slot = 1;
	else if ( strcmp( save.c_str(), "SaveSlot2.xml" ) == 0)
		slot = 2;
	else
		slot = 3;

	//Save Map Data
	LevelManager::GetInstance()->SaveTemp(slot);
}

//Note
void PlayState::MakeNote(string note)
{
	currState = NOTE;
	this->note = note;
	char_num = 0;
}

bool PlayState::NoteInput( void )
{
	if(this->m_pIM->CheckEnter() || this->m_pIM->CheckMelee() || this->m_pIM->CheckAb1() || this->m_pIM->CheckAb2() || this->m_pIM->CheckAb3())
	{
		//Exit Note
		if( char_num == this->note.size() )
		{
			this->currState = PLAY;

			/////////////////////////////////////////////////////////////////
			// BUG FIX
			// Reference Bug # BB-009
			// BUG FIX START
			/////////////////////////////////////////////////////////////////
			CInputManager::GetInstance()->NoteMelee();
			///////////////////////////////////////////////////////////////////////////
			// BUG FIX END  Reference # BB-009
			//////////////////////////////////////////////////////////////////////////
		}
		else
		{
			char_num = this->note.size();
		}
	}
	return true;
}

void PlayState::NoteUpdate( float deltaTime )
{
	if( char_num <= this->note.size() )
		char_num += (int)(deltaTime * 100);
	if( char_num > this->note.size() )
		char_num = this->note.size();
}

void PlayState::NoteRender( void )
{
	unsigned int cols = 51;

	string text;
	unsigned int rows = note.size() / cols + 1;
	float scaleY = rows*25 / 50.0f;
	CSGD_TextureManager::GetInstance()->Draw(noteImgID,40,30, 2.8f, scaleY,0,0,0,0,D3DCOLOR_ARGB(255,128,0,0));

	for( unsigned int r = 0; r < rows ; r++)
	{
		for( unsigned int i = 0; i < cols && r*cols + i < char_num; i++)
			text.push_back(this->note[r*cols + i]);
		m_pBF->Draw(text, 50, 35 + r*25,0.5f, D3DCOLOR_ARGB(255, 255,255, 255));
		text.clear();
	}

}

bool PlayState::DeathInput( void )
{
	if( this->m_pIM->CheckUp() )
	{
		CAudioManager::GetInstance()->PlaySFX( SFX_CURSOR );
		deathSelect--;
		if( (m_pPlayer->GetCurrency() < 50 && deathSelect == 0 ) || deathSelect < 0)
			deathSelect = EXIT;
	}
	if( this->m_pIM->CheckDown() )
	{
		CAudioManager::GetInstance()->PlaySFX( SFX_CURSOR );
		deathSelect++;

		if( deathSelect > EXIT )
		{
			if( m_pPlayer->GetCurrency() < 50 )
				deathSelect = TOWN;
			else
				deathSelect = CHECK;
		}
	}
	if( this->m_pIM->CheckEnter() )
	{
		CAudioManager::GetInstance()->PlaySFX( SFX_CONFIRM );

		switch( deathSelect )
		{
		case CHECK:
			{
				m_pPlayer->AddCurrency(-50);

				Portal portal;
				portal.port = m_pPlayer->GetPortalID();
				portal.map = LevelManager::GetInstance()->GetLevelName();

				LoadLevelMsg* msg = new LoadLevelMsg(portal);
				CMessageSystem::GetInstace()->SendMsg(msg);
				this->currState = PLAY;
				break;
			}
		case TOWN:
			{
				Portal portal;
				portal.port = 0;
				portal.map = "Demo";

				LoadLevelMsg* msg = new LoadLevelMsg(portal);
				CMessageSystem::GetInstace()->SendMsg(msg);
				this->currState = PLAY;
				break;
			}
		case RELOAD:
			{
				GoBackToMenu();
				CLoadGameState::GetInstance()->SetNewGame(false);
				CGame::GetInstance()->ChangeToNew( CLoadGameState::GetInstance());
				break;
			}
		case EXIT:
			{
				GoBackToMenu();
				break;
			}
		}
	}
	return true;
}

void PlayState::DeathUpdate( float deltaTime )
{

}

void PlayState::DeathRender( void )
{
	m_pBF->Draw("GAME OVER", 230, 70, 1.0f, D3DCOLOR_ARGB(255, 255,0, 0));
	m_pBF->Draw("Your game may be over,", 190, 150, 0.5f, D3DCOLOR_ARGB(255, 255,255, 255));
	m_pBF->Draw("but this doesn't have to be the end.", 190, 200, 0.5f, D3DCOLOR_ARGB(255, 255,255, 255));
	m_pBF->Draw("What will you do?", 190, 250, 0.5f, D3DCOLOR_ARGB(255, 255,255, 255));

	for( int i = CHECK; i <= EXIT; i++)
	{
		int alpha = (i == deathSelect) ? 255 : 128;
		if( m_pPlayer->GetCurrency() < 50 && i == CHECK )
			continue;
		CSGD_TextureManager::GetInstance()->Draw(deathBoxImgID,175,295 + i*60,1.2f,0.238f,0,0,0,0,D3DCOLOR_ARGB(alpha,255,255,255));
	}

	if( m_pPlayer->GetCurrency() >= 50 )
	{
		m_pBF->Draw("Checkpoint ($50)",	250, 305, 0.75f, D3DCOLOR_ARGB(255, 0,255, 255));
	}

	m_pBF->Draw("Respawn in Town",	260, 365, 0.75f, D3DCOLOR_ARGB(255, 0,255, 255));
	m_pBF->Draw("Load Game",		330, 425, 0.75f, D3DCOLOR_ARGB(255, 0,255, 255));
	m_pBF->Draw("Exit",				370, 485, 0.75f, D3DCOLOR_ARGB(255, 0,255, 255));


}

void PlayState::GoToDeath(void)
{ 
	this->currState = DEATH;
	CAudioManager::GetInstance()->StopMus();
	CAudioManager::GetInstance()->PlaySFX(SFX_DEATH);
}

void PlayState::GoBackToMenu(void)
{
	CGame::GetInstance()->ChangeToPrev(2);
	exit = false;
}

void PlayState::LoadLevel(Portal port)
{
	if (m_pPlayer != nullptr)
		m_pPlayer->ClearParticleEffects();

	if( strcmp(port.map.c_str(), "exit") == 0)
	{
		//GoBackToMenu();
		exit = true;
	}
	else
	{
		//Load the new level
		LevelManager::GetInstance()->LoadLevel( port.map );

		if( strcmp( port.map.c_str(), "Snowy") == 0 )
			this->Weather_Snow();
		else if( strcmp( port.map.c_str(), "Mountain") == 0 )
			this->Weather_Rain();
		else
			this->Weather_None();

		Pair Pix = LevelManager::GetInstance()->GetPair();
		Portal other = LevelManager::GetInstance()->GetPortal(port.port);

		this->m_szMapName = port.map;
		this->m_nPortal = port.port;

		this->m_pPlayer->SetPortalID(port.port);

		Pair loc;
		loc.X = other.exit.X * Pix.X + Pix.X / 2;
		loc.Y = other.exit.Y * Pix.Y + Pix.Y;
		PlayState::GetInstance()->MovePlayer(loc.X, loc.Y);

		//Update camera
		Camera::GetInstance()->Center( this->m_pPlayer );
	}
}

void PlayState::LoadingRender()
{
	CSGD_TextureManager::GetInstance()->Draw(this->m_nLoadingBackgroundImgID, 0, 0);

	RECT rRect;
	rRect.top = 0;
	rRect.bottom = 46;
	rRect.left = 0;
	rRect.right = (LONG)(513 / 100 * this->m_nLoaded);

	CSGD_TextureManager::GetInstance()->Draw(this->m_nLoadingBarImgID, 144, 495, 1.0f, 1.0f, &rRect);

	CBitmapFont::GetInstance()->Draw("Loading, please wait...", 155, 500, 0.9f, D3DCOLOR_ARGB(255, 200, 20, 20));
}

void PlayState::HelpMessage( void )
{
	currState = PAUSE; 
	ShowHelpMessage = true; 
	m_nHelpIndex = 0;
	CAudioManager::GetInstance()->PlaySFX(SFX_BACK);
}

void PlayState::LoadingUpdate()
{
	switch(this->m_nLoadingIndex)
	{
	case 0:
		{
			//initialize messgae system
			m_pMS			= CMessageSystem::GetInstace();
			m_pMS->InitMeassageSys(&MessageProc);

			m_pIM			= CInputManager::GetInstance();

			m_pES			= CEventSystem::GetInstance();

			m_pEM			= CEntityManager::GetInstance();
			m_pBF			= CBitmapFont::GetInstance();
			this->m_nLoaded = 5;
			this->m_nLoadingIndex = 1;
		}
		break;
	case 1:
		{
			// Pause Menu
			m_nMenuButtonID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/menubutton2.png")); 
			m_nBoxImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resource/Graphics/Pause.png")); 
			m_nSelectedIndex = 1;
			m_bPaused = false;

			this->m_nLoaded = 10;
			this->m_nLoadingIndex = 2;
		}
		break;
	case 2:
		{
			// Player
			AnimationManager::GetInstance()->Load("playerNoneStandWalk.xml");
			AnimationManager::GetInstance()->Load("playerNoneStabSlashBlock.xml");
			AnimationManager::GetInstance()->Load("playerNoneLungeCastSmash.xml");
			AnimationManager::GetInstance()->Load("playerIceStandWalk.xml");
			AnimationManager::GetInstance()->Load("playerIceStabSlashBlock.xml");
			AnimationManager::GetInstance()->Load("playerIceLungeCastSmash.xml");
			AnimationManager::GetInstance()->Load("playerFireStandWalk.xml");
			AnimationManager::GetInstance()->Load("playerFireStabSlashBlock.xml");
			AnimationManager::GetInstance()->Load("playerFireLungeCastSmash.xml");
			AnimationManager::GetInstance()->Load("playerEarthStandWalk.xml");
			AnimationManager::GetInstance()->Load("playerEarthStabSlashBlock.xml");
			AnimationManager::GetInstance()->Load("playerEarthLungeCastSmash.xml");
			AnimationManager::GetInstance()->Load("playerLightningStandWalk.xml");
			AnimationManager::GetInstance()->Load("playerLightningStabSlashBlock.xml");
			AnimationManager::GetInstance()->Load("playerLightningLungeCastSmash.xml");

			// Abilities
			AnimationManager::GetInstance()->Load("fireball.xml");
			AnimationManager::GetInstance()->Load("explosion.xml");
			AnimationManager::GetInstance()->Load("lightning.xml");
			AnimationManager::GetInstance()->Load("earthshard.xml");
			AnimationManager::GetInstance()->Load("icebolt.xml");
			AnimationManager::GetInstance()->Load("chakram.xml");

			this->m_nLoaded = 25;
			this->m_nLoadingIndex = 3;
		}
		break;
	case 3:
		{
			// Boss
			AnimationManager::GetInstance()->Load("bossIceStandWalk.xml");
			AnimationManager::GetInstance()->Load("bossIceStabSlashBlock.xml");
			AnimationManager::GetInstance()->Load("bossIceLungeCastSmash.xml");
			AnimationManager::GetInstance()->Load("bossFireStandWalk.xml");
			AnimationManager::GetInstance()->Load("bossFireStabSlashBlock.xml");
			AnimationManager::GetInstance()->Load("bossFireLungeCastSmash.xml");
			AnimationManager::GetInstance()->Load("bossEarthStandWalk.xml");
			AnimationManager::GetInstance()->Load("bossEarthStabSlashBlock.xml");
			AnimationManager::GetInstance()->Load("bossEarthLungeCastSmash.xml");
			AnimationManager::GetInstance()->Load("bossLightningStandWalk.xml");
			AnimationManager::GetInstance()->Load("bossLightningStabSlashBlock.xml");
			AnimationManager::GetInstance()->Load("bossLightningLungeCastSmash.xml");

			this->m_nLoaded = 40;
			this->m_nLoadingIndex = 4;
		}
		break;
	case 4:
		{
			// Enemies
			AnimationManager::GetInstance()->Load("wraith_desert.xml");
			AnimationManager::GetInstance()->Load("wraith_snowy.xml");
			AnimationManager::GetInstance()->Load("wraith_mountain.xml");
			AnimationManager::GetInstance()->Load("wraith_forest.xml");
			AnimationManager::GetInstance()->Load("meleeWightDesert.xml");
			AnimationManager::GetInstance()->Load("meleeWightSnowy.xml");
			AnimationManager::GetInstance()->Load("meleeWightMountain.xml");
			AnimationManager::GetInstance()->Load("meleeWightForest.xml");
			AnimationManager::GetInstance()->Load("zombieSnowy.xml");
			AnimationManager::GetInstance()->Load("zombieDesert.xml");
			AnimationManager::GetInstance()->Load("zombieMountain.xml");
			AnimationManager::GetInstance()->Load("zombieForest.xml");
			AnimationManager::GetInstance()->Load("archer_desert.xml");
			AnimationManager::GetInstance()->Load("archer_forest.xml");
			AnimationManager::GetInstance()->Load("archer_snowy.xml");
			AnimationManager::GetInstance()->Load("archer_mountain.xml");

			// Projectile
			AnimationManager::GetInstance()->Load("arrow.xml");

			this->m_nLoaded = 60;
			this->m_nLoadingIndex = 5;
		}
		break;
	case 5:
		{
			// NPCs
			AnimationManager::GetInstance()->Load("npc_jorge.xml");
			AnimationManager::GetInstance()->Load("npc_mark.xml");
			AnimationManager::GetInstance()->Load("npc_daniel.xml");
			AnimationManager::GetInstance()->Load("npc_evan.xml");
			AnimationManager::GetInstance()->Load("npc_bob.xml");
			AnimationManager::GetInstance()->Load("npc_mom.xml");
			AnimationManager::GetInstance()->Load("npc_jim.xml");
			AnimationManager::GetInstance()->Load("npc_vanessa.xml");
			AnimationManager::GetInstance()->Load("npc_witch.xml");

			// Objects
			AnimationManager::GetInstance()->Load("key.xml");
			AnimationManager::GetInstance()->Load("lightning_switch.xml");
			AnimationManager::GetInstance()->Load("doorGrass.xml");
			AnimationManager::GetInstance()->Load("shard.xml");

			// Blank
			AnimationManager::GetInstance()->Load("blank.xml");

			this->m_nLoaded = 75;
			this->m_nLoadingIndex = 6;
		}
		break;
	case 6:
		{
			CItemManager::GetInstance()->Load("shopitems.xml");
			CreatePlayer(0, 0);

			CAudioManager::GetInstance()->StopMus();
			m_pLM			= LevelManager::GetInstance();
			m_pLM->Enter();

			this->m_nLoaded = 90;
			this->m_nLoadingIndex = 7;
		}
		break;
	case 7:
		{
			/////////////////////////////////////////////////////////////////
			// BUG FIX
			// Reference Bug # BB-003
			// BUG FIX START
			/////////////////////////////////////////////////////////////////
			CLoadGameState::GetInstance()->UpdateMap();
			Portal pPortal;
			pPortal.port = m_nPortal;
			pPortal.map = m_szMapName;

			LoadLevel( pPortal );

			///////////////////////////////////////////////////////////////////////////
			// BUG FIX END  Reference # BB-003
			//////////////////////////////////////////////////////////////////////////

			this->m_bLoading = false;
			this->m_nLoadingIndex = -1;

			fadeRate = CAudioManager::GetInstance()->GetNormalVol() * 2;
			CAudioManager::GetInstance()->OtherMusicVolume(-1.0f);
			this->aState = FADEIN;
		}
		break;
	default:
		{
		}
		break;
	}
}

bool PlayState::EndInput(void)
{
	return true;
}

void PlayState::EndUpdate(float deltaTime)
{
	endAlpha -= deltaTime;
	if( endBool )
	{
		currState = PLAY;
		endBool = false;
		endAlpha = 5.0f;
		CAudioManager::GetInstance()->PlayWalk();
	}
	else if( endAlpha <= -3.0 )
	{
		//Go to credits
		endBool = true;
		CGame::GetInstance()->ChangeToNew(CCreditsState::GetInstance());
	}
}

void PlayState::EndRender( void )
{
	int alpha = int((endAlpha / 5.0f) * 255.0f);
	if( alpha < 0 )
		alpha = 0;
	m_pLM->Render(alpha);
	m_pEM->RenderAll();

	string text = "CONGRATULATIONS!";
	m_pBF->Draw(text.c_str(), 50, 500, 1.25f, D3DCOLOR_ARGB(255 - alpha,128,255,128));
}

void PlayState::GoToEnd(void)
{
	this->currState = END;
	this->m_pPlayer->GoToEndAnime();
	CAudioManager::GetInstance()->StopMus();
	//CAudioManager::GetInstance()->PlaySFX(SFX_DEATH);
}

bool PlayState::TransInput(void)
{
	return true;
}

void PlayState::TransUpdate(float deltaTime)
{

	this->transTimer -= deltaTime;
	if( transTimer <= 0 && !transLoaded)
	{
		LoadLevel(this->transPort);
		transLoaded = true;
	}
	if( transTimer <= -0.5f )
	{
		SavePlayer();
		this->currState = PLAY;
		transLoaded = false;
		transTimer = 0.0f;
	}
	AudioUpdate(deltaTime);

}

void PlayState::TransRender(void)
{
	float transAlpha = ( transTimer >= 0 ) ? transTimer * 2.0f : -transTimer * 2.0f;
	int asdf = int(transAlpha * 255.0f);

	m_pLM->Render(asdf);
	m_pEM->RenderPlayer();

	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-058
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	// Renders doors, as they are an entity, fading
	m_pEM->RenderDoor(asdf);

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-058
	//////////////////////////////////////////////////////////////////////////
}

void PlayState::GoToTrans(void)
{
	this->currState = TRANS;
	this->transTimer = 0.5f;
	agro = true;
	aState = FADEOUT;
	fadeRate = CAudioManager::GetInstance()->GetNormalVol() * 2;
}

void PlayState::AudioUpdate(float fElapsedTime)
{
	CAudioManager* am = CAudioManager::GetInstance();
	if( agro != CEntityManager::GetInstance()->Aggroed() )
	{
		//Fade out
		aState = FADEOUT;
		agro = CEntityManager::GetInstance()->Aggroed();
		fadeRate = am->GetNormalVol() * 2;

	}
	switch( aState )
	{
	case FADEOUT:
		{
			am->OtherMusicVolume( -fElapsedTime* fadeRate );
			float vol = am->GetMusicVolume();
			if( vol == 0.0f ) //When silent change song and go back up
			{
				am->StopMus();
				if( agro )
					am->PlayBattle();
				else
					am->PlayWalk();
				aState = FADEIN;
			}
			break;
		}
	case FADEIN:
		{
			am->OtherMusicVolume( fElapsedTime * fadeRate);
			float vol = am->GetMusicVolume();
			float max = am->GetNormalVol();
			if( vol >= max ) //When at full change go back to normal
			{
				/////////////////////////////////////////////////////////////////
				// BUG FIX
				// Reference Bug # BB-041
				// BUG FIX START
				/////////////////////////////////////////////////////////////////

				//am->AdjustMusicVolume( max );
				am->AdjustMusicVolume( max-vol );

				/////////////////////////////////////////////////////////////////
				// BUG FIX END  Reference # BB-041
				/////////////////////////////////////////////////////////////////
				aState = REGULAR;
			}
			break;
		}
	}
}