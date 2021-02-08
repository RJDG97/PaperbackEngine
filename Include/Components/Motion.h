/**********************************************************************************
*\file         Motion.h
*\brief        Contains declaration of functions and variables used for
*			   the Motion Component
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#ifndef _MOTION_H_
#define _MOTION_H_

#include "Entity/Entity.h" 
#include "MathLib/Vector2D.h"
#include <sstream>

class Motion : public Component {
	Vector2D velocity_;
    Vector2D acceleration_;
	float mass_;
	float inv_mass_;
	float force_;
	bool is_left_;
	bool is_top_;
	bool alive_;

public:
	friend class Physics;
	friend class Collision;
	friend class PlayState;
	friend class Emitter;
	friend class ParticleSystem;

/******************************************************************************/
/*!
  \fn Motion()

  \brief Constructor for Motion that defaults the data members of the
		 component
*/
/******************************************************************************/
	Motion();

/******************************************************************************/
/*!
  \fn ~Motion()

  \brief Destructor for Motion that removes the component from the
		 Physics system motion map
*/
/******************************************************************************/
	~Motion();

/******************************************************************************/
/*!
  \fn Init()

  \brief Adds the component itself to the Physics system motion map
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
  \fn GetMass()

  \brief Gets mass
*/
/******************************************************************************/
	float GetMass();

/******************************************************************************/
/*!
  \fn SetMass()

  \brief Sets mass
*/
/******************************************************************************/
	void SetMass(float new_mass);

/******************************************************************************/
/*!
  \fn GetForce()

  \brief Gets force
*/
/******************************************************************************/
	float GetForce();

/******************************************************************************/
/*!
  \fn SetForce()

  \brief Sets force
*/
/******************************************************************************/
	void SetForce(float new_force);

/******************************************************************************/
/*!
  \fn IsLeft()

  \brief Get left direction bool
*/
/******************************************************************************/
	bool IsLeft();

/******************************************************************************/
/*!
  \fn SetIsLeft()

  \brief Set left direction bool
*/
/******************************************************************************/
	void SetIsLeft(bool status);

/******************************************************************************/
/*!
  \fn IsLeft()

  \brief Get left direction bool
*/
/******************************************************************************/
	bool IsTop();

/******************************************************************************/
/*!
  \fn SetIsLeft()

  \brief Set left direction bool
*/
/******************************************************************************/
	void SetIsTop(bool status);

/******************************************************************************/
/*!
  \fn GetVelocity()

  \brief Get velocity
*/
/******************************************************************************/
	Vector2D GetVelocity();

	void SetVelocity(const Vector2D& vec);
};

#endif