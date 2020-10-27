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
#include "Components/TextureRenderer.h"
#include "Components/AnimationRenderer.h"
#include "Components/TextRenderer.h"
#include "Components/Name.h"
#include "Components/Clickable.h"
#include "Components/InputController.h"

#include "Components/AI.h"

#include "prettywriter.h"

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

	FACTORY->AddComponentCreator("Transform", new ComponentCreator<Transform>(ComponentTypes::TRANSFORM));
	FACTORY->AddComponentCreator("Health", new ComponentCreator<Health>(ComponentTypes::HEALTH));
	FACTORY->AddComponentCreator("Motion", new ComponentCreator<Motion>(ComponentTypes::MOTION));
	FACTORY->AddComponentCreator("AABB", new ComponentCreator<AABB>(ComponentTypes::AABB));
	FACTORY->AddComponentCreator("Name",  new ComponentCreator<Name>(ComponentTypes::NAME));
	FACTORY->AddComponentCreator("Scale", new ComponentCreator<Scale>(ComponentTypes::SCALE));
	FACTORY->AddComponentCreator("Status", new ComponentCreator<Status>(ComponentTypes::STATUS));
	FACTORY->AddComponentCreator("BasicAI", new ComponentCreator<BasicAI>(ComponentTypes::BASICAI));
	FACTORY->AddComponentCreator("PointLight", new ComponentCreator<PointLight>(ComponentTypes::POINTLIGHT));
	FACTORY->AddComponentCreator("TextureRenderer", new ComponentCreator<TextureRenderer>(ComponentTypes::TEXTURERENDERER));
	FACTORY->AddComponentCreator("AnimationRenderer", new ComponentCreator<AnimationRenderer>(ComponentTypes::ANIMATIONRENDERER));
	FACTORY->AddComponentCreator("TextRenderer", new ComponentCreator<TextRenderer>(ComponentTypes::TEXTRENDERER));
	FACTORY->AddComponentCreator("Clickable", new ComponentCreator<Clickable>(ComponentTypes::CLICKABLE));
	FACTORY->AddComponentCreator("InputController", new ComponentCreator<InputController>(ComponentTypes::INPUTCONTROLLER));

	FACTORY->AddComponentCreator("AI", new ComponentCreator<AI>(ComponentTypes::AI));

	M_DEBUG->WriteDebugMessage("EntityFactory System Init\n");
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

	std::cout << "Purged all entities" << std::endl;
}

void EntityFactory::DestroyAllArchetypes() {
	// Clean up archetypes

	for (EntityArchetypeMapTypeIt it2 = entity_archetype_map_.begin(); it2 != entity_archetype_map_.end(); ++it2) {
		// Log EntityType of entity that is being deleted (Archetype)
		std::stringstream str;
		str << "Deleting entity of type: " << typeid(it2->first).name() << "\n";
		M_DEBUG->WriteDebugMessage(str.str());
		// Delete all entities
		delete it2->second;
	}

	entity_archetype_map_.clear();
}

Entity* EntityFactory::CreateEmptyEntity() {

	//creates entity w/ id
	//used for creating entities programmatically
	Entity* entity = new Entity();
	StoreEntityID(entity);
	return entity;
}

Entity* EntityFactory::CloneArchetype(const std::string& archetype_name) {
	
	EntityArchetypeMapTypeIt it = entity_archetype_map_.find(archetype_name);
	DEBUG_ASSERT((it != entity_archetype_map_.end()), "Archetype was not found!");

	Entity* cloned = it->second->Clone();

	M_DEBUG->WriteDebugMessage("Preparing to init cloned entity\n");

	cloned->Init();

	return cloned;
}

void EntityFactory::CreateAllArchetypes(const std::string& filename) {

	M_DEBUG->WriteDebugMessage("Beginning loading of all archetypes\n");

	rapidjson::Document doc;
	DeSerializeJSON(filename, doc);

	// Treats entire filestream at index as array and ensure that it is an array
	const rapidjson::Value& entity_arr = doc;
	DEBUG_ASSERT(entity_arr.IsObject(), "Entry does not exist in JSON");

	// Iterate through each prefab
	for (rapidjson::Value::ConstMemberIterator var_it = entity_arr.MemberBegin(); var_it != entity_arr.MemberEnd(); ++var_it) {
		
		const rapidjson::Value& value_arr = var_it->value;

		std::string prefab_name{ var_it->name.GetString() };

		M_DEBUG->WriteDebugMessage("Loading prefab from JSON of type: " + prefab_name + "\n");

		// Create an empty entity
		Entity* archetype = new Entity();

		//archetype->entity_type_ = EntityTypes::NONE; // player as placeholder to test

		// Iterate through the body of the prefab that contains components
		for (rapidjson::Value::ConstValueIterator it = value_arr.Begin(); it != value_arr.End(); ++it) {

			// Each value is essentially a container for multiple members
			// IsObject enforces that the member is an object that will contain data:key pairs
			const rapidjson::Value& member = *it;

			DEBUG_ASSERT(member.IsObject(), "Entry does not exist in JSON");

			IComponentCreator* creator;
			std::shared_ptr<Component> component;

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
				component->DeSerialize(stream);

				// Attaches the component to the entity
				archetype->AddComponent(creator->GetComponentTypeID(), component);
			}
		}

		entity_archetype_map_[var_it->name.GetString()] = archetype;

		//adds entity to entity_id_map_
		//ideally to use archetype design so remove in favor of that once implementation is done
		//StoreEntityID(archetype);

		// Sets the owner of the component to be "archetype" (*this) and adds the relevant
		// component pointers to their main systems
		archetype->InitArchetype();
	}
}

