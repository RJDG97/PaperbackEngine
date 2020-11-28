#pragma once
#ifndef _TRANSITION_MANAGER_H_
#define _TRANSITION_MANAGER_H_

#include "Manager/IManager.h"
#include "Manager/ComponentManager.h"
#include "Systems/GraphicsSystem.h"
#include "Entity/Entity.h"
#include <vector>
#include <string>


struct SceneTransition
{
	using TextureSequence = std::vector<std::string>;
	using TextureSequenceIt = TextureSequence::iterator;

	size_t size_;
	float timer_;
	float default_timer_;
	float default_transition_timer_;
	float dark_timer_;
	float default_dark_timer_;
	std::vector<std::string> texture_sequence_;
	TextureSequenceIt current_texture_;
};



class TransitionManager : public IManager
{
public:

	friend class TransitionSystem;

	using SceneTransitions = std::map<std::string, std::shared_ptr<SceneTransition>>;
	using SceneTransitionsIt = SceneTransitions::iterator;

	using TextureRendererType = CMap<TextureRenderer>;
	using TextureRendererTypeIt = TextureRendererType::MapTypeIt;


/******************************************************************************/
/*!
  \fn TransitionManager()

  \brief Transition manager constructor
*/
/******************************************************************************/
	TransitionManager();

/******************************************************************************/
/*!
  \fn Init()

  \brief Init transition manager
*/
/******************************************************************************/
	void Init() override;

/******************************************************************************/
/*!
  \fn ResetTransition()

  \brief Resets the transition to a closed type and sets the transition type
*/
/******************************************************************************/
	void ResetTransition(const std::string& id, GameState* state);

/******************************************************************************/
/*!
  \fn DelayTransition()

  \brief Delays the opening transition
*/
/******************************************************************************/
	bool DelayTransition(const float& frametime);

/******************************************************************************/
/*!
  \fn OpenTransition()

  \brief Does the opening transition
*/
/******************************************************************************/
	void OpenTransition(const float& frametime);

/******************************************************************************/
/*!
  \fn CloseTransition()

  \brief Does the closing transition
*/
/******************************************************************************/
	void CloseTransition(const float& frametime);

/******************************************************************************/
/*!
  \fn LoadTransition()

  \brief Given a transition name and a stream, initialize a transition scene
*/
/******************************************************************************/
	void LoadTransition(std::string name, std::stringstream& data);

/******************************************************************************/
/*!
  \fn DeSerialize()

  \brief Initializes data members of transition manager
*/
/******************************************************************************/
	void DeSerialize(const std::string& filepath);

private:

	TextureRendererType* texture_arr_;

	glm::vec2 transition_speed_;
	glm::vec2 max_size_;
	glm::vec2 current_size_;
	std::shared_ptr<GraphicsSystem> graphics_system_;
	std::shared_ptr<ComponentManager> component_manager_;
	SceneTransitions transition_map_;
	SceneTransition* current_transition_;
	GameState* next_state_;
	bool begin_, end_;
};


#endif