/**********************************************************************************
*\file         Physics.h
*\brief        Contains declaration of functions and variables used for
*			   the Physics System
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "Systems/ISystem.h"
#include "Entity/Entity.h"
#include "Entity/ComponentTypes.h"
#include "Entity/ComponentCreator.h"
#include "Systems/Message.h"
#include "Components/Transform.h"
#include "Components/Health.h"
#include "Systems/Factory.h"
#include "Systems/GraphicsSystem.h"
#include "Manager/ForcesManager.h"
#include "Manager/ComponentManager.h"
#include "Manager/LogicManager.h"
#include "Components/Motion.h"
#include "Components/Status.h"
#include <unordered_map>

class Physics : public ISystem
{
public:

	using MotionType = CMap<Motion>;
	using MotionIt = MotionType::MapTypeIt;

	using TransformType = CMap<Transform>;
	using TransformIt = TransformType::MapTypeIt;
	
	using StatusMapType = CMap<Status>;
	using StatusIt = StatusMapType::MapTypeIt;

	using ParticleType = CMap<Particle>;
	using ParticleIt = ParticleType::MapTypeIt;

/******************************************************************************/
/*!
  \fn ChangeVelocity()

  \brief Used to change the velocity of all components that have the Player
		 entity type
*/
/******************************************************************************/
	void ChangeVelocity(Message* m);

/******************************************************************************/
/*!
  \fn Physics()

  \brief Constructor for the Physics System
*/
/******************************************************************************/
	Physics();

/******************************************************************************/
/*!
  \fn Init()

  \brief Initialises the system
*/
/******************************************************************************/
	void Init() override;

/******************************************************************************/
/*!
  \fn Update()

  \brief Computes the new velocity for every Motion component and then
		 updates the Transform components based on that new Motion component
*/
/******************************************************************************/
	virtual void Update(float frametime) override;

/******************************************************************************/
/*!
  \fn GetName()

  \brief Returns the name of the system
*/
/******************************************************************************/
	virtual std::string GetName() override { return "Physics"; }

/******************************************************************************/
/*!
  \fn SendMessageD()

  \brief Receives messages broadcasted from Core Engine and processes it
*/
/******************************************************************************/
	void SendMessageD(Message* msg) override;

private:

	bool debug_;

	// System and manager pointers
	ForcesManager* force_mgr;
	GraphicsSystem* graphics_sys_;
	ComponentManager* component_mgr_;
	LogicManager* logic_mgr_;
	
	// Component map pointers
	TransformType* transform_arr_;
	MotionType* motion_arr_;
	StatusMapType* status_arr_;
	ParticleType* particle_arr_;

	using LogicComponentType = CMap<LogicComponent>;
	using LogicIt = LogicComponentType::MapTypeIt;
	LogicComponentType* logic_arr_;
};

/******************************************************************************/
/*!
  \fn VerifyZeroFloat()

  \brief Verifies whether a float is between EPSILON and -EPSILON
*/
/******************************************************************************/
bool VerifyZeroFloat(const float& val);

#endif