#pragma once
#ifndef _CHILD_SCRIPT_H_
#define _CHILD_SCRIPT_H_

#include "Entity/Entity.h"
#include "Engine/Core.h"
#include "Manager/ComponentManager.h"
#include "Components/LogicComponent.h"

namespace Child_Script
{
	void UpdateChildOffset(const EntityID child_id, const EntityID parent_id) {

		ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();

		Transform* child_xform = component_mgr->GetComponent<Transform>(child_id);
		Transform* parent_xform = component_mgr->GetComponent<Transform>(parent_id);

		if (child_xform && parent_xform) {
			
			Vector2D updated_pos = child_xform->GetOffset() + parent_xform->GetPosition();
			child_xform->SetPosition(updated_pos);
		}
	}
}

void SetLogicFn(LogicComponent::LogicUpdate& update, std::string fn_name) {

	if (fn_name == "UpdateChildOffset")
		update = Child_Script::UpdateChildOffset;
}

#endif // !_UI_SCRIPT_H_