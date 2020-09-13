#pragma once
#ifndef _ANIMATION_MANAGER_H_
#define _ANIMATION_MANAGER_H_
#include "TextureManager.h"

enum AnimationName
{
	Player_Idle,
	Player_Walk,
	Player_Attack
};

class AnimationManager
{
	typedef GLuint Texture;
	typedef std::vector<Texture*> Animation;

	std::map<size_t, Animation> animations;
	
	TextureManager& texture_manager;

public:

	AnimationManager();
	void Init();
	void AddAnimation(AnimationName name, size_t num_frames, size_t texID);
};

#endif