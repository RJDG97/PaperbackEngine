#pragma once
#ifndef _PLAYER_SCRIPT_H_
#define _PLAYER_SCRIPT_H_

#include "Entity/Entity.h"
#include "Engine/Core.h"
#include "Systems/Physics.h"
#include "Manager/ComponentManager.h"
#include "Components/LogicComponent.h"

#include "Script/Stag_Script.h"
#include "Script/Mite_Script.h"


namespace Player_Scripts
{

	void UpdateChildOffset(const EntityID& parent_id) {

		ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();
		ParentChild* pc = component_mgr->GetComponent<ParentChild>(parent_id);
		Transform* parent_xform = component_mgr->GetComponent<Transform>(parent_id);

		if (!pc || !parent_xform) return;

		for (auto& child : pc->GetChildren()) {

			Transform* child_xform = component_mgr->GetComponent<Transform>(child->GetID());

			if (child_xform) {

				Vector2D updated_pos = child_xform->GetOffset() + parent_xform->GetPosition();
				child_xform->SetPosition(updated_pos);
			}
		}
	}

	void TextureUpdateScript(const EntityID& parent_id) {
		
		std::shared_ptr<GraphicsSystem> graphics = CORE->GetSystem<GraphicsSystem>();
		ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();

		AnimationRenderer* renderer = component_mgr->GetComponent<AnimationRenderer>(parent_id);
		Status* status = component_mgr->GetComponent<Status>(parent_id);
		Motion* motion = component_mgr->GetComponent<Motion>(parent_id);
		Name* name = component_mgr->GetComponent<Name>(parent_id);

		// If any pointers are invalid, return
		if (!renderer || !status || !motion || !name)
			return;

		switch (status->GetStatus())
		{
			case StatusType::BURROW:
			{
				if (VerifyZeroFloat(motion->GetVelocity().x) && VerifyZeroFloat(motion->GetVelocity().y)) {

					graphics->ChangeAnimation(renderer, "Player_Burrow_Idle");
				}
				else {

					graphics->ChangeAnimation(renderer, "Player_Burrow_Walk");
				}
				break;
			}
			case StatusType::INVISIBLE:
			{
				graphics->ChangeAnimation(renderer, "Player_Hiding");
				break;
			}
			default:
			{
				// If velocity is essentially 0, set player to idle
				if (VerifyZeroFloat(motion->GetVelocity().x) && VerifyZeroFloat(motion->GetVelocity().y)) {

					graphics->ChangeAnimation(renderer, "Player_Idle");
				}
				// If velocity isn't 0 set to walk
				else {

					graphics->ChangeAnimation(renderer, "Player_Walk");
				}
			}

		}

		if (motion->GetVelocity().x > 0 && motion->GetIsLeft()) {

			graphics->FlipTextureY(renderer);
			motion->SetIsLeft(false);
		}
		else if (motion->GetVelocity().x < 0 && !motion->GetIsLeft()) {

			graphics->FlipTextureY(renderer);
			motion->SetIsLeft(true);
		}
	}

	void PlayerControllerScript(const EntityID& id) {
		
		(void)id;
	}
}

void SetLogicFn(LogicComponent::LogicUpdate& update, std::string fn_name, std::string entity_name) {

	if (entity_name == "Player") {

		if (fn_name == "UpdateChildOffset")
			update = Player_Scripts::UpdateChildOffset;

		else if (fn_name == "UpdateTexture")
			update = Player_Scripts::TextureUpdateScript;

		//else if (fn_name == "PlayerControllerScript")
		//	update = Player_Scripts::PlayerControllerScript;
	}
	if (entity_name == "Enemy" ||
		entity_name == "Stagbeetle") {
		
		//if (fn_name == "UpdateChildOffset")
		//	update = Stag_Script::UpdateChildOffset;

		if (fn_name == "UpdateTexture")
			update = Stag_Script::TextureUpdateScript;
	}
	if (entity_name == "Mite") {
		
		//if (fn_name == "UpdateChildOffset")
		//	update = Mite_Script::UpdateChildOffset;

		if (fn_name == "UpdateTexture")
			update = Mite_Script::TextureUpdateScript;
	}
}

#endif // !_PLAYER_SCRIPT_H_