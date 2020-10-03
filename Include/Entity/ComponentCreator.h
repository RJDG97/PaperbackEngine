#pragma once
#include "Entity/ComponentTypes.h"

class Component;

class ComponentCreator {

	ComponentTypes type_id_;
public:

/******************************************************************************/
/*!
  \fn ComponentCreator()

  \brief Instantiate component with it's Type ID
*/
/******************************************************************************/
	ComponentCreator(ComponentTypes type_id) : type_id_(type_id)
	{}

/******************************************************************************/
/*!
  \fn GetComponentTypeID()

  \brief Returns the type id of the components that the creator will create
*/
/******************************************************************************/
	ComponentTypes GetComponentTypeID() const { return type_id_; }

/******************************************************************************/
/*!
  \fn Create()

  \brief Creates and returns a new instance of the component
*/
/******************************************************************************/
	virtual Component* Create() = 0;

	virtual ~ComponentCreator() = default;
};


template <typename type>
class ComponentCreatorType : public ComponentCreator {
public:

/******************************************************************************/
/*!
  \fn ComponentCreator()

  \brief Instantiate component with it's Type ID
*/
/******************************************************************************/
	ComponentCreatorType(ComponentTypes id) : ComponentCreator(id) 
	{}

/******************************************************************************/
/*!
  \fn Create()

  \brief Creates and returns a new instance of the component that the creator
		 is templated for
*/
/******************************************************************************/
	virtual Component* Create() {
		return new type();
	}
};