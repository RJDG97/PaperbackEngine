/**********************************************************************************
*\file         UI_Script.h
*\brief        Contains definition of functions and variables used for
*			   the UI Script
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _COLLECTIBLE_SCRIPT_H_
#define _COLLECTIBLE_SCRIPT_H_

#include "Entity/Entity.h"
#include "Engine/Core.h"
#include "Manager/ComponentManager.h"
#include "Manager/EntityManager.h"
#include "Components/LogicComponent.h"

namespace Collectible_Script
{
	///******************************************************************************/
	///*!
	//  \fn Key_Collected()

	//  \brief Collectible update script for a Key
	//*/
	///******************************************************************************/
	//void Key_Collected(const EntityID& collectible_id) {

	//	ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();

	//	// Grab relevant components
	//	AnimationRenderer* renderer = component_mgr->GetComponent<AnimationRenderer>(collectible_id);
	//	PointLight* point_light = component_mgr->GetComponent<PointLight>(collectible_id);
	//	AABB* aabb = component_mgr->GetComponent<AABB>(collectible_id);

	//	// Toggle to inactive (Potentially delete them)
	//	if (renderer)
	//		renderer->SetAlive(false);
	//	if (point_light)
	//		point_light->SetAlive(false);
	//	if (aabb)
	//		aabb->SetAlive(false);
	//}

	/******************************************************************************/
	/*!
	  \fn Puddle_Collected()

	  \brief Collectible update script for a Puddle
	*/
	/******************************************************************************/
	void CollectedCollectible(const EntityID& collectible_id) {

		ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();

		// Grab relevant components
		AnimationRenderer* renderer = component_mgr->GetComponent<AnimationRenderer>(collectible_id);
		Collectible* collectible = component_mgr->GetComponent<Collectible>(collectible_id);
		PointLight* point_light = component_mgr->GetComponent<PointLight>(collectible_id);
		AABB* aabb = component_mgr->GetComponent<AABB>(collectible_id);

		// Toggle to inactive (Potentially delete them)
		if (renderer)
			renderer->SetAlive(false);
		if (point_light)
			point_light->SetAlive(false);
		if (aabb)
			aabb->SetAlive(false);

		switch (collectible->GetItemType())
		{
			case CollectibleType::PUDDLE:
			{
				MessageBGM_Play msg{ "PlayerDrink" };
				CORE->BroadcastMessage(&msg);
				break;
			}
			case CollectibleType::KEY:
			{
				CORE->GetSystem<DialogueSystem>()->SetCurrentDialogue("keypickedup");
				break;
			}
		}
	}
}

#endif