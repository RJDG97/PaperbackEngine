#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Entity/Entity.h"
#include "MathLib/Vector2D.h"
#include "Components/IComponent.h"
#include <sstream>
#include <memory>

class Transform : public Component {
	Vector2D position_;
	float rotation_;
public:
	friend class Physics;
	friend class GraphicsSystem;
	friend class Collision;
	friend class LightingSystem;
	friend class PlayState;

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
	void DeSerialize(std::stringstream& data) override;

/******************************************************************************/
/*!
  \fn SerializeClone()

  \brief Serializes data members within the Component
*/
/******************************************************************************/
	void DeSerializeClone(std::stringstream& data);

/******************************************************************************/
/*!
  \fn GetRotation()

  \brief Returns the rotation of the component
*/
/******************************************************************************/
	float GetRotation() const;

/******************************************************************************/
/*!
  \fn SetRotation()

  \brief Sets the rotation of the component
*/
/******************************************************************************/
	void SetRotation(const float& new_rot);

/******************************************************************************/
/*!
  \fn Clone()

  \brief Clones the existing component
*/
/******************************************************************************/
	std::shared_ptr<Component> Clone() override;

};
#endif