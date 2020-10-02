#pragma once

#ifndef _ANIMATIONRENDERER_H_
#define _ANIMATIONRENDERER_H_

#include "Entity/Entity.h"
#include "Manager/ModelManager.h"
#include "Manager/AnimationManager.h"
#include "Manager/ShaderManager.h"
#include <glm/glm.hpp>

class AnimationRenderer : public Component {

	Model model_;
	Shader shdr_pgm_;

	std::map<std::string, Animation*> obj_animations_;	// all possible animations that an object can switch between
	Animation* current_animation_;						// current animation playing
	std::list<Texture*>::iterator current_frame_;		// current frame of current animation

	glm::mat3 mdl_to_ndc_xform_;						// model-to-NDC transform
	glm::mat3 mdl_xform_;								// model (model-to-world)

	bool play_animation_;								// true - play animation, false - stop playing animation
	bool has_finished_animating_;						// true - animation has not finished, false - still in middle of animation

	float time_elapsed_;

public:

	AnimationRenderer();
	~AnimationRenderer();

	void Init();

	void AddAnimation(std::string animation_name);
	void SetAnimation(std::string animation_name);

	void Update(float frametime, glm::mat3 world_to_ndc_xform);
	void Draw();
	void Serialize(std::stringstream& data) override;
};

#endif