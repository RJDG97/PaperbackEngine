#ifndef FrameRateController_H
#define FrameRateController_H

#include <chrono>

enum TimeUnit
{
	ms = 0,
	s,
};

class Time_Channel
{
protected:

	// Time variables
	std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::high_resolution_clock::now();
	std::chrono::time_point<std::chrono::steady_clock> end;
	// Time Passed
	std::chrono::duration<float> timeelapsed = start - start;
	// Duration of frame (DeltaTime)
	std::chrono::duration<float> frametime;
	// Whether Timer is running
	bool running = false;

public:

	// Timer update loop
	void TimerUpdate();

	// Start Timer
	void TimerStart();

	// Pause Timer
	void TimerStop();

	// Reset Timer
	void TimerReset();

	// Get Time elapsed
	float TimeElapsed(TimeUnit string);

	// Get FrameTime
	float GetFrameTime(TimeUnit string);
};

class FrameRateController : public Time_Channel
{
	int Frames = 0;
	float FPS = 60.0f;
	float MinFrameTime = 1 / FPS;

public:

	std::chrono::duration<float> Dt;

	// Get Number of Frames
	int GetFrames();
	// Start of Framerate Controller loop
	void FrameControllerStart();
	// End of Framerate Controller loop
	void FrameControllerEnd();
	// Change FPS
	void SetFPS(float);
};

#endif // FrameRateController_H
