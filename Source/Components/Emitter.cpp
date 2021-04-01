/**********************************************************************************
*\file         Emitter.cpp
*\brief        Contains definition of functions and variables used for
*			   the Emitter Component
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Components/Emitter.h"
#include "Components/Transform.h"
#include "Manager/ForcesManager.h"
#include "Manager/ParticleManager.h"
#include "Manager/ComponentManager.h"
#include "Systems/GraphicsSystem.h"
#include "Engine/Core.h"
#include "MathLib/Matrix3x3.h"
#include <glm/gtc/random.hpp>


Emitter::Emitter() :
	alive_{ false },
	lifetime_{},
	interval_{},
	spawn_interval_{},
	request_{},
	current_spawn_{},
	max_spawn_{}
{  }


Emitter::~Emitter() {

	CORE->GetManager<ComponentManager>()->RemoveComponent<Emitter>(Component::GetOwner()->GetID());
}


void Emitter::Init() {

	CORE->GetManager<ComponentManager>()->AddComponent<Emitter>(Component::GetOwner()->GetID(), this);
}


void Emitter::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartObject();

	writer->Key("component");
	writer->String("Emitter");

	writer->EndObject();
}


void Emitter::SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartObject();

	writer->Key("component");
	writer->String("Emitter");

	writer->Key("ui");
	writer->String(std::to_string(ui_).c_str());

	writer->Key("alive");
	writer->String(std::to_string(alive_).c_str());

	writer->Key("emitter lifetime");
	writer->String(std::to_string(lifetime_).c_str());

	writer->Key("emitter spawn interval");
	writer->String(std::to_string(spawn_interval_).c_str());
	
	writer->Key("emitter request");
	writer->String(std::to_string(request_).c_str());
	
	writer->Key("emitter maximum spawn");
	writer->String(std::to_string(max_spawn_).c_str());
	
	writer->Key("particle lifetime range");
	writer->String((std::to_string(particle_lifetime_.lifetime_range_.x) + " " + std::to_string(particle_lifetime_.lifetime_range_.y)).c_str());

	writer->Key("particle min pos");
	writer->String((std::to_string(particle_position_.min_pos_.x) + " " + std::to_string(particle_position_.min_pos_.y)).c_str());

	writer->Key("particle max pos");
	writer->String((std::to_string(particle_position_.max_pos_.x) + " " + std::to_string(particle_position_.max_pos_.y)).c_str());

	writer->Key("particle force range");
	writer->String((std::to_string(particle_force_.force_range_.x) + " " + std::to_string(particle_force_.force_range_.y)).c_str());

	writer->Key("particle direction range");
	writer->String((std::to_string(particle_force_.direction_range_.x) + " " + std::to_string(particle_force_.direction_range_.y)).c_str());

	writer->Key("particle rotation speed");
	writer->String((std::to_string(particle_rotation_.rotation_speed_.x) + " " + std::to_string(particle_rotation_.rotation_speed_.y)).c_str());

	writer->Key("min rotation angle range");
	writer->String((std::to_string(particle_rotation_.min_rotation_range_.x) + " " + std::to_string(particle_rotation_.min_rotation_range_.y)).c_str());

	writer->Key("max rotation angle range");
	writer->String((std::to_string(particle_rotation_.max_rotation_range_.x) + " " + std::to_string(particle_rotation_.max_rotation_range_.y)).c_str());

	writer->Key("Particle scale range");
	writer->String((std::to_string(particle_scale_.scale_range_.x) + " " + std::to_string(particle_scale_.scale_range_.y)).c_str());

	writer->Key("number of textures");
	writer->String(std::to_string(particle_texture_.number_of_textures_).c_str());

	for (auto& name : particle_texture_.texture_names_) {

		writer->Key("texture");
		writer->String(name.c_str());
	}

	writer->Key("has_destination");
	writer->String(std::to_string(particle_destination_.set_destination_).c_str());

	// Only write extra stuff if there is a destination
	if (particle_destination_.set_destination_) {
		writer->Key("destination");
		writer->String((std::to_string(particle_destination_.destination_.x) + " " + std::to_string(particle_destination_.destination_.x)).c_str());

		writer->Key("destination_time_range");
		writer->String((std::to_string(particle_destination_.time_range_.x) + " " + std::to_string(particle_destination_.time_range_.y)).c_str());
	}

	writer->EndObject();
}


void Emitter::DeSerialize(std::stringstream& data) {
	
	(void)data;
}


void Emitter::DeSerializeClone(std::stringstream& data) {

	// Initialize emitter's data
	data >> ui_ >> alive_ >> lifetime_ >> spawn_interval_ >> request_ >> max_spawn_;

	// Initialize particle's lifetime range
	data >> particle_lifetime_.lifetime_range_.x >> particle_lifetime_.lifetime_range_.y;

	// Initialize particle's spawn range
	data >> particle_position_.min_pos_.x >> particle_position_.min_pos_.y;
	data >> particle_position_.max_pos_.x >> particle_position_.max_pos_.y;

	// Initialize particle's force range
	data >> particle_force_.force_range_.x >> particle_force_.force_range_.y;
	data >> particle_force_.direction_range_.x >> particle_force_.direction_range_.y;

	// Initialize particle's rotation range
	data >> particle_rotation_.rotation_speed_.x >> particle_rotation_.rotation_speed_.y;
	data >> particle_rotation_.min_rotation_range_.x >> particle_rotation_.min_rotation_range_.y;
	data >> particle_rotation_.max_rotation_range_.x >> particle_rotation_.max_rotation_range_.y;

	// Initialize particle's scale range
	data >> particle_scale_.scale_range_.x >> particle_scale_.scale_range_.y;

	// Initialize particle's opactiy range
	data >> particle_opacity_.opacity_range_.x >> particle_opacity_.opacity_range_.y;

	// Initialize particle's texture selection
	data >> particle_texture_.number_of_textures_;

	for (size_t i = 0; i < particle_texture_.number_of_textures_; ++i) {
		
		std::string name{};
		data >> name;
		particle_texture_.texture_names_.push_back(name);
	}

	// Initialize the particle's destination selection
	data >> particle_destination_.set_destination_;

	data >> particle_destination_.destination_.x >> particle_destination_.destination_.y;
	data >> particle_destination_.time_range_.x >> particle_destination_.time_range_.y;

	//alive_ = true;
}


std::shared_ptr<Component> Emitter::Clone() {

	return std::make_shared<Emitter>();	
}


void Emitter::SetParticle(const EntityID& id) {

	// Get relevant managers
	std::shared_ptr<ComponentManager> component_manager = CORE->GetManager<ComponentManager>();
	std::shared_ptr<TextureManager> texture_manager = CORE->GetManager<TextureManager>();
	std::shared_ptr<GraphicsSystem> graphics_system = CORE->GetSystem<GraphicsSystem>();
	std::shared_ptr<ForcesManager> forces_manager = CORE->GetManager<ForcesManager>();

	// Get relevant components
	Particle* particle = component_manager->GetComponent<Particle>(id);
	UIParticle* ui_particle = component_manager->GetComponent<UIParticle>(id);
	Transform* xform_p = component_manager->GetComponent<Transform>(id);
	Scale* scale_p = component_manager->GetComponent<Scale>(id);
	Transform* xform_e = component_manager->GetComponent<Transform>(GetOwner()->GetID());
	Motion* motion = component_manager->GetComponent<Motion>(id);
	TextureRenderer* texture_renderer = component_manager->GetComponent<TextureRenderer>(id);

	// Check if all of them are valid (Might replace with DEBUG_ASSERT)
	if (!xform_p || !xform_e || !motion || !texture_renderer)
		return;

	if (particle) {

		particle_lifetime_.Generate(particle);
		particle_position_.Generate(xform_p, xform_e, false);
		particle_force_.Generate(forces_manager, particle, id);
		particle_rotation_.Generate(xform_p);
		particle_texture_.Generate(graphics_system, texture_renderer);
		particle_destination_.Init(particle);

		// Set parent id
		particle->spawner_ = GetOwner()->GetID();

		// Set active
		particle->alive_ = true;
	}
	else if (ui_particle) {
		
		particle_lifetime_.Generate(ui_particle);
		particle_position_.Generate(xform_p, xform_e, true);
		particle_force_.Generate(forces_manager, ui_particle, id);
		particle_rotation_.Generate(xform_p);
		particle_scale_.Generate(scale_p);
		particle_opacity_.Generate(texture_renderer);
		particle_texture_.Generate(graphics_system, texture_renderer);
		particle_destination_.Init(ui_particle);

		// Set parent id
		ui_particle->spawner_ = GetOwner()->GetID();

		// Set active
		ui_particle->alive_ = true;
		//component_manager->GetComponent<PointLight>(id)->SetAlive(true);
	}
	motion->alive_ = true;
	texture_renderer->alive_ = true;

	// Decrement quota
	++current_spawn_;
}

void Emitter::Spawn(float frametime) {

	(void)frametime;

	// Count number of particles that can be requested
	if (!request_)
		return;

	size_t rand_val = ui_ ? request_ : rand() % request_;
	size_t count_to_request_ = max_spawn_ > current_spawn_ + rand_val ? 
		rand_val : 0;

	// Attempt to retrieve non-active particles to spawn
	std::vector<EntityID> particles{};

	CORE->GetManager<ParticleManager>()->GetParticles(particles, count_to_request_, ui_);

	if (particles.empty())
		return;

	for (auto& id : particles) {

		SetParticle(id);
	}
}

bool Emitter::IsAlive() const {

	return alive_;
}

size_t Emitter::GetRequest() const {
	
	return request_;
}

void Emitter::SetAlive(bool status) {

	alive_ = status;
}

void Emitter::SetRequest(size_t new_request) {

	request_ = new_request;
}

float Emitter::GetLifeTime() {

	return lifetime_;
}

void Emitter::SetLifeTime(float new_lifetime) {

	lifetime_ = new_lifetime;
}

float Emitter::GetInterval() {

	return interval_;
}

float Emitter::GetSpawnInterval() {

	return spawn_interval_;
}

void Emitter::SetSpawnInterval(float new_spawn_interval) {

	spawn_interval_ = new_spawn_interval;
}

size_t Emitter::GetCurrentNumberSpawned() {

	return current_spawn_;
}

size_t Emitter::GetMaxNumberParticles() {

	return max_spawn_;
}

void Emitter::SetMaxNumberParticles(size_t particle_limit) {

	max_spawn_ = particle_limit;
}

GenerateLifetime Emitter::GetLifeTimeStruct() {

	return particle_lifetime_;
}

GeneratePosition Emitter::GetPositionStruct() {

	return particle_position_;
}

GenerateForce Emitter::GetForceStruct() {

	return particle_force_;
}

GenerateRotation Emitter::GetRotationStruct() {

	return particle_rotation_;
}

GenerateTexture Emitter::GetTextureStruct() {

	return particle_texture_;
}

void Emitter::SetLifeTimeStruct(Vector2D lifetime) {

	particle_lifetime_.lifetime_range_ = lifetime;
}

void Emitter::SetPositionStruct(Vector2D min_pos, Vector2D max_pos) {

	particle_position_.min_pos_ = min_pos;
	particle_position_.max_pos_ = max_pos;
}

void Emitter::SetForceStruct(Vector2D force_rng, Vector2D direction_rng) {

	particle_force_.force_range_ = force_rng;
	particle_force_.direction_range_ = direction_rng;
}

void Emitter::SetRotationStruct(Vector2D rot_speed, Vector2D min_rot, Vector2D max_rot) {

	particle_rotation_.rotation_speed_ = rot_speed;
	particle_rotation_.min_rotation_range_ = min_rot;
	particle_rotation_.max_rotation_range_ = max_rot;
}

void Emitter::SetTextureStruct(size_t tex_num, std::vector<std::string> textures_) {

	particle_texture_.number_of_textures_ = tex_num;
	particle_texture_.texture_names_.clear();
	particle_texture_.texture_names_ = textures_;
}

void Emitter::SetDestinationStruct(Vector2D destination, Vector2D time, bool status) {

	particle_destination_.destination_ = destination;
	particle_destination_.time_range_ = time;
	particle_destination_.set_destination_ = status;
}







// Generate particle data
void GenerateLifetime::Generate(Particle* particle) {

	float lifetime = glm::linearRand(lifetime_range_.x, lifetime_range_.y);
	particle->SetLifetime(lifetime);
}

void GenerateLifetime::Generate(UIParticle* particle) {

	float lifetime = glm::linearRand(lifetime_range_.x, lifetime_range_.y);
	particle->SetLifetime(lifetime);
}

void GeneratePosition::Generate(Transform* particle_transform, Transform* emitter_transform, bool ui) {

	if (!ui) {

		Vector2D spawn{};
		spawn.x = glm::linearRand(min_pos_.x, max_pos_.x);
		spawn.y = glm::linearRand(min_pos_.y, max_pos_.y);

		spawn += emitter_transform->GetPosition();

		particle_transform->SetPosition(spawn);
	}
	else {

		Vector2D spawn{};

		particle_transform->SetPosition(spawn);
	}
}

void GenerateForce::Generate(std::shared_ptr<ForcesManager> force_manager, Particle* particle, EntityID particle_id) {

	// Rand direction
	Vector2D vec{};
	Matrix3x3 mtx{};
	float direction = glm::linearRand(direction_range_.x, direction_range_.y);

	Mtx33RotDeg(mtx, direction);
	// Rand force
	vec.x = vec.y = glm::linearRand(force_range_.x, force_range_.y);
	vec = mtx * vec;

	force_manager->AddForce(particle_id, "Particle", particle->GetLifetime(), vec);
}

void GenerateForce::Generate(std::shared_ptr<ForcesManager> force_manager, UIParticle* particle, EntityID particle_id) {

	// Rand direction
	Vector2D vec{};
	Matrix3x3 mtx{};
	float direction = glm::linearRand(direction_range_.x, direction_range_.y);

	Mtx33RotDeg(mtx, direction);
	// Rand force
	vec.x = vec.y = glm::linearRand(force_range_.x, force_range_.y);
	vec = mtx * vec;

	force_manager->AddForce(particle_id, "Particle", particle->GetLifetime(), vec);
}

void GenerateRotation::Generate(Transform* transform) {

	// Rand rotation speed
	float texture_rotation_speed = glm::linearRand(rotation_speed_.x, rotation_speed_.y);
	transform->SetRotationSpeed(texture_rotation_speed);


	// Rand rotation range
	Vector2D rotation_range{};
	rotation_range.x = glm::linearRand(min_rotation_range_.x, max_rotation_range_.x);
	rotation_range.y = glm::linearRand(min_rotation_range_.y, max_rotation_range_.y);

	if (rotation_range.y < rotation_range.x)
		std::swap(rotation_range.y, rotation_range.x);

	transform->SetRotation(rotation_range.x + texture_rotation_speed);
	transform->SetRotationRange(rotation_range);
}

void GenerateTexture::Generate(std::shared_ptr<GraphicsSystem> graphics_system, TextureRenderer* texture) {

	// Rand texture choice
	if (!number_of_textures_)
		return;

	size_t index = number_of_textures_ == 1 ? 0 : glm::linearRand(0, static_cast<int>(number_of_textures_ - 1));
	graphics_system->ChangeTexture(texture, texture_names_[index]);
}

void GenerateScale::Generate(Scale* scale) {
	
	float updated_scale = glm::linearRand(scale_range_.x, scale_range_.y);

	scale->SetScale({ updated_scale, updated_scale });
}

void GenerateOpacity::Generate(TextureRenderer* renderer) {

	float updated_opacity = glm::linearRand(opacity_range_.x, opacity_range_.y);
	renderer->SetOpacity(updated_opacity);
}

void GenerateDestination::Generate(std::shared_ptr<ForcesManager> force_manager, Particle* particle, EntityID particle_id) {

	if (set_destination_) {
		CameraSystem* cam_sys = &*CORE->GetSystem<CameraSystem>();
		Vector2D particle_pos = CORE->GetManager<ComponentManager>()->GetComponent<Transform>(particle_id)->GetPosition();
		Vector2D particle_des = cam_sys->UIToGameCoords(destination_);
		float travel_time = glm::linearRand(time_range_.x, time_range_.y);

		Vector2D p_force = (particle_des - particle_pos * CORE->GetGlobalScale()) / travel_time;

		particle->SetLifetime(travel_time);
		particle->SetAlive(true);
		particle->SetDestination(false);
		force_manager->AddForce(particle_id, "Particle_To_Destination", travel_time, p_force);
	}
}

void GenerateDestination::Generate(std::shared_ptr<ForcesManager> force_manager, UIParticle* particle, EntityID particle_id) {

	if (set_destination_) {

		ComponentManager* c_mgr = &*CORE->GetManager<ComponentManager>();
		Motion* motion = c_mgr->GetComponent<Motion>(particle_id);
		Vector2D particle_pos = c_mgr->GetComponent<Transform>(particle_id)->GetPosition();
		Vector2D final_des{};

		for (auto& [p_id, parent] : *c_mgr->GetComponentArray<ParentChild>()) {

			//Name* name = c_mgr->GetComponent<Name>(p_id);

			if (parent->GetName() != "Watergauge") continue;

			Entity* player = CORE->GetManager<EntityManager>()->GetPlayerEntities();
			TextureRenderer* texture = c_mgr->GetComponent<TextureRenderer>(p_id);
			Transform* xform = c_mgr->GetComponent<Transform>(p_id);
			Scale* scale = c_mgr->GetComponent<Scale>(p_id);
			Health* health = c_mgr->GetComponent<Health>(player ? player->GetID() : 0);

			if (!xform || !scale || !health || !texture) return;

			Vector2D hp_scale = scale->GetScale() / static_cast<float>(health->GetMaxHealth());
			hp_scale.y = 0.0f;
			// Center of Watergauge + ( Offset to right side of Watergauge based on HP ) / Scale to convert to Screen coords
			final_des = destination_ + ((((health->GetCurrentHealth() - health->GetMaxHealth() * 0.5f) + 2) * hp_scale) / CORE->GetGlobalScale());
		}

		float travel_time = glm::linearRand(time_range_.x, time_range_.y);

		motion->SetVelocity(Vector2D{});
		Vector2D p_force = ( (final_des - particle_pos ) * CORE->GetGlobalScale() ) / travel_time;

		particle->SetAlive(true);
		particle->SetDestination(false);
		particle->SetLifetime(travel_time);
		force_manager->AddForce(particle_id, "Particle_To_Destination", travel_time, p_force * 0.25f);
	}
}

void GenerateDestination::Init(Particle* particle) {

	if (set_destination_)
		particle->SetDestination(true);
	else
		particle->SetDestination(false);
}

void GenerateDestination::Init(UIParticle* particle) {

	if (set_destination_)
		particle->SetDestination(true);
	else
		particle->SetDestination(false);
}