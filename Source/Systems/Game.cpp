#include <iostream>
#include "Systems/Game.h"
#include "GameStates/GameState.h"
#include "Systems/InputSystem.h"
#include "Engine/Core.h"
#include "GameStates/SplashState.h"
#include "GameStates/MenuState.h"
#include "GameStates/PlayState.h"
#include "Systems/Debug.h"
#include <algorithm>

bool Game::CheckExist(GameState* compare) {
	if (!states_.empty() && states_.back() == compare) {
		std::cout << "Error: State already exists on stack" << std::endl;
		return true;
	}
	return false;
}

Game::Game() :
	debug_{ false }
{}

void Game::Init()
{
	b_running_ = true;

	//CORE->GetManager<TextureManager>()->TempTextureBatchLoad();
	//CORE->GetManager<AnimationManager>()->TempAnimationBatchLoad();
	FACTORY->CreateAllArchetypes("Resources/EntityConfig/archetypes.json");

	ComponentManager* comp_mgr = &*CORE->GetManager<ComponentManager>();

	input_controller_arr_ = comp_mgr->GetComponentArray<InputController>();
	status_arr_ = comp_mgr->GetComponentArray<Status>();
	basicai_arr_ = comp_mgr->GetComponentArray<BasicAI>();

	files_to_load_ = LoadAllTextureJson();

	//CORE->GetManager<TextureManager>()->TextureBatchLoad("Menu");
	CORE->GetManager<AnimationManager>()->AnimationBatchLoad("Menu");
	CORE->GetManager<FontManager>()->FontBatchLoad("Menu");

	//CORE->GetManager<TextureManager>()->TextureBatchLoad("Play");
	CORE->GetManager<AnimationManager>()->AnimationBatchLoad("Play");
	CORE->GetManager<FontManager>()->FontBatchLoad("Play");

	//CORE->GetManager<TextureManager>()->TextureBatchLoad("UIProps");

	// to load all the files for textures
	for (int i = 0; i < files_to_load_.size(); ++i)
		CORE->GetManager<TextureManager>()->TextureBatchLoad(files_to_load_[i]);

	ChangeState(&m_SplashState);

	M_DEBUG->WriteDebugMessage("Game System Init\n");
}

// takes a pointer to a gamestate
void Game::ChangeState(GameState* state, std::string level_name)
{
	// Remove current state
	if (!states_.empty()) {
	
		states_.back()->Free();
		states_.pop_back();
	}

	states_.push_back(state);
	states_.back()->Init(level_name);
}

// temporarily pushes a new state onto the stack
void Game::PushState(GameState* state) // need to check if current state already exists in stack
{
	(void)state;
}

void Game::PopState()
{
	// cleanup the current state
	if (!states_.empty())
	{
		states_.back()->Free();
		states_.pop_back();
	}
}

void Game::Update(float frametime)
{

	// let the current state take control
	if (!states_.empty() && !CORE->GetCorePauseStatus() && !CORE->GetGamePauseStatus()) {
	
		states_.back()->Update(this, frametime);
	}
}

void Game::Draw()
{
	// let the current state take control
	states_.back()->Draw(this);
}

void Game::Free()
{
	while (!states_.empty())
	{
		states_.back()->Free();
		states_.pop_back();
	}

	if (!files_to_load_.empty())
		files_to_load_.clear();
}

std::string Game::GetStateName() {

	if (!states_.empty())
		return states_.back()->GetStateName();

	// Do check the return string
	return {};
}

void Game::SendMessageD(Message* m) {

	//assume all messages of concern to game are only input message
	switch (m->message_id_) {

	case MessageIDTypes::DEBUG_ALL: 
	{

		debug_ = !debug_;
		states_.back()->StateInputHandler(m, this);
		break;
	}
	case MessageIDTypes::M_BUTTON_PRESS:
	case MessageIDTypes::M_BUTTON_TRIGGERED:
	{

		//forward onto state to handle
		states_.back()->StateInputHandler(m, this);
		break;
	}
	case MessageIDTypes::BUTTON:
	{
		states_.back()->StateInputHandler(m, this);
		break;
	}
	case MessageIDTypes::GSM_LOSE:
	case MessageIDTypes::GSM_WIN:
	case MessageIDTypes::C_MOVEMENT:
	{
		//value larger than what can be gotten from input flags
		states_.back()->StateInputHandler(m);
		break;
	}
	}
}

std::vector<std::string> Game::LoadAllTextureJson() {

	std::string filename = {};
	std::string file = {};
	std::vector<std::string> tempfiles = {};
	for (auto& texjson : fs::directory_iterator("Resources/AssetsLoading")) {

		if (fs::is_regular_file(texjson) && texjson.path().extension() == ".json" && texjson.path().filename().generic_string().find("texture") != texjson.path().filename().generic_string().npos) {

			filename = texjson.path().filename().generic_string().c_str();

			if (filename.find("_") != filename.npos)
				file = filename.substr(0, filename.find("_"));

			tempfiles.push_back(file);
		}
	}

	return tempfiles;
}
