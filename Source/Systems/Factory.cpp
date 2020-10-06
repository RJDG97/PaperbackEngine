#include "Systems/Factory.h"
#include "Entity/Entity.h"
#include "Entity/ComponentCreator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include "Systems/Debug.h"

#include "Components/Scale.h"
#include "Components/Status.h"
#include "Components/Health.h"
#include "Components/PointLight.h"
#include "Components/AABB.h"
#include "Components/Transform.h"
#include "Components/Motion.h"
#include "Components/Status.h"
#include "COmponents/BasicAI.h"

EntityFactory* FACTORY = NULL;

EntityFactory::EntityFactory() {

	FACTORY = this;
	debug_ = false;
	last_entity_id_ = 0;
}

EntityFactory::~EntityFactory() {

	ComponentMapType::iterator it = component_map_.begin();

	//loops through and deleles every component creator
	for (; it != component_map_.end(); ++it) {

		delete it->second;
	}
}

void EntityFactory::Init() {

	FACTORY->AddComponentCreator("Transform", new ComponentCreatorType<Transform>(ComponentTypes::TRANSFORM));
	FACTORY->AddComponentCreator("Health", new ComponentCreatorType<Health>(ComponentTypes::HEALTH));
	FACTORY->AddComponentCreator("Motion", new ComponentCreatorType<Motion>(ComponentTypes::MOTION));
	FACTORY->AddComponentCreator("AABB", new ComponentCreatorType<AABB>(ComponentTypes::AABB));
	FACTORY->AddComponentCreator("Scale", new ComponentCreatorType<Scale>(ComponentTypes::SCALE));
	FACTORY->AddComponentCreator("Status", new ComponentCreatorType<Status>(ComponentTypes::STATUS));
	FACTORY->AddComponentCreator("Health", new ComponentCreatorType<Health>(ComponentTypes::HEALTH));
	FACTORY->AddComponentCreator("PointLight", new ComponentCreatorType<PointLight>(ComponentTypes::POINTLIGHT));
	FACTORY->AddComponentCreator("BasicAI", new ComponentCreatorType<BasicAI>(ComponentTypes::BASICAI));

	M_DEBUG->WriteDebugMessage("EntityFactory System Init\n");
}

Entity* EntityFactory::Create(const std::string& filename) {
	(void)(filename);

	// Create the entity and initialize all components from file

	//SerializeArchetype("Player", EntityTypes::Player);

	// Removing temporarily, gonna replace with some clone of archetype
	//Entity* entity = SerializeArchetype(filename, "Player", EntityTypes::Player);
	
	//TestBuild(); //BuildAndSerialize(filename);

	// If entity was successfully created, initialize
	/*if (entity) {
		entity->Init();
	}*/

	// Removing temporarily
	//return entity;
	return nullptr;
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
			M_DEBUG->WriteDebugMessage("Deleting entity id: " + std::to_string(beginIt->first) + "\n");
			delete entity;
			entity_id_map_.erase(beginIt);
		}
	}
	// Clear and resize the set
	objects_to_delete.clear();
}

