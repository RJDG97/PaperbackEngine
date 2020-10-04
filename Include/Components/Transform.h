#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Entity/Entity.h"
#include "MathLib/Vector2D.h"
#include <sstream>

class Transform : public Component {
	Vector2D position_;
	float rotation_;
public:
	friend class Physics;
	friend class GraphicsSystem;
	friend class Collision;
	friend class PointLight;

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

  \brief Reads data from a stringstream and stores them into the data members
*/
/******************************************************************************/
	void Serialize(std::stringstream& data) override;
};

#endif