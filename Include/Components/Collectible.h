/**********************************************************************************
*\file         Collectible.h
*\brief        Contains declaration of functions and variables used for
*			   the Collectible Component
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#ifndef _COLLECTIBLE_H_
#define _COLLECTIBLE_H_

#include <memory>
#include <string>
#include <sstream>
#include "MathLib/Vector2D.h"
#include "Components/IComponent.h"


enum class CollectibleType
{
	NONE = 0,
	PUDDLE,
	SPORE,
	KEY
};

/******************************************************************************/
/*!
  \fn StringToCollectible()

  \brief Converts the item's name into an ENUM
*/
/******************************************************************************/
const CollectibleType StringToCollectible(const std::string& type);


class Collectible : public Component {

public:

	friend class Inventory;

	/******************************************************************************/
	/*!
	  \fn Collectible()

	  \brief Constructor for Collectible that defaults the data members of the
			 component
	*/
	/******************************************************************************/
	Collectible();

	/******************************************************************************/
	/*!
	  \fn ~Collectible()

	  \brief Destructor for Collectible that removes the component
	*/
	/******************************************************************************/
	~Collectible();

	/******************************************************************************/
	/*!
	  \fn Init()

	  \brief Adds the component itself to the inventory map
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
	  \fn GetItemType()

	  \brief Get the item type
	*/
	/******************************************************************************/
	const CollectibleType& GetItemType() const;

private:

	std::string item_name_;
	std::string item_description_;
	CollectibleType item_type_;
};


#endif