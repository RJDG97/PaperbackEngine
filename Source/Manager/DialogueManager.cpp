#include "Manager/DialogueManager.h"
#include "Systems/Game.h"

DialogueContent::DialogueContent(std::string portrait, std::string name, std::string speech)
    : portrait_ {portrait},
      name_ {name},
      speech_ {speech}
{

}

std::string* DialogueContent::GetSpeech()
{
    return &speech_;
}

void DialogueManager::Init()
{
}

void DialogueManager::LoadDialogueSet(std::string level_name)
{
    rapidjson::Document dialogueset_to_load;
    std::string dialogueset_path = "Resources/Dialogue/" + level_name + "_dialogue.json";

    DeserializeJSON(dialogueset_path, dialogueset_to_load);

    const rapidjson::Value& files_arr = dialogueset_to_load;
    DEBUG_ASSERT(files_arr.IsObject(), "Level JSON does not exist in proper format");

    //handle sorting of information into the map
    for (rapidjson::Value::ConstMemberIterator file_it = files_arr.MemberBegin(); file_it != files_arr.MemberEnd(); ++file_it) {

        std::string dialogue_name { file_it->name.GetString() };
        std::string dialogue_path { file_it->value.GetString() };

        LoadDialogue(dialogue_name, dialogue_path);
    }
}

void DialogueManager::LoadDialogue(std::string dialogue_name, std::string path)
{
    rapidjson::Document dialogue_to_load;

    DeserializeJSON(path, dialogue_to_load);

    const rapidjson::Value& files_arr = dialogue_to_load;
    DEBUG_ASSERT(files_arr.IsObject(), "Level JSON does not exist in proper format");

    std::vector<DialogueContent> contents;

    for (rapidjson::Value::ConstMemberIterator file_it = files_arr.MemberBegin(); file_it != files_arr.MemberEnd(); ++file_it) {

        std::string portrait = file_it->value.GetString();
        ++file_it;
        std::string name = file_it->name.GetString();
        std::string speech = file_it->value.GetString();

        contents.push_back({ portrait, name, speech });

    }
}

void DialogueManager::DeserializeJSON(const std::string& filename, rapidjson::Document& doc)
{
    std::ifstream input_file(filename.c_str());
    DEBUG_ASSERT(input_file.is_open(), "File does not exist");

    // Read each line separated by a '\n' into a stringstream
    std::stringstream json_doc_buffer;
    std::string input;

    while (std::getline(input_file, input)) {

        json_doc_buffer << input << "\n";
    }

    // Close the file (.json) after
    input_file.close();

    // Parse the stringstream into document (DOM) format
    doc.Parse(json_doc_buffer.str().c_str());
}

Dialogue* DialogueManager::GetDialogue(std::string dialogue_name)
{
    return &dialogues_[dialogue_name];
}

std::vector<DialogueContent> Dialogue::GetContents()
{
    return contents_;
}
