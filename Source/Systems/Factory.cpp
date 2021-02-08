/**********************************************************************************
*\file         Factory.cpp
*\brief        Contains definition of functions and variables used for
*			   the Factory System
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Systems/Factory.h"
#include "Engine/Core.h"
#include "Entity/Entity.h"
#include "Entity/ComponentCreator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include "Systems/Debug.h"

#include "Manager/ForcesManager.h"

#include "Components/Scale.h"
#include "Components/Status.h"
#include "Components/Health.h"
#include "Components/PointLight.h"
#include "Components/ConeLight.h"
#include "Components/AABB.h"
#include "Components/Transform.h"
#include "Components/Motion.h"
#include "Components/BasicAI.h"
#include "Components/TextureRenderer.h"
#include "Components/AnimationRenderer.h"
#include "Components/TextRenderer.h"
#include "Components/Camera.h"
#include "Components/Name.h"
#include "Components/Clickable.h"
#include "Components/InputController.h"
#include "Components/ParentChild.h"
#include "Components/LogicComponent.h"
#include "Components/Emitter.h"
#include "Components/Particle.h"
#include "Components/SoundEmitter.h"
#include "Components/Collectible.h"
#include "Components/Unlockable.h"

#include "Components/AI.h"

#include "prettywriter.h"

EntityFactory* FACTORY = NULL;

EntityFactory::EntityFactory() {

	FACTORY = this;
	debug_ = false;
	last_entity_id_ = 0;
}

EntityFactory::~EntityFactory() {

}

std::string EntityFactory::GetLevelPath(const std::string& name) {
	
	if (name == "Menu") {
		return levels_.menu_.path_;
	}
	else if (name == "Play") {

		return levels_.GetPlayLevel(levels_.current_play_index_)->path_;
	}
	else if (name == "Splash") {
		return levels_.splash_.path_;
	}
	else if (name == "Credits") {
		return levels_.credits_.path_;
	}
	else if (name == "Win") {
		return levels_.win_.path_;
	}
	else if (name == "Lose") {
		return levels_.lose_.path_;
	}
	else if (name == "Editor") {
		return levels_.editor_.path_;
	}
	else if (name == "Pause") {
		return levels_.pause_.path_;
	}

	return {};
}

Level* EntityFactory::GetLevel(const std::string& name) {

	if (name == "Menu") {
		return &levels_.menu_;
	}
	else if (name == "Play") {
		return levels_.GetPlayLevel(levels_.current_play_index_);
	}
	else if (name == "Splash") {
		return &levels_.splash_;
	}
	else if (name == "Credits") {
		return &levels_.credits_;
	}
	else if (name == "Editor") {
		return &levels_.editor_;
	}
	else if (name == "Pause") {
		return &levels_.pause_;
	}
	return nullptr;
}

Levels* EntityFactory::GetLevelsFile() {

	return &levels_;
}

void EntityFactory::Init() {

	comp_mgr_ = &*CORE->GetManager<ComponentManager>();
	entity_mgr_ = &*CORE->GetManager<EntityManager>();

	comp_mgr_->AddComponentCreator("Transform", new ComponentCreator<Transform>(ComponentTypes::TRANSFORM));
	comp_mgr_->AddComponentCreator("Health", new ComponentCreator<Health>(ComponentTypes::HEALTH));
	comp_mgr_->AddComponentCreator("Motion", new ComponentCreator<Motion>(ComponentTypes::MOTION));
	comp_mgr_->AddComponentCreator("AABB", new ComponentCreator<AABB>(ComponentTypes::AABB));
	comp_mgr_->AddComponentCreator("Name",  new ComponentCreator<Name>(ComponentTypes::NAME));
	comp_mgr_->AddComponentCreator("Scale", new ComponentCreator<Scale>(ComponentTypes::SCALE));
	comp_mgr_->AddComponentCreator("Status", new ComponentCreator<Status>(ComponentTypes::STATUS));
	comp_mgr_->AddComponentCreator("BasicAI", new ComponentCreator<BasicAI>(ComponentTypes::BASICAI));
	comp_mgr_->AddComponentCreator("PointLight", new ComponentCreator<PointLight>(ComponentTypes::POINTLIGHT));
	comp_mgr_->AddComponentCreator("ConeLight", new ComponentCreator<ConeLight>(ComponentTypes::CONELIGHT));
	comp_mgr_->AddComponentCreator("TextureRenderer", new ComponentCreator<TextureRenderer>(ComponentTypes::TEXTURERENDERER));
	comp_mgr_->AddComponentCreator("AnimationRenderer", new ComponentCreator<AnimationRenderer>(ComponentTypes::ANIMATIONRENDERER));
	comp_mgr_->AddComponentCreator("TextRenderer", new ComponentCreator<TextRenderer>(ComponentTypes::TEXTRENDERER));
	comp_mgr_->AddComponentCreator("Camera", new ComponentCreator<Camera>(ComponentTypes::CAMERA));
	comp_mgr_->AddComponentCreator("Clickable", new ComponentCreator<Clickable>(ComponentTypes::CLICKABLE));
	comp_mgr_->AddComponentCreator("InputController", new ComponentCreator<InputController>(ComponentTypes::INPUTCONTROLLER));
	comp_mgr_->AddComponentCreator("AI", new ComponentCreator<AI>(ComponentTypes::AI));
	comp_mgr_->AddComponentCreator("ParentChild", new ComponentCreator<ParentChild>(ComponentTypes::PARENTCHILD));
	comp_mgr_->AddComponentCreator("LogicComponent", new ComponentCreator<LogicComponent>(ComponentTypes::LOGICCOMPONENT));
	comp_mgr_->AddComponentCreator("Inventory", new ComponentCreator<Inventory>(ComponentTypes::INVENTORY));
	comp_mgr_->AddComponentCreator("Particle", new ComponentCreator<Particle>(ComponentTypes::PARTICLE));
	comp_mgr_->AddComponentCreator("Emitter", new ComponentCreator<Emitter>(ComponentTypes::EMITTER));
	comp_mgr_->AddComponentCreator("SoundEmitter", new ComponentCreator<SoundEmitter>(ComponentTypes::SOUNDEMITTER));
	comp_mgr_->AddComponentCreator("Collectible", new ComponentCreator<Collectible>(ComponentTypes::COLLECTIBLE));
	comp_mgr_->AddComponentCreator("Unlockable", new ComponentCreator<Unlockable>(ComponentTypes::UNLOCKABLE));
	comp_mgr_->AddComponentCreator("Interactable", new ComponentCreator<Interactable>(ComponentTypes::INTERACTABLE));
	comp_mgr_->AddComponentCreator("DialogueTrigger", new ComponentCreator<DialogueTrigger>(ComponentTypes::INTERACTABLE));

	//load the levels json here
	levels_.DeSerialize("Resources/EntityConfig/levels.json");
	levels_.DeSerializeLevels();

	M_DEBUG->WriteDebugMessage("EntityFactory System Init\n");
}

void EntityFactory::Destroy(Entity* entity) {
	// Insert the entity into the set for deletion next update loop
	//objects_to_delete.insert(entity);
	entity_mgr_->DeleteEntity(entity);
	std::cout << "Entity destroyed" << std::endl;
}

void EntityFactory::Update(float frametime) {
	// Begin and end iterators

	(void)frametime;

	// Remove all entities that are marked as delete
	entity_mgr_->UpdateEntityMap();
	entity_mgr_->UpdateArchetypeMap();
}

void EntityFactory::DestroyAllEntities() {

	entity_mgr_->DeletePlayerEntities();
	entity_mgr_->DeleteAllEntities();
	CORE->GetManager<ForcesManager>()->Purge();
}

void EntityFactory::DestroyAllArchetypes() {

	entity_mgr_->DeleteAllArchetype();
}

Entity* EntityFactory::CreateEmptyEntity() {

	return entity_mgr_->CreateEmptyEntity();
}

Entity* EntityFactory::CloneArchetype(const std::string& archetype_name) {

	return entity_mgr_->CloneArchetype(archetype_name);
}




// Serialization stuff

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
				creator = comp_mgr_->GetComponentCreator(member.MemberBegin()->value.GetString());

				DEBUG_ASSERT((creator != nullptr), "Component Creator does not exist");

				//creator = component_it->second;

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

		//entity_archetype_map_[var_it->name.GetString()] = archetype;
		entity_mgr_->AddNewArchetype(var_it->name.GetString(), archetype);

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

void EntityFactory::LoadLevel(const std::string& level_name) {

	std::string filename = GetLevelPath(level_name);

	if (level_name != "Pause")
		levels_.current_state_ = GetLevel(level_name);

	DeSerializeLevelEntities(filename);
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

	entity_mgr_->SortPlayerEntities();
}

void EntityFactory::SerializeArchetypes(const std::string& filename) {

	rapidjson::StringBuffer sb;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);

	std::ofstream filestream(filename);

	if (filestream.is_open()) {

		// Start the formatting for JSON
		writer.StartObject();

		EntityArchetypeMapType archetype_map = entity_mgr_->GetArchetypes();

		for (EntityArchetypeMapTypeIt begin = archetype_map.begin(); begin != archetype_map.end(); ++begin) {
			
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

void EntityFactory::SerializeCurrentLevelEntities() {

	// Loop through and make 1 entry per entity type
	for (Level::EntityPathsIt it = levels_.current_state_->entity_paths_.begin(); it != levels_.current_state_->entity_paths_.end(); ++it) {
		
		// Loads the path for a specific archetype
		std::ofstream filestream(it->second);

		rapidjson::StringBuffer sb;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);

		//assuming that it->first is the name of an archetype
		if (filestream.is_open()) {

			// Start the formatting for JSON
			writer.StartObject();

			EntityIdMapType map = entity_mgr_->GetEntities();

			for (EntityIdMapTypeIt begin = map.begin(); begin != map.end(); ++begin) {

				// only log if the name of the entity is the same as the current entity path key
				if (ENTITYNAME(begin->second) == it->first) {
					
					// Begin entity
					writer.Key(it->first.c_str());

					begin->second->SerializeClone(&writer);
				}
			}

			// Closing json
			writer.EndObject();

			filestream << sb.GetString();
		}

		filestream.close();
	}
}

void EntityFactory::SerializeLevelPaths() {

	levels_.SerializeLevels();
}

void EntityFactory::StoreEntityID(Entity* entity) {

	entity_mgr_->StoreEntityID(entity);
}

Entity* EntityFactory::GetObjectWithID(EntityID id) {

	return entity_mgr_->GetEntity(id);
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