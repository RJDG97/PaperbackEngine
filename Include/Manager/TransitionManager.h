/**********************************************************************************
*\file         TransitionManager.h
*\brief        Contains declaration of functions and variables used for
*			   the Transition Manager
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


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
	bool skip_;
	bool cutscene_;
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
	  \fn ResetCurrentTransitionTimer()

	  \brief Resets the timer of the current transition
	*/
	/******************************************************************************/
	void ResetCurrentTransitionTimer();

	/******************************************************************************/
	/*!
	  \fn ResetTransition()

	  \brief Resets the transition to a closed type and sets the transition type
	*/
	/******************************************************************************/
	bool ResetTransition(const std::string& id, GameState* state, bool size_cap = false);

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
	  \fn CheckInTransition()

	  \brief Returns bool if transition is in progress
	*/
	/******************************************************************************/
	bool CheckInTransition() const;

	/******************************************************************************/
	/*!
	  \fn ResetVignetteScale()

	  \brief Reset the vignette size at the start of each level
	*/
	/******************************************************************************/
	void ResetVignetteScale();

	/******************************************************************************/
	/*!
	  \fn ResetCustom()

	  \brief Reset the vignette size at the start of each level
	*/
	/******************************************************************************/
	void ResetCustom();

	/******************************************************************************/
	/*!
	  \fn SkipTransition()

	  \brief Jumps to the end of the transition
	*/
	/******************************************************************************/
	void SkipTransition();

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
	glm::vec2 max_size_, custom_max_size_;
	glm::vec2 max_clear_size_;
	glm::vec2 current_size_;
	glm::vec2 clear_current_size_;
	std::shared_ptr<GraphicsSystem> graphics_system_;
	std::shared_ptr<ComponentManager> component_manager_;
	SceneTransitions transition_map_;
	SceneTransition* current_transition_;
	GameState* next_state_;
	bool begin_, end_, custom_, skipping_;
};


#endif