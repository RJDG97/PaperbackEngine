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

	transition_manager_->current_transition_->timer_ -= frametime;

	if (transition_manager_->current_transition_->timer_ < 0.0f) {

		transition_manager_->current_transition_->timer_ = transition_manager_->current_transition_->default_timer_;
		transition_manager_->end_ = true;
	}
}

void TransitionSystem::SendMessageD(Message* m) {
	
	(void)m;
}