#pragma once

#ifndef _POINTLIGHT_H_
#define _POINTLIGHT_H_

#include "Entity/Entity.h"
#include "Manager/ModelManager.h"
#include "Manager/ShaderManager.h"
#include "MathLib/Vector2D.h"

class PointLight : public Component {

	Model model_;
	Shader shdr_pgm_;

	glm::vec3 color_;
	float radius_;
	float intensity_;

	glm::vec2 pos_;

public:

	PointLight();
	~PointLight();

	void Init();
	void Update(float frametime, glm::vec2 cam_pos, glm::vec2 cam_size_);
	void Draw();
	void Serialize(std::stringstream& data) override;
};

#endif