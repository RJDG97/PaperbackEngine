/**********************************************************************************
*\file         ForcesManager.h
*\brief        Contains declaration of functions and variables used for
*			   the Forces Manager
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _FORCES_MANAGER_H_
#define _FORCES_MANAGER_H_

#include <map>
#include <vector>
#include <string>
#include "Entity/Entity.h"
#include "MathLib/Vector2D.h"
#include "Manager/IManager.h"
#include "Components/Motion.h"

class ForcesManager : public IManager
{
public:
	struct Force {

		std::string name_;
		float age_;
		float lifespan_;
		Vector2D force_;

		Force(const std::string& name, const float& lifespan, const Vector2D& force);
	};

	using ForceVec = std::vector<Force>;
	using ForceVecIt = std::vector<Force>::iterator;

	struct EntityForces {

		ForceVec vec_;
		Vector2D sum_of_forces_;

		bool VerifyForce(const std::string& name);
	};

	using ForceMapIt = std::map<EntityID, EntityForces>::iterator;

private:
	
	std::map<EntityID, EntityForces> force_map_;



public:

/******************************************************************************/
/*!
	\fn ForcesManager()

	\brief Default constructor for Texture
*/
/******************************************************************************/
	ForcesManager() = default;

/******************************************************************************/
/*!
	\fn AddForce()

	\brief Adds a Force to a vector of applied forces active on a Motion component
*/
/******************************************************************************/
	void AddForce(EntityID id, const std::string& name, const float& lifespan, const Vector2D& force);

/******************************************************************************/
/*!
	\fn GetForce()

	\brief Returns the force applied onto an entity
*/
/******************************************************************************/
	Vector2D GetForce(EntityID id);

/******************************************************************************/
/*!
	\fn Init()

	\brief Initialize data in force manager
*/
/******************************************************************************/
	void Init() override;

/******************************************************************************/
/*!
	\fn Update()

	\brief Update all force components within the manager, remove all forces
		   that have reached the end of their lifetime
*/
/******************************************************************************/
	void Update(float frametime) override;

/******************************************************************************/
/*!
	\fn Purge()

	\brief Remove all force entries, used on destroying all entities
*/
/******************************************************************************/
	void Purge() ;

/******************************************************************************/
/*!
	\fn ~ForcesManager()

	\brief Virtual destructor
*/
/******************************************************************************/
	virtual ~ForcesManager() = default;
};

#endif