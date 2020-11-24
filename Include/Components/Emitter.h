#pragma once
#ifndef _EMITTER_H_
#define _EMITTER_H_

#include "Entity/Entity.h"
#include "MathLib/Vector2D.h"
#include "Components/IComponent.h"
#include <sstream>
#include <memory>

class Emitter : public Component {

	bool alive_;				// Status of emitter
	float lifetime_;			// Lifetime of emitter
	size_t request_;			// Number of particles to request per interval
	float interval_;			// Interval that counts down every frame, spawns once it hits < 0
	float default_interval_;	// Fixed interval to spawn
	size_t current_spawn_;		// Current number of particles the emitter has spawned
	size_t max_spawn_;			// Maximum number of particles the emitter can spawn at any time

	Vector2D lifetime_range_;	 // Range for lifetime of particles
	Vector2D min_pos_, max_pos_; // Range for spawning 
    Vector2D force_range_;		 // Range for magnitude of vector
    Vector2D direction_range_;	 // Range for direction of vector (-1 to 1)

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
};

#endif