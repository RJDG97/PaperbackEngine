/**********************************************************************************
*\file         Transform.h
*\brief        Contains declaration of functions and variables used for
*			   the Transform Component
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Entity/Entity.h"
#include "MathLib/Vector2D.h"
#include "Components/IComponent.h"
#include <sstream>
#include <memory>

class Transform : public Component {
	Vector2D position_;
	float rotation_;
	float rotation_speed_;
	Vector2D rotation_range_;
	Vector2D offset_;
	Vector2D aabb_offset_;
public:
	friend class Physics;
	friend class GraphicsSystem;
	friend class Collision;
	friend class LightingSystem;
	friend class PlayState;

/******************************************************************************/
/*!
  \fn Transform()

  \brief Constructor for Transform that defaults the data members of the 
		 component
*/
/******************************************************************************/
	Transform();

/******************************************************************************/
/*!
  \fn ~Transform()

  \brief Destructor for Transform that removes the component from the 
		 Physics system transform map
*/
/******************************************************************************/
	~Transform();

/******************************************************************************/
/*!
  \fn Init()

  \brief Adds the component itself to the Physics system transform map
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

  \brief Serialises a component as a clone into JSON format
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

  \brief Serializes data members within the Component
*/
/******************************************************************************/
	void DeSerializeClone(std::stringstream& data);

/******************************************************************************/
/*!
  \fn GetRotation()

  \brief Returns the rotation of the component
*/
/******************************************************************************/
	float GetRotation() const;

/******************************************************************************/
/*!
  \fn SetRotation()

  \brief Sets the rotation of the component
*/
/******************************************************************************/
	void SetRotation(const float& new_rot);

/******************************************************************************/
/*!
  \fn GetRotationSpeed()

  \brief Returns the rotation of the component
*/
/******************************************************************************/
	float GetRotationSpeed() const;

/******************************************************************************/
/*!
  \fn SetRotationSpeed()

  \brief Sets the rotation of the component
*/
/******************************************************************************/
	void SetRotationSpeed(const float& new_speed);

/******************************************************************************/
/*!
  \fn GetRotationRange()

  \brief Returns the rotation of the component
*/
/******************************************************************************/
	Vector2D GetRotationRange() const;

/******************************************************************************/
/*!
  \fn SetRotationRange()

  \brief Sets the rotation of the component
*/
/******************************************************************************/
	void SetRotationRange(const Vector2D& new_range);

/******************************************************************************/
/*!
  \fn GetPosition()

  \brief Returns the rotation of the component
*/
/******************************************************************************/
	Vector2D GetPosition() const;

/******************************************************************************/
/*!
  \fn SetRotation()

  \brief Sets the rotation of the component
*/
/******************************************************************************/
	void SetPosition(const Vector2D& new_pos);

/******************************************************************************/
/*!
  \fn GetOffset()

  \brief Returns the offset value
*/
/******************************************************************************/
	Vector2D GetOffset() const;

/******************************************************************************/
/*!
  \fn SetOffset()

  \brief Sets the offset
*/
/******************************************************************************/
	void SetOffset(const Vector2D& offset);

/******************************************************************************/
/*!
  \fn AddOffset()

  \brief Adds the offset value
*/
/******************************************************************************/
	void AddOffset(const Vector2D& offset);

/******************************************************************************/
/*!
  \fn GetAABBOffset()

  \brief Get the entity AABB offset
*/
/******************************************************************************/
	Vector2D GetAABBOffset() const;

/******************************************************************************/
/*!
  \fn SetAABBOffset(Vector2D new_offset)

  \brief Set the entity AABB offset
*/
/******************************************************************************/
	void SetAABBOffset(const Vector2D& new_offset);

/******************************************************************************/
/*!
  \fn GetOffsetAABBPos()

  \brief Returns the entity's position offset by the aabb offset
*/
/******************************************************************************/
	Vector2D GetOffsetAABBPos() const;

/******************************************************************************/
/*!
  \fn Clone()

  \brief Clones the existing component
*/
/******************************************************************************/
	std::shared_ptr<Component> Clone() override;

};
#endif