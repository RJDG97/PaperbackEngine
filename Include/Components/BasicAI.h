/**********************************************************************************
*\file         BasicAI.h
*\brief        Contains declaration of functions and variables used for
*			   the BasicAI Component
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#ifndef _BASICAI_H_
#define _BASICAI_H_

#include "Entity/Entity.h" 
#include "MathLib/Vector2D.h"
#include <sstream>
#include <vector>

class BasicAI : public Component {
	
	float speed;
	size_t num_destinations_;
	using DestinationIt = std::vector<Vector2D>::iterator;
	std::vector<Vector2D> destinations_;
	DestinationIt current_destination_;

public:
	friend class PlayState;
	friend class EditorState;

/******************************************************************************/
/*!
  \fn BasicAI()

  \brief Constructor for BasicAI that defaults the data members of the
		 component
*/
/******************************************************************************/
	BasicAI();

/******************************************************************************/
/*!
  \fn ~BasicAI()

  \brief Destructor for BasicAI that removes the component from the
		 Game system AI component map
*/
/******************************************************************************/
	~BasicAI();

/******************************************************************************/
/*!
  \fn Init()

  \brief Adds the component itself to the Game system AI map
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

  \brief Retrieves data that will initialise data that can be unique from other
		 components
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
  \fn GetDestinations()

  \brief Get destinations
*/
/******************************************************************************/
	std::vector<Vector2D>& GetDestinations();

/******************************************************************************/
/*!
  \fn SetDestinations()

  \brief Set destinations
*/
/******************************************************************************/
	void SetDestinations(std::vector<Vector2D> des);

/******************************************************************************/
/*!
  \fn GetCurrentDes()

  \brief Get the current destination
*/
/******************************************************************************/
	DestinationIt GetCurrentDes();

/******************************************************************************/
/*!
  \fn SetCurrentDes()

  \brief Set the current destination
*/
/******************************************************************************/
	void SetCurrentDes(DestinationIt Cdes);
	
/******************************************************************************/
/*!
  \fn Update()

  \brief Handles the updating of the component's logic
*/
/******************************************************************************/
	void Update(float frametime);
};

#endif