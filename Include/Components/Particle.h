#pragma once
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "Entity/Entity.h"
#include "Components/IComponent.h"
#include <sstream>
#include <memory>

class Particle : public Component {

	bool alive_;
	float lifetime_;
	float initial_lifetime_;
	EntityID spawner_;

public:
	friend class Emitter;
	friend class ParticleSystem;
	friend class ParticleManager;

/******************************************************************************/
/*!
  \fn Particle()

  \brief Constructor for Particle
*/
/******************************************************************************/
	Particle();

/******************************************************************************/
/*!
  \fn ~Particle()

  \brief Destructor for Particle
*/
/******************************************************************************/
	~Particle();

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
  \fn Clone()

  \brief Clones the existing component
*/
/******************************************************************************/
	std::shared_ptr<Component> Clone() override;

/******************************************************************************/
/*!
  \fn IsAlive()

  \brief Retrieve the not dead status
*/
/******************************************************************************/
	bool IsAlive() const;

/******************************************************************************/
/*!
  \fn GetLifetime()

  \brief Gets the lifetime of the particle
*/
/******************************************************************************/
	float GetLifetime();

/******************************************************************************/
/*!
  \fn SetLifetime()

  \brief Sets the new lifetime of the particle
*/
/******************************************************************************/
	void SetLifetime(float lifetime);
};
#endif