void EntityFactory::DestroyAllEntities() {
	M_DEBUG->WriteDebugMessage("Deleting all entities!\n");
	EntityIdMapType::iterator it = entity_id_map_.begin();

	// Loop through all entities
	for (; it != entity_id_map_.end(); ++it) {
		// Log id of entity that is being deleted
		M_DEBUG->WriteDebugMessage("Deleting entity id: " + std::to_string(it->first) + "\n");
		// Delete all entities
		delete it->second;
	}
	// Clear and resize map
	entity_id_map_.clear();

	//clean up archetypes
	
	for (EntityArchetypeMapTypeIt it2 = entity_archetype_map_.begin(); it2 != entity_archetype_map_.end(); ++it2) {
		// Log EntityType of entity that is being deleted (Archetype)
		std::stringstream str;
		str << "Deleting entity of type: " << typeid(it2->first).name() << "\n";
		M_DEBUG->WriteDebugMessage(str.str());
		// Delete all entities
		delete it2->second;
	}

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

//serialises single archetype 
Entity* EntityFactory::CreateAndSerializeArchetype(const std::string& filename, const std::string& entity_name, EntityTypes id) {
	// Create an empty entity
	Entity* archetype = new Entity();

	archetype->entity_type_ = id;

	// Load the input file (.json) and ensure it is open
	std::ifstream input_file(filename.c_str()/*"TestJSON/2compTest.json"*/);
	//assert(input_file);
	DEBUG_ASSERT(input_file.is_open(), "File does not exist");

	M_DEBUG->WriteDebugMessage("Creating a new entity of type: " + entity_name + "\n");

	// Read each line separated by a '\n' into a stringstream
	std::stringstream json_doc_buffer;
	std::string input;

	while (std::getline(input_file, input)) {

		json_doc_buffer << input << "\n";
	}

	// Close the file (.json) after
	input_file.close();

	// Parse the stringstream into document (DOM) format
	rapidjson::Document doc;
	doc.Parse(json_doc_buffer.str().c_str());

	// Treats entire filestream at index as array and ensure that it is an array
	const rapidjson::Value& value_arr = doc[entity_name.c_str()];
	DEBUG_ASSERT(value_arr.IsArray(), "Entry does not exist in JSON");

	// Iterate through the body of the "Header"
	for (rapidjson::Value::ConstValueIterator it = value_arr.Begin(); it != value_arr.End(); ++it) {

		// Each value is essentially a container for multiple members
		// IsObject enforces that the member is an object that will contain data:key pairs
		const rapidjson::Value& member = *it;
		
		DEBUG_ASSERT(member.IsObject(), "Entry does not exist in JSON");

		ComponentCreator* creator;
		Component* component;

		// For every new component in the "Entity"s body
		if (member.MemberBegin()->name == "component") {

			// Check whether the component's name has been registered
			//component_map_.find(member.MemberBegin()->value.GetString())->second;
			ComponentMapType::iterator component_it = component_map_.find(member.MemberBegin()->value.GetString());

			DEBUG_ASSERT((component_it != component_map_.end()), "Component Creator does not exist");

			creator = component_it->second;
			
			component = creator->Create();

			//stores the data into a stream that is easier to read data from
			std::stringstream stream;

			for (rapidjson::Value::ConstMemberIterator it2 = ++member.MemberBegin(); it2 != member.MemberEnd(); ++it2) {

				stream << it2->value.GetString() << " ";
			}

			//passes the converted data to the component to read
			component->Serialize(stream);

			// Attaches the component to the entity
			archetype->AddComponent(creator->GetComponentTypeID(), component);
		}
		
		/*
		for (rapidjson::Value::ConstMemberIterator it2 = member.MemberBegin(); it2 != member.MemberEnd(); ++it2) {

			//each member contains multiple data:key pairs that can be read and interpreted
			std::cout << it2->name.GetString() << ": " << it2->value.GetString() << std::endl;

			//component->Serialize();

				//ComponentCreator* creator = component_map_.find("Motion")->second;
				//Component* component = creator->Create();
				//ret->AddComponent(creator->GetComponentTypeID(), component);
		}
		*/
	}

	//entity_archetype_map_[id] = archetype;

	//adds entity to entity_id_map_
	//ideally to use archetype design so remove in favor of that once implementation is done
	StoreEntityID(archetype);
	
	// Sets the owner of the component to be "archetype" (*this) and adds the relevant
	// component pointers to their main systems
	archetype->Init();

	return archetype;
}


void EntityFactory::StoreEntityID(Entity* entity) {
	
	//increment counter and store entity into id map
	++last_entity_id_;
	entity->object_id_ = last_entity_id_;

	entity_id_map_[last_entity_id_] = entity;
	
	M_DEBUG->WriteDebugMessage("Storing entity with ID: " + std::to_string(last_entity_id_) + "\n");
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
	case MessageIDTypes::FTY_PURGE: // Delete all entities
	{

		DestroyAllEntities();
		break;
	}
	case MessageIDTypes::FTY_DELETE: // Delete a specific entity stored within the message
	{

		Entity_Message* m = dynamic_cast<Entity_Message*>(msg);
		EntityIdMapTypeIt ent = entity_id_map_.find(m->entity_one_);
		Destroy(&*ent->second);
		break;
	}
	case MessageIDTypes::DEBUG_ALL:
	{

		debug_ = true;
	}
	default:
		break;
	}
}