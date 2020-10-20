#include "Systems/FrameRateController.h"
#include <iostream>

FrameRateController PE_FrameRate;

// Starts Timer
void Time_Channel::TimerStart()
{
	running_ = true;
}

// Stops Timer
void Time_Channel::TimerStop()
{
	running_ = false;
}

// Resets Timer
void Time_Channel::TimerReset()
{
	timeelapsed_ = frametime_ - frametime_;
}

// Updates Timer
void Time_Channel::TimerUpdate()
{
	// Get time at end of frame
	end_ = std::chrono::high_resolution_clock::now();
	// Get duration of frame
	frametime_ = end_ - start_;
	// Start new frame time
	start_ = std::chrono::high_resolution_clock::now();
	// Update Timer is running
	if (running_)
		timeelapsed_ += frametime_;
}

// Returns amount of time passed
float Time_Channel::TimeElapsed(TimeUnit string)
{
	// return value in either ms or s
	switch (string)
	{
	case ms:
		return timeelapsed_.count() * 1000.0f;
	case s:
		return timeelapsed_.count();
	}
	return 0.0f;
}

// Return time taken for one frame
float Time_Channel::GetFrameTime(TimeUnit string)
{
	// return value in either ms or s
	switch (string)
	{
	case ms:
		return frametime_.count() * 1000.0f;
	case s:
		return frametime_.count();
	}
	return 0.0f;
}

// Return number of frames elapsed
int FrameRateController::GetFrames()
{
	return frames_;
}

// Set start of the frame
void FrameRateController::FrameControllerStart()
{
	// Get time at start of frame
	start_ = std::chrono::high_resolution_clock::now();
}

// Set end of the frame
void FrameRateController::FrameControllerEnd()
{
	// Get Current time
	std::chrono::time_point<std::chrono::steady_clock> currenttime = std::chrono::high_resolution_clock::now();
	// Check whether time elapsed is past min frame time
	std::chrono::duration<float> frametime = currenttime - start_;
	while (frametime.count() < minframetime_)
	{
		// Check whether current time is more than min frame time
		end_ = std::chrono::high_resolution_clock::now();
		frametime = end_ - start_;
	}
	// Set Delta as frametime
	delta_ = end_ - start_;
	frametime = delta_;
	timeelapsed_ += frametime;
	dt_ = PE_FrameRate.delta_.count();

	while (timeelapsed_.count() >= minframetime_) {

		timeelapsed_ -= std::chrono::duration<float>(minframetime_);
		currentsteps_++;
	}

	// increment frames
	frames_++;
}

// Updates the framerate
void FrameRateController::FrameRateLoop()
{
	// Get time at start of frame
	if (frames_ == 0)
		start_ = std::chrono::high_resolution_clock::now();
	FrameControllerEnd();
	// Get time at start of frame
	FrameControllerStart();
	
}

// Change the amount of frames per second
void FrameRateController::SetFPS(float x)
{
	// Change Frames per Second
	fps_ = x;
	minframetime_ = 1 / fps_;
}

// Get the current amount of frames per second
int FrameRateController::GetFPS()
{
	static int prevFPS, newFPS, seconds;

	// Update number of frames in the past second
	if (seconds != static_cast<int>(TimeElapsed(s)))
	{
		newFPS = GetFrames() - prevFPS;
		prevFPS = GetFrames();
	}
	seconds = static_cast<int>(TimeElapsed(s));
	return newFPS;
}




// Placeholders
void FrameRateController::StartSystemTimer() {
	system_start_ = std::chrono::high_resolution_clock::now();
}
void FrameRateController::EndSystemTimer() {
	system_end_ = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> frametime = system_end_ - system_start_;
	system_update_time = frametime.count();
}

void FrameRateController::SetSystemPerformance(ISystem* system) {
	system_performance_[system->GetName()] = system_update_time;
}

void FrameRateController::PrintSystemPerformance() {
	total_time = 0.0f;
	std::cout << "=========================================\nDisplaying System Performance Data:" << std::endl;
	// To compute total time taken for 1 update loop
	for (PerformanceIt begin = system_performance_.begin(); begin != system_performance_.end(); ++begin) {
		total_time += begin->second;
	}
	// To compute and print performance data for each system
	for (PerformanceIt begin = system_performance_.begin(); begin != system_performance_.end(); ++begin) {
		std::cout << begin->first << ": " << begin->second / total_time * 100 << "%" << std::endl;
	}
	std::cout << "=========================================\n" << std::endl;
}