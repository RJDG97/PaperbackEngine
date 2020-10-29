#include "GameStates/Levels.h"
#include "Systems/Debug.h"
#include "Systems/Factory.h"


Level::Level() {


}

Level::Level(const std::string name, const std::string path_name) :
	name_{ name },
	path_{ path_name }
{	}

void Level::DeSerialize() {
	
	if (path_ != "") {
		
		rapidjson::Document doc;
		DeSerializeJSON(path_, doc);

		const rapidjson::Value& files_arr = doc;
		DEBUG_ASSERT(files_arr.IsObject(), "Level JSON does not exist in proper format");

		//handle sorting of information into the map
		for (rapidjson::Value::ConstMemberIterator file_it = files_arr.MemberBegin(); file_it != files_arr.MemberEnd(); ++file_it) {

			std::string path_name{ file_it->value.GetString() };
			std::string archetype_name{ file_it->name.GetString() };

			entity_paths_.insert({ archetype_name, path_name });
		}
	}
}

void Level::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {
	
}

void Level::AddNewEntityPath() {
	
	entity_paths_.insert({});
}

void Level::DeleteEntityPath() {

	entity_paths_.erase(entity_paths_.end());
}




Levels::Levels() :
	current_play_index_{}
{
}

void Levels::DeSerialize(rapidjson::Document* doc) { //needs to directly load from doc format
	
	// Treats entire filestream at index as array and ensure that it is an array
	const rapidjson::Value& files_arr = *doc;
	DEBUG_ASSERT(files_arr.IsObject(), "Levels JSON does not exist in proper format");

	// Iterate through each level path
	for (rapidjson::Value::ConstMemberIterator file_it = files_arr.MemberBegin(); file_it != files_arr.MemberEnd(); ++file_it) {

		std::string path_name{ file_it->value.GetString() };
		std::string level_type{ file_it->name.GetString() };

		if (level_type == "Menu") {

			menu_.name_ = level_type;
			menu_.path_ = path_name;
		}
		else if (level_type == "Splash") {

			splash_.name_ = level_type;
			splash_.path_ = path_name;
		}
		else if (level_type == "Credits") {

			credits_.name_ = level_type;
			credits_.path_ = path_name;
		}
		else if (level_type == "Play") {

			plays_.push_back({level_type, path_name});
		}
	}
}

void Levels::DeSerializeLevels() {

	menu_.DeSerialize();
	splash_.DeSerialize();
	credits_.DeSerialize();

	for (PlaysIt play = plays_.begin(); play != plays_.end(); ++play) {

		play->DeSerialize();
	}
}

void Levels::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer, std::string filename) {
	
}

Level* Levels::GetPlayLevel(size_t index) {
	// Check if index is valid before returning
	if (index <= plays_.size()) {

		current_play_index_ = index; // double check on this
		return &plays_[index];
	}

	return nullptr;
}

Level* Levels::GetNextPlayableLevel() {
	
	
	if (++current_play_index_ <= plays_.size()) {

		return &plays_[current_play_index_];
	}

	current_play_index_ = 0;
	return &menu_;
}

void Levels::AddNewPlayLevelEntry() {
	
	plays_.push_back({});
}

void Levels::DeletePlayLevelEntry() {
	
	plays_.pop_back();
}