void EntityFactory::CloneLevelEntities(const std::string& filename, const std::string& archetype_name) {

	// In this case, we'll be loading multiple entries of entities, each one expected to have 
	// the same components but different values
	M_DEBUG->WriteDebugMessage("Beginning cloning and serializing of " + archetype_name + "\n");

	rapidjson::Document doc;
	DeSerializeJSON(filename, doc);

	const rapidjson::Value& ent_arr = doc;
	DEBUG_ASSERT(ent_arr.IsObject(), "Entity JSON does not exist in proper format");

	// Iterate through each prefab
	for (rapidjson::Value::ConstMemberIterator var_it = ent_arr.MemberBegin(); var_it != ent_arr.MemberEnd(); ++var_it) {

		const rapidjson::Value& value_arr = var_it->value;

		// Create an empty entity
		Entity* cloned = CloneArchetype(archetype_name);

		// Iterate through the body of the prefab that contains components
		for (rapidjson::Value::ConstValueIterator it = value_arr.Begin(); it != value_arr.End(); ++it) {

			// Each value is essentially a container for multiple members
			// IsObject enforces that the member is an object that will contain data:key pairs
			const rapidjson::Value& member = *it;
			DEBUG_ASSERT(member.IsObject(), "Entry does not exist in JSON");

			// For every new component in the "Entity"s body
			if (member.MemberBegin()->name == "component") {

				ComponentTypes comp_type = StringToComponentType(member.MemberBegin()->value.GetString());

				//stores the data into a stream that is easier to read data from
				std::stringstream stream;

				for (rapidjson::Value::ConstMemberIterator it2 = ++member.MemberBegin(); it2 != member.MemberEnd(); ++it2) {

					stream << it2->value.GetString() << " ";
				}

				cloned->GetComponent(comp_type)->DeSerializeClone(stream);
			}
		}
	}
}

//serialises level
void EntityFactory::DeSerializeLevelEntities(const std::string& filename) {

	M_DEBUG->WriteDebugMessage("Beginning loading of level entities\n");

	// Parse the stringstream into document (DOM) format
	rapidjson::Document doc;
	DeSerializeJSON(filename, doc);

	// Treats entire filestream at index as array and ensure that it is an array
	const rapidjson::Value& files_arr = doc;
	DEBUG_ASSERT(files_arr.IsObject(), "Level JSON does not exist in proper format");

	// Iterate through each prefab
	for (rapidjson::Value::ConstMemberIterator file_it = files_arr.MemberBegin(); file_it != files_arr.MemberEnd(); ++file_it) {

		std::string file_name{ file_it->value.GetString() };
		std::string archetype_name{ file_it->name.GetString() };

		M_DEBUG->WriteDebugMessage("Cloning archetype: " + archetype_name + "\n");

		CloneLevelEntities(file_name, archetype_name);
	}	
}

void EntityFactory::SerializeLevelEntities(const std::string& filename) {

	(void)filename;
	rapidjson::StringBuffer sb;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);

	std::ofstream filestream("AyyLmao.json");

	if (filestream.is_open()) {

		// Start the formatting for JSON
		writer.StartObject();

		for (EntityArchetypeMapTypeIt begin = entity_archetype_map_.begin(); begin != entity_archetype_map_.end(); ++begin) {
			
			// Begin entity
			writer.Key(begin->first.c_str());
			
			begin->second->Serialize(&writer);
		}

		//closing json
		writer.EndObject();

		filestream << sb.GetString();
	}

	filestream.close();
}

void EntityFactory::StoreEntityID(Entity* entity) {
	
	//increment counter and store entity into id map
	++last_entity_id_;
	entity->object_id_ = last_entity_id_;

	entity_id_map_[last_entity_id_] = entity;
	
	M_DEBUG->WriteDebugMessage("Storing entity with ID: " + std::to_string(last_entity_id_) + "\n");
}

void EntityFactory::AddComponentCreator(const std::string& name, IComponentCreator* creator) {
	
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
	return nullptr;
}

void EntityFactory::SendMessageD(Message* msg) {
	
	switch (msg->message_id_)
	{
	case MessageIDTypes::FTY_PURGE: // Delete all entities
	{

		DestroyAllEntities();
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

void DeSerializeJSON(const std::string& filename, rapidjson::Document& doc) {

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