/**********************************************************************************
*\file         TransitionSystem.cpp
*\brief        Contains definition of functions and variables used for
*			   the Transition System
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Engine/Core.h"
#include "Systems/TransitionSystem.h"


void TransitionSystem::Init() {
	
	component_manager_ = CORE->GetManager<ComponentManager>();
	transition_manager_ = CORE->GetManager<TransitionManager>();
}


void TransitionSystem::Update(float frametime) {

	if (!transition_manager_->current_transition_)
		return;

	SceneTransition* transition = transition_manager_->current_transition_;

	UNREFERENCED_PARAMETER(transition);

	// If currently changing textures, keep the screen black for a while more
	if (transition_manager_->DelayTransition(frametime))
		return;

	if (transition_manager_->begin_) {

		transition_manager_->OpenTransition(frametime);
		return;
	}

	if (transition_manager_->end_) {

		transition_manager_->CloseTransition(frametime);
		return;
	}

	if (!transition_manager_->current_transition_->cutscene_)
		transition_manager_->current_transition_->timer_ -= frametime;

	if (transition_manager_->current_transition_->timer_ < 0.0f) {

		transition_manager_->current_transition_->timer_ = transition_manager_->current_transition_->default_timer_;
		transition_manager_->end_ = true;
	}
}

void TransitionSystem::SendMessageD(Message* m) {
	
	(void)m;
}