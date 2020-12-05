/**********************************************************************************
*\file         SoundSystem.h
*\brief        Contains declaration of functions and variables used for
*			   the Sound System
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#ifndef _AUDIO_ENGINE_H_
#define _AUDIO_ENGINE_H_

// FMOD
#include "fmod.hpp"
#include "fmod_common.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Manager/ComponentManager.h"
#include "Components/SoundEmitter.h"
#include "Systems/ISystem.h"
#include "Systems/GraphicsSystem.h"
#include "prettywriter.h"
#include <memory>


class SoundFile
{
	friend class SoundSystem;

public:
	SoundFile();
	SoundFile(std::string path, float vol, float min_distance, float volume_falloff, bool loop);
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer);
	~SoundFile();

private:
	FMOD::Sound* sound_;
	std::string path_;
	float volume_;
	float original_volume_;
	float min_distance_;
	float volume_falloff_;
	bool loop_;
};

class SoundChannel
{
	friend class SoundSystem;

public:

	/******************************************************************************/
	/*!
	  \fn SoundChannel()

	  \brief Constructor for SoundChannel
	*/
	/******************************************************************************/
	SoundChannel();

	/******************************************************************************/
	/*!
	  \fn SoundChannel()

	  \brief Constructor for SoundChannel
	*/
	/******************************************************************************/
	SoundChannel(float volume, float min_distance, float volume_falloff);

	/******************************************************************************/
	/*!
	  \fn SoundChannel()

	  \brief Destructor for SoundChannel
	*/
	/******************************************************************************/
	~SoundChannel();

private:
	FMOD::Channel* channel_;
	float volume_;
	float original_volume_;
	float min_distance_;
	float sqr_min_distance_;
	float volume_falloff_;
	bool pause_;
	bool mute_;
};


class SoundSystem : public ISystem
{
	// Sound map
	using SoundMap = std::map<std::string, std::shared_ptr<SoundFile>>;
	using SoundIt = SoundMap::iterator;
	SoundMap sound_library_;

	// Channel map
	using ChannelMap = std::map<std::string, std::shared_ptr<SoundChannel>>;
	using ChannelIt = ChannelMap::iterator;
	ChannelMap channel_library_;

	using SoundEmitterType = CMap<SoundEmitter>;
	using SoundEmitterTypeIt = SoundEmitterType::MapTypeIt;
	SoundEmitterType* sound_emitter_arr_;

	using PlayerArr = std::vector<Entity*>;
	PlayerArr* players_;

	std::shared_ptr<ComponentManager> component_manager_;
	std::shared_ptr<GraphicsSystem> graphics_system_;

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

  \brief Set volume for a sound channel
*/
/******************************************************************************/
	void SetVolume(std::string fileID, const float& vol);

/******************************************************************************/
/*!
  \fn GetSoundLibrary()

  \brief Return reference to sound library
*/
/******************************************************************************/
	SoundMap& GetSoundLibrary();

/******************************************************************************/
/*!
  \fn GetChannelLibrary()

  \brief Return reference to channel library
*/
/******************************************************************************/
	ChannelMap& GetChannelLibrary();

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
	void LoadSound(std::string name, std::stringstream& data);

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
	void MuteSound(std::string file_id, bool status, bool all = 0);

/******************************************************************************/
/*!
  \fn PauseSound()

  \brief Pauses all sound channels that are currently active
*/
/******************************************************************************/
	void PauseSound(std::string file_id, bool status, bool all = 0);

/******************************************************************************/
/*!
  \fn RemoveSound()

  \brief Removes a sound file that is currently loaded
*/
/******************************************************************************/
	void RemoveSound(std::string name);

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
	void Init() override;

/******************************************************************************/
/*!
  \fn Update()

  \brief Updates all sound channels and removes channels that are done playing
         their sound files
*/
/******************************************************************************/
	void Update(float frametime) override;

/******************************************************************************/
/*!
  \fn Draw()

  \brief Draws sound lines
*/
/******************************************************************************/
	void Draw() override;

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

/******************************************************************************/
/*!
  \fn Serialize()

  \brief Saves all sound files that have been loaded into the sound system
*/
/******************************************************************************/
	void Serialize(const std::string& filepath);
};


#endif