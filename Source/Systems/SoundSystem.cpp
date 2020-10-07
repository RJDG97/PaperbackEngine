#include "Systems/SoundSystem.h"
#include "Engine/Core.h"
#include "Systems/InputSystem.h"
#include "Systems/Debug.h"

SoundSystem::SoundSystem() : 
	b_mute_{ false }, 
	b_paused_{ false },
	debug_{ false }
{
	// Create system
	FMOD::System_Create(&f_system_);
	// Initialize system
	f_system_->init(32, FMOD_INIT_NORMAL, f_system_);
}

SoundSystem::~SoundSystem() {
	// Iterate through sound library and release all files
	for (auto begin = sound_library_.begin(); begin != sound_library_.end(); ++begin) {
		begin->second->release();
	}

	// Terminate system
	f_system_->close();
	f_system_->release();
}

bool SoundSystem::CheckError(FMOD_RESULT fResult) {
	if (fResult == FMOD_OK) {
		return 1;
	}
	else {
		std::cout << "FMOD Error: " << fResult << std::endl;
		return 0;
	}
}

void SoundSystem::LoadSound(std::string file_location, std::string file_id, bool loop_status) {
	// Determine whether sound file already exists within sound library
	SoundIt it = sound_library_.find(file_id);

	FMOD_MODE mode = FMOD_DEFAULT;
	mode |= loop_status ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;

	// If sound file does not exist within sound library, include it
	if (it == sound_library_.end()) {
		FMOD::Sound* sound = nullptr;
		// Load the sound file
		if (CheckError(f_system_->createSound(file_location.c_str(), mode, 0, &sound))) {
			// If sound file is successfully loaded
			if (sound != nullptr) {
				// Insert sound file into sound library
				sound_library_.insert(std::pair<std::string, FMOD::Sound*>(file_id, sound));
			}
		}
	}
}

void SoundSystem::PlaySounds(std::string fileID) {
	// Check whether sound file exists within sound library
	SoundIt it = sound_library_.find(fileID);

	// If sound file exists within the sound library
	if (it != sound_library_.end()) {
		// Check whether current sound file is already playing
		ChannelIt channelIT = channel_library_.find(fileID);
		// If sound file is currently not playing
		if (channelIT == channel_library_.end()) {
			FMOD::Channel* channel = nullptr;
			// Load the channel with the sound file and play
			if (CheckError(f_system_->playSound(it->second, 0, false, &channel))) {
				// If channel is playing the sound file successfully
				if (channel != nullptr) {
					// Insert channel into channel library
					channel_library_.insert(std::pair<std::string, FMOD::Channel*>(fileID, channel));
				}
			}
		}
	}
	else {
		std::cout << fileID.c_str() << " does not exist!" << std::endl;
		return;
	}
}

void SoundSystem::StopSound(std::string fileID, bool stopAllChannels) {
	// Stop a single channel
	if (!stopAllChannels) {
		// Check whether sound file is currently playing in a channel
		ChannelIt it = channel_library_.find(fileID);

		// If sound file is current playing in a channel
		if (it != channel_library_.end()) {
			// Stop the channel
			it->second->stop();
		}
	}
	// Stop all channels (End of level)
	else {
		// Iterate through all active channels
		for (auto& it : channel_library_) {
			// Stop the channel
			it.second->stop();
		}
	}
}

void SoundSystem::MuteSound() {
	// If there are no active channels, return
	if (channel_library_.begin() == channel_library_.end())
		return;

	// Get the status of channels (Muted / Not Muted)
	channel_library_.begin()->second->getMute(&b_mute_);
	// Toggle mute status
	b_mute_ = !b_mute_;

	// Assign updated status to all active channels
	for (auto& it : channel_library_) {
		it.second->setMute(b_mute_);
	}
}

void SoundSystem::PauseSound() {
	// If there are no active channels, return
	if (channel_library_.begin() == channel_library_.end())
		return;

	// Get the status of channels (Paused / Not Paused)
	channel_library_.begin()->second->getPaused(&b_paused_);
	// Toggle pause status
	b_paused_ = !b_paused_;

	// Assign updated status to all active channels
	for (auto& it : channel_library_) {
		it.second->setPaused(b_paused_);
	}
}

void SoundSystem::RemoveCompletedChannel() {
	// Iterate through all completed channels
	for (auto& channel : completed_channel_) {
		std::cout << "Deleting completed channel: " << channel->first.c_str() << std::endl;
		channel_library_.erase(channel);
	}

	// Clear and resize completed channel
	completed_channel_.clear();
}

void SoundSystem::Init() {
	// Load all sound files
	LoadSound("Resources/SoundCache/GameBGM.wav", "BGM", 1);
	LoadSound("Resources/SoundCache/MenuBGM.wav", "Menu", 1);

	M_DEBUG->WriteDebugMessage("Sound System Init\n");
}

void SoundSystem::Update(float frametime) {

	/*!
		Check all active channel's status (Done playing?)
		- Push all completed channels into vector for deletion
	*/
	(void)frametime;
	
	{
		// Iterate through all active channels
		for (auto channel = channel_library_.begin(); channel != channel_library_.end(); ++channel) {
			bool b_playing = false;
			// Get channel's status (Playing / Completed)
			channel->second->isPlaying(&b_playing);
			if (!b_playing) {
				// Push back the completed channel after its completed
				completed_channel_.push_back(channel);
			}
		}

		if (completed_channel_.size() > 0) {
			// If channel has completed playing sound file, notify
			// system to remove it
			Message msg(MessageIDTypes::BGM_COMPLETED);
			CORE->BroadcastMessage(&msg);
		}
	}
}

std::string SoundSystem::GetName() {
	
	return { "SoundSystem" };
}

//receives message as base class pointer and uses them based on message id value
void SoundSystem::SendMessageD(Message* m) {

	switch (m->message_id_) {
	case MessageIDTypes::BGM_PLAY:
	{
		//plays a fileID as included in the message
		MessageBGM_Play* msg = dynamic_cast<MessageBGM_Play*>(m);
		std::cout << "Playing Sound File: " << msg->file_id_ << std::endl;
		PlaySounds(msg->file_id_);
		break;
	}
	case MessageIDTypes::BGM_PAUSE:
	{
		// Pause all current active channels
		PauseSound();
		break;
	}
	case MessageIDTypes::BGM_MUTE:
	{
		// Mute all current active channels
		MuteSound();
		break;
	}
	case MessageIDTypes::BGM_STOP:
	{
		// stops the current BGM
		//need to check id of song to stop
		StopSound("BGM");
		break;
	}
	case MessageIDTypes::BGM_RELOAD:
	{
		// Reload bgm and play
		LoadSound("Resources/SoundCache/KK_BBG.mp3", "BGM");
		PlaySounds("BGM");
	}
	case MessageIDTypes::BGM_COMPLETED:
	{
		RemoveCompletedChannel();
		break;
	}
	default:
		break;
	}
}