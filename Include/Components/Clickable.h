/**********************************************************************************
*\file         Clickable.h
*\brief        Contains declaration of functions and variables used for
*			   the Clickable Component
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#ifndef _CLICKABLE_H_
#define _CLICKABLE_H_

#include "Entity/Entity.h" 
#include "MathLib/Vector2D.h"
#include "IComponent.h"
#include <sstream>
#include <memory>

enum class ButtonStates
{
	DEFAULT = 0,
	HOVERED,
	CLICKED
};

class Clickable : public Component {
	Vector2D scale_; // double check if needed
	Vector2D top_right_;
    Vector2D bottom_left_;
	bool collided_, active_; // double check if needed
	size_t index_;
	size_t group_;

public:
	friend class Collision;

/******************************************************************************/
/*!
  \fn Clickable()

  \brief Constructor for Clickable that defaults the data members of the
		 component
*/
/******************************************************************************/
	Clickable();

/******************************************************************************/
/*!
  \fn ~Clickable()

  \brief Destructor for Clickable that removes the component from the
		 Collision system aabb map
*/
/******************************************************************************/
	~Clickable();

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

	size_t GetIndex();

/******************************************************************************/
/*!
  \fn GetActive()

  \brief Get active status
*/
/******************************************************************************/
	bool GetActive();

/******************************************************************************/
/*!
  \fn Init()

  \brief Adds the component itself to the Collision System Clickable map
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

  \brief Reads data from a stringstream and stores them into the data members
		 of cloned component
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
};

#endif