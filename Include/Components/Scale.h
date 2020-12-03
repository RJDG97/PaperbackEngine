/**********************************************************************************
*\file         Scale.h
*\brief        Contains declaration of functions and variables used for
*			   the Scale Component
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#ifndef _SCALE_H_
#define _SCALE_H_

#include "Entity/Entity.h"
#include "MathLib/Vector2D.h"
#include <sstream>
//#include "Graphics.h"

class Scale : public Component { // double check friend class afterwards
	Vector2D scale_;
public:
	friend class GraphicsSystem;
	friend class Collision;

/******************************************************************************/
/*!
  \fn Scale()

  \brief Constructor for Scale that defaults the data members of the
		 component
*/
/******************************************************************************/
	Scale();

/******************************************************************************/
/*!
  \fn Init()

  \brief Scale will add itself to a relevant system's map 
*/
/******************************************************************************/
	void Init();

/******************************************************************************/
/*!
  \fn GetScale()

  \brief Returns the scale factor of the component
*/
/******************************************************************************/
	Vector2D GetScale() const;

/******************************************************************************/
/*!
  \fn SetScale()

  \brief Sets the scale factor of the component
*/
/******************************************************************************/
	void SetScale(const Vector2D& new_scale);

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
  \fn DeSerializeClone()

  \brief Reads data from a stringstream and stores them into the data members
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
  \fn ~Scale()

  \brief Destructor for Scale that defaults the data members of the
		 component
*/
/******************************************************************************/
	~Scale();
};

#endif