#include "Source.h"

#include "Systems/WindowsSystem.h"
#include <memory>
#include "MathLib/Vector2D.h"
#include "MathLib/Matrix3x3.h"

#include "Systems/Game.h"
#include "GameStates/MenuState.h"
#include "Systems/GraphicsSystem.h"
#include "Systems/LightingSystem.h"

#include "Manager/ModelManager.h"
#include "Manager/TextureManager.h"
#include "Manager/ShaderManager.h"
#include "Manager/AnimationManager.h"

#include "Engine/Core.h"
#include "Systems/Physics.h"
#include "Systems/Factory.h"
#include "Systems/Game.h"
#include "Systems/Collision.h"
#include "Systems/Debug.h"
#include <sstream>

int WINAPI WinMain(HINSTANCE currentInstance, HINSTANCE previousInstance, PSTR cmdLine, INT cmdCount) {

	UNREFERENCED_PARAMETER(currentInstance);
	UNREFERENCED_PARAMETER(previousInstance);
	UNREFERENCED_PARAMETER(cmdLine);
	UNREFERENCED_PARAMETER(cmdCount);

	// Checking for memory leaks
	// Creating console window
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	CreateDebugWindow();
#endif

	// Create a unique instance of Core Engine
	std::unique_ptr<CoreEngine> engine = std::make_unique<CoreEngine>();

	// Add Systems to the Core Engine
	// *Note: Reorder based on what system 
	// has priority over the other
	engine->AddSystem<WindowsSystem>();
	engine->AddSystem<GraphicsSystem>();
	engine->AddSystem<LightingSystem>();
	engine->AddSystem<InputSystem>();
	engine->AddSystem<Physics>();
	engine->AddSystem<Collision>();
	engine->AddSystem<EntityFactory>();
	engine->AddSystem<SoundSystem>();
	engine->AddSystem<Game>();

	// Add Managers to the Core Engine
	engine->AddManager<ModelManager>();
	engine->AddManager<TextureManager>();
	engine->AddManager<ShaderManager>();
	engine->AddManager<AnimationManager>();

	// Initialize all Systems & Managers that
	// were added to the Core Engine
	engine->Initialize();

	// ** Core Engine's Game Loop **
	engine->GameLoop();

	// Release all resources allocated during
	// runtime and compile time
	FACTORY->DestroyAllEntities();
	engine->DestroySystems();
	engine->DestroyManagers();
	EngineDebug::DeleteInstance();

	return 0;
}