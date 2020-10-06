#ifndef _MOTION_H_
#define _MOTION_H_

#include "Entity/Entity.h" 
#include "MathLib/Vector2D.h"
#include <sstream>

class Motion : public Component {
	Vector2D velocity_;
    Vector2D acceleration_;
public:
	friend class Physics;
	friend class Collision;
	friend class PlayState;

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

  \brief Reads data from a stringstream and stores them into the data members
*/
/******************************************************************************/
	void Serialize(std::stringstream& data) override;
};

#endif