/**********************************************************************************
*\file         Emitter.h
*\brief        Contains declaration of functions and variables used for
*			   the Emitter Component
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#ifndef _EMITTER_H_
#define _EMITTER_H_

#include "Entity/Entity.h"
#include "MathLib/Vector2D.h"
#include "Components/IComponent.h"
#include "Manager/ForcesManager.h"
#include <algorithm>
#include <sstream>
#include <memory>

class Transform;
class Particle;
class UIParticle;
class Scale;
class TextureRenderer;
class GraphicsSystem;

// Particle customizations
struct GenerateLifetime
{
	// Range for lifetime of particles
	Vector2D lifetime_range_;

/******************************************************************************/
/*!
  \fn Generate()

  \brief Generates a particle's lifespan
*/
/******************************************************************************/
	void Generate(Particle* particle);
	void Generate(UIParticle* particle);
};

struct GeneratePosition
{
	// Range for spawning 
	Vector2D min_pos_, max_pos_;

/******************************************************************************/
/*!
  \fn Generate()

  \brief Generates a particle's position
*/
/******************************************************************************/
	void Generate(Transform* particle_transform, Transform* emitter_transform, bool ui);
};

struct GenerateForce
{
	// Range for magnitude of vector
	Vector2D force_range_;
	// Range for direction of vector (-1 to 1)
	Vector2D direction_range_;

/******************************************************************************/
/*!
  \fn Generate()

  \brief Generates a particle's force and direction
*/
/******************************************************************************/
	void Generate(std::shared_ptr<ForcesManager> force_manager, Particle* particle, EntityID particle_id);
	void Generate(std::shared_ptr<ForcesManager> force_manager, UIParticle* particle, EntityID particle_id);
};

struct GenerateRotation
{
	// Range for rotation speed of texture
	Vector2D rotation_speed_;
	// Range for rotation of texture
	Vector2D min_rotation_range_, max_rotation_range_;

/******************************************************************************/
/*!
  \fn Generate()

  \brief Generates a particle's rotation
*/
/******************************************************************************/
	void Generate(Transform* transform);

};

struct GenerateTexture
{
	// Different available textures
	size_t number_of_textures_;
	std::vector<std::string> texture_names_;

/******************************************************************************/
/*!
  \fn Generate()

  \brief Generates a particle's texture
*/
/******************************************************************************/
	void Generate(std::shared_ptr<GraphicsSystem> graphics_system, TextureRenderer* texture);
};

struct GenerateScale
{
	// Different available textures
	Vector2D scale_range_;

/******************************************************************************/
/*!
  \fn Generate()

  \brief Generates a particle's scale
*/
/******************************************************************************/
	void Generate(Scale* scale);
};

struct GenerateOpacity
{
	// Different available textures
	Vector2D opacity_range_;

	/******************************************************************************/
	/*!
	  \fn Generate()

	  \brief Generates a particle's opacity
	*/
	/******************************************************************************/
	void Generate(TextureRenderer* renderer);
};

struct GenerateDestination
{
	// Coordinate of End Destination
	Vector2D destination_;
	// Whether there is a destination
	bool set_destination_;
	// Time range it takes to reach destination
	Vector2D time_range_;


	/******************************************************************************/
	/*!
	  \fn Generate()

	  \brief Generates a particle's force post initial lifetime
	*/
	/******************************************************************************/
	void Generate(std::shared_ptr<ForcesManager> force_manager, Particle* particle, EntityID particle_id);
	void Generate(std::shared_ptr<ForcesManager> force_manager, UIParticle* particle, EntityID particle_id);

	/******************************************************************************/
	/*!
	  \fn Init()

	  \brief Initializes the particle to contain a destination
	*/
	/******************************************************************************/
	void Init(Particle* particle);
	void Init(UIParticle* particle);
};



class Emitter : public Component {

	bool ui_;							// UI Emitter
	bool alive_;						// Status of emitter
	float lifetime_;					// Lifetime of emitter
	float interval_;					// Interval that counts down every frame, spawns once it hits < 0
	float spawn_interval_;				// Fixed interval to spawn
	size_t request_;					// Number of particles to request per interval
	size_t current_spawn_;				// Current number of particles the emitter has spawned
	size_t max_spawn_;					// Maximum number of particles the emitter can spawn at any time


	GenerateLifetime particle_lifetime_;
	GeneratePosition particle_position_;
	GenerateForce particle_force_;
	GenerateRotation particle_rotation_;
	GenerateScale particle_scale_;
	GenerateOpacity particle_opacity_;
	GenerateTexture particle_texture_;
	GenerateDestination particle_destination_;

/******************************************************************************/
/*!
  \fn SetParticle()

  \brief Helper function to set a particle based on preset parameters defined
		 by the emitter
*/
/******************************************************************************/
	void SetParticle(const EntityID& id);

public:
	
	friend class ParticleSystem;
	friend class ParticleManager;

/******************************************************************************/
/*!
  \fn Emitter()

  \brief Constructor for Emitter
*/
/******************************************************************************/
	Emitter();

/******************************************************************************/
/*!
  \fn ~Emitter()

  \brief Destructor for Emitter
*/
/******************************************************************************/
	~Emitter();

/******************************************************************************/
/*!
  \fn Init()

  \brief
*/
/******************************************************************************/
	void Init();

/******************************************************************************/
/*!
  \fn Serialize()

  \brief Serialises a component into JSON format
*/
/******************************************************************************/
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) override;
	
/******************************************************************************/
/*!
  \fn SerializeClone()

  \brief Serialises a component into JSON format
*/
/******************************************************************************/
	void SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) override;

