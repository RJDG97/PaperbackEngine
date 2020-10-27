#ifndef _NAME_H_
#define _NAME_H_

#include "Entity/Entity.h"
#include "Components/IComponent.h"
#include <sstream>
#include <memory>

class Name : public Component {

	std::string name_;
public:

/******************************************************************************/
/*!
  \fn Name()

  \brief Constructor for Name
*/
/******************************************************************************/
	Name();

/******************************************************************************/
/*!
  \fn ~Name()

  \brief Destructor for Name
*/
/******************************************************************************/
	~Name();

/******************************************************************************/
/*!
  \fn Init()

  \brief 
*/
/******************************************************************************/
	void Init();

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
  \fn GetEntityName()

  \brief Returns the type of the entity
*/
/******************************************************************************/
	std::string GetEntityName() const;

/******************************************************************************/
/*!
  \fn Clone()

  \brief Clones the existing component
*/
/******************************************************************************/
	std::shared_ptr<Component> Clone() override;

};
#endif