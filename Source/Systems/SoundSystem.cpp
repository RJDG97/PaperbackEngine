/**********************************************************************************
*\file         SoundSystem.cpp
*\brief        Contains definition of functions and variables used for
*			   the Sound System
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Manager/EntityManager.h"
#include "Systems/SoundSystem.h"
#include "Engine/Core.h"
#include "Systems/InputSystem.h"
#include "MathLib/MathHelper.h"
#include "Systems/Debug.h"

SoundSystem::SoundSystem() : 
	b_mute_{ false }, 
	b_paused_{ false },
	debug_{ false },
	volume_{ 0.3f }
{
	// Create system
	FMOD::System_Create(&f_system_);
	// Initialize system
	f_system_->init(32, FMOD_INIT_NORMAL, f_system_);
}

SoundSystem::~SoundSystem() {

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

SoundSystem::SoundMap& SoundSystem::GetSoundLibrary() {
	
	return sound_library_;
}

SoundSystem::ChannelMap& SoundSystem::GetChannelLibrary() {
	
	return channel_library_;
}

void SoundSystem::SetVolume(std::string fileID, const float& vol) {
	
	auto s_it = sound_library_.find(fileID);

	if (s_it != sound_library_.end()) {
		
		s_it->second->volume_ = vol;

		auto c_it = channel_library_.find(fileID);

		if (c_it != channel_library_.end()) {
			
			c_it->second->volume_ = vol;
		}
	}
}

void SoundSystem::LoadSound(std::string name, std::stringstream& data) {

	std::shared_ptr<SoundFile> sf = std::make_shared<SoundFile>();

	// Read sound file path
	data >> sf->path_;

	// Read SoundFile data
	data >> sf->volume_ >> sf->min_distance_ 
		 >> sf->volume_falloff_ >> sf->loop_;

	sf->original_volume_ = sf->volume_;

	// Determine whether sound file already exists within sound library
	SoundIt it = sound_library_.find(name);

	FMOD_MODE mode = FMOD_DEFAULT;
	mode |= sf->loop_ ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;

	// If sound file does not exist within sound library, include it
	if (it == sound_library_.end()) {

		// Load the sound file
		if (CheckError(f_system_->createSound(sf->path_.c_str(), mode, 0, &sf->sound_))) {

			// If sound file is successfully loaded
			if (sf->sound_ != nullptr) {

				// Insert sound file into sound library
				//sound_library_.emplace(std::pair<std::string, SoundFile*>(name, sf));
				sound_library_[name] = sf;
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

			std::shared_ptr<SoundChannel> channel = std::make_shared<SoundChannel>();

			// Load the channel with the sound file and play
			if (CheckError(f_system_->playSound(it->second->sound_, 0, false, &channel->channel_))) {

				// If channel is playing the sound file successfully
				if (channel != nullptr) {

					channel->volume_ = it->second->volume_;
					channel->original_volume_ = it->second->original_volume_;
					channel->min_distance_ = it->second->min_distance_;
					channel->sqr_min_distance_ = it->second->min_distance_ * it->second->min_distance_;
					channel->volume_falloff_ = it->second->volume_falloff_;

					channel_library_[fileID] = channel;
					//channel_library_.emplace(std::pair<std::string, SoundChannel*>(fileID, channel));
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

		ChannelIt it = channel_library_.find(fileID);

		if (it != channel_library_.end()) {

			it->second->channel_->stop();
		}
	}
	// Stop all channels (End of level)
	else {

		for (auto& [name, sound_channel] : channel_library_) {

			sound_channel->channel_->stop();
		}
	}
}

void SoundSystem::MuteSound(std::string file_id, bool status, bool all) {

	// If there are no active channels, return
	if (channel_library_.begin() == channel_library_.end())
		return;

	if (!all) {

		auto it = channel_library_.find(file_id);

		if (it != channel_library_.end()) {
			
			it->second->channel_->setMute(status);
		}
	}
	else {
		
		b_mute_ = !b_mute_;

		for (auto& [name, sound_channel] : channel_library_) {
			
			sound_channel->mute_ = b_mute_;
			sound_channel->channel_->setMute(b_mute_);
		}
	}
}

void SoundSystem::PauseSound(std::string file_id, bool status, bool all) {

	// If there are no active channels, return
	if (channel_library_.begin() == channel_library_.end())
		return;

	if (!all) {

		auto it = channel_library_.find(file_id);

		if (it != channel_library_.end()) {

			it->second->channel_->setPaused(status);
		}
	}
	else {

		b_paused_ = status;

		for (auto& [name, sound_channel] : channel_library_) {

			sound_channel->pause_ = b_paused_;
			sound_channel->channel_->setPaused(b_paused_);
		}
	}
}

void SoundSystem::RemoveSound(std::string name) {

	auto channel_it = channel_library_.find(name);
	auto sound_it = sound_library_.find(name);

	if (channel_it != channel_library_.end()) {
		
		channel_library_.erase(channel_it);
	}
	if (sound_it != sound_library_.end()) {
		
		sound_library_.erase(sound_it);
	}
}

void SoundSystem::RemoveCompletedChannel() {

	// Iterate through all completed channels
	for (auto& channel : completed_channel_) {

		channel_library_.erase(channel);
	}

	// Clear and resize completed channel
	completed_channel_.clear();
}

void SoundSystem::DeSerialize(const std::string& filepath) {

	// Parse the stringstream into document (DOM) format
	rapidjson::Document doc;
	DeSerializeJSON(filepath, doc);

	// Treats entire filestream at index as array and ensure that it is an array
	const rapidjson::Value& sound_arr = doc;
	DEBUG_ASSERT(sound_arr.IsObject(), "Entry does not exist in JSON");

	// Iterate through each prefab
	for (rapidjson::Value::ConstMemberIterator var_it = sound_arr.MemberBegin(); var_it != sound_arr.MemberEnd(); ++var_it) {

		const rapidjson::Value& value_arr = var_it->value;

		std::string sound_name{ var_it->name.GetString() };

		M_DEBUG->WriteDebugMessage("Loading soundfile: " + sound_name + "\n");

		// Iterate through the body of the prefab that contains components
		for (rapidjson::Value::ConstValueIterator it = value_arr.Begin(); it != value_arr.End(); ++it) {

			// Each value is essentially a container for multiple members
			// IsObject enforces that the member is an object that will contain data:key pairs
			const rapidjson::Value& member = *it;

			DEBUG_ASSERT(member.IsObject(), "Entry does not exist in JSON");
			//stores the data into a stream that is easier to read data from
			std::stringstream stream;

			for (rapidjson::Value::ConstMemberIterator it2 = member.MemberBegin(); it2 != member.MemberEnd(); ++it2) {

				stream << it2->value.GetString() << " ";
			}

			LoadSound(sound_name, stream);
		}
	}
}

void SoundSystem::Serialize(const std::string& filepath) {
	
	std::ofstream file(filepath);

	DEBUG_ASSERT(file.is_open(), "File does not exist");

	rapidjson::StringBuffer sb;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);

	writer.StartObject();

	for (auto& [name, soundfile] : sound_library_) {
		
		writer.Key(name.c_str());
		writer.StartArray();
		soundfile->Serialize(&writer);
		writer.EndArray();
	}

	writer.EndObject();

	file << sb.GetString();

	file.close();
}

void SoundSystem::Init() {

	// Load all sound files
	DeSerialize("Resources/AssetsLoading/sounds.json");
	component_manager_ = CORE->GetManager<ComponentManager>();
	graphics_system_ = CORE->GetSystem<GraphicsSystem>();
	sound_emitter_arr_ = component_manager_->GetComponentArray<SoundEmitter>();
	players_ = &CORE->GetManager<EntityManager>()->GetPlayerEntities();

	M_DEBUG->WriteDebugMessage("Sound System Init\n");
}

void SoundSystem::Update(float frametime) {

	(void)frametime;	
	
	if (!players_->empty()) {

		EntityID player_id = players_->back()->GetID();
		Transform* player_transform = component_manager_->GetComponent<Transform>(player_id);

		DEBUG_ASSERT(player_transform, "Player does not have a transform component!");

		for (auto& [id, sound_emitter] : *sound_emitter_arr_) {

			ChannelIt it = channel_library_.find(sound_emitter->sound_name_);
			if (!sound_emitter->num_sound_lines_)
				continue;

			// If currently not playing sound, play it
			if (it == channel_library_.end()) {

				PlaySounds(sound_emitter->sound_name_);
				return;
			}

			float dist = sound_emitter->GetMinDist(player_transform->GetOffsetAABBPos());

			if (dist > it->second->min_distance_) {

				float diff = dist - it->second->min_distance_;

				// Set volume
				it->second->volume_ = it->second->original_volume_ - (diff * it->second->volume_falloff_);
				// If volume lower than 0.0f, set to 0.0f
				it->second->volume_ = it->second->volume_ < 0.0f ? 0.0f : it->second->volume_;
			}
		}
	}

	for (auto channel = channel_library_.begin(); channel != channel_library_.end(); ++channel) {
		
		bool b_playing = false;
		
		// Get status of channel
		channel->second->channel_->setVolume(channel->second->volume_);
		channel->second->channel_->isPlaying(&b_playing);
	
		if (!b_playing) {
			
			completed_channel_.push_back(channel);
		}
	}
	
	if (completed_channel_.size() > 0) {

		RemoveCompletedChannel();
	}
	
}

void SoundSystem::Draw() {

	if (debug_) {
		
		std::vector<std::pair<glm::vec2, glm::vec2>> points;

		for (auto& [id, sound_emitter] : *sound_emitter_arr_) {

			for (size_t i = 0; i < sound_emitter->GetSoundLines().size(); ++i) {
				
				std::vector<glm::vec2> line{};
				SoundLineToGLM(sound_emitter->GetSoundLines()[i], line);
				points.push_back({line[0], line[1]});

				if (points.size() == graphics_system_->GetBatchSize())
				{
					graphics_system_->DrawDebugLines(points, { 1.0f, 0.0f, 0.0f, 1.0f }, 1.0f);
					points.clear();
				}
			}
		}

		if (points.size() > 0)
		{
			graphics_system_->DrawDebugLines(points, { 1.0f, 0.0f, 0.0f, 1.0f }, 1.0f);
		}
	}
}

std::string SoundSystem::GetName() {
	
	return { "SoundSystem" };
}

//receives message as base class pointer and uses them based on message id value
void SoundSystem::SendMessageD(Message* m) {

	switch (m->message_id_) {
	case MessageIDTypes::DEBUG_ALL:
	{
		debug_ = !debug_;
		break;
	}
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
		PauseSound("all", true, true);
		break;
	}
	case MessageIDTypes::BGM_RESUME:
	{
		// Resume all current active channels
		PauseSound("all", false, true);
		break;
	}
	case MessageIDTypes::BGM_MUTE:
	{
		// Mute all current active channels
		MuteSound("all", true, true);
		break;
	}
	case MessageIDTypes::BGM_STOP:
	{
		// stops the current BGM
		//need to check id of song to stop
		MessageBGM_Stop* msg = dynamic_cast<MessageBGM_Stop*>(m);
		std::cout << "Stopping Sound File: " << msg->file_id_ << std::endl;
		StopSound(msg->file_id_);
		break;
	}
	default:
		break;
	}
}





// Sound file
SoundFile::SoundFile() :
	sound_{ nullptr },
	path_{ },
	volume_{ 1.0f },
	original_volume_{ 1.0f },
	min_distance_{ },
	volume_falloff_{ },
	loop_{ false }
{ }

SoundFile::SoundFile(std::string path, float vol, float min_distance, float volume_falloff, bool loop) :
	sound_{ nullptr },
	path_{ path },
	volume_{ vol },
	original_volume_{ vol },
	min_distance_{ min_distance },
	volume_falloff_{ volume_falloff },
	loop_{ loop }
{ }

void SoundFile::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartObject();

	writer->Key("path");
	writer->String(path_.c_str());

	writer->Key("volume");
	writer->String(std::to_string(volume_).c_str());

	writer->Key("min distance");
	writer->String(std::to_string(min_distance_).c_str());

	writer->Key("volume falloff");
	writer->String(std::to_string(volume_falloff_).c_str());

	writer->Key("loop");
	writer->String(std::to_string(loop_).c_str());

	writer->EndObject();
}

SoundFile::~SoundFile() {
}



// Sound channel
SoundChannel::SoundChannel() :
	channel_{ nullptr },
	volume_{ 1.0f },
	original_volume_{ 1.0f },
	min_distance_{ 0.0f },
	sqr_min_distance_{ 0.0f },
	volume_falloff_{ 0.0f },
	pause_{ false },
	mute_{ false }
{ }

SoundChannel::SoundChannel(float volume, float min_distance, float volume_falloff) :
	channel_{ nullptr },
	volume_{ volume },
	original_volume_{ volume },
	min_distance_{ min_distance },
	sqr_min_distance_{ min_distance * min_distance },
	volume_falloff_{ volume_falloff },
	pause_{ false },
	mute_{ false }
{ }

SoundChannel::~SoundChannel() {
	
	channel_->stop();
}