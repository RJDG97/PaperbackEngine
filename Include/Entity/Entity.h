#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "Components/IComponent.h"
#include "Components/Name.h"
#include "prettywriter.h"
#include <vector>
#include <memory>
#include <string>

//enum class EntityTypes;


using EntityID = size_t;
using ComponentArr = std::vector<std::shared_ptr<Component>>;

// defines a single entity that owns components
// aka gameobject in Unity terms
class Entity {
	using EntityIt = std::vector<std::shared_ptr<Component>>::iterator;

	// Vector of components attached to the entity
	ComponentArr components_;
	// Unique ID of an entity (Begins at 1)
	EntityID object_id_;
	//EntityTypes entity_type_;

/******************************************************************************/
/*!
  \fn Entity()

  \brief Constructor for Entity, defaults the data members
*/
/******************************************************************************/
	Entity();

/******************************************************************************/
/*!
  \fn ~Entity()

  \brief Destructor for Entity, ensures that all components that are assigned 
		 are cleaned up
*/
/******************************************************************************/
	~Entity();
public:

	friend class EntityFactory;

/******************************************************************************/
/*!
  \fn GetComponent()

  \brief Returns a specific component by base pointer if the entity has it, 
		 otherwise a nullptr will be returned
*/
/******************************************************************************/
	std::shared_ptr<Component> GetComponent(ComponentTypes type_id);

/******************************************************************************/
/*!
  \fn GetComponentType()

  \brief A version of GetComponent that returns a dynamic_cast component
*/
/******************************************************************************/
	template <typename return_type>
	std::shared_ptr<return_type> GetComponentType(ComponentTypes id);

/******************************************************************************/
/*!
  \fn Init()

  \brief Initialises all components attached to the entity and 
		 sets the owner pointer of the components
*/
/******************************************************************************/
	void Init();

/******************************************************************************/
/*!
  \fn InitArchetype()

  \brief Sets the owner of all components attached to a archetype entity 
*/
/******************************************************************************/
	void InitArchetype();

/******************************************************************************/
/*!
  \fn Destroy()

  \brief Used for properly destroying an entity by adding it to the factory's
		 destroy list, which will destroy the entity in the next game loop
*/
/******************************************************************************/
	void Destroy();

/******************************************************************************/
/*!
  \fn AddComponent()

  \brief Adds a component to the current entity
*/
/******************************************************************************/
	void AddComponent(ComponentTypes typeId, std::shared_ptr<Component> component);

/******************************************************************************/
/*!
  \fn Serialize()

  \brief Serialises a entity as an archetype
*/
/******************************************************************************/
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer);

/******************************************************************************/
/*!
  \fn GetID()

  \brief Returns the entity id of the entity
*/
/******************************************************************************/
	EntityID GetID() { return object_id_; };

/******************************************************************************/
/*!
  \fn GetType()

  \brief Returns the entity type of the entity
*/
/******************************************************************************/
	//EntityTypes GetType() { return entity_type_; };

/******************************************************************************/
/*!
  \fn Clone()

  \brief Clones a entity in its entirety, down to the components that it 
		 contains
*/
/******************************************************************************/
	Entity* Clone();
};

/******************************************************************************/
/*!
  \fn StringToComponentType()

  \brief Function that converts input string from JSON into an equivalent enum
*/
/******************************************************************************/
ComponentTypes StringToComponentType(const std::string str);

//Assumes that name component is a guaranteed component for every entity
#define ENTITYNAME(ref) std::dynamic_pointer_cast<Name>(ref->GetComponent(ComponentTypes::NAME))->GetEntityName()

#endif