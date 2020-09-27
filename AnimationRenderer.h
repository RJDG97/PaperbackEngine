#pragma once

#ifndef _ANIMATIONRENDERER_H_
#define _ANIMATIONRENDERER_H_

#include "Entity.h"
#include "ModelManager.h"
#include "AnimationManager.h"
#include "ShaderManager.h"
#include <glm/glm.hpp>

class AnimationRenderer : public Component {

	Model model_;
	Shader shdr_pgm_;

	Animation* current_animation_;					// current animation playing
	std::list<Texture*>::iterator current_frame_;	// current frame of current animation

	glm::mat3 mdl_to_ndc_xform_;					// model-to-NDC transform
	glm::mat3 mdl_xform_;							// model (model-to-world)

	glm::vec2 orientation_;							// x - angle of rotation in degrees, y - speed of rotation
	glm::vec2 scaling_;								// scaling parameters
	glm::vec2 position_;							// translation vector coordinates

	bool play_animation_;							// true - play animation, false - stop playing animation
	bool has_finished_animating;					// true - animation has not finished, false - still in middle of animation

	float time_elapsed;

public:

	AnimationRenderer();
	~AnimationRenderer();

	void Init();
	void PublishResults();
	//void Serialize(ISerializer& str);

	void SetAnimation(GLint animation_id);

	void Update(float frametime, glm::mat3 world_to_ndc_xform);
	void Draw();
	void Serialize(std::stringstream& data) override;
};

#endif