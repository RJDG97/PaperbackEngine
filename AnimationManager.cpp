#include "AnimationManager.h"
#include "GraphicsSystem.h"

AnimationManager* ANIMATIONMANAGER;

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

AnimationManager::AnimationManager()
{
	ANIMATIONMANAGER = this;
}

void AnimationManager::Init()
{
	
}

void AnimationManager::TempFunctionForTesting()
{
	AddAnimation(AnimationName::Player_Attack, 6, TextureName::Carpet_Brick_BtmRight, 0.5f);
}

void AnimationManager::AddAnimation(GLint animation_id,
									size_t num_frames,
									size_t texID,
									GLfloat frame_duration)
{
	std::list<Texture*> temp;

	for (int i = 0; i < num_frames; ++i)
	{
		temp.push_back(TEXTUREMANAGER->GetTexture(texID + i));
	}

	animations_[animation_id] = Animation{ temp, frame_duration };
}

bool AnimationManager::DeleteAnimation(GLint animation_id)
{
	if (animations_.find(animation_id) != animations_.end())
	{
		animations_.erase(animation_id);
		return true;
	}

	return false;
}

Animation* AnimationManager::GetAnimation(GLint animation_id)
{
	return &animations_[animation_id];
}

void AnimationManager::ChangeAnimationFrameDuration(GLint animation_id, GLfloat new_frame_duration)
{
	animations_[animation_id].ChangeFrameDuration(new_frame_duration);
}
