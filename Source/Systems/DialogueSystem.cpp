#include "Systems/DialogueSystem.h"
#include "Engine/Core.h"

void DialogueSystem::Init()
{
	dialogue_manager_ = &*CORE->GetManager<DialogueManager>();

	text_speed_ = 1.0f;
	textbox_scale_ = 0.0f;
	textbox_scale_speed_ = 10.0f;
	dialogue_status_ = DialogueStatus::INACTIVE;
}

void DialogueSystem::Update(float frametime)
{
	switch (dialogue_status_)
	{
		case DialogueStatus::INACTIVE :{

			return;
		}

		case DialogueStatus::OPENING: {

			textbox_scale_ += textbox_scale_speed_ * frametime;

			if (textbox_scale_ > 1.0f)
			{
				textbox_scale_ = 1.0f;
				dialogue_status_ = DialogueStatus::ADVANCING;
			}
		}

		case DialogueStatus::ADVANCING: {

			text_elapsed_time_ += frametime;

			size_t num_characters = static_cast<int>(text_elapsed_time_ / text_speed_);

			if (num_characters > entire_speech_->size())
			{
				text_elapsed_time_ = 0.0f;
				current_speech_ = *entire_speech_;
				dialogue_status_ = DialogueStatus::FINISHED_ADVANCING;
			}
			
			else
			{
				current_speech_ = entire_speech_->substr(0, num_characters);
			}
		}

		case DialogueStatus::FINISHED_ADVANCING: {


		}

		case DialogueStatus::CLOSING: {

			textbox_scale_ -= textbox_scale_speed_ * frametime;

			if (textbox_scale_ < 0.0f)
			{
				textbox_scale_ = 0.0f;
				dialogue_status_ = DialogueStatus::INACTIVE;
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
	current_dialogue_ = dialogue_manager_->GetDialogue(dialogue_name);
	current_dialogue_content_ = current_dialogue_->GetContents().begin();
	entire_speech_ = current_dialogue_content_->GetSpeech();
}

void DialogueSystem::AdvanceText()
{
	switch (dialogue_status_) {

		case DialogueStatus::ADVANCING : {

			text_elapsed_time_ = 0.0f;
			current_speech_ = *entire_speech_;
			dialogue_status_ = DialogueStatus::FINISHED_ADVANCING;
		}

		case DialogueStatus::FINISHED_ADVANCING: {

			if ((current_dialogue_content_ + 1) == current_dialogue_->GetContents().end())
			{
				current_speech_ = {};
				dialogue_status_ = DialogueStatus::CLOSING;
			}

			else
			{
				++current_dialogue_content_;
				entire_speech_ = current_dialogue_content_->GetSpeech();
				dialogue_status_ = DialogueStatus::ADVANCING;
			}
		}
	}
}