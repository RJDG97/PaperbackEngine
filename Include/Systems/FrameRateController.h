#ifndef FrameRateController_H
#define FrameRateController_H

#include <chrono>
#include <map>
#include <string>
#include "ISystem.h"
#include <iostream>

enum TimeUnit
{
	ms = 0,
	s,
};

class Time_Channel
{
protected:

	// Time variables
	std::chrono::time_point<std::chrono::steady_clock> start_ = std::chrono::high_resolution_clock::now();
	std::chrono::time_point<std::chrono::steady_clock> end_;
	// Time Passed
	float timeelapsed_ = 0.0f;
	// Duration of frame (DeltaTime)
	std::chrono::duration<float> delta_;
	// Whether Timer is running
	bool running_ = false;

public:

/******************************************************************************/
/*!
  \fn TimerUpdate()

  \brief Updates the Timer (timeelapsed, frametime etc.)
*/
/******************************************************************************/
	void TimerUpdate();

/******************************************************************************/
/*!
  \fn TimerStart()

  \brief Starts the Timer
*/
/******************************************************************************/
	void TimerStart();

/******************************************************************************/
/*!
  \fn TimerStops()

  \brief Stops the Timer
*/
/******************************************************************************/
	void TimerStop();

/******************************************************************************/
/*!
  \fn TimerReset()

  \brief Set Timer to 0
*/
/******************************************************************************/
	void TimerReset();

/******************************************************************************/
/*!
  \fn TimerElapsed()

  \brief Returns current timer value
*/
/******************************************************************************/
	float TimeElapsed(TimeUnit string);

/******************************************************************************/
/*!
  \fn TimerFrameTime()

  \brief Returns duration of one frame
*/
/******************************************************************************/
	float GetFrameTime(TimeUnit string);
};

class FrameRateController : public Time_Channel
{
	int frames_ = 0, currentsteps_ = 1;
	float fps_ = 60.0f;
	float fixedframetime_ = 1 / fps_;

	// Placeholder start
	using PerformanceIt = std::map<std::string, float>::iterator;
	std::map<std::string, float> system_performance_;

	std::chrono::time_point<std::chrono::steady_clock> system_start_;
	std::chrono::time_point<std::chrono::steady_clock> system_end_;

	float total_time; // For 1 full core engine loop
	float system_update_time;
	// Placeholder end

public:

/******************************************************************************/
/*!
  \fn GetFPS()

  \brief Returns current framerate
*/
/******************************************************************************/
	int GetFPS();

/******************************************************************************/
/*!
  \fn GetFrames()

  \brief Returns total number of frames 
*/
/******************************************************************************/
	int GetFrames();

/******************************************************************************/
/*!
  \fn FrameControllerStart()

  \brief Sets the time of the start of the game loop
*/
/******************************************************************************/
	void FrameControllerStart();

/******************************************************************************/
/*!
  \fn FrameControllerEnd()

  \brief Sets the time of the end of the game loop
*/
/******************************************************************************/
	void FrameControllerEnd();

/******************************************************************************/
/*!
  \fn FrameRareLoop()

  \brief Updates the Framerate
*/
/******************************************************************************/
	void FrameRateLoop();

/******************************************************************************/
/*!
  \fn SetFPS(float)

  \brief change the current framerate
*/
/******************************************************************************/
	void SetFPS(float);

	float GetDelta();

	float GetFixedDelta();

	int GetSteps();

	void SetSystemPerformance(ISystem* system);
	void PrintSystemPerformance();
	void StartSystemTimer();
	void EndSystemTimer();
};

extern FrameRateController PE_FrameRate;

#endif // FrameRateController_H
