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

  \brief Reads data from a stringstream and stores them into the data members
*/
/******************************************************************************/
	void Serialize(std::stringstream& data) override;

/******************************************************************************/
/*!
  \fn Clone()

  \brief Clones the existing component
*/
/******************************************************************************/
	std::shared_ptr<Component> Clone() override;
};

#endif