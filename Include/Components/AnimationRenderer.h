#pragma once

#ifndef _ANIMATIONRENDERER_H_
#define _ANIMATIONRENDERER_H_

#include "Entity/Entity.h"
#include "Manager/ModelManager.h"
#include "Manager/AnimationManager.h"
#include "Manager/ShaderManager.h"
#include "Components/Renderer.h"
#include <glm/glm.hpp>

class AnimationRenderer : public Renderer {

	std::map<std::string, Animation*> obj_animations_;	// all possible animations that an object can switch between
	Animation* current_animation_;						// current animation playing
	std::list<Texture*>::iterator current_frame_;		// current frame of current animation

	bool play_animation_;								// true - play animation, false - stop playing animation
	bool has_finished_animating_;						// true - animation has not finished, false - still in middle of animation

	float time_elapsed_;

public:

	friend class GraphicsSystem;

	AnimationRenderer();
	~AnimationRenderer();

	void Init();

	void Serialize(std::stringstream& data) override;
};

#endif