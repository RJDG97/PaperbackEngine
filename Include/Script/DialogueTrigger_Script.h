/**********************************************************************************
*\file         DialogueTrigger_Script.h
*\brief        Contains definition of functions and variables used for
*			   the DialogueTrigger Script
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _DIALOGUE_TRIGGER_SCRIPT_H_
#define _DIALOGUE_TRIGGER_SCRIPT_H_

#include "Entity/Entity.h"
#include "Engine/Core.h"
#include "Manager/ComponentManager.h"
#include "Systems/DialogueSystem.h"

namespace DialogueTrigger_Script
{
	/******************************************************************************/
	/*!
	  \fn TriggerDialogue()

	  \brief Interactable update script for DialogueTrigger
	*/
	/******************************************************************************/
	void TriggerDialogue(const EntityID& dialogue_id) {

		std::shared_ptr<ComponentManager> component_mgr = CORE->GetManager<ComponentManager>();

		component_mgr->GetComponent<AABB>(dialogue_id)->SetAlive(false);

		std::shared_ptr<DialogueSystem> dialogue_sys = CORE->GetSystem<DialogueSystem>();

		dialogue_sys->SetCurrentDialogue(component_mgr->GetComponent<DialogueTrigger>(dialogue_id)->GetDialogue());
	}
}

#endif