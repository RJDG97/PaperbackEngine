/**********************************************************************************
*\file         Parenting.cpp
*\brief        Contains definition of functions and variables used for
*			   the Parenting System
*
*\author	   Jun Pu, Lee, 100% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Engine/Core.h"
#include "Systems/Parenting.h"
#include "Manager/ComponentManager.h"


void ParentingSystem::Init() {
	logic_manager_ = CORE->GetManager<LogicManager>();
	entity_manager_ = CORE->GetManager<EntityManager>();
	component_manager_ = CORE->GetManager<ComponentManager>();
	child_arr_ = component_manager_->GetComponentArray<Child>();
	parent_arr_ = component_manager_->GetComponentArray<ParentChild>();
}


void ParentingSystem::Update(float frametime) {

	(void)frametime;

	if (CORE->GetCorePauseStatus())
		return;
	
	const EntityID playerid = entity_manager_->GetPlayerEntities() ? entity_manager_->GetPlayerEntities()->GetID() : 0;

	for (auto& [p_id, parent] : *parent_arr_) {

		UpdateChildOffset(p_id);
		logic_manager_->Exec("Player_UpdateHealth", playerid, p_id);
	}
}

void ParentingSystem::UpdateChildOffset(const EntityID& parent_id) {

	ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();
	ParentChild* pc = component_mgr->GetComponent<ParentChild>(parent_id);
	Transform* parent_xform = component_mgr->GetComponent<Transform>(parent_id);

	if (!pc || !parent_xform) return;

	for (auto& child : pc->GetChildren()) {

		Transform* child_xform = component_mgr->GetComponent<Transform>(child->GetID());
		Name* name = component_mgr->GetComponent<Name>(child->GetID());

		if (child_xform && name) {

			if (name->GetName() == "Waterbase" || name->GetName() == "Watershade") continue;

			Vector2D updated_pos = child_xform->GetOffset() + parent_xform->GetPosition() + parent_xform->GetOffset();
			child_xform->SetPosition(updated_pos);
		}
	}
}

void ParentingSystem::LinkParentAndChild() {
	
	if (!child_arr_ || !parent_arr_) {

		return;
	}

	for (auto& [id, child] : *child_arr_) {

		std::string childsparent = child->ParentName();

		if (childsparent == "")
			continue;
		
		for (auto& [p_id, parent] : *parent_arr_) {
			
			if (childsparent == parent->GetName()) {
			
				parent->AddChild( child->GetOwner()->GetID() );
				child->SetParentID(p_id);
				break;
			}
		}
	}
}

void ParentingSystem::AddChildToParent(const EntityID& parent_id, const EntityID& child_id) {

	ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();

	ParentChild* parent = component_mgr->GetComponent<ParentChild>(parent_id);
	Child* child = component_mgr->GetComponent<Child>(child_id);

	if (!parent || !child)
		return;

	parent->AddChild( child->GetOwner()->GetID() );
}

void ParentingSystem::RemoveChildFromParent(const EntityID& parent_id, const EntityID& child_id) {

	ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();

	ParentChild* parent = component_mgr->GetComponent<ParentChild>(parent_id);
	Child* child = component_mgr->GetComponent<Child>(child_id);

	if (!parent || !child)
		return;

	parent->RemoveChild( child->GetOwner()->GetID() );
}

void ParentingSystem::SendMessageD(Message* m) {

	(void)m;
}