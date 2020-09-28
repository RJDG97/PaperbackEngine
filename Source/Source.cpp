#include "Source.h"

#include "Systems/WindowsSystem.h"
#include <memory>
#include "MathLib/Vector2D.h"
#include "MathLib/Matrix3x3.h"

#include "Systems/Game.h"
#include "GameStates/MenuState.h"
#include "Systems/GraphicsSystem.h"
#include "Systems/LightingSystem.h"

#include "glhelper.h"

#include "Manager/ModelManager.h"
#include "Manager/TextureManager.h"
#include "Manager/ShaderManager.h"
#include "Manager/AnimationManager.h"

/*
temp headers
*/
#include "Engine/Core.h"
#include "Systems/Physics.h"
#include "Systems/Factory.h"
#include "Systems/Game.h"
#include "Systems/Collision.h"


#define EPSILON		0.0001f
#define PI			3.14159265358f
int running = 1;

void testMath();

int WINAPI WinMain(HINSTANCE currentInstance, HINSTANCE previousInstance, PSTR cmdLine, INT cmdCount) {

	//Game game;

	UNREFERENCED_PARAMETER(previousInstance);
	UNREFERENCED_PARAMETER(cmdLine);

	// Register the window class
	//WindowsSystem::Instance()->Init(currentInstance, "GAM200 MonkeyTypewriters", 800, 600);

	// Checking for memory leaks
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	createDebugWindow();
#endif

	// Set up OpenGL context
	//graphicsSystem.OpenGLInit();

	//GLHelper::Instance()->init(800, 600, "Placeholder");
	//GraphicsSystem::Instance().Init();

	// // Create a sound class
	// SoundSystem EngineSoundSystem;

	// EngineSoundSystem.loadSound("Resources/SoundCache/KK_BBG.mp3", "BGM", 1);
	// EngineSoundSystem.loadSound("Resources/SoundCache/Kachow.mp3", "Kachow");
	// EngineSoundSystem.playSound("BGM");


	// Display Window
	//ShowWindow(WindowsSystem::Instance()->getHandle(), cmdCount);
	//UpdateWindow(WindowsSystem::Instance()->getHandle());

	//game.init();
	// Push the menu state onto the stack
	//game.ChangeState(MenuState::Instance());

	//while (game.Running())
	//{
	//	// FrameRateController
	//	PE_FrameRate.FrameRateLoop();

	//	// Process windows messages
	//	//WindowsSystem::Instance()->ProcessMessage();

	//	/********Math Test*********/

	//	if (sys_input.CheckTriggeredInput(PE_K))
	//		testMath();

	//	/********Math Test*********/

	//	/********Input Test********/

	//	static int forward = PE_W;

	//	if (sys_input.CheckTriggeredInput(PE_S))
	//		std::cout << "Pressed" << std::endl;

	//	if (sys_input.CheckCurrentInput(PE_S))
	//		std::cout << "S" << std::endl;

	//	if (sys_input.CheckReleasedInput(PE_S))
	//		std::cout << "Released" << std::endl;

	//	if (sys_input.CheckTriggeredInput(forward))
	//		std::cout << "Forward" << std::endl;
	//	std::cin >> forward;

	//	sys_input.UpdateKeyInput(PE_E, forward);

	//	if (sys_input.CheckTriggeredInput(PE_LBUTTON)) {
	//		Vector2D vec;
	//		sys_input.GetCursorPosition(vec);
	//	}

	//	if (sys_input.CheckCurrentInput(PE_RBUTTON)) {
	//		Vector2D vec2;
	//		sys_input.GetCursorPositionDelta(vec2, PE_RBUTTON);
	//	}

	//	/********Input Test********/

	//	/********Sound Test********/

	//	if (sys_input.CheckTriggeredInput(PE_5)) {
	//		EngineSoundSystem.playSound("Kachow");
	//	}

	//	if (sys_input.CheckTriggeredInput(PE_6)) {
	//		EngineSoundSystem.stopSound("BGM");
	//	}

	//	if (sys_input.CheckTriggeredInput(PE_7)) {
	//		EngineSoundSystem.loadSound("Resources/SoundCache/KK_BBG.mp3", "BGM");
	//		EngineSoundSystem.playSound("BGM");
	//	}

	//	if (sys_input.CheckTriggeredInput(PE_8)) {
	//		EngineSoundSystem.muteSound();
	//	}

	//	if (sys_input.CheckTriggeredInput(PE_9)) {
	//		EngineSoundSystem.pauseSound();
	//	}

	//	EngineSoundSystem.updateSound();

	//	/********Sound Test********/

	//	/********Frame Test********/

	//	if (sys_input.CheckTriggeredInput(PE_TAB))
	//	{
	//		std::cout << PE_FrameRate.GetFrames() << std::endl;
	//		std::cout << PE_FrameRate.Dt << std::endl;
	//		std::cout << PE_FrameRate.TimeElapsed(s) << std::endl;
	//	}

	//	// Change FPS
	//	if (sys_input.CheckTriggeredInput(PE_1))
	//		PE_FrameRate.SetFPS(90);
	//	if (sys_input.CheckTriggeredInput(PE_2))
	//		PE_FrameRate.SetFPS(60);

	//	/********Frame Test********/

	//	/*** State/Scene Manager ***/

	//	game.update();
	//	game.draw();

	//	/*** State/Scene Manager ***/

	//}

	//game.free();

	//WindowsSystem::Instance()->UnloadInstance();
	//return 0;
	
	std::cout << "Testing" << std::endl;
	CoreEngine* engine = new CoreEngine();

	//to reorder based on what system has priority over the other
	engine->AddSystem(new WindowsSystem());
	//engine->AddSystem(new LightingSystem());
	engine->AddSystem(new GraphicsSystem());
	engine->AddSystem(new InputSystem());
	engine->AddSystem(new Physics());
	engine->AddSystem(new Collision());
	engine->AddSystem(new EntityFactory());
	engine->AddSystem(new SoundSystem());
	engine->AddSystem(new Game());

	//just for testing!!
	ModelManager model_manager;
	TextureManager texture_manager;
	ShaderManager shader_manager;
	AnimationManager animation_manager;
	texture_manager.Init();

	engine->Initialize();

	engine->GameLoop();

	FACTORY->DestroyAllEntities();

	engine->DestroySystems();

	delete engine;

	//WindowsSystem::Instance()->UnloadInstance();

	return 0;
}


