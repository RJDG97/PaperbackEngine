#pragma once
#include "Entity/ComponentTypes.h"

class Component;

class IComponentCreator {

	ComponentTypes type_id_;
public:

/******************************************************************************/
/*!
  \fn ComponentCreator()

  \brief Instantiate component with it's Type ID
*/
/******************************************************************************/
	IComponentCreator(ComponentTypes type_id) : type_id_(type_id)
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
	virtual std::shared_ptr<Component> Create() = 0;

	virtual ~IComponentCreator() = default;
};


template <typename type>
class ComponentCreator : public IComponentCreator {
public:

/******************************************************************************/
/*!
  \fn ComponentCreator()

  \brief Instantiate component with it's Type ID
*/
/******************************************************************************/
	ComponentCreator(ComponentTypes id) : IComponentCreator(id) 
	{}

/******************************************************************************/
/*!
  \fn Create()

  \brief Creates and returns a new instance of the component that the creator
		 is templated for
*/
/******************************************************************************/
	virtual std::shared_ptr<Component> Create() override {
		return std::make_shared<type>();
	}
};