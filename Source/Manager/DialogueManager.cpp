/**********************************************************************************
*\file         DialogueManager.cpp
*\brief        Contains definition of functions and variables used for
*			   the Dialogue Manager
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Manager/DialogueManager.h"
#include "Systems/Game.h"

DialogueContent::DialogueContent(DialogueCameraEffect effect,
                                 std::string left_portrait,
                                 std::string right_portrait,
                                 std::string name,
                                 std::string speech,
					             bool left_darken,
					             bool right_darken)
    : effect_ { effect },
      left_portrait_ { left_portrait },
      right_portrait_{ right_portrait },
      name_{ name },
      speech_ {speech},
      left_darken_ { left_darken },
      right_darken_ { right_darken }
{

}

Dialogue::Dialogue(std::vector<DialogueContent> contents)
    : contents_ { contents }
{

}

DialogueCameraEffect* DialogueContent::GetDialogueCameraEffect()
{
    return &effect_;
}

std::string* DialogueContent::GetSpeech()
{
    return &speech_;
}

std::string DialogueContent::GetName()
{
    return name_;
}

std::string DialogueContent::GetPortraitLeft()
{
    return left_portrait_;
}

std::string DialogueContent::GetPortraitRight()
{
    return right_portrait_;
}

bool DialogueContent::GetLeftDarken()
{
    return left_darken_;
}

bool DialogueContent::GetRightDarken()
{
    return right_darken_;
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
    DEBUG_ASSERT(files_arr.IsObject(), "Dialogue JSON does not exist in proper format");

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
        
        std::stringstream camera_effect{};
        camera_effect << file_it->value.GetString();
        
        DialogueCameraEffect effect;
        std::string effect_type;
        camera_effect >> effect_type;

        if (effect_type == "Shake")
        {
            effect.type_ = CAMERA_SHAKE;
            camera_effect >> effect.amplitude_ >> effect.duration_ >> effect.delay_;
        }

        else if (effect_type == "Move")
        {
            effect.type_ = CAMERA_MOVE;
            Vector2D position;
            camera_effect >> position.x >> position.y;
            effect.destination_ = position;
        }

        else if (effect_type == "None")
        {
            effect.type_ = CAMERA_NONE;
        }
        
        else
        {
            DEBUG_ASSERT(false, "Invalid camera effect!");
        }
        
        ++file_it;

        std::stringstream portrait;
        portrait << file_it->value.GetString();

        std::string left_portrait;
        std::string right_portrait;
        bool left_darken;
        bool right_darken;

        portrait >> left_portrait >> right_portrait >> left_darken >> right_darken;

        ++file_it;

        std::string name = file_it->name.GetString();
        std::string speech = file_it->value.GetString();
        
        contents.push_back({ effect, left_portrait, right_portrait, name, speech, !left_darken, !right_darken });
    }

    dialogues_[dialogue_name] = { contents };
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

std::vector<DialogueContent>* Dialogue::GetContents()
{
    return &contents_;
}