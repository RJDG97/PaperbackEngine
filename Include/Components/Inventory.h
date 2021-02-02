/**********************************************************************************
*\file         Inventory.h
*\brief        Contains declaration of functions and variables used for
*			   the Inventory Component
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#ifndef _INVENTORY_H_
#define _INVENTORY_H_

#include <set>
#include <memory>
#include <string>
#include <sstream>
#include <unordered_map>
#include "Entity/Entity.h"
#include "MathLib/Vector2D.h"
#include "Components/IComponent.h"
#include "Components/Collectible.h"
#include "Components/Unlockable.h"

struct ItemDescription
{
	size_t count_;
	std::string description_;
	std::vector<EntityID> entities_;

	ItemDescription() {}

	ItemDescription(const size_t& count, const std::string& description) :
		count_{ count },
		description_{ description },
		entities_{}
	{}

	~ItemDescription() {
		entities_.clear();
	}
};

class Inventory : public Component {

public:

	using ItemName = std::string;

/******************************************************************************/
/*!
  \fn Inventory()

  \brief Constructor for Inventory that defaults the data members of the 
		 component
*/
/******************************************************************************/
	Inventory();

/******************************************************************************/
/*!
  \fn ~Inventory()

  \brief Destructor for Inventory that removes the component
*/
/******************************************************************************/
	~Inventory();

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
  \fn InsertItem()

  \brief Inserts an item into the inventory
*/
/******************************************************************************/
	void InsertItem(const Collectible& item);

/******************************************************************************/
/*!
  \fn RemoveItem()

  \brief Inserts an item into the inventory
*/
/******************************************************************************/
	void RemoveItem(const ItemName& name);

/******************************************************************************/
/*!
  \fn HasItem()

  \brief Checks whether an inventory contains an item
*/
/******************************************************************************/
	bool HasItem(const std::string& name);

/******************************************************************************/
/*!
  \fn GetInventory()

  \brief Gets the inventory of the entity
*/
/******************************************************************************/
	std::unordered_map<ItemName, ItemDescription> GetInventory();

/******************************************************************************/
/*!
  \fn GetInventorySize()

  \brief Gets the inventory of the entity
*/
/******************************************************************************/
	size_t GetInventorySize();

private:

	size_t current_capacity_;
	size_t max_capacity_;
	std::unordered_map<ItemName, ItemDescription> inventory_;
};
#endif