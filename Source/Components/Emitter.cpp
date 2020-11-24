#include "Components/Emitter.h"
#include "Components/Transform.h"
#include "Manager/ForcesManager.h"
#include "Manager/ParticleManager.h"
#include "Manager/ComponentManager.h"
#include "Engine/Core.h"

#include <glm/gtc/random.hpp>


Emitter::Emitter() :
	lifetime_{},
	alive_{ false },
	request_{},
	interval_{},
	default_interval_{},
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

	writer->Key("lifetime range");
	writer->String((std::to_string(lifetime_range_.x) + std::to_string(lifetime_range_.y)).c_str());

	writer->Key("min pos");
	writer->String((std::to_string(min_pos_.x) + std::to_string(min_pos_.y)).c_str());
	
	writer->Key("max pos");
	writer->String((std::to_string(max_pos_.x) + std::to_string(max_pos_.y)).c_str());
	
	writer->Key("force range");
	writer->String((std::to_string(force_range_.x) + std::to_string(force_range_.y)).c_str());
	
	writer->Key("direction range");
	writer->String((std::to_string(direction_range_.x) + std::to_string(direction_range_.y)).c_str());

	writer->Key("max spawn");
	writer->String(std::to_string(max_spawn_).c_str());

	writer->EndObject();
}

void Emitter::DeSerialize(std::stringstream& data) {
	
	(void)data;
}


void Emitter::DeSerializeClone(std::stringstream& data) {

	data >> lifetime_ >> request_ >> interval_ >> default_interval_
		>> lifetime_range_.x >> lifetime_range_.y
		>> min_pos_.x >> min_pos_.y >> max_pos_.x >> max_pos_.y
		>> force_range_.x >> force_range_.y
		>> direction_range_.x >> direction_range_.x >> max_spawn_;

	alive_ = true;
}


std::shared_ptr<Component> Emitter::Clone() {

	return std::make_shared<Emitter>();	
}


void Emitter::SetParticle(const EntityID& id) {

	std::shared_ptr<ComponentManager> component_manager = CORE->GetManager<ComponentManager>();
	std::shared_ptr<ForcesManager> forces_manager = CORE->GetManager<ForcesManager>();

	Particle* particle = component_manager->GetComponent<Particle>(id);
	Transform* xform = component_manager->GetComponent<Transform>(id);
	Motion* motion = component_manager->GetComponent<Motion>(id);
	TextureRenderer* texture_renderer= component_manager->GetComponent<TextureRenderer>(id);

	if (!particle || !xform)
		return;

	// Rand lifetime of particle
	float lifetime = glm::linearRand(lifetime_range_.x, lifetime_range_.y);

	// Rand spawn position
	Vector2D spawn{};
	spawn.x = glm::linearRand( min_pos_.x, max_pos_.x );
	spawn.y = glm::linearRand( min_pos_.y, max_pos_.y );
	
	xform->SetPosition(spawn + component_manager->GetComponent<Transform>(GetOwner()->GetID())->GetPosition());

	// Rand lifetime
	particle->lifetime_ = glm::linearRand(lifetime_range_.x, lifetime_range_.y);

	// Rand direction
	float direction = glm::linearRand( direction_range_.x, direction_range_.y );
	Vector2D vec = { sinf(direction), cosf(direction) };
	
	// Rand magnitude
	vec *= glm::linearRand( force_range_.x, force_range_.y );

	forces_manager->AddForce(id, "Particle", particle->lifetime_, vec);

	// Rand rotation range


	// Set parent id
	particle->spawner_ = GetOwner()->GetID();

	// Set active
	particle->alive_ = true;
	motion->alive_ = true;
	texture_renderer->alive_ = true;

	// Decrement quota
	++current_spawn_;
}


void Emitter::Spawn(float frametime) {
	
	lifetime_ -= frametime;
	interval_ -= frametime;

	if (interval_ > 0.0f)
		return;
	else {
		interval_ = default_interval_;
	}

	if (lifetime_ < 0.0f) {

		alive_ = false;
		lifetime_ = 0.0f;
		return;
	}

	// Count number of particles that can be requested
	size_t rand_val = rand() % request_;
	size_t count_to_request_ = max_spawn_ > current_spawn_ + rand_val ? 
		rand_val : 0;

	//current_spawn_ += count_to_request_;

	// Attempt to retrieve non-active particles to spawn
	std::vector<EntityID> particles{};

	CORE->GetManager<ParticleManager>()->GetParticles(particles, count_to_request_);

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