/******************************************************************************/
/*!
  \fn DeSerialize()

  \brief Reads data from a stringstream and stores them into the data members
*/
/******************************************************************************/
	void DeSerialize(std::stringstream& data) override;

/******************************************************************************/
/*!
  \fn DeSerializeClone()

  \brief Reads data from a stringstream and stores them into the data members
*/
/******************************************************************************/
	void DeSerializeClone(std::stringstream& data) override;

/******************************************************************************/
/*!
  \fn Clone()

  \brief Clones the existing component
*/
/******************************************************************************/
	std::shared_ptr<Component> Clone() override;

/******************************************************************************/
/*!
  \fn Spawn()

  \brief Spawns new active particles
*/
/******************************************************************************/
	void Spawn(float frametime);

/******************************************************************************/
/*!
  \fn IsAlive()

  \brief Retrieve the status
*/
/******************************************************************************/
	bool IsAlive() const;

/******************************************************************************/
/*!
  \fn GetRequest()

  \brief Retrieve the number of particles to request for
*/
/******************************************************************************/
	size_t GetRequest() const;

/******************************************************************************/
/*!
  \fn GetLifeTime()

  \brief Get the lifetime of the emitter
*/
/******************************************************************************/
	float GetLifeTime();
/******************************************************************************/
/*!
  \fn GetInterval()

  \brief Get the time interval of the emitter
*/
/******************************************************************************/	
	float GetInterval();

/******************************************************************************/
/*!
  \fn GetSpawnInterval()

  \brief Get the spawn interval of the emitter
*/
/******************************************************************************/
	float GetSpawnInterval();

/******************************************************************************/
/*!
  \fn GetCurrentNumberSpawned()

  \brief Get the current number of particles in the scene
*/
/******************************************************************************/
	size_t GetCurrentNumberSpawned();

/******************************************************************************/
/*!
  \fn GetMaxNumberParticles()

  \brief Get the limit of the number of particles
*/
/******************************************************************************/
	size_t GetMaxNumberParticles();

/******************************************************************************/
/*!
  \fn SetAlive(bool status)

  \brief Sets the alive status of the emitter
*/
/******************************************************************************/
	void SetAlive(bool status);

/******************************************************************************/
/*!
  \fn SetLifeTime(float new_lifetime)

  \brief Sets the lifetime of the emitter
*/
/******************************************************************************/
	void SetLifeTime(float new_lifetime);

/******************************************************************************/
/*!
  \fn SetSpawnInterval(float new_spawn_interval)

  \brief Sets the interval of the spawning of particles
*/
/******************************************************************************/
	void SetSpawnInterval(float new_spawn_interval);

/******************************************************************************/
/*!
  \fn SetMaxNumberParticles(size_t particle_limit)

  \brief Set the Max number of particles to be spawned
*/
/******************************************************************************/
	void SetMaxNumberParticles(size_t particle_limit);

/******************************************************************************/
/*!
  \fn SetRequest()

  \brief Set the number of particles to request for
*/
/******************************************************************************/
	void SetRequest(size_t new_request);

/******************************************************************************/
/*!
  \fn GetLifeTimeStruct()

  \brief Gets the GenerateLifetime struct and all the variables
*/
/******************************************************************************/
	GenerateLifetime GetLifeTimeStruct();

/******************************************************************************/
/*!
  \fn GetPositionStruct()

  \brief Gets the GeneratePosition struct and all the variables
*/
/******************************************************************************/
	GeneratePosition GetPositionStruct();

/******************************************************************************/
/*!
  \fn GetForceStruct()

  \brief Gets the GenerateForce struct and all the variables 
*/
/******************************************************************************/
	GenerateForce	 GetForceStruct();

/******************************************************************************/
/*!
  \fn GetRotationStruct()

  \brief  Gets the GenerateRotation struct and all the variables
*/
/******************************************************************************/
	GenerateRotation GetRotationStruct();

/******************************************************************************/
/*!
  \fn GenerateTexture  GetTextureStruct()

  \brief Gets the GenerateTexture struct and all the variables
*/
/******************************************************************************/
	GenerateTexture  GetTextureStruct();

/******************************************************************************/
/*!
  \fn SetLifeTimeStruct(Vector2D lifetime)

  \brief Set the variables for the GenerateLifetime struct
*/
/******************************************************************************/	
	void SetLifeTimeStruct(Vector2D lifetime);

/******************************************************************************/
/*!
  \fn SetPositionStruct(Vector2D min_pos, Vector2D max_pos)

  \brief Set the variables for the GeneratePosition struct
*/
/******************************************************************************/
	void SetPositionStruct(Vector2D min_pos, Vector2D max_pos);

/******************************************************************************/
/*!
  \fn SetForceStruct(Vector2D force_rng, Vector2D direction_rng)

  \brief Set the variables for the GenerateForces struct
*/
/******************************************************************************/
	void SetForceStruct(Vector2D force_rng, Vector2D direction_rng);

/******************************************************************************/
/*!
  \fn SetRotationStruct(Vector2D rot_speed, Vector2D min_rot, Vector2D max_rot)

  \brief Set the variables for the GenerateRotation struct
*/
/******************************************************************************/
	void SetRotationStruct(Vector2D rot_speed, Vector2D min_rot, Vector2D max_rot);

/******************************************************************************/
/*!
  \fn SetTextureStruct(size_t tex_num, std::vector<std::string> textures_)

  \brief Set the variables for the GenerateTexture struct
*/
/******************************************************************************/
	void SetTextureStruct(size_t tex_num, std::vector<std::string> textures_);

/******************************************************************************/
/*!
  \fn SetDestinationStruct()

  \brief Set the variables for the GenerateTexture struct
*/
/******************************************************************************/
	void SetDestinationStruct(Vector2D destination, Vector2D time, bool status);
};


#endif