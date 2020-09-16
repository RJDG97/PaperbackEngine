#ifndef _AUDIO_ENGINE_H_
#define _AUDIO_ENGINE_H_

// FMOD
#include "Extern/inc/fmod.hpp"
#include "Extern/inc/fmod_common.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>

/*
newly added isystem stuff
*/
#include "TestFiles/ISystem.h"

class MessageBGM_Play : public Message {
public:
	std::string _fileID;

	MessageBGM_Play(const std::string fileID) :
		Message(MessageIDTypes::BGM_Play),
		_fileID{ fileID }
	{}
};

class SoundSystem : public ISystem
{
	// Container to hold sound files
	std::map<std::string, FMOD::Sound*> soundLibrary;		// Format: <name, soundFile>
	// Sound library iterator
	using SoundIt = std::map<std::string, FMOD::Sound*>::iterator;

	// Container to hold fmod channels (Max 32)
	std::map<std::string, FMOD::Channel*> channelLibrary;	// Format: <name, channelName>
	// Channel library iterator
	using ChannelIt = std::map<std::string, FMOD::Channel*>::iterator;

	// Container to hold completed channels to be deleted
	std::vector<ChannelIt> completedChannel;

	// Create system
	FMOD::System* f_system = nullptr;
	// Mute or Pause system
	bool bMute;
	bool bPaused;

public:
	SoundSystem();
	~SoundSystem();
	bool checkError(FMOD_RESULT fResult);
	void loadSound(std::string fileLocation, std::string fileID, bool loopStatus = 0);
	void playSound(std::string fileID);
	void stopSound(std::string fileID, bool stopAllChannels = 0);
	void muteSound();
	void pauseSound();
	void removeCompletedChannel();

	virtual void init();
	virtual void update(float frametime);
	virtual std::string GetName();
	virtual void SendMessageD(Message* m);
};

#endif