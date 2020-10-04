#pragma once
#ifndef _ANIMATION_MANAGER_H_
#define _ANIMATION_MANAGER_H_
#include "Manager/TextureManager.h"
#include "Manager/IManager.h"
#include <list>

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
	std::map<std::string, Animation> animations_;
	
	TextureManager* texture_manager_;

public:

	void Init() override;
	void TempFunctionForTesting();
	void CreateAnimation(std::string animation_name, size_t num_frames, std::string texture_name, GLfloat frame_duration);
	bool DeleteAnimation(std::string animation_name);

	Animation* GetAnimation(std::string animation_name);

	void ChangeAnimationFrameDuration(std::string animation_name, GLfloat new_frame_duration);
};

#endif