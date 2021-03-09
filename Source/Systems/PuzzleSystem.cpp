#include "Systems/PuzzleSystem.h"
#include "Engine/Core.h"


void PuzzleSystem::Init() {
	
	component_manager_ = &*CORE->GetManager<ComponentManager>();
	graphics_system_ = &*CORE->GetSystem<GraphicsSystem>();
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

		graphics_system_->ChangeAnimation(anim, "Stone_Fall_One");
		b_anim->SetAlive(false);

		CORE->SetMovementLock(true);
	}
	// After animation has been set once already
	else {
		if (anim && anim->FinishedAnimating()) {

			Child* child = component_manager_->GetComponent<Child>(hole_id);
			if (child) {

				EntityID puzzle_id = child->ParentID();
				Puzzle* puzzle = component_manager_->GetComponent<Puzzle>(puzzle_id);
				++puzzle->current_;
			}

			// Disable both colliders
			aabb1->SetAlive(false);
			aabb2->SetAlive(false);

			CORE->SetMovementLock(false);
			anim->SetAnimationStatus(false);
		}
	}
}

void PuzzleSystem::SendMessageD(Message* m) {
	
	(void)m;
}