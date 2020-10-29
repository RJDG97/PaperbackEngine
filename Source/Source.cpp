#include "Source.h"

#include "Systems/WindowsSystem.h"
#include <memory>
#include "MathLib/Vector2D.h"
#include "MathLib/Matrix3x3.h"

#include "Systems/Game.h"
#include "GameStates/MenuState.h"
#include "Systems/GraphicsSystem.h"
#include "Systems/LightingSystem.h"
#include "Systems/ImguiSystem.h"
#include "Systems/LogicSystem.h"

#include "Manager/ModelManager.h"
#include "Manager/TextureManager.h"
#include "Manager/ShaderManager.h"
#include "Manager/AnimationManager.h"
#include "Manager/ForcesManager.h"
#include "Manager/AIManager.h"

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
	CORE = std::make_unique<CoreEngine>();

	// Add Systems to the Core Engine
	// *Note: Reorder based on what system 
	// has priority over the other

	try {
		CORE->AddSystem<WindowsSystem>();
		CORE->AddSystem<CameraSystem>();
		CORE->AddSystem<GraphicsSystem>();
		CORE->AddSystem<LightingSystem>();
		CORE->AddSystem<ImguiSystem>();
		CORE->AddSystem<InputSystem>();
		CORE->AddSystem<Physics>();
		CORE->AddSystem<Collision>();
		CORE->AddSystem<EntityFactory>();
		CORE->AddSystem<SoundSystem>();
		CORE->AddSystem<Game>();

		CORE->AddSystem<LogicSystem>();

		// Add Managers to the Core Engine
		CORE->AddManager<ModelManager>();
		CORE->AddManager<TextureManager>();
		CORE->AddManager<ShaderManager>();
		CORE->AddManager<FontManager>();
		CORE->AddManager<AnimationManager>();
		CORE->AddManager<ForcesManager>();
		CORE->AddManager<AIManager>();

		// Initialize all Systems & Managers that
		// were added to the Core Engine
		CORE->Initialize();

		// ** Core Engine's Game Loop **
		CORE->GameLoop();

		// Release all resources allocated during
		// runtime and compile time
		FACTORY->DestroyAllEntities();
		FACTORY->DestroyAllArchetypes();
		EngineDebug::DeleteInstance();
	}
	catch (std::exception& e) {

		DEBUG_ASSERT(false, e.what());
	}
	catch (...) {

		DEBUG_ASSERT(false, "Game has crashed, unhandled exception");
	}

	return 0;
}