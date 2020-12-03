/**********************************************************************************
*\file         Attack.cpp
*\brief        Contains definition of functions and variables used for
*			   the Attack Component
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Components/Attack.h"

Attack::Attack() : attack_power_{}
{}

void Attack::Init() {
	// Create the map afterwards
	//GAMELOGIC->Attacks[Component::GetOwner()->GetID()] = *this;
}

void Attack::DeSerialize(std::stringstream& data) {
	
	(void)(data);
}

void Attack::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartObject();

	writer->Key("component");
	writer->String("Attack");

	writer->Key("power");
	writer->String(std::to_string(attack_power_).c_str());

	writer->EndObject();
}