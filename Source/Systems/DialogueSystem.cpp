/**********************************************************************************
*\file         DialogueSystem.cpp
*\brief        Contains definition of functions and variables used for
*			   the Dialogue System
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Systems/DialogueSystem.h"
#include "Engine/Core.h"

void DialogueSystem::Init()
{
	dialogue_manager_ = &*CORE->GetManager<DialogueManager>();
	sound_system_ = &*CORE->GetSystem<SoundSystem>();
	component_manager_ = &*CORE->GetManager<ComponentManager>();
	graphics_system_ = &*CORE->GetSystem<GraphicsSystem>();

	text_speed_ = 0.0f; //speed
	//text_speed_ = 0.02f; //normal
	textbox_max_scale_ = { 0.0f, 0.0f };
	textbox_scale_speed_ = 2000.0f;
	dialogue_status_ = DialogueStatus::INACTIVE;
	num_characters_ = 0;
}

void DialogueSystem::Update(float frametime)
{
	if (CORE->GetCorePauseStatus() && CORE->GetGamePauseStatus())
	{
		return;
	}

	if (!entire_speech_ || !dialogue_box_renderer_ || !dialogue_text_renderer_)
	{
		return;
	}

	else
	{
		switch (dialogue_status_)
		{
			case DialogueStatus::INACTIVE: {

				dialogue_box_renderer_->SetAlive(false);
				dialogue_text_renderer_->SetAlive(false);

				if (dialogue_portrait_renderer_left_)
				{
					dialogue_portrait_renderer_left_->SetAlive(false);
				}

				if (dialogue_portrait_renderer_right_)
				{
					dialogue_portrait_renderer_right_->SetAlive(false);
				}

				if (dialogue_speakername_renderer_)
				{
					dialogue_speakername_renderer_->SetAlive(false);
				}

				return;
			}

			case DialogueStatus::OPENING: {

				float new_scale = dialogue_box_scale_->GetScale().y + textbox_scale_speed_ * frametime;
				dialogue_box_scale_->SetScale({ textbox_max_scale_.x, new_scale });

				if (new_scale > textbox_max_scale_.y)
				{
					new_scale = textbox_max_scale_.y;
					dialogue_box_scale_->SetScale({ textbox_max_scale_.x, new_scale });
					dialogue_status_ = DialogueStatus::ADVANCING;
					
					if (dialogue_speakername_renderer_)
					{
						dialogue_speakername_renderer_->SetText(current_dialogue_content_->GetName());
					}

					UpdatePortraits();
				}

				break;
			}

			case DialogueStatus::ADVANCING: {

				text_elapsed_time_ += frametime;

				if (text_elapsed_time_ > text_speed_)
				{
					++num_characters_;
					text_elapsed_time_ = 0.0f;
					sound_system_->PlaySounds("TextAdvancing");
					//sound_system_->PlayTaggedSounds("TextAdvancing");

					if (num_characters_ > entire_speech_->size())
					{
						current_speech_ = *entire_speech_;
						num_characters_ = 0;
						dialogue_text_renderer_->SetText(current_speech_);
						dialogue_status_ = DialogueStatus::FINISHED_ADVANCING;
					}

					else
					{
						current_speech_ = entire_speech_->substr(0, num_characters_);
						dialogue_text_renderer_->SetText(current_speech_);
						
					}
				}

				break;
			}

			case DialogueStatus::FINISHED_ADVANCING: {

				break;
			}

			case DialogueStatus::CLOSING: {

				if (dialogue_text_renderer_)
				{
					dialogue_text_renderer_->SetText(" ");
				}

				float new_scale = dialogue_box_scale_->GetScale().y - textbox_scale_speed_ * frametime;
				dialogue_box_scale_->SetScale({ textbox_max_scale_.x, new_scale });

				if (new_scale < 0.0f)
				{
					new_scale = 0.0f;
					dialogue_box_scale_->SetScale({ textbox_max_scale_.x, new_scale });
					dialogue_status_ = DialogueStatus::INACTIVE;

					CORE->SetMovementLock();
				}

				break;
			}
		}
	}

}

void DialogueSystem::SendMessageD(Message* m)
{
	(void) m;
}

void DialogueSystem::SetCurrentDialogue(std::string dialogue_name)
{
	if (dialogue_box_renderer_ == nullptr)
	{
		CMap<TextureRenderer>* texture_renderer_arr_ = component_manager_->GetComponentArray<TextureRenderer>();

		for (auto it = texture_renderer_arr_->begin(); it != texture_renderer_arr_->end(); ++it)
		{
			if (component_manager_->GetComponent<Name>(it->first)->GetName() == "DialogueBox")
			{
				dialogue_box_renderer_ = it->second;
				dialogue_box_scale_ = component_manager_->GetComponent<Scale>(it->first);
				std::list<Entity*> children = component_manager_->GetComponent<ParentChild>(it->first)->GetChildren();
				textbox_max_scale_ = component_manager_->GetComponent<Scale>(it->first)->GetScale();

				auto text = std::find_if(std::begin(children), std::end(children),
										 [&](Entity* child) {
											 if (component_manager_->GetComponent<Name>(
												 child->GetID())->GetName() == "DialogueText") {

												 return true;
											 }

											 return false;
										 });

				dialogue_text_renderer_ = component_manager_->GetComponent<TextRenderer>((*text)->GetID());

				auto speaker_name = std::find_if(std::begin(children), std::end(children),
												 [&](Entity* child) {
													 if (component_manager_->GetComponent<Name>(
														 child->GetID())->GetName() == "DialogueSpeakerName") {

														 return true;
													 }

													 return false;
												 });

				if (speaker_name == std::end(children))
				{
					break;
				}

				dialogue_speakername_renderer_ =
					component_manager_->GetComponent<TextRenderer>((*speaker_name)->GetID());

				auto portrait = std::find_if(std::begin(children), std::end(children),
											 [&](Entity* child) {
												 if (component_manager_->GetComponent<Name>(
													child->GetID())->GetName() == "DialoguePortraitLeft") {

													 return true;
												 }

												 return false;
											 });

				dialogue_portrait_renderer_left_ =
					component_manager_->GetComponent<TextureRenderer>((*portrait)->GetID());

				portrait = std::find_if(std::begin(children), std::end(children),
										[&](Entity* child) {
											if (component_manager_->GetComponent<Name>(
												child->GetID())->GetName() == "DialoguePortraitRight") {

												return true;
											}

											return false;
										});

				dialogue_portrait_renderer_right_ =
					component_manager_->GetComponent<TextureRenderer>((*portrait)->GetID());

				graphics_system_->FlipTextureY(dialogue_portrait_renderer_right_);

				break;
			}
		}

		//if still no dialogue box renderer found
		if (!dialogue_box_renderer_) {

			return;
		}
	}

	text_elapsed_time_ = 0.0f;
	num_characters_ = 0;
	dialogue_box_renderer_->SetAlive(true);
	dialogue_text_renderer_->SetAlive(true);

	if (dialogue_speakername_renderer_)
	{
		dialogue_speakername_renderer_->SetAlive(true);
	}

	dialogue_box_scale_->SetScale({ dialogue_box_scale_->GetScale().x, 0.0f });
	current_dialogue_ = dialogue_manager_->GetDialogue(dialogue_name);

	// If dialogue is nullptr or empty
	if (!current_dialogue_ || current_dialogue_->GetContents()->begin() == current_dialogue_->GetContents()->end()) {
		entire_speech_ = nullptr;
		return;
	}

	current_dialogue_content_ = current_dialogue_->GetContents()->begin();
	entire_speech_ = current_dialogue_content_->GetSpeech();
	dialogue_status_ = DialogueStatus::OPENING;

	CORE->SetMovementLock(true);
}

void DialogueSystem::AdvanceText()
{
	if (CORE->GetCorePauseStatus() && CORE->GetGamePauseStatus())
	{
		return;
	}

	switch (dialogue_status_) {

		case DialogueStatus::ADVANCING : {

			text_elapsed_time_ = 0.0f;
			current_speech_ = *entire_speech_;
			dialogue_text_renderer_->SetText(current_speech_);
			dialogue_status_ = DialogueStatus::FINISHED_ADVANCING;

			break;
		}

		case DialogueStatus::FINISHED_ADVANCING: {

			if ((current_dialogue_content_ + 1) == current_dialogue_->GetContents()->end())
			{
				num_characters_ = 0;
				current_speech_ = {};
				dialogue_status_ = DialogueStatus::CLOSING;
			}

			else
			{
				num_characters_ = 0;
				++current_dialogue_content_;
				entire_speech_ = current_dialogue_content_->GetSpeech();

				if (dialogue_speakername_renderer_)
				{
					dialogue_speakername_renderer_->SetText(current_dialogue_content_->GetName());
				}

				UpdatePortraits();
				dialogue_status_ = DialogueStatus::ADVANCING;
			}

			break;
		}
	}
}

void DialogueSystem::TempCleanup()
{
	dialogue_box_scale_ = nullptr;
	dialogue_box_renderer_ = nullptr;
	dialogue_text_renderer_ = nullptr;

	num_characters_ = 0;
	current_speech_ = {};
	dialogue_status_ = DialogueStatus::INACTIVE;
}

DialogueSystem::DialogueStatus DialogueSystem::GetCurrentDialogueStatus() const {

	return dialogue_status_;
}

void DialogueSystem::UpdatePortraits()
{
	if (!dialogue_portrait_renderer_left_ || !dialogue_portrait_renderer_right_)
	{
		return;
	}

	if (current_dialogue_content_->GetPortraitLeft() == "None")
	{
		dialogue_portrait_renderer_left_->SetAlive(false);
	}

	else
	{
		dialogue_portrait_renderer_left_->SetAlive(true);
		graphics_system_->ChangeTexture(dialogue_portrait_renderer_left_, current_dialogue_content_->GetPortraitLeft());

		if (current_dialogue_content_->GetLeftDarken())
		{
			dialogue_portrait_renderer_left_->SetTint({0.4f, 0.4f, 0.4f});
		}

		else
		{
			dialogue_portrait_renderer_left_->SetTint({ 1.0f, 1.0f, 1.0f });
		}
	}

	if (current_dialogue_content_->GetPortraitRight() == "None")
	{
		dialogue_portrait_renderer_right_->SetAlive(false);
	}

	else
	{

		dialogue_portrait_renderer_right_->SetAlive(true);
		graphics_system_->ChangeTexture(dialogue_portrait_renderer_right_, current_dialogue_content_->GetPortraitRight());

		if (current_dialogue_content_->GetRightDarken())
		{
			dialogue_portrait_renderer_right_->SetTint({ 0.4f, 0.4f, 0.4f });
		}

		else
		{
			dialogue_portrait_renderer_right_->SetTint({ 1.0f, 1.0f, 1.0f });
		}
	}
}
