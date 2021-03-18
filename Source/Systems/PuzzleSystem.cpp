#include "Systems/PuzzleSystem.h"
#include "Engine/Core.h"


void PuzzleSystem::Init() {
	
	component_manager_ = &*CORE->GetManager<ComponentManager>();
	graphics_system_ = &*CORE->GetSystem<GraphicsSystem>();
	stop_anim_arr_ = component_manager_->GetComponentArray<StopAnimation>();
	puzzle_arr_ = component_manager_->GetComponentArray<Puzzle>();
}

void PuzzleSystem::Update(float frametime) {

	(void)frametime;
	
	for (auto& [id, puzzle] : *puzzle_arr_) {
		
		if (puzzle->current_ == puzzle->requirement_) {
			
			// Activate whatever puzzle stuff was intended (E.g. gate open, etc)
			// By activating some logic script
		}
	}

	for (auto& [id, stop] : *stop_anim_arr_) {
		
		AnimationRenderer* anim = component_manager_->GetComponent<AnimationRenderer>(id);

		if (!anim) return;
		if (!anim->IsAlive()) return;

		if (stop->Name() == anim->GetCurrentAnimation() && anim->FinishedAnimating())
			anim->SetAnimationStatus(false);
	}
}

void PuzzleSystem::UpdatePuzzleEntities(AABB* aabb1, AABB* aabb2) {
	
	// Hole
	EntityID hole_id = aabb2->GetOwner()->GetID();
	Child* child = component_manager_->GetComponent<Child>(hole_id);
	AnimationRenderer* anim = component_manager_->GetComponent<AnimationRenderer>(hole_id);

	// Pushable
	EntityID pushable_id = aabb1->GetOwner()->GetID();
	AnimationRenderer* b_anim = component_manager_->GetComponent<AnimationRenderer>(pushable_id);

	// Initial setting of animation
	if (aabb1->GetAlive() && aabb2->GetAlive() && b_anim->IsAlive()) {

		if (anim->GetCurrentAnimation() == "Stone_Fall_One") return;

		graphics_system_->ChangeAnimation(anim, "Stone_Fall_One");
		b_anim->SetAlive(false);
		aabb1->SetAlive(false);

		//CORE->SetMovementLock(true);
	}
	// After animation has been set once already
	else {
		if (anim && anim->GetCurrentAnimation() == "Stone_Fall_One" && anim->FinishedAnimating()) {

			Child* child = component_manager_->GetComponent<Child>(hole_id);
			if (child) {

				EntityID puzzle_id = child->ParentID();
				Puzzle* puzzle = component_manager_->GetComponent<Puzzle>(puzzle_id);
				++puzzle->current_;
			}

			// Disable boulder collider
			aabb1->SetAlive(false);

			// Disable hole collider
			//aabb2->SetAlive(false);

			//CORE->SetMovementLock(false);
			anim->SetAnimationStatus(false);
		}
	}
}

void PuzzleSystem::SendMessageD(Message* m) {
	
	(void)m;
}