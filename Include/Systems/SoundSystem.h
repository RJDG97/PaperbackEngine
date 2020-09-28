#ifndef _AUDIO_ENGINE_H_
#define _AUDIO_ENGINE_H_

// FMOD
#include "fmod.hpp"
#include "fmod_common.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Systems/ISystem.h"

class SoundSystem : public ISystem
{
	// Container to hold sound files
	std::map<std::string, FMOD::Sound*> sound_library_;		// Format: <name, soundFile>
	// Sound library iterator
	using SoundIt = std::map<std::string, FMOD::Sound*>::iterator;

	// Container to hold fmod channels (Max 32)
	std::map<std::string, FMOD::Channel*> channel_library_;	// Format: <name, channelName>
	// Channel library iterator
	using ChannelIt = std::map<std::string, FMOD::Channel*>::iterator;

	// Container to hold completed channels to be deleted
	std::vector<ChannelIt> completed_channel_;

	// Create system
	FMOD::System* f_system_ = nullptr;
	// Mute or Pause system
	bool b_mute_;
	bool b_paused_;

public:
	SoundSystem();
	~SoundSystem();
	bool CheckError(FMOD_RESULT f_result);
	void LoadSound(std::string file_location, std::string file_id, bool loop_status = 0);
	void PlaySounds(std::string file_id);
	void StopSound(std::string file_id, bool stop_all_channels = 0);
	void MuteSound();
	void PauseSound();
	void RemoveCompletedChannel();

	virtual void Init() override;
	virtual void Update(float frametime) override;
	virtual std::string GetName() override;
	virtual void SendMessageD(Message* m) override;
};

#endif