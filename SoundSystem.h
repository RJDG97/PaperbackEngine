#ifndef _AUDIO_ENGINE_H_
#define _AUDIO_ENGINE_H_

// FMOD
#include "Extern/inc/fmod.hpp"
#include "Extern/inc/fmod_common.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>

class SoundSystem
{
	// Container to hold sound files
	std::map<std::string, FMOD::Sound*> soundLibrary;		// Format: <name, soundFile>
	// Container to hold fmod channels (Max 32)
	std::map<std::string, FMOD::Channel*> channelLibrary;	// Format: <name, channelName>
	// Create system
	FMOD::System* f_system = nullptr;
	// Mute or Pause system
	bool bMute;
	bool bPaused;

public:
	SoundSystem();
	bool checkError(FMOD_RESULT fResult);
	void loadSound(std::string fileLocation, std::string fileID, bool loopStatus = 0);
	void playSound(std::string fileID);
	void updateSound();
	void stopSound(std::string fileID, bool stopAllChannels = 0);
	void muteSound();
	void pauseSound();
	void unloadSound();
};

#endif