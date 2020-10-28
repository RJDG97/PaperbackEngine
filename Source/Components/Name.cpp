#include "Components/Name.h"
#include "MathLib/Vector2D.h"
#include "Engine/Core.h"
#include <iostream> 

Name::Name() : 
	name_{ }
{}

Name::~Name() {
	// Blank for now
}

void Name::Init() {
	// Blank for now
}

void Name::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartObject();

	writer->Key("component");
	writer->String("Name");

	writer->Key("name");
	writer->String(name_.c_str());

	writer->EndObject();
}

void Name::DeSerialize(std::stringstream& data) {
	/*
	rapidjson::Value::ConstMemberIterator it2 = member.MemberBegin()
	it2->name.GetString() << ": " << it2->value.GetString()
	*/
	std::cout << "Entered Serialize Name w/ stream" << std::endl;
	
	data >> name_;
}

std::string Name::GetEntityName() const {

	return name_;
}

std::shared_ptr<Component> Name::Clone() {
	M_DEBUG->WriteDebugMessage("Cloning Name Component\n");
	std::shared_ptr<Name> cloned = std::make_shared<Name>();

	cloned->name_ = name_;

	return cloned;
}

std::string Name::GetName()
{
	return name_;
}
