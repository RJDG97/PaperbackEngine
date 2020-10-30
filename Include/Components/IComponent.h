#include "Entity/ComponentTypes.h"
#include "../rapidjson/filereadstream.h"
#include "../rapidjson/document.h"
#include "prettywriter.h"
#include <memory>

#ifndef _COMPONENT_H_
#define _COMPONENT_H_

// Forward declaration of Entity class
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
  \fn Serialize()

  \brief Serialises a component into JSON format
*/
/******************************************************************************/
	virtual void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) { (void)writer; };

/******************************************************************************/
/*!
  \fn SerializeClone()

  \brief Serialises a component as a clone into JSON format
*/
/******************************************************************************/
	virtual void SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) { (void)writer; };

/******************************************************************************/
/*!
  \fn DeSerialize()

  \brief Loads a component from JSON format
*/
/******************************************************************************/
	virtual void DeSerialize(std::stringstream& data) { (void)data; };

/******************************************************************************/
/*!
  \fn SerializeClone()

  \brief Retrieves data that will initialise data that can be unique from other
		 components
*/
/******************************************************************************/
	virtual void DeSerializeClone(std::stringstream& data) { (void)data; };

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