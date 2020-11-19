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

/*
class SoundFile
{
public:
	SoundFile();
	SoundFile(std::string name, float vol, bool pause, bool mute);
	void TogglePause();
	void SetPause(bool status);
	void ToggleMute();
	void SetMute(bool status);
	void Play();
	void Stop();
	~SoundFile();

private:
	FMOD::Sound* sound_;
	float volume_;
	bool pause_;
	bool mute_;
};
*/

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
	bool debug_;
	float volume_;

public:

/******************************************************************************/
/*!
  \fn SoundSystem()

  \brief Constructor for SoundSystem, ensures that fmod is initialised for use
*/
/******************************************************************************/
	SoundSystem();

/******************************************************************************/
/*!
  \fn ~SoundSystem()

  \brief Desstructor for SoundSystem, ensures that fmod assigned resources
		 have been released
*/
/******************************************************************************/
	~SoundSystem();

/******************************************************************************/
/*!
  \fn SetVolume()

  \brief Set volume for all sound channels
*/
/******************************************************************************/
	void SetVolume(const float& vol);

/******************************************************************************/
/*!
  \fn GetVolume()

  \brief Get volume for all sound channels
*/
/******************************************************************************/
	float GetVolume();

/******************************************************************************/
/*!
  \fn CheckError()

  \brief Checks if the FMOD sound file has been successfully loaded, 
		 returning a true if it is so
*/
/******************************************************************************/
	bool CheckError(FMOD_RESULT f_result);

/******************************************************************************/
/*!
  \fn LoadSound()

  \brief Given a file location and id, passes on the variables to an FMOD 
		 API to load the sound file
*/
/******************************************************************************/
	void LoadSound(std::string file_location, std::string file_id, bool loop_status = 0);

/******************************************************************************/
/*!
  \fn PlaySound()

  \brief Attaches the loaded sound file to an inactive channel to play the
		 the sound file
*/
/******************************************************************************/
	void PlaySounds(std::string file_id);

/******************************************************************************/
/*!
  \fn StopSound()

  \brief Stops a particular channel from playing or stop all active channels
*/
/******************************************************************************/
	void StopSound(std::string file_id, bool stop_all_channels = 0);

/******************************************************************************/
/*!
  \fn MuteSound()

  \brief Mutes all sound channels that are currently active
*/
/******************************************************************************/
	void MuteSound();

/******************************************************************************/
/*!
  \fn PauseSound()

  \brief Pauses all sound channels that are currently active
*/
/******************************************************************************/
	void PauseSound();

/******************************************************************************/
/*!
  \fn RemoveCompletedChannel()

  \brief Helper function that checks for channels that are completed and removes
         them from the map of currently active sound channels
*/
/******************************************************************************/
	void RemoveCompletedChannel();

/******************************************************************************/
/*!
  \fn Init()

  \brief Initializes all sound files that are to be loaded into the game
*/
/******************************************************************************/
	virtual void Init() override;

/******************************************************************************/
/*!
  \fn Update()

  \brief Updates all sound channels and removes channels that are done playing
         their sound files
*/
/******************************************************************************/
	virtual void Update(float frametime) override;

/******************************************************************************/
/*!
  \fn GetName()

  \brief Returns the system's name
*/
/******************************************************************************/
	virtual std::string GetName() override;

/******************************************************************************/
/*!
  \fn SendMessageD()

  \brief Receives messages broadcasted from Core Engine and processes it
*/
/******************************************************************************/
	virtual void SendMessageD(Message* m) override;

/******************************************************************************/
/*!
  \fn DeSerialize()

  \brief Initializes data members of sound system
*/
/******************************************************************************/
	void DeSerialize(const std::string& filepath);
};

#endif