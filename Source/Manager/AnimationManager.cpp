#include "Manager/AnimationManager.h"
#include "Systems/GraphicsSystem.h"
#include "Systems/Debug.h"
#include "Engine/Core.h"

Animation::Animation(std::list <Texture*> animation_frames, GLfloat frame_duration)
	: animation_frames_{ animation_frames },
	  frame_duration_{ frame_duration }
{

}

void Animation::ChangeFrameDuration(GLfloat frame_duration)
{
	frame_duration_ = frame_duration;
}

float Animation::GetFrameDuration()
{
	return frame_duration_;
}

std::list<Texture*>::iterator Animation::GetLastFrame()
{
	return animation_frames_.end();
}

std::list<Texture*>::iterator Animation::GetFirstFrame()
{
	return animation_frames_.begin();
}

void AnimationManager::Init()
{
	// Empty implementation for now
	M_DEBUG->WriteDebugMessage("Animation Manager Init\n");
}

void AnimationManager::TempFunctionForTesting()
{
	CreateAnimation("Player_Walk", 8, "Player_Walk", 0.07f);
}

void AnimationManager::CreateAnimation(std::string animation_name,
									   size_t num_frames,
									   std::string texture_name,
									   GLfloat frame_duration)
{
	std::list<Texture*> temp;

	for (int i = 0; i < num_frames; ++i)
	{
		temp.push_back(CORE->GetManager<TextureManager>()->GetTexture(texture_name + std::to_string(i)));
	}

	animations_[animation_name] = Animation{ temp, frame_duration };
}

bool AnimationManager::DeleteAnimation(std::string animation_name)
{
	if (animations_.find(animation_name) != animations_.end())
	{
		animations_.erase(animation_name);
		return true;
	}

	return false;
}

Animation* AnimationManager::GetAnimation(std::string animation_name)
{
	return &animations_[animation_name];
}

void AnimationManager::ChangeAnimationFrameDuration(std::string animation_name, GLfloat new_frame_duration)
{
	animations_[animation_name].ChangeFrameDuration(new_frame_duration);
}
