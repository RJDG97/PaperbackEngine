#ifndef _INVENTORY_H_
#define _INVENTORY_H_

#include "Entity/Entity.h"
#include "MathLib/Vector2D.h"
#include "Components/IComponent.h"
#include <sstream>
#include <memory>

class Inventory : public Component {
	bool has_key_;
	size_t keys_;
public:

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
  \fn HasKey()

  \brief Returns a bool value
*/
/******************************************************************************/
	bool HasKey() const;

/******************************************************************************/
/*!
  \fn SetRotation()

  \brief Sets the rotation of the component
*/
/******************************************************************************/
	void SetHasKey(const bool val = true);

/******************************************************************************/
/*!
  \fn GetKeys()

  \brief Returns the number of held keys
*/
/******************************************************************************/
	size_t GetKeys() const;

/******************************************************************************/
/*!
  \fn Clone()

  \brief Clones the existing component
*/
/******************************************************************************/
	std::shared_ptr<Component> Clone() override;

};
#endif