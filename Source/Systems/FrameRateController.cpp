#include "Systems/FrameRateController.h"

FrameRateController PE_FrameRate;

void Time_Channel::TimerStart()
{
	running = true;
}

void Time_Channel::TimerStop()
{
	running = false;
}

void Time_Channel::TimerReset()
{
	timeelapsed = frametime - frametime;
}

void Time_Channel::TimerUpdate()
{
	// Get time at end of frame
	end = std::chrono::high_resolution_clock::now();
	// Get duration of frame
	frametime = end - start;
	// Start new frame time
	start = std::chrono::high_resolution_clock::now();
	// Update Timer is running
	if (running)
		timeelapsed += frametime;
}

float Time_Channel::TimeElapsed(TimeUnit string)
{
	// return value in either ms or s
	switch (string)
	{
	case ms:
		return timeelapsed.count() * 1000.0f;
	case s:
		return timeelapsed.count();
	}
	return 0.0f;
}

float Time_Channel::GetFrameTime(TimeUnit string)
{
	// return value in either ms or s
	switch (string)
	{
	case ms:
		return frametime.count() * 1000.0f;
	case s:
		return frametime.count();
	}
	return 0.0f;
}

int FrameRateController::GetFrames()
{
	return Frames;
}

void FrameRateController::FrameControllerStart()
{
	// Get time at start of frame
	start = std::chrono::high_resolution_clock::now();
}

void FrameRateController::FrameControllerEnd()
{
	// Get Current time
	std::chrono::time_point<std::chrono::steady_clock> currenttime = std::chrono::high_resolution_clock::now();
	// Check whether time elapsed is past min frame time
	std::chrono::duration<float> _FrameTime = currenttime - start;
	while (_FrameTime.count() < MinFrameTime)
	{
		// Check whether current time is more than min frame time
		end = std::chrono::high_resolution_clock::now();
		_FrameTime = end - start;
	}
	// Set Delta as frametime
	Delta = end - start;
	frametime = Delta;
	timeelapsed += frametime;
	Dt = PE_FrameRate.Delta.count();

	// increment frames
	Frames++;
}

void FrameRateController::FrameRateLoop()
{
	// Get time at start of frame
	if (Frames == 0)
		start = std::chrono::high_resolution_clock::now();
	FrameControllerEnd();
	// Get time at start of frame
	FrameControllerStart();
	
}

void FrameRateController::SetFPS(float x)
{
	// Change Frames per Second
	FPS = x;
	MinFrameTime = 1 / FPS;
}


