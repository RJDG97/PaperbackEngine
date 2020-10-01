#pragma once
#ifndef _ANIMATION_MANAGER_H_
#define _ANIMATION_MANAGER_H_
#include "Manager/TextureManager.h"
#include "Manager/IManager.h"
#include <list>

enum AnimationName
{
	Player_Idle,
	Player_Walk,
	Player_Attack
};

class Animation
{
	std::list <Texture*> animation_frames_;
	GLfloat frame_duration_;

public:

	Animation() = default;
	Animation(std::list <Texture*> animation_frames, GLfloat frame_duration);
	void ChangeFrameDuration(GLfloat frame_duration);
	float GetFrameDuration();

	std::list<Texture*>::iterator GetLastFrame();
	std::list<Texture*>::iterator GetFirstFrame();
};

class AnimationManager : public IManager
{
	std::map<size_t, Animation> animations_;
	
	TextureManager* texture_manager_;

public:

	void Init() override;
	void TempFunctionForTesting();
	void CreateAnimation(GLint animation_id, size_t num_frames, size_t texID, GLfloat frame_duration);
	bool DeleteAnimation(GLint animation_id);

	Animation* GetAnimation(GLint animation_id);

	void ChangeAnimationFrameDuration(GLint animation_id, GLfloat new_frame_duration);
};

#endif