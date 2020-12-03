/**********************************************************************************
*\file         AABB.h
*\brief        Contains declaration of functions and variables used for
*			   the AABB Component
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _AABB_H_
#define _AABB_H_

#include "Entity/Entity.h" 
#include "MathLib/Vector2D.h"
#include "IComponent.h"
#include <sstream>
#include <memory>

class AABB : public Component {
	Vector2D scale_;
	Vector2D top_right_;
    Vector2D bottom_left_;
	bool collided;
	size_t layer_;
public:
	friend class Collision;

/******************************************************************************/
/*!
  \fn AABB()

  \brief Constructor for AABB that defaults the data members of the
		 component
*/
/******************************************************************************/
	AABB();

/******************************************************************************/
/*!
  \fn ~AABB()

  \brief Destructor for AABB that removes the component from the
		 Collision system aabb map
*/
/******************************************************************************/
	~AABB();

/******************************************************************************/
/*!
  \fn GetBottomLeft()

  \brief Returns the bottom left coordinates of the component
*/
/******************************************************************************/
	Vector2D GetBottomLeft() const { 
		return bottom_left_;
	}

/******************************************************************************/
/*!
  \fn GetTopRight()

  \brief Returns the top right coordinates of the component
*/
/******************************************************************************/
	Vector2D GetTopRight() const {
		return top_right_;
	}

/******************************************************************************/
/*!
  \fn SetBottomLeft()

  \brief Sets the bottom left coordinates of the component
*/
/******************************************************************************/
	void SetBottomLeft(const Vector2D bottom_left) { 
		bottom_left_ = bottom_left;
	}

/******************************************************************************/
/*!
  \fn SetTopRight()

  \brief Sets the top right coordinates of the component
*/
/******************************************************************************/
	void SetTopRight(const Vector2D& top_right) {
		top_right_ = top_right;
	}

/******************************************************************************/
/*!
  \fn Init()

  \brief Adds the component itself to the Collision system aabb map
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
	void SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer);

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
  \fn GetAABBScale()

  \brief Retrieve the entity AABB scale
*/
/******************************************************************************/
	Vector2D GetAABBScale();

/******************************************************************************/
/*!
  \fn SetAABBScale()

  \brief Set the entity AABB scale
*/
/******************************************************************************/
	void SetAABBScale(Vector2D newscale);

/******************************************************************************/
/*!
  \fn GetLayer()

  \brief Returns the layer that the component is on
*/
/******************************************************************************/
	size_t GetLayer() const;
};

#endif