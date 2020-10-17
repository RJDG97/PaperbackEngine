#include "Entity/ComponentTypes.h"
//#include "ComponentTypes.h"
#include "../rapidjson/filereadstream.h"
//#include "lib/rapidjson/filereadstream.h"
#include "../rapidjson/document.h"
//#include "lib/rapidjson/document.h"
#include <memory>


#ifndef _COMPONENT_H_
#define _COMPONENT_H_

//forward declaration of entity class
//will be inherited by file containing Entity definition
class Entity;

class Component {

	// Pointer to the entity that owns the component
	Entity* owner_;
	ComponentTypes type_id_;

public:
	friend class Entity;

/******************************************************************************/
/*!
  \fn Init()

  \brief Initialize the component attached to the entity during compile time
*/
/******************************************************************************/
	virtual void Init() {};

/******************************************************************************/
/*!
  \fn Serialize

  \brief Retrieves the data from the stringstream to initialize data members
*/
/******************************************************************************/
	virtual void Serialize(std::stringstream& data) { (void)data; };

/******************************************************************************/
/*!
  \fn GetOwner()

  \brief Returns a pointer to the entity that owns the component
*/
/******************************************************************************/
	Entity* GetOwner() { return owner_; }

/******************************************************************************/
/*!
  \fn GetComponentTypeID()

  \brief Returns the ID type of the component
*/
/******************************************************************************/
	ComponentTypes GetComponentTypeID() { return type_id_; }

/******************************************************************************/
/*!
  \fn Clone()

  \brief Clones the existing component
*/
/******************************************************************************/
	virtual std::shared_ptr<Component> Clone() { return nullptr; };

protected:
/******************************************************************************/
/*!
  \fn ~Component()

  \brief Virtual destructor so all derived components will have their destructors
         called
*/
/******************************************************************************/
	virtual ~Component() = default;
};

#endif