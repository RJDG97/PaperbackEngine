#include "GameStates/Levels.h"
#include "Systems/Debug.h"
#include "Systems/Factory.h"


Level::Level() :
	name_{},
	path_{}
{
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

void Level::Serialize() {

	Serialize(path_);
}

void Level::Serialize(const std::string filepath) {
	
	rapidjson::StringBuffer sb;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);

	std::ofstream filestream(filepath);

	if (filestream.is_open()) {

		writer.StartObject();

		for (EntityPathsIt it = entity_paths_.begin(); it != entity_paths_.end(); ++it) {
			
			writer.Key(it->first.c_str());
			writer.String(it->second.c_str());
		}

		writer.EndObject();

		filestream << sb.GetString();
	}

	filestream.close();
}

void Level::AddNewEntityPath() {
	
	entity_paths_.insert({});
}

void Level::DeleteEntityPath() {

	entity_paths_.erase(entity_paths_.end());
}




Levels::Levels() :
	path_{},
	current_play_index_{}
{
}

void Levels::DeSerialize(const std::string filepath) { //needs to directly load from doc format
	
	rapidjson::Document doc;
	DeSerializeJSON(filepath, doc);

	path_ = filepath;

	// Treats entire filestream at index as array and ensure that it is an array
	const rapidjson::Value& files_arr = doc;
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
		else if (level_type == "Win") {

			win_.name_ = level_type;
			win_.path_ = path_name;
		}
		else if (level_type == "Lose") {

			lose_.name_ = level_type;
			lose_.path_ = path_name;
		}
		else if (level_type == "Play") {

			plays_.push_back({level_type, path_name});
		}
		else if (level_type == "Editor") {

			editor_.name_ = level_type;
			editor_.path_ = path_name;
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

void Levels::Serialize() {

	Serialize(path_);
}

void Levels::Serialize(const std::string filename) {
	
	rapidjson::StringBuffer sb;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);

	std::ofstream filestream(filename);

	if (filestream.is_open()) {

		writer.StartObject();

		if (menu_.name_ != "") {

			writer.Key(menu_.name_.c_str());
			writer.String(menu_.path_.c_str());
		}

		if (splash_.name_ != "") {

			writer.Key(splash_.name_.c_str());
			writer.String(splash_.path_.c_str());
		}

		if (credits_.name_ != "") {

			writer.Key(credits_.name_.c_str());
			writer.String(credits_.path_.c_str());
		}

		if (win_.name_ != "") {

			writer.Key(win_.name_.c_str());
			writer.String(win_.path_.c_str());
		}

		if (lose_.name_ != "") {

			writer.Key(lose_.name_.c_str());
			writer.String(lose_.path_.c_str());
		}

		if (editor_.name_ != "") {

			writer.Key(editor_.name_.c_str());
			writer.String(editor_.path_.c_str());
		}

		for (PlaysIt it = plays_.begin(); it != plays_.end(); ++it) {
			
			writer.Key(it->name_.c_str());
			writer.String(it->path_.c_str());
		}

		writer.EndObject();

		filestream << sb.GetString();
	}

	filestream.close();
}

// USE AT OWN RISK, WILL OVERWRITE CURRENTLY SET PARAMETERS
// UNSAFE TO USE UNTIL WENQING ADDS HER GRAPHICS SERIALIZE OWO
void Levels::SerializeLevels() {

	Serialize();

	menu_.Serialize();
	splash_.Serialize();
	credits_.Serialize();
	win_.Serialize();
	lose_.Serialize();
	
	for (PlaysIt it = plays_.begin(); it != plays_.end(); ++it) {
		it->Serialize();
	}
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