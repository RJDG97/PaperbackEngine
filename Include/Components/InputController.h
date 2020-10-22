#ifndef _INPUT_CONTROLLER_H_
#define _INPUT_CONTROLLER_H_

#include "Entity/Entity.h" 
#include "MathLib/Vector2D.h"
#include "IComponent.h"
#include <sstream>
#include <memory>
#include <map>

class InputController : public Component {
	
	using InputMapType = std::map<std::string, size_t>;
	using InputMapTypeIt = InputMapType::iterator;
	InputMapType input_map_;
	size_t num_entries_;

public:
	friend class Game;
	friend class PlayState;
	friend class MenuState;

/******************************************************************************/
/*!
  \fn InputController()

  \brief Constructor for InputController that defaults the data members of the
		 component
*/
/******************************************************************************/
	InputController();

/******************************************************************************/
/*!
  \fn ~InputController()

  \brief Destructor for InputController that delinks the components from
		 any system that it is attached to
*/
/******************************************************************************/
	~InputController();

/******************************************************************************/
/*!
  \fn Init()

  \brief Adds the component itself to necessary systems
*/
/******************************************************************************/
	void Init();

/******************************************************************************/
/*!
  \fn VerifyKey()

  \brief Verifies if a key exists in the input map and if so if it matches
*/
/******************************************************************************/
	bool VerifyKey(const std::string& name, const size_t value);

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

  \brief Retrieves data that will initialise data that can be unique from other
		 components
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
};

#endif