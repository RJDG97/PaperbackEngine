#pragma once
#include "Manager/IManager.h"
#include "../rapidjson/filereadstream.h"
#include "../rapidjson/document.h"
#include "prettywriter.h"
#include <vector>
#include <string>
#include <map>

class DialogueContent
{
	std::string portrait_;
	std::string name_;
	std::string speech_;

public:

	DialogueContent() = default;
	DialogueContent(std::string portrait, std::string name, std::string speech);

	std::string* GetSpeech();
};

class Dialogue
{
	std::vector<DialogueContent> contents_;

public:

	Dialogue() = default;
	Dialogue(std::vector<DialogueContent> contents);

	std::vector<DialogueContent>* GetContents();
};

class DialogueManager : public IManager {

	std::map<std::string, Dialogue> dialogues_;

public:

	void Init();

	void LoadDialogueSet(std::string level_name);

	void LoadDialogue(std::string dialogue_name, std::string path);

	void DeserializeJSON(const std::string& filename, rapidjson::Document& doc);

	Dialogue* GetDialogue(std::string dialogue_name);
};
