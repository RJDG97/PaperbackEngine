#include "GameStates/SplashState.h"
#include "GameStates/MenuState.h"
#include "Manager/TransitionManager.h"
#include "Engine/Core.h"
#include "Systems/Factory.h"

SplashState m_SplashState;

SplashState::SplashState()
{ }


void SplashState::Init(std::string) {
	
	component_mgr_ = &*CORE->GetManager<ComponentManager>();
	entity_mgr_ = &*CORE->GetManager<EntityManager>();

	CORE->ResetGodMode();
	CORE->ResetCorePauseStatus();
	CORE->ResetGamePauseStatus();

	FACTORY->LoadLevel("Splash");
	CORE->GetManager<TransitionManager>()->ResetTransition("Splash", &m_MenuState);

	MessageBGM_Play msg{ "MenuDefault" };
	CORE->BroadcastMessage(&msg);
}


void SplashState::Free() {

	CORE->ResetGodMode();
	CORE->ResetCorePauseStatus();
	CORE->ResetGamePauseStatus();

	FACTORY->DestroyAllEntities();
}


void SplashState::Update(Game* game, float frametime) {
	
	(void)game;
	(void)frametime;
}


void SplashState::Draw(Game* game) {
	
	(void)game;
}


void SplashState::StateInputHandler(Message* msg, Game* game) {
	
	(void)msg;
	(void)game;
}