/*************************************************************************/
/*                            Testing Math Fn's                          */
/*************************************************************************/

void PrintVector(char* txt, const Vec2& pVec0)
{
	printf("%s:\t%f, %f\n", txt, pVec0.x, pVec0.y);
}

void PrintMtx33(char* pString, Mtx33& pMtx)
{
	printf("%s:\n", pString);
	printf("%8.5f %8.5f %8.5f\n", pMtx.m00, pMtx.m01, pMtx.m02);
	printf("%8.5f %8.5f %8.5f\n", pMtx.m10, pMtx.m11, pMtx.m12);
	printf("%8.5f %8.5f %8.5f\n", pMtx.m20, pMtx.m21, pMtx.m22);
}

float CompareMtx33(const Mtx33& pSrc, const Mtx33& pDst)
{
	float d = 0.0f;

	for (long i = 0; i < 9; i++)
		d += fabs(pSrc.m[i] - pDst.m[i]);

	return d;
}

float CompareVec2(const Vec2& pSrc, const Vec2& pDst)
{
	float d = 0.0f;

	d += fabs(pSrc.x - pDst.x);
	d += fabs(pSrc.y - pDst.y);

	return d;
};

void testMath() {
	// Testing Vector2D
//--------------------------------------------------------------------------
	Vec2 v1, v2, v3, result;

	printf("Testing Vector2D:\n");
	printf("-----------------------------\n");

	Vec2 v4(1.0f, 2.0f);
	result.x = 1.0f;	result.y = 2.0f;
	printf("Vec2Set: \t\t%s\n",
		(CompareVec2(result, v4) < EPSILON) ? "Pass" : "Fail");

	// Vector2D Negate
	//--------------------------------------------------------------------------
	v1.x = 2.0f; v1.y = -4.0f;
	v2 = -v1;
	result.x = -2.0f;	result.y = 4.0f;
	printf("Vec2Neg: \t\t%s\n",
		(CompareVec2(result, v2) < EPSILON) ? "Pass" : "Fail");

	// Vector2D Add
	//--------------------------------------------------------------------------
	v1.x = 2.0f; v1.y = -4.0f;
	v2.x = 1.0f; v2.y = 7.0f;
	v3 = v1 + v2;
	result.x = result.y = 3.0f;
	printf("Vec2Add 1: \t\t%s\n",
		(CompareVec2(result, v3) < EPSILON) ? "Pass" : "Fail");

	v1.x = 2.0f; v1.y = -4.0f;
	v2.x = 1.0f; v2.y = 7.0f;
	v1 += v2;
	result.x = result.y = 3.0f;
	printf("Vec2Add 2: \t\t%s\n",
		(CompareVec2(result, v1) < EPSILON) ? "Pass" : "Fail");

	// Vector2D Subtract
	//--------------------------------------------------------------------------
	v1.x = 2.0f; v1.y = -4.0f;
	v2.x = 1.0f; v2.y = 7.0f;
	v3 = v1 - v2;
	result.x = 1.0f;	result.y = -11.0f;
	printf("Vec2Sub 1: \t\t%s\n",
		(CompareVec2(result, v3) < EPSILON) ? "Pass" : "Fail");

	v1.x = 2.0f; v1.y = -4.0f;
	v2.x = 1.0f; v2.y = 7.0f;
	v1 -= v2;
	result.x = 1.0f;	result.y = -11.0f;
	printf("Vec2Sub 2: \t\t%s\n",
		(CompareVec2(result, v1) < EPSILON) ? "Pass" : "Fail");

	// Vector2D Normalize
	//--------------------------------------------------------------------------
	v1.x = 3.0f; v1.y = 4.0f;
	Vector2DNormalize(v2, v1);
	result.x = 0.6f;	result.y = 0.8f;
	printf("Vec2Normalize: \t\t%s\n",
		(CompareVec2(result, v2) < EPSILON) ? "Pass" : "Fail");

	// Vector2D Scale
	//--------------------------------------------------------------------------
	v1.x = 2.0f; v1.y = -5.0f;
	v2 = v1 * 3.0f;
	result.x = 6.0f;	result.y = -15.0f;
	printf("Vec2Scale 1: \t\t%s\n",
		(CompareVec2(result, v2) < EPSILON) ? "Pass" : "Fail");

	v1.x = 2.0f; v1.y = -5.0f;
	v2 = 3.0f * v1;
	result.x = 6.0f;	result.y = -15.0f;
	printf("Vec2Scale 2: \t\t%s\n",
		(CompareVec2(result, v2) < EPSILON) ? "Pass" : "Fail");

	v1.x = 2.0f; v1.y = -5.0f;
	v1 *= 3.0f;
	result.x = 6.0f;	result.y = -15.0f;
	printf("Vec2Scale 3: \t\t%s\n",
		(CompareVec2(result, v1) < EPSILON) ? "Pass" : "Fail");

	v1.x = 6.0f;	v1.y = -15.0f;
	v2 = v1 / 3.0f;
	result.x = 2.0f; result.y = -5.0f;
	printf("Vec2Scale 4: \t\t%s\n",
		(CompareVec2(result, v2) < EPSILON) ? "Pass" : "Fail");

	v1.x = 6.0f;	v1.y = -15.0f;
	v1 /= 3.0f;
	result.x = 2.0f; result.y = -5.0f;
	printf("Vec2Scale 5: \t\t%s\n",
		(CompareVec2(result, v1) < EPSILON) ? "Pass" : "Fail");

	// Vector2D Length
	//--------------------------------------------------------------------------
	v1.x = 3.0f; v1.y = -4.0f;
	printf("Vec2Length: \t\t%s\n",
		(fabs(Vector2DLength(v1) - 5.0f) < EPSILON) ? "Pass" : "Fail");

	// Vector2D Square Length
	//--------------------------------------------------------------------------
	v1.x = 3.0f; v1.y = -4.0f;
	printf("Vec2SquareLength: \t%s\n",
		(fabs(Vector2DSquareLength(v1) - 25.0f) < EPSILON) ? "Pass" : "Fail");

	// Vector2D Distance
	//--------------------------------------------------------------------------
	v1.x = 2.0f;	v1.y = 3.0f;
	v2.x = 4.0f;	v2.y = -1.0f;
	printf("Vec2Distance: \t\t%s\n",
		(fabs(Vector2DDistance(v1, v2) - 4.472136) < EPSILON) ? "Pass" : "Fail");

	// Vector2D Square Distance
	//--------------------------------------------------------------------------
	v1.x = 2.0f;	v1.y = 3.0f;
	v2.x = 4.0f;	v2.y = -1.0f;
	printf("Vec2SquareDistance: \t%s\n",
		(fabs(Vector2DSquareDistance(v1, v2) - 20.0f) < EPSILON) ? "Pass" : "Fail");

	// Vector2D Dot Product
	//--------------------------------------------------------------------------
	v1.x = 3.0f;	v1.y = 2.0f;
	v2.x = 4.0f;	v2.y = -6.0f;
	printf("Vec2DotProduct: \t%s\n",
		(fabs(Vector2DDotProduct(v1, v2)) < EPSILON) ? "Pass" : "Fail");

	// Vector2D Cross Product Magnitude
	//--------------------------------------------------------------------------
	v1.x = 3.0f;	v1.y = 2.0f;
	v2.x = 4.0f;	v2.y = -6.0f;
	printf("Vec2CrossProductMag: \t%s\n",
		(fabs(Vector2DCrossProductMag(v1, v2) + 26.0f) < EPSILON) ? "Pass" : "Fail");



	// Testing Matrix3x3
	//--------------------------------------------------------------------------
	Mtx33 id, m0, m1;
	Vec2  u;
	float d, x, y, * p;
	long  n;

	printf("\nTesting Matrix3x3:\n");
	printf("-----------------------------\n");

	// create an id matrix for reference
	memset(id.m, 0, 9 * sizeof(float));
	id.m[0] = id.m[4] = id.m[8] = 1.0f;

	// Converion constructors
	//--------------------------------------------------------------------------
	// generate an array of random numbers
	p = new float[9];
	for (int i = 0; i < 9; ++i)
		p[i] = 2.0f * rand() / (float)(RAND_MAX)-1.0f;

	memcpy(m0.m, p, 9 * sizeof(float));
	Mtx33 m2(p);
	printf("Mtx33Constructor 1: \t%s\n",
		(CompareMtx33(m0, m2) < EPSILON) ? "Pass" : "Fail");

	Mtx33 m3(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8]);
	printf("Mtx33Constructor 2: \t%s\n",
		(CompareMtx33(m0, m3) < EPSILON) ? "Pass" : "Fail");

	delete[]p;

	// Identity Matrix
	//--------------------------------------------------------------------------
	Mtx33Identity(m0);
	d = CompareMtx33(id, m0);
	printf("Mtx33Identity: \t\t%s\n",
		(CompareMtx33(id, m0) < EPSILON) ? "Pass" : "Fail");

	// Translate Matrix
	//--------------------------------------------------------------------------
	// generate 2 random numbers
	x = 2.0f * rand() / (float)(RAND_MAX)-1.0f;
	y = 2.0f * rand() / (float)(RAND_MAX)-1.0f;

	Mtx33Translate(m0, x, y);
	m0.m02 -= x;
	m0.m12 -= y;
	printf("Mtx33Translate: \t%s\n",
		(CompareMtx33(id, m0) < EPSILON) ? "Pass" : "Fail");

	// Scale Matrix
	//--------------------------------------------------------------------------
	// generate 2 random numbers
	x = 2.0f * rand() / (float)(RAND_MAX)-1.0f;
	y = 2.0f * rand() / (float)(RAND_MAX)-1.0f;

	Mtx33Scale(m0, x, y);
	m0.m00 /= x;
	m0.m11 /= y;

	printf("Mtx33Scale: \t\t%s\n",
		(CompareMtx33(id, m0) < EPSILON) ? "Pass" : "Fail");

	// Concatenate Matrix
	//--------------------------------------------------------------------------
	// generate 2 random numbers
	x = 2.0f * rand() / (float)(RAND_MAX)-1.0f;
	y = 2.0f * rand() / (float)(RAND_MAX)-1.0f;

	Mtx33Translate(m0, x, y);
	Mtx33Scale(m1, x, y);
	m0 = m0 * m1;
	m0.m02 -= x;
	m0.m12 -= y;
	m0.m00 /= x;
	m0.m11 /= y;

	printf("Mtx33Concat 1: \t\t%s\n",
		(CompareMtx33(id, m0) < EPSILON) ? "Pass" : "Fail");

	// generate 2 random numbers
	x = 2.0f * rand() / (float)(RAND_MAX)-1.0f;
	y = 2.0f * rand() / (float)(RAND_MAX)-1.0f;

	Mtx33Translate(m0, x, y);
	Mtx33Scale(m1, x, y);
	m0 = m1 * m0;
	m0.m02 -= x * x;
	m0.m12 -= y * y;
	m0.m00 /= x;
	m0.m11 /= y;

	printf("Mtx33Concat 2: \t\t%s\n",
		(CompareMtx33(id, m0) < EPSILON) ? "Pass" : "Fail");

	// generate 2 random numbers
	x = 2.0f * rand() / (float)(RAND_MAX)-1.0f;
	y = 2.0f * rand() / (float)(RAND_MAX)-1.0f;

	Mtx33Translate(m0, x, y);
	Mtx33Scale(m1, x, y);
	m0 *= m1;
	m0.m02 -= x;
	m0.m12 -= y;
	m0.m00 /= x;
	m0.m11 /= y;

	printf("Mtx33Concat 3: \t\t%s\n",
		(CompareMtx33(id, m0) < EPSILON) ? "Pass" : "Fail");

	// Rotation Matrix in radians
	//--------------------------------------------------------------------------
	n = (rand() % 16) + 15;
	Mtx33Identity(m0);
	Mtx33RotRad(m1, 2.0f * PI / n);

	for (long i = 0; i < n; i++)
		m0 = m1 * m0;

	printf("Mtx33RotRad: \t\t%s\n",
		(CompareMtx33(id, m0) < EPSILON) ? "Pass" : "Fail");

	// Rotation Matrix in degrees
	//--------------------------------------------------------------------------
	n = (rand() % 16) + 15;
	Mtx33Identity(m0);
	Mtx33RotDeg(m1, 360.0f / n);

	for (long i = 0; i < n; i++)
		m0 = m1 * m0;

	printf("Mtx33RotDeg: \t\t%s\n",
		(CompareMtx33(id, m0) < EPSILON) ? "Pass" : "Fail");

	// Transpose Matrix
	//--------------------------------------------------------------------------

	Mtx33RotRad(m0, rand() / (float)(RAND_MAX) * 2.0f * PI);
	Mtx33Transpose(m1, m0);
	m0 = m1 * m0;

	printf("Mtx33Transpose: \t%s\n",
		(CompareMtx33(id, m0) < EPSILON) ? "Pass" : "Fail");

	// Vector Multiplication Matrix
	//--------------------------------------------------------------------------
	// generate 2 random numbers
	x = 2.0f * rand() / (float)(RAND_MAX)-1.0f;
	y = 2.0f * rand() / (float)(RAND_MAX)-1.0f;

	n = (rand() % 16) + 15;
	u.x = x;
	u.y = y;
	Mtx33RotRad(m0, 2.0f * PI / n);

	for (long i = 0; i < n; i++)
		u = m0 * u;

	printf("Mtx33MultVec 1: \t%s\n",
		((fabs(u.x - x) + fabs(u.y - y)) < EPSILON) ? "Pass" : "Fail");

	// generate 2 random numbers
	x = 2.0f * rand() / (float)(RAND_MAX)-1.0f;
	y = 2.0f * rand() / (float)(RAND_MAX)-1.0f;

	n = (rand() % 16) + 15;
	u.x = x;
	u.y = y;
	Mtx33Translate(m0, x, y);

	for (long i = 1; i < n; i++)
		u = m0 * u;

	printf("Mtx33MultVec 2: \t%s\n",
		((fabs(u.x - x * n) + fabs(u.y - y * n)) < EPSILON) ? "Pass" : "Fail");

	// Inverse Matrix
	//--------------------------------------------------------------------------
	Mtx33 mInv;
	float det = 0.0f;
	bool quit = false;

	// loop until we find an invertible matrix
	while (!quit)
	{
		x = 2.0f * rand() / (float)(RAND_MAX)-1.0f;
		y = 2.0f * rand() / (float)(RAND_MAX)-1.0f;
		Mtx33Translate(m0, x, y);

		n = (rand() % 16) + 15;
		Mtx33RotRad(m1, 2.0f * PI / n);

		Mtx33Scale(m2, x, y);

		m3 = m0 * m1 * m2;

		Mtx33Inverse(&mInv, &det, m3);

		if (det <= EPSILON)
			continue;

		Mtx33Inverse(&mInv, &det, mInv);

		if (det <= EPSILON)
			continue;

		printf("Mtx33Inverse: \t\t%s\n",
			(CompareMtx33(mInv, m3) < EPSILON) ? "Pass" : "Fail");
		quit = true;
	}
}
