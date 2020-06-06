#include "SoundSystem.h"

SoundSystem::SoundSystem() : bMute{ false }, bPaused{ false } {
	// Create system
	FMOD::System_Create(&f_system);
	// Initialize system
	f_system->init(32, FMOD_INIT_NORMAL, f_system);
}

bool SoundSystem::checkError(FMOD_RESULT fResult) {
	if (fResult == FMOD_OK) {
		return 1;
	}
	else {
		std::cout << "FMOD Error: " << fResult << std::endl;
		return 0;
	}
}

void SoundSystem::loadSound(std::string fileLocation, std::string fileID, bool loopStatus) {
	std::map<std::string, FMOD::Sound*>::iterator it = soundLibrary.find(fileID);
	FMOD_MODE mode = FMOD_DEFAULT;
	mode |= loopStatus ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;

	if (it == soundLibrary.end()) {
		FMOD::Sound* sound = nullptr;
		if (checkError(f_system->createSound(fileLocation.c_str(), mode, 0, &sound))) {
			if (sound != nullptr) {
				soundLibrary.insert(std::pair<std::string, FMOD::Sound*>(fileID, sound));
			}
		}
	}
}

void SoundSystem::playSound(std::string fileID) {
	std::map<std::string, FMOD::Sound*>::iterator it = soundLibrary.find(fileID);

	if (it != soundLibrary.end()) {
		auto channelIT = channelLibrary.find(fileID);
		if (channelIT == channelLibrary.end()) {
			FMOD::Channel* channel = nullptr;
			if (checkError(f_system->playSound(it->second, 0, false, &channel))) {
				if (channel != nullptr) {
					channelLibrary.insert(std::pair<std::string, FMOD::Channel*>(fileID, channel));
				}
			}
		}
	}
	else {
		std::cout << fileID.c_str() << " does not exist!" << std::endl;
		return;
	}
}

void SoundSystem::updateSound() {
	std::vector<std::map<std::string, FMOD::Channel*>::iterator> completedChannel;

	for (auto channel = channelLibrary.begin(); channel != channelLibrary.end(); ++channel) {
		bool bPlaying = false;
		channel->second->isPlaying(&bPlaying);
		if (!bPlaying) {
			completedChannel.push_back(channel);
		}
	}

	for (auto& channel : completedChannel) {
		std::cout << "Deleting completed channel: " << channel->first.c_str() << std::endl;
		channelLibrary.erase(channel);
	}
}

void SoundSystem::stopSound(std::string fileID, bool stopAllChannels) {
	if (!stopAllChannels) {
		std::map<std::string, FMOD::Channel*>::iterator it = channelLibrary.find(fileID);

		if (it != channelLibrary.end()) {
			it->second->stop();
		}
	}
	else {
		for (auto& it : channelLibrary) {
			it.second->stop();
		}
	}
}

void SoundSystem::muteSound() {
	if (channelLibrary.begin() == channelLibrary.end())
		return;

	channelLibrary.begin()->second->getMute(&bMute);
	bMute = !bMute;

	for (auto& it : channelLibrary) {
		it.second->setMute(bMute);
	}
}

void SoundSystem::pauseSound() {
	if (channelLibrary.begin() == channelLibrary.end())
		return;

	channelLibrary.begin()->second->getPaused(&bPaused);
	bPaused = !bPaused;

	for (auto& it : channelLibrary) {
		it.second->setPaused(bPaused);
	}
}

void SoundSystem::unloadSound() {
	for (auto begin = soundLibrary.begin(); begin != soundLibrary.end(); ++begin) {
		begin->second->release();
	}
	// Terminate system
	f_system->close();
	f_system->release();
}