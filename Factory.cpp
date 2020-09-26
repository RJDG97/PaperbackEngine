#include "Factory.h"
#include "Entity.h"
#include "ComponentCreator.h"
#include <iostream>
#include <fstream>
#include <sstream>

/*
#include "ISerializer.h"
#include "istreamwrapper.h"
#include "reader.h"
#include "writer.h"

#include <fstream>*/

EntityFactory* FACTORY = NULL;

EntityFactory::EntityFactory() {

	FACTORY = this;
	last_entity_id_ = 0;
}

EntityFactory::~EntityFactory() {

	ComponentMapType::iterator it = component_map_.begin();

	//loops through and deleles every component creator
	for (; it != component_map_.end(); ++it) {

		delete it->second;
	}
}

Entity* EntityFactory::Create(const std::string& filename) {
	// Create the entity and initialize all components from file
	
	BuildAndSerialize(filename);
	Entity* entity = TestBuild(); //BuildAndSerialize(filename);
	// If entity was successfully created, initialize
	if (entity) {
		entity->Init();
	}

	return entity;
}


void EntityFactory::Destroy(Entity* entity) {
	// Insert the entity into the set for deletion next update loop
	objects_to_delete.insert(entity);
	std::cout << "Entity destroyed" << std::endl;
}


void EntityFactory::Update(float frametime) {
	// Begin and end iterators

	(void)frametime;
	EntityIt begin = objects_to_delete.begin();
	EntityIt end = objects_to_delete.end();

	// Loop through all entities that are to be removed and remove them
	for (; begin != end; ++begin) {
		Entity* entity = *begin;

		// Get entity's id
		EntityID id = entity->object_id_;
		// Check whether id exists within the map
		EntityIdMapTypeIt beginIt = entity_id_map_.find(id);

		// If it still exists, delete the entity
		if (beginIt != entity_id_map_.end()) {
			delete entity;
			entity_id_map_.erase(beginIt);
		}
	}
	// Clear and resize the set
	objects_to_delete.clear();
}

void EntityFactory::DestroyAllEntities() {
	
	EntityIdMapType::iterator it = entity_id_map_.begin();

	// Loop through all entities
	for (; it != entity_id_map_.end(); ++it) {
		// Delete all entities
		delete it->second;
	}
	// Clear and resize map
	entity_id_map_.clear();

	std::cout << "Purged all entities" << std::endl;
}

Entity* EntityFactory::CreateEmptyEntity() {

	//creates entity w/ id
	//used for creating entities programmatically
	Entity* entity = new Entity();
	StoreEntityID(entity);
	return entity;
}


Entity* EntityFactory::TestBuild() {

	Entity* ret = new Entity();

	ComponentCreator* creator = component_map_.find("Motion")->second;

	Component* component = creator->Create();

	ret->AddComponent(creator->GetComponentTypeID(), component);

	/*creator = _componentMap.find("Health")->second;

	component = creator->create();

	ret->AddComponent(creator->_typeId, component);*/

	creator = component_map_.find("Transform")->second;

	component = creator->Create();

	ret->AddComponent(creator->GetComponentTypeID(), component);

	creator = component_map_.find("AnimationRenderer")->second;

	component = creator->Create();

	ret->AddComponent(creator->GetComponentTypeID(), component);

	StoreEntityID(ret);

	return ret;
}

Entity* EntityFactory::BuildAndSerialize(const std::string& filename) {

	(void)filename;
	
	//Entity* built = new Entity{};

	std::ifstream input_stream("TestJSON/test.json");
	assert(input_stream);
	
	std::stringstream json_doc_buffer;
	std::string input;

	//reformats stream into format that can be parsed by rapidjson's Document class
	while (std::getline(input_stream, input)) {
		
		json_doc_buffer << input << "\n";
	}

	input_stream.close();

	rapidjson::Document doc;
	doc.Parse(json_doc_buffer.str().c_str());

	//treats entire filestream at index as array
	const rapidjson::Value& value_arr = doc["player"];
	assert(value_arr.IsArray());
	
	for (rapidjson::Value::ConstValueIterator it = value_arr.Begin(); it != value_arr.End(); ++it) {

		//each value is essentially a container for multiple members
		//IsObject enforces that the member is an object that will contain data:key pairs
		const rapidjson::Value& member = *it;
		assert(member.IsObject());
		
		for (rapidjson::Value::ConstMemberIterator it2 = member.MemberBegin(); it2 != member.MemberEnd(); ++it2) {

			//each member contains multiple data:key pairs that can be read and interpreted
			std::cout << it2->name.GetString() << std::endl;
		}
	}

	return nullptr;
}

void EntityFactory::StoreEntityID(Entity* entity) {
	
	//increment counter and store entity into id map
	++last_entity_id_;
	entity->object_id_ = last_entity_id_;

	entity_id_map_[last_entity_id_] = entity;
	std::cout << "Entity stored with ID: " << last_entity_id_ << std::endl;
}

void EntityFactory::AddComponentCreator(const std::string& name, ComponentCreator* creator) {
	
	//binds component creator to entry with component name
	component_map_.emplace(name, creator);
}

Entity* EntityFactory::GetObjectWithID(EntityID id) {
	
	EntityIdMapType::iterator it = entity_id_map_.find(id);

	// If entity id exists, return pointer to entity
	if (it != entity_id_map_.end()) {
		return it->second;
	}
	// Else, return null
	return NULL;
}

void EntityFactory::SendMessageD(Message* msg) {
	
	switch (msg->message_id_)
	{
	case MessageIDTypes::FTY_Purge:
	{

		DestroyAllEntities();
		break;
	}
	case MessageIDTypes::FTY_Delete:
	{

		Entity_Message* m = dynamic_cast<Entity_Message*>(msg);
		EntityIdMapTypeIt ent = entity_id_map_.find(m->entity_one_);
		Destroy(&*ent->second);
		break;
	}
	default:
		break;
	}
}