/**********************************************************************************
*\file         Unlockable.h
*\brief        Contains declaration of functions and variables used for
*			   the Unlockable Component
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#ifndef _UNLOCKABLE_H_
#define _UNLOCKABLE_H_

#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include "MathLib/Vector2D.h"
#include "Components/IComponent.h"

class Unlockable : public Component {

public:

	/******************************************************************************/
	/*!
	  \fn Unlockable()

	  \brief Constructor for Unlockable that defaults the data members of the
			 component
	*/
	/******************************************************************************/
	Unlockable();

	/******************************************************************************/
	/*!
	  \fn ~Unlockable()

	  \brief Destructor for Unlockable that removes the component
	*/
	/******************************************************************************/
	~Unlockable();

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
	  \fn GetRequiredItems()

	  \brief Gets a list of required collectibles for a particular "event"
	*/
	/******************************************************************************/
	const std::vector<std::string>& GetRequiredItems() const;

private:

	size_t count_;
	std::vector<std::string> required_items_;
};
#endif