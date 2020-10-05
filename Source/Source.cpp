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


//#define EPSILON		0.0001f
//#define PI			3.14159265358f
int running = 1;

int WINAPI WinMain(HINSTANCE currentInstance, HINSTANCE previousInstance, PSTR cmdLine, INT cmdCount) {

	UNREFERENCED_PARAMETER(currentInstance);
	UNREFERENCED_PARAMETER(previousInstance);
	UNREFERENCED_PARAMETER(cmdLine);
	UNREFERENCED_PARAMETER(cmdCount);

	// Checking for memory leaks
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	CreateDebugWindow();
#endif

	CoreEngine* engine = new CoreEngine();

	//to reorder based on what system has priority over the other
	engine->AddSystem<WindowsSystem>();
	engine->AddSystem<GraphicsSystem>();
	engine->AddSystem<LightingSystem>();
	engine->AddSystem<InputSystem>();
	engine->AddSystem<Physics>();
	engine->AddSystem<Collision>();
	engine->AddSystem<EntityFactory>();
	engine->AddSystem<SoundSystem>();
	engine->AddSystem<Game>();

	engine->AddManager<ModelManager>();
	engine->AddManager<TextureManager>();
	engine->AddManager<ShaderManager>();
	engine->AddManager<AnimationManager>();

	//Physics* test_phy = engine->GetSystem<Physics>();

	engine->Initialize();

	engine->GameLoop();

	FACTORY->DestroyAllEntities();

	engine->DestroySystems();

	engine->DestroyManagers();

	delete engine;
	EngineDebug::DeleteInstance();

	return 0;
}