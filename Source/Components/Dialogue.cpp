/**********************************************************************************
*\file         Dialogue.cpp
*\brief        Contains definition of functions and variables used for
*			   the Dialogue Component
* 
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Components/Dialogue.h"
#include "Manager/ComponentManager.h"
#include "Engine/Core.h"
#include "Systems/Debug.h"
#include <iostream>

DialogueTrigger::DialogueTrigger() :
	dialogue_name_{}
{}

DialogueTrigger::~DialogueTrigger() {

	CORE->GetManager<ComponentManager>()->RemoveComponent<DialogueTrigger>(Component::GetOwner()->GetID());
}

void DialogueTrigger::Init() {

	CORE->GetManager<ComponentManager>()->AddComponent<DialogueTrigger>(Component::GetOwner()->GetID(), this);
}

void DialogueTrigger::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {
	
	writer->StartObject();

	writer->Key("component");
	writer->String("DialogueTrigger");

	writer->Key("dialogue name");
	writer->String(dialogue_name_.c_str());

	writer->EndObject();
}

void DialogueTrigger::DeSerialize(std::stringstream& data) {

	std::cout << "Serializing Dialogue Component" << std::endl;
	
	data >> dialogue_name_;
}

void DialogueTrigger::DeSerializeClone(std::stringstream& data) {

	DeSerialize(data);
}

void DialogueTrigger::SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	Serialize(writer);
}

std::shared_ptr<Component> DialogueTrigger::Clone() {
	M_DEBUG->WriteDebugMessage("Cloning DialogueTrigger Component\n");
	
	std::shared_ptr<DialogueTrigger> cloned = std::make_shared<DialogueTrigger>();

	cloned->dialogue_name_ = dialogue_name_;

	return cloned;
}

std::string DialogueTrigger::GetDialogue() const {

	return dialogue_name_;
}

void DialogueTrigger::SetDialogueName(const std::string& dialogue_name) {

	dialogue_name_ = dialogue_